#pragma once
#ifndef OPERATIONSCODE_H
#define OPERATIONSCODE_H

enum class OperationsCode
{
	SLP = 0, // ���
	RRQ = 1, // ������ �� ������
	WRQ = 2, // ������ �� ������
	DATA = 3, // �������� ������
	ACC = 4, // �������������
	ERR = 5 // ��������� �� ������
};

#endif // !OPERATIONSCODE_H
