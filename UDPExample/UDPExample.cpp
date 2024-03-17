#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <cstring>
#include <iostream>
using namespace std;

int main()
{
    WSADATA wsadata;

    int res = WSAStartup(MAKEWORD(2, 2), &wsadata);
    if (res != NO_ERROR)
    {
        cout << "WSAStartup failked with error " << res << endl;
        return 1;
    }

    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET)
    {
        cout << "socket failed with error " << WSAGetLastError() << endl;
        return 2;
    }

    sockaddr_in addr;
    addr.sin_family = AF_INET;    
    addr.sin_port = htons(23000);   
    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr.s_addr);

    if (bind(udpSocket, (SOCKADDR*)&addr, sizeof(addr)) != NO_ERROR)
    {
        cout << "bind failed with error " << WSAGetLastError() << endl;
        return 3;
    }

    const size_t receiveBufSize = 1024;
    char receiveBuf[receiveBufSize];

    sockaddr_in senderAddr;
    int senderAddrSize = sizeof(senderAddr);

    cout << "Receiving data...." << endl;
    int bytesReceived = recvfrom(udpSocket, receiveBuf, receiveBufSize, 0, (SOCKADDR*)&senderAddr, &senderAddrSize);

    if (bytesReceived == SOCKET_ERROR)
    {
        cout << "recvfrom failed with error " << WSAGetLastError() << endl;
        return 4;
    }

    receiveBuf[bytesReceived] = '\0';
    COORD point;// Координата точки
    HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE); // получаем адрес собственного окна
    int counter = 0;
    for (int i = 0; i < strlen(receiveBuf); i++) {
        if (receiveBuf[i] == ' ') {
            counter = i;
            break;
        }
    }
 
    /*point.Y += 5;*/
   
   
    cout << "Received from " << senderAddr.sin_addr.s_host << endl;
   
    cout << "Data: " << endl;
    cout.write(receiveBuf, counter);
    CONSOLE_SCREEN_BUFFER_INFO bufferInfo;
    GetConsoleScreenBufferInfo(h, &bufferInfo);
    point.X = bufferInfo.dwCursorPosition.X;
    point.Y = bufferInfo.dwCursorPosition.Y;
    SetConsoleCursorPosition(h, point); /// установка курсора в указанную позицию!

    if (strstr(receiveBuf, "green") != NULL) {
        SetConsoleTextAttribute(h, 10);  // изменение цвета шрифта
    }
    else if (strstr(receiveBuf, "aqua") != NULL) {
        SetConsoleTextAttribute(h, 3);  // изменение цвета шрифта

    }
    else if (strstr(receiveBuf, "red") != NULL) {
        SetConsoleTextAttribute(h, 12);  // изменение цвета шрифта
    }
    else if (strstr(receiveBuf, "aqua") != NULL) {
        SetConsoleTextAttribute(h, 9);  // изменение цвета шрифта
    }
    else if (strstr(receiveBuf, "yellow") != NULL) {
        SetConsoleTextAttribute(h, 14);  // изменение цвета шрифта
    }
    else if (strstr(receiveBuf, "white") != NULL) {
        SetConsoleTextAttribute(h, 7);  // изменение цвета шрифта
    }
    GetConsoleScreenBufferInfo(h, &bufferInfo);
    point.X = bufferInfo.dwCursorPosition.X+1;
    point.Y = bufferInfo.dwCursorPosition.Y;
    SetConsoleCursorPosition(h, point); /// установка курсора в указанную позицию!
    for (int i = counter + 1; i < strlen(receiveBuf); i++) {
        cout << receiveBuf[i];
        if (receiveBuf[i] == ' ')
            break;
    }

    // Вернем цвет текста к стандартному
    SetConsoleTextAttribute(h, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);



    const size_t sendBufSize = 1024;
    char sendBuf[sendBufSize] = "Hello from UDPExample";

    int sendResult = sendto(udpSocket, sendBuf, strlen(sendBuf), 0, (SOCKADDR*)&senderAddr, senderAddrSize);
    if (sendResult == SOCKET_ERROR)
    {
        cout << "sendto failed with error " << WSAGetLastError() << endl;
        return 4;
    }


    closesocket(udpSocket);
    WSACleanup();
}

