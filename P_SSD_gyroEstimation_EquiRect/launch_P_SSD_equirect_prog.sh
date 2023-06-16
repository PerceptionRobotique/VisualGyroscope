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

# 1. Number of subdivisions of the sphere
SubdivLvl=5

# 2. Directory containing the images to treat
imDir=../../../Matrice600Pro/SixDOFs/Images_Equirectangular/

# 3. Name of the first reference image
iRef=1074

# 4. Name of the first image to take account of
i0=0
# i0=8791

# 5. Name of the last image to take account of
i360=2798
# i360=8792

# 6. The image sequence looping step
iStep=1

# 7. Image mask to avoid taking account of unused pixel, white pixels of this mask are taken in account, black are not
mask=../../../Matrice600Pro/SixDOFs/mask_hexarotor.png
# mask=./../../2021_MPP_SSD_gyroEstimation_EquiRect_media/images_full/maskFull.png

# 8. Number of tested initial guesses
nbTries=0

# 9. Selects which estimation type to consider between 0 pure gyro, 1 incremental gyro, 2 incremental gyro with key images
estimationType=0

# 10. if 1, outputs the rotation compensated dualfisheye image
stabilization=1

# 12. If initial poses file
poseFile=../../../common/2023_04_10_sky-top_MPP_1LastDoF_lg0p325_2triesForInit-poses_1074_0_2798.txt

# Building the main programm
mkdir build
cd build

echo "Building from source the program"

cmake ..
make -j8 -B

echo "Build finished, launching program"

./P_SSD_gyroEstimation_Equirect $SubdivLvl $imDir $iRef $i0 $i360 $iStep $mask $nbTries $estimationType $stabilization $poseFile