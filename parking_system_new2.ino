#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#include <SPI.h>

#include <DHT.h>

#define IRPin A0
#define model 1080
#define DHTPIN D6
#define DHTTYPE DHT22
// Set these to run example.
#define FIREBASE_HOST "parkingspaces-e0315-default-rtdb.europe-west1.firebasedatabase.app"
#define FIREBASE_AUTH "EUaLihEoFl6BaXKJaXS7bn84VMTrmTGy39TdG2Qh"
#define WIFI_SSID "MEO-2F41D0"
#define WIFI_PASSWORD "M3artysuaU"




DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

//Variables
int chk;
float hum;  //Stores humidity value
float temp1; //Stores temperature value


int fsrAnalogPin = A0; // FSR is connected to analog 1
int fsrReading;      // the analog reading from the FSR resistor divider
int distance_cm;


int sensorPin = A0;


int led1 = D3; //Amarelo
int led3 = D7; //Vermelho
int led2 = D8; //Verde
int reservado = 0;

const int trigPin = D5;
const int echoPin = D4;
int duration, distance;

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  if (Firebase.failed()) {
   Serial.print("ERROR FIREBASE:");
   Serial.println(Firebase.error());  
   return;
  }


}

void loop() {
  Firebase.getInt("reservado");
   // handle error
  if (Firebase.failed()) {
   Serial.print("get int failed:");
   Serial.println(Firebase.error());  
   return;
  }
  // put your main code here, to run repeatedly:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;
  Firebase.setInt("distance",distance);
  // handle error
  if (Firebase.failed()) {
   Serial.print("setting /number failed 1 :");
   Serial.println(Firebase.error());  
   return;
  }

  delay(100);
  
  hum = dht.readHumidity();
  temp1= dht.readTemperature();
  Firebase.setInt("temperatura",temp1);
   // handle error
  if (Firebase.failed()) {
   Serial.print("setting /number failed 2 :");
   Serial.println(Firebase.error());  
   return;
  }
  // Serial.print("Humidity: ");
  // Serial.print(hum);
  Serial.print(" %, Temp: ");
  Serial.print(temp1);
  Serial.println(" Celsius");
  
  fsrReading = analogRead(fsrAnalogPin);
  Serial.print("Pressure = ");
  Serial.println(fsrReading);
  Firebase.setInt("pressure",fsrReading);
  // handle error
  if (Firebase.failed()) {
    Serial.print("setting /number failed 3:");
    Serial.println(Firebase.error());  
    return;
  }
  
  
  
  Serial.print("distance  : ");
  Serial.print(distance);
  Serial.println(" cm");

  
 // Parking Slot 1 IR & Led
 if (distance > 20 && fsrReading < 10)       
       {
         digitalWrite(led3, HIGH);
         digitalWrite(led2, LOW);
         
       }

 if (distance < 20 && fsrReading > 10){   
    if (reservado > 0){
      digitalWrite(led1,LOW);
      Firebase.setInt("reservado",0);
      // handle error
      if (Firebase.failed()) {
           Serial.print("setting /number failed 4:");
           Serial.println(Firebase.error());  
           return;
       }
    }
    digitalWrite(led3, LOW);
    digitalWrite(led2, HIGH);
    Serial.print("###################################################################### ");
  }

 if (reservado > 0){
  digitalWrite(led1,HIGH);
  digitalWrite(led3, LOW);
  digitalWrite(led2, LOW);
 }

}
