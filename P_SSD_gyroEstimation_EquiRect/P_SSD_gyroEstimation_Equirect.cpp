/*!
 \file P_SSD_gyroEstimation_Equirect.cpp
 \brief Photometric SSD for spherical camera orientation estimation (3 DOFs), exploiting PeR core, core_extended, io, features, estimation and sensor_pose_estimation modules
 \param subDiv the number of subdivision levels for the spherical image sampling
 \param imDir the directory containing the Equirectangular images to process
 \param iRef the reference image index (in the lexicographical order)
 \param i0 the first image index of the sequence to process
 \param i360 the last image index
 \param iStep the image sequence looping step
 \param Mask the image file of the mask (white pixels are to be considered whereas black pixels are not)
 \param nbTries the number of tested initial guesses for the optimization (the one leading to the lower MPP-SSD is kept)
 \param estimationType selects which estimation type to consider between 0 pure gyro, 1 incremental gyro, 2 incremental gyro with key images
 \param stabilization if 1, outputs the rotation compensated dualfisheye image
 \param ficPosesInit the text file of initial poses (one pose line per image to process)
 *
 \author Guillaume CARON, Antoine ANDRE
 \version 0.3
 \date june 2021, April 2023
 */

#include <iostream>
#include <iomanip>

#include <per/prRegularlySampledCSImage.h>

#include <per/prIntensity.h>
#include <per/prFeaturesSet.h>

#include <per/prSSDCmp.h>

#include <per/prPoseSphericalEstim.h>

#include <boost/regex.hpp>
#include <boost/filesystem.hpp>

#include <visp/vpImage.h>
#include <visp/vpImageIo.h>

#include <visp/vpTime.h>

#include <visp/vpDisplayX.h>

#define INTERPTYPE prInterpType::IMAGEPLANE_BILINEAR

// #define VERBOSE

/*!
 * \fn main()
 * \brief Main function of the MPP SSD based spherical orientation estimation
 *
 * 1. Get the parameters of the command line
 * 2. Gyro objects initialization, considering the pose estimation of a spherical camera from the feature set of photometric information compared thanks to the SSD
 * 3. Successive computation of the "desired" festures set for every image of the sequence that are used to register the request spherical image considering zero values angles initialization, the optimal angles of the previous image (the request image changes at every iteration), the optimal angles of the previous image (the resquest image changes only if the P-SSD error is greater than a threshold)
 * 4. Save the P-SSD at optimal poses, optimal poses, processing times and key images numbers to files
 *
 * \return
 *          0 if the program ended without any issue
 *         -2 if no subdivision level is provided
 *         -3 no image files directory path
 *         -4 no reference image file index
 *         -5 no initial file index
 *         -6 no last file index
 *         -7 no image step
 */
