# Visual Gyroscope

[![LibPeR](https://img.shields.io/badge/LibPeR-0.1.1-green)](https://github.com/PerceptionRobotique/libPeR_base/releases/tag/v0.1.1)
[![ViSP](https://img.shields.io/badge/ViSP-3.5-green)](https://visp.inria.fr/)
[![OpenCV](https://img.shields.io/badge/OpenCV-4.2-green)](https://opencv.org/)

**Direct visual orientation estimation of omnidirectional images**

Based on [**[LibPeR]**](https://github.com/PerceptionRobotique/libPeR_base) `C++` library, this repository contains multiple examples of code to demonstrate various use cases of direct visual orientation estimation.

## Available methods

Summary of available methods and omnidirectional image types:

|                 | Photometric                              | Mixture of Photometric Potential           |
| --------------- | ---------------------------------------- | ------------------------------------------ |
| Dual fisheye    | [folder](P_SSD_gyroEstimation/)          | [folder](MPP_SSD_gyroEstimation/)          |
| Equirectangular | [folder](P_SSD_gyroEstimation_EquiRect/) | [folder](MPP_SSD_gyroEstimation_EquiRect/) |

### Photometric Visual Gyroscope, Dual fisheye

**OmniCV 2022 CVPR Workshop** [**[paper]**](https://openaccess.thecvf.com/content/CVPR2022W/OmniCV/papers/Andre_Photometric_Visual_Gyroscope_for_Full-View_Spherical_Camera_CVPRW_2022_paper.pdf):

> André, A. N., & Caron, G. (2022). Photometric Visual Gyroscope for Full-View Spherical Camera. In Proceedings of the IEEE/CVF Conference on Computer Vision and Pattern Recognition (pp. 5232-5235).

Based on **dual fisheye omnidirectional images**, compute the relative orientation by minimizing the photometric cost.

Example available in folder **[P_SSD_gyroEstimation/](P_SSD_gyroEstimation/)**

### Photometric Visual Gyroscope, Equirectangular

**OmniCV 2023 CVPR Workshop** [**[paper]**](https://openaccess.thecvf.com/content/CVPR2023W/OmniCV/papers/Berenguel-Baeta_Visual_Gyroscope_Combination_of_Deep_Learning_Features_and_Direct_Alignment_CVPRW_2023_paper.pdf):

> Berenguel-Baeta, B., André, A. N., Caron, G., Bermudez-Cameo, J., & Guerrero, J. J. (2023). Visual Gyroscope: Combination of Deep Learning Features and Direct Alignment for Panoramic Stabilization. In Proceedings of the IEEE/CVF Conference on Computer Vision and Pattern Recognition (pp. 6444-6447).

Based on **equirectangular omnidirectional images**, compute the relative orientation by minimizing the photometric cost.

Example available in folder **[P_SSD_gyroEstimation_EquiRect/](P_SSD_gyroEstimation_EquiRect/)**

### Mixture of Photometric Potential Visual Gyroscope, Dual fisheye

**ICRA 2018** [**[paper]**](https://hal.science/hal-01716939/file/CaMo_ICRA18.pdf)

> Caron, G., & Morbidi, F. (2018, May). Spherical visual gyroscope for autonomous robots using the mixture of photometric potentials. In 2018 IEEE International Conference on Robotics and Automation (ICRA) (pp. 820-827). IEEE.

Based on **dual fisheye omnidirectional images**, considers each vertex of the spherical mesh as a mixture of photometric potentials and minimizes the overall cost to retrieve the orientation.

Example available in folder **[MPP_SSD_gyroEstimation/](MPP_SSD_gyroEstimation/)**

### Mixture of Photometric Potential Visual Gyroscope, Equirectangular

**OmniCV 2023 CVPR Workshop** [**[paper]**](https://openaccess.thecvf.com/content/CVPR2023W/OmniCV/papers/Berenguel-Baeta_Visual_Gyroscope_Combination_of_Deep_Learning_Features_and_Direct_Alignment_CVPRW_2023_paper.pdf):

> Berenguel-Baeta, B., André, A. N., Caron, G., Bermudez-Cameo, J., & Guerrero, J. J. (2023). Visual Gyroscope: Combination of Deep Learning Features and Direct Alignment for Panoramic Stabilization. In Proceedings of the IEEE/CVF Conference on Computer Vision and Pattern Recognition (pp. 6444-6447).

Based on **equirectangular omnidirectional images**, considers each vertex of the spherical mesh as a mixture of photometric potentials and minimizes the overall cost to retrieve the orientation.

Example available in folder **[MPP_SSD_gyroEstimation_EquiRect/](MPP_SSD_gyroEstimation_EquiRect/)**

OmniCV 2023 CVPR Workshop [**[paper]**](https://openaccess.thecvf.com/content/CVPR2023W/OmniCV/papers/Berenguel-Baeta_Visual_Gyroscope_Combination_of_Deep_Learning_Features_and_Direct_Alignment_CVPRW_2023_paper.pdf):

> Berenguel-Baeta, B., André, A. N., Caron, G., Bermudez-Cameo, J., & Guerrero, J. J. (2023). Visual Gyroscope: Combination of Deep Learning Features and Direct Alignment for Panoramic Stabilization. In Proceedings of the IEEE/CVF Conference on Computer Vision and Pattern Recognition (pp. 6444-6447).

## How to use

### Build

**Install the following dependencies:**

- [LibPeR](https://github.com/PerceptionRobotique/libPeR_base) that contains the methods for visual orientation estimation of omnidirectional images (tested with 0.1.1)
- [ViSP](https://visp.inria.fr/) (tested with 3.5)
- Boost (tested with 1.71.0)
- LibXML2

**Compile the example**:

```
mkdir build && cd build
cmake ..
make -j12
```

**Run the compiled program**

To perform the visual orientation estimation, the examples rely on some common parameters. _For an exhaustive list, please read the explanations attached to each example_.

- Path to omnidirectional images. [**[link]**](https://home.mis.u-picardie.fr/~panoramis/) **to PanoraMIS** dataset used in the articles
- Number of subdivision of the icosahedron: any value in {3,4,5}
- `iRef` reference image number
- `i0` first image to measure the orientation
- `i360` last image to measure the orientation
- `iStep` step used to process the images
- `Mask` if needed to occult unwanted parts of the images to align
- `nbTries` if $>0$, perform initial guesses on an estimated orientation, before estimating the orientation using the optimization law
- `estimationType`
  - 0 for pure gyroscope
  - 1 for odometry
  - 2 for odometry with key images
- `stabilization` 1 to compensate and stabilize the output computed images or 0 to simply save output unstabilized images

Some parameters are specific to the image used or to the method:

- `lambdaG` Sets the Gaussian expansion for `MPP`-based methods
- `calibFile` provides the intrinsic parameters of the dual fisheye camera, as a `.xml` file

## Credits

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
