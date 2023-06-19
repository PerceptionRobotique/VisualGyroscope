# Photometric SSD Gyro Estimation Dual Fisheye

Estimates the orientation between a given reference image (`iRef`) and a current image, using **dual fisheye images**.

Relies on the **photometric cost** minimization between the reference and a current image whose pixels intensities are mapped to a subdivided icosahedron.

## Build

Be sure to install 3rd party libraries listed [here](../Readme.md)

Run the following commands:

```
mkdir build && cd build
cmake ..
make -j12
```

## Parameters

- `xmlFic` the dual fisheye camera calibration xml file
- `subDiv` the number of subdivision levels for the spherical image sampling
- `imDir` the directory containing the **dual fisheye** images to process
- `iRef` the reference image index (in the lexicographical order)
- `i0` the first image index of the sequence to process
- `i360` the last image index
- `iStep` the image sequence looping step
- `Mask` the image file of the mask (white pixels are to be considered whereas black pixels are not)
- `nbTries` the number of tested initial guesses for the optimization (the one leading to the lower cost is kept)
- `estimationType` selects which estimation type to consider between 0 pure gyro, 1 incremental gyro, 2 incremental gyro with key images
- `stabilization` if 1, outputs the rotation compensated dualfisheye image

## Associated article

OmniCV 2022 CVPR Workshop [**[paper]**](https://openaccess.thecvf.com/content/CVPR2022W/OmniCV/papers/Andre_Photometric_Visual_Gyroscope_for_Full-View_Spherical_Camera_CVPRW_2022_paper.pdf):

> André, A. N., & Caron, G. (2022). Photometric Visual Gyroscope for Full-View Spherical Camera. In Proceedings of the IEEE/CVF Conference on Computer Vision and Pattern Recognition (pp. 5232-5235).

```
This software was developed at:
MIS - UPJV
33 rue Saint-Leu
80039 AMIENS CEDEX
France

and at
CNRS - AIST JRL (Joint Robotics Laboratory)
1-1-1 Umezono, Tsukuba, Ibaraki
Japan

This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.

Description:
Insight about how to set the project and build the program
Authors:
Guillaume CARON, Antoine ANDRE

```
