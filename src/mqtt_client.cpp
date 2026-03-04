#include "mqtt_client.hpp"
#include "mqtt/client.h"

int MqttClient::connect()
{
    try
    {
        auto connOpts = mqtt::connect_options_builder()
                            .connect_timeout(TIMEOUT)
                            .clean_session()
                            .finalize();
        async_client.connect(connOpts)->wait();
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
    try
    {
        mqtt::topic top(async_client, topic, QOS);
        mqtt::token_ptr tok;
        tok = top.publish(payload);
        tok->wait();
    }
    catch (mqtt::exception &e)
    {
        while (!async_client.is_connected())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(250));
        }
    }
}

int MqttClient::disconnect(void)
{
    auto toks = async_client.get_pending_delivery_tokens();
    if (!toks.empty())
    {
        std::cout << "Error: There are pending delivery tokens!" << std::endl;
        return -1;
    }

    async_client.disconnect()->wait();
    std::cout << "MqttClient disconnected.\n"
              << std::endl;
    return 0;
}

int MqttClient::task_loop(void)
{
    int err_type = mqtt_client.connect();
    if (err_type != 0)
    {
        std::cerr << "Error connecting to mqtt server.." << std::endl;
        return -1;
    }
    std::cout << "Connected to mqtt server" << std::endl;
    while (1)
    {
        if (!async_client.is_connected())
        {
            std::cout << "Reconnecting..." << std::endl;
            async_client.reconnect();
        }
        std::this_thread::sleep_for(std::chrono::seconds(5));
    }
}
