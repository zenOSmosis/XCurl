/**
* Compile: clear && g++ test.cpp -std=c++11 -l curl -o test && ./test
*/
#include <stdio.h>
#include <curl/curl.h>

#include <string>
#include <sstream>
#include <iostream>

using namespace std;

// TODO: Include ability to process stream data
class XCurl
{
	private: CURL *_curl;
			 struct curl_slist *_requestHeaders; // Ref: http://curl.haxx.se/libcurl/c/httpcustomheader.html

	/**
	* The constructor.
	*/
	public: XCurl(string url)
	{
		printf("Initializing curl\n");

		this->_curl = curl_easy_init();

		if (this->_curl) {
			printf("Initialized curl\n");
		}

		this->setURL(url);
	}

	/**
	* The destructor.
	*/
	public: ~XCurl()
	{
		printf("Closing XCurl\n");

		//delete this->_curl; // Clean up
	}

	public: bool setRequestHeader(string key, string value)
	{
		string concat = key + ": " + value;

		this->_requestHeaders = curl_slist_append(this->_requestHeaders, concat.c_str());
		//printf("%s", concat.c_str());

		//printf(this->_requestHeaders);
		if (curl_easy_setopt(this->_curl, CURLOPT_HTTPHEADER, this->_requestHeaders)) {
			return true;
		}

		return false;
	}

	/**
	* Ref: http://curl.haxx.se/libcurl/c/curl_easy_setopt.html
	*/
	/*public: bool setOpt()
	{

	}*/

	/*public: char getOpt()
	{

	}*/

	public: bool setURL(string url)
	{
		if (curl_easy_setopt(this->_curl, CURLOPT_URL, url.c_str())) {
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

		//delete this; // Automatically clean up
	}

	// TODO: Copy the pointer value and return
	public: string getEffectiveURL()
	{
		char *info[128];

		curl_easy_getinfo(this->_curl, CURLINFO_EFFECTIVE_URL, &info);

		return string(*info);
	}

	public: long getResponseCode()
	{
		long codep;

		curl_easy_getinfo(this->_curl, CURLINFO_RESPONSE_CODE, &codep);

		return (long)codep;
	}

	public: double getTotalTime()
	{
		double timep;

		curl_easy_getinfo(this->_curl, CURLINFO_TOTAL_TIME, &timep);

		return (double)timep;
	}

	/**
	* Note: Returns an integer, as opposed to the decimal in CURLINFO_SIZE_DOWNLOAD
	*/
	public: int getDownloadSize()
	{
		double dip;

		curl_easy_getinfo(this->_curl, CURLINFO_SIZE_DOWNLOAD, &dip);

		return (int)dip;
	}
};