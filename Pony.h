#ifndef PONY_H
#define PONY_H
 
#include <WProgram.h>
#include <SPI.h>
#include <Ethernet.h>
#include "PonyRequest.h"


#define STRING_BUFFER_SIZE 128

typedef char * (*callback)(PonyRequest);

/*
class route {
public:
	char *uri;
	String method;
	callback func;
};
*/

struct ROUTE {
	char * uri;
	char * method;
	callback func;
};

class Pony {
	int count;
	int capacity;
	ROUTE * registry;
private:
	void dispatch(Client, ROUTE, PonyRequest);
public:
	Pony();
	~Pony();
	void listen(Client);
	void add(char*, char*, callback);
};

#endif
