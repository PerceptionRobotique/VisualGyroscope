/*!
 \file prRegularSphereData.h
 \brief Automatically generated header file for the regularly sampled sphere with various levels of detail
 \author Guillaume CARON (generated with Matlab and the SphereSampling toolbox)
 \version 0.1
 \date april 2017
 */

#if !defined(_PRREGULARSPHEREDATA_H)
#define _PRREGULARSPHEREDATA_H

#include <per/prCartesian3DPointVec.h>

namespace prData
{
    prCartesian3DPointVec samplesCoordinates0[20];
    prCartesian3DPointVec samplesCoordinates1[20];
    prCartesian3DPointVec samplesCoordinates2[20];
    prCartesian3DPointVec samplesCoordinates3[20];
    prCartesian3DPointVec samplesCoordinates4[20];
    prCartesian3DPointVec samplesCoordinates5[20];
    prCartesian3DPointVec samplesCoordinates6[20];
    prCartesian3DPointVec samplesCoordinates7[20];
    prCartesian3DPointVec samplesCoordinates8[20];
    prCartesian3DPointVec samplesCoordinates9[20];

    prCartesian3DPointVec *samplesCoordinates[10] = {samplesCoordinates0, samplesCoordinates1, samplesCoordinates2, samplesCoordinates3, samplesCoordinates4, samplesCoordinates5, samplesCoordinates6, samplesCoordinates7, samplesCoordinates8, samplesCoordinates9};
    
    unsigned long nbSphericalPts[10] = {20, 20, 20, 20, 20, 20, 20, 20, 20, 20};
}

#endif  //_PRREGULARSPHEREDATA_H
