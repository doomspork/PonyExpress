#include "PonyRequest.h"

PonyRequest::PonyRequest(Client client) {
	count = 0;
	capacity = 0;
  contentLength = 0;
	vars = NULL;
	this->parse(client);
}

PonyRequest::~PonyRequest() {
	if(vars == NULL) {
		return;
	}
	delete [] vars;
}

void PonyRequest::parse(Client client) {
  char * buffer = new char[STRING_BUFFER_SIZE];
  memset(buffer, 0, STRING_BUFFER_SIZE);
  char c;
  boolean tail = false;
  do {
		tail = (strlen(buffer) == 2);
		memset(buffer, 0, STRING_BUFFER_SIZE);
		int index = 2;
    buffer[0] = client.read();
    buffer[1] = client.read();
    while (buffer[index-2] != '\r' && buffer[index-1] != '\n') {
      c = client.read();
      if (index < STRING_BUFFER_SIZE) {
        buffer[index] = c;
      }
      index++;
    }
    if(strncmp(buffer, GET, 3) == 0 || strncmp(buffer, POST, 4) == 0) {
      this->parseRequestInformation(buffer);
    } else if(strncmp(buffer, "Content-Length: ", 16) == 0) {
      this->parseContentLength(buffer);
    }
  } while(!tail);
  if(contentLength > 0) {
    this->parsePostParameters(buffer);
  }
	delete []buffer;
}

void PonyRequest::parseContentLength(char * buffer) {
  int index = 16;
  int len = index - strlen(buffer);
	char vars[len];
	
	for(int i = index; i< strlen(buffer) ; i++) {
		if (buffer[i] != ' ' && buffer[i] != '\r' && buffer[i] != '\n') {
			vars[i-index] = buffer[i];
		} else {
			break;
		}
	}
  contentLength = atoi(vars);
	delete []vars;
}

void PonyRequest::parseRequestInformation(char * buffer) {
	int first = (buffer[3] == ' ') ? 3 : 4;
 	httpMethod = new char[first + 1];
	httpMethod[first] = '\0';
	memcpy(httpMethod, buffer, first);
	int second = first + 1;
	do {
  	second++;
	}while(buffer[second] != ' ');
	second--;
	int size = (second - first);

	path = new char[size + 1];
	path[size] = '\0';
	first += 1; //We want first to be the index after the space
	for(int i = 0 ; i < size ; i++) {
	  path[i] = buffer[first + i]; //WHAT?!?!
	}
}
  
void PonyRequest::parsePostParameters(char * buffer) {
  int count = 0;
  HTTP_VAR v[count];
  int last = 0;
  char * tokens = strtok(buffer, "&");
  for(int i = 0 ; i < sizeof(tokens) ; i++) {
    Serial.println(tokens[i]); 
  }
}

void PonyRequest::addRequestParameter(String key, String value) {
	/*if (count + 1 > capacity) {
		int capacity = capacity + 10;
		HTTP_VAR * newArray = new HTTP_VAR[capacity];
		
		for(int i = 0 ; i < count ; i++) {
  newArray[i] = vars[i];
}
		delete [] vars;
		
		vars = newArray;
	}
	
	HTTP_VAR var;
	var.key = key;
	var.value = value;
	
	vars[count] = var;
	count++;
*/
}

char * PonyRequest::getHttpMethod() {
  return httpMethod;
}

char * PonyRequest::getPath() {
	return path;
}

char * PonyRequest::getUrl() {
	return "";
}

HTTP_VAR * PonyRequest::getRequestParameters() {
	return vars;
}
