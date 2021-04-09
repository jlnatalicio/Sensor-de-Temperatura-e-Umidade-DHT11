//---------------------------------------------------------------
// Internet das Coisas - Trabalho 1º Semestre
//
// Alunos:                                R.As:
//        Eliana Harume Rodrigues Tamari       201404433
//        José Leonardo Natalício              817110728
//        Rafael de Oliveira Silva             201312986
//
// Turma: EET6AN-MCA                      Curso: Eng. Eletrônica
// Professor: Eng. Núncio Perrella
//
// Junho de 2019
//
//---------------------------------------------------------------
#include <WiFi.h>
#include <DHT.h>
#include <ThingSpeak.h>
#define DHTPIN 5 // DHT11 esta conectado no pino GPIO5 (D5) placa NodeMCU
#define DHTTYPE DHT11 // Definindo tipo de sensor que será utilizado
//#define ch_id 804223 // ID do canal

      DHT dht(DHTPIN, DHTTYPE);
      int led = 2; // Atribuição de led ao pino 2
      int number1 = 0;
      int number2 = random(0,100);
      String myStatus = "";
      String ApiKey = "NMLR12KIU7FKKLT2"; // Chave de Escrita Thingspeak - APIKEY
      const char* server = "api.thingspeak.com"; // Servidor
      
      // Parâmetros da rede WiFi
      const char* ssid = "ssid"; // Definir Rede Wifi a ser utilizada
      const char* password = "senha"; // Senha da rede Wifi
      char contadorString[6];
      WiFiClient client;
//---------------------------------------------------------------
void setup()
{
      Serial.begin(115200); // Velocidade de comunicação: 115200 bauds
      delay(50);
      dht.begin(); // Sensor DHT11 inicializado
      // WiFi.mode(WIFI_STA);
      ThingSpeak.begin(client); // Inicia ThingSpeak
      Serial.print("Conectando à ");
      Serial.print("[ ");
      Serial.print(ssid);
      Serial.println(" ]");
      WiFi.begin(ssid, password);
      while (WiFi.status() != WL_CONNECTED)
      {
            delay(500);
            Serial.print(".");
      }
      Serial.println("");
      Serial.println("*********************");
      Serial.println("** WiFi Conectado! **");
      Serial.println("*********************");
      
      // Mostra IP address
      Serial.println("Endereço IP: ");
      Serial.println(WiFi.localIP());
}
//---------------------------------------------------------------
void loop()
{
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      
      if (isnan(h) || isnan(t))
      {
            Serial.println("Falha na leitura do sensor!");
            return;
      }
      
      if (client.connect(server,80))
      {
            String postStr = ApiKey;
            postStr +="&field1=";
            postStr += String(t);
            postStr +="&field2=";
            postStr += String(h);
            postStr += "\r\n\r\n";
            client.print("POST /update HTTP/1.1\n");
            client.print("Host: api.thingspeak.com\n");
            client.print("Connection: close\n");
            client.print("X-THINGSPEAKAPIKEY: "+ApiKey+"\n");
            client.print("Content-Type: application/x-www-form-urlencoded\n");
            client.print("Content-Length: ");
            client.print(postStr.length());
            client.print("\n\n");
            client.print(postStr);
            Serial.print("Temperatura: ");
            Serial.print(t);
            Serial.println(" ºC");
            Serial.print("Umidade: ");
            Serial.print(h);
            Serial.println("%. Enviado para Thingspeak.");
      }
      delay(20000);
      client.stop();
      Serial.println("Aguardando leitura...");
      delay(10000);
}
