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

	void makeOperation(SOCKET* _accept, OperationsCode* code, char* recvbuf, char* sendbuf, int buflen); // Выполнить операцию

private:
	char* _path;
	char* _mode;
	

	void RRQ(SOCKET* _accept, char* recvbuf, char* sendbuf, int buflen); // Запрос на чтение
	void WRQ(SOCKET* _accept, char* recvbuf, char* sendbuf, int buflen); // Запрос на запись
	void DATA(SOCKET* _accept, char* recvbuf, int buflen); // Получение данных
	bool ACC(SOCKET* _accept, char* recvbuf, int buflen); // Подтверждение
	const char* ERR(Error err); // Сообщение об ошибке
};

#endif
