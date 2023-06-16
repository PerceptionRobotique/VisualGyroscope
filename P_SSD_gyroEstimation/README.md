```
#############################################################################
#
# This file is part of the libPR software.
# Copyright (C) 2022 by MIS lab (UPJV). All rights reserved.
#
# See http://mis.u-picardie.fr/~g-caron/fr/index.php?page=7 for more information.
#
# This software was developed at:
# MIS - UPJV
# 33 rue Saint-Leu
# 80039 AMIENS CEDEX
# France
#
# This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
# WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
#
# Description:
# Configuration file to launch an example of photometric gyroscope, based on
# images taken either from the Panoramis set (https://home.mis.u-picardie.fr/~panoramis/)
# or from the drone images set used in https://hal.archives-ouvertes.fr/hal-01716939
#
# Authors:
# Guillaume Caron, Antoine André
#
#############################################################################
```

# Photometric SSD Gyro Estimation

Short program to study the spherical orientation estimation along based on the photometric information contained in the image.

This program uses the images contained in the dataset Panoramis (see https://home.mis.u-picardie.fr/~panoramis/) and the images described in https://hal.archives-ouvertes.fr/hal-01716939.

## How to launch

Simply use the `./launch_P_SSD_prog.sh` file in a terminal. This short bash script will create a build directory, run the CMakeFile and launch the program from there.

The parameters given as an input to the main `.cpp` program can be changed directly in this launching file.

**LibPeR version to use : branch `PVS`**
