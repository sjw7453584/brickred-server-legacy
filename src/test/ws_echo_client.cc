#include <cerrno>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <string>

#include "test/test_util.h"
#include <brickred/dynamic_buffer.h>
#include <brickred/io_service.h>
#include <brickred/random.h>
#include <brickred/socket_address.h>
#include <brickred/tcp_service.h>
#include <brickred/protocol/web_socket_protocol.h>

using namespace brickred;
using namespace brickred::protocol;

class WsEchoClient {
public:
    WsEchoClient() : tcp_service_(io_service_), socket_id_(-1)
    {
        tcp_service_.setRecvMessageCallback(BRICKRED_BIND_MEM_FUNC(
            &WsEchoClient::recvMessageCallback, this));
        tcp_service_.setPeerCloseCallback(BRICKRED_BIND_MEM_FUNC(
            &WsEchoClient::peerCloseCallback, this));
        tcp_service_.setErrorCallback(BRICKRED_BIND_MEM_FUNC(
            &WsEchoClient::errorCallback, this));
    }

    ~WsEchoClient()
    {
    }

    bool echo(const SocketAddress &addr, const std::string &host,
              const std::string &message)
    {
        socket_id_ = tcp_service_.connect(addr);
        if (-1 == socket_id_) {
            printf("[error] socket connect failed: %s\n", strerror(errno));
            return false;
        }

        message_ = message;

        protocol_.setHandshakeHeader("Host", host);
        protocol_.startAsClient(tcp_service_, socket_id_,
                                random_generator_);

        io_service_.loop();

        return true;
    }

    void recvMessageCallback(TcpService *service,
                             TcpService::SocketId socket_id,
                             DynamicBuffer *buffer)
    {
        for (;;) {
            WebSocketProtocol::RetCode::type ret =
                protocol_.recvMessage(buffer);
            if (WebSocketProtocol::RetCode::CONNECTION_ESTABLISHED == ret) {
                protocol_.sendMessage(message_.c_str(), message_.size());

            } else if (WebSocketProtocol::RetCode::WAITING_MORE_DATA == ret) {
                return;

            } else if (WebSocketProtocol::RetCode::ERROR == ret) {
                printf("[error] recieve Message failed\n");
                quit();
                break;

            } else if (WebSocketProtocol::RetCode::PEER_CLOSED == ret) {
                printf("[ws peer close]\n");
                quit();
                break;

            } else if (WebSocketProtocol::RetCode::MESSAGE_READY == ret) {
                DynamicBuffer message;
                if (protocol_.retrieveMessage(&message) == false) {
                    printf("[error] retrieve message failed\n");
                    quit();
                    break;
                }
                printf("[recieve message] %zd bytes\n",
                       message.readableBytes());
                test::hexdump(message.readBegin(), message.readableBytes());
                protocol_.sendPingFrame();
                protocol_.sendCloseFrame();

            } else if (WebSocketProtocol::RetCode::PONG_FRAME == ret) {
                printf("[recieve pong]\n");

            } else if (WebSocketProtocol::RetCode::PING_FRAME == ret) {
                printf("[recieve ping]\n");
            }
        }
    }

    void peerCloseCallback(TcpService *service,
                           TcpService::SocketId socket_id)
    {
        printf("[peer close] %lx\n", socket_id);
        quit();
    }

    void errorCallback(TcpService *service,
                       TcpService::SocketId socket_id,
                       int error)
    {
        printf("[error] %lx: %s\n", socket_id, strerror(error));
        quit();
    }

    void quit()
    {
        tcp_service_.closeSocket(socket_id_);
        io_service_.quit();
    }

private:
    IOService io_service_;
    TcpService tcp_service_;
    Random random_generator_;
    TcpService::SocketId socket_id_;
    WebSocketProtocol protocol_;
    std::string message_;
};

int main(int argc, char *argv[])
{
    if (argc < 4) {
        fprintf(stderr, "usage: %s <host> <port> <message>\n", argv[0]);
        return -1;
    }

    std::vector<SocketAddress> addr_list;
    SocketAddress::getAddressByDomain(argv[1], &addr_list);
    if (addr_list.empty()) {
        fprintf(stderr, "dns query failed\n");
        return -1;
    }

    WsEchoClient client;
    client.echo(SocketAddress(addr_list[0].getIp(),
        ::atoi(argv[2])), argv[1], argv[3]);

    return 0;
}

