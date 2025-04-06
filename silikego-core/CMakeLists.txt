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

project(SilikegoCore
	VERSION       0.0.0
	DESCRIPTION   "Mathematical expression parser library written in C"
	HOMEPAGE_URL  "https://vdamewood.com/software/projects/silikego"
	LANGUAGES     CXX
)
set(PRERELEASE_SUFFIX "indev")

include(GNUInstallDirs)

if(APPLE)
	option(SILIKEGO_CORE_BUILD_FRAMEWORK   "Build Silikego Core as a framework." On)
	set(SILIKEGO_CORE_INSTALL_FRAMEWORKDIR  Frameworks  CACHE STRING
		"Locaton to install framework (ignored if SILIKEGO_CORE_BUILD_FRAMEWORK is off)"
	)
endif()
set(SILIKEGO_CORE_INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/SilikegoCore" CACHE STRING
		"Locaton to install CMake files (ignored if SILIKEGO_CORE_BUILD_FRAMEWORK is on)"
)

if(SILIKEGO_CORE_BUILD_FRAMEWORK)
	set(CMAKEDIR "${SILIKEGO_CORE_INSTALL_FRAMEWORKDIR}/SilikegoCore.framework/Resources")
	set(RUNTIME_COMPONENT "SilikegoCore")
	set(DEVELOPMENT_COMPONENT "SilikegoCore")
else()
	set(CMAKEDIR "${SILIKEGO_CORE_INSTALL_CMAKEDIR}")
	set(RUNTIME_COMPONENT "SilikegoCore_Runtime")
	set(DEVELOPMENT_COMPONENT "SilikegoCore_Development")
endif()


add_library(SilikegoCore)
add_library(Silikego::SilikegoCore ALIAS SilikegoCore)
set(PUBLIC_HEADERS
	include/SilikegoCore/Value.h
	include/SilikegoCore/FunctionCaller.h
	include/SilikegoCore/SyntaxTree.h
	include/SilikegoCore/DataSource.h
	include/SilikegoCore/StringSource.h
	include/SilikegoCore/Token.h
	include/SilikegoCore/Lexer.h
	include/SilikegoCore/InfixParser.h
	${CMAKE_CURRENT_BINARY_DIR}/include/SilikegoCore/Api.h
)
target_sources(SilikegoCore
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
target_include_directories(SilikegoCore
	PUBLIC
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/include>"
		"$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
)
if (NOT SILIKEGO_CORE_BUILD_FRAMEWORK)
	target_include_directories(SilikegoCore
	INTERFACE
		$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)
endif()
target_compile_features(SilikegoCore PRIVATE cxx_std_11)
set_target_properties(SilikegoCore PROPERTIES
	PUBLIC_HEADER             "${PUBLIC_HEADERS}"
	C_VISIBILITY_PRESET       hidden
	VISIBILITY_INLINES_HIDDEN On
	FRAMEWORK                 "${SILIKEGO_CORE_BUILD_FRAMEWORK}"
	FRAMEWORK_VERSION         ${PROJECT_VERSION}
	VERSION                   ${PROJECT_VERSION}
	SOVERSION                 ${PROJECT_VERSION_MAJOR}
)
if(WIN32)
	if(PRERELEASE_SUFFIX STREQUAL "")
		set(VSV_PRERELEASE_FLAG 0)
	else()
		set(VSV_PRERELEASE_FLAG VS_FF_PRERELEASE)
	endif()

	set(VSV_DEBUG_FLAG 0)
	set(VSV_FILENAME "SilikegoCore.dll")
	configure_file(
		src/version.rc.in
		${CMAKE_CURRENT_BINARY_DIR}/version-release.rc
		@ONLY
  	)
	target_sources(SilikegoCore PRIVATE $<$<CONFIG:Release>:${CMAKE_CURRENT_BINARY_DIR}/version-release.rc>)

	set(VSV_DEBUG_FLAG VS_FF_DEBUG)
	set(VSV_FILENAME "SilikegoCore.dll")
	configure_file(
		src/version.rc.in
		${CMAKE_CURRENT_BINARY_DIR}/version-debug.rc
		@ONLY
  	)
	target_sources(SilikegoCore PRIVATE $<$<CONFIG:Debug>:${CMAKE_CURRENT_BINARY_DIR}/version-debug.rc>)
endif()

include(GenerateExportHeader)
generate_export_header(SilikegoCore
	EXPORT_FILE_NAME include/SilikegoCore/Api.h
)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
	SilikegoCoreConfigVersion.cmake
	VERSION ${PROJECT_VERSION}
	COMPATIBILITY SameMajorVersion
)

