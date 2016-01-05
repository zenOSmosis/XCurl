// Motivation for doing this: Because stream processing via PHP is probably very inefficent.
// Plus, I want to get better at C++.

#include <stdio.h>
#include "headers/XCurl.hpp"

#include <iostream>
//#include <pre/json/to_json.hpp>

int main()
{
	XCurl *xCurl = new XCurl("http://zenosmosis.com");

	xCurl->setRequestHeader("test", "123");

	xCurl->exec();
	//xCurl->exec();

	//char *info[128];
	printf("\n");
	printf("%s", xCurl->getEffectiveURL().c_str());
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

	delete xCurl; // Cleanup
}
