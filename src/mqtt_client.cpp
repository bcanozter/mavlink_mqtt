#include "mqtt_client.hpp"
#include "mqtt/client.h"

const std::string DFLT_SERVER_URI{"mqtt://localhost:1883"};
MqttClient mqtt_client(DFLT_SERVER_URI, "mavlink_mqtt_client", mqtt::NO_PERSISTENCE);

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
