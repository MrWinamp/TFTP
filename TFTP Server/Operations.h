#pragma once
#ifndef OPERATIONS_H
#define OPERATIONS_H
#include "OperationsCode.h"
#include "Error.h"
#include <WinSock2.h>
#include <iostream>
#include <fstream>
#include <io.h>

class Operations
{
public:
	Operations();
	~Operations();

	void makeOperation(SOCKET* _accept, OperationsCode* code, char* recvbuf, char* sendbuf, int buflen); // ��������� ��������

private:
	char* _path;
	char* _mode;
	

	void RRQ(SOCKET* _accept, char* recvbuf, char* sendbuf, int buflen); // ������ �� ������
	void WRQ(SOCKET* _accept, char* recvbuf, char* sendbuf, int buflen); // ������ �� ������
	void DATA(SOCKET* _accept, char* recvbuf, int buflen); // ��������� ������
	bool ACC(SOCKET* _accept, char* recvbuf, int buflen); // �������������
	const char* ERR(Error err); // ��������� �� ������
};

#endif
