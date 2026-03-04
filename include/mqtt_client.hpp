#pragma once
#include "mqtt/async_client.h"
#include <string>

const auto TIMEOUT = std::chrono::seconds(10);
const int QOS = 1;

class callback : public virtual mqtt::callback
{
public:
    void connection_lost(const std::string &cause) override
    {
        std::cout << "\nConnection lost" << std::endl;
        if (!cause.empty())
        {
            std::cout << "\tcause: " << cause << std::endl;
        }
    }

    void delivery_complete(mqtt::delivery_token_ptr tok) override
    {
        //
    }
};

class MqttClient
{
private:
    mqtt::async_client async_client;
    std::string server_uri;
    std::string client_id;
    mqtt::persistence_type type;
    callback cb;

public:
    MqttClient(std::string serverURI, std::string clientId,
               mqtt::persistence_type persistence) : async_client(serverURI, clientId, persistence)
    {
        async_client.set_callback(cb);
    }
    int connect();
    void publish(const std::string topic,
                 const std::string payload, const int QOS);
    int disconnect(void);
    int task_loop(void);
};
extern MqttClient mqtt_client;