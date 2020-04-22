#############################################################################
#
# This file is part of the libPR software.
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
# libPR overall configuration file. Freely inspired from the CMakeList tree of the 
# ViSP library. 
#
# Authors:
# Guillaume Caron
#
#############################################################################

project(PGm_SSD_costFunction)

cmake_minimum_required(VERSION 2.6)

find_package(PER REQUIRED per_core per_core_extended per_io per_features per_estimation)
if(PER_FOUND)
	include(${PER_USE_FILE})
endif(PER_FOUND)

# Boost
FIND_PACKAGE(Boost REQUIRED)

link_directories(/opt/local/lib/ /Users/guillaume/Developpement/librairies/visp-3.0.0/build/lib/Release/ ${Boost_LIBRARY_DIRS})

set(PGmSSDcostFunction_cpp
  PGmSSDcostFunction.cpp
)

foreach(cpp ${PGmSSDcostFunction_cpp})
  per_add_target(${cpp})
  if(COMMAND pr_add_dependency)
    pr_add_dependency(${cpp} "PGmSSDcostFunction")
  endif()
endforeach()

target_link_libraries(PGmSSDcostFunction libboost_system-mt.dylib libboost_filesystem-mt.dylib libboost_regex-mt.dylib)