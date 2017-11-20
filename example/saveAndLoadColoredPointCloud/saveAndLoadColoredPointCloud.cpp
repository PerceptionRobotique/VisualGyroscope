/*!
 \file saveAndLoadColoredPointCloud.cpp
 \brief Example program of the PeR core_extended and io_extended modules
 \author Guillaume CARON
 \version 0.1
 \date april 2017
 */

#include <iostream>

#include <per/prColoredPointCloud.h>

/*!
 * \fn main()
 * \brief Main function of the sample program of the PeR core_extended and io_extended modules
 *
 * Saving a colored 3D point cloud to an XXX file and load
 *
 */
int main()
{
    prWorldModel *cPC = NULL; /*!< Generic world model */

    std::cout << "Colored 3D point cloud creation" << std::endl;
    //colored 3D point cloud definition made of 4 RGB colored 3D points
    cPC = new prColoredPointCloud(4);
    
    //setting first point and display if it was correctly added to the world model
    std::cout << ((((prColoredPointCloud *)cPC)->setPointAndColor(0, new prCartesian3DPointVec(-0.5, -0.5, -0.5, 1.0), new prRGBColor(255, 0, 0))==0)?"one colored point added":"failed to add a colored point") << std::endl;
    //setting second point and display if it was correctly added to the world model
    std::cout << ((((prColoredPointCloud *)cPC)->setPointAndColor(1, new prCartesian3DPointVec(0.5, -0.5, 0.25, 1.0), new prRGBColor(0, 255, 0))==0)?"one colored point added":"failed to add a colored point") << std::endl;
    //setting third point and display if it was correctly added to the world model
    std::cout << ((((prColoredPointCloud *)cPC)->setPointAndColor(2, new prCartesian3DPointVec(0.5, 0.5, 0.0, 1.0), new prRGBColor(0, 0, 255))==0)?"one colored point added":"failed to add a colored point") << std::endl;
    //setting fourth point and display if it was correctly added to the world model
    std::cout << ((((prColoredPointCloud *)cPC)->setPointAndColor(3, new prCartesian3DPointVec(-0.5, 0.5, 0.25, 1.0), new prRGBColor(255, 0, 255))==0)?"one colored point added":"failed to add a colored point") << std::endl;
    
    //display the world model data
    std::cout << "The colored 3D point cloud is made of " << cPC->getNumberOfElements() << " points of which coordinates and colors are : " << std::endl;
    for(unsigned long pointIndex = 0 ; pointIndex < cPC->getNumberOfElements() ; pointIndex++)
    {
        prCartesian3DPointVec *P;
        ((prColoredPointCloud *)cPC)->getPoint(pointIndex, P);
        prRGBColor *c;
        ((prColoredPointCloud *)cPC)->getColor(pointIndex, c);
        std::cout << "Point " << pointIndex << " : X = " << P->get_X() << " ; Y = " << P->get_Y() << " ; Z = " << P->get_Z() << " ; R = " << (int)(c->get_R()) << " ; G = " << (int)(c->get_G()) << " ; B = " << (int)(c->get_B()) <<  std::endl;
    }
    
    /*
    // Save the colored 3D point cloud to XXX file
    {
        prColoredPointCloudXXX toFile("myColoredPC.xxx");
        
        toFile << cPC;
    }
     */
    std::cout << "... and after saving the colored 3D point cloud to an XXX file..." << std::endl;
     
    //Forget the world model and its elements
    // Memory free
    delete cPC;
    cPC = NULL;

    //Recreate an empty colored point cloud model ?
    cPC = new prColoredPointCloud();
    
    /*
    //(Recreate) and load the world model from the XXX file
    {
        prColoredPointCloudXXX fromFile("myColoredPC.xxx");
        
        fromFile >> cPC;
    }
    */
    std::cout << "... and loading the XXX file to an empty world model..." << std::endl;
    
    // If a world model is loaded, print its elements
    if((cPC != NULL) && (cPC->getNumberOfElements() >= 1))
    {
        std::cout << "The colored 3D point cloud is made of " << cPC->getNumberOfElements() << " points of which coordinates and colors are : " << std::endl;
        for(unsigned long pointIndex = 0 ; pointIndex < cPC->getNumberOfElements() ; pointIndex++)
        {
            prCartesian3DPointVec *P;
            ((prColoredPointCloud *)cPC)->getPoint(pointIndex, P);
            prRGBColor *c;
            ((prColoredPointCloud *)cPC)->getColor(pointIndex, c);
            std::cout << "Point " << pointIndex << " : X = " << P->get_X() << " ; Y = " << P->get_Y() << " ; Z = " << P->get_Z() << " ; R = " << (int)(c->get_R()) << " ; G = " << (int)(c->get_G()) << " ; B = " << (int)(c->get_B()) <<  std::endl;
        }
    }
    else
        std::cout << "The world model is empty." << std::endl;
    
	return 0;
}
