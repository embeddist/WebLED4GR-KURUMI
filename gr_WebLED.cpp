/*
  Web Server & LED Blink

 A simple web server that shows and control the value of the LED Digital.
 using an Arduino Wiznet Ethernet shield.

 This sketch is merged Web-server and LED Blink.
 You can do controlling and monitoring the RGB LED on GR KURUMI Board via Internet or Local Network.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Pin 22,23,24 has an LED connected on most Arduino boards.

 made 23 Aug 2013
 by Soohwan Kim

 Thanks to David A. Mellis & Tom Igoe
 */
#include <SPI.h>
#include <Ethernet.h>
void blink();
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
#if defined(WIZ550io_WITH_MACADDRESS) // Use assigned MAC address of WIZ550io
;
#else
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
#endif
IPAddress ip(192,168,1,177);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

// Pin 22,23,24 has an LED connected on most Arduino boards.
// give it a name:
int led_red   = 22;
int led_green = 23;
int led_blue  = 24;
bool flag_blink = 0;

void setup() {

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // start the Ethernet connection and the server:
#if defined(WIZ550io_WITH_MACADDRESS)
  Ethernet.begin(ip);
#else
  Ethernet.begin(mac, ip);
#endif
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

  // initialize the digital pin as an output.
  pinMode(led_red, OUTPUT);
  pinMode(led_green, OUTPUT);
  pinMode(led_blue, OUTPUT);

}


void loop() {
  int parse_cnt = 0;
  char parse_arr[3];
  //digitalWrite(led_red, HIGH);   // turn the RED LED on (HIGH is the voltage level)
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);

	   // 5th~7th data of HTTP GET Message is parsed as parse_arr
	   // When web-page address is http://192.168.1.177/G0,
	   //
	   // parse_arr[0] = '/'
	   // parse_arr[1] = 'G'
	   // parse_arr[2] = '0'
	   //
	   // GET /G0 HTTP/1.1
	   // Accept: text/html, application/xhtml+xml,....
	   //
       if(parse_cnt == 4){
          parse_arr[0] = c;
        }else if (parse_cnt == 5){
          parse_arr[1] = c;
        }else if (parse_cnt == 6){
          parse_arr[2] = c;
        }else{
        	//Clipping for overflow...
        	if (parse_cnt>8) parse_cnt--;
        }

        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {

          if(parse_arr[0] == '/'){
        	  switch(parse_arr[1]){
        	  case('R') :
					  if(parse_arr[2] == '0'){
						  digitalWrite(led_red, HIGH);
					  }else if(parse_arr[2] == '1'){
						  digitalWrite(led_red, LOW);
					  }
        	  	  break;
        	  case('G') :
					  if(parse_arr[2] == '0'){
						  digitalWrite(led_green, HIGH);
					  }else if(parse_arr[2] == '1'){
						  digitalWrite(led_green, LOW);
					  }
        		  break;
        	  case('B') :
					  if(parse_arr[2] == '0'){
						  digitalWrite(led_blue, HIGH);
					  }else if(parse_arr[2] == '1'){
						  digitalWrite(led_blue, LOW);
					  }else if(parse_arr[2] == '2'){ //Blink start
        	            	  flag_blink=1;
					  }else if(parse_arr[2] == '3'){ //Blink stop
        	            	  flag_blink=0;
					  }
        		  break;
        	  }
          }


          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
	      client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          int sensorReading = digitalRead(led_red);
          client.print("digitalRead(LED_RED)");//client.print(led_red);
          client.print(" is ");
          client.print(sensorReading);
          client.println("<br />");
          sensorReading = digitalRead(led_green);
          client.print("digitalRead(LED_GREEN)");//client.print(led_green);
		  client.print(" is ");
		  client.print(sensorReading);
		  client.println("<br />");
		  sensorReading = digitalRead(led_blue);
		  client.print("digitalRead(LED_BLUE)");//client.print(led_blue);
		  client.print(" is ");
		  client.print(sensorReading);
		  client.println("<br />");
          client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
      parse_cnt++;
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }

  if(flag_blink==1){
	  blink();
  }

}

//Turns on an LED on for half second, then off for one half, repeatedly
void blink() {
  digitalWrite(led_red, HIGH);   // turn the RED LED on (HIGH is the voltage level)
  delay(500);                   // wait for a second
  digitalWrite(led_red, LOW);    // turn the RED LED off by making the voltage LOW

  digitalWrite(led_green, HIGH); // turn the GREEN LED on (HIGH is the voltage level)
  delay(500);                   // wait for a second
  digitalWrite(led_green, LOW);  // turn the GREEN LED off by making the voltage LOW

  digitalWrite(led_blue, HIGH);  // turn the BLUE LED on (HIGH is the voltage level)
  delay(500);                   // wait for a second
  digitalWrite(led_blue, LOW);   // turn the BLUE LED off by making the voltage LOW
}