int main(int argc, char **argv)
{

    // 1. Get the parameters of the command line

    // Get the number of subdivision levels
    if (argc < 2)
    {
#ifdef VERBOSE
        std::cout << "no subdivision level" << std::endl;
#endif
        return -2;
    }
    unsigned int subdivLevel = atoi(argv[1]);

#ifdef VERBOSE
    std::cout << "Subdivision level: " << subdivLevel << std::endl;
#endif

    // Loading the reference image with respect to which the cost function will be computed
    vpImage<unsigned char> I_req;
    if (argc < 3)
    {
#ifdef VERBOSE
        std::cout << "no image files directory path given" << std::endl;
#endif
        return -3;
    }
#ifdef VERBOSE
    std::cout << "Image files directory: " << (char *)argv[2] << std::endl;
#endif

    // Get filename thanks to boost
    char myFilter[1024];
    char *chemin = (char *)argv[2];
    char ext[] = "png";

    if (argc < 4)
    {
#ifdef VERBOSE
        std::cout << "no reference image file number given" << std::endl;
#endif
        return -4;
    }
    unsigned int iRef = atoi(argv[3]);

#ifdef VERBOSE
    std::cout << "Reference image file number: " << iRef << std::endl;
#endif

    if (argc < 5)
    {
#ifdef VERBOSE
        std::cout << "no initial image file number given" << std::endl;
#endif
        return -5;
    }
    unsigned int i0 = atoi(argv[4]); // 1;//0;

#ifdef VERBOSE
    std::cout << "Initial image file number: " << i0 << std::endl;
#endif

    if (argc < 6)
    {
#ifdef VERBOSE
        std::cout << "no image files count given" << std::endl;
#endif
        return -6;
    }
    unsigned int i360 = atoi(argv[5]);

#ifdef VERBOSE
    std::cout << "images files count: " << i360 << std::endl;
#endif

    if (argc < 7)
    {
#ifdef VERBOSE
        std::cout << "no image sequence step given" << std::endl;
#endif
        return -7;
    }
    unsigned int iStep = atoi(argv[6]);

#ifdef VERBOSE
    std::cout << "Image sequence step :" << iStep << std::endl;
#endif

    sprintf(myFilter, "%06d.*\\.%s", iRef, ext);

    boost::filesystem::path dir(chemin);
    boost::regex my_filter(myFilter);
    std::string name;

    for (boost::filesystem::directory_iterator iter(dir), end; iter != end; ++iter)
    {
        name = iter->path().filename().string();
        if (boost::regex_match(name, my_filter))
        {
            std::cout << iter->path().string() << " loaded" << std::endl;
            vpImageIo::read(I_req, iter->path().string());
            break;
        }
    }

    // vpDisplayX disp;
    // disp.init(I_req_full, 25, 25, "I_req");
    // vpDisplay::display(I_req_full);
    // vpDisplay::flush(I_req_full);

    // lecture de l'image "masque"
    // Chargement du masque
    vpImage<unsigned char> Mask;
    if (argc < 8)
    {
#ifdef VERBOSE
        std::cout << "no mask image given" << std::endl;
#endif
        Mask.resize(I_req.getHeight(), I_req.getWidth(), 255);
    }
    else
    {
#ifdef VERBOSE
        std::cout << "tries to read mask file: " << argv[7] << std::endl;
#endif
        try
        {
            vpImageIo::read(Mask, argv[7]);
        }
        catch (vpException e)
        {
            std::cout << "unable to load mask file" << std::endl;
            Mask.resize(I_req.getHeight(), I_req.getWidth(), 255);
        }
    }

    // nombre de coups d'essai pour definir r_0
    unsigned int nbTries = 1;
    if (argc < 9)
    {
#ifdef VERBOSE
        std::cout << "no initial number of tries given" << std::endl;
#endif
        // return -9;
    }
    else
        nbTries = atoi(argv[8]);

#ifdef VERBOSE
    std::cout << "Number of initial tries: " << nbTries << std::endl;
#endif

    // type d'estimation (0 : gyro pur ; 1 : odometrie ; 2 : odometrie a images cles)
    unsigned int estimationType = 0;
    if (argc < 10)
    {
#ifdef VERBOSE
        std::cout << "no estimation type given" << std::endl;
#endif
        // return -9;
    }
    else
        estimationType = atoi(argv[9]);

#ifdef VERBOSE
    std::cout << "Estimation type: " << ((estimationType == 0) ? "pure gyro" : ((estimationType == 1) ? "odometry-like" : "odometry with key images")) << std::endl;
#endif

    // stabilisation de la sequence
    unsigned int stabilisation = 0;
    if (argc < 11)
    {
#ifdef VERBOSE
        std::cout << "no stabilisation parameter given" << std::endl;
#endif
        // return -9;
    }
    else
        stabilisation = atoi(argv[10]);
#ifdef VERBOSE
    std::cout << "Sequence stabilization: " << ((stabilisation == 1) ? "On" : "Off") << std::endl;
#endif

    // fichier avec les poses initiales r_0
    bool ficInit = false;
    std::vector<vpPoseVector> v_pv_init;
    if (argc < 12)
    {
#ifdef VERBOSE
        std::cout << "no initial poses file given" << std::endl;
#endif
        // return -9;
    }
    else
    {
#ifdef VERBOSE
        std::cout << "Tries to read pose file: " << argv[11] << std::endl;
#endif

        std::ifstream ficPosesInit(argv[11]);

        if (ficPosesInit.is_open())
        {
            ficInit = true;

            vpPoseVector r;
            while (!ficPosesInit.eof())
            {
                ficPosesInit >> r[0] >> r[1] >> r[2] >> r[3] >> r[4] >> r[5];
                r[0] = r[1] = r[2] = 0.; // this is to ensure ignoring the translations
                                         //		          ficPosesInit >> r[3] >> r[4] >> r[5];
                v_pv_init.push_back(r);
            }
            ficPosesInit.close();
        }
        else
        {
            ficInit = false;
#ifdef VERBOSE
            std::cout << "Pose file does not exist" << std::endl;
#endif
        }
    }
#ifdef VERBOSE
    std::cout << "end parameters list" << std::endl;
#endif
    // 2. Gyro objects initialization, considering the pose estimation of a spherical camera from the feature set of photometric Gaussian mixture 3D samples compared thanks to the SSD

    /*
    //En image plane
    prPhotometricGMS<pr2DCartesianPointVec> G_sample;
    pr2DCartesianPointVec u_g;
    G_sample.buildFrom(I_req, u_g, lambda_g);
    */

    /*unsigned */ int ehaut, elarg;
    ehaut = I_req.getHeight();
    elarg = I_req.getWidth();
    // nbPixelse = ehaut*elarg;
    prEquirectangular ecam(elarg * 0.5 / M_PI, ehaut * 0.5 / (M_PI * 0.5), elarg * 0.5, ehaut * 0.5);

    // En image spherique
    // initialisation de l'estimation d'orientation
    prPoseSphericalEstim<prFeaturesSet<prCartesian3DPointVec, prIntensity<prCartesian3DPointVec, prEquirectangular>, prRegularlySampledCSImage>, prSSDCmp<prCartesian3DPointVec, prIntensity<prCartesian3DPointVec, prEquirectangular>>> gyro;
    //    bool dofs[6] = {false, false, false, true, false, false}; //"compas"
    bool dofs[6] = {false, false, false, true, true, true}; //"gyro"

    gyro.setdof(dofs[0], dofs[1], dofs[2], dofs[3], dofs[4], dofs[5]);

    // prepare the request spherical image (here, the reference image is always considered as the request in order to compute the rotations that allow to rotate it to the current image)
    prRegularlySampledCSImage<unsigned char> IS_req(subdivLevel); // the regularly sample spherical image to be set from the acquired/loaded dual fisheye image
    IS_req.setInterpType(prInterpType::IMAGEPLANE_BILINEAR);

    IS_req.buildFromEquiRect(I_req, ecam, &Mask);     // Goulot !
    prRegularlySampledCSImage<float> GS(subdivLevel); // contient tous les pr3DCartesianPointVec XS_g et fera GS_sample.buildFrom(IS_req, XS_g);

    prFeaturesSet<prCartesian3DPointVec, prIntensity<prCartesian3DPointVec, prEquirectangular>, prRegularlySampledCSImage> fSet_req;
    prIntensity<prCartesian3DPointVec, prEquirectangular> GS_sample_req;
    GS_sample_req.setSensor(&ecam);
    // TODO : calculer en parallele un fSet_req avec lambda_g /= 10 pour les dernières itérations --> précision accrue, sans perdre de temps
    fSet_req.buildFrom(IS_req, GS, GS_sample_req);

    gyro.buildFrom(fSet_req);

    prIntensity<prCartesian3DPointVec, prEquirectangular> GS_sample;
    GS_sample.setSensor(&ecam);
    std::cout << "nb features : " << fSet_req.set.size() << std::endl;

    vpDisplayX disp2;

    // to save iterations
    std::ostringstream s;
    std::string filename;
    s.str("");
    s.setf(std::ios::right, std::ios::adjustfield);
    s << chemin << "/iter_" << iRef << "_" << i0 << "_" << i360 << ".txt";
    filename = s.str();
    gyro.startSaveIterations((char *)filename.c_str());

    // Pour chaque image du dataset
    int nbPass = 0;
    bool clickOut = false;
    unsigned int imNum = i0;
    std::vector<double> err;
    std::vector<vpPoseVector> pv;
    double temps;
    std::vector<double> v_temps;
    std::vector<unsigned int> v_keyImageNum;
    v_temps.reserve((i360 - i0) / iStep);

    vpPoseVector r, r_to_save;
    vpHomogeneousMatrix key_dMc, dMd_prec;

    bool poseJacobianCompute = true;
    // activate the M-Estimator
    bool robust = false; // true;//
    vpImage<unsigned char> I_des;

    // 3. Successive computation of the "desired" festures set for every image of the sequence that are used to register the request spherical image considering zero values angles initialization, the optimal angles of the previous image (the request image changes at every iteration), the optimal angles of the previous image (the resquest image changes only if the MPP-SSD error is greater than a threshold)
    // double angle = -177.5*M_PI/180.;
    prFeaturesSet<prCartesian3DPointVec, prIntensity<prCartesian3DPointVec, prEquirectangular>, prRegularlySampledCSImage> fSet_des;
    double seuilErr = 0.0325; // 0.015; //0.0077;// // OK pour 0,325 seul et subdiv3
    while (!clickOut && (imNum <= i360))
    {
        temps = vpTime::measureTimeMs();
        std::cout << "num request image : " << nbPass << std::endl;

        switch (estimationType)
        {
        case 0: // gyro pur
        default:
        {
            r.set(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
            break;
        }
        case 1: // odometrie
        {
            if (nbPass > 0)
            {
                key_dMc.buildFrom(r_to_save);
                fSet_req = fSet_des;
                gyro.buildFrom(fSet_req);
                r.set(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
            }
            break;
        }
        case 2: // odometrie a images cles
        {
            if ((nbPass > 0) && (err[nbPass - 1] > seuilErr))
            {
                key_dMc.buildFrom(r_to_save);
                fSet_req = fSet_des; // check si ce n'est pas encore la precedente !
                gyro.buildFrom(fSet_req);
                v_keyImageNum.push_back(nbPass - 1);
                r.set(0.0, 0.0, 0.0, 0.0, 0.0, 0.0);
            }
            break;
        }
        case 4: // gyro "sequence" (pas de reinit)
        {
            break;
        }
        }

        sprintf(myFilter, "%06d.*\\.%s", imNum, ext);

        my_filter.set_expression(myFilter);

        for (boost::filesystem::directory_iterator iter(dir), end; iter != end; ++iter)
        {
            name = iter->path().leaf().string();
            if (boost::regex_match(name, my_filter))
            {
                std::cout << iter->path().string() << " loaded" << std::endl;
                vpImageIo::read(I_des, iter->path().string());
                break;
            }
        }
        if (nbPass == 0)
            disp2.init(I_des, 500, 50, "I_des");

        vpDisplay::display(I_des);
        vpDisplay::flush(I_des);

        // Desired feature set setting from the current image
        prRegularlySampledCSImage<unsigned char> IS_des(subdivLevel);
        IS_des.setInterpType(prInterpType::IMAGEPLANE_BILINEAR);
        IS_des.buildFromEquiRect(I_des, ecam, &Mask);

        // calculer en parallele un fSet_des avec lambda_g /= 10 pour les dernières itérations --> précision accrue, sans perdre de temps
        fSet_des.buildFrom(IS_des, GS, GS_sample, poseJacobianCompute); // Goulot !
        std::cout << "nb features : " << fSet_des.set.size() << std::endl;

        // if there is a file provided as initial poses, they are used instead of other strategies
        if (ficInit)
        {
            r = v_pv_init[nbPass];
            std::cout << "r init : " << r.t() << std::endl;
        }
        else
        {
            // trying to select the best initial 3D orientation guess
            if (nbTries > 1)
            {
                vpPoseVector r_best_init;
                double err_min_init = 1e20;
                double angle[3] = {0, 0, 0}, pasAngulaire;
                vpHomogeneousMatrix dMc;
                double err0;
                pasAngulaire = 2.0 * M_PI / nbTries;

                unsigned int nbTriesPerDOF[3] = {1, 1, 1};
                if (dofs[3])
                {
                    nbTriesPerDOF[0] = nbTries;
                    angle[0] = -pasAngulaire * floor(nbTries * 0.5);
                }
                if (dofs[4])
                {
                    nbTriesPerDOF[1] = 1; // nbTries;
                    angle[1] = -pasAngulaire * floor(nbTries * 0.5);
                }
                if (dofs[5])
                {
                    nbTriesPerDOF[2] = 1; // nbTries;
                    angle[2] = -pasAngulaire * floor(nbTries * 0.5);
                }
                for (int iTry0 = 0; iTry0 < nbTriesPerDOF[0]; iTry0++, angle[0] += pasAngulaire)
                {
                    r[3] = angle[0];
                    if (dofs[4])
                        angle[1] = -pasAngulaire * floor(nbTries * 0.5);
                    else
                        angle[1] = 0;
                    for (int iTry1 = 0; iTry1 < nbTriesPerDOF[1]; iTry1++, angle[1] += pasAngulaire)
                    {
                        r[4] = angle[1];
                        if (dofs[5])
                            angle[2] = -pasAngulaire * floor(nbTries * 0.5);
                        else
                            angle[2] = 0;
                        for (int iTry2 = 0; iTry2 < nbTriesPerDOF[2]; iTry2++, angle[2] += pasAngulaire)
                        {
                            r[5] = angle[2];

                            dMc.buildFrom(r);
                            fSet_req.update(dMc);

                            prSSDCmp<prCartesian3DPointVec, prIntensity<prCartesian3DPointVec, prEquirectangular>> errorComputer(fSet_req, fSet_des, robust);
                            prIntensity<prCartesian3DPointVec, prEquirectangular> GS_error = errorComputer.getRobustCost();
                            err0 = GS_error.getGMS();

                            if (err0 < err_min_init)
                            {
                                err_min_init = err0;
                                r_best_init = r;
                            }
                        }
                    }
                }
                r = r_best_init;
            }
        }

        // register the request feature set over the desired one and save the optimal MPP-SSD
        err.push_back(gyro.track(fSet_des, r, 1.0, robust));

        v_temps.push_back(vpTime::measureTimeMs() - temps);
        std::cout << "Pass " << nbPass << " time : " << v_temps[nbPass] << " ms" << std::endl;

        dMd_prec.buildFrom(r);
        r_to_save.buildFrom(dMd_prec * key_dMc);
        pv.push_back(r_to_save);

        std::cout << "Pose optim : " << r.t() << " cum : " << r_to_save.t() << std::endl;

        std::cout << "weighted FPP-SSD : " << err[nbPass] << std::endl;

        clickOut = vpDisplay::getClick(I_req, false);

        vpImage<unsigned char> I_r(I_des.getHeight(), I_des.getWidth());
        if (stabilisation)
        {
            vpPoseVector ir;
            ir.buildFrom(vpHomogeneousMatrix(r_to_save).inverse());
            IS_des.toEquiRect(I_r, ir, ecam, &Mask);
        }
        else
        {
            IS_req.toEquiRect(I_r, r, ecam, &Mask);
        }
        s.str("");
        s.setf(std::ios::right, std::ios::adjustfield);
        s << chemin << "/rotComp/" << std::setfill('0') << std::setw(6) << imNum << ".png";
        filename = s.str();
        vpImageIo::write(I_r, filename);

        imNum += iStep;
        nbPass++;
        // angle += 2.5*M_PI/180.;
    }

    // 4. Save the MMP-SSD at optimal poses, optimal poses, processing times and key images numbers to files

    // save err list to file
    s.str("");
    s.setf(std::ios::right, std::ios::adjustfield);
    s << chemin << "/errors_" << iRef << "_" << i0 << "_" << i360 << ".txt";
    filename = s.str();
    std::ofstream ficerrMin(filename.c_str());
    std::vector<double>::iterator it_err = err.begin();
    for (; it_err != err.end(); it_err++)
    {
        ficerrMin << *it_err << std::endl;
    }
    ficerrMin.close();

    // save poses list to file
    s.str("");
    s.setf(std::ios::right, std::ios::adjustfield);
    s << chemin << "/poses_" << iRef << "_" << i0 << "_" << i360 << ".txt";
    filename = s.str();
    std::ofstream ficPoses(filename.c_str());
    std::vector<vpPoseVector>::iterator it_pv = pv.begin();
    for (; it_pv != pv.end(); it_pv++)
    {
        ficPoses << it_pv->t() << std::endl;
    }
    ficPoses.close();

    // save times list to file
    s.str("");
    s.setf(std::ios::right, std::ios::adjustfield);
    s << chemin << "/time_" << iRef << "_" << i0 << "_" << i360 << ".txt";
    filename = s.str();
    std::ofstream ficTime(filename.c_str());
    std::vector<double>::iterator it_time = v_temps.begin();
    for (; it_time != v_temps.end(); it_time++)
    {
        ficTime << *it_time << std::endl;
    }
    ficTime.close();

    // save key images numbers to file
    s.str("");
    s.setf(std::ios::right, std::ios::adjustfield);
    s << chemin << "/keys_" << iRef << "_" << i0 << "_" << i360 << ".txt";
    filename = s.str();
    std::ofstream ficKeys(filename.c_str());
    std::vector<unsigned int>::iterator it_keys = v_keyImageNum.begin();
    for (; it_keys != v_keyImageNum.end(); it_keys++)
    {
        ficKeys << *it_keys << std::endl;
    }
    ficKeys.close();

    return 0;
}
