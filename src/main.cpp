#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "CESJT";
const char* password = "itisjtsmg";
int est;
const int ldr = 25;
int lightlvl;
// Configuro sv asincrono
AsyncWebServer server(80);

// No cambia la página
const char pagina_template[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang ="es">
<head>
    <meta charset="UTF-8">
    <title>Monitor de Luz Ambiental</title>
    <style>
        body {
            background-color: COLOR; 
            color: white;
        }
        .titulo{
            font-family: 'Segoe UI';
            text-align: center;
        }
        .lectura{
            border: 3px dotted white;
            border-radius: 20px;
            padding: 25px;
            font-size: 40px;
        }
    </style>
    <body>
        <h1 class = "titulo">Monitor de Luz Ambiental</h1>
        <div class = "lectura">
            <span>Nivel de Luz: <b>XXX%</b></span>
        </div>
    </body>
</head>
</html>
)rawliteral";


// Funciones dedicadas para cada ruta (/...)

// Función para manejar la petición a la página principal (/)
void handleRootRequest(AsyncWebServerRequest *request) {
    String pagina = pagina_template; //hago una variable para no modificar a la original
    //parte para cambiar lo que dice el boton una vez que se prende o apaga
    lightlvl = analogRead(ldr);
    int lightP = (lightlvl * 100) / 1023;
    String colorFondo;
    if(lightlvl <= 341){
      colorFondo = "#2c3e50";
    }else if(lightlvl > 341 && lightlvl <= 682){
      colorFondo = "#e67e22";
    }else if(lightlvl > 682 && lightlvl <= 1023){
      colorFondo = "#f1c40f";
    }
    
    //pagina.replace busca el texto y lo reemplaza con lo que pongas. 
    pagina.replace("XXX", String(lightP));
    pagina.replace("COLOR", colorFondo);
    request->send(200, "text/html", pagina);
}

void setup() {
  pinMode(ldr, INPUT);
  Serial.begin(115200);
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  // Conexion wifi como antes
  int timeout = 20; 
  while (WiFi.status() != WL_CONNECTED && timeout > 0) {
    delay(500);
    Serial.print(".");
    timeout--;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFallo la conexion. Reiniciando...");
    delay(1000);
    ESP.restart();
  }
  Serial.println("\nWiFi conectado!");
  Serial.print("Dirección IP: http://");
  Serial.println(WiFi.localIP());

  // defino las rutas para las request 
  // Ahora ponemos el nombre de nuestra función dedicada
  server.on("/", HTTP_GET, handleRootRequest);

  // Iniciar servidor
  server.begin();
}

void loop() {
  // El loop sigue libre para las tareas de hardware

}