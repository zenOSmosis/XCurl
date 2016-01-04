#include <stdio.h>
#include <curl/curl.h>

#include <string>
#include <sstream>
#include <iostream>

using namespace std;

//TODO: Include ability to process stream data
class XCurl
{
	//public:
		
		//XCurl(); // This is the constructor
	private: CURL *_curl;
			//string _uri;

	public: XCurl(char uri[128]) {
		printf("Initializing curl\n");

		this->_curl = curl_easy_init();

		if (this->_curl) {
			printf("Initialized curl\n");
		}

		this->setURI(uri);
	}

	public: bool setURI(char uri[128])
	{
		curl_easy_setopt(this->_curl, CURLOPT_URL, uri);

		//return
	}

	public: bool performCurl()
	{

		if (this->_curl) {
			CURLcode res;

			res = curl_easy_perform(this->_curl);

			curl_easy_cleanup(this->_curl);
		}
	}
};

int main()
{
	char uri[128] = "http://zenosmosis.com";
	XCurl *xCurl = new XCurl(uri);

	xCurl->performCurl();

	delete xCurl; // Clean up

	//return 0;
}