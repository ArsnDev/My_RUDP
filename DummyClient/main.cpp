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
	}
	SOCKET sendSocket = INVALID_SOCKET;
\
	sendSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sendSocket == INVALID_SOCKET)
	{
		std::cout << "socket function failed with error : " << WSAGetLastError() << "\n";
		WSACleanup();
		return 1;
	}

	sockaddr_in serverAddr;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(9000);
	
	iResult = inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

	static unsigned int mySequenceNum = 0;

	std::cout << "Starting dummy client...\n";

	while (true)
	{
		MyPacket packetToSend;
		packetToSend.header.sequenceNum = mySequenceNum++;
		packetToSend.header.ackNum = 0;

		iResult = sendto(sendSocket, reinterpret_cast<char*>(&packetToSend), sizeof(packetToSend), 0, reinterpret_cast<SOCKADDR*>(&serverAddr), sizeof(serverAddr));
		if (iResult == SOCKET_ERROR)
		{
			std::cout << "sendto failed with error: " << WSAGetLastError() << "\n";
			closesocket(sendSocket);
			WSACleanup();
			return 1;
		}
		std::cout << "Packet Sent! Seq: " << packetToSend.header.sequenceNum - 1 << "\n";
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
	WSACleanup();
	return 0;
}