#ifndef PONYREQUEST_H
#define PONYREQUEST_H

#include <WProgram.h>

struct HTTP_VAR {
	String key;
	String value;
};

class PonyRequest {
	HTTP_VAR *vars;
	int count;
	int capacity;
	String httpMethod;
private:
	void parse(String);
	void addRequestParameter(String, String);
public:
	PonyRequest(String);
	String getHttpMethod();
	String getPath();
	String getUrl();
	HTTP_VAR *getRequestParameters();
};

#endif
