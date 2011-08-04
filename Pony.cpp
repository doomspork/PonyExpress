#include "Pony.h"

//Constructor
Pony::Pony() {
	count = 0;
	capacity = 0;
	registry = NULL;
}

//Deconstructor 
Pony::~Pony() {
	if (registry == NULL) {
		return;
	}
	delete [] registry; 
}

void Pony::add(char * method, char * uri, callback func) {
	if (count + 1 > capacity) {
		capacity = capacity + 10; // simplest capacity growth algorithm ever ;)
		ROUTE * newArray = new ROUTE[capacity];
		
		for(int i = 0 ; i < count ; i++) {
			newArray[i] = registry[i];
		}
		delete [] registry;
		registry = newArray;
	}
	
	// create the new route
	ROUTE r;
	r.uri = new char[strlen(uri) + 1];
	r.uri[strlen(uri)] = '\0';
	strcpy(r.uri, uri);
	r.method = method;
	r.func = func;
	
	// add the new route to our array and increment count
	registry[count] = r;
	count++;
}

void Pony::notFound(callback func) {
	error = func;
}

void Pony::listen(Client client) {
  PonyRequest * request;
	//while (client.connected()) {	
		if (client.available()) {
      request = new PonyRequest(client);
      ROUTE r;
			bool found = false;
      //The registry should really enable us to only search applicable http method routes
      int cnt = sizeof(registry);
    	for(int i = 0 ; i < cnt ; i++) {
				if(strncmp(request->getHttpMethod(), registry[i].method, 2) == 0) {
      		int len = strlen(request->getPath());
      		if(len == strlen(registry[i].uri)) {
        		if(strncmp(request->getPath(), registry[i].uri, len) == 0) { 
          		r = registry[i];
							found = true;
          		break;
						}
      		}
    		}
  		}
			if(found) {
				this->dispatch(client, r, *request);
			} else {
				this->notFound(client, *request);
			}
			delay(1);
			client.stop();	
		}
  //}
  delete request;
}

void Pony::dispatch(Client client, ROUTE r, PonyRequest request) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println(r.func(request));
}

void Pony::notFound(Client client, PonyRequest request) {
  client.println("HTTP/1.1 404 Not Found");
  client.println();
	client.println(error(request));
}
