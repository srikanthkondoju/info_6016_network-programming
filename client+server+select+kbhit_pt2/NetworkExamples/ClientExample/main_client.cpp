#define WIN32_LEAN_AND_MEAN			// Strip rarely used calls

#include <Windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include<iostream>
#include<thread>
#include <vector>
#include<string.h>
#include<string>

//std::getline(std::cin , line);

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")

#define DEFAULT_BUFLEN 512
#define DEFAULT_PORT "5150"

int receive(SOCKET connectSocket);
std::string myname;


int main(int argc, char** argv)
{
	int rooms_joined = 0;
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

	// #1 socket
	SOCKET connectSocket = INVALID_SOCKET;

	struct addrinfo *result = NULL;
	struct addrinfo *ptr = NULL;
	struct addrinfo hints;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// resolve the server address and port
	iResult = getaddrinfo("127.0.0.1", DEFAULT_PORT, &hints, &result);
	if (iResult != 0)
	{
		printf("getaddrinfo() failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}
	else
	{
		//printf("getaddrinfo() successful!\n");
	}

	// #2 connect
	// Attempt to connect to the server until a socket succeeds
	for (ptr = result; ptr != NULL; ptr = ptr->ai_next)
	{
		// Create a SOCKET for connecting to the server
		connectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (connectSocket == INVALID_SOCKET)
		{
			printf("socket() failed with error code %d\n", WSAGetLastError());
			freeaddrinfo(result);
			WSACleanup();
			return 1;
		}

		// Attempt to connect to the server
		iResult = connect(connectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR)
		{
			printf("connect() failed with error code %d\n", WSAGetLastError());
			closesocket(connectSocket);
			connectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	freeaddrinfo(result);

	if (connectSocket == INVALID_SOCKET)
	{
		printf("Unable to connect to the server!\n");
		WSACleanup();
		return 1;
	}
	printf("Successfully connected to the server on socket %d!\n", (int)connectSocket);
	std::thread t1(receive, connectSocket);
	t1.detach();
	/*while (true)
	{
		printf(" valid commands\n1.join Room\n2.Leave Room\n3.Send message to the room ");
		int choice = 0;

	}*/
	

	std::vector<std::string> myrooms;

	printf("please input your name: ");
	std::getline(std::cin, myname);//std::cin >> myname;
	//myname = "." + myname;
	// #3 write & read
	while (true)
	{
		printf("1.join room\n2.leave room\n3.send message to the room\n");
		int mychoice = 0;
		std::cin >> mychoice;
		//std::cin.clear();
		//std::cin.ignore(INT_MAX);
		std::string roomname;
		std::string msg;
		std::vector<std::string>::iterator it;
		switch (mychoice)
		{
		case 1:
			printf("please enter room name: ");
			std::cin >> roomname;
			//std::cin.ignore(1000,'\n');
			//getline(std::cin, roomname);
			//std::cin >> roomname;
			msg = "add/" + myname+"/"+roomname;
			iResult = send(connectSocket, msg.c_str(), sizeof(msg), 0);
			if (iResult > 0)
			{
				myrooms.push_back(roomname);
				printf("joined sucessfully\n");
			}

			break;
		case 2:
			printf("please enter room name: ");
			//std::cin.ignore(1000, '\n');
			std::cin >> roomname;
			//std::getline(std::cin, roomname);//std::cin >> roomname;
			it = std::find(myrooms.begin(), myrooms.end(), roomname);
			if (it != myrooms.end())
			{
				msg = "rmv/" + myname + "/" + roomname;
				iResult = send(connectSocket, msg.c_str(), sizeof(msg), 0);
				int index = std::distance(myrooms.begin(), it);
				myrooms.erase(myrooms.begin() + index);
				std::cout << "Removed sucessfully" << std::endl;
			}
			else
			{
				std::cout << "you are not a member of "+roomname << std::endl;
			}
			break;
		case 3:
			std::cout << "please input room name: " << std::endl;
			//std::cin.ignore(1000, '\n');
			//std::getline(std::cin, roomname);//std::cin >> ;
			std::cin >> roomname;
			it = std::find(myrooms.begin(), myrooms.end(), roomname);
			if (it != myrooms.end())
			{
				std::cout << "please input message: " << std::endl;
				/*std::cin.ignore(1000, '\n');
				std::getline(std::cin, msg);*/
				std::cin >> msg;
				msg = "msg/" + myname + "/" + roomname + "/" + msg;
				iResult = send(connectSocket, msg.c_str(), sizeof(msg), 0);
				std::cout << "sent sucessfully" << std::endl;
			}
			else
			{
				std::cout << "you are need to join the room in order to send the message to " + roomname << std::endl;
			}
			
			
			
			break;
		default:
			std::cout << "invalid choice" << std::endl;
			break;
		}
	}
	//printf("enter message to send to server: ");

	//std::cout << "enter message to send to the server: " << std::endl;
	
	std::string buff;

	std::cin >> buff;



	////printf("Sending a packet to the server...\n");
	//iResult = send(connectSocket, buff.c_str(), sizeof(buff), 0);
	//if (iResult == SOCKET_ERROR)
	//{
	//	printf("send() failed with error: %d\n", WSAGetLastError());
	//	closesocket(connectSocket);
	//	WSACleanup();
	//	return 1;
	//}
	////printf("Bytes sent: %d\n", iResult);

	//// Receive a message from the server before quitting
	//char recvbuf[DEFAULT_BUFLEN];
	//int recvbuflen = DEFAULT_BUFLEN;
	//printf("Waiting to receive data from the server...\n");
	//system("Pause");
	//

	// #4 close
	closesocket(connectSocket);
	WSACleanup();

	return 0;
}

int receive(SOCKET connectSocket)
{
	int iResult = 0;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;
	std::string msgstring,msgtype,msgowner,actlmsg,msgroom;
	while (true)
	{
		//printf("waiting to receive\n");
		iResult = recv(connectSocket, recvbuf, sizeof(recvbuf), 0);
		msgstring = recvbuf;
		if (iResult > 0)
		{
			msgtype = msgstring.substr(0, 3);
			size_t temp, index = msgstring.find('/');
			msgtype = msgstring.substr(0, index);

			temp = index;
			index = msgstring.find('/', index + 1);
			msgowner = msgstring.substr(temp + 1, index - temp - 1);

			temp = index;
			index = msgstring.find('/', index + 1);
			msgroom = msgstring.substr(temp + 1, index - temp - 1);
			temp = index;
			if (msgowner.compare(myname) != 0)
			{
				if (msgtype.compare("msg") == 0)
				{
					index = msgstring.find('/', index + 1);
					actlmsg = msgstring.substr(temp + 1, index - temp - 1);
					std::cout << "client :" + msgowner + " room :" + msgroom + " message :" + actlmsg << std::endl;
				}
				else if (msgtype.compare("add") == 0)
				{
					std::cout << "client :" + msgowner + " joined the room :" + msgroom << std::endl;
				}
				else if (msgtype.compare("rmv") == 0)
				{
					std::cout << "client :" + msgowner + " left the room :" + msgroom << std::endl;
				}
			}
		}
		else if (iResult == 0)
		{
			printf("Connection closed\n");
		}
		else
		{
			printf("recv failed with error: %d\n", WSAGetLastError());
			closesocket(connectSocket);
			WSACleanup();
			return 1;
		}
	}
	
}