#include <WinSock2.h>
#include <WS2tcpip.h>
#include <iostream>
#include <tchar.h>

using std::cout;
using std::cin;
using std::endl;

int main()
{
	// Implementing a TCP Client.
	SOCKET TCPClientSocket, acceptSocket;
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
	TCPClientSocket = INVALID_SOCKET;
	TCPClientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (TCPClientSocket == INVALID_SOCKET)
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

	

	//3. Connecting to the server, this binds the IP and Port automatically
	sockaddr_in clientService;
	clientService.sin_family = AF_INET;
	InetPton(AF_INET, _T("127.0.0.1"), &clientService.sin_addr.s_addr);
	clientService.sin_port = htons(port);
	if (connect(TCPClientSocket, (SOCKADDR*)&clientService, sizeof(clientService)) == SOCKET_ERROR)
	{
		cout << "Client: Connect() - Failed to connect." << endl;
		WSACleanup();
		return -1;
	}
	else
	{
		cout << "Client: connect() is OK." << endl;
		cout << "Client: Can start sending and receiving data..." << endl;
	}

	system("pause");


	// 4. Send message to the server

	char buffer[200];
	cout << "Enter a message to send to the server." << endl;
	cin.getline(buffer, sizeof(buffer));

	int byteCount = send(TCPClientSocket, buffer, sizeof(buffer), 0);

	if (byteCount > 0)
	{
		cout << "Message Sent: " << buffer << endl;
	}
	else
	{
		WSACleanup();
	}
	//5. After  sending data to server, receive data sent back by server
	char confirmationBuffer[200] = { 0 };
	//cout << "Enter a message to send back to the client." << endl;
	//cin.getline(confirmationBuffer, sizeof(confirmationBuffer));
	byteCount = recv(TCPClientSocket, confirmationBuffer, sizeof(confirmationBuffer), 0);
	if (byteCount > 0)
	{
		cout << "Message Received: " << confirmationBuffer << endl;
	}
	else
	{
		WSACleanup();
	}

	system("pause");


	//5. Close socket
	closesocket(TCPClientSocket);
	WSACleanup();
	return 0;
}