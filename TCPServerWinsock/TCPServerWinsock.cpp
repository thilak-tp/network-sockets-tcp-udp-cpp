#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <tchar.h>

using std::cout;
using std::cin;
using std::endl;

int main()
{
	// Implementing a TCP server.
	SOCKET TCPServerSocket, acceptSocket;
	int port = 12345;
	WSADATA wsaData;
	int wsaErr;

	WORD wVersionRequested = MAKEWORD(2, 2);

	//1. Load ws2 DLL using WSAStartup function
	wsaErr = WSAStartup(wVersionRequested, &wsaData);
	if (wsaErr != 0)
	{
		cout << "The WinSock DLL not found!" << endl;
		return -1;
	}
	else
	{
		cout << "The WinSock DLL found!: OK" << endl;
		cout << "The status : " << wsaData.szSystemStatus << endl;
	}


	//2.  Creating a socket
	TCPServerSocket = INVALID_SOCKET;
	TCPServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (TCPServerSocket == INVALID_SOCKET)
	{
		cout << "Error occured while creating a socket: " << WSAGetLastError() << endl;
		// Unload the ws2 DLL
		WSACleanup();
		return -1;
	}
	else
	{
		cout << "Socket created: OK!" << endl;
	}

	//3. Binding the IP and  port to the socket
	sockaddr_in service;
	service.sin_family = AF_INET;
	InetPton(AF_INET, _T("127.0.0.1"), &service.sin_addr);
	//htons is used to correct the endian from little to  big
	service.sin_port = htons(port);

	if (bind(TCPServerSocket, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR)
	{
		cout << "Bind() failer!: " << WSAGetLastError() << endl;
		closesocket(TCPServerSocket);
		// Unload the ws2 DLL
		WSACleanup();
		return -1;
	}
	else
	{
		cout << "Bind() is OK!" << endl;
	}

	// 4. Listen on the socket for client requests
	if (listen(TCPServerSocket, 1) == SOCKET_ERROR)
	{
		cout << "Listen(): Error listening on socket: " << WSAGetLastError() << endl;
	}
	else
	{
		cout << "Listen(): is OK, I'm waiting for connections...."<<endl;
	}


	//5. Accept incoming connection else waiting for a client request
	// We are receiving the copy of the accepted socket so that the initial socket
	// can continue to listen while this can be used to communicate with the client
	acceptSocket = accept(TCPServerSocket, NULL, NULL);
	if (acceptSocket == INVALID_SOCKET)
	{
		cout << "Accept Failed!: " << WSAGetLastError() << endl;
		WSACleanup();
		return -1;
	}
	cout << "Accepted Connection" << endl;
	//6. Receiving data from the client

	char buffer[200] = {0};
	//cout << "Enter a message to send to the server." << endl;
	//cin.getline(buffer, sizeof(buffer));

	int byteCount = recv(acceptSocket, buffer, sizeof(buffer), 0);

	if (byteCount > 0)
	{
		cout << "Message Received: " << buffer << endl;
	}
	else
	{
		WSACleanup();
	}

	//7. After receiving sending data back to client
	char confirmationBuffer[200] = {0};
	cout << "Enter a message to send back to the client." << endl;
	cin.getline(confirmationBuffer, sizeof(confirmationBuffer));
	byteCount = send(acceptSocket, confirmationBuffer, sizeof(confirmationBuffer), 0);
	if (byteCount > 0)
	{
		cout << "Message Sent: " << confirmationBuffer << endl;
	}
	else
	{
		WSACleanup();
	}

	// Cleanup and disconnect
	system("pause");
	WSACleanup();

	
	return 0;
	
}
