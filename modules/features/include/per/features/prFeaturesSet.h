/*!
 \file prFeatureSet.h
 \brief Definition of the base class of feature sets
 \author Guillaume CARON
 \version 0.1
 \date august 2017
 */

#if !defined(_PRFEATURESSET_H)
#define _PRFEATURESSET_H

#include <per/prFeature.h>
#include <per/prCartesian3DPointVec.h>
#include <per/prPhotometricGMS.h>

#include <per/prRegularlySampledCSImage.h>

#include <vector>

/*!
 \class prFeatureSet prFeatureSet.h <per/prFeatureSet.h>
 \brief Base class for a computer sensing feature set
 */
template<typename T>
class prFeaturesSet
{
public:
    /*!
     * \fn prFeaturesSet()
     * \brief Default constructor of a prFeaturesSet object
     */
    prFeaturesSet()
    {
        
    }
    
    /*!
     * \fn ~prFeatureSet()
     * \brief Default destructor of a prFeatureSet object
     */
    ~prFeaturesSet()
    {
        
    }
    
    /*!
     * \fn int computeFeaturePoseJacobian(vpMatrix & L, unsigned int nbDOF, bool *dof)
     * \brief Computes the feature set pose Jacobian (d gm / d r)
     * \param L (output) the output feature set Jacobian with respect to the pose
     * \param nbDOF number of degrees of freedom to consider (between 1 and 6)
     * \param dof the boolean array of 6 elements to know which of the nbDOF activated DOFs are activated
     * \return  0 if the pose Jacobian might be computed for the entire feature set
     *         -1 if any feature pose Jacobian could not be obtained
     */
    int computeFeaturePoseJacobian(vpMatrix & L, unsigned int nbDOF, bool *dof)
    {
        unsigned int numDOF;
        unsigned long nbSamples = sampler.getNbSamples();
        vpRowVector Ls;
        double *pt_L, *pt_Ls;
        L.resize(nbSamples, nbDOF);
        
        typename std::vector<T>::iterator it_set = set.begin();
        for(unsigned long ns = 0 ; ns < nbSamples ; ns++, it_set++)
        {
            if(it_set->getPoseJacobian(Ls) == -1)
            {
                std::cout << ns << " : pose jacobian not initialized" << std::endl;
                return -1;
            }
            
            pt_L = L[ns]; // get pointer on line ns of L
            pt_Ls = Ls.data;
            for (numDOF = 0 ; numDOF < 6 ; numDOF++, pt_Ls++)
                if(dof[numDOF])
                {
                    *pt_L = *pt_Ls;
                    pt_L++;
                }
        }
        
        return 0;
    }
    
    /*!
     * \fn int update(vpHomogeneousMatrix & dMc, bool computePoseDerivatives = false)
     * \brief Updates each feature of the feature set based on the provided frame change to be applied on the sampling "grid"
     * \param dMc the frame change to apply
     * \param computePoseDerivatives boolean to set if the pose Jacobian must be recomputed (computePoseDerivatives == true) or not (computePoseDerivatives == false)
     * \return  0 if the update succeeds
     */
    int update(vpHomogeneousMatrix & dMc, bool computePoseDerivatives = false)
    {
        return buildFromTransformed(computePoseDerivatives, false, true, &dMc);
    }

