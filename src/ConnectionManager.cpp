// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// source tree.

// The original version of this code was taken from https://github.com/elgatosf/streamdeck-cpu
// which was published under the MIT license.

#include "ConnectionManager.h"
#include "Utils.h"

#include "StreamDeckPlugin.h"

namespace Mumble {
namespace StreamDeckIntegration {

	void ConnectionManager::onOpen(WebsocketClient *client, websocketpp::connection_hdl connectionHandler) {
		// Register plugin with StreamDeck
		nlohmann::json jsonObject;
		jsonObject["event"] = m_registerEvent;
		jsonObject["uuid"]  = m_pluginUUID;

		websocketpp::lib::error_code ec;
		m_websocket.send(m_connectionHandle, jsonObject.dump(), websocketpp::frame::opcode::text, ec);

		// Apparently the first message that gets written to the log gets lost somewhere on Elgato's end
		// (only causes the log file to be created).
		// Thus we make sure to log an unnecessary thing right here at the beginning. We don't want it
		// to be jibberish though in case it does end up in the log after all.
		api_logMessage("Mumble StreamDeckIntegration is running");
	}

	void ConnectionManager::onFail(WebsocketClient *client, websocketpp::connection_hdl connectionHandler) {
		std::string reason;

		if (client != nullptr) {
			WebsocketClient::connection_ptr connection = client->get_con_from_hdl(connectionHandler);
			if (connection != NULL) {
				reason = connection->get_ec().message();
			}
		}
	}

	void ConnectionManager::onClose(WebsocketClient *client, websocketpp::connection_hdl connectionHandler) {
		std::string reason;

		if (client != nullptr) {
			WebsocketClient::connection_ptr connection = client->get_con_from_hdl(connectionHandler);
			if (connection != NULL) {
				reason = connection->get_remote_close_reason();
			}
		}
	}

	void ConnectionManager::onMessage(websocketpp::connection_hdl, WebsocketClient::message_ptr msg) {
		if (msg != NULL && msg->get_opcode() == websocketpp::frame::opcode::text) {
			std::string message = msg->get_payload();

			try {
				nlohmann::json receivedJson = nlohmann::json::parse(message);

				std::string event      = Utils::getStringByName(receivedJson, kESDSDKCommonEvent);
				std::string context    = Utils::getStringByName(receivedJson, kESDSDKCommonContext);
				std::string action     = Utils::getStringByName(receivedJson, kESDSDKCommonAction);
				std::string deviceID   = Utils::getStringByName(receivedJson, kESDSDKCommonDevice);
				nlohmann::json payload = Utils::getObjectByName(receivedJson, kESDSDKCommonPayload);

				if (event == kESDSDKEventKeyDown) {
					m_plugin.keyDownForAction(action, context, payload, deviceID);
				} else if (event == kESDSDKEventKeyUp) {
					m_plugin.keyUpForAction(action, context, payload, deviceID);
				} else if (event == kESDSDKEventWillAppear) {
					m_plugin.willAppearForAction(action, context, payload, deviceID);
				} else if (event == kESDSDKEventWillDisappear) {
					m_plugin.willDisappearForAction(action, context, payload, deviceID);
				} else if (event == kESDSDKEventDeviceDidConnect) {
					nlohmann::json deviceInfo = Utils::getObjectByName(receivedJson, kESDSDKCommonDeviceInfo);
					m_plugin.deviceDidConnect(deviceID, deviceInfo);
				} else if (event == kESDSDKEventDeviceDidDisconnect) {
					m_plugin.deviceDidDisconnect(deviceID);
				} else if (event == kESDSDKEventDidReceiveGlobalSettings) {
					const nlohmann::json settings = Utils::getObjectByName(payload, kESDSDKPayloadSettings);
					m_plugin.receivedGlobalSettings(settings);
				} else if (event == kESDSDKEventSendToPlugin) {
					m_plugin.receivedData(payload, context);
				}
			} catch (...) {
				reportError("Connection Manager encountered unexpected exception during event processing");
			}
		}
	}

