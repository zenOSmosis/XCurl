/**
* Compile: clear && g++ test.cpp -std=c++11 -l curl -o test && ./test
*/
#include <stdio.h>
#include <curl/curl.h>

#include <string>
#include <sstream>
#include <iostream>

using namespace std;

const int XCURL_URI_LENGTH = 128;

//TODO: Include ability to process stream data
class XCurl
{
	private: CURL *_curl;

	/**
	* The constructor.
	*/
	public: XCurl(char uri[XCURL_URI_LENGTH])
	{
		printf("Initializing curl\n");

		this->_curl = curl_easy_init();

		if (this->_curl) {
			printf("Initialized curl\n");
		}

		this->setURI(uri);
	}

	/**
	* The destructor.
	*/
	public: ~XCurl()
	{
		printf("Closing XCurl\n");

		//delete this->_curl; // Clean up
	}

	/**
	* Ref: http://curl.haxx.se/libcurl/c/curl_easy_setopt.html
	*/
	public: bool setOpt()
	{

	}

	public: char getOpt()
	{

	}

	public: bool setURI(char uri[XCURL_URI_LENGTH])
	{
		if (curl_easy_setopt(this->_curl, CURLOPT_URL, uri)) {
			return true;
		}

		return false;
	}

	public: bool exec()
	{

		if (this->_curl) {
			CURLcode res;

			res = curl_easy_perform(this->_curl);

			curl_easy_cleanup(this->_curl);
		}

		delete this; // Automatically clean up
	}
};

int main()
{
	char uri[128] = "http://zenosmosis.com";
	XCurl *xCurl = new XCurl(uri);

	xCurl->exec();
	//xCurl->exec();

	//return 0;
}