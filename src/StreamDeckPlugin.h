// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// source tree.

// The original version of this code was taken from https://github.com/elgatosf/streamdeck-cpu
// which was published under the MIT license.

#ifndef MUMBLE_STREAMDECK_INTEGRATION_STREAMDECKPLUGIN_H_
#define MUMBLE_STREAMDECK_INTEGRATION_STREAMDECKPLUGIN_H_

#include <string>

#include <nlohmann/json.hpp>

namespace Mumble {
namespace StreamDeckIntegration {

	class ConnectionManager;

	class StreamDeckPlugin {
	public:
		StreamDeckPlugin() {}
		virtual ~StreamDeckPlugin() {}

		void setConnectionManager(ConnectionManager *inConnectionManager) { m_connectionManager = inConnectionManager; }

		virtual void keyDownForAction(const std::string &inAction, const std::string &inContext,
									  const nlohmann::json &inPayload, const std::string &inDeviceID) = 0;
		virtual void keyUpForAction(const std::string &inAction, const std::string &inContext,
									const nlohmann::json &inPayload, const std::string &inDeviceID)   = 0;

		virtual void willAppearForAction(const std::string &inAction, const std::string &inContext,
										 const nlohmann::json &inPayload, const std::string &inDeviceID)    = 0;
		virtual void willDisappearForAction(const std::string &inAction, const std::string &inContext,
											const nlohmann::json &inPayload, const std::string &inDeviceID) = 0;

		virtual void deviceDidConnect(const std::string &inDeviceID, const nlohmann::json &inDeviceInfo) = 0;
		virtual void deviceDidDisconnect(const std::string &inDeviceID)                                  = 0;

		virtual void sendToPlugin(const std::string &inAction, const std::string &inContext,
								  const nlohmann::json &inPayload, const std::string &inDeviceID) = 0;

		virtual void receivedGlobalSettings(const nlohmann::json &settings) = 0;

		virtual void receivedData(const nlohmann::json &data, const std::string &context) = 0;

	protected:
		ConnectionManager *m_connectionManager = nullptr;
	};

};     // namespace StreamDeckIntegration
};     // namespace Mumble
#endif // MUMBLE_STREAMDECK_INTEGRATION_STREAMDECKPLUGIN_H_
