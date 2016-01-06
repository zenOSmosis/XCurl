// Motivation for doing this: Because stream processing via PHP is probably very inefficent.
// Plus, I want to get better at C++.

#include <stdio.h>
#include "headers/XCurl.hpp"

struct {
	string body;
	//string header;
	int _chunkCount;
} response;

void writeFunction(string chunk, void *xCurlInstance) {
	//response._chunkCount++;

	XCurl *xCurl = ((XCurl*)xCurlInstance);


	/*int diff = xCurl->getReceivedHeaderSize() - response.header.length();

	// Bug: This is running multiple times
	if (diff) {
		response.header = response.header.substr(0, xCurl->getReceivedHeaderSize());
	}*/

	printf("%s", chunk.c_str());
	
	//
	//printf("%ld", xCurl->getReceivedHeaderSize());
	//response.body.append(chunk.c_str());
}

int main()
{
	//XCurl *xCurl = new XCurl("http://zenosmosis.com");
	XCurl *xCurl = new XCurl("http://localhost/XCurl/web/index.php");

	xCurl->setUserAgent("Mozilla");
	xCurl->setUserPassword("admin", "password");
	xCurl->addRequestHeader("Test", "12345");
	xCurl->setRequestMethod("POST");

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
