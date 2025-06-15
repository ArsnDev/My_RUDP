#pragma once

struct RUDPHeader
{
	unsigned int sequenceNum;
	unsigned int ackNum;
};
struct MyPacket
{
	RUDPHeader header;
	char message[1024]; // Real Data comes here
};

class RUDP
{
};