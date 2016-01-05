// Motivation for doing this: Because stream processing via PHP is probably very inefficent.
// Plus, I want to get better at C++.

#include <stdio.h>
#include "headers/XCurl.hpp"

#include <iostream>
//#include <pre/json/to_json.hpp>

int main()
{
	char uri[128] = "http://zenosmosis.com";
	XCurl *xCurl = new XCurl(uri);

	xCurl->setRequestHeader("test", "123");

	//xCurl->exec();
	//xCurl->exec();

	//char *info[128];
	printf("\n");
	printf("%s", xCurl->getInfo());
	printf("\n");
	printf("%ld", xCurl->getResponseCode());
	printf("\n");
	printf("%f", xCurl->getTotalTime());
	printf("\n");
	printf("%i", xCurl->getDownloadSize());
	printf("\n");

	//xCurl->getInfo();

	//printf(info);

	// 
	// std::cout << pre::json::xCurl->getInfo() << std::endl;

	//return 0;
}
