#include <ESP8266WiFi.h>
WiFiServer server(80);  //Initialize the server on Port 80

//int LED_PIN = 2; //or 16; //same thing
int LED_PIN_OFF = 0; // use GPIO0 (D3) for the LED
int LED_PIN_ON = 14; // use GPIO14 (D5) for the second LED

void setup() {


  WiFi.mode(WIFI_AP);                          //Our ESP8266-12E is an AccessPoint
  WiFi.softAP("FREE-http://192.168.4.1", "");  // Provide the (SSID, password) level blank for no password
  server.begin();
  // Start the HTTP Server... 198.168.4.1   -on phone comes up as sometimes as 192.168.4.2 try .1 first
  Serial.begin(115200);                        //Start communication between the ESP8266-12E and the monitor window
  IPAddress HTTPS_ServerIP = WiFi.softAPIP();  // Obtain the IP of the Server
  Serial.print("Server IP is: ");              // Print the IP to the monitor window
  Serial.println(HTTPS_ServerIP);
  //lcd.print(HTTPS_ServerIP);
  pinMode(LED_PIN_ON, OUTPUT); 
  pinMode(LED_PIN_OFF, OUTPUT);
  digitalWrite(LED_PIN_ON, LOW);  //Initial state is OFF
  digitalWrite(LED_PIN_OFF, LOW);
}

void loop() {
  /* check to see if client has connected.....*/
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  //Looking under the hood
  Serial.println("Connected");
  //Read what the browser has sent into a String class and print the request to the monitor
  //only interested in first line so replace....Enter the following in the address field of your
  //browser http://192.168.4.1/PARAM.
  //The browser sends a GET request to the server. Notice "/PARAM" following the GET request. Off all the
  //text sent we are only interested in the first line of the request. Thus we replace the code
  //String request = client.readString(); with String request = client.readStringUntil('\r');
  String request = client.readStringUntil('\r');
  //Looking under the hood
  Serial.println(request);
  //Handle the Request
  //In the address bar of your browse type the following URL:
  //http://192.168.4.1/off The LED on the ESP8266-E12 will turn OFF.
  //Then type the following URL:http://192.168.4.1/on
  if (request.indexOf("/off") != -1) {
    digitalWrite(LED_PIN_OFF, HIGH);
    delay(3000);
    digitalWrite(LED_PIN_OFF, LOW);
    
  } else if (request.indexOf("/on") != -1) {
    digitalWrite(LED_PIN_ON, HIGH);
    delay(3000);
    digitalWrite(LED_PIN_ON, LOW);
      
  }

  // Prepare the HTML document to respond and add buttons:
  String s = "HTTP/1.1 200 ";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";
  s += "<br><br><br>Ready to rock,";
  s += "<br>Press a button!";
  s += "<br><br><input type=\"button\" name=\"b1\" value=\"Turn LED  ON\" onclick=\"location.href='/on'\" style=\"font-size: 24px; padding: 10px 20px;\">";
  s += "<br><br>";
  s += "<br><br><input type=\"button\" name=\"b1\" value=\"Turn LED OFF\" onclick=\"location.href='/off'\" style=\"font-size: 24px; padding: 10px 20px;\">";
  //s += "<br><br><input type=\"text\" name=\"b2\">";
  //s += "<input type=\"submit\" value=\"Upload Text \" onclick=\"location.href='>%b2%<'\">";
  s += "</html>\n";
  Serial.println(request);
  client.flush();   //clear previous info in the stream
  client.print(s);  // Send the response to the client
  delay(1);
  Serial.println("Client disonnected");
}
