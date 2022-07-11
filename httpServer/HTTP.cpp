#include "HTTP.h"

namespace HTTP {
	WSADATA wsa;
	struct sockaddr_in server;
	SOCKET serverSocket;
	int iResult;
}

//void HTTP::parseRequest(SOCKET client, Request* r)
//{
//	int nret = 0;// = recv(client, request, sizeof(request), NULL); // Just in case if the Client sends something, We Receive it.
//	//int lineCount = 0;
//	//do
//	//{
//		char* buffer = new char[256];
//		nret = recv(client, buffer, 256, NULL);
//		//std::cout << "LINE> " << line << std::endl;
//		if ((nret == SOCKET_ERROR) && (errno == EAGAIN || errno == EWOULDBLOCK))
//		{
//			std::cout << "Error Reading From Recv()\n";
//		}
//		else
//		{ // TBD: va distinto l'header dal body in qualche modo
//			std::string line;
//			std::stringstream bufferStream(buffer);
//			int lineCount = 0;
//			while (std::getline(bufferStream, line, '\n') && line != "\n")
//			{
//				std::cout << line << std::endl;
//				std::stringstream lineStream(line);
//				if (lineCount == 0)
//				{
//					// method path http version
//					// get /entity?param=1 HTTP/1.1
//					// post /entity HTTP/1.1
//					std::string token;
//					
//					// METHOD
//					std::getline(lineStream, token, ' ');
//					r->headerData.push_back({ "METHOD", token });
//					// PATH
//					std::getline(lineStream, token, ' ');
//					r->headerData.push_back({ "PATH", token });
//					// HTTP VER.
//					std::getline(lineStream, token, ' ');
//					r->headerData.push_back({ "PROTOCOL", token });
//
//				}
//				else
//				{ 
//					// il resto dell header
//					//Host: 127.0.0.1 : 8080
//					//User - Agent : curl / 7.79.1
//					//Accept : */*
//					
//					std::string key;
//					std::string value;
//
//					// KEY
//					std::getline(lineStream, key, ' ');
//					// VALUE
//					std::getline(lineStream, value, ' ');
//					r->headerData.push_back({ key.substr(0, key.size() -1), value });
//
//				}
//				lineCount++;
//			}
//		}
//		//lineCount++;
//	//} while (nret <= 0);
//
//}

void HTTP::Server(const char* url, int port)
{
	// Simple. Start WSA(Windows Sockets API). If the return answer is not 0. It means error so therefore, 
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		std::cerr << "WinsocK Failed" << std::endl; // Print out this on the Screen.
		//getchar(); // Pause Console 
		exit(true); // Exit Program, Note that in Computing, 1 = True, 0 = False. So 0 or 1 can also be entered as, exit(1);
	}

	std::cout << "Windows Socket API Started" << std::endl; // Notifiy user that, WSA (Windows Socket API) has started.

	HTTP::serverSocket = socket(AF_INET, SOCK_STREAM, NULL);
	if (HTTP::serverSocket == SOCKET_ERROR)
	{
		std::cerr << "Socket Error : " << WSAGetLastError() << std::endl;
		//getchar(); // Pause Console 
		exit(true); // Exit Program, Note that in Computing, 1 = True, 0 = False. So 0 or 1 can also be entered as, exit(1);
	}
	else if (HTTP::serverSocket == INVALID_SOCKET) {
		std::cerr << "Socket Error : " << WSAGetLastError() << std::endl;
		//getchar(); // Pause Console 
		exit(true); // Exit Program, Note that in Computing, 1 = True, 0 = False. So 0 or 1 can also be entered as, exit(1);
	}

	std::cout << "Socket Created" << std::endl;

	HTTP::server.sin_family = AF_INET; // Using AF_INET Address Family.
	HTTP::server.sin_port = htons(port); // Defining PORT
	//InetPton(AF_INET, _T(url), &server.sin_addr); // Defining The Network Address to Run the Server on	
	size_t size = strlen(url);
	wchar_t* wtext = new wchar_t[16];
	size_t outSize;
	mbstowcs_s(&outSize, wtext, 16,  url, size);
	LPWSTR ptr = wtext;
	InetPton(AF_INET, ptr, &server.sin_addr); // Defining The Network Address to Run the Server on

	HTTP::iResult = bind(HTTP::serverSocket, (struct sockaddr*)&server, sizeof(server)); // binding the Host Address and Port Number
	if (iResult == SOCKET_ERROR) // If Bind gives Error
	{
		std::cerr << "Bind Error " << WSAGetLastError() << std::endl;
		//getchar(); // Pause Console 
		exit(true); // Exit Program, Note that in Computing, 1 = True, 0 = False. So 0 or 1 can also be entered as, exit(1);
	}


	std::cout << "Listening on : " << url << ":" << port << std::endl; // Tell the User we Started Listening.
	iResult = listen(HTTP::serverSocket, AF_INET); // Then Actually Start Listening for incoming Connections.

}

void HTTP::service(void(*func)(SOCKET, HTTP::Request, HTTP::Response*))
{
	while (1)
	{
		SOCKET clientS;
		clientS = accept(HTTP::serverSocket, NULL, NULL);
		if (clientS == SOCKET_ERROR) { // if Accepting Connection is a Error
			std::cerr << "Accept Failed! " << WSAGetLastError() << std::endl;
			//getchar(); // Pause Console 
			//exit(true); // Exit Program, Note that in Computing, 1 = True, 0 = False. So 0 or 1 can also be entered as, exit(1);
			}
		else {
			std::cout << "Connection established" << std::endl; // notify user that someone connected
			//send(clientS, message, strlen(message), MSG_OOB); // Send Client a Message
			HTTP::Request req;
			HTTP::Response res;

			char* buffer = new char[256];
			HTTP::iResult = recv(clientS, buffer, 256, NULL);
			std::cout << "request: " << std::endl << buffer << std::endl;
			//req.content = buffer;
			HTTP::parseRequest(&req, buffer);
			res.protocol = "HTTP/1.1";
			res.serverName = "Custom/Viyadadna";

			func(clientS, req, &res);
			std::string respBuffer = res.protocol + ' ' +
				res.errorCode + ' ' +
				res.status + '\n' +
				res.serverName + '\n' +
				res.contentType + "\n\n" +
				res.content;

			const char* response = respBuffer.c_str();
			std::cout << "Response: " << std::endl << res.errorCode << std::endl;
				//"HTTP/1.1 200 OK \nServer: Custom/C++ (By DVD)\nContent-Type: text/html;\n\n <html><body><b>Hello world</b></html></body>";
			
			send(clientS, response, strlen(response), NULL);

			}
		
		closesocket(clientS); // close the Client Socket now that our Work is Complete.
		//WSACleanup(); // Clean Windows Socket API.
		//std::cout << "Connection closed." << std::endl;
	}

}

void HTTP::parseRequest(Request* r, const char* buffer)
{
}
