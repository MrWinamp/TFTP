#pragma once
#ifndef ERROR_H
#define ERROR_H

enum class Error
{
	Error_undefined, // ������ �� ����������
	File_not_available, // ���� �� ������
	Access_error, // ������ �������
	The_disc_is_full, // ���� �����
	Invalid_TFTP_operation, // ������������ TFTP ��������
	Unknown_transfer_ID, // ����������� ������������� ��������
	File_already_exists, // ���� ��� ����������
	No_such_user, // ��� ������ ������������
};
#endif // !ERROR_H
