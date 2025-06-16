#include "pch.h"
#include "RUDP.h"
#pragma comment(lib, "Ws2_32.lib")

int main()
{
	WSADATA wsaData;
	int iResult = 0;

	SOCKET recvSocket = INVALID_SOCKET;

	char recvBuf[sizeof(MyPacket)];
	int bufLen = sizeof(MyPacket);

	sockaddr_in recvAddr;
	sockaddr_in senderAddr;
	int senderAddrSize = sizeof(senderAddr);

	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != NO_ERROR)
	{
		std::cout << "Error at WSAStartup()\n";
	}

	recvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (recvSocket == INVALID_SOCKET)
	{
		std::cout << "socket function failed with error : " << WSAGetLastError() << "\n";
		WSACleanup();
		return 1;
	}

	recvAddr.sin_family = AF_INET;
	recvAddr.sin_port = htons(9000);
	
	char reuseAddr = 1;
	iResult = setsockopt(recvSocket, SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof(reuseAddr));
	if (iResult == SOCKET_ERROR)
	{
		std::cout << "setsockopt for SO_REUSEADDR failed with error: " << WSAGetLastError() << "\n";
	}
	iResult = inet_pton(AF_INET, "127.0.0.1", &recvAddr.sin_addr);
	iResult = bind(recvSocket, reinterpret_cast<SOCKADDR*>(&recvAddr), sizeof(recvAddr));
	if (iResult == SOCKET_ERROR)
	{
		std::cout << "bind failed with error " << WSAGetLastError() << "\n";
		closesocket(recvSocket);
		WSACleanup();
		return 1;
	}
	else
	{
		std::cout << "bind successed\n";
	}

	static unsigned int serverSequenceNum = 0;

	while (true)
	{
		iResult = recvfrom(recvSocket, recvBuf, bufLen, 0, reinterpret_cast<SOCKADDR*>(&senderAddr), &senderAddrSize);
		if (iResult == SOCKET_ERROR)
		{
			std::cout << "recvfrom failed " << WSAGetLastError() << "\n";
		}

		RUDPHeader* recvHeader = reinterpret_cast<RUDPHeader*>(recvBuf);
		std::cout << "Packet Recved Seq: " << recvHeader->sequenceNum << ", Ack: " << recvHeader->ackNum << "\n";
		unsigned int seqToAck = recvHeader->sequenceNum;

		RUDPHeader sendHeader;
		sendHeader.sequenceNum = serverSequenceNum++;
		sendHeader.ackNum = seqToAck;

		sendto(recvSocket, reinterpret_cast<char*>(&sendHeader), sizeof(sendHeader), 0, reinterpret_cast<SOCKADDR*>(&senderAddr), sizeof(senderAddr));
		std::cout << " -> Sent ACK for Seq: " << seqToAck << "\n";
	}

	WSACleanup();
	return 0;
}