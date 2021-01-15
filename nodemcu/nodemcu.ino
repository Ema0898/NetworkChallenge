#include <PubSubClient.h>
#include <ESP8266WiFi.h>

const char* ssid = "Familia Sanchez";
const char* password = "sanchez1";
const char* mqttServer = "192.168.0.24";
const int mqttPort = 1883;
const char* mqttUser = "esp8266_user";
const char* mqttPassword = "esp82661234";

WiFiClient espClient;
PubSubClient client(espClient);

void turnOff() {
  digitalWrite(D0, LOW);
  digitalWrite(D1, LOW);
}

void turnOn() {
  digitalWrite(D0, HIGH);
  digitalWrite(D1, HIGH);
}

void callback(char* topic, byte* payload, unsigned int length) { 
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
 
  Serial.print("Message:");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }

  if ((char)payload[0] == '1') {
    turnOff();
    digitalWrite(D0, OUTPUT);
    delay(100);
  } else if ((char)payload[0] == '2') {
    turnOff();
    digitalWrite(D1, OUTPUT);
    delay(100);
  } else if ((char)payload[0] == '3') {
    turnOn();
    delay(100);
  }
 
  Serial.println();
  Serial.println("-----------------------"); 
}

void reconnect() {
  uint8_t retries = 3;
  
  while (!client.connected()) {
    Serial.print("Intentando conexion MQTT...");   
    
    if (client.connect("ESP8266Client", mqttUser, mqttPassword)) {
    //if (client.connect("ESP8266Client")) {
      Serial.println("conectado");
      client.subscribe("piCam");
    } else {
      Serial.print("fallo, rc=");
      Serial.print(client.state());
      Serial.println(" intenta nuevamente en 5 segundos");
      delay(5000);
    }
    retries--;
    if (retries == 0) {
      while (1);
    }
  }
}

void setup() {
  
  Serial.begin(9600);

  pinMode(D0, OUTPUT);
  pinMode(D1, OUTPUT);

  WiFi.begin(ssid, password);
 
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
   }   
    
   client.setServer(mqttServer, mqttPort);
   client.setCallback(callback);

   while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
 
    if (client.connect("ESP8266Client", mqttUser, mqttPassword)) {
    //if (client.connect("ESP8266Client")) { 
      Serial.println("connected");  
 
    } else { 
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000); 
    }
  }

  client.subscribe("piCam");
}

void loop() {

  if (!client.connected()) {
    Serial.println("Disconnected");
    reconnect();
  }

  client.loop(); 
  
  delay(250); 
}
