#include <vehicle.hpp>
#include <json.hpp>
#include <string>

#include "mqtt_client.hpp"
// https://github.com/nlohmann/json
#include "json.hpp"

using json = nlohmann::json;

void Vehicle::handle_message(const mavlink_message_t *msg)
{
    switch (msg->msgid)
    {
    case MAVLINK_MSG_ID_HEARTBEAT:
        handle_heartbeat(msg);
        break;

    case MAVLINK_MSG_ID_SYS_STATUS:
        handle_sys_status(msg);
        break;
    case MAVLINK_MSG_ID_ATTITUDE:
        handle_attitude(msg);
        break;
    }
}

void Vehicle::handle_attitude(const mavlink_message_t *message)
{
    mavlink_attitude_t attitude;
    mavlink_msg_attitude_decode(message, &attitude);
    memcpy(&this->attitude, &attitude, sizeof(mavlink_attitude_t));
    std::string topic = "vehicle/" + std::to_string(message->sysid) + "/attitude";
    json j = json{
        {"time_boot_ms", this->attitude.time_boot_ms},
        {"roll", this->attitude.roll},
        {"pitch", this->attitude.pitch},
        {"yaw", this->attitude.yaw},
        {"rollspeed", this->attitude.rollspeed},
        {"pitchspeed", this->attitude.pitchspeed},
        {"yawspeed", this->attitude.yawspeed},
    };
    std::string payload = j.dump();
    mqtt_client.publish(topic, payload, QOS);
}

void Vehicle::handle_sys_status(const mavlink_message_t *message)
{
    mavlink_sys_status_t sys_status;
    mavlink_msg_sys_status_decode(message, &sys_status);
    memcpy(&this->sys_status, &sys_status, sizeof(mavlink_sys_status_t));

    const uint16_t voltage_battery = (uint16_t)sys_status.voltage_battery;
    const int16_t current_battery = (int16_t)sys_status.current_battery;
    const int8_t battery_remaining = (int8_t)sys_status.battery_remaining;

    std::string topic = "vehicle/" + std::to_string(message->sysid) + "/sys_status";
    json j = json{
        {"voltage_battery", voltage_battery},
        {"current_battery", current_battery},
        {"battery_remaining", battery_remaining}};
    std::string payload = j.dump();
    mqtt_client.publish(topic, payload, QOS);
}

void Vehicle::handle_heartbeat(const mavlink_message_t *message)
{
    mavlink_heartbeat_t heartbeat;
    mavlink_msg_heartbeat_decode(message, &heartbeat);
    memcpy(&this->heartbeat, &heartbeat, sizeof(mavlink_heartbeat_t));

    std::string topic = "vehicle/" + std::to_string(message->sysid) + "/heartbeat";
    json j = json{
        {"autpilot", this->heartbeat.autopilot},
        {"base_mode", this->heartbeat.base_mode},
        {"custom_mode", this->heartbeat.custom_mode},
        {"system_status", this->heartbeat.system_status}};
    std::string payload = j.dump();
    mqtt_client.publish(topic, payload, QOS);
}
