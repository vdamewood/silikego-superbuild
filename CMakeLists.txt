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

option(SILIKEGO_BUILD_CLI  "Build silikego-calc command-line tool" On)
option(SILIKEGO_BUILD_GUI  "Build silikego-calc GUI app for the current system" On)

project(Silikego)

add_subdirectory(silikego)

if(SILIKEGO_BUILD_CLI)
	add_subdirectory(silikego-calc-cli)
endif()

if(SILIKEGO_BUILD_GUI)
	if(APPLE)
		add_subdirectory(silikego-calc-cocoa)
	elseif(UNIX)
		add_subdirectory(silikego-calc-gtkmm)
	elseif(WIN32)
		add_subdirectory(silikego-calc-winapi)
	else()
		message(FATAL_ERROR "A GUI program is not available for this system.")
	endif()
endif()
