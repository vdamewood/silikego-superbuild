# CMakeLists.txt: Build Script
# Copyright 2012-2023 Vincent Damewood
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

if(SILIKEGO_BUILD_FRAMEWORK)
	set(CMAKEDIR "${SILIKEGO_INSTALL_FRAMEWORKDIR}/Silikego.framework/Resources")
	set(RUNTIME_COMPONENT "Silikego")
	set(DEVELOPMENT_COMPONENT "Silikego")
else()
	set(CMAKEDIR "${SILIKEGO_INSTALL_CMAKEDIR}")
	set(RUNTIME_COMPONENT "Silikego_Runtime")
	set(DEVELOPMENT_COMPONENT "Silikego_Development")
endif()

add_library(Silikego)
add_library(Silikego::Silikego ALIAS Silikego)

set(PUBLIC_HEADERS
	include/Silikego/Value.h
	include/Silikego/FunctionCaller.h
	include/Silikego/SyntaxTree.h
	include/Silikego/DataSource.h
	include/Silikego/StringSource.h
	include/Silikego/Token.h
	include/Silikego/Lexer.h
	include/Silikego/InfixParser.h
	${CMAKE_CURRENT_BINARY_DIR}/include/Silikego/Api.h
)
target_sources(Silikego
	PRIVATE
		${PUBLIC_HEADERS}
		src/Functions.h
		src/Value.cc
		src/Functions.cc
		src/FunctionCaller.cc
		src/SyntaxTree.cc
		src/DataSource.cc
		src/StringSource.cc
		src/Token.cc
		src/Lexer.cc
		src/InfixParser.cc
)
target_include_directories(Silikego
	PUBLIC
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)
if (NOT SILIKEGO_BUILD_FRAMEWORK)
	target_include_directories(Silikego
	INTERFACE
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
endif()
target_compile_features(Silikego PRIVATE cxx_std_11)
set_target_properties(Silikego PROPERTIES
	PUBLIC_HEADER             "${PUBLIC_HEADERS}"
	CXX_VISIBILITY_PRESET     hidden
	VISIBILITY_INLINES_HIDDEN On
	FRAMEWORK                 "${SILIKEGO_BUILD_FRAMEWORK}"
	FRAMEWORK_VERSION         ${PROJECT_VERSION}
	VERSION                   ${PROJECT_VERSION}
	SOVERSION                 ${PROJECT_VERSION_MAJOR}
	RUNTIME_OUTPUT_DIRECTORY  "${PROJECT_BINARY_DIR}/products"
	LIBRARY_OUTPUT_DIRECTORY  "${PROJECT_BINARY_DIR}/products"
	ARCHIVE_OUTPUT_DIRECTORY  "${PROJECT_BINARY_DIR}/products"
	PDB_OUTPUT_DIRECTORY      "${PROJECT_BINARY_DIR}/products"
)
if(WIN32)
	set(VSV_DEBUG_FLAG 0)
	set(VSV_FILENAME "Silikego.dll")
	configure_file(
		src/version.rc.in
		${CMAKE_CURRENT_BINARY_DIR}/version-release.rc
		@ONLY
  	)
	target_sources(Silikego PRIVATE $<$<CONFIG:Release>:${CMAKE_CURRENT_BINARY_DIR}/version-release.rc>)

	set(VSV_DEBUG_FLAG VS_FF_DEBUG)
	set(VSV_FILENAME "Silikego.dll")
	configure_file(
		src/version.rc.in
		${CMAKE_CURRENT_BINARY_DIR}/version-debug.rc
		@ONLY
  	)
	target_sources(Silikego PRIVATE $<$<CONFIG:Debug>:${CMAKE_CURRENT_BINARY_DIR}/version-debug.rc>)
endif()

include(GenerateExportHeader)
generate_export_header(Silikego
	EXPORT_FILE_NAME include/Silikego/Api.h
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	SilikegoConfigVersion.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion
)

install(TARGETS Silikego
	EXPORT Silikego
RUNTIME
	DESTINATION           "${CMAKE_INSTALL_BINDIR}"
	COMPONENT             "${RUNTIME_COMPONENT}"
ARCHIVE
	DESTINATION           "${CMAKE_INSTALL_LIBDIR}"
	COMPONENT             "${DEVELOPMENT_COMPONENT}"
LIBRARY
	DESTINATION           "${CMAKE_INSTALL_LIBDIR}"
	COMPONENT             "${RUNTIME_COMPONENT}"
	NAMELINK_COMPONENT    "${DEVELOPMENT_COMPONENT}"
FRAMEWORK
	DESTINATION           "${SILIKEGO_INSTALL_FRAMEWORKDIR}"
	COMPONENT             "${DEVELOPMENT_COMPONENT}"
PUBLIC_HEADER
	DESTINATION           "${CMAKE_INSTALL_INCLUDEDIR}/Silikego"
	COMPONENT             "${DEVELOPMENT_COMPONENT}"
)
install(EXPORT Silikego
	NAMESPACE             Silikego::
	FILE                  SilikegoTarget.cmake
	DESTINATION           "${CMAKEDIR}"
	COMPONENT             "${DEVELOPMENT_COMPONENT}"
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/SilikegoConfigVersion.cmake"
               "SilikegoConfig.cmake"
	DESTINATION           "${CMAKEDIR}"
	COMPONENT             "${DEVELOPMENT_COMPONENT}"
)
install(FILES
		"${PROJECT_SOURCE_DIR}/Readme.md"
	DESTINATION "${CMAKE_INSTALL_DATADIR}/doc/Silikego"
	COMPONENT "${RUNTIME_COMPONENT}"
)

install(FILES
		"${PROJECT_SOURCE_DIR}/License.txt"
		"${PROJECT_SOURCE_DIR}/License.Gpl.txt"
	DESTINATION "${CMAKE_INSTALL_DATADIR}/doc/Silikego"
	COMPONENT "${RUNTIME_COMPONENT}"
)
