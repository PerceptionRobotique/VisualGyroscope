#include <per/prPointCloud.h>

prPointCloud::prPointCloud(unsigned int nbPoints) : prWorldModel(1, nbPoints)
{

}

prPointCloud::~prPointCloud()
{

}

int prPointCloud::setPoint(unsigned int pointIndex, prCartesian3DPointVec *wX)
{
    return setElement(0, pointIndex, wX);
}

int prPointCloud::getPoint(unsigned int pointIndex, prCartesian3DPointVec * &wX)
{
    int ret;
    prPhysicalElement *p;
    
    ret = getElement(0, pointIndex, p);
    if(ret != 0)
        return ret;
    
    wX = (prCartesian3DPointVec *)p;
    
    return 0;
}
