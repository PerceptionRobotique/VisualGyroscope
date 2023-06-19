# Photometric MPP-SSD Gyro Estimation Dual Fisheye

Estimates the orientation between a given reference image (`iRef`) and a current image, using **dual fisheye images**.

Relies on the **Mixture of Photometric Potential cost** minimization between the reference and a current image whose pixels intensities are mapped to a subdivided icosahedron.

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
- `lambdaG` the Gaussian expansion parameter
- `imDir` the directory containing the **dual fisheye** images to process
- `iRef` the reference image index (in the lexicographical order)
- `i0` the first image index of the sequence to process
- `i360` the last image index
- `iStep` the image sequence looping step
- `Mask` the image file of the mask (white pixels are to be considered whereas black pixels are not)
- `nbTries` the number of tested initial guesses for the optimization (the one leading to the lower cost is kept)
- `estimationType` selects which estimation type to consider between 0 pure gyro, 1 incremental gyro, 2 incremental gyro with key images
- `stabilization` if 1, outputs the rotation compensated dualfisheye image
- `truncGauss` if 1, considers truncated Gaussian domain (+ or - 3 lambda_g at most)
- `ficPosesInit` the text file of initial poses, one pose line per image to process (no example provided)

## Associated article

ICRA 2018 [**[paper]**](https://hal.science/hal-01716939/file/CaMo_ICRA18.pdf)

> Caron, G., & Morbidi, F. (2018, May). Spherical visual gyroscope for autonomous robots using the mixture of photometric potentials. In 2018 IEEE International Conference on Robotics and Automation (ICRA) (pp. 820-827). IEEE.

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
