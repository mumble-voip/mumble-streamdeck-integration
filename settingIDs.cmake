# Copyright 2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# source tree.

set(MUMBLE_STREAMDECK_CHANNEL_JOIN_ACTION_CHANNEL_NAME_SETTING "channelJoin_channelName")
set(MUMBLE_STREAMDECK_CHANNEL_JOIN_ACTION_CHANNEL_PASSWORD_SETTING "channelJoin_channelPassword")

set(MUBMLE_STREAMDECK_SETTINGS "")
list(APPEND MUBMLE_STREAMDECK_SETTINGS "MUMBLE_STREAMDECK_CHANNEL_JOIN_ACTION_CHANNEL_NAME_SETTING")
list(APPEND MUBMLE_STREAMDECK_SETTINGS "MUMBLE_STREAMDECK_CHANNEL_JOIN_ACTION_CHANNEL_PASSWORD_SETTING")

# create include file for CXX code
file(WRITE "${CXX_SETTINGS_INCLUDE_FILE}" "#ifndef SETTING_IDS_H_\n#define SETTING_IDS_H_\n")
foreach(CURRENT_SETTING IN LISTS MUBMLE_STREAMDECK_SETTINGS)
	file(APPEND "${CXX_SETTINGS_INCLUDE_FILE}" "constexpr const char *${CURRENT_SETTING} = \"${${CURRENT_SETTING}}\";\n")
endforeach()
file(APPEND "${CXX_SETTINGS_INCLUDE_FILE}" "#endif\n")