	ConnectionManager::ConnectionManager(int port, const std::string &pluguUID, const std::string &registerEvent,
										 const std::string &info, StreamDeckPlugin &plugin)
		: m_port(port), m_pluginUUID(pluguUID), m_registerEvent(registerEvent), m_plugin(plugin) {
		plugin.setConnectionManager(this);
	}

	void ConnectionManager::run() {
		try {
			// Create the endpoint
			m_websocket.clear_access_channels(websocketpp::log::alevel::all);
			m_websocket.clear_error_channels(websocketpp::log::elevel::all);

			// Initialize ASIO
			m_websocket.init_asio();

			// Register our message handler
			m_websocket.set_open_handler(websocketpp::lib::bind(&ConnectionManager::onOpen, this, &m_websocket,
																websocketpp::lib::placeholders::_1));
			m_websocket.set_fail_handler(websocketpp::lib::bind(&ConnectionManager::onFail, this, &m_websocket,
																websocketpp::lib::placeholders::_1));
			m_websocket.set_close_handler(websocketpp::lib::bind(&ConnectionManager::onClose, this, &m_websocket,
																 websocketpp::lib::placeholders::_1));
			m_websocket.set_message_handler(websocketpp::lib::bind(&ConnectionManager::onMessage, this,
																   websocketpp::lib::placeholders::_1,
																   websocketpp::lib::placeholders::_2));

			websocketpp::lib::error_code ec;
			std::string uri                            = "ws://127.0.0.1:" + std::to_string(m_port);
			WebsocketClient::connection_ptr connection = m_websocket.get_connection(uri, ec);
			if (ec) {
				return;
			}

			m_connectionHandle = connection->get_handle();

			// Note that connect here only requests a connection. No network messages are
			// exchanged until the event loop starts running in the next line.
			m_websocket.connect(connection);

			// Start the ASIO io_service run loop
			// this will cause a single connection to be made to the server. m_websocket.run()
			// will exit when this connection is closed.
			m_websocket.run();
		} catch (websocketpp::exception const &) {
		}
	}

	void ConnectionManager::reportError(const std::string &errorMessage, const std::string &context) {
		// Log the error message
		api_logMessage("Mumble plugin error: " + errorMessage);

		if (context.size() > 0) {
			// Also show an alert for the given context
			api_showAlertForContext(context);
		}
	}

	void ConnectionManager::api_setTitle(const std::string &title, const std::string &context, ESDSDKTarget target) {
		nlohmann::json jsonObject;

		jsonObject[kESDSDKCommonEvent]   = kESDSDKEventSetTitle;
		jsonObject[kESDSDKCommonContext] = context;

		nlohmann::json payload;
		payload[kESDSDKPayloadTarget]    = target;
		payload[kESDSDKPayloadTitle]     = title;
		jsonObject[kESDSDKCommonPayload] = payload;

		websocketpp::lib::error_code ec;
		m_websocket.send(m_connectionHandle, jsonObject.dump(), websocketpp::frame::opcode::text, ec);
	}

	void ConnectionManager::api_setImage(const std::string &base64ImageString, const std::string &context,
										 ESDSDKTarget target) {
		nlohmann::json jsonObject;

		jsonObject[kESDSDKCommonEvent]   = kESDSDKEventSetImage;
		jsonObject[kESDSDKCommonContext] = context;

		nlohmann::json payload;
		payload[kESDSDKPayloadTarget] = target;
		const std::string prefix      = "data:image/png;base64,";
		if (base64ImageString.empty() || base64ImageString.substr(0, prefix.length()).find(prefix) == 0)
			payload[kESDSDKPayloadImage] = base64ImageString;
		else
			payload[kESDSDKPayloadImage] = "data:image/png;base64," + base64ImageString;
		jsonObject[kESDSDKCommonPayload] = payload;

		websocketpp::lib::error_code ec;
		m_websocket.send(m_connectionHandle, jsonObject.dump(), websocketpp::frame::opcode::text, ec);
	}

