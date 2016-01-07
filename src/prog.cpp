#include <stdio.h>
#include "headers/XCurlCLIInterpreter.hpp"

using namespace std;

void writeFunction(string chunk, XCurl *xCurl) {
    /* print the chunk to stdout */
	printf("%s", chunk.c_str());
}

int main(int argc, char *argv[])
{
    // Total number of arguments is argc
     std::string configPath = argv[1];
    
	//XCurl *xCurl = new XCurl("http://zenosmosis.com");
	XCurl *xCurl = new XCurlCLIInterpreter(configPath);

	//xCurl->setUserAgent("Mozilla");
	//xCurl->setUserPassword("admin", "password");
	//xCurl->addRequestHeader("Test", "12345");
	//xCurl->setRequestMethod("POST");

	xCurl->setWriteFunction(&writeFunction);
	xCurl->exec();

	//printf("%s", response.body.c_str());

	//printf("%s", xCurl->getExec().c_str());
	//xCurl->exec();

	//char *info[128];
	printf("\n");
	printf("Effective URL: %s", xCurl->getEffectiveURL().c_str());
	printf("\n");
	printf("Received header size: %li", xCurl->getReceivedHeaderSize());
	printf("\n");
	printf("Response code: %ld", xCurl->getResponseCode());
	printf("\n");
	printf("Total time: %f", xCurl->getTotalTime());
	printf("\n");
	printf("Download size: %i", xCurl->getDownloadSize());
	printf("\n");

	//printf("Chunks: %i", chunks);
	//printf("\n");

	//xCurl->getInfo();

	//printf(info);

	// 
	// std::cout << pre::json::xCurl->getInfo() << std::endl;

	//return 0;

	delete xCurl; // Cleanup
}
