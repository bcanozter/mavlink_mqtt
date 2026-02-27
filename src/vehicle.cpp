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
    }
}

void Vehicle::handle_sys_status(const mavlink_message_t *message)
{
    mavlink_sys_status_t sys_status;
    mavlink_msg_sys_status_decode(message, &sys_status);
    std::string topic = "vehicle/" + std::to_string(message->sysid) + "/sys_status";
    json j = json{
        {"voltage_battery", (uint16_t)sys_status.voltage_battery},
        {"current_battery", (int16_t)sys_status.current_battery},
        {"battery_remaining", (int8_t)sys_status.battery_remaining}};
    std::string payload = j.dump();
    mqtt_client.publish(topic, payload, QOS);
}

void Vehicle::handle_heartbeat(const mavlink_message_t *message)
{
    mavlink_heartbeat_t heartbeat;
    mavlink_msg_heartbeat_decode(message, &heartbeat);

    switch (heartbeat.autopilot)
    {
    case MAV_AUTOPILOT_GENERIC:
        break;
    case MAV_AUTOPILOT_ARDUPILOTMEGA:
        break;
    case MAV_AUTOPILOT_PX4:
        break;
    default:
        break;
    }
}
