#include <ESP8266WiFi.h>

const char* SSID = "VIZCAYA_COLIMA";
const char* password = "*ceuva.colima2022";

int foco_status = LOW;

WiFiServer serverHTTP(80);

void setup() {
  Serial.begin(9600);
  pinMode(D1, OUTPUT);
  digitalWrite(D1, LOW);

  WiFi.begin(SSID, password);

  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Wifi Connected");

  Serial.print("IP asignada: ");
  Serial.print(WiFi.localIP());
  Serial.println("");

  serverHTTP.begin();
  Serial.println("Servidor conectado");
}

void loop() {
  WiFiClient client = serverHTTP.available();
  if(!client){
    return;
  }

  Serial.println("Ya se conecto un cliente");

  while(!client.available()){
    delay(1);
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);

  if(request.indexOf("/FOCO=ON") != -1){
    digitalWrite(D1, HIGH);
    foco_status = HIGH;
  }
  if(request.indexOf("/FOCO=OFF") != -1){
    digitalWrite(D1, LOW);
    foco_status = LOW;
  }

  client.println("HTTP/1.1 200 OK");
  client.println("Content-type: text/html");
  client.println("");

  client.println("<!DOCTYPE HTML>");
  client.println("<html> ");
  client.println("<head> <meta charset='UTF-8'> </head>");
  client.println("<body>");
    client.print("Estado del foco: ");
    if(foco_status == LOW){
      client.println("Apagado");
    }else{
      client.println("Encendido");
    }
  client.println("<br>");
  client.println("<a href=\"/FOCO=ON\"\"><button>Encender</button></a>");
  client.println("<a href=\"/FOCO=OFF\"\"><button>Apagar</button></a><br />");
  client.println("<br>");
  client.println("</body>");
  client.println("</html> ");

  delay(1);
  client.stop();
  Serial.println("Cliente desconectado");
}
