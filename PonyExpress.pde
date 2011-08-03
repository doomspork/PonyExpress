#include <SPI.h>
#include <Ethernet.h>

#include "cppfix.h"
#include "Pony.h"


Pony pony = Pony();
Server server = Server(80);

char * index(PonyRequest request) {
  return "<html><body><form method='POST' action='/post'><input type='text' name='key' value='value'/><input type='submit' value='Submit'/></form></body></html>";
}

char * post(PonyRequest request) {
  return "<html><body>Successfully</body></html>";
}

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x59, 0x50 };  

byte ip[] = { 10, 0, 0, 21 };

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  server.begin();
  pony.add(GET, "/", index);
  pony.add(POST, "/post", post);
}

void loop() {
  Client client = server.available();
  if (client) {
    pony.listen(client);
  }
}
