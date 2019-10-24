#define WIN32_LEAN_AND_MEAN			// Strip rarely used calls
#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <thread>
#include<vector>
#include<map>
#include<iostream>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "5150"

int send_and_receive(SOCKET acceptSocket);
std::vector<std::string> rooms;
std::vector<SOCKET> myclients;
std::map<std::string, std::vector<SOCKET>> roomsandclients;

void broadcast(std::string msgroom,std::string msgstring);

int main(int argc, char** argv)
{

	while (true)
	{
		WSADATA wsaData;
		int iResult;

		// Initialize Winsock
		iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0)
		{
			// Something went wrong, tell the user the error id
			printf("WSAStartup failed with error: %d\n", iResult);
			return 1;
		}
		else
		{
			//printf("WSAStartup() was successful!\n");
		}

		// #1 Socket
		SOCKET listenSocket = INVALID_SOCKET;
		SOCKET acceptSocket = INVALID_SOCKET;

		struct addrinfo* addrResult = NULL;
		struct addrinfo hints;

		// Define our connection address info 
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;

		// Resolve the server address and port
		iResult = getaddrinfo(NULL, DEFAULT_PORT, &hints, &addrResult);
		if (iResult != 0)
		{
			printf("getaddrinfo() failed with error %d\n", iResult);
			WSACleanup();
			return 1;
		}
		else
		{
			//printf("getaddrinfo() is good!\n");
		}

		// Create a SOCKET for connecting to the server
		listenSocket = socket(
			addrResult->ai_family,
			addrResult->ai_socktype,
			addrResult->ai_protocol
		);
		if (listenSocket == INVALID_SOCKET)
		{
			// https://docs.microsoft.com/en-us/windows/win32/winsock/windows-sockets-error-codes-2
			printf("socket() failed with error %d\n", WSAGetLastError());
			freeaddrinfo(addrResult);
			WSACleanup();
			return 1;
		}
		else
		{
			//printf("socket() is created!\n");
		}

		// #2 Bind - Setup the TCP listening socket
		iResult = bind(
			listenSocket,
			addrResult->ai_addr,
			(int)addrResult->ai_addrlen
		);
		if (iResult == SOCKET_ERROR)
		{
			printf("bind failed with error: %d\n", WSAGetLastError());
			freeaddrinfo(addrResult);
			closesocket(listenSocket);
			WSACleanup();
			return 1;
		}
		else
		{
			//printf("bind() is good!\n");
		}

		// We don't need this anymore
		freeaddrinfo(addrResult);

		// #3 Listen
		iResult = listen(listenSocket, SOMAXCONN);
		if (iResult == SOCKET_ERROR)
		{
			printf("listen() failed with error %d\n", WSAGetLastError());
			closesocket(listenSocket);
			WSACleanup();
			return 1;
		}
		else
		{
			//printf("listen() was successful!\n");
		}

		// #4 Accept		(Blocking call)
		printf("Waiting for client to connect...\n");
		acceptSocket = accept(listenSocket, NULL, NULL);
		if (acceptSocket == INVALID_SOCKET)
		{
			printf("accept() failed with error: %d\n", WSAGetLastError());
			closesocket(listenSocket);
			WSACleanup();
			return 1;
		}
		else
		{
			//printf("accept() is OK!\n");
			//myvector.push_back(acceptSocket);
			//printf("Accepted client on socket %d\n", acceptSocket);
		}

		// No longer need server socket
		closesocket(listenSocket);
		//printf("press 0 to broadcast");
		//int choice;
		//std::cin >> choice;
		
		//if (choice == 0)
		//{
		//	for (SOCKET acpskt : myvector)
		//	{
		//		int result = send(acpskt, "broadcasting", 12, 0);
		//	}
		//}
		
		std::thread t1(send_and_receive, acceptSocket);
		t1.detach();
		//printf("waiting for new clients \n");

	}
	
	return 0;
}

