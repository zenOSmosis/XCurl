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

	public: XCurl(char uri[XCURL_URI_LENGTH]) {
		printf("Initializing curl\n");

		this->_curl = curl_easy_init();

		if (this->_curl) {
			printf("Initialized curl\n");
		}

		this->setURI(uri);
	}

	public: bool setURI(char uri[XCURL_URI_LENGTH])
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