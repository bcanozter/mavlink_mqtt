#pragma once
#include <mavlink/common/mavlink.h>

class Vehicle
{
public:
    uint8_t sysid;
    uint8_t compid;

    Vehicle(uint8_t s, uint8_t c) : sysid(s), compid(c) {}
    /* Message Handlers*/
    void handle_heartbeat(const mavlink_message_t *message);
    void handle_sys_status(const mavlink_message_t *message);
    void handle_message(const mavlink_message_t *msg);
};