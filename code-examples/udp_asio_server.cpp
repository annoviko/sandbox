#include <stdio.h>


#include <boost/thread.hpp>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>


enum class EFaxStatReq : unsigned char
{
	GetLimitSipResources,
	GetFreeSipResources
};


class FaxStatServer
{
private:
    boost::asio::io_service         m_ioService;
    boost::thread                   m_srvThread;
    boost::asio::ip::udp::socket    m_udpSocket;
    boost::asio::ip::udp::endpoint  m_endPoint;
    boost::array<char, 1>           m_rcvBuffer;

public:
    FaxStatServer(void) :
        m_ioService(),
        m_udpSocket(m_ioService, boost::asio::ip::udp::endpoint(boost::asio::ip::udp::v4(), 45001))
    {
    }

public:
    void Start(void)
    {
        m_srvThread = boost::thread(boost::bind(&boost::asio::io_service::run, boost::ref(m_ioService)));
        StartReceive();
    }

    void Stop(void)
    {
        m_ioService.stop();
        m_udpSocket.cancel();
        m_udpSocket.close();
        m_srvThread.join();
        
        printf("Fax Statistic Server is terminated.\n");
    }

private:
    void StartReceive(void)
    {
        m_udpSocket.async_receive_from(
            boost::asio::buffer(m_rcvBuffer), m_endPoint,
            boost::bind(&FaxStatServer::ReceiverProcessor, this,
                boost::asio::placeholders::error,
                boost::asio::placeholders::bytes_transferred));
    }

    void ReceiverProcessor(const boost::system::error_code & error, std::size_t bytes_transferred)
    {
        if (error && error != boost::asio::error::message_size)
        {
            printf("Fax Statistic Server receive error (code: '%d').\n", error.value());
        }
        
        EFaxStatReq requestType = (EFaxStatReq) m_rcvBuffer.at(0);
        
        boost::array<unsigned long, 1> sndBuffer;
        sndBuffer[0] = (unsigned long) -1;

        switch(requestType)
        {
            case EFaxStatReq::GetLimitSipResources:
                sndBuffer[0] = 1000;
                break;
            
            case EFaxStatReq::GetFreeSipResources:
                sndBuffer[0] = 50000;
                break;

            default:
                printf("Unknown request '%d' is received.\n", (char) requestType);
                break;
        }
        
        m_udpSocket.async_send_to(boost::asio::buffer(sndBuffer), m_endPoint,
          boost::bind(&FaxStatServer::SenderProcessor, this, sndBuffer[0],
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));
    }

    void SenderProcessor(const unsigned long cntrValue, const boost::system::error_code& ec, std::size_t bytes_transferred)
    {
        StartReceive();
    }
};




#include <chrono>
#include <thread>

int main(int argc, char *argv[])
{
	std::cout << "[Main] Fax Server is created." << std::endl;

	FaxStatServer server;
	
	std::cout << "[Main] Fax Server is started." << std::endl;
	
	server.Start();
	
	for (int i = 0; i < 3; i++)
	{
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	
	server.Stop();
	std::cout << "[Main] Fax Server is terminated." << std::endl;
	
	return 0;
}