int send_and_receive(SOCKET acceptSocket)
{
	int iResult = 0;
	// #5 recv & send	(Blocking calls)
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	int iSendResult;

	do
	{
		printf("Waiting to receive data from the client...\n");
		iResult = recv(acceptSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0)
		{
			// We have received data successfully!
			// iResult is the number of bytes received
			std::string msgstring, msgtype, msgowner, msgroom;
			int iResult = 0;
			msgstring = recvbuf;
			size_t temp, index = msgstring.find('/');
			msgtype = msgtype = msgstring.substr(0, index);
			
			temp = index;
			index = msgstring.find('/', index + 1);
			msgowner = msgstring.substr(temp + 1, index - temp - 1);
			temp = index;

			temp = index;
			index = msgstring.find('/', index + 1);
			msgroom = msgstring.substr(temp + 1, index - temp - 1);
			
			if (msgtype.compare("add") == 0)
			{
				if (rooms.empty())
				{
					rooms.push_back(msgroom);
					std::vector<SOCKET> temp;
					temp.push_back(acceptSocket);
					roomsandclients.insert(std::pair<std::string,std::vector<SOCKET>>(msgroom, temp));
				}
				else 
				{
					std::vector<std::string>::iterator it = std::find(rooms.begin(), rooms.end(), msgroom);
					if (it != rooms.end())
					{	
						int index = std::distance(rooms.begin(), it);
						std::string mapIndex = rooms[index];
						std::vector<SOCKET> temp = roomsandclients[mapIndex];
						temp.push_back(acceptSocket);
						roomsandclients[mapIndex] = temp;
						broadcast(rooms[index], msgstring);
					}
					else
					{
						rooms.push_back(msgroom);
						std::vector<SOCKET> temp;
						temp.push_back(acceptSocket);
						roomsandclients.insert(std::pair<std::string, std::vector<SOCKET>>(msgroom, temp));
						broadcast(msgroom, msgstring);
					}
				}
			}
			else if (msgtype.compare("rmv") == 0)
			{
				std::vector<std::string>::iterator it = std::find(rooms.begin(), rooms.end(), msgroom);
				if (it != rooms.end())
				{
					int index = std::distance(rooms.begin(), it);
					std::string mapIndex = rooms[index];
					std::vector<SOCKET> temp = roomsandclients[mapIndex];
					//printf(" size before erasing %d", temp.size());
					temp.erase(temp.begin() + index);
					roomsandclients[mapIndex] = temp;
					//printf(" size after erasing %d", temp.size());
					broadcast(msgroom, msgstring);
				}
			}
			else if (msgtype.compare("msg") == 0)
			{
				/*temp = index;
				index = msgstring.find('/', index + 1);
				std::string  actualmessage = msgstring.substr(temp+1, index - temp-1);*/
				broadcast(msgroom, msgstring);
			}
			/*
			for (int i = 0; i != iResult; i++)
			{
				printf("%c", recvbuf[i]);
			}
			printf("Bytes received: %d\n", iResult);*/



			// Send data to the client
			/*iSendResult = send(acceptSocket, recvbuf, iResult, 0);
			if (iSendResult == SOCKET_ERROR)
			{
				printf("send failed with error: %d\n", WSAGetLastError());
				closesocket(acceptSocket);
				WSACleanup();
				return 1;
			}
			printf("Bytes sent: %d\n", iSendResult);*/
		}
		else if (iResult < 0)
		{
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(acceptSocket);
			WSACleanup();
			return 1;
		}
		else // iResult == 0
		{
			printf("Connection closing...\n");
		}
	} while (iResult > 0);

	// #6 close
	iResult = shutdown(acceptSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %d\n", WSAGetLastError());
		closesocket(acceptSocket);
		WSACleanup();
		return 1;
	}

	// cleanup
	closesocket(acceptSocket);
	WSACleanup();
}

void broadcast(std::string room,std::string msgstring)
{
		std::vector<SOCKET> temp = roomsandclients[room];
		for (SOCKET acpskt : temp)
		{
			int iResult = send(acpskt, msgstring.c_str(), sizeof(msgstring), 0);
			if (iResult < 0)
				printf("error while connecting to client on socket %d", acpskt);
		}
}