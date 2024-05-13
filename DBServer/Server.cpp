#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <winsock2.h>
#include <sstream>
#include <string>
#include <fstream>
#include <cstdio>
#include <filesystem>
#include <vector>
#include <cstring>
#include <windows.h>
#include "Server.h"
#include "DataBase.h"

//

void ServerSendResponseToClient(SOCKET acceptSocket, const std::string& response) {
	int sbyteCount = send(acceptSocket, response.c_str(), response.size(), 0);
	if (sbyteCount == SOCKET_ERROR) {
		std::cout << "Server send error: " << WSAGetLastError() << std::endl;
	}
	else {
		std::cout << "Server: Sent " << sbyteCount << " bytes" << std::endl;
	}
}

int ServerStart()
{
	int retVal = ServerSocketInitialize();

	if (retVal)
	{
		return retVal;
	}

	// Create a socket
	SOCKET serverSocket;
	serverSocket = INVALID_SOCKET;
	serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	// Check for socket creation success
	if (serverSocket == INVALID_SOCKET) {
		std::cout << "Error at socket(): " << WSAGetLastError() << std::endl;
		return 0;
	}
	else {
		std::cout << "Socket is OK!" << std::endl;
	}

	// Bind the socket to an IP address and port number
	sockaddr_in service;
	service.sin_family = AF_INET;
	service.sin_addr.s_addr = inet_addr("127.0.0.1");  // Replace with your desired IP address
	service.sin_port = htons(55555);  // Choose a port number

	// Use the bind function
	if (bind(serverSocket, reinterpret_cast<SOCKADDR*>(&service), sizeof(service)) == SOCKET_ERROR) {
		std::cout << "bind() failed: " << WSAGetLastError() << std::endl;
		closesocket(serverSocket);
		return 0;
	}
	else {
		std::cout << "bind() is OK!" << std::endl;
	}

	// Listen for incoming connections
	if (listen(serverSocket, 1) == SOCKET_ERROR) {
		std::cout << "listen(): Error listening on socket: " << WSAGetLastError() << std::endl;
	}
	else {
		std::cout << "listen() is OK! I'm waiting for new connections..." << std::endl;
	}

	while (1) {
		ServerRecieveCommand(serverSocket);
	}
}

int ServerSocketInitialize()
{
	WSADATA wsaData;
	int wsaerr;
	WORD wVersionRequested = MAKEWORD(2, 2);
	wsaerr = WSAStartup(wVersionRequested, &wsaData);

	if (wsaerr != 0) {
		std::cout << "The Winsock dll not found!" << std::endl;
	}
	else {
		std::cout << "The Winsock dll found" << std::endl;
		std::cout << "The status: " << wsaData.szSystemStatus << std::endl;
	}

	return wsaerr;
}

void ServerRecieveCommand(const SOCKET& serverSocket)
{
	// Accept incoming connections
	SOCKET acceptSocket;
	acceptSocket = accept(serverSocket, nullptr, nullptr);

	// Check for successful connection
	if (acceptSocket == INVALID_SOCKET) {
		std::cout << "accept failed: " << WSAGetLastError() << std::endl;
		return;
	}
	else {
		std::cout << "accept() is OK!" << std::endl;
	}

	// Receive data from the client
	char receiveBuffer[200];
	int rbyteCount = recv(acceptSocket, receiveBuffer, 200, 0);
	if (rbyteCount < 0) {
		std::cout << "Server recv error: " << WSAGetLastError() << std::endl;
	}
	else {
		receiveBuffer[rbyteCount] = '\0';
		std::cout << "Received data: " << receiveBuffer << std::endl;
	}

	std::istringstream iss(receiveBuffer);
	std::string command, collection, data;
	iss >> command >> collection; // Получаем команду и имя коллекции/файла

	// Читаем оставшуюся часть строки как данные
	std::getline(iss, data);
	if (data.length() > 0)
	{
		data = data.substr(1); // Удаляем пробел в начале строки
	}

	ServerExecuteCommand(command, collection, data, acceptSocket);

	shutdown(acceptSocket, SD_BOTH);
	closesocket(acceptSocket);
}

void ServerExecuteCommand(std::string& command, std::string& collection, std::string& data, const SOCKET& acceptSocket)
{
	// В зависимости от команды выполняем определенные действия
	if (command == "UPDATE_FILE") {
		DatabaseUpdateFile(collection, data);
		ServerSendResponseToClient(acceptSocket, "File updated successfully");
	}
	else if (command == "DELETE_FILE") {
		DatabaseDeleteFile(collection);
		ServerSendResponseToClient(acceptSocket, "File deleted successfully");
	}
	else if (command == "CREATE_FILE") {
		DatabaseCreateFile(collection, data);
		ServerSendResponseToClient(acceptSocket, "File created successfully");
	}
	else if (command == "GET_FILE") {
		std::string fileContent = DatabaseGetFileContents(collection);
		ServerSendResponseToClient(acceptSocket, fileContent);
	}
	else if (command == "LIST_FILES") {
		std::vector<std::string> files = DatabaseListFiles(collection);
		// Преобразуем список файлов в одну строку
		std::string fileListStr;
		for (const auto& file : files) {
			fileListStr += file + "\n";
		}
		ServerSendResponseToClient(acceptSocket, fileListStr);
	}
	else if (command == "CREATE_COLLECTION") {
		if (!DatabaseCreateCollection(collection)) {
			// Обработка ошибки, если создание коллекции не удалось
			ServerSendResponseToClient(acceptSocket, "Collection created failed");
		}
		else {
			ServerSendResponseToClient(acceptSocket, "Collection created successfully");
		}
	}
	else if (command == "DELETE_COLLECTION") {
		if (!DatabaseDeleteCollection(collection)) {
			// Обработка ошибки, если удаление коллекции не удалось
			ServerSendResponseToClient(acceptSocket, "Collection deleted failed");
		}
		else {
			ServerSendResponseToClient(acceptSocket, "Collection deleted successfully");
		}
	}
	else if (command == "LIST_COLLECTIONS") {
		std::vector<std::string> collections = DatabaseListCollections();
		// Преобразуем список коллекций в одну строку
		std::string collectionListStr;
		for (const auto& collection : collections) {
			collectionListStr += collection + "\n";
		}
		ServerSendResponseToClient(acceptSocket, collectionListStr);
	}
	else {
		ServerSendResponseToClient(acceptSocket, "Unknown command");
	}
}
