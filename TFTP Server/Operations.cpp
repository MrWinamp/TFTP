#include "Operations.h"

Operations::Operations()
{
	_path = new char[MAX_PATH];
	_mode = new char[MAX_PATH];
}

Operations::~Operations()
{
	delete [] _path;
	_path = nullptr;
	delete [] _mode;
	_mode = nullptr;
}

void Operations::makeOperation(SOCKET * _accept, OperationsCode * code, char* recvbuf, char * sendbuf, int buflen)
{
	switch (*code)
	{
	case OperationsCode::RRQ:
		RRQ(_accept, recvbuf, sendbuf, buflen); // ������ �� ������
		break;
	case OperationsCode::WRQ:
		WRQ(_accept, recvbuf, sendbuf, buflen);
		break;
	case OperationsCode::DATA:
		//DATA();
		break;
	case OperationsCode::ACC:
		//ACC();
		break;
	case OperationsCode::ERR:
		//ERR(); // ���������� ������
		break;
	default:
		break;
	}
}

void Operations::RRQ(SOCKET* _accept, char* recvbuf, char* sendbuf, int buflen)
{ // ������ �� ������
	std::cout << "RRQ\n";
	memset(_path, 0, MAX_PATH); // ������� ����� ����� �����
	memset(_mode, 0, 9); // ������� ����� ������
	int i;
	int j;

	for (i = 2, j = 0; i < (int)strlen(recvbuf) && recvbuf[i] != '0'; i++, j++) // ��������� ��� �����
	{
		_path[j] = recvbuf[i];
	}

	i++;
	_path[j] = '\0';

	for (j = 0; i < (int)strlen(recvbuf) && recvbuf[i] != '0'; i++, j++) // ��������� �����
	{
		_mode[j] = recvbuf[i];
	}
	_mode[j] = '\0';

	std::cout << "Path: " << _path << std::endl;
	std::cout << "Mode: " << _mode << std::endl;

	if (_access(_path, 0) != -1)
	{ // ���� ����������, ��������� ������ ��� �������� � ���������� ������
		std::cout << "File exist\n";
		memset(sendbuf, 0, buflen);
		int f_eof, count = 0, i, result;
		char number[2], tmp;

		FILE * file;
		fopen_s(&file, _path, "r");

		do {
			number[0] = count / 10 + '0'; // ��������� ����� �����
			number[1] = count + '0';

			sendbuf[0] = '0'; // ������������ ��� ��������
			sendbuf[1] = '3';
			sendbuf[3] = number[0]; // ������������� ����� �����
			sendbuf[4] = number[1];
			for (i = 5; i < 512 && !feof(file); i++) // ������ ������
			{
				fscanf_s(file, "%c", &tmp);
				sendbuf[i] = tmp;
			}

			result = send(*_accept, sendbuf, (int)strlen(sendbuf), 0); // ��������� �����
			if (result == SOCKET_ERROR) {
				printf("Send failed: %d\n", WSAGetLastError()); // ������
			}
			else
				printf("Bytes Sent: %ld\n", result);
			
			memset(recvbuf, 0, buflen);

			ACC(_accept, recvbuf, buflen); // ���� ������������� !!! ����� ����������
			std::cout << "ACCEPT: " << recvbuf << std::endl;

			f_eof = i;
			count++; // ����������� ����� �����
		} while (f_eof == 512 && !feof(file));

		fclose(file);
	}
	else
	{ // ���������� ��������� �� ������
		std::cout << "Can't find this file\n";
		int result;
		result = send(*_accept, ERR(Error::File_not_available), (int)strlen(ERR(Error::File_not_available)), 0); // ��������� ���������
		if (result == SOCKET_ERROR) {
			printf("Send failed: %d\n", WSAGetLastError()); // ������
		}
		else
			printf("Bytes Sent: %ld\n", result);
	}
}

