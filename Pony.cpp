#include "Pony.h"
//#include "cppfix.h"

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

	// delete all allocated objects
	for (int i = 0; i < count; ++i)
	{
		delete registry[i];
	}
	
	// delete the pointer array
	delete [] registry;
}

void Pony::begin(int port) {
	//server = Server(port);
	//server.begin();
}

void Pony::add(String method, char *uri, callback func) {
	// we have to re-size if our new count will exceed the
	// current array capacity
	if (count + 1 > capacity)
	{
		capacity = capacity + 10; // simplest capacity growth algorithm ever ;)
		route ** newArray = new route*[capacity];
		
		// copy from old array into new
		memcpy(newArray[0], registry[0], count * sizeof(route*));
		
		// delete the old array
		delete [] registry;
		
		// re-assign registry array
		registry = newArray;
	}
	
	// create the new route
	route * r = new route();
	r->method = method;
	r->uri = uri;
	r->func = func;
	
	// add the new route to our array and increment count
	registry[count] = r;
	count++;
}

void Pony::listen(Client client) {
  String buffer = "";
  while (client.connected()) {	
    if (client.available()) {
      char c = ' ';
      while(c != -1) {
        c = client.read();
        buffer.concat(c);
      }
    }
    PonyRequest * request = new PonyRequest(buffer);
		Serial.println(request->getHttpMethod());
		//The registry should really enable us to only search applicable http method routes
		route * r = NULL;
		for(int i =  0 ; i < sizeof(registry) ; i++) { 
			if(request->getHttpMethod() == registry[i]->method && 
				 request->getPath() == registry[i]->uri) {
					r = registry[i];
			}
			
		}
    this->dispatch(client, *r, *request);
    delay(1);
    client.stop();
  }
}

void Pony::dispatch(Client client, route r, PonyRequest request) {
  String response = r.func(request);
  Serial.println(response);
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println();
  client.println(response);
}
