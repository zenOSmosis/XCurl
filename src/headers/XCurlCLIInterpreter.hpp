#include <string>
#include "XCurl.hpp"
#include "dist/jsoncpp.cpp"
#include "dist/json/json.h"

class XCurlCLIInterpreter : public XCurl
{
    public: XCurlCLIInterpreter(std::string configPath) : XCurl("")
	{

        // sniff out config file
        
        FILE * pFile;
        long lSize;
        char * buffer;
        size_t result;
        
        pFile = fopen (configPath.c_str() , "r" );
        if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

        // obtain file size:
        fseek (pFile , 0 , SEEK_END);
        lSize = ftell (pFile);
        rewind (pFile);

        // allocate memory to contain the whole file:
        buffer = (char*) malloc (sizeof(char)*lSize);
        if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

        // copy the file into the buffer:
        result = fread (buffer,1,lSize,pFile);
        if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

        /* the whole file is now loaded in the memory buffer. */
        std::string bufferString = buffer;
        printf("%s", bufferString.c_str());
        
        //Parse the buffer string
        Json::Value root;
        
        Json::Reader::parse(bufferString, root, true);


        // terminate
        fclose (pFile);
        free (buffer);
        
 
        exit(0);  // Temporary, for development
	}
};