#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>

#include "test/test_util.h"
#include <brickred/dynamic_buffer.h>
#include <brickred/io_service.h>
#include <brickred/socket_address.h>
#include <brickred/string_util.h>
#include <brickred/tcp_service.h>
#include <brickred/protocol/http_protocol.h>
#include <brickred/protocol/http_request.h>
#include <brickred/protocol/http_response.h>

using namespace brickred;
using namespace brickred::protocol;

class HttpClient {
public:
    HttpClient() : tcp_service_(io_service_), socket_id_(-1)
    {
        tcp_service_.setRecvMessageCallback(BRICKRED_BIND_MEM_FUNC(
            &HttpClient::recvMessageCallback, this));
        tcp_service_.setPeerCloseCallback(BRICKRED_BIND_MEM_FUNC(
            &HttpClient::peerCloseCallback, this));
        tcp_service_.setErrorCallback(BRICKRED_BIND_MEM_FUNC(
            &HttpClient::errorCallback, this));
    }

    ~HttpClient()
    {
    }

    bool request(const SocketAddress &addr, const std::string &request_uri)
    {
        socket_id_ = tcp_service_.connect(addr);
        if (-1 == socket_id_) {
            printf("[error] socket connect failed: %s\n", strerror(errno));
            return false;
        }

        sendHttpRequest(addr, request_uri);
        io_service_.loop();

        return true;
    }

    void sendHttpRequest(const SocketAddress &addr,
                         const std::string &request_uri)
    {
        HttpRequest request;
        request.setVersion(HttpMessage::Version::HTTP_1_1);
        request.setMethod(HttpRequest::Method::GET);
        request.setRequestUri(request_uri);
        request.setHeader("Host",
            addr.getIp() + ":" + string_util::toString(addr.getPort()));
        request.setHeader("User-Agent", "brickred http client");
        request.setHeader("Connection", "keep-alive");

        DynamicBuffer buffer;
        HttpProtocol::writeMessage(request, &buffer);

        tcp_service_.sendMessage(socket_id_,
            buffer.readBegin(), buffer.readableBytes());
    }

    void printHttpResponse(const HttpResponse &response)
    {
        printf("[recv http response] %d %s\n",
               response.getStatusCode(),
               response.getReasonPhrase().c_str());

        // print header
        printf("  header:\n");
        for (HttpMessage::HeaderMap::const_iterator iter =
                 response.getHeaders().begin();
             iter != response.getHeaders().end(); ++iter) {
            printf("    %s: %s\n",
                   iter->first.c_str(), iter->second.c_str());
        }
        // print body
        printf("  body:\n");
        test::hexdump(response.getBody().c_str(),
                      response.getBody().size());
    }

    void recvMessageCallback(TcpService *service,
                             TcpService::SocketId socket_id,
                             DynamicBuffer *buffer)
    {
        HttpProtocol::RetCode::type ret = protocol_.recvMessage(buffer);
        if (HttpProtocol::RetCode::WAITING_MORE_DATA == ret) {
            return;

        } else if (HttpProtocol::RetCode::ERROR == ret) {
            printf("[error] recieve Message failed\n");
            quit();

        } else if (HttpProtocol::RetCode::MESSAGE_READY == ret) {
            HttpResponse response;
            if (protocol_.retrieveResponse(&response) == false) {
                printf("[error] retrieve response failed\n");
                quit();
            }
            printHttpResponse(response);
            quit();
            
        } else {
            quit();
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
    TcpService::SocketId socket_id_;
    HttpProtocol protocol_;
};

int main(int argc, char *argv[])
{
    if (argc < 4) {
        fprintf(stderr, "usage: %s <ip> <port> <request_uri>\n", argv[0]);
        return -1;
    }

    HttpClient client;
    client.request(SocketAddress(argv[1], ::atoi(argv[2])), argv[3]);

    return 0;
}

