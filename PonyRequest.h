#ifndef PONYREQUEST_H
#define PONYREQUEST_H

#include <WProgram.h>
#include <SPI.h>
#include <Ethernet.h>

#define GET "GET"
#define POST "POST"
#define PUT "PUT"
#define DELETE "DELETE"

#define STRING_BUFFER_SIZE 128

struct HTTP_VAR {
	char * key;
	char * value;
};

class PonyRequest {
	HTTP_VAR *vars;
	int count;
	int capacity;
	char * httpMethod;
        char * path;
        int contentLength;
private:
	void parse(Client);
  void parsePostParameters(char *);
  void parseContentLength(char *);
  void parseRequestInformation(char *);
	void addRequestParameter(String, String);
public:
	PonyRequest(Client);
	~PonyRequest();
	char * getHttpMethod();
	char * getPath();
	char * getUrl();
	HTTP_VAR *getRequestParameters();
};

#endif
