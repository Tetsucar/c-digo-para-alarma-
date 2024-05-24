//Enviar un mensaje de WhatsApp desde ESP32
#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "OTTOSINGO";
const char* password = "migel1914";
//COLOCAMOS EL TOKEN QUE NOS ENTREGA META
String token="Bearer EAAXZAE7ZCT4v8BO705vpb8yzTZBIZAREARjO1UITmvV0HCgDR6tMpjDUckUo7RSIWZA2GhhoFmaAd5zPTbjDnQtuQQHkZAqMdXIJtD3t2xN8Yxg1E5j64kCrUPQOSpv0YCa0ULc1ZBfixJHscN6CGdfxbmWkZAufoABL1eM7YNlaZCWiPxg3n0u128qxn2lIaK3MXcUiI2sNK4e5QSAFSBm8ZD";
//COLOCAMOS LA URL A DONDE SE ENVIAN LOS MENSAJES DE WHATSAPP
String servidor = "https://graph.facebook.com/v18.0/227161703822746/messages";
//CREAMOS UNA JSON DONDE SE COLOCA EL NUMERO DE TELEFONO Y EL MENSAJE PARA CUANDO ESTE LA PUERTA ABIERTA
String payload = "{\"messaging_product\":\"whatsapp\",\"to\":\"aqui va el numero de celular\",\"type\":\"text\",\"text\": {\"body\": \"la puerta se ha abierto\"}}";
//CREAMOS UNA JSON DONDE SE COLOCA EL NUMERO DE TELEFONO Y EL MENSAJE PARA CUANDO ESTE LA PUERTA CERRADA
String payloadC = "{\"messaging_product\":\"whatsapp\",\"to\":\"aqui va el numero de celular\",\"type\":\"text\",\"text\": {\"body\": \"la puerta esta cerrada\"}}";
//PIN DEL SENSOR DE MOVIMIENTO
const int pinSensorMov = 15;
//ESTADO DEL SENSOR
int estadoActual=HIGH;
int espNum=0;
void setup() {
  Serial.begin(9600);
  //ACTIVAMOS EL PIN SEL SENSOR DE MOVIMIENTO
  pinMode(pinSensorMov, INPUT);
  //COLOCAMOS USUARIO Y CONTRASEÑA DE NUESTRA RED WIFI
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Se ha conectado al wifi con la ip: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  //DETECTAMOS EL ESTADO DEL DETECTO DE MOVIMIENTO
  estadoActual = digitalRead(pinSensorMov);
  //SI SE DETECTA MOVIMIENTO
  if (estadoActual == LOW && espNum==1) {
    Serial.println("Hay Movimiento, puerta abierta");
    delay(5000);
    if(WiFi.status()== WL_CONNECTED){
      //INICIAMOS EL OBJETO HTTP QUE POSTERIORMENTE ENVIARA EL MENSAJE
      HTTPClient http;
      //COLOCAMOS LA URL DEL SERVIDOR A DONDE SE ENVIARA EL MENSAJE
      http.begin(servidor.c_str());
      //COLOCAMOS LA CABECERA DONDE INDICAMOS QUE SERA TIPO JSON
      http.addHeader("Content-Type", "application/json"); 
      //AGREGAMOS EL TOKEN EN LA CABECERA DE LOS DATOS A ENVIAR
      http.addHeader("Authorization", token);    
      //ENVIAMOS LOS DATOS VIA POST
      int httpPostCode = http.POST(payload);
      //SI SE LOGRARON ENVIAR LOS DATOS
      if (httpPostCode > 0) {
        //RECIBIMOS LA RESPUESTA QUE NOS ENTREGA META
        int httpResponseCode = http.GET();
        //SI HAY RESPUESTA LA MOSTRAMOS
        if (httpResponseCode>0) {
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
          String payload = http.getString();
          Serial.println(payload);
        }
        else {
          Serial.print("Error code: ");
          Serial.println(httpResponseCode);
        }
      }
      espNum=0;
      http.end();
    }
    else {
      Serial.println("WiFi Desconectado");
    }
    
  }//SI LA PUERTA ESTA CERRADA
  else if (estadoActual == HIGH && espNum==0) {
    Serial.println("Hay Movimiento, puerta cerrada");
    delay(5000);
    if(WiFi.status()== WL_CONNECTED){
      //INICIAMOS EL OBJETO HTTP QUE POSTERIORMENTE ENVIARA EL MENSAJE
      HTTPClient http;
      //COLOCAMOS LA URL DEL SERVIDOR A DONDE SE ENVIARA EL MENSAJE
      http.begin(servidor.c_str());
      //COLOCAMOS LA CABECERA DONDE INDICAMOS QUE SERA TIPO JSON
      http.addHeader("Content-Type", "application/json"); 
      //AGREGAMOS EL TOKEN EN LA CABECERA DE LOS DATOS A ENVIAR
      http.addHeader("Authorization", token);    
      //ENVIAMOS LOS DATOS VIA POST
      int httpPostCode = http.POST(payloadC);
      //SI SE LOGRARON ENVIAR LOS DATOS
      if (httpPostCode > 0) {
        //RECIBIMOS LA RESPUESTA QUE NOS ENTREGA META
        int httpResponseCode = http.GET();
        //SI HAY RESPUESTA LA MOSTRAMOS
        if (httpResponseCode>0) {
          Serial.print("HTTP Response code: ");
          Serial.println(httpResponseCode);
          String payloadC = http.getString();
          Serial.println(payloadC);
        }
        else {
          Serial.print("Error code: ");
          Serial.println(httpResponseCode);
        }
      }
      espNum=1;
      http.end();
    }
    else {
      Serial.println("WiFi Desconectado");
    }

  }
  //SI NO SE DETECTA MOVIMIENTO
  else{
    Serial.println("No hay Movimiento");
  }
  delay(1000);
}
