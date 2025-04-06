# CMakeLists.txt: Build Script
# Copyright 2012-2025 Vincent Damewood
#
# This library is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this library. If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.18)

project(Silikego
	VERSION       0.0.0
	DESCRIPTION   "Mathematical expression parser written in C++"
	HOMEPAGE_URL  "https://vdamewood.com/software/projects/silikego"
	LANGUAGES     CXX
)

set(SILIKEGO_COPYRIGHT "Copyright 2012 - 2023 Vincent Damewood")

#########################
# Configuration Options #
#########################

include(GNUInstallDirs)

option(SILIKEGO_BUILD_SHARED_LIBRARY  "Build Silikego as a shared library (build as a static library is off)" On)
option(SILIKEGO_BUILD_CLI             "Build Silikegilo, a command-line interface for Silikego" On)
option(SILIKEGO_BUILD_GUI             "Build Silikegujo, a GUI interface for Silikego" On)
option(SILIKEGO_PACKAGE               "Add 'package' and 'package_source' build targets for packages" On)
option(SILIKEGO_PACKAGE_ZIP           "Package as zip file (ignored if SILIKEGO_PACKAGE is off)" On)
option(SILIKEGO_PACKAGE_7ZIP          "Package as 7zip file (ignored if SILIKEGO_PACKAGE is off)" On)
option(SILIKEGO_PACKAGE_TXZ           "Package as XZ-compressed tar file (ignored if SILIKEGO_PACKAGE is off)" On)

set(SILIKEGO_PACKAGE_VENDOR  "Unknown Vendor"  CACHE STRING
	"The name of the vendor when generating packages (ignored if SILIKEGO_PACKAGE is off)"
)

if(APPLE)
	option(SILIKEGO_BUILD_FRAMEWORK   "Build Silikego as a framework." On)
	option(SILIKEGO_PACKAGE_DMG       "Package as DMG file (ignored if SILIKEGO_PACKAGE is off)" On)

	set(SILIKEGO_INSTALL_APPDIR Applications  CACHE STRING
		"Locaton to install application bundles"
	)
	set(SILIKEGO_INSTALL_FRAMEWORKDIR  Frameworks  CACHE STRING
		"Locaton to install framework (ignored if SILIKEGO_BUILD_FRAMEWORK is off)"
	)
endif()

if(WIN32)
	option(SILIKEGO_PACKAGE_MSI       "Package as MSI Installer  (Ignored if SILIKEGO_PACKAGE is false)" On)

	set(SILIKEGO_MSI_GUID  "00000000-0000-0000-0000-000000000000"  CACHE STRING
		"The Upgrade GUID used for MSI Installers (Ignored if SILIKEGO_PACKAGE or SILIKEGO_PACKAGE_MSI is false)"
	)
	option(SILIKEGO_OFFICIAL_RELEASE  "Build an official release of Siolikego." Off)
	if(SILIKEGO_OFFICIAL_RELEASE)
	  set(VSV_PRERELEASE_FLAG 0)
    else()
	  set(VSV_PRERELEASE_FLAG VS_FF_PRERELEASE)
    endif()


	set(TMP_INSTALL_CMAKEDIR "cmake")
else()
	set(TMP_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/Silikego")
endif()

# We use TMP_INSTALL_CMAKEDIR here to avoid a duplicate call to set()
# so there's a single place to edit the docstring.
set(SILIKEGO_INSTALL_CMAKEDIR "${TMP_INSTALL_CMAKEDIR}" CACHE STRING
		"Locaton to install CMake files (ignored if SILIKEGO_BUILD_FRAMEWORK is on)"
)

###########
# Targets #
###########

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_subdirectory(silikego)
if(SILIKEGO_BUILD_CLI)
	add_subdirectory(silikegilo)
endif()
if(SILIKEGO_BUILD_GUI)
	if(APPLE)
		add_subdirectory(silikegujo-macos)
	elseif(UNIX)
		add_subdirectory(silikegujo-unix)
	elseif(WIN32)
		add_subdirectory(silikegujo-windows)
	else()
		message(FATAL_ERROR "A GUI program is not available for this system.")
	endif()
endif()


#############################
# Package-creation Settings #
#############################

if (SILIKEGO_PACKAGE)
	add_subdirectory(packaging)
endif()
