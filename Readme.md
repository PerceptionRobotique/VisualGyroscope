Visual Gyroscope

Repository containing the routines to launch the gyroscope based on [LibPeR](https://github.com/PerceptionRobotique/libPeR_base) under different use cases.

The directories are organized under three different categories:

- `P_SSD_` which refers to purely photometric based gyroscope
- `MPP_SSD_` which refers to Mixture of Photometric Potential based gyroscope
- `MPaP_SSD_` which refers to Photometric Anisotropic Gaussian mixture based gyroscope

In either case, all provided gyroscopes are based on the optimization of the cost function computed as the sum of squared differences between a reference and a desired image

The programs are able to perform the alignment both with _dual fisheye images_ or with _equirectangular images_

```
Copyright (C) 2017-2023 by MIS lab (UPJV). All rights reserved.

See http://mis.u-picardie.fr/~g-caron/fr/index.php?page=7 for more information.

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
