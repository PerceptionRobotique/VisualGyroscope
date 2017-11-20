/*!
 \file prColoredPointCloud.h
 \brief Header file for the prColoredPointCloud class
 \author Guillaume CARON
 \version 0.1
 \date february 2017
 */

#if !defined(_PRCOLOREDPOINTCLOUD_H)
#define _PRCOLOREDPOINTCLOUD_H

#include <per/prPointCloud.h>
#include <per/prRGBColor.h>

/*!
 \class prColoredPointCloud prColoredPointCloud.h <per/prColoredPointCloud.h>
 \brief Class defining a world model made of 3D points, each having a color (assumed being a RGB color in version 0.1)
 */
class prColoredPointCloud : public prPointCloud
{
public:
    prColoredPointCloud(unsigned int nbPoints = 0);

    ~prColoredPointCloud();
    
    /*!
     * \fn int setColor(unsigned int pointIndex, prRGBColor *RGB)
     * \brief Sets the RGB color of the point of index pointIndex
     * \param pointIndex the index of the point to set
     * \param RGB the RGB color
     * \return  0 if the set successes
     *         -2 if the pointIndex is out of range
     */
    int setColor(unsigned int pointIndex, prRGBColor *RGB);
    
    /*!
     * \fn int setPointAndColor(unsigned int pointIndex, prCartesian3DPointVec *wX, prRGBColor *RGB);
     * \brief Sets both the 3D coordinates and the RGB color of the point of index pointIndex
     * \param pointIndex the index of the point to set
     * \param wX the 3D coordinates
     * \param RGB the RGB color
     * \return  0 if the set successes
     *         -2 if the pointIndex is out of range for the 3D coordinates
     *         -4 if the pointIndex is out of range for the RGB color
     */
    int setPointAndColor(unsigned int pointIndex, prCartesian3DPointVec *wX, prRGBColor *RGB);
    
    /*!
     * \fn int getColor(unsigned int pointIndex, prRGBColor * &RGB)
     * \brief Gets the RGB color of the point stored at index pointIndex
     * \param pointIndex the index of the point of which color is to get
     * \param RGB a pointer to the got color (output)
     * \return  0 if the set successes
     *         -2 if the pointIndex is out of range
     *         -3 if no point is stored at index pointIndex (RGB is not modified)
     */
    int getColor(unsigned int pointIndex, prRGBColor * &RGB);
};

#endif  //_PRCOLOREDPOINTCLOUD_H
