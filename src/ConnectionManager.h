// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// source tree.

// The original version of this code was taken from https://github.com/elgatosf/streamdeck-cpu
// which was published under the MIT license.

#ifndef MUMBLE_STREAMDECK_INTEGRATION_CONNECTIONMANAGER_H_
#define MUMBLE_STREAMDECK_INTEGRATION_CONNECTIONMANAGER_H_

#include <string>

#include "ESDSDKDefines.h"

#include <websocketpp/client.hpp>
#include <websocketpp/common/memory.hpp>
#include <websocketpp/common/thread.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>

#include <nlohmann/json.hpp>

namespace Mumble {
namespace StreamDeckIntegration {

	typedef websocketpp::config::asio_client::message_type::ptr message_ptr;
	typedef websocketpp::client< websocketpp::config::asio_client > WebsocketClient;

	class StreamDeckPlugin;

	class ConnectionManager {
	public:
		ConnectionManager(int port, const std::string &pluginUUID, const std::string &registerEvent,
						  const std::string &info, StreamDeckPlugin &plugin);

		/// Start the event loop
		void run();

		/**
		 * Reports about an error that occured. This involves writing the error message
		 * to the log file and optionally triggering an alert for the provided context.
		 *
		 * @param errorMessage The error message that should be logged
		 * @param context If given this is the context that an alert will be shown for
		 */
		void reportError(const std::string &errorMessage, const std::string &context = "");

		// API to communicate with the Stream Deck application
		void api_setTitle(const std::string &title, const std::string &context, ESDSDKTarget target);
		void api_setImage(const std::string &base64ImageString, const std::string &context, ESDSDKTarget target);
		void api_showAlertForContext(const std::string &context);
		void api_showOKForContext(const std::string &context);
		void api_setSettings(const nlohmann::json &settings, const std::string &context);
		void api_setState(int state, const std::string &context);
		void api_sendToPropertyInspector(const std::string &action, const std::string &context,
										 const nlohmann::json &payload);
		void api_switchToProfile(const std::string &deviceID, const std::string &profileName);
		void api_logMessage(const std::string &message);

	private:
		// Websocket callbacks
		void onOpen(WebsocketClient *client, websocketpp::connection_hdl connectionHandler);
		void onFail(WebsocketClient *client, websocketpp::connection_hdl connectionHandler);
		void onClose(WebsocketClient *client, websocketpp::connection_hdl connectionHandler);
		void onMessage(websocketpp::connection_hdl, WebsocketClient::message_ptr msg);

		// Member variables
		int m_port = 0;
		std::string m_pluginUUID;
		std::string m_registerEvent;
		websocketpp::connection_hdl m_connectionHandle;
		WebsocketClient m_websocket;
		StreamDeckPlugin &m_plugin;
	};

}; // namespace StreamDeckIntegration
}; // namespace Mumble

#endif // MUMBLE_STREAMDECK_INTEGRATION_CONNECTIONMANAGER_H_
