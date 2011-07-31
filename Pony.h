#ifndef PONY_H
#define PONY_H
 
#include <WProgram.h>
#include <SPI.h>
#include <Ethernet.h>
#include "PonyRequest.h"
//#include "cppfix.h"

#define GET "GET"
#define POST "POST"
#define PUT "PUT"
#define DELETE "DELETE"

typedef String(*callback)();

class route {
public:
	char *uri;
	String method;
	callback func;
};

class Pony {
	int count;
	int capacity;
	route ** registry;
private:
	void dispatch(Client, route, PonyRequest);
public:
	Pony();
	~Pony();
	void begin(int);
	void listen(Client);
	void add(String, char*, callback);
};

#endif
