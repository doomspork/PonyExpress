#include <SPI.h>
#include <Ethernet.h>

#include "cppfix.h"
#include "Pony.h"

Pony pony = Pony();
Server server = Server(80);

String index(PonyRequest request) {
  String response = "";
  response.concat("<html><body>");
  response.concat("<form method='POST' action='/post'>");
  response.concat("<input type='text' name='key' value='value'/>");
  response.concat("<input type='submit' value='Submit'/>");
  response.concat("</form>");
  response.concat("</body></html>");
  return response;
}

byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x59, 0x50 };  

byte ip[] = { 10, 0, 0, 21 };

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac, ip);
  server.begin();
  pony.add(GET, "/", index);
}

void loop() {
  Client client = server.available();
  if (client) {
    pony.listen(client);
  }
}
