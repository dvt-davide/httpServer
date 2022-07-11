#include"HTTP.h"
//using namespace std;


void myFunc(SOCKET client, HTTP::Request req, HTTP::Response* res)
{	
	res->errorCode = "200";
	res->status = "OK";
	res->contentType = "Content-Type: text/html";
	res->content = "<html><body><b>Hello world</b></html></body>";
}

int main()
{
	try {
		//HTTP::Server("192.168.1.11", 8080);// indirizzo ip locale o comunque uno che si può bindare nel socket
		HTTP::Server("127.0.0.1", 8080);

		HTTP::service(&myFunc);
	}
	catch (...)
	{
		WSACleanup(); // Clean Windows Socket API.
		std::cout << "Server shut down;" << std::endl;
	}
}