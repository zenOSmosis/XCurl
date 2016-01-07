// TODO: Enable connection reutilization and unset any counts, etc. on re-utilization

#include <curl/curl.h>
#include <curl/easy.h>
#include <curl/curlbuild.h>
#include <string>
#include <sstream>
#include <iostream>
#include <cstring>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/regex.hpp>
#include <boost/algorithm/string/regex.hpp>
#include <boost/foreach.hpp>
#include <boost/tokenizer.hpp>

using namespace std;
using namespace boost;

// TODO: Include ability to process stream data
class XCurl
{
	private: CURL *_curl;
			 struct curl_slist *_requestHeaders;
			 void (*_writeFunction)(string chunk, XCurl *instance);

			 int _writeChunkCount;
			 string _writeBuffer;

			 string _responseHeadersRaw;
			 std::vector<std::string> _responseHeaderLineTokens;

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

	/**
	* The destructor.
	*/
	public: ~XCurl()
	{
		printf("Closing XCurl\n");

		/* free the custom headers */ 
    	curl_slist_free_all(this->_requestHeaders);
	}

	public: void setWriteFunction(void (*callback)(string chunk, XCurl *instance))
	{
		this->_writeFunction = callback;
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

	/**
	* Populates the _responseHeaders.
	*/
	private: static size_t _execHeaderCallback(char *buffer, size_t size, size_t nitems, void* dest_p)
	{
		int realsize=size*nitems;

		/* convert buffer into a string */
		string chunk((char*)buffer,realsize);

		/* append to _responseHeadersRaw property */
		((XCurl*)dest_p)->_responseHeadersRaw.append(chunk);

		//printf("%s", chunk.c_str());
		return nitems * size;
	}

	// This is the function we pass to LC, which writes the output to a BufferStruct
	private: static size_t _execWriteCallback(void* source_p,size_t size, size_t nmemb, void* dest_p){
	   	XCurl *self = ((XCurl*)dest_p);

	   	int realsize = size * nmemb;

	   	/* determine first instance of chunk write */
	   	if (self->_writeChunkCount == 0) {
            // TODO: Replace with g_strsplit () from glib.h (GNU C Library)
            
	   		/* split the response headers into lines */
		   	std::string str = self->_responseHeadersRaw;
			split(self->_responseHeaderLineTokens, str, boost::algorithm::is_any_of("\n"));
	   	}

	    /* convert source_p into a string */
	    string chunk((char*)source_p,realsize);

	    if (self->_writeFunction) {
	    	/* call the simplified write function callback */
	    	self->_writeFunction(chunk.c_str(), self);
	    }

	   	/* increment the write chunk count */
	   	self->_writeChunkCount++;

	    return realsize;
	}

	public: void exec()
	{
		if (this->_curl) {

			CURLcode res;

			curl_easy_setopt(this->_curl, CURLOPT_HEADERFUNCTION, &XCurl::_execHeaderCallback);
			curl_easy_setopt(this->_curl, CURLOPT_HEADERDATA, this);

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
		if (this->_writeFunction != NULL) {
			printf("Cannot use this method after setWriteFunction() has been called");
			throw std::exception();
		}

		/* clear the _write buffer */
		this->_writeBuffer.empty();

		this->setWriteFunction([](string chunk, XCurl *instance){
			//printf("%s", chunk.c_str());
			//printf("%s", buffer.c_str());
			((XCurl*)instance)->_writeBuffer.append(chunk);
			//printf("%lu", ((XCurl*)pass)->getResponseCode());
		});

		/* execute the request */
		this->exec();

		/* return the write buffer as a string */
		return this->_writeBuffer.c_str();
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