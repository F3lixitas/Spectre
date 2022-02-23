cmake_minimum_required(VERSION 3.21)
project(Spectre)

set(CMAKE_CXX_STANDARD 23)

find_package(Vulkan)
if (NOT ${Vulkan_INCLUDE_DIRS} STREQUAL "")
	set(VULKAN_PATH ${Vulkan_INCLUDE_DIRS})
	STRING(REGEX REPLACE "/Include" "" VULKAN_PATH ${VULKAN_PATH})
endif()
		 
if(NOT Vulkan_FOUND)
	# CMake may fail to locate the libraries but could be able to 
	# provide some path in Vulkan SDK include directory variable
	# 'Vulkan_INCLUDE_DIRS', try to extract path from this.
	message(STATUS "Failed to locate Vulkan SDK, retrying again...")
	if(EXISTS "${VULKAN_PATH}")
		message(STATUS "Successfully located the Vulkan SDK: ${VULKAN_PATH}")
	else()
		message("Error: Unable to locate Vulkan SDK. Please turn off auto locate option by specifying 'AUTO_LOCATE_VULKAN' as 'OFF'")
		message("and specify manually path using 'VULKAN_SDK' and 'VULKAN_VERSION' variables in the CMakeLists.txt.")
		return()
	endif()
endif()

add_executable(Spectre main.cpp)
