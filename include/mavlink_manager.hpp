#pragma once

#include <string>
#include <sys/socket.h>
#include <mavlink/common/mavlink.h>
#include <unordered_map>
#include <memory>

#include <vehicle.hpp>
class MavlinkManager
{
private:
    int socket_fd;
    int _init_socket(std::string address, int port);

public:
    int initUDP(std::string address, int port);
    int initSerial(std::string device, int baud);
    void receive_some(int socket_fd, struct sockaddr_in *src_addr, socklen_t *src_addr_len, bool *src_addr_set);
    void send_some(int socket_fd, const struct sockaddr_in *src_addr, socklen_t src_addr_len);
    /* Send */
    void send_heartbeat(int socket_fd, const struct sockaddr_in *src_addr, socklen_t src_addr_len);

    void handle_mavlink_message(const mavlink_message_t *msg);
    std::unordered_map<uint8_t, std::unique_ptr<Vehicle>> vehicles;
};