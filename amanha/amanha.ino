// https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

#ifdef ESP8266
 #include <ESP8266WiFi.h>  // Pinos para esp8266 MCU
 #else
 #include <WiFi.h>  
#endif
 
#include <PubSubClient.h>
#include <WiFiClientSecure.h>

//---- WiFi settings
const char* ssid = "Techmax 2";
const char* password = "Automotivo#23";

//---- MQTT Broker settings
const char* mqtt_server = "b90868acd72847f29197841b836336e9.s2.eu.hivemq.cloud"; // Trocar por url gerada
const char* mqtt_username = "Teste"; // usuário gerado
const char* mqtt_password = "testando"; // senha do usuário gerado
const int mqtt_port =8883;

 

WiFiClientSecure espClient;   // Para conexão sem senha, comente essa linha e descomente a linha sob ela
//WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;

#define MSG_BUFFER_SIZE	(50)
char msg[MSG_BUFFER_SIZE];


int sensor1 = 0;
float sensor2 = 0;
int command1 = 0;

const char* sensor1_topic= "sensor1";
const char*  sensor2_topic="sensor2";
//const char*  sensor2_topic="sensor3";

const char* command1_topic="command1";
//const char* command1_topic="command2";





static const char *root_ca PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIFazCCA1OgAwIBAgIRAIIQz7DSQONZRGPgu2OCiwAwDQYJKoZIhvcNAQELBQAw
TzELMAkGA1UEBhMCVVMxKTAnBgNVBAoTIEludGVybmV0IFNlY3VyaXR5IFJlc2Vh
cmNoIEdyb3VwMRUwEwYDVQQDEwxJU1JHIFJvb3QgWDEwHhcNMTUwNjA0MTEwNDM4
WhcNMzUwNjA0MTEwNDM4WjBPMQswCQYDVQQGEwJVUzEpMCcGA1UEChMgSW50ZXJu
ZXQgU2VjdXJpdHkgUmVzZWFyY2ggR3JvdXAxFTATBgNVBAMTDElTUkcgUm9vdCBY
MTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAK3oJHP0FDfzm54rVygc
h77ct984kIxuPOZXoHj3dcKi/vVqbvYATyjb3miGbESTtrFj/RQSa78f0uoxmyF+
0TM8ukj13Xnfs7j/EvEhmkvBioZxaUpmZmyPfjxwv60pIgbz5MDmgK7iS4+3mX6U
A5/TR5d8mUgjU+g4rk8Kb4Mu0UlXjIB0ttov0DiNewNwIRt18jA8+o+u3dpjq+sW
T8KOEUt+zwvo/7V3LvSye0rgTBIlDHCNAymg4VMk7BPZ7hm/ELNKjD+Jo2FR3qyH
B5T0Y3HsLuJvW5iB4YlcNHlsdu87kGJ55tukmi8mxdAQ4Q7e2RCOFvu396j3x+UC
B5iPNgiV5+I3lg02dZ77DnKxHZu8A/lJBdiB3QW0KtZB6awBdpUKD9jf1b0SHzUv
KBds0pjBqAlkd25HN7rOrFleaJ1/ctaJxQZBKT5ZPt0m9STJEadao0xAH0ahmbWn
OlFuhjuefXKnEgV4We0+UXgVCwOPjdAvBbI+e0ocS3MFEvzG6uBQE3xDk3SzynTn
jh8BCNAw1FtxNrQHusEwMFxIt4I7mKZ9YIqioymCzLq9gwQbooMDQaHWBfEbwrbw
qHyGO0aoSCqI3Haadr8faqU9GY/rOPNk3sgrDQoo//fb4hVC1CLQJ13hef4Y53CI
rU7m2Ys6xt0nUW7/vGT1M0NPAgMBAAGjQjBAMA4GA1UdDwEB/wQEAwIBBjAPBgNV
HRMBAf8EBTADAQH/MB0GA1UdDgQWBBR5tFnme7bl5AFzgAiIyBpY9umbbjANBgkq
hkiG9w0BAQsFAAOCAgEAVR9YqbyyqFDQDLHYGmkgJykIrGF1XIpu+ILlaS/V9lZL
ubhzEFnTIZd+50xx+7LSYK05qAvqFyFWhfFQDlnrzuBZ6brJFe+GnY+EgPbk6ZGQ
3BebYhtF8GaV0nxvwuo77x/Py9auJ/GpsMiu/X1+mvoiBOv/2X/qkSsisRcOj/KK
NFtY2PwByVS5uCbMiogziUwthDyC3+6WVwW6LLv3xLfHTjuCvjHIInNzktHCgKQ5
ORAzI4JMPJ+GslWYHb4phowim57iaztXOoJwTdwJx4nLCgdNbOhdjsnvzqvHu7Ur
TkXWStAmzOVyyghqpZXjFaH3pO3JLF+l+/+sKAIuvtd7u+Nxe5AW0wdeRlN8NwdC
jNPElpzVmbUq4JUagEiuTDkHzsxHpFKVK7q4+63SM1N95R1NbdWhscdCb+ZAJzVc
oyi3B43njTOQ5yOf+1CceWxG1bQVs5ZufpsMljq4Ui0/1lvh+wjChP4kqKOJ2qxq
4RgqsahDYVvTH9w7jXbyLeiNdd8XM2w9U/t7y0Ff/9yi0GE44Za4rF2LN9d11TPA
mRGunUHBcnWEvgJBQl9nJEiU0Zsnvgc/ubhPgXRR4Xq37Z0j4r7g1SgEEzwxA57d
emyPxgcYxn/eR44/KJ4EBs+lVDR3veyJm+kXQ99b21/+jh5Xos1AnX5iItreGCc=
-----END CERTIFICATE-----
)EOF";  //só nao mexe