install(TARGETS SilikegoCore
	EXPORT SilikegoCore
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
	DESTINATION           "${SILIKEGO_CORE_INSTALL_FRAMEWORKDIR}"
	COMPONENT             "${DEVELOPMENT_COMPONENT}"
PUBLIC_HEADER
	DESTINATION           "${CMAKE_INSTALL_INCLUDEDIR}/SilikegoCore"
	COMPONENT             "${DEVELOPMENT_COMPONENT}"
)
install(EXPORT SilikegoCore
	NAMESPACE             Silikego::
	FILE                  SilikegoCoreTarget.cmake
	DESTINATION           "${CMAKEDIR}"
	COMPONENT             "${DEVELOPMENT_COMPONENT}"
)
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/SilikegoCoreConfigVersion.cmake"
		"SilikegoCoreConfig.cmake"
	DESTINATION           "${CMAKEDIR}"
	COMPONENT             "${DEVELOPMENT_COMPONENT}"
)
install(FILES
		"${PROJECT_SOURCE_DIR}/Readme.md"
	DESTINATION "${CMAKE_INSTALL_DATADIR}/doc/SilikegoCore"
	COMPONENT "${RUNTIME_COMPONENT}"
)

install(FILES
		"${PROJECT_SOURCE_DIR}/License.txt"
		"${PROJECT_SOURCE_DIR}/License.Gpl.txt"
	DESTINATION "${CMAKE_INSTALL_DATADIR}/doc/SilikegoCore"
	COMPONENT "${RUNTIME_COMPONENT}"
)

function(silikego_core_fixup_bundle TGT)
	if(APPLE AND TARGET ${TGT})
		get_target_property(TGT_IS_BUNDLE ${TGT} MACOSX_BUNDLE)
	else()
		set(TGT_IS_BUNDLE No)
	endif()
	get_target_property(SILIKEGO_CORE_TYPE Silikego::SilikegoCore TYPE)
	get_target_property(SILIKEGO_CORE_IS_FRAMEWORK Silikego::SilikegoCore FRAMEWORK)

	if (${SILIKEGO_CORE_TYPE} STREQUAL SHARED_LIBRARY AND TGT_IS_BUNDLE)
		get_target_property(TGT_RPATH ${TGT} INSTALL_RPATH)
		if(NOT "@executable_path/../Frameworks" IN_LIST TGT_RPATH)
			list(APPEND TGT_RPATH "@executable_path/../Frameworks")
			set_target_properties(${TGT} PROPERTIES
				INSTALL_RPATH  ${TGT_RPATH})
		endif()
		set_target_properties(${TGT} PROPERTIES
			BUILD_WITH_INSTALL_RPATH On)

		if(SILIKEGO_CORE_IS_FRAMEWORK)
			add_custom_command(TARGET ${TGT} POST_BUILD
				COMMAND "cmake" "-E" "make_directory"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks/$<TARGET_FILE_NAME:Silikego::SilikegoCore>.framework/Versions/$<TARGET_PROPERTY:Silikego::SilikegoCore,FRAMEWORK_VERSION>/Resources"
				COMMAND "cmake" "-E" "copy"
					"$<TARGET_FILE_DIR:Silikego::SilikegoCore>/Resources/Info.plist"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks/$<TARGET_FILE_NAME:Silikego::SilikegoCore>.framework/Versions/$<TARGET_PROPERTY:Silikego::SilikegoCore,FRAMEWORK_VERSION>/Resources/"
				COMMAND "cmake" "-E" "copy"
					"$<TARGET_FILE:Silikego::SilikegoCore>"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks/$<TARGET_FILE_NAME:Silikego::SilikegoCore>.framework/Versions/$<TARGET_PROPERTY:Silikego::SilikegoCore,FRAMEWORK_VERSION>/"
				COMMAND "cmake" "-E" "create_symlink"
					"$<TARGET_PROPERTY:Silikego::SilikegoCore,FRAMEWORK_VERSION>"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks/$<TARGET_FILE_NAME:Silikego::SilikegoCore>.framework/Versions/Current"
				COMMAND "cmake" "-E" "create_symlink"
					"Versions/Current/Resources"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks/$<TARGET_FILE_NAME:Silikego::SilikegoCore>.framework/Resources"
				COMMAND "cmake" "-E" "create_symlink"
					"Versions/Current/$<TARGET_FILE_NAME:Silikego::SilikegoCore>"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks/$<TARGET_FILE_NAME:Silikego::SilikegoCore>.framework/$<TARGET_FILE_NAME:Silikego::SilikegoCore>"
				COMMENT "Installing Silikego Core framework into ${TGT} bundle"
			)
		else()
			add_custom_command(TARGET ${TGT} POST_BUILD
				COMMAND "cmake" "-E" "make_directory"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks"
				COMMAND "cmake" "-E" "copy"
					"$<TARGET_FILE:Silikego::SilikegoCore>"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks/"
				COMMAND "cmake" "-E" "create_symlink"
					"$<TARGET_FILE_NAME:Silikego::SilikegoCore>"
					"$<TARGET_BUNDLE_CONTENT_DIR:${TGT}>/Frameworks/$<TARGET_SONAME_FILE_NAME:Silikego::SilikegoCore>"
				COMMENT "Installing Silikego Core dylib into ${TGT} bundle"
			)
		endif()
	endif()
endfunction()
