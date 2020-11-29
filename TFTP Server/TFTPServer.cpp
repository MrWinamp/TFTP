#include "TFTPServer.h"

TFTPServer::TFTPServer()
{

}

int TFTPServer::start(PCSTR host, PCSTR port)
{ // Запускаем сервер и начинаем слушать клиентов
    WSAData wsaData;
    int result;
    SOCKET ListenSocket;

    result = WSAStartup(MAKEWORD(2, 2), &wsaData); // Регистрация библиотеки сокетов
    if (result != 0)
    {
        printf("WSAStartup failed with error: %d\n", result); // Ошибка
        return 1;
    }

    ListenSocket = socket(AF_INET, SOCK_STREAM, 0); // Инициализируем сокет
    if (ListenSocket < 0)
    {
        printf("ERROR opening socket"); // Ошибка
        return 1;
    }

    struct addrinfo hints; // Подготавливаем структуру адреса
    struct addrinfo* servinfo;
    memset(&hints, 0, sizeof hints); // Обнуляем
    hints.ai_family = AF_UNSPEC;     // Без разницы IPv4 или IPv6
    hints.ai_socktype = SOCK_STREAM; // TCP stream sockets
    hints.ai_flags = AI_NUMERICHOST;
    getaddrinfo(host, port, &hints, &servinfo);

    result = bind(ListenSocket, servinfo->ai_addr, servinfo->ai_addrlen); // Привязываем соккет к адресу
    if (result == SOCKET_ERROR) {
        printf("bind failed with error %u\n", WSAGetLastError()); // Ошибка
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    result = listen(ListenSocket, 1); // Включаем прослушку
    if (result == SOCKET_ERROR) {
        printf("listen failed with error: %ld\n", WSAGetLastError()); // Ошибка
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }

    printf("Waiting for client to connect...\n");

    _accept = accept(ListenSocket, NULL, NULL); // Принять входящее соединение
    if (_accept == INVALID_SOCKET) {
        printf("accept failed with error: %ld\n", WSAGetLastError()); // Ошибка
        closesocket(ListenSocket);
        WSACleanup();
        return 1;
    }
    else
        printf("Client connected.\n");

    result = clientHandler(); // Обрабатываем клиента

    closesocket(ListenSocket); // Закрываем соккеты
    closesocket(_accept);

    freeaddrinfo(servinfo); // Освобождаем память структуры
    WSACleanup(); // прекращает использование библиотеки DLL Winsock 2 (Ws2_32.dll)

    if (result)
        return result;
    return 0;
}

std::mutex mtx;

int TFTPServer::sendPackage(char* sendbuf, int sendbuflen)
{
    int result;
    result = send(_accept, sendbuf, (int)strlen(sendbuf), 0); // Отправить сообщение
    if (result == SOCKET_ERROR) {
        printf("send failed: %d\n", WSAGetLastError()); // Ошибка
        return 1;
    }
    else
        printf("Bytes Sent: %ld\n", result);
    return 0;
}

int TFTPServer::recvPackage(char * recvbuf, int buflen)
{
    int result;
    memset(recvbuf, 0, buflen);

    result = recv(_accept, recvbuf, buflen, 0); // Принять сообщение
    if (result > 0)
    {
        printf("Bytes received: %d\n", result);
        recvbuf[result] = '\0';
        std::cout << recvbuf << std::endl;
    }
    else if (result == 0)
    {
        printf("Connection closed\n");
        result = shutdown(_accept, SD_SEND);
        if (result == SOCKET_ERROR) {
            printf("shutdown failed: %d\n", WSAGetLastError()); // Ошибка
            return 1;
        }
    }
    else
    {
        printf("recv failed: %d\n", WSAGetLastError()); // Ошибка
        return 1;
    }

    return 0;
}

OperationsCode TFTPServer::packageParsing(char * recvbuf)
{ // Парсим код операции и кастуем его в перечисляемый класс
    OperationsCode code;
    code = static_cast<OperationsCode>(((int)recvbuf[1]-48) + 10 * ((int)recvbuf[0]-48));
    return code;
}

int TFTPServer::clientHandler()
{ // Обрабатываем общение с клиентом
    int result;
    const int buflen = 516;
    char recvbuf[buflen];
    char sendbuf[buflen];
    OperationsCode code;
    Operations operations;

    std::cout << "Chatting with client\n";
    
    while (true)
    {
        result = recvPackage(recvbuf, buflen);
        if (result)
            break;
        code = packageParsing(recvbuf);
        std::cout << "Parsing: " << static_cast<int>(code) << std::endl;
        operations.makeOperation(&_accept, &code, recvbuf, sendbuf, buflen);
        code = OperationsCode::SLP;
    }

    return 0;
}