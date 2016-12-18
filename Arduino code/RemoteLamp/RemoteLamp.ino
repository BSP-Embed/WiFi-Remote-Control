/*******************************************************************
Written by Marco Schwartz for Open Home Automation.
BSD license, all text above must be included in any redistribution
Based on the original sketches supplied with the ESP8266/Arduino
implementation written by Ivan Grokhotkov
Modified by bsp.embed@gmail.com.
Check our YouTube channel BSPEmbed
******************************************************************/

/* Required libraries */
#include <ESP8266WiFi.h>

/* WiFi Parameters of Your Router */
const char* ssid = "WIFISSID";
const char* password = "PASSWORD";

/* Create an instance of the server */
WiFiServer server(80);

/* Port Pins */
int output_pin = 5;
int Indicator_pin = 16;


void setup() {
  Serial.begin(115200);
  delay(10);
  pinMode(output_pin, OUTPUT);
  pinMode(Indicator_pin, OUTPUT);
  digitalWrite(output_pin, 0);
  Serial.println();                 /* For debug console */
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);       /* Connect to WiFi network */

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");

  server.begin();                   /* Start the server */
  Serial.println("Server started");
  Serial.println(WiFi.localIP());  /* Print the IP address */
  digitalWrite(Indicator_pin, 1);  /* Indicate by LED */
}


void loop() {
  
  WiFiClient client = server.available(); /* Check if a client has connected */
  
  if (!client) 
    return;
    
  Serial.println("new client");          /* Wait until the client sends some data */
  while(!client.available())
    delay(1);
  
  String req = client.readStringUntil('\r'); /* Read the first line of the request */
  Serial.println(req);
  client.flush();

  if (req.indexOf("/on") != -1)           /* check the request */
    digitalWrite(output_pin, 1);
  else if (req.indexOf("/off") != -1) 
    digitalWrite(output_pin, 0);
  
  client.flush();               /* Prepare the response */
 
  String s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  s += "<head>";
  s += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">";
  s += "<script src=\"https://code.jquery.com/jquery-2.1.3.min.js\"></script>";
  s += "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.4/css/bootstrap.min.css\">";
  s += "</head>";
  s += "<div class=\"container\">";
  s += "<h1>Lamp Control</h1>";
  s += "<div class=\"row\">";
  s += "<div class=\"col-md-2\"><input class=\"btn btn-block btn-lg btn-primary\" type=\"button\" value=\"On\" onclick=\"on()\"></div>";
  s += "<div class=\"col-md-2\"><input class=\"btn btn-block btn-lg btn-danger\" type=\"button\" value=\"Off\" onclick=\"off()\"></div>";
  s += "</div></div>";
  s += "<script>function on() {$.get(\"/on\");}</script>";
  s += "<script>function off() {$.get(\"/off\");}</script>";

  client.print(s);       /* Send the response to the client */
  delay(1);
  Serial.println("Client disconnected");

  /* The client will actually be disconnected */
  /* when the function returns and 'client' object is detroyed */
}
