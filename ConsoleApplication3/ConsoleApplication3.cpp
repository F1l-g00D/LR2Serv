#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <windows.h>
#include <fstream>
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    SetConsoleOutputCP(1251);
    //Сервер
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(5000);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    bind(serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 1);
    cout << "Сервер чекає...\n";

    SOCKET clientSocket;
    sockaddr_in clientAddr{};
    int size = sizeof(clientAddr);
    clientSocket = accept(serverSocket, (sockaddr*)&clientAddr, &size);

    cout << "Клієнт підключився\n";
    ofstream file("received.rtf", ios::binary);
    //4096 для суцільної передачі
    const int FRAGMENT_SIZE = 512;
    char buffer[FRAGMENT_SIZE];
    int bytes;
    while ((bytes = recv(clientSocket, buffer, FRAGMENT_SIZE, 0)) > 0) {
        file.write(buffer, bytes);
        cout << "Отримано: " << bytes << " байт\n";
    }
    cout << "Файл отримано\n";
    file.close();
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();
    return 0;
}
