/*
 Basic ESP8266 MQTT example

 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.

 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off

 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.

 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"

*/
/*const char* ssid = "fanavaran";
const char* password = "NaTa1300LKj#@";*/


#include <ESP8266WiFi.h>
#include <PubSubClient.h>


// Update these with values suitable for your network.


const char* ssid = "MobinNet0660";
const char* password = "460D0660";
const char* mqtt_server = "192.168.1.193";


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
int value = 0;
char msg_createIndex[] = "{\"index\":\"10\",\"controller\":\"index\","
              "\"action\":\"create\",\"requestId\":\"reqid\",\"body\":{\"message\":\"some\"}}";
              /* JSON OBJECT */
//{"index":"10","controller":"index","action":"create","requestId":"reqid","body":{"message":"some"}}


char msg[] = "{\"index\":\"index2\",\"collection\":\"collection3\",\"controller\":\"collection\","
              "\"action\":\"create\",\"requestId\":\"reqid\",\"body\":{\"message\":\"some\"}}";
              /* JSON OBJECT*/
 //{"index":"index","collection":"collection2","controller":"collection","action":"create","requestId":"reqid","body":{"message":"some"}}
    
              
char msg_updateDoc[] = "{\"index\":\"devices\",\"collection\":\"active_nodes\",\"controller\":\"document\","
                        "\"action\":\"update\",\"refresh\":\"wait_for\",\"_id\":\"nodeB\",\"body\":{\"status\":false}}";
                        /* JSON OBJECT */
//{"index":"devices","collection":"active_nodes","controller":"document","action":"update","refresh":"wait_for","_id":"nodeB","body":{"status":false}}


char msg_createOrReplaceDoc[] = "{\"index\":\"index\",\"collection\":\"collection2\",\"controller\":\"document\","
                        "\"action\":\"createOrReplace\",\"_id\":\"reqid\",\"body\":{\"firstName\":\"Farimaah\",\"lastName\":\"Anvari\"}}";
                        /* JSON OBJECT */
//{"index":"index","collection":"collection","controller":"document","action":"createOrReplace","_id":"requestid","body":{"firstName":"Farimaah","lastName":"Anvari"}}




void setup_wifi() {

  delay(100);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      Serial.print(client.state());
      // Once connected, publish an announcement...
      client.publish("Kuzzle/request",msg);
      Serial.println(msg);
      // ... and resubscribe
      client.subscribe("Kuzzle/response");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  client.subscribe("Kuzzle/response");
  long now = millis();
  if (now - lastMsg > 120000) {
    lastMsg = now;
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("Kuzzle/request",msg);
  }
}
