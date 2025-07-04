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

project(SilikegoCli
	VERSION       0.0.0
	DESCRIPTION   "Command-line frontend for Silikego"
	HOMEPAGE_URL  "https://vdamewood.com/software/projects/silikego"
	LANGUAGES     CXX
)
set(PRERELEASE_SUFFIX "indev")

include(GNUInstallDirs)

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
if(NOT TARGET Silikego::SilikegoCore)
	find_package(SilikegoCore REQUIRED)
endif()
find_package(Readline)

add_executable(SilikegoCli)
target_sources(SilikegoCli
	PRIVATE
		src/StringSource.h
		src/Main.cc
		src/StringSource.cc

)
set_target_properties(SilikegoCli PROPERTIES
	OUTPUT_NAME               silikego
)
target_compile_features(SilikegoCli PRIVATE cxx_std_17)
target_link_libraries(SilikegoCli PRIVATE Silikego::SilikegoCore)

if (UNIX)
	target_compile_definitions(SilikegoCli PRIVATE USE_UNIX=1)
endif()

if(Readline_FOUND)
	target_link_libraries(SilikegoCli PRIVATE Readline::Readline)
	target_compile_definitions(SilikegoCli PRIVATE HAVE_READLINE=1)
else()
	target_sources(SilikegoCli PRIVATE src/rl.cc)
	target_compile_definitions(SilikegoCli PRIVATE HAVE_READLINE=0)
endif()

get_target_property(SILIKEGO_CORE_TYPE Silikego::SilikegoCore TYPE)
if(${SILIKEGO_CORE_TYPE} STREQUAL SHARED_LIBRARY)
	if(APPLE)
		get_target_property(SILIKEGO_CORE_FRAMEWORK Silikego::SilikegoCore FRAMEWORK)
		if (SILIKEGO_CORE_FRAMEWORK)
			set_target_properties(SilikegoCli PROPERTIES
				INSTALL_RPATH  "@executable_path/../Frameworks"
			)
		else()
			set_target_properties(SilikegoCli PROPERTIES
				INSTALL_RPATH  "@executable_path/../${CMAKE_INSTALL_LIBDIR}"
			)
		endif()
	else()
		set_target_properties(SilikegoCli PROPERTIES
			INSTALL_RPATH  "\${Origin}/../${CMAKE_INSTALL_LIBDIR}"
	)
	endif()
endif()

if(WIN32)
	if(PRERELEASE_SUFFIX STREQUAL "")
		set(VSV_PRERELEASE_FLAG 0)
	else()
		set(VSV_PRERELEASE_FLAG VS_FF_PRERELEASE)
	endif()

	set(VSV_DEBUG_FLAG 0)
	set(VSV_FILENAME "silikego.exe")
	configure_file(
		src/version.rc.in
		${CMAKE_CURRENT_BINARY_DIR}/version-release.rc
		@ONLY
	)
	target_sources(SilikegoCli PRIVATE $<$<CONFIG:Release>:${CMAKE_CURRENT_BINARY_DIR}/version-release.rc>)

	set(VSV_DEBUG_FLAG VS_FF_DEBUG)
	set(VSV_FILENAME "silikego.exe")
	configure_file(
		src/version.rc.in
		${CMAKE_CURRENT_BINARY_DIR}/version-debug.rc
		@ONLY
  	)
	target_sources(SilikegoCli PRIVATE $<$<CONFIG:Debug>:${CMAKE_CURRENT_BINARY_DIR}/version-debug.rc>)
endif()

install(TARGETS SilikegoCli
RUNTIME
	DESTINATION  ${CMAKE_INSTALL_BINDIR}
	COMPONENT    SilikegoCli
)
