#include <SPI.h>
#include <Ethernet.h>
#include <SharpIR.h>


#define IRPin A0
#define model 1080

byte mac[] = { 0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x04 };
byte ip[] = { 192, 168, 1, 166 }; //COLOQUE UMA FAIXA DE IP DISPONÍVEL DO SEU ROTEADOR. EX: 192.168.1.110  **** ISSO VARIA, NO MEU CASO É: 192.168.0.175
byte gateway[] = {192, 168, 1, 254}; //GATEWAY DE CONEXÃO (ALTERE PARA O GATEWAY DO SEU ROTEADOR)
byte subnet[] = {255, 255, 255, 0}; //MASCARA DE REDE (ALTERE PARA A SUA MÁSCARA DE REDE)


EthernetServer server(23); //PORTA EM QUE A CONEXÃO SERÁ FEITA
boolean gotAMessage = false;

int fsrAnalogPin = A1; // FSR is connected to analog 1
int fsrReading;      // the analog reading from the FSR resistor divider
int distance_cm;

SharpIR mySensor = SharpIR(IRPin, model);

int sensorPin = A0;


int led1 = 6;
int led3 = 7;
int led2 = 12;
int reservado = 0;

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


  fsrReading = analogRead(fsrAnalogPin);
  Serial.print("Analog reading = ");
  Serial.println(fsrReading);
  
  distance_cm = mySensor.distance();
  
  Serial.print("distance  : ");
  Serial.print(distance_cm);
  Serial.println(" cm");

  
 // Parking Slot 1 IR & Led
 if (distance_cm > 10  && fsrReading < 10)       
       {
         digitalWrite(led3, HIGH);
         digitalWrite(led2, LOW);
       }

 if (distance_cm < 10 && fsrReading > 10){   
    if (reservado > 0){
      digitalWrite(led1,LOW);
    }
    digitalWrite(led3, LOW);
    digitalWrite(led2, HIGH);
  }

 if (reservado > 0){
  digitalWrite(led1,HIGH)
 }



  
            
}
