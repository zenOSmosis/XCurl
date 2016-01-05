// Motivation for doing this: Because stream processing via PHP is probably very inefficent.
// Plus, I want to get better at C++.

//Compile & debug: clear && g++ prog.cpp -std=c++11 -l curl -o test && ./test

#include <stdio.h>
#include "headers/XCurl.hpp"

int main()
{
	//XCurl *xCurl = new XCurl("http://zenosmosis.com");
	XCurl *xCurl = new XCurl("http://localhost/XCurl/web/index.php");

	xCurl->setUserAgent("Mozilla");
	xCurl->setUserPassword("admin", "password");
	xCurl->setRequestHeader("Test", "12345");
	xCurl->setRequestMethod("POST");

	xCurl->exec();
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

	//xCurl->getInfo();

	//printf(info);

	// 
	// std::cout << pre::json::xCurl->getInfo() << std::endl;

	//return 0;

	delete xCurl; // Cleanup
}
