#ifndef MESSAGETYPE_H
#define MESSAGETYPE_H

#include <stdint.h>

constexpr auto CONNECT_ID_MARK = uint8_t(1);
constexpr auto RECEIVE_BY_ID = uint8_t(2);
constexpr auto END_RECEIVE_BY_ID = uint8_t(3);
constexpr auto INIT_RECEIVE = uint8_t(4);
constexpr auto SEND_BY_ID = uint8_t(10);
constexpr auto STATE_CHANGED = uint8_t(11);
constexpr auto STATE_NOT_CHANGED = uint8_t(12);
constexpr auto VALUE_CHANGED = uint8_t(13);
constexpr auto VALUE_NOT_CHANGED = uint8_t(12);
constexpr auto QML_AS_ZIP = uint8_t(2);
constexpr auto QML_STRING = uint8_t(1);
constexpr auto UNLOAD_DATA = uint8_t(14);
constexpr auto LOAD_ZIP_REQUEST = uint8_t(15);
constexpr auto LOAD_STATES = uint8_t(6);
constexpr auto SET_SIMDATA = uint8_t(7);

#endif // MESSAGETYPE_H
