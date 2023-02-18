
#include <ESP8266WiFi.h>
#include <FirebaseESP8266.h>  
#include <OneWire.h>
#include <DallasTemperature.h>

#define FIREBASE_HOST "noit-fc786-default-rtdb.europe-west1.firebasedatabase.app"//https://project-4235439483729091549-default-rtdb.firebaseio.com/
#define FIREBASE_AUTH "7A60LhM4iWiJN3ZIoOossH3p5ixRhhoveLhpnht6" //aXp8DjyzgH4jFWD41ZqLg41EoTtaJcy7j9qVJ1K4
#define WIFI_SSID "stoyan"  
#define WIFI_PASSWORD "12345677"  
FirebaseData firebaseData;

// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;     

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);


void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  
  Serial.println("Serial communication started\n\n");           
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                    
  //try to connect with wifi
  Serial.print("Connecting to ");
  Serial.print(WIFI_SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("Connected to ");
  Serial.println(WIFI_SSID);
  Serial.print("IP Address is : ");
  Serial.println(WiFi.localIP());//print local IP address
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);// connect to firebase
  Firebase.reconnectWiFi(true);
  delay(1000);
  
  // Start the DS18B20 sensor
  sensors.begin();

}

void loop() {
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  

  if (Firebase.pushInt(firebaseData, "/temperature", temperatureC))
  { 
  // On successful Write operation, function returns 1  
               Serial.println("Storingdata Uploaded Successfully");
               Serial.print(temperatureC);
               Serial.println("ºC");
               
               
     }
  else {        
    Serial.println(firebaseData.errorReason());
    }
  
  delay(5000);
}
