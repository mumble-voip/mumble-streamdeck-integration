// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// source tree.

// The original version of this code was taken from https://github.com/elgatosf/streamdeck-cpu
// which was published under the MIT license.

#include "ConnectionManager.h"
#include "ESDSDKDefines.h"
#include "Utils.h"

#include "MumblePlugin.h"

#include <iostream>
#include <memory>

using namespace Mumble::StreamDeckIntegration;

int main(int argc, const char **argv) {
	if (argc != 9) {
		std::cerr << "Invalid number of parameters: " << argc - 1 << " instead of 8" << std::endl;
		return 1;
	}

	int port = 0;
	std::string pluginUUID;
	std::string registerEvent;
	std::string info;

	for (int argumentIndex = 0; argumentIndex < 4; argumentIndex++) {
		std::string parameter(argv[1 + 2 * argumentIndex]);
		std::string value(argv[1 + 2 * argumentIndex + 1]);

		if (parameter == kESDSDKPortParameter) {
			port = std::atoi(value.c_str());
		} else if (parameter == kESDSDKPluginUUIDParameter) {
			pluginUUID = value;
		} else if (parameter == kESDSDKRegisterEventParameter) {
			registerEvent = value;
		} else if (parameter == kESDSDKInfoParameter) {
			info = value;
		}
	}

	if (port == 0) {
		std::cerr << "Invalid port number" << std::endl;
		return 1;
	}

	if (pluginUUID.empty()) {
		std::cerr << "Invalid plugin UUID" << std::endl;
		return 1;
	}

	if (registerEvent.empty()) {
		std::cerr << "Invalid registerEvent" << std::endl;
		return 1;
	}

	if (info.empty()) {
		std::cerr << "Invalid info" << std::endl;
		return 1;
	}

	// Create the plugin
	std::unique_ptr< MumblePlugin > plugin = std::make_unique< MumblePlugin >();

	// Create the connection manager
	std::unique_ptr< ConnectionManager > connectionManager =
		std::make_unique< ConnectionManager >(port, pluginUUID, registerEvent, info, *plugin);

	// Connect and start the event loop
	connectionManager->run();

	return 0;
}
