Web LED for GR-KURUMI
================
<markdown>
## Web Server & LED Blink
A simple web server that shows and control the value of the LED Digital using an Arduino Wiznet Ethernet shield. This sketch for GR-KURUMI is merged Web-server and LED Blink. You can do controlling and monitoring the RGB LED on GR KURUMI Board via Internet or Local Network.  
After opening a web-browser and navigating to the Ethernet shield’s IP address, the GR-KURUMI will respond with just enough HTML for a browser to display the data. A RGB LED on GR-KURUMI is also controlled by the request of web-browser.


## Circuit
 * Ethernet shield attached to pins 10, 11, 12, 13  
 * Pin 22,23,24 has an LED connected on most Arduino boards.
    - Pin22: LED_RED
    - Pin23: LED_GREEN
    - Pin24: LED_BULE
 

## SW
 * Download WIZnet Library - [Wiz_Ethernet_Library](https://github.com/Wiznet/WIZ_Ethernet_Library)  
 * Download Ethernet Shield example for GR-KURUMI - [Ethernet Shield examples](https://github.com/Wiznet/gr_ethernet_examples)  
 
 * gr_WebLED.cpp - Web Server & LED Blink


## Code
 
 * Configuration of IP address  
```c
IPAddress ip(192,168,1,177);
```

 * Initalize the Port (port80 is default for HTTP)  
```c
EthernetServer server(80);
```

 * Parsing of HTTP GET Message  

  When web-page address is http://192.168.1.177/G0 to on the Green LEN,
  the received HTTP GET Message is as below,  
```c
  GET /G0 HTTP/1.1  
  Accept: text/html, application/xhtml+xml,....
```

  So, 5th~7th data are parsed to control RGB LEDs.  
```c 
  //5th~7th data of HTTP GET Message is parsed as parse_arr  
  parse_arr[0] = '/'  
  parse_arr[1] = 'G'  
  parse_arr[2] = '0'  
```
 * Contorl RGB LEDs as data pased from HTTP GET Message  
```c 
 if(parse_arr[0] == '/'){  
   switch(parse_arr[1]){  
   case('R') :  
   // http://192.168.1.177:R0 => Red LED OFF			  	  
   if(parse_arr[2] == '0'){    
	   digitalWrite(led_red, HIGH);  
   // http://192.168.1.177:R1 => Red LED ON    			  
   }else if(parse_arr[2] == '1'){  
           digitalWrite(led_red, LOW);  
   }  
           break;  
  ... 
```  

 * Send a standard http response  
  Check current LED status as digitalRead() and Send http respoonse inclued the LED status.  
```c 
          int sensorReading = digitalRead(led_red);
          client.print("digitalRead(LED_RED)");//client.print(led_red);
          client.print(" is ");
          client.print(sensorReading);
          client.println("<br />");
  ... 
```

</markdown>
