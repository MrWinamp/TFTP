#pragma once
#ifndef ERROR_H
#define ERROR_H

enum class Error
{
	Error_undefined, // Ошибка не определена
	File_not_available, // Файл не найден
	Access_error, // Ошибка доступа
	The_disc_is_full, // Диск полон
	Invalid_TFTP_operation, // Некорректная TFTP операция
	Unknown_transfer_ID, // Неизвестный идентификатор передачи
	File_already_exists, // Файл уже существует
	No_such_user, // Нет такого пользователя
};
#endif // !ERROR_H
