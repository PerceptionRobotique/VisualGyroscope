/*!
 \file prPointCloud.h
 \brief Header file for the prPointCloud class
 \author Guillaume CARON
 \version 0.1
 \date february 2017
 */

#if !defined(_PRPOINTCLOUD_H)
#define _PRPOINTCLOUD_H

#include <per/prWorldModel.h>
#include <per/prCartesian3DPointVec.h>

/*!
 \class prPointCloud prPointCloud.h <per/prPointCloud.h>
 \brief Class defining a world model made of 3D points
 */
class prPointCloud : public prWorldModel
{
public:
    
    /*!
     * \fn prPointCloud(unsigned int nbPoints = 0)
     * \brief Constructor of a prPointCloud object
     * \param nbPoints the total number of points of the cloud
     */
    prPointCloud(unsigned int nbPoints = 0);
    
    /*!
     * \fn ~prPointCloud()
     * \brief Default constructor of a prPointCloud object
     */
    ~prPointCloud();
    
    /*!
     * \fn int setPoint(unsigned int pointIndex, prCartesian3DPointVec *wX)
     * \brief Sets the 3D coordinates of the point of index pointIndex
     * \param pointIndex the index of the point to set
     * \param wX the 3D coordinates
     * \return  0 if the set successes
     *         -2 if the pointIndex is out of range
     */
    int setPoint(unsigned int pointIndex, prCartesian3DPointVec *wX);

    /*!
     * \fn int getPoint(unsigned int pointIndex, prCartesian3DPointVec * &wX)
     * \brief Gets the 3D coordinates of the point stored at index pointIndex
     * \param pointIndex the index of the point to get
     * \param wX a pointer to the got point (output)
     * \return  0 if the set successes
     *         -2 if the pointIndex is out of range
     *         -3 if no point is stored at index pointIndex (wX is not modified)
     */
    int getPoint(unsigned int pointIndex, prCartesian3DPointVec * &wX);
};

#endif  //_PRPOINTCLOUD_H
