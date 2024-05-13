#pragma once

#include <winsock.h>
#include <string>

int ServerStart();

int ServerSocketInitialize();

void ServerRecieveCommand(const SOCKET& serverSocket);

void ServerExecuteCommand(std::string& command, std::string& collection, std::string& data, const SOCKET& acceptSocket);
