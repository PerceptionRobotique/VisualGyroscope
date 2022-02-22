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

#!/bin/bash

clear

echo "Variables initialization"

# 1. Camera calibration file
CalibFile=../../../Sequence3/calib_twin_fisheye.xml
# CalibFile=../../../2021_MPP_SSD_gyroEstimation_EquiRect_media/calibration/calib.xml

# 2. Number of subdivisions of the sphere
SubdivLvl=5

# 3. Directory containing the images to treat
imDir=../../../Sequence3/images/Images
# imDir=../../../2021_MPP_SSD_gyroEstimation_EquiRect_media/images_full/

# 4. Name of the first reference image
iRef=000000
# iRef=8790

# 5. Name of the first image to take account of
i0=000001
# i0=8791

# 6. Name of the last image to take account of
i360=00002
# i360=8792

# 7. The image sequence looping step
iStep=1

# 8. Image mask to avoid taking account of unused pixel, white pixels of this mask are taken in account, black are not
mask=./../../Sequence3/maskFull.png
# mask=./../../2021_MPP_SSD_gyroEstimation_EquiRect_media/images_full/maskFull.png

# 9. Number of tested initial guesses
nbTries=1

# 10. Selects which estimation type to consider between 0 pure gyro, 1 incremental gyro, 2 incremental gyro with key images
estimationType=1

# 11. if 1, outputs the rotation compensated dualfisheye image
stabilization=1

# Building the main programm
mkdir build
cd build

echo "Building from source the program"

cmake ..
make -B

echo "Build finished, launching program"

./P_SSD_gyroEstimation $CalibFile $SubdivLvl $imDir $iRef $i0 $i360 $iStep $mask $nbTries $estimationType $stabilization