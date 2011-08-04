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
	delete [] path;
	delete [] httpMethod;
}

void PonyRequest::parse(Client client) {
	char * buffer;
  char c;
  boolean tail = false;
  do {
		buffer = new char[STRING_BUFFER_SIZE];
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
		delete [] buffer;
  } while(!tail);
  if(contentLength > 0) {
    this->parsePostParameters(buffer);
  }
}

void PonyRequest::parseRequestInformation(char * buffer) {
	char * s = strstr(buffer, " ");
	httpMethod = new char[s - buffer + 1]; 
	strncpy(httpMethod, buffer, s - buffer); 
	httpMethod[s - buffer] = '\0';
	
	char * p = strstr(s + 1, " ");
	char * url = new char[p - s];
	strncpy(url, s + 1, p - s - 1);
	url[p - s - 1] = '\0';
	
	if(strchr(url, '?') != NULL){
		this->parseGetParameters(url);
	} else {
		path = new char[strlen(url)];
		strcpy(path, url);
	}
	delete [] url;
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
	delete [] vars;
}

void PonyRequest::parsePostParameters(char * buffer) {
  /*int count = 0;
  int last = 0;
  char * tokens = strtok(buffer, "&");
  for(int i = 0 ; i < sizeof(tokens) ; i++) {
    Serial.println(tokens[i]); 
  }*/
}

void PonyRequest::parseGetParameters(char * buffer) {
	char * s = strstr(buffer, "?"); 
	path = new char[s - buffer + 1]; 
	strncpy(path, buffer, s - buffer); 
	path[s - buffer] = '\0';
	char * parameters = new char[strlen(buffer) - (s - buffer)]; 
	strcpy(parameters, s + 1);
	
	if(parameters != NULL) {
		char * param = strtok(parameters, "&");
  	do {
    	this->addRequestParameter(param);
    	param = strtok(NULL, "&");  
  	} while(param != NULL);
	}
}

void PonyRequest::addRequestParameter(char * params) {
	char * key = strtok(params, "=");
	char * value = strtok(NULL, " \n");
	HTTP_VAR var;
	var.key = new char[strlen(key)];
	strcpy(var.key, key);
	var.value = new char[strlen(value)];
	strcpy(var.value, value);
	
	if(var.value != NULL) {
		if (count + 1 > capacity) {
			int capacity = capacity + 10;
			HTTP_VAR * newArray = new HTTP_VAR[capacity];
			for(int i = 0 ; i < count ; i++) {
  			newArray[i] = vars[i];
			}
			delete [] vars;
			vars = newArray;
		}
	
		vars[count] = var;
		count++;
	}
	delete [] key;
	delete [] value;
}

char * PonyRequest::getHttpMethod() {
  return httpMethod;
}

char * PonyRequest::getPath() {
	return path;
}

HTTP_VAR * PonyRequest::getRequestParameters() {
	return vars;
}
