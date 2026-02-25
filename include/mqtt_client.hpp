#pragma once
#include "mqtt/async_client.h"
#include <string>
class MqttClient
{
private:
    mqtt::async_client async_client;
    std::string server_uri;
    std::string client_id;
    mqtt::persistence_type type;

public:
    MqttClient(std::string serverURI, std::string clientId,
               mqtt::persistence_type persistence) : async_client(serverURI, clientId, persistence)
    {
    }
    int connect();
    void publish(const std::string topic,
                 const std::string payload, const int QOS);
};