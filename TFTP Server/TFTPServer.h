#pragma once
#ifndef TFTPSERVER_H
#define TFTPSERVER_H

#include <iostream>
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <thread>
#include <mutex>
#include "Error.h"
#include "Operations.h"

#pragma comment(lib, "Ws2_32.lib") // ��������� dll

#define DEFAULT_BUFLEN 512 

class TFTPServer
{
public:
	TFTPServer();
	int start(PCSTR host, PCSTR port); // ������ �������, ����������� �������, ���������, ���������� � ��������
private:
	SOCKET _accept; // ������������� ��������������� �������
	int clientHandler(); // ������������ ������� � ��������
	OperationsCode packageParsing(char * recvbuf);
	int sendPackage(char * sendbuf, int sendbuflen);
	int recvPackage(char * recvbuf, int recvbuflen);
};

#endif