#ifndef MESSAGETYPE_H
#define MESSAGETYPE_H

#include <stdint.h>

static constexpr auto CONNECTION_TYPE = uint8_t(1);
static constexpr auto SEND_BY_ID = uint8_t(2);
static constexpr auto END_SEND_BY_ID = uint8_t(3);
static constexpr auto START_SEND_BY_ID = uint8_t(4);
static constexpr auto SEND_STATE = uint8_t(5);
static constexpr auto LOAD_STATES = uint8_t(6);
static constexpr auto SET_SIMDATA = uint8_t(7);
// defines for QML
static constexpr auto QML_STRING = uint8_t(1);
static constexpr auto QML_ZIP = uint8_t(2);
static constexpr auto RECEIVE_BY_CLIENT_ID = uint8_t(10);
static constexpr auto STATE_CHANGED = uint8_t(11);
static constexpr auto STATE_NOT_CHANGED = uint8_t(12);
static constexpr auto REQUEST_STATE_CHANGE = uint8_t(13);
static constexpr auto ZIP_UPLOAD = uint8_t(15);
static constexpr auto DATA_UNLOAD = uint8_t(14);


#endif // MESSAGETYPE_H
