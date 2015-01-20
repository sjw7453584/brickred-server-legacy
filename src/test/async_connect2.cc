#include <stdint.h>
#include <cstdio>
#include <cstdlib>
#include <string>

#include <brickred/command_line_option.h>
#include <brickred/dynamic_buffer.h>
#include <brickred/io_service.h>
#include <brickred/socket_address.h>
#include <brickred/tcp_service.h>
#include <brickred/thread.h>

using namespace brickred;

class AsyncConnector {
public:
    AsyncConnector() :
        tcp_service_(io_service_),
        connect_timer_(-1),
        max_conn_count_(0), conn_count_(0),
        conn_count_once_(0), conn_delay_ms_(0),
        timeout_(0),
        verbose_(false)
    {
        tcp_service_.setNewConnectionCallback(BRICKRED_BIND_MEM_FUNC(
            &AsyncConnector::onNewConnection, this));
        tcp_service_.setRecvMessageCallback(BRICKRED_BIND_MEM_FUNC(
            &AsyncConnector::onRecvMessage, this));
        tcp_service_.setPeerCloseCallback(BRICKRED_BIND_MEM_FUNC(
            &AsyncConnector::onPeerClose, this));
        tcp_service_.setErrorCallback(BRICKRED_BIND_MEM_FUNC(
            &AsyncConnector::onError, this));
    }

    ~AsyncConnector()
    {
        if (connect_timer_ != -1) {
            io_service_.stopTimer(connect_timer_);
        }
    }

    void run(const SocketAddress &addr, int max_conn_count,
             int conn_count_once, int conn_delay_ms, int timeout,
             bool verbose)
    {
        addr_ = addr;
        max_conn_count_ = max_conn_count;
        conn_count_once_ = conn_count_once;
        conn_delay_ms_ = conn_delay_ms;
        timeout_ = timeout;
        verbose_ = verbose;

        connect();
        connect_timer_ = io_service_.startTimer(1000,
            BRICKRED_BIND_MEM_FUNC(&AsyncConnector::onTimer, this));

        io_service_.loop();
    }

private:
    void connect()
    {
        if (verbose_) {
            ::printf("connection_alive(%d/%d)\n",
                conn_count_, max_conn_count_);
        }

        int count = 0;

        for (int i = conn_count_; i < max_conn_count_; ++i)
        {
            bool complete = false;
            if (tcp_service_.asyncConnect(addr_,
                                          &complete, timeout_) >= 0) {
                if (complete) {
                    ++conn_count_;
                }
            }

            ++count;
            if (count == conn_count_once_) {
                count = 0;
                this_thread::sleepFor(conn_delay_ms_);
            }
        }
    }

    void onTimer(int64_t timer_id)
    {
        connect();
    }

    void onNewConnection(TcpService *service,
                         TcpService::SocketId from_socket_id,
                         TcpService::SocketId socket_id)
    {
        ++conn_count_;
    }

    void onRecvMessage(TcpService *service,
                       TcpService::SocketId socket_id,
                       DynamicBuffer *buffer)
    {
        buffer->read(buffer->readableBytes());
    }

    void onPeerClose(TcpService *service,
                     TcpService::SocketId socket_id)
    {
        service->closeSocket(socket_id);
        --conn_count_;
    }

    void onError(TcpService *service,
                 TcpService::SocketId socket_id,
                 int error)
    {
        service->closeSocket(socket_id);
        --conn_count_;
    }

private:
    IOService io_service_;
    TcpService tcp_service_;
    SocketAddress addr_;
    int64_t connect_timer_;
    int max_conn_count_;
    int conn_count_;
    int conn_count_once_;
    int conn_delay_ms_;
    int timeout_;
    bool verbose_;
};

static void printUsage(const char *progname)
{
    ::fprintf(stderr, "usage: %s <ip> <port>\n"
              "[-c <conn_count_once>] [-d conn_delay_ms]\n"
              "[-n <max_conn_count>] [-t <timeout>]\n",
              progname);
}

int main(int argc, char *argv[])
{
    std::string ip;
    uint16_t port;
    int max_conn_count;
    int conn_count_once = 50;
    int conn_delay_ms = 20;
    int timeout = 5000;
    bool verbose = false;

    CommandLineOption options;
    options.addOption("c", CommandLineOption::ParameterType::REQUIRED);
    options.addOption("d", CommandLineOption::ParameterType::REQUIRED);
    options.addOption("n", CommandLineOption::ParameterType::REQUIRED);
    options.addOption("t", CommandLineOption::ParameterType::REQUIRED);
    options.addOption("v");

    if (options.parse(argc, argv) == false) {
        printUsage(argv[0]);
        return -1;
    }
    if (options.hasOption("c")) {
        conn_count_once = ::atoi(options.getParameter("c").c_str());
    }
    if (options.hasOption("d")) {
        conn_delay_ms = ::atoi(options.getParameter("d").c_str());
    }
    if (options.hasOption("n")) {
        max_conn_count = ::atoi(options.getParameter("n").c_str());
    }
    if (options.hasOption("t")) {
        timeout = ::atoi(options.getParameter("t").c_str());
    }
    if (options.hasOption("v")) {
        verbose = true;
    }

    if (options.getLeftArguments().size() != 3) {
        printUsage(argv[0]);
        return -1;
    }

    ip = options.getLeftArguments()[0];
    port = ::atoi(options.getLeftArguments()[1].c_str());
    max_conn_count = ::atoi(options.getLeftArguments()[2].c_str());

    AsyncConnector connector;
    connector.run(SocketAddress(ip, port), max_conn_count,
                  conn_count_once, conn_delay_ms, timeout, verbose);

    return 0;
}
