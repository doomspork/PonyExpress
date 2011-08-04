# Pony (Express)
A small route dispatcher for Arduino's ethernet capabilities.

## Defining your destinations
Setting a destination is as easy as defining a method in your Arduino Sketch
that accepts a *PonyRequest* object and returns a character pointer.  
```char * index(PonyRequest request) {```  
```return "You've reached the index!";```
```}```

Now it's necessary to create an instance of the *Pony* object.  This allows us to tell Pony how we want it to use our methods.  
```Pony pony = Pony();```

Adding a method to Pony is as simple as telling Pony what type of request to apply to (GET, POST, PUT, DELETE), what URI to look for, and which method to call.  This is typically handled in the `setup()` method of the Arduino Sketch.  Pony tries to help you by defining GET, POST, PUT, and DELETE values.  
```pony.add(GET, "/", index);```

The last thing necessary to get  Pony running is to instruct it to listen within your `loop()` method.  To function properly Pony requires access to the Arduino Ethernet library's client object.  
```void loop() {  
  Client client = server.available();  
  if (client) {  
    pony.listen(client);  
  }  
}```

