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

find_program(IBTOOL ibtool)
if (NOT IBTOOL)
	message(FATAL_ERROR "Command ibtool not found. Can't build GUI. Quitting.")
endif()

add_custom_command(OUTPUT MainMenu.nib
	COMMAND "${IBTOOL}" --compile MainMenu.nib "${CMAKE_CURRENT_SOURCE_DIR}/MainMenu.xib"
	COMMENT "Compiling ${CMAKE_CURRENT_SOURCE_DIR}/MainMenu.xib"
)
set_source_files_properties(MainMenu.nib PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
set_source_files_properties(silikegujo.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

add_executable(SilikegujoForMacOS MACOSX_BUNDLE)
target_sources(SilikegujoForMacOS
	PRIVATE
		Delegate.h
		Delegate.mm
		Main.mm
		MainMenu.nib
		silikegujo.icns
)
set_target_properties(SilikegujoForMacOS PROPERTIES
	OUTPUT_NAME                         Silikegujo
	MACOSX_BUNDLE_GUI_IDENTIFIER        "com.vdamewood.SilikegujoForMacOS"
	MACOSX_BUNDLE_BUNDLE_NAME           "Silikegujo"
	MACOSX_BUNDLE_SHORT_VERSION_STRING  ${PROJECT_VERSION}
	MACOSX_BUNDLE_BUNDLE_VERSION        ${PROJECT_VERSION}
	MACOSX_BUNDLE_COPYRIGHT             "${SILIKEGO_COPYRIGHT}"
	MACOSX_BUNDLE_ICON_FILE             silikegujo.icns
	MACOSX_BUNDLE_INFO_PLIST            "${CMAKE_CURRENT_SOURCE_DIR}/Info.plist.in"
	RUNTIME_OUTPUT_DIRECTORY            "${PROJECT_BINARY_DIR}/products"
	LIBRARY_OUTPUT_DIRECTORY            "${PROJECT_BINARY_DIR}/products"
	ARCHIVE_OUTPUT_DIRECTORY            "${PROJECT_BINARY_DIR}/products"
	PDB_OUTPUT_DIRECTORY                "${PROJECT_BINARY_DIR}/products"
)
target_compile_options(SilikegujoForMacOS
	PRIVATE
		"-fobjc-arc"
)
target_compile_features(SilikegujoForMacOS PRIVATE cxx_std_11)
target_link_libraries(SilikegujoForMacOS
	PRIVATE
		Silikego::Silikego
		"-framework Cocoa"
)

get_target_property(SILIKEGO_TYPE Silikego::Silikego TYPE)
if (${SILIKEGO_TYPE} STREQUAL SHARED_LIBRARY)
	set_target_properties(SilikegujoForMacOS PROPERTIES
		BUILD_WITH_INSTALL_RPATH        On
		INSTALL_RPATH                   "@executable_path/../Frameworks"
	)

	if(SILIKEGO_BUILD_FRAMEWORK)
		add_custom_command(TARGET SilikegujoForMacOS POST_BUILD
			COMMAND "cmake" "-E" "make_directory"
				"$<TARGET_BUNDLE_CONTENT_DIR:SilikegujoForMacOS>/Frameworks/$<TARGET_FILE_NAME:Silikego>.framework/Versions/${PROJECT_VERSION}/Resources"
			COMMAND "cmake" "-E" "copy"
				"$<TARGET_FILE_DIR:Silikego>/Resources/Info.plist"
				"$<TARGET_BUNDLE_CONTENT_DIR:SilikegujoForMacOS>/Frameworks/$<TARGET_FILE_NAME:Silikego>.framework/Versions/${PROJECT_VERSION}/Resources/"
			COMMAND "cmake" "-E" "copy"
				"$<TARGET_FILE:Silikego>"
				"$<TARGET_BUNDLE_CONTENT_DIR:SilikegujoForMacOS>/Frameworks/$<TARGET_FILE_NAME:Silikego>.framework/Versions/${PROJECT_VERSION}/"
			COMMAND "cmake" "-E" "create_symlink"
				"${PROJECT_VERSION}"
				"$<TARGET_BUNDLE_CONTENT_DIR:SilikegujoForMacOS>/Frameworks/$<TARGET_FILE_NAME:Silikego>.framework/Versions/Current"
			COMMAND "cmake" "-E" "create_symlink"
				"Versions/Current/Resources"
				"$<TARGET_BUNDLE_CONTENT_DIR:SilikegujoForMacOS>/Frameworks/$<TARGET_FILE_NAME:Silikego>.framework/Resources"
			COMMAND "cmake" "-E" "create_symlink"
				"Versions/Current/$<TARGET_FILE_NAME:Silikego>"
				"$<TARGET_BUNDLE_CONTENT_DIR:SilikegujoForMacOS>/Frameworks/$<TARGET_FILE_NAME:Silikego>.framework/$<TARGET_FILE_NAME:Silikego>"
			COMMENT "Installing framework into application bundle"
		)
	else()
		add_custom_command(TARGET SilikegujoForMacOS POST_BUILD
			COMMAND "cmake" "-E" "make_directory"
				"$<TARGET_BUNDLE_CONTENT_DIR:SilikegujoForMacOS>/Frameworks"
			COMMAND "cmake" "-E" "copy"
				"$<TARGET_FILE:Silikego>"
				"$<TARGET_BUNDLE_CONTENT_DIR:SilikegujoForMacOS>/Frameworks/"
			COMMAND "cmake" "-E" "create_symlink"
				"$<TARGET_FILE_NAME:Silikego>"
				"$<TARGET_BUNDLE_CONTENT_DIR:SilikegujoForMacOS>/Frameworks/$<TARGET_SONAME_FILE_NAME:Silikego>"
			COMMENT "Installing dylib into application bundle"
		)
	endif()
endif()

install(TARGETS SilikegujoForMacOS
BUNDLE
	DESTINATION  ${SILIKEGO_INSTALL_APPDIR}
	COMPONENT    Silikegujo
)
