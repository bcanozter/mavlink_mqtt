#pragma once
#include <mavlink/common/mavlink.h>

class Vehicle
{
public:
    // from the mavlink_messsage
    uint8_t sysid;
    uint8_t compid;
    // heartbeat

    mavlink_heartbeat_t heartbeat;
    mavlink_sys_status_t sys_status;
    mavlink_attitude_t attitude;

    Vehicle(uint8_t s, uint8_t c) : sysid(s), compid(c) {}

    /* Message Handlers*/
    void handle_heartbeat(const mavlink_message_t *message);
    void handle_sys_status(const mavlink_message_t *message);
    void handle_message(const mavlink_message_t *msg);
    void handle_attitude(const mavlink_message_t *msg);
};