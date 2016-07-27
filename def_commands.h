#ifndef DEF_COMMANDS_H
#define DEF_COMMANDS_H

#include <QString>

enum serial_commands {
    COMMAND_READY = 0, // 0
    COMMAND_DONE,
    COMMAND_TOGGLE_LED,
    COMMAND_ECHO_DATA,
};

enum serial_responses {
    RESPONSE_ERROR = 0, // 0
    RESPONSE_DATA,
    RESPONSE_READY,
    RESPONSE_GO,
    RESPONSE_DONE,
    RESPONSE_ECHO_DATA,
};

const QString GO = "GO;";
const QString READY = "RDY;";
const QString DONE = "DONE;";

serial_responses previousResponse=RESPONSE_ERROR;

#endif // DEF_COMMANDS_H
