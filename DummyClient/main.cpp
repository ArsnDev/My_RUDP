#include "pch.h"
#include "RUDP.h"
#pragma comment(lib, "Ws2_32.lib")

int main()
{
	WSADATA wsaData;
	int iResult = 0;

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
	{
		std::cout << "Error at WSAStartup()\n";
		return 1;
	}

	SOCKET clientSocket = INVALID_SOCKET;
	clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (clientSocket == INVALID_SOCKET)
	{
		std::cout << "socket function failed with error : " << WSAGetLastError() << "\n";
		WSACleanup();
		return 1;
	}

	unsigned long nonBlocking = 1;
	iResult = ioctlsocket(clientSocket, FIONBIO, &nonBlocking);
	if (iResult == SOCKET_ERROR)
	{
		std::cout << "ioctlsocket failed with error: " << WSAGetLastError() << std::endl;
		closesocket(clientSocket);
		WSACleanup();
		return 1;
	}

	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(9000);
	iResult = inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

	static unsigned int mySequenceNum = 0;
	char recvBuf[sizeof(MyPacket)];
	int bufLen = sizeof(MyPacket);

	std::cout << "Starting dummy client...\n";

	while (true)
	{
		MyPacket packetToSend;
		packetToSend.header.sequenceNum = mySequenceNum++;
		packetToSend.header.ackNum = 0;

		iResult = sendto(clientSocket, reinterpret_cast<char*>(&packetToSend), sizeof(packetToSend), 0, reinterpret_cast<SOCKADDR*>(&serverAddr), sizeof(serverAddr));
		if (iResult == SOCKET_ERROR)
		{
			std::cout << "sendto failed with error: " << WSAGetLastError() << "\n";
			closesocket(clientSocket);
			WSACleanup();
			return 1;
		}
		std::cout << "Packet Sent! Seq: " << packetToSend.header.sequenceNum << "\n";
		
		iResult = recvfrom(clientSocket, recvBuf, bufLen, 0, nullptr, nullptr);
		if (iResult > 0)
		{
			RUDPHeader* ackHeader = reinterpret_cast<RUDPHeader*>(recvBuf);
			std::cout << "  -> ACK Received for my Seq: " << ackHeader->ackNum
				<< " (Server Seq: " << ackHeader->sequenceNum << ")\n";
		}
		else if (iResult == SOCKET_ERROR)
		{
			int errorCode = WSAGetLastError();
			if (errorCode == WSAEWOULDBLOCK)
			{
			}
			else
			{
				std::cout << "recvfrom failed with error: " << errorCode << "\n";
			}
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	closesocket(clientSocket);
	WSACleanup();
	return 0;
}