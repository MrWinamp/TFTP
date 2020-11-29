#pragma once
#ifndef OPERATIONSCODE_H
#define OPERATIONSCODE_H

enum class OperationsCode
{
	SLP = 0, // Сон
	RRQ = 1, // Запрос на чтение
	WRQ = 2, // Запрос на запись
	DATA = 3, // Передача данных
	ACC = 4, // Подтверждение
	ERR = 5 // Сообщение об ошибке
};

#endif // !OPERATIONSCODE_H
