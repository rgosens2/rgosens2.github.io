#include <ESP8266WiFi.h> 
#include <Adafruit_NeoPixel.h> 
 
const char* ssid = "TMNL-DA3531"; //SSID aus dem Router
const char* password = "qqqqqqqq12345678"; //Passwort für den Zugang zum WLAN
 
WiFiServer server(80); //Port auf welchem der Server laufen soll.

#define PIN D2 //RGB LED Chip auf dem Digitalen PIN D2
//"Einrichten" des NeopPixel, 1 NeoPixel auf dem digitalen Pin D2
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);


int redValue = 0;
int greenValue = 0;
int blueValue = 0;
int brightnessValue = 10;
 
void setup() {
  Serial.begin(115200); //Baudrate für die Serielle Geschwindigkeit.
  delay(10); //10ms. Warten damit die Seriele Kommunikation aufgebaut wurde.
   
  Serial.print("Aufbau der Verbindung zu: "); //Ausgabe der SSID auf der Seriellen Schnittstelle.
  Serial.println(ssid);
  
  WiFi.begin(ssid, password); //Initialisieren der Wifi Verbindung.
 
  while (WiFi.status() != WL_CONNECTED) { //Warten bis die Verbindung aufgebaut wurde.
    delay(500);
    //Einen Punkt auf der Seriellen Schnittstelle ausgeben so das der Benutzer erkennt dass, das Sketch noch läuft.
    Serial.print("."); 
  }
  //Bei erfolgreicher Verbindung wird der folgende Text ausgeben.
  Serial.print("Mit ");
  Serial.print(ssid);
  Serial.println(" erfolgreich verbunden!");

  server.begin(); // Starten des Servers.
  Serial.println("Server gestartet"); //Ausgabe auf der Seriellen Schnittstelle das der Server gestartet wurde.
  
  pixels.begin(); //initialisieren
 
  // Ausgabe der IP Adresse 
  Serial.print("Adresse : http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop() { 
   WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // Wenn sich ein Client verbunden hat solange warten bis Daten gesendet werden.
  Serial.println("Neuer Client verbunden.");
  while(!client.available()){
    delay(1);
  }

  //Lesen der Anfrage vom Client
  String request = client.readStringUntil('\r');
  Serial.print("Vom Client empfangen: ");
  Serial.println(request);
  client.flush();

  char d1 = '?';
  char d2 = '&';
 
  int counter = 0;
  for(int i=0;i<request.length();i++){
    char c = request.charAt(i);
    if(c == d1 || c == d2){
      String value = request.substring(i+3,i+6);
      int intValue = value.toInt();
      Serial.println(value);
      if(counter == 0){
       redValue = intValue;
      } else if(counter == 1){
        greenValue = intValue;
      } else if(counter == 2){
        blueValue = intValue;
      }else if(counter == 3){
        brightnessValue = intValue;
      }
      Serial.println("found");
      counter++;
    }
  }
    
  writeResponse(client);
  updateRgbLed();
}

void updateRgbLed(){
 pixels.setPixelColor(0, pixels.Color(redValue, greenValue , blueValue )); 
 pixels.setBrightness(brightnessValue);
 pixels.show(); //Aktualisieren des NeoPixels
}

/**
 * Die Funktion gibt den HTML Kopf auf dem Client aus.
 * Dieses wird für jeden Respond verwendet.
 **/
void writeResponse(WiFiClient client){  
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>"); 
  client.println("<head>"); 
  client.println("<script type='text/javascript' src='http://progs.draeger-it.blog/wemosd1/jquerymin.js'></script>"); 
  client.println("<script type='text/javascript' src='https://rgosens2.github.io/functions.js'></script>"); 
  client.println("<link rel='stylesheet' href='https://rgosens2.github.io/slider.css'/>");   
  client.println("<link rel='stylesheet' href='https://rgosens2.github.io/styles.css'/>");   
  client.println("</head>"); 
  client.println("<body>");  
  client.println("<div class='wrapper'>"); 
  client.println("<h2>RGB LED Shield am Wemos D1 Mini</h2>");
  client.println("<form>");
  client.println("<div class='container'>");
  client.println("<img src='https://draeger-it.blog/wp-content/uploads/2018/01/wemos_mit_rgbled-1.jpg ' width='150' height='*'/>");   
  client.println("</div>"); 
  client.println("<div class='container'>");  
  client.println("<label for='redSlider'>Rot</label>"); 
  client.print("<div class='slidecontainer'><input type='range' min='0' max='255' value='");
  client.print(redValue);
  client.println("' class='slider' id='redSlider'/></div>");
  client.println("<label for='greenSlider'>Gr&uuml;n</label>"); 
  client.print("<div class='slidecontainer'><input type='range' min='0' max='255' value='");
  client.print(greenValue);
  client.println("' class='slider' id='greenSlider'/></div>");
  client.println("<label for='blueSlider'>Blau</label>"); 
  client.print("<div class='slidecontainer'><input type='range' min='0' max='255' value='");
  client.print(blueValue);
  client.println("' class='slider' id='blueSlider'/></div>");
  client.println("<label for='brightnessSlider'>Helligkeit</label>"); 
  client.print("<div class='slidecontainer'><input type='range' min='0' max='255' value='");
  client.print(brightnessValue);
  client.println("' class='slider' id='brightnessSlider'/></div>");
  client.println("</div>"); 
  client.println("<div class='previewContainer'>");
  client.println("<div class='preview'></div>");
  client.println("</div>");   
  client.println("<div style='clear:both'>"); 
  client.println("<input type='button' value='Absenden' onClick='sendValues();'/>"); 
  client.println("</form>"); 
  client.println("</div>");   
  client.println("</body>"); 
  client.println("</html>"); 
}