    /*!
     * \fn int buildFromTransformed(bool computePoseDerivatives = false, bool updateSampler = false, bool reinit = false, vpHomogeneousMatrix *dMc = NULL)
     * \brief Builds the feature set, optionnaly considering a frame change of the sampling "grid" (the sampler can be modified iff dMc is pointing to a valid homogeneous matrix)
     * \param computePoseDerivatives boolean to set if the pose Jacobian must be recomputed (computePoseDerivatives == true) or not (computePoseDerivatives == false)
     * \param updateSampler boolean to set if the sampling grid must updated by the frame change (updateSampler == true) or not (updateSampler& == false)
     * \param reinit boolean to set if the feature set must be cleared (reinit == true) or not (reinit == false) before (re)building it
     * \param dMc a pointer to the frame change to apply (can be NULL if no transformation is to apply)
     * \return  0 if the update succeeds
     */
    int buildFromTransformed(bool computePoseDerivatives = false, bool updateSampler = false, bool reinit = false, vpHomogeneousMatrix *dMc = NULL)
    {
        unsigned long nbSamples = sampler.getNbSamples();
        
        if(dMc == NULL)
        {
        if(reinit || (set.size() != nbSamples) )
        {
            set.clear();
            set.reserve(nbSamples);
            
            float *pt_bitmap = sampler.bitmap;
            prCartesian3DPointVec *pt_XS = sampler.XS;
            
            if(updateSampler)
            {
                for(unsigned long ns = 0 ; ns < nbSamples ; ns++, pt_XS++, pt_bitmap++)
                {
                    GS_req_sample.buildFrom(input, *pt_XS, computePoseDerivatives);
                    set.push_back(GS_req_sample);
                    *pt_bitmap = GS_req_sample.getGMS();
                }
            }
            else
            {
                for(unsigned long ns = 0 ; ns < nbSamples ; ns++, pt_XS++)
                {
                    GS_req_sample.buildFrom(input, *pt_XS, computePoseDerivatives);
                    set.push_back(GS_req_sample);
                }
            }
        }
        else
        {
            float *pt_bitmap = sampler.bitmap;
            prCartesian3DPointVec *pt_XS = sampler.XS;
            typename std::vector<T>::iterator it_set = set.begin();
            
            if(updateSampler)
            {
                for(unsigned long ns = 0 ; ns < nbSamples ; ns++, pt_XS++, pt_bitmap++, it_set++)
                {
                    GS_req_sample.buildFrom(input, *pt_XS, computePoseDerivatives);
                    //set.push_back(GS_req_sample);
                    *it_set = GS_req_sample;
                    *pt_bitmap = GS_req_sample.getGMS();
                }
            }
            else
            {
                for(unsigned long ns = 0 ; ns < nbSamples ; ns++, pt_XS++, it_set++)
                {
                    GS_req_sample.buildFrom(input, *pt_XS, computePoseDerivatives);
                    //set.push_back(GS_req_sample);
                    *it_set = GS_req_sample;
                    
                }
            }
        }
        }
        else
        {
            if(reinit || (set.size() != nbSamples) )
            {
                set.clear();
                set.reserve(nbSamples);
                
                float *pt_bitmap = sampler.bitmap;
                prCartesian3DPointVec *pt_XS = sampler.XS;
                
                if(updateSampler)
                {
                    for(unsigned long ns = 0 ; ns < nbSamples ; ns++, pt_XS++, pt_bitmap++)
                    {
                        *pt_XS = pt_XS->changeFrame(*dMc); //update sampler
                        
                        GS_req_sample.buildFrom(input, *pt_XS, computePoseDerivatives);
                        set.push_back(GS_req_sample);
                        
                        *pt_bitmap = GS_req_sample.getGMS(); //update sampler
                    }
                }
                else
                {
                    prCartesian3DPointVec XS;
                    
                    for(unsigned long ns = 0 ; ns < nbSamples ; ns++, pt_XS++)
                    {
                        XS = pt_XS->changeFrame(*dMc);
                        GS_req_sample.buildFrom(input, XS, computePoseDerivatives);
                        set.push_back(GS_req_sample);
                    }
                }
            }
            else
            {
                float *pt_bitmap = sampler.bitmap;
                prCartesian3DPointVec *pt_XS = sampler.XS;
                typename std::vector<T>::iterator it_set = set.begin();
                
                if(updateSampler)
                {
                    for(unsigned long ns = 0 ; ns < nbSamples ; ns++, pt_XS++, pt_bitmap++, it_set++)
                    {
                        *pt_XS = pt_XS->changeFrame(*dMc); //update sampler
                        
                        GS_req_sample.buildFrom(input, *pt_XS, computePoseDerivatives);
                        //set.push_back(GS_req_sample);
                        *it_set = GS_req_sample;
                        
                        *pt_bitmap = GS_req_sample.getGMS(); //update sampler
                    }
                }
                else
                {
                    prCartesian3DPointVec XS;
                    
                    for(unsigned long ns = 0 ; ns < nbSamples ; ns++, pt_XS++, it_set++)
                    {
                        XS = pt_XS->changeFrame(*dMc);
                        GS_req_sample.buildFrom(input, XS, computePoseDerivatives);
                        //set.push_back(GS_req_sample);
                        *it_set = GS_req_sample;                        
                    }
                }
            }
        }

        return 0;
    }
    
    /*!
     * \fn int buildFrom(prRegularlySampledCSImage<unsigned char> & input_, prRegularlySampledCSImage<float> & sampler_, prPhotometricGMS<prCartesian3DPointVec> & GS_req_sample_, bool computePoseDerivatives = false, bool updateSampler = false, bool reinit = false)
     * \brief Builds the feature set from the input data and considering the provided sampler ("sampling grid") and the feature type
     * \param input_ the input data from which to compute the feature set
     * \param sampler_ the sempling "grid" with which to computer the feature set
     * \param GS_req_sample_ the feature type that will populate the feature set
     * \param computePoseDerivatives boolean to set if the pose Jacobian must be recomputed (computePoseDerivatives == true) or not (computePoseDerivatives == false)
     * \param updateSampler boolean to set if the sampling grid must updated by the frame change (updateSampler == true) or not (updateSampler& == false)
     * \param reinit boolean to set if the feature set must be cleared (reinit == true) or not (reinit == false) before (re)building it
     * \return  0 if the update succeeds
     */
    int buildFrom(prRegularlySampledCSImage<unsigned char> & input_, prRegularlySampledCSImage<float> & sampler_, prPhotometricGMS<prCartesian3DPointVec> & GS_req_sample_, bool computePoseDerivatives = false, bool updateSampler = false, bool reinit = false)
    {
        input = input_;
        sampler = sampler_;
        GS_req_sample = GS_req_sample_;
        
        return buildFromTransformed(computePoseDerivatives, updateSampler, reinit);
    }
    
    /*!
     * \fn prFeaturesSet<T>& operator=(const prFeaturesSet<T>& FSet)
     * \brief = operator override to copy prFeaturesSet object
     * \param FSet the feature set to copy to the current
     * \return the prFeaturesSet<T> self reference object
     */
    prFeaturesSet<T>& operator=(const prFeaturesSet<T>& FSet)
    {
        set = FSet.set;
        
        input = FSet.input;
        sampler = FSet.sampler;
        GS_req_sample = FSet.GS_req_sample;
        
        return *this;
    }
    
//private:
    std::vector<T> set; /*!< the data structure containing the set of features */
    
    prRegularlySampledCSImage<unsigned char> input;  /*!< the input data from which to compute the features */
    prRegularlySampledCSImage<float> sampler;  /*!< the sampling grid with which to sample the input data */
    prPhotometricGMS<prCartesian3DPointVec> GS_req_sample;  /*!< the feature type that will fill the features set */
};

#endif  //_PRFEATURESSET_H
