#include <iostream>
#include <WS2tcpip.h>
#include <Winsock2.h>
#include <string>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main()
{
    // Initialize winsock
    WSADATA wsData;
    WORD ver = MAKEWORD(2, 2);

    int wsOk = WSAStartup(ver, &wsData);
    if (wsOk != 0)
    {
        cerr << "Can't Initialize winsock! Quitting" << endl;
        return 1;
    }

    // Create a socket
    SOCKET listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == INVALID_SOCKET)
    {
        cerr << "Can't create a socket! Quitting" << endl;
        WSACleanup();
        return 1;
    }

    // Bind the ip address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(54000);
    hint.sin_addr.S_un.S_addr = INADDR_ANY;

    if (bind(listening, (sockaddr*)&hint, sizeof(hint)) == SOCKET_ERROR)
    {
        cerr << "Can't bind socket! Quitting" << endl;
        closesocket(listening);
        WSACleanup();
        return 1;
    }

    // Tell Winsock the socket is for listening
    if (listen(listening, SOMAXCONN) == SOCKET_ERROR)
    {
        cerr << "Can't listen on socket! Quitting" << endl;
        closesocket(listening);
        WSACleanup();
        return 1;
    }

    // Wait for a connection
    sockaddr_in client;
    int clientSize = sizeof(client);

    SOCKET clientSocket = accept(listening, (sockaddr*)&client, &clientSize);
    if (clientSocket == INVALID_SOCKET)
    {
        cerr << "Can't accept client connection! Quitting" << endl;
        closesocket(listening);
        WSACleanup();
        return 1;
    }

    // Get the client IP address and port number
    char clientIP[INET_ADDRSTRLEN]; // Client's IP address
    ZeroMemory(clientIP, INET_ADDRSTRLEN);

    DWORD ipAddr = client.sin_addr.S_un.S_addr;
    sprintf(clientIP, "%d.%d.%d.%d",
            ipAddr & 0xFF,
            (ipAddr >> 8) & 0xFF,
            (ipAddr >> 16) & 0xFF,
            (ipAddr >> 24) & 0xFF);

    int clientPort = ntohs(client.sin_port);

    cout << clientIP << " connected on port " << clientPort << endl;

    // Close listening socket
    closesocket(listening);

    // While loop: accept and echo message back to client
    char buf[4096];

    while (true)
    {
        ZeroMemory(buf, 4096);

        // Wait for client to send data
        int bytesReceived = recv(clientSocket, buf, 4096, 0);
        if (bytesReceived == SOCKET_ERROR)
        {
            cerr << "Error in recv(). Quitting" << endl;
            break;
        }

        if (bytesReceived == 0)
        {
            cout << "Client disconnected " << endl;
            break;
        }

        cout << string(buf, 0, bytesReceived) << endl;

        // Echo message back to client
        send(clientSocket, buf, bytesReceived, 0);
    }

    // Close the socket
    closesocket(clientSocket);

    // Cleanup winsock
    WSACleanup();

    return 0;
}
