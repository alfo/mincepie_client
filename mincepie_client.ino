/*
  Web client
 
 This sketch connects to a website (http://www.google.com)
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 created 18 Dec 2009
 modified 9 Apr 2012
 by David A. Mellis
 
 */

#include <SPI.h>
#include <Ethernet.h>

// Network Settings

byte mac[] = {  0x90, 0xA2, 0xDA, 0x09, 0x00, 0x03 }; // MAC Found on sticker
IPAddress ip(10, 0, 1, 105);
IPAddress gateway(10, 0, 1, 1);
byte server[] = {10 , 0 , 1 , 8 }; // Google

// LED Settings
int rLED = 3;
int gLED = 5;
int bLED = 6;

int buttonState = 0;

// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

void setup() {
  // Set up RGB LED
  pinMode(rLED, OUTPUT);
  pinMode(gLED, OUTPUT);
  pinMode(bLED, OUTPUT);
  pinMode(2, INPUT);
  
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
  Serial.println("Connected to Serial");

  // start the Ethernet connection:
  Ethernet.begin(mac, ip);
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");
  
  
 // print your local IP address:
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print("."); 
  }
  
  Serial.println();
  
  setLED(160, 255, 97); // Set the LED to begin with
  
  digitalWrite(2, HIGH);

}

void loop()
{
  if (handleButton()) {
    setLED(255,100,0);
    eat();
    setLED(160, 255, 97);
    delay(5000);
  }
}

void eat() {
  if (client.connect(server, 80)) {  //starts client connection, checks for connection
    Serial.println("connected");
    client.println("GET /mincepie/eat.php HTTP/1.0"); //download text
    client.println(); //end of get request
  } 
  else {
    Serial.println("connection failed"); //error message if no client connect
    Serial.println();
  }

  while(client.connected() && !client.available()) delay(1); //waits for data
// add counter as an attempt to bail out if too much input comes in
//
  int counter = 0;
  while (client.connected() || client.available()) { //connected or data available
    char c = client.read(); //gets byte from ethernet buffer
    Serial.print(c); //prints byte to serial monitor 
    counter = counter + 1;
    if (counter >= 500 ) {
      exit;  
    }
  }
  Serial.println();
  Serial.println("disconnecting.");
  Serial.println("==================");
  Serial.println();
  client.stop(); //stop client
}

void setLED(int r, int g, int b) {
  analogWrite(rLED, 255-r);
  analogWrite(gLED, 255-g);
  analogWrite(bLED, 255-b);
}

boolean handleButton() {
  int buttonPressed = !digitalRead(2);
  return buttonPressed;
}
