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
		/**
		 * Get object by name
		 *
		 * @param json The JSON struct to operate on
		 * @param name The name of the object that shall be extracted
		 * @returns The found JSON object or a null object if no object of that name was found
		 */
		nlohmann::json getObjectByName(const nlohmann::json &json, const std::string &name);

		/**
		 * Get array by name
		 *
		 * @param json The JSON struct to operate on
		 * @param name The name of the array that shall be extracted
		 * @returns The found JSON object or a null object if no object of that name was found
		 */
		nlohmann::json GetArrayByName(const nlohmann::json &json, const std::string &name);

		/**
		 * Get string by name
		 *
		 * @param json The JSON struct to operate on
		 * @param name The name of the array that shall be extracted
		 * @param defaultValue The value to return in case this function fails
		 * @returns The found string or the provided defaultValue if no string of that name could be found
		 */
		std::string getStringByName(const nlohmann::json &json, const std::string &name,
									const std::string &defaultValue = "");

		/**
		 * Get string
		 *
		 * @param json The JSON struct to operate on
		 * @param defaultValue The value to return in case this function fails
		 * @returns The found string or the provided defaultValue if no string could be found
		 */
		std::string getString(const nlohmann::json &json, const std::string &defaultValue = "");

		/**
		 * Get bool by name
		 *
		 * @param json The JSON struct to operate on
		 * @param name The name of the array that shall be extracted
		 * @param defaultValue The value to return in case this function fails
		 * @returns The found value or the provided defaultValue if no bool of that name could be found
		 */
		bool getBoolByName(const nlohmann::json &json, const std::string &name, bool defaultValue = false);

		/**
		 * Get integer by name
		 *
		 * @param json The JSON struct to operate on
		 * @param name The name of the array that shall be extracted
		 * @param defaultValue The value to return in case this function fails
		 * @returns The found value or the provided defaultValue if no integer of that name could be found
		 */
		int getIntByName(const nlohmann::json &json, const std::string &name, int defaultValue = 0);

		/**
		 * Get unsigned integer by name
		 *
		 * @param json The JSON struct to operate on
		 * @param name The name of the unsigned integer that shall be extracted
		 * @param defaultValue The value to return in case this function fails
		 * @returns The found value or the provided defaultValue if no unsigned integer of that name could be found
		 */
		unsigned int getUnsignedIntByName(const nlohmann::json &json, const std::string &name,
										  unsigned int defaultValue = 0);

		/**
		 * Get float by name
		 *
		 * @param json The JSON struct to operate on
		 * @param name The name of the float that shall be extracted
		 * @param defaultValue The value to return in case this function fails
		 * @returns The found value or the provided defaultValue if no float of that name could be found
		 */
		float getFloatByName(const nlohmann::json &json, const std::string &name, float defaultValue = 0.0);

	}; // namespace Utils
};     // namespace StreamDeckIntegration
};     // namespace Mumble

#endif // MUMBLE_STREAMDECK_INTEGRATION_UTILS_H_
