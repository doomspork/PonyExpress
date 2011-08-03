#include "Pony.h"

//Constructor
Pony::Pony() {
	count = 0;
	capacity = 0;
	registry = NULL;
}

//Deconstructor 
Pony::~Pony() {
	if (registry == NULL)
	{
		return;
	}
	
	// delete the pointer array
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
	r.method = method;
	r.uri = uri;
	r.func = func;
	
	// add the new route to our array and increment count
	registry[count] = r;
	count++;
}

void Pony::listen(Client client) {
  PonyRequest * request;
	while (client.connected()) {	
		if (client.available()) {
      request = new PonyRequest(client);
      ROUTE r;
      //The registry should really enable us to only search applicable http method routes
      int cnt = sizeof(registry);
    	for(int i = 0 ; i < cnt ; i++) {
     		if(strncmp(request->getHttpMethod(), registry[i].method, 2) == 0) {
      int reqLen = strlen(request->getPath());
      int regLen = strlen(registry[i].uri);
      if(reqLen == regLen) {
        if(strncmp(request->getPath(), registry[i].uri, reqLen) == 0) { 
          r = registry[i];
          break;
	}
      }
    }
  }
			this->dispatch(client, r, *request);
		        delay(1);
			client.stop();	
		}
  }
  delete request;
}

void Pony::dispatch(Client client, ROUTE r, PonyRequest request) {
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println(r.func(request));
}
