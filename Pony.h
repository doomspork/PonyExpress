#ifndef PONY_H
#define PONY_H
 
#include <WProgram.h>
#include <SPI.h>
#include <Ethernet.h>
#include "PonyRequest.h"

#define STRING_BUFFER_SIZE 128

typedef char * (*callback)(PonyRequest);

struct ROUTE {
	char * uri;
	char * method;
	callback func;
};

class Pony {
	int count;
	int capacity;
	ROUTE * registry;
	callback error;
private:
	void dispatch(Client, ROUTE, PonyRequest);
	void notFound(Client, PonyRequest);
public:
	Pony();
	~Pony();
	void add(char*, char*, callback);
	void notFound(callback);
	void listen(Client);
};

#endif
