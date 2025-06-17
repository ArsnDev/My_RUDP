#pragma once

class RUDP
{
	RUDP();
	~RUDP();

	bool create();
	bool close();
	bool bind();
	int sendTo(const char* buffer, int len, const sockaddr_in& to);
	int recvFrom(char* buffer, int len, sockaddr_in& from);
	void setNonBlock();

private:
	SOCKET sock;
};