//==========================================
void setup_wifi() {
  delay(10);
  Serial.print("\nConectando na rede:  ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("\nWiFi conectedo\nIP da rede: ");
  Serial.println(WiFi.localIP());
}


//=====================================
void reconnect() {
  // Loop até conectar
  while (!client.connected()) {
    Serial.print("Tentando conectar ao MQTT...");
    String clientId = "ESP32Client-";   // Create a random client ID
    clientId += String(random(0xffff), HEX);
    // Quando conecta
    if (client.connect(clientId.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("conectado");

      client.subscribe(command1_topic);   // adiciona os tópicos aqui
      
    } else {
      Serial.print("falha, reconectando=");
      Serial.print(client.state());
      Serial.println(" tente novamente em 5 segundos");   // Aguarda 5 seg antes de reiniciar
    }
  }
}

//================================================ setup
//================================================
void setup() {
  Serial.begin(115200);
  while (!Serial) delay(1);
  setup_wifi();
  pinMode(BUILTIN_LED, OUTPUT);     // Inicializa o LED interno


  #ifdef ESP8266
    espClient.setInsecure();
  #else   // para o ESP32
    espClient.setCACert(root_ca);      // habbilite essa linha e o certificado para conexão segura
  #endif
  
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}


//================================================ loop
//================================================
void loop() {

  if (!client.connected()) reconnect();
  client.loop();

  //---- exemplo: publicando valor dos sensores a cada 5 segundos
  unsigned long now = millis();
  if (now - lastMsg > 10) {
    lastMsg = now;
    sensor1= random(50);       // Colocar o valor dos sensores aqui
    sensor2= 20+random(80);    // Colocar o valor dos sensores aqui
    publishMessage(sensor1_topic,String(sensor1),true);    
    publishMessage(sensor2_topic,String(sensor2),true);
    
  }
}

//=======================================  
// Esse void é chamado toda vez que se tem ualgo chegando do broker

void callback(char* topic, byte* payload, unsigned int length) {
  String incommingMessage = "";
  for (int i = 0; i < length; i++) incommingMessage+=(char)payload[i];
  
  Serial.println("Mensagem recebida ["+String(topic)+"]"+incommingMessage);
  
  //--- checa mensagem que foi recebida
    if( strcmp(topic,command1_topic) == 0){
     if (incommingMessage.equals("1")) digitalWrite(BUILTIN_LED, LOW);   //  LED on 
     else digitalWrite(BUILTIN_LED, HIGH);  //  LED off 
  }

   //  checa outros comandos
 /*  else  if( strcmp(topic,command2_topic) == 0){
     if (incommingMessage.equals("1")) {  } // faça algo
  }
  */
}



//======================================= enviando como string
void publishMessage(const char* topic, String payload , boolean retained){
  if (client.publish(topic, payload.c_str(), true))
      Serial.println("Mensagem enviada ["+String(topic)+"]: "+payload);
}