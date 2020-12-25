# Copyright 2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# source tree.

cmake_minimum_required(VERSION 3.15)

set(version "0.1.0" CACHE STRING "Project version")

project(mumble-streamdeck-integration
	VERSION ${version}
	DESCRIPTION "A plugin for integrating Mumble with Elgato's Stream Deck"
	LANGUAGES "C" "CXX"
)

option(static "Prefer static libraries instead of shared ones" OFF)
option(enable-packaging "Create a build target \"package\" that'll package the plugin" OFF)

set(3RDPARTY_DIR "${CMAKE_SOURCE_DIR}/3rdParty")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


set(CXX_ACTION_UUID_INCLUDE_FILE "${CMAKE_BINARY_DIR}/MumbleActionIDs.h")
set(CXX_SETTINGS_INCLUDE_FILE "${CMAKE_BINARY_DIR}/MumbleSettingIDs.h")

include(actionIDs.cmake)
include(settingIDs.cmake)


set(PLUGIN_DIR_NAME "info.mumble.mumble.sdPlugin")
set(PLUGIN_SKELETON_DIR "${CMAKE_SOURCE_DIR}/pluginSkeleton")
set(PLUGIN_DIR "${CMAKE_BINARY_DIR}/${PLUGIN_DIR_NAME}")

# Copy plugin skeleton to build dir
file(COPY "${PLUGIN_SKELETON_DIR}/css" "${PLUGIN_SKELETON_DIR}/images" DESTINATION "${PLUGIN_DIR}")

# configure 
configure_file("${PLUGIN_SKELETON_DIR}/manifest.json.in" "${PLUGIN_DIR}/manifest.json")
configure_file("${PLUGIN_SKELETON_DIR}/property_inspector.html.in" "${PLUGIN_DIR}/property_inspector.html")
configure_file("${PLUGIN_SKELETON_DIR}/property_inspector.js.in" "${PLUGIN_DIR}/property_inspector.js")

set(Boost_USE_STATIC_LIBS ${static})


set(JSON_BuildTests OFF CACHE INTERNAL "")
set(JSON_Install OFF CACHE INTERNAL "")
set(JSON_ImplicitConversions OFF CACHE INTERNAL "")

add_subdirectory("${3RDPARTY_DIR}/nlohmann" "nlohmann")
add_subdirectory("${3RDPARTY_DIR}/websocketpp" "websocketpp")

add_executable(streamdeck_integration
	src/main.cpp
	src/MumblePlugin.cpp
	src/ConnectionManager.cpp
	src/Utils.cpp
)
# Set the output directory of the executable
# We have to use a generator expression that always evaluates to the same String in order to
# prevent multi-config generators from creating per-config sub-directories instead of creating
# the executable directly in the specified dir.
set_target_properties(streamdeck_integration PROPERTIES RUNTIME_OUTPUT_DIRECTORY "$<$<BOOL:true>:${PLUGIN_DIR}>")

# As the WebSocketpp project does not configure its targets properly, we have to
# search for its Boost dependencies as well.
set(WEBSOCKETPP_BOOST_LIBS random system thread regex)
find_package(Boost COMPONENTS filesystem ${WEBSOCKETPP_BOOST_LIBS} REQUIRED)

target_link_libraries(streamdeck_integration
	nlohmann_json::nlohmann_json
	${Boost_LIBRARIES}
)

target_include_directories(streamdeck_integration PRIVATE
	"${CMAKE_BINARY_DIR}"
	"${3RDPARTY_DIR}/websocketpp/"
	${Boost_INCLUDE_DIRS}
)


if(enable-packaging)
	if(NOT STREAMDECK_DISTRIBUTION_TOOL)
		find_program(STREAMDECK_DISTRIBUTION_TOOL NAMES DistributionTool DistributionTool.exe)

		if(NOT STREAMDECK_DISTRIBUTION_TOOL)
			#message(FATAL_ERROR "Unable to locate the Stream Deck Distribution tool - won't be able to package plugin")
			message(FATAL_ERROR "Unable to locate the Stream Deck Distribution tool - Try setting STREAMDECK_DISTRIBUTION_TOOL to the path of the tool on your local disk")
		endif()
	endif()

	set(BUNDLED_PLUGIN_DIR "${CMAKE_SOURCE_DIR}/bundled")

	message(STATUS "Using Stream Deck Distribution tool ${STREAMDECK_DISTRIBUTION_TOOL}")

	add_custom_target(package COMMENT "Packages the plugin to a proper .streamDeckPlugin file")
	add_dependencies(package streamdeck_integration)

	# bundle the plugin
	add_custom_command(TARGET package PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E rm -rf "${BUNDLED_PLUGIN_DIR}"
	)
	add_custom_command(TARGET package PRE_BUILD
		COMMAND ${CMAKE_COMMAND} -E make_directory "${BUNDLED_PLUGIN_DIR}"
	)
	add_custom_command(TARGET package POST_BUILD
		COMMAND ${STREAMDECK_DISTRIBUTION_TOOL} -b -i "${CMAKE_CURRENT_BINARY_DIR}/${PLUGIN_DIR_NAME}" -o "${BUNDLED_PLUGIN_DIR}"
	)
endif()
