#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <SPI.h>
#include <DHT.h>

#define IRPin A0
#define model 1080
#define DHTPIN D3
#define DHTTYPE DHT22
#define FIREBASE_HOST "tester-ffb0b-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "30IyAQP5DtaaoSnc0VRi4ATw4jvLHQkJok7PEv2Z"
#define WIFI_SSID "SE2021"
#define WIFI_PASSWORD ""


DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

//Variables
int chk;
float hum;  //Stores humidity value
float temp1; //Stores temperature value
int fsrAnalogPin = A0; // FSR is connected to analog 1
int fsrReading;      // the analog reading from the FSR resistor divider
int distance_cm;
int sensorPin = A0;
int fireStatus;

int led1 = D6; //Amarelo
int led3 = D7; //Verde
int led2 = D8; //Vermelho
int reserved;

const int trigPin = D5;
const int echoPin = D4;
int duration, distance;


void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
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
  delay(100);
  reserved = Firebase.getInt("reserved");
  Serial.println(reserved);
  
  if (Firebase.failed()) {
   Serial.print("get int failed:");
   Serial.println(Firebase.error());  
  }
  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = (duration*.0343)/2;

  hum = dht.readHumidity();
  temp1= dht.readTemperature();

  Serial.print("Temp: ");
  Serial.print(temp1);
  Serial.println(" Celsius");
  
  fsrReading = analogRead(fsrAnalogPin);
  Serial.print("Pressure = ");
  Serial.println(fsrReading);
  
  if (temp1 > 45){
    Firebase.setInt("temp",1);
    if (Firebase.failed()) {
           Serial.print("fail Temp");
           Serial.println(Firebase.error());  
        }
  }
  
  Serial.print("distance  : ");
  Serial.print(distance);
  Serial.println(" cm");

 if (distance > 10 && fsrReading < 400)       
       {
         digitalWrite(led3, HIGH);
         digitalWrite(led2, LOW);
         Firebase.setBool("occupied",false);
         if (Firebase.failed()) {
           Serial.print("fail occupied");
           Serial.println(Firebase.error());  
          }
         Serial.println("-------------------------------------------------"); 
     }

  if (reserved <= 1){
       digitalWrite(led1, LOW);
       Serial.println("LOW LOW LOW LOW LOW LWO"); 
  }
  if (reserved > 1){
       digitalWrite(led1, HIGH);
       Serial.println("HIGH HIGH HIGH");
  }

 if (distance < 10 && fsrReading > 400){   

    Firebase.setInt("reserved",2);
    reserved = Firebase.getInt("reserved");
    
    Firebase.setBool("occupied",true);
    if (Firebase.failed()) {
         Serial.print("Failed occupied");
         Serial.println(Firebase.error());  
     }
    digitalWrite(led3, LOW);
    digitalWrite(led2, HIGH);
    Serial.print("###################################################################### ");
  }



}
