# CMakeLists.txt: Build Script
# Copyright 2012-2023 Vincent Damewood
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program. If not, see <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.18)

project(SilikegujoForMacOS
	VERSION       0.0.0
	DESCRIPTION   "GUI frontend for Silikego for macOS"
	HOMEPAGE_URL  "https://vdamewood.com/software/projects/silikego"
	LANGUAGES     OBJCXX
)
set(PRERELEASE_SUFFIX "indev")

include(GNUInstallDirs)

set(SILIKEGUJO_BUNDLE_COPYRIGHT "Copyright 2012 - 2024 Vincent Damewood")
set(SILIKEGUJO_INSTALL_APPDIR Applications  CACHE STRING
	"Locaton to install application bundle"
)

if(NOT TARGET Silikego::SilikegoCore)
	find_package(SilikegoCore REQUIRED)
endif()

find_program(IBTOOL ibtool)
if (NOT IBTOOL)
	message(FATAL_ERROR "Command ibtool not found. Can't build GUI. Quitting.")
endif()

add_custom_command(OUTPUT MainMenu.nib
	COMMAND "${IBTOOL}" --compile MainMenu.nib "${CMAKE_CURRENT_SOURCE_DIR}/src/MainMenu.xib"
	COMMENT "Compiling ${CMAKE_CURRENT_SOURCE_DIR}/src/MainMenu.xib"
)
set_source_files_properties(MainMenu.nib PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
set_source_files_properties(src/silikegujo.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

add_executable(SilikegujoForMacOS MACOSX_BUNDLE)
target_sources(SilikegujoForMacOS
	PRIVATE
		src/Delegate.h
		src/Delegate.mm
		src/Main.mm
		MainMenu.nib
		src/silikegujo.icns
)
set_target_properties(SilikegujoForMacOS PROPERTIES
	OUTPUT_NAME                         Silikujo
	MACOSX_BUNDLE_GUI_IDENTIFIER        "com.vdamewood.SilikegujoForMacOS"
	MACOSX_BUNDLE_BUNDLE_NAME           "Silikegujo"
	MACOSX_BUNDLE_SHORT_VERSION_STRING  ${PROJECT_VERSION}
	MACOSX_BUNDLE_BUNDLE_VERSION        ${PROJECT_VERSION}
	MACOSX_BUNDLE_COPYRIGHT             "${SILIKEGUJO_BUNDLE_COPYRIGHT}"
	MACOSX_BUNDLE_ICON_FILE             silikegujo.icns
	MACOSX_BUNDLE_INFO_PLIST            "${CMAKE_CURRENT_SOURCE_DIR}/src/Info.plist.in"
)
target_compile_options(SilikegujoForMacOS
	PRIVATE
		"-fobjc-arc"
)
target_compile_features(SilikegujoForMacOS PRIVATE cxx_std_17)
target_link_libraries(SilikegujoForMacOS
	PRIVATE
		Silikego::SilikegoCore
		"-framework Cocoa"
)
silikego_core_fixup_bundle(SilikegujoForMacOS)

install(TARGETS SilikegujoForMacOS
BUNDLE
	DESTINATION  ${SILIKEGUJO_INSTALL_APPDIR}
	COMPONENT    Silikegujo
)
