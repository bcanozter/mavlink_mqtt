#include "mqtt_client.hpp"
#include "mqtt/client.h"

int MqttClient::connect()
{
    try
    {
        async_client.connect()->wait();
        return 0;
    }
    catch (const mqtt::exception &e)
    {
        std::cerr << "MQTT connect error: " << e.what() << std::endl;
        return -1;
    }
}

void MqttClient::publish(const std::string topic,
                         const std::string payload, int QOS)
{
    mqtt::topic top(async_client, topic, QOS);
    mqtt::token_ptr tok;
    tok = top.publish(payload);
    tok->wait();
}
