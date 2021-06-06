#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

#include <SPI.h>

#include <DHT.h>

#define IRPin A0
#define model 1080
#define DHTPIN D6
#define DHTTYPE DHT22
// Set these to run example.
#define FIREBASE_HOST "tester-ffb0b-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "30IyAQP5DtaaoSnc0VRi4ATw4jvLHQkJok7PEv2Z"
#define WIFI_SSID "MEO-2F41D0"
#define WIFI_PASSWORD "Vc96rXJEXhCwJs"




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

int led1 = D3; //Amarelo
int led3 = D7; //Vermelho
int led2 = D8; //Verde
bool reserved = 0;

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
  Firebase.set("led", 9);
}

void loop() {
  
  reserved = Firebase.getBool("reserved");
  Serial.println(reserved);
  
  if (Firebase.failed()) {
   Serial.print("get bool failed:");
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

 if (distance < 10 && fsrReading > 400){   
  if (reserved == true){
    digitalWrite(led1,LOW);
    Firebase.setBool("reserved",false);

    if (Firebase.failed()) {
         Serial.print("failed reserved ");
         Serial.println(Firebase.error());  
     }
    }
    Firebase.setBool("occupied",true);
    if (Firebase.failed()) {
         Serial.print("Failed occupied");
         Serial.println(Firebase.error());  
     }
     
    digitalWrite(led3, LOW);
    digitalWrite(led2, HIGH);
    Serial.print("###################################################################### ");
  }

 if (reserved != true){
    digitalWrite(led1,HIGH);
    digitalWrite(led3, LOW);
    digitalWrite(led2, LOW);
 }

}
