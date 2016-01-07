#include <string>
#include "XCurl.hpp"
#include "dist/jsoncpp.cpp"
#include "dist/json/json.h"
#include <iostream>
#include <fstream>
#include <string>


class XCurlCLIInterpreter : public XCurl
{
    public: XCurlCLIInterpreter(std::string configPath) : XCurl("")
	{
        
        Json::Value root;   // will contains the root value after parsing.
        Json::Reader reader;
        std::ifstream test(configPath, std::ifstream::binary);
        bool parsingSuccessful = reader.parse(test, root, false);
        if (!parsingSuccessful)
        {
            printf("Could not parse configuration file\n");
            exit(1);
        }
        
        std::string url = root.get("url", "ASCII").asString();      
        this->setURL(url);
        
  
        Json::Value headers = root["headers"];
        for (Json::ValueIterator itr = headers.begin(); itr != headers.end(); itr++)
        {
            Json::Value key = itr.key();
            Json::Value value = (*itr);
            
            this->addRequestHeader(key.asString().c_str(), value.asString().c_str());
        }
        
        printf("%s", url.c_str());
        // terminate
        //fclose (pFile);
        //free (buffer);
	}
};