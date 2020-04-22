/*!
 \file PGmSSDcostFunction.cpp
 \brief Photometric Gaussian mixture based SSD Cost function computation exploiting PeR core and io modules
 * example command line :
 * ./PGmSSDcostFunction /Users/guillaume/Acquisitions/gyrovisu/spherique/gyro/SVMIS/calib/resultats/calib_subdiv3.xml 3 /Users/guillaume/Acquisitions/gyrovisu/spherique/gyro/SVMIS/OneDOF/subdiv3/ 71 0 143 /Users/guillaume/Acquisitions/gyrovisu/spherique/gyro/SVMIS/OneDOF/subdiv3/maskFull.png
 * ./PGmSSDcostFunction /Users/guillaume/Acquisitions/gyrovisu/spherique/gyro/SVMIS/calib/resultats/calib_subdiv3.xml 3 /Users/guillaume/Acquisitions/gyrovisu/spherique/gyro/SVMIS/OneDOF/subdiv3/ 215 144 287 /Users/guillaume/Acquisitions/gyrovisu/spherique/gyro/SVMIS/OneDOF/subdiv3/maskFull.png 
 * ./PGmSSDcostFunction /Users/guillaume/Acquisitions/gyrovisu/spherique/gyro/SVMIS/calib/resultats/calib_subdiv3.xml 3 /Users/guillaume/Acquisitions/gyrovisu/spherique/gyro/SVMIS/OneDOF/subdiv3/ 359 288 431 /Users/guillaume/Acquisitions/gyrovisu/spherique/gyro/SVMIS/OneDOF/subdiv3/maskFull.png
 * ./PGmSSDcostFunction /Users/guillaume/Acquisitions/gyrovisu/spherique/gyro/SVMIS/calib/resultats/calib_subdiv3.xml 3 /Users/guillaume/Acquisitions/gyrovisu/spherique/gyro/SVMIS/OneDOF/subdiv3/  503 432 575 /Users/guillaume/Acquisitions/gyrovisu/spherique/gyro/SVMIS/OneDOF/subdiv3/maskFull.png
 * ./PGmSSDcostFunction /Users/guillaume/Acquisitions/gyrovisu/spherique/gyro/SVMIS/calib/resultats/calib_subdiv3.xml 3 0.50 /Users/guillaume/Acquisitions/gyrovisu/spherique/gyro/SVMIS/OneDOF/subdiv3/  647 576 719 /Users/guillaume/Acquisitions/gyrovisu/spherique/gyro/SVMIS/OneDOF/subdiv3/maskFull.png

 *
 \author Guillaume CARON
 \version 0.1
 \date april 2017
 */

#include <iostream>

#include <per/prStereoModel.h>

#include <per/prStereoModelXML.h>
#include <per/prRegularlySampledCSImage.h>

#include <per/prPhotometricGMS.h>
#include <per/prFeaturesSet.h>
#include <per/prSSDCmp.h>

#include <boost/regex.hpp>
#include <boost/filesystem.hpp>

#include <visp/vpImage.h>
#include <visp/vpImageIo.h>

#include <visp/vpTime.h>

#include <visp/vpDisplayX.h>

#define INTERPTYPE prInterpType::IMAGEPLANE_BILINEAR
//#define INTERPTYPE prInterpType::IMAGEPLANE_NEARESTNEIGH

//#define VERBOSE

/*!
 * \fn main()
 * \brief Main function of the PGm SSD cost function program
 *
 * 1. Loading a divergent stereovision system made of two fisheye cameras considering the Barreto's model from an XML file got from the MV calibration software
 *
 *
 */
int main(int argc, char **argv)
{
    //Loading the twinfisheye intrinsic parameters
    if(argc < 2)
    {
#ifdef VERBOSE
        std::cout << "no XML stereo rig file given" << std::endl;
#endif
        return -1;
    }
    
    //Create an empty rig
    prStereoModel stereoCam(2);

    // Load the stereo rig parameters from the XML file
    {
        prStereoModelXML fromFile(argv[1]);
        
        fromFile >> stereoCam;
    }

#ifdef VERBOSE
    std::cout << "Loading the XML file to an empty rig..." << std::endl;
    
    // If a sensor is loaded, print its parameters
    if(stereoCam.get_nbsens() >= 1)
    {
        std::cout << "the stereo rig is made of a " << ((prCameraModel *)stereoCam.sen[0])->getName() << " camera of intrinsic parameters alpha_u = " << ((prCameraModel *)stereoCam.sen[0])->getau() << " ; alpha_v = " << ((prCameraModel *)stereoCam.sen[0])->getav() << " ; u_0 = " << ((prCameraModel *)stereoCam.sen[0])->getu0() << " ; v_0 = " << ((prCameraModel *)stereoCam.sen[0])->getv0();
        if(((prCameraModel *)stereoCam.sen[0])->getType() == Omni)
            std::cout << " ; xi = " << ((prOmni *)stereoCam.sen[0])->getXi();
        std::cout << "..." << std::endl;
    }

    // If a second sensor is loaded, print its parameters and its pose relatively to the first camera
    if(stereoCam.get_nbsens() >= 2)
    {
        std::cout << "... and a " << ((prCameraModel *)stereoCam.sen[1])->getName() << " camera of intrinsic parameters alpha_u = " << ((prCameraModel *)stereoCam.sen[1])->getau() << " ; alpha_v = " << ((prCameraModel *)stereoCam.sen[1])->getav() << " ; u_0 = " << ((prCameraModel *)stereoCam.sen[1])->getu0() << " ; v_0 = " << ((prCameraModel *)stereoCam.sen[1])->getv0();
        if(((prCameraModel *)stereoCam.sen[1])->getType() == Omni)
            std::cout << " ; xi = " << ((prOmni *)stereoCam.sen[1])->getXi();
        std::cout << "..." << std::endl;
   
        std::cout << "...at camera pose cpMco = " << std::endl << stereoCam.sjMr[1] << std::endl << " relative to the first camera." << std::endl;
    }
#endif

    //Get the number of subdivision levels
    if(argc < 3)
    {
#ifdef VERBOSE
        std::cout << "no subdivision level" << std::endl;
#endif
        return -1;
    }
    unsigned int subdivLevel = atoi(argv[2]);
    
    //Get the lambda_g value
    if(argc < 4)
    {
#ifdef VERBOSE
        std::cout << "no lambda_g" << std::endl;
#endif
        return -1;
    }
    float lambda_g = atof(argv[3]);//0.35;//0.035;//
    std::cout << "check lambda_g : " << lambda_g << std::endl;
    
    //Loading the reference image with respect to which the cost function will be computed
    vpImage<unsigned char> I_req;
    if(argc < 5)
    {
#ifdef VERBOSE
        std::cout << "no image files directory path given" << std::endl;
#endif
        return -1;
    }

    //Get filename thanks to boost
    char myFilter[1024];
    char *chemin = (char *)argv[4];
    char ext[] = "png";
    if(argc < 6)
    {
#ifdef VERBOSE
        std::cout << "no reference image file number given" << std::endl;
#endif
        return -1;
    }
    unsigned int iRef = atoi(argv[5]); //72
    
    if(argc < 7)
    {
#ifdef VERBOSE
        std::cout << "no initial image file number given" << std::endl;
#endif
        return -1;
    }
    unsigned int i0 = atoi(argv[6]);//1;//0;
    
    if(argc < 8)
    {
#ifdef VERBOSE
        std::cout << "no image files count given" << std::endl;
#endif
        return -1;
    }
    unsigned int i360 = atoi(argv[7]);
    
    sprintf(myFilter, "%06d.*\\.%s", iRef, ext);
    
    boost::filesystem::path dir(chemin);
    boost::regex my_filter( myFilter );
    std::string name;
    
    for (boost::filesystem::directory_iterator iter(dir),end; iter!=end; ++iter)
    {
        name = iter->path().filename().string();
        if (boost::regex_match(name, my_filter))
        {
            vpImageIo::read(I_req, iter->path().string());
            break;
        }
    }
    vpDisplayX disp;
    disp.init(I_req, 25, 25, "I_req");
    vpDisplay::display(I_req);
    vpDisplay::flush(I_req);
    
    //lecture de l'image "masque"
    //Chargement du masque
    vpImage<unsigned char> Mask;
    if(argc < 9)
    {
#ifdef VERBOSE
        std::cout << "no mask image given" << std::endl;
#endif
        Mask.resize(I_req.getHeight(), I_req.getWidth(), 255);
    }
    else
    {
        try
        {
            std::cout << argv[8] << std::endl;
            vpImageIo::read(Mask, argv[7]);
        }
        catch(vpException e)
        {
            Mask.resize(I_req.getHeight(), I_req.getWidth(), 255);
        }
    }
    
    /*
    //En image plane
    prPhotometricGMS<pr2DCartesianPointVec> G_sample;
    pr2DCartesianPointVec u_g;
    G_sample.buildFrom(I_req, u_g, lambda_g);
    */
    
    //En image spherique
    prRegularlySampledCSImage<unsigned char> IS_req(subdivLevel);
    IS_req.setInterpType(INTERPTYPE);
    IS_req.buildFromTwinOmni(I_req, stereoCam, &Mask); // Goulot !
    IS_req.toAbsZN();
    prRegularlySampledCSImage<float> GS(subdivLevel); //contient tous les pr3DCartesianPointVec XS_g et fera GS_sample.buildFrom(IS_req, XS_g);

    prFeaturesSet<prPhotometricGMS<prCartesian3DPointVec> > fSet_req;

    prPhotometricGMS<prCartesian3DPointVec> GS_sample_req(lambda_g);
    fSet_req.buildFrom(IS_req, GS, GS_sample_req);

    //GS_sample.setLambda(lambda_g);
    
    std::cout << "nb features : " << fSet_req.set.size() << std::endl;

    vpDisplayX disp2;
    
    //Pour chaque image du dataset
    int nbPass = 0;
    bool clickOut = false;
    unsigned int imNum = i0;
    std::vector<double> err;
    double temps;

    bool robust = true;//false;//
    
    while(!clickOut && (imNum <= i360))
    {
        temps = vpTime::measureTimeMs();
        vpImage<unsigned char> I_des;
        std::cout << "num request image : " << nbPass << std::endl;
        
        sprintf(myFilter, "%06d.*\\.%s", imNum, ext);
        
        my_filter.set_expression(myFilter);
        
        for (boost::filesystem::directory_iterator iter(dir),end; iter!=end; ++iter)
        {
            name = iter->path().leaf().string();
            if (boost::regex_match(name, my_filter))
            {
                vpImageIo::read(I_des, iter->path().string());
                break;
            }
        }
        if(nbPass == 0)
            disp2.init(I_des, 500, 50, "I_des");

        vpDisplay::display(I_des);
        vpDisplay::flush(I_des);
        
        prRegularlySampledCSImage<unsigned char> IS_des(subdivLevel);
        IS_des.setInterpType(INTERPTYPE);
        IS_des.buildFromTwinOmni(I_des, stereoCam, &Mask);
        IS_des.toAbsZN();

        prFeaturesSet<prPhotometricGMS<prCartesian3DPointVec> > fSet_des;
        prPhotometricGMS<prCartesian3DPointVec> GS_sample(lambda_g);
        fSet_des.buildFrom(IS_des, GS, GS_sample); // Goulot !

        std::cout << "nb features : " << fSet_des.set.size() << std::endl;
        
        prSSDCmp<prPhotometricGMS<prCartesian3DPointVec> > errorComputer(fSet_req, fSet_des, robust);
        prPhotometricGMS<prCartesian3DPointVec> GS_error = errorComputer.getRobustCost();
        //err.push_back(sqrt(GS_error.getGMS()));
        err.push_back(GS_error.getGMS());
        
        std::cout << "weighted FPP-SSD : " << err[nbPass] << std::endl;

        clickOut=vpDisplay::getClick(I_req,false);

        std::cout << "Pass " << nbPass << " time : " << vpTime::measureTimeMs()-temps << " ms" << std::endl;
 
        imNum++;
        nbPass++;

    }
    
    std::ostringstream s;
    std::string filename;
    //save err list to file
    s.str("");
    s.setf(std::ios::right, std::ios::adjustfield);
    s << chemin << "/errors_lg" << argv[3] << ".txt";
    filename = s.str();
    std::ofstream ficerrMin(filename.c_str());
    std::vector<double>::iterator it_err = err.begin();
    for(;it_err != err.end() ; it_err++)
    {
        ficerrMin << *it_err << std::endl;
    }
    ficerrMin.close();
    
	return 0;
}
