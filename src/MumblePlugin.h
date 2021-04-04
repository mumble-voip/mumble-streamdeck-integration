// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// source tree.

#ifndef MUMBLE_STREAMDECK_INTEGRATION_MUMBLEPLUGIN_H_
#define MUMBLE_STREAMDECK_INTEGRATION_MUMBLEPLUGIN_H_

#include "StreamDeckPlugin.h"

#include <boost/filesystem.hpp>

#include <exception>

namespace Mumble {
namespace StreamDeckIntegration {

	class PluginException : std::exception {
	private:
		std::string m_errorMsg;

	public:
		PluginException(const std::string &errorMsg) : m_errorMsg(errorMsg) {}
		PluginException(const std::string &&errorMsg) : m_errorMsg(std::move(errorMsg)) {}

		const char *what() const noexcept override { return m_errorMsg.c_str(); }
	};

	class MumblePlugin : public StreamDeckPlugin {
	public:
		MumblePlugin() {}
		virtual ~MumblePlugin() {}

		virtual void keyDownForAction(const std::string &actionID, const std::string &context,
									  const nlohmann::json &payload, const std::string &deviceID) override;
		virtual void keyUpForAction(const std::string &actionID, const std::string &context,
									const nlohmann::json &payload, const std::string &deviceID) override;

		virtual void willAppearForAction(const std::string &actionID, const std::string &context,
										 const nlohmann::json &payload, const std::string &deviceID) override;
		virtual void willDisappearForAction(const std::string &actionID, const std::string &context,
											const nlohmann::json &payload, const std::string &deviceID) override;

		virtual void deviceDidConnect(const std::string &deviceID, const nlohmann::json &deviceInfo) override;
		virtual void deviceDidDisconnect(const std::string &deviceID) override;

		virtual void sendToPlugin(const std::string &actionID, const std::string &context,
								  const nlohmann::json &payload, const std::string &deviceID) override;

		virtual void receivedGlobalSettings(const nlohmann::json &settings) override;

		virtual void receivedData(const nlohmann::json &data, const std::string &context) override;

	private:
		nlohmann::json m_settings;

		/**
		 * Gets the JSON object that is to be sent to the CLI for the given action
		 *
		 * @param actionID String representation of the action
		 * @param settings Settings to respect for the action (may be an empty JSON struct)
		 * @returns The respective JSON
		 *
		 * @throws Plugexception If there is no action with the given ID
		 */
		nlohmann::json getJSONForAction(const std::string &actionID, const nlohmann::json &settings) const;
		/**
		 * Tries to locate the CLI application's path in the host system
		 *
		 * @throws Plugexception In case the CLI can't be found
		 */
		boost::filesystem::path findCLI();
		/**
		 * Sends the given action JSON to the CLI at the given location and processes the
		 * resulting output.
		 *
		 * @param action The JSON describing the action that is sent to the CLI
		 * @param cliPath The path to the CLI executable
		 * @returns The JSON response from the CLI
		 *
		 * @throws PluginException If anything goes wrong
		 */
		nlohmann::json executeAction(const nlohmann::json &action, const boost::filesystem::path &cliPath);
	};

};     // namespace StreamDeckIntegration
};     // namespace Mumble
#endif // MUMBLE_STREAMDECK_INTEGRATION_MUMBLEPLUGIN_H_
