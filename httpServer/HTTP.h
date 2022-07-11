#pragma once
#include <string.h>
#include <winsock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <tchar.h>
#include <stdlib.h>
#include <map>
#include <sstream>
#include <vector>
#include <cstring>  
#pragma comment(lib,"ws2_32.lib")

namespace HTTP {

	//typedef struct {
	//	std::string key;
	//	std::string value;
	//} keyValue;

	//typedef struct {
	//	std::string key;
	//	keyValue* value;
	//} keyValues;

	//typedef struct {
	//	std::vector<keyValue> headerData;
	//	std::vector <keyValues> body;
	//} Request;

	//void parseRequest(SOCKET client, Request *r); 

	typedef struct  {
		std::map<std::string, std::string> header;
		std::string content;
	} Request;
	typedef struct  {
		std::string protocol;
		std::string errorCode;
		std::string status;
		std::string serverName;
		std::string contentType;
		std::string content;
	} Response;

	void Server(const char* url, int port);
	void service(void(*func)(SOCKET, HTTP::Request, HTTP::Response*));
	void parseRequest(Request* r, const char* buffer);
};