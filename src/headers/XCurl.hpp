#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/curlbuild.h>
#include <string>
#include <sstream>
#include <iostream>
#include <cstring>

using namespace std;

// TODO: Include ability to process stream data
class XCurl
{
	private: CURL *_curl;
			 struct curl_slist *_requestHeaders;
			 void (*_writeFunction)(string chunk, void *pass);
			 string _writeBuffer;

	/**
	* The constructor.
	*/
	public: XCurl(string url)
	{
		// Initialize the write callback
		this->setWriteFunction(NULL);

		printf("Initializing curl\n");

		curl_global_init(CURL_GLOBAL_ALL);

		this->_curl = curl_easy_init();

		if (this->_curl) {
			printf("Initialized curl\n");
		}

		this->setURL(url);
	}

	public: void setWriteFunction(void (*callback)(string chunk, void *pass))
	{
		this->_writeFunction = callback;
	}

	/**
	* The destructor.
	*/
	public: ~XCurl()
	{
		printf("Closing XCurl\n");

		/* free the custom headers */ 
    	curl_slist_free_all(this->_requestHeaders);
	}

	public: bool addRequestHeader(string key, string value)
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

	public: bool setUserAgent(string value)
	{
		return this->addRequestHeader("User-Agent", value);
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

	public: bool setRequestMethod(string requestMethod)
	{
		if (curl_easy_setopt(this->_curl, CURLOPT_CUSTOMREQUEST, requestMethod.c_str())) {
			return true;
		}

		return false;
	}

	public: bool setURL(string url)
	{
		if (curl_easy_setopt(this->_curl, CURLOPT_URL, url.c_str())) {
			return true;
		}

		return false;
	}

	/**
	* Ref: http://curl.haxx.se/libcurl/c/CURLOPT_USERPWD.html
	*/
	public: bool setUserPassword(string username, string password)
	{
		string userpass = username + ":" + password;

		if (curl_easy_setopt(this->_curl, CURLOPT_USERPWD, userpass.c_str())) {
			return true;
		}

		return false;
	}

	// This is the function we pass to LC, which writes the output to a BufferStruct
	private: static size_t _execWriteCallback (void* source_p,size_t size, size_t nmemb, void* dest_p){
	    int realsize=size*nmemb;
	    string chunk((char*)source_p,realsize);
	    //printf("%s", chunk.c_str());
	    if (((XCurl*)dest_p)->_writeFunction) {
	    	((XCurl*)dest_p)->_writeFunction(chunk.c_str(), dest_p);
	    }

	    //*((stringstream*)dest_p) << chunk;
	    return realsize;
	}

	public: void exec()
	{
		if (this->_curl) {
			/* temporarily outputting response headers */
			//curl_easy_setopt(this->_curl, CURLOPT_HEADER, true);

			CURLcode res;

			// The callback approach below seems to not utilize the correct scope
			curl_easy_setopt(this->_curl, CURLOPT_WRITEFUNCTION, &XCurl::_execWriteCallback);
			curl_easy_setopt(this->_curl, CURLOPT_WRITEDATA, this); // Passing our BufferStruct to LC

			/* Perform the request */
			res = curl_easy_perform(this->_curl);

			curl_easy_cleanup(this->_curl);
		}

		//delete this; // Automatically clean up
	}

	public: string getExec()
	{
		this->_writeBuffer.empty();

		this->setWriteFunction([](string chunk, void *pass){
			//printf("%s", chunk.c_str());
			//printf("%s", buffer.c_str());
			((XCurl*)pass)->_writeBuffer.append(chunk);
			//printf("%lu", ((XCurl*)pass)->getResponseCode());
		});
		this->exec();

		//printf("%s", this->_writeBuffer.c_str());

		return this->_writeBuffer.c_str();

		/*void writeFunction(string chunk) {
			printf("%s", chunk.c_str());

			chunks++;
		}*/

		//return buffer;
	}

	public: long getReceivedHeaderSize()
	{
		long sizep;

		curl_easy_getinfo(this->_curl, CURLINFO_HEADER_SIZE, &sizep);

		return sizep;
	}

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