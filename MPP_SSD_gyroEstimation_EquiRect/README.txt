#############################################################################
#
# This file is part of the MPP_SSD_gyroEstimation software.
# Copyright (C) 2017 by MIS lab (UPJV). All rights reserved.
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
# Insight about how to set the project and build the program
#
# Authors:
# Guillaume Caron
#
#############################################################################

0. install libPeR, libboost-filesystem-dev, libboost-system-dev, libboost-regex-dev
1. create a new directory named build in MPP_SSD_gyroEstimation_EquiRect
2. use cmake to fill the build directory
3. open the project in build or use make in the latter directory to build the exe file
4. run the program from the command line at the project top directory, considering it includes the 2021_MPP_SSD_gyroEstimation_EquiRect_media directory, with arguments as:

./build/Release/MPPSSDgyroEstim_EquiRect 3 0.325 ./2021_MPP_SSD_gyroEstimation_EquiRect_media/images_subdiv3/ 8790 8790 8890 1 ./2021_MPP_SSD_gyroEstimation_EquiRect_media/images_subdiv3/maskFull.png 1 1 1 0

that are, in the reading order:
- the number of subdivision levels for the spherical image sampling
- the Gaussian expansion parameter
- the directory containing the images to process
- the reference image index (in the lexicographical order)
- the first image index of the sequence to process
- the last image index
- the image sequence looping step
- the image file of the mask (white pixels are to be considered whereas black pixels are not)
- the number of tested initial guesses for the optimization (the one leading to the lower MPP-SSD is kept)
- selects which estimation type to consider between 0 pure gyro, 1 incremental gyro, 2 incremental gyro with key images
- if 1, outputs the rotation compensated equirectangular image in jpg file
- if 1, considers truncated Gaussian domain (+ or - 3 lambda_g at most)
- ficPosesInit the text file of initial poses, one pose line per image to process (no example provided)

Sample calibration, images and mask files are provided in the "MPP_SSD_gyroEstimation_EquiRect_media" archive to be downloaded from here: http://mis.u-picardie.fr/~g-caron/data/PeR/2021_MPP_SSD_gyroEstimation_EquiRect_media.zip 

Note: validated with libPeR/devel
