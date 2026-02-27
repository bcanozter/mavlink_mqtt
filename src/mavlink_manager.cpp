#include "mavlink_manager.hpp"
#include <bits/stdc++.h>
#include <unistd.h>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX_RECV 2048

int MavlinkManager::_init_socket(std::string address, int port)
{
    struct sockaddr_in addr;

    socket_fd = socket(PF_INET, SOCK_DGRAM, 0);
    if (socket_fd < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(address.c_str());

    if (bind(socket_fd, (struct sockaddr *)(&addr), sizeof(addr)) != 0)
    {
        printf("bind error: %s\n", strerror(errno));
        return -1;
    }
    return 0;
}
int MavlinkManager::initUDP(std::string address, int port)
{
    int err;
    err = _init_socket(address, port);
    if (err != 0)
    {
        printf("error init socket_fd\n");
        return -1;
    }

    // struct timeval tv;
    // tv.tv_sec = 0;
    // tv.tv_usec = 100000;
    // if (setsockopt(socket_fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
    //     printf("setsockopt error: %s\n", strerror(errno));
    //     return -3;
    // }
    struct sockaddr_in src_addr = {};
    socklen_t src_addr_len = sizeof(src_addr);
    bool src_addr_set = false;
    while (1)
    {
        receive_some(socket_fd, (struct sockaddr_in *)&src_addr, &src_addr_len, &src_addr_set);
        if (src_addr_set)
        {
            send_some(socket_fd, (struct sockaddr_in *)&src_addr, src_addr_len);
        }
    }
    return 0;
}

void MavlinkManager::receive_some(int socket_fd, struct sockaddr_in *src_addr, socklen_t *src_addr_len, bool *src_addr_set)
{

    char buffer[MAX_RECV];

    const int ret = recvfrom(socket_fd, buffer, MAX_RECV, MSG_WAITALL,
                             (struct sockaddr *)src_addr, src_addr_len);

    if (ret < 0)
    {
        printf("recvfrom error: %s\n", strerror(errno));
        return;
    }
    else if (ret == 0)
    {
        return;
    }
    *src_addr_set = true;
    mavlink_message_t message;
    mavlink_status_t status;
    for (int i = 0; i < ret; ++i)
    {
        if (mavlink_parse_char(MAVLINK_COMM_0, buffer[i], &message, &status) == 1)
        {

            // printf(
            //     "Received message %d from %d/%d\n",
            //     message.msgid, message.sysid, message.compid);

            switch (message.msgid)
            {
            case MAVLINK_MSG_ID_HEARTBEAT:
                handle_heartbeat(&message);
                break;
            }
        }
    }
}

void MavlinkManager::handle_heartbeat(const mavlink_message_t *message)
{
    mavlink_heartbeat_t heartbeat;
    mavlink_msg_heartbeat_decode(message, &heartbeat);

    printf("Got heartbeat from ");
    switch (heartbeat.autopilot)
    {
    case MAV_AUTOPILOT_GENERIC:
        printf("generic");
        break;
    case MAV_AUTOPILOT_ARDUPILOTMEGA:
        printf("ArduPilot");
        break;
    case MAV_AUTOPILOT_PX4:
        printf("PX4");
        break;
    default:
        printf("other");
        break;
    }
    printf(" autopilot\n");
}

void MavlinkManager::send_some(int socket_fd, const struct sockaddr_in *src_addr, socklen_t src_addr_len)
{
    static time_t last_time = 0;
    time_t current_time = time(NULL);
    if (current_time - last_time >= 1)
    {
        send_heartbeat(socket_fd, src_addr, src_addr_len);
        last_time = current_time;
    }
}

void MavlinkManager::send_heartbeat(int socket_fd, const struct sockaddr_in *src_addr, socklen_t src_addr_len)
{
    mavlink_message_t message;

    const uint8_t system_id = 69;
    const uint8_t base_mode = 0;
    const uint8_t custom_mode = 0;
    mavlink_msg_heartbeat_pack_chan(
        system_id,
        MAV_COMP_ID_PERIPHERAL,
        MAVLINK_COMM_0,
        &message,
        MAV_TYPE_GENERIC,
        MAV_AUTOPILOT_GENERIC,
        base_mode,
        custom_mode,
        MAV_STATE_STANDBY);

    uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
    const int len = mavlink_msg_to_send_buffer(buffer, &message);
    // printf("addr:%s  port:%u, len: %u\n", inet_ntoa(src_addr->sin_addr), src_addr->sin_port);
    int ret = sendto(socket_fd, buffer, len, MSG_CONFIRM, (const struct sockaddr *)src_addr, src_addr_len);
    if (ret != len)
    {
        printf("sendto error: %s\n", strerror(errno));
    }
    else
    {
        printf("Sent heartbeat\n");
    }
}

int MavlinkManager::initSerial(std::string device, int baud)
{
    return 0;
}
