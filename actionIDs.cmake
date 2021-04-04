# Copyright 2021 The Mumble Developers. All rights reserved.
# Use of this source code is governed by a BSD-style license
# that can be found in the LICENSE file at the root of the
# source tree.

set(MUMBLE_STREAMDECK_TOGGLE_LOCAL_USER_MUTE_ACTION_UUID "info.mumble.mumble.actions.toggle-local-user-mute")
set(MUMBLE_STREAMDECK_TOGGLE_LOCAL_USER_DEAF_ACTION_UUID "info.mumble.mumble.actions.toggle-local-user-deaf")
set(MUMBLE_STREAMDECK_JOIN_CHANNEL_ACTION_UUID "info.mumble.mumble.actions.join-channel")

set(MUBMLE_STREAMDECK_ACTION_UUIDS "")
list(APPEND MUBMLE_STREAMDECK_ACTION_UUIDS "MUMBLE_STREAMDECK_TOGGLE_LOCAL_USER_MUTE_ACTION_UUID")
list(APPEND MUBMLE_STREAMDECK_ACTION_UUIDS "MUMBLE_STREAMDECK_TOGGLE_LOCAL_USER_DEAF_ACTION_UUID")
list(APPEND MUBMLE_STREAMDECK_ACTION_UUIDS "MUMBLE_STREAMDECK_JOIN_CHANNEL_ACTION_UUID")


# create include file for CXX code
file(WRITE "${CXX_ACTION_UUID_INCLUDE_FILE}" "#ifndef ACTION_UUIDS_H_\n#define ACTION_UUIDS_H_\n")
foreach(CURRENT_ID IN LISTS MUBMLE_STREAMDECK_ACTION_UUIDS)
	file(APPEND "${CXX_ACTION_UUID_INCLUDE_FILE}" "constexpr const char *${CURRENT_ID} = \"${${CURRENT_ID}}\";\n")
endforeach()
file(APPEND "${CXX_ACTION_UUID_INCLUDE_FILE}" "#endif\n")

