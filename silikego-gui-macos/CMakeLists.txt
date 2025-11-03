# CMakeLists.txt: Build Script
# Copyright 2012-2025 Vincent Damewood
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

project(SilikegoGuiMacOS
	VERSION       0.0.0
	DESCRIPTION   "GUI frontend for Silikego for macOS"
	HOMEPAGE_URL  "https://vdamewood.com/software/projects/silikego"
	LANGUAGES     OBJCXX
)
set(PRERELEASE_SUFFIX "indev")

include(GNUInstallDirs)

set(SILIKEGO_GUI_MACOS_BUNDLE_COPYRIGHT "Copyright 2012 - 2025 Vincent Damewood")
set(SILIKEGO_GUI_MACOS_INSTALL_APPDIR Applications  CACHE STRING
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
add_custom_command(OUTPUT
	Assets.car
	SilikegoIcon.icns
COMMAND ACTOOL
	--platform macosx
	--minimum-deployment-target 14.0
	--output-partial-info-plist /dev/null
	--compile ${CMAKE_CURRENT_BINARY_DIR}
	--app-icon SilikegoIcon
	${CMAKE_CURRENT_SOURCE_DIR}/src/SilikegoIcon.icon
COMMENT "Compiling application icon"
)
set_source_files_properties(Assets.car PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
set_source_files_properties(SilikegoIcon.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

add_executable(SilikegoGuiMacOS MACOSX_BUNDLE)
target_sources(SilikegoGuiMacOS
	PRIVATE
		src/Delegate.h
		src/Delegate.mm
		src/Main.mm
		MainMenu.nib
		Assets.car
		SilikegoIcon.icns
)
set_target_properties(SilikegoGuiMacOS PROPERTIES
	OUTPUT_NAME                         "Silikego GUI"
	MACOSX_BUNDLE_GUI_IDENTIFIER        "com.vdamewood.SilikegoGuiMacOS"
	MACOSX_BUNDLE_BUNDLE_NAME           "Silikego GUI"
	MACOSX_BUNDLE_SHORT_VERSION_STRING  ${PROJECT_VERSION}
	MACOSX_BUNDLE_BUNDLE_VERSION        ${PROJECT_VERSION}
	MACOSX_BUNDLE_COPYRIGHT             "${SILIKEGO_GUI_BUNDLE_COPYRIGHT}"
	MACOSX_BUNDLE_ICON_FILE             SilikegoIcon
	MACOSX_BUNDLE_INFO_PLIST            "${CMAKE_CURRENT_SOURCE_DIR}/src/Info.plist.in"
)
target_compile_options(SilikegoGuiMacOS
	PRIVATE
		"-fobjc-arc"
)
target_compile_features(SilikegoGuiMacOS PRIVATE cxx_std_17)
target_link_libraries(SilikegoGuiMacOS
	PRIVATE
		Silikego::SilikegoCore
		"-framework Cocoa"
)
silikego_core_fixup_bundle(SilikegoGuiMacOS)

install(TARGETS SilikegoGuiMacOS
BUNDLE
	DESTINATION  ${SILIKEGO_GUI_MACOS_INSTALL_APPDIR}
	COMPONENT    SilikegoGuiMacOS
)