	void ConnectionManager::api_showAlertForContext(const std::string &context) {
		nlohmann::json jsonObject;

		jsonObject[kESDSDKCommonEvent]   = kESDSDKEventShowAlert;
		jsonObject[kESDSDKCommonContext] = context;

		websocketpp::lib::error_code ec;
		m_websocket.send(m_connectionHandle, jsonObject.dump(), websocketpp::frame::opcode::text, ec);
	}

	void ConnectionManager::api_showOKForContext(const std::string &context) {
		nlohmann::json jsonObject;

		jsonObject[kESDSDKCommonEvent]   = kESDSDKEventShowOK;
		jsonObject[kESDSDKCommonContext] = context;

		websocketpp::lib::error_code ec;
		m_websocket.send(m_connectionHandle, jsonObject.dump(), websocketpp::frame::opcode::text, ec);
	}

	void ConnectionManager::api_setSettings(const nlohmann::json &settings, const std::string &context) {
		nlohmann::json jsonObject;

		jsonObject[kESDSDKCommonEvent]   = kESDSDKEventSetSettings;
		jsonObject[kESDSDKCommonContext] = context;
		jsonObject[kESDSDKCommonPayload] = settings;

		websocketpp::lib::error_code ec;
		m_websocket.send(m_connectionHandle, jsonObject.dump(), websocketpp::frame::opcode::text, ec);
	}

	void ConnectionManager::api_setState(int state, const std::string &context) {
		nlohmann::json jsonObject;

		nlohmann::json payload;
		payload[kESDSDKPayloadState] = state;

		jsonObject[kESDSDKCommonEvent]   = kESDSDKEventSetState;
		jsonObject[kESDSDKCommonContext] = context;
		jsonObject[kESDSDKCommonPayload] = payload;

		websocketpp::lib::error_code ec;
		m_websocket.send(m_connectionHandle, jsonObject.dump(), websocketpp::frame::opcode::text, ec);
	}

	void ConnectionManager::api_sendToPropertyInspector(const std::string &action, const std::string &context,
														const nlohmann::json &payload) {
		nlohmann::json jsonObject;

		jsonObject[kESDSDKCommonEvent]   = kESDSDKEventSendToPropertyInspector;
		jsonObject[kESDSDKCommonContext] = context;
		jsonObject[kESDSDKCommonAction]  = action;
		jsonObject[kESDSDKCommonPayload] = payload;

		websocketpp::lib::error_code ec;
		m_websocket.send(m_connectionHandle, jsonObject.dump(), websocketpp::frame::opcode::text, ec);
	}

	void ConnectionManager::api_switchToProfile(const std::string &deviceID, const std::string &profileName) {
		if (!deviceID.empty()) {
			nlohmann::json jsonObject;

			jsonObject[kESDSDKCommonEvent]   = kESDSDKEventSwitchToProfile;
			jsonObject[kESDSDKCommonContext] = m_pluginUUID;
			jsonObject[kESDSDKCommonDevice]  = deviceID;

			if (!profileName.empty()) {
				nlohmann::json payload;
				payload[kESDSDKPayloadProfile]   = profileName;
				jsonObject[kESDSDKCommonPayload] = payload;
			}

			websocketpp::lib::error_code ec;
			m_websocket.send(m_connectionHandle, jsonObject.dump(), websocketpp::frame::opcode::text, ec);
		}
	}

	void ConnectionManager::api_logMessage(const std::string &message) {
		if (!message.empty()) {
			nlohmann::json jsonObject;

			jsonObject[kESDSDKCommonEvent] = kESDSDKEventLogMessage;

			nlohmann::json payload;
			payload[kESDSDKPayloadMessage]   = message;
			jsonObject[kESDSDKCommonPayload] = payload;

			websocketpp::lib::error_code ec;
			m_websocket.send(m_connectionHandle, jsonObject.dump(), websocketpp::frame::opcode::text, ec);
		}
	}

}; // namespace StreamDeckIntegration
}; // namespace Mumble
