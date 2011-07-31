#include "PonyRequest.h"

PonyRequest::PonyRequest(String buffer) {
	count = 0;
	capacity = 0;
	vars = NULL;
	this->parse(buffer);
}

PonyRequest::~PonyRequest() {
	if(vars == NULL) {
		return;
	}

	delete [] vars;
}

void PonyRequest::parse(String buffer) {
 httpMethod = buffer.substring(0, buffer.indexOf(' '));
 Serial.println("Method: " + httpMethod);
 if(httpMethod.equals("POST")) {
		String vars = buffer.substring(buffer.lastIndexOf('\n\r'));
		int lastIndex = 0;
		while(lastIndex != -1) {
			int index = vars.indexOf('&');
			String group = vars.substring(lastIndex, index);
			Serial.println(group);
			int eqIndex = group.indexOf('=');
			String key = group.substring(0, eqIndex - 1);
			String value = group.substring(eqIndex + 1);
			this->addRequestParameter(key.trim(), value.trim());
			lastIndex = index;
		}
	} else if (httpMethod.equals("GET")) {
		//Parse the get vars
	}
}

void PonyRequest::addRequestParameter(String key, String value) {
	if (count + 1 > capacity) {
		int capacity = capacity + 10;
		HTTP_VAR * newArray = new HTTP_VAR[capacity];
		memcpy(newArray, vars, count * sizeof(vars));

		delete [] vars;
		
		vars = newArray;
	}
	
	HTTP_VAR var;
	var.key = key;
	var.value = value;
	
	vars[count] = var;
	count++;
}

String PonyRequest::getHttpMethod() {
  return httpMethod;
}

String PonyRequest::getPath() {
	return "/";
}

String PonyRequest::getUrl() {
	return "";
}

HTTP_VAR * PonyRequest::getRequestParameters() {
	return vars;
}