void Operations::WRQ(SOCKET* _accept, char* recvbuf, char* sendbuf, int buflen)
{
	std::cout << "WRQ\n";
	memset(_path, 0, MAX_PATH); // ������� ����� ����� �����
	memset(_mode, 0, 9); // ������� ����� ������
	int i;
	int j;

	for (i = 2, j = 0; i < (int)strlen(recvbuf) && recvbuf[i] != '0'; i++, j++) // ��������� ��� �����
	{
		_path[j] = recvbuf[i];
	}

	i++;
	_path[j] = '\0';

	for (j = 0; i < (int)strlen(recvbuf) && recvbuf[i] != '0'; i++, j++) // ��������� �����
	{
		_mode[j] = recvbuf[i];
	}
	_mode[j] = '\0';

	std::cout << "Path: " << _path << std::endl;
	std::cout << "Mode: " << _mode << std::endl;

	if (_access(_path, 0) != -1)
	{ // ���� ����������, ���������� ��������� �� ������
		std::cout << "File exist\n";
		std::cout << "Can't find this file\n";
		int result;
		result = send(*_accept, ERR(Error::File_already_exists), (int)strlen(ERR(Error::File_already_exists)), 0); // ��������� ���������
		if (result == SOCKET_ERROR) {
			printf("Send failed: %d\n", WSAGetLastError()); // ������
		}
		else
			printf("Bytes Sent: %ld\n", result);
	}
	else
	{ // ����� ���, �������� ������ �� ������
		int f_eof, count = 0, i, result;
		char number[2], tmp;

		FILE* file;
		fopen_s(&file, _path, "w"); // ��������� ���� �� ������		

		do {
			memset(sendbuf, 0, buflen);
			number[0] = count / 10 + '0'; // ��������� ����� �����
			number[1] = count + '0';

			sendbuf[0] = '0'; // ������������ ��� ��������
			sendbuf[1] = '4';
			sendbuf[3] = number[0]; // ������������� ����� �����
			sendbuf[4] = number[1];
			sendbuf[5] = '\0';

			result = send(*_accept, sendbuf, (int)strlen(sendbuf), 0); // ��������� ������������� � ���������� ������
			if (result == SOCKET_ERROR) {
				printf("Send failed: %d\n", WSAGetLastError()); // ������
			}
			else
				printf("Bytes Sent: %ld\n", result);

			DATA(_accept, recvbuf, buflen);

			for (i = 5; i < (int)strlen(recvbuf); i++) // ��������� ������
			{
				tmp = recvbuf[i];
				fprintf_s(file, "%c", tmp);
			}
			f_eof = i;
			count++; // ����������� ����� �����
		} while (f_eof == 512);

		fclose(file);
	}
}

void Operations::DATA(SOCKET* _accept, char* recvbuf, int buflen)
{
	std::cout << "DATA\n";
	int result;

	do {
		result = recv(*_accept, recvbuf, buflen, 0); // ���� �������������
		if (result > 0)
		{
			printf("Bytes received: %d\n", result);
			recvbuf[result] = '\0';
			std::cout << recvbuf << std::endl;
			break;
		}
		else if (result == 0)
		{
			printf("Connection closed\n");
			result = shutdown(*_accept, SD_SEND);
			if (result == SOCKET_ERROR) {
				printf("shutdown failed: %d\n", WSAGetLastError()); // ������
			}
			break;
		}
		else
		{
			printf("recv failed: %d\n", WSAGetLastError()); // ������
			break;
		}
	} while (true);
}

bool Operations::ACC(SOCKET*_accept, char* recvbuf, int buflen)
{
	int result;

	std::cout << "ACC\n";
	do {
		result = recv(*_accept, recvbuf, buflen, 0); // ���� �������������
		if (result > 0)
		{
			printf("Bytes received: %d\n", result);
			recvbuf[result] = '\0';
			std::cout << recvbuf << std::endl;
		}
		else if (result == 0)
		{
			printf("Connection closed\n");
			result = shutdown(*_accept, SD_SEND);
			if (result == SOCKET_ERROR) {
				printf("shutdown failed: %d\n", WSAGetLastError()); // ������
			}
		}
		else
		{
			printf("recv failed: %d\n", WSAGetLastError()); // ������
		}
	} while (result != 4);
	return true;
}

const char* Operations::ERR(Error err)
{
	std::cout << "ERR\n";
	switch (err)
	{
	case Error::Error_undefined:
		return "0500Error_undefined";
		break;
	case Error::File_not_available:
		return "0501File_not_available";
		break;
	case Error::Access_error:
		return "0502Access_error";
		break;
	case Error::The_disc_is_full:
		return "0503The_disc_is_full";
		break;
	case Error::Invalid_TFTP_operation:
		return "0504Invalid_TFTP_operation";
		break;
	case Error::Unknown_transfer_ID:
		return "0505Unknown_transfer_ID";
		break;
	case Error::File_already_exists:
		return "0506File_already_exists";
		break;
	case Error::No_such_user:
		return "0507No_such_user";
		break;
	default:
		return "0500Unknown_error";
		break;
	}
}