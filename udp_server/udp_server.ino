#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid     = "NODEMCU";
const char* password = "password";

WiFiUDP UDPTestServer;
unsigned int UDPPort = 12312;

const int packetSize = 4;
byte packetBuffer[packetSize];

void setup() {
  Serial.begin(115200);
  delay(10);

  Serial.println();
  Serial.println();
  Serial.print("Making Access point ");
  Serial.println(ssid);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password,6, false);

  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);
  
  //WiFi.begin(ssid, password);
  //WiFi.config(IPAddress(192, 168, 1, 60), IPAddress(192, 168, 1, 1), IPAddress(255, 255, 255, 0));

  //while (WiFi.status() != WL_CONNECTED) {
  //  delay(500);
  //  Serial.print(".");
  //}

  //  Serial.println("");
  //  Serial.println("WiFi connected");
  //Serial.println("IP address: ");
  //Serial.println(WiFi.localIP());

  UDPTestServer.begin(UDPPort);

}

int value = 0;

void loop() {
  handleUDPServer();
  delay(1);
}

void handleUDPServer() {
  int cb = UDPTestServer.parsePacket();
  int packetlength;
  if (cb) {
    if(cb<packetSize){
      packetlength=cb;
    }else{
      packetlength=packetSize;
    }
    UDPTestServer.read(packetBuffer, packetSize);
    String myData = "";
    for (int i = 0; i < packetlength; i++) {
      myData += (char)packetBuffer[i];
    }
    Serial.println(myData);
  }
}
