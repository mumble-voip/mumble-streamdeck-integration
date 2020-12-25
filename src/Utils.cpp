// Copyright 2021 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// source tree.

// The original version of this code was taken from https://github.com/elgatosf/streamdeck-cpu
// which was published under the MIT license.

#ifndef MUMBLE_STREAMDECK_INTEGRATION_UTILS_H_
#define MUMBLE_STREAMDECK_INTEGRATION_UTILS_H_

#include <string>

#include <nlohmann/json.hpp>

namespace Mumble {
namespace StreamDeckIntegration {
	namespace Utils {

		nlohmann::json getObjectByName(const nlohmann::json &json, const std::string &name) {
			// Check desired value exists
			nlohmann::json::const_iterator iter(json.find(name));
			if (iter == json.end()) {
				return {};
			}

			// Check value is an array
			if (!iter->is_object()) {
				return {};
			}

			// Return found object
			return *iter;
		}

		nlohmann::json getArrayByName(const nlohmann::json &json, const std::string &name) {
			// Check desired value exists
			nlohmann::json::const_iterator iter(json.find(name));
			if (iter == json.end()) {
				return {};
			}

			// Check value is an array
			if (!iter->is_array()) {
				return {};
			}

			// Return found array
			return *iter;
		}

		std::string getStringByName(const nlohmann::json &json, const std::string &name,
									const std::string &defaultValue) {
			// Check desired value exists
			nlohmann::json::const_iterator iter(json.find(name));
			if (iter == json.end()) {
				return defaultValue;
			}

			// Check value is a string
			if (!iter->is_string()) {
				return defaultValue;
			}

			// Return value
			return (*iter).get< std::string >();
		}

		std::string getString(const nlohmann::json &json, const std::string &defaultValue) {
			// Check value is a string
			if (!json.is_string()) {
				return defaultValue;
			}

			return json.get< std::string >();
		}

		bool getBoolByName(const nlohmann::json &json, const std::string &name, bool defaultValue) {
			// Check desired value exists
			nlohmann::json::const_iterator iter(json.find(name));
			if (iter == json.end()) {
				return defaultValue;
			}

			// Check value is a bool
			if (!iter->is_boolean()) {
				return defaultValue;
			}

			// Return value
			return (*iter).get< bool >();
		}

		int getIntByName(const nlohmann::json &json, const std::string &name, int defaultValue) {
			// Check desired value exists
			nlohmann::json::const_iterator iter(json.find(name));
			if (iter == json.end()) {
				return defaultValue;
			}

			// Check value is an integer
			if (!iter->is_number_integer()) {
				return defaultValue;
			}

			// Return value
			return (*iter).get< int >();
		}

		unsigned int getUnsignedIntByName(const nlohmann::json &json, const std::string &name,
										  unsigned int defaultValue) {
			// Check desired value exists
			nlohmann::json::const_iterator iter(json.find(name));
			if (iter == json.end()) {
				return defaultValue;
			}

			// Check value is an unsigned integer
			if (!iter->is_number_unsigned()) {
				return defaultValue;
			}

			// Return value
			return (*iter).get< unsigned int >();
		}

		float getFloatByName(const nlohmann::json &json, const std::string &name, float defaultValue) {
			// Check desired value exists
			nlohmann::json::const_iterator iter(json.find(name));
			if (iter == json.end()) {
				return defaultValue;
			}

			// Check value is an integer
			if (!iter->is_number_float() && !iter->is_number_integer()) {
				return defaultValue;
			}

			// Return value
			return (*iter).get< float >();
		}

	}; // namespace Utils
};     // namespace StreamDeckIntegration
};     // namespace Mumble

#endif // MUMBLE_STREAMDECK_INTEGRATION_UTILS_H_
