#pragma once
class IClient
{
private:
	int socket;
public:
	virtual int GetSocket() = 0;
	virtual void SetSocket(int _socket) = 0;
	virtual void Act(char message[]) = 0;
};