#include <per/prColoredPointCloud.h>

prColoredPointCloud::prColoredPointCloud(unsigned int nbPoints) : prPointCloud(nbPoints)
{
    //Add a type of elements to deal with the color (one color per point)
    addElementType(nbPoints);
}

prColoredPointCloud::~prColoredPointCloud()
{
    
}

int prColoredPointCloud::setColor(unsigned int pointIndex, prRGBColor *RGB)
{
    return setElement(1, pointIndex, RGB);
}

int prColoredPointCloud::getColor(unsigned int pointIndex, prRGBColor * &RGB)
{
    int ret;
    prPhysicalElement *p;
    
    ret = getElement(1, pointIndex, p);
    if(ret != 0)
        return ret;
    
    RGB = (prRGBColor *)p;
    
    return 0;
}

int prColoredPointCloud::setPointAndColor(unsigned int pointIndex, prCartesian3DPointVec *wX, prRGBColor *RGB)
{
    int ret;
    ret = setPoint(pointIndex, wX);
    if(ret != 0)
        return ret;
    
    ret = setColor(pointIndex, RGB);
    if(ret != 0)
        return ret-2;
    
    return 0;
}
