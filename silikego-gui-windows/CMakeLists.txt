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

project(SilikegoGuiWindows
	VERSION       0.0.0
	DESCRIPTION   "GUI frontend for Silikego for Windows"
	HOMEPAGE_URL  "https://vdamewood.com/software/projects/silikego"
	LANGUAGES     CXX
)
set(PRERELEASE_SUFFIX "indev")

include(GNUInstallDirs)

if(NOT TARGET Silikego::SilikegoCore)
	find_package(SilikegoCore REQUIRED)
endif()

add_executable(SilikegoGuiWindows WIN32)
target_sources(SilikegoGuiWindows
	PRIVATE
		src/EvalWindow.h
		src/About.h
		src/Id.h
		src/WinMain.cc
		src/EvalWindow.cc
		src/EvalWindow.rc
		src/About.cc
		src/About.rc
		src/Icon.rc
)
set_target_properties(SilikegoGuiWindows PROPERTIES
	OUTPUT_NAME               silikego-gui-windows
)
if(PRERELEASE_SUFFIX STREQUAL "")
	set(VSV_PRERELEASE_FLAG 0)
else()
	set(VSV_PRERELEASE_FLAG VS_FF_PRERELEASE)
endif()
set(VSV_DEBUG_FLAG 0)
set(VSV_FILENAME "silikego-gui-windows.exe")
configure_file(
	src/Version.rc.in
	${CMAKE_CURRENT_BINARY_DIR}/Version.Release.rc
	@ONLY
)
target_sources(SilikegoGuiWindows PRIVATE $<$<CONFIG:Release>:${CMAKE_CURRENT_BINARY_DIR}/Version.Release.rc>)

set(VSV_DEBUG_FLAG VS_FF_DEBUG)
set(VSV_FILENAME "silikego-gui-windows.exe")
configure_file(
	src/Version.rc.in
	${CMAKE_CURRENT_BINARY_DIR}/Version.Debug.rc
	@ONLY
)
target_sources(SilikegoGuiWindows PRIVATE $<$<CONFIG:Debug>:${CMAKE_CURRENT_BINARY_DIR}/Version.Debug.rc>)
target_compile_features(SilikegoGuiWindows PRIVATE cxx_std_17)
target_link_libraries(SilikegoGuiWindows PRIVATE Silikego::SilikegoCore)

install(TARGETS SilikegoGuiWindows
RUNTIME
	DESTINATION  ${CMAKE_INSTALL_BINDIR}
	COMPONENT    SilikegoGuiWindows
)
