#pragma once

#include <string>
#include <sys/socket.h>
#include <mavlink/v2.0/common/mavlink.h>
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
    /* Message Handlers*/
    void handle_heartbeat(const mavlink_message_t *message);
    void handle_sys_status(const mavlink_message_t *message);
    /* Send */
    void send_heartbeat(int socket_fd, const struct sockaddr_in *src_addr, socklen_t src_addr_len);
};