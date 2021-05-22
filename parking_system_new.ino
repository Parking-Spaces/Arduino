#include <UbidotsEthernet.h>
#include <SPI.h>
#include <Ethernet.h>
#include <SharpIR.h>


#define IRPin A0
#define model 1080


/********************************
 * Constants and objects
 *******************************/
/* Assigns the Ubidots parameters */
/*char const * TOKEN = "BBFF-QgZk0lStvTL1YTlpzIMu2sdn9m0oUs"; // Assign your Ubidots TOKEN
char const * DEVICE_LABEL = "arduino-uno"; // Assign the unique device label
char const * VARIABLE_LABEL = "estado"; // Assign the unique variable label to get the last value*/


/********************************
 * Constants and objects
 *******************************/
/* Assigns the Ubidots parameters */
char const * TOKEN = "BBFF-QgZk0lStvTL1YTlpzIMu2sdn9m0oUs"; // Assign your Ubidots TOKEN
char const * VARIABLE_LABEL_1 = "temperature"; // Assign the unique variable label to send the data
char const * VARIABLE_LABEL_2 = "proximity"; // Assign the unique variable label to send the data
char const * VARIABLE_LABEL_3 = "pressure"; // Assign the unique variable label to send the data


byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x04 };
byte ip[] = { 192, 168, 1, 166 }; 
byte gateway[] = {192, 168, 1, 254}; 
byte subnet[] = {255, 255, 255, 0}; 


EthernetServer server(23);
boolean gotAMessage = false;

int fsrAnalogPin = A1; // FSR is connected to analog 1
int fsrReading;      // the analog reading from the FSR resistor divider
int distance_cm;

SharpIR mySensor = SharpIR(IRPin, model);

int sensorPin = A0;


int led1 = 6;
int led3 = 7;
int led2 = 5;
int reservado = 0;
int temp = 0;

Ubidots client(TOKEN);

void setup()
{    
    
    Serial.begin(9600); 

    while (!Serial) {
      ;
    }
    Serial.println();
    Serial.println("Trying to get an IP");
    if (Ethernet.begin(mac) == 0) {
      Serial.println("Failed to configure Ethernet Using DHCP");
      Ethernet.begin(mac, ip, gateway, subnet);
    }

    Serial.println("IP check");

    Serial.println();
    server.begin();
    
     

   
}


void loop()
{
  Ethernet.maintain();

  fsrReading = analogRead(fsrAnalogPin);
  Serial.print("Analog reading = ");
  Serial.println(fsrReading);
  
  distance_cm = mySensor.distance();
  
  Serial.print("distance  : ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  
 // Parking Slot 1 IR & Led
 if (distance_cm > 20  && fsrReading < 10)       
       {
         digitalWrite(led3, HIGH);
         digitalWrite(led2, LOW);
       }

 if (distance_cm < 20 && fsrReading > 10){   
    if (reservado > 0){
      digitalWrite(led1,LOW);
    }
    digitalWrite(led3, LOW);
    digitalWrite(led2, HIGH);
  }

 if (reservado > 0){
  digitalWrite(led1,HIGH);
  digitalWrite(led3, LOW);
  digitalWrite(led2, LOW);
 }

  client.add(VARIABLE_LABEL_1, temp);
  client.add(VARIABLE_LABEL_2, distance_cm);
  client.add(VARIABLE_LABEL_3, fsrReading);
  client.sendAll();

  
            
}
