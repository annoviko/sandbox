#include <iostream>
#include <stdio.h>
#include <thread>

#include <future>
#include <chrono>

#include <boost/thread.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/use_future.hpp>


enum class EFaxStatReq : unsigned char
{
    GetLimitSipResources,
    GetFreeSipResources
};


class FaxStatCollector
{
private:
    boost::asio::io_service         m_ioService;
    boost::asio::io_service::work   m_ioWork;
    std::thread                     m_ioThread;

    boost::asio::ip::udp::socket    m_udpSocket;

public:
    FaxStatCollector(void) :
        m_ioService(),
        m_ioWork(m_ioService),
        m_ioThread([&](){ m_ioService.run(); }),
        m_udpSocket(m_ioService)
    {
        m_udpSocket.open(boost::asio::ip::udp::v4());
    }

    ~FaxStatCollector(void)
    {
        m_udpSocket.close();
        
        m_ioService.stop();
        m_ioThread.join();
    }

public:
    unsigned long GetLimitSipResources(void)
    {
        return GetBmTamServerCounterValue(EFaxStatReq::GetLimitSipResources);
    }

    unsigned long GetFreeSipResources(void)
    {
        return GetBmTamServerCounterValue(EFaxStatReq::GetFreeSipResources);
    }

private:
    void SendRequest(const EFaxStatReq counterType)
    {
        boost::asio::ip::udp::resolver resolver(m_ioService);
        boost::asio::ip::udp::resolver::query query(boost::asio::ip::udp::v4(), "127.0.0.1", "45001");
        
        boost::asio::ip::udp::endpoint receiverEndpoint = *resolver.resolve(query);

        boost::array<char, 1> sndBuffer  = { (char) counterType };
        m_udpSocket.send_to(boost::asio::buffer(sndBuffer), receiverEndpoint);
    }

    unsigned long ReceiveResponse(void)
    {
        boost::array<unsigned long, 1> rcvBuffer = { 0 };
        boost::asio::ip::udp::endpoint senderEndpoint;
        
        std::future<std::size_t> readResult = m_udpSocket.async_receive_from(
            boost::asio::buffer(rcvBuffer), senderEndpoint, 0, boost::asio::use_future);

        if (readResult.wait_for(std::chrono::milliseconds(20)) == std::future_status::timeout)
        {
            printf("Impossible to read counter value due to timeout.\n");
            m_udpSocket.cancel();
        }

        
        return rcvBuffer.at(0);
    }

    unsigned long GetBmTamServerCounterValue(const EFaxStatReq counterType)
    {
        SendRequest(counterType);
        return ReceiveResponse();
    }
};


int main(int argc, char *argv[])
{
    FaxStatCollector collector;

    while(true)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        printf("Limit: '%d', Free: '%d'.\n", (int) collector.GetLimitSipResources(), (int) collector.GetFreeSipResources());
    }

    return 0;
}
