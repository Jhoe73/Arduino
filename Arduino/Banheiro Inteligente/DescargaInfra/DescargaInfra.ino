#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"

//Inicia a serial por software nos pinos 10 e 11
SoftwareSerial mySoftwareSerial(10, 11); // RX, TX

DFRobotDFPlayerMini myDFPlayer;

char buf;

int infraPin = 6;
int estadoInfra;

unsigned long temporizacaoInfraBranco;
unsigned long temporizacaoInfraPreto;
unsigned long tempoParaDescarga = 1200; // Tempo para dar descarga depois do uso in milliseconds
unsigned long tempoParaUso = 3250; // Tempo para considerar banheiro em uso in milliseconds
unsigned char alguemUsou;

void setup()
{
  Serial.begin(2400);
  pinMode(infraPin, INPUT);

  //Comunicacao serial com o modulo
  mySoftwareSerial.begin(9600);
  myDFPlayer.begin(mySoftwareSerial);

  //Definicoes iniciais
  myDFPlayer.setTimeOut(500); //Timeout serial 500ms
  myDFPlayer.volume(25); //Volume 5
  myDFPlayer.EQ(0); //Equalizacao normal
  
}

void loop()
{
  estadoInfra = digitalRead(infraPin);
  VerificaInfra();
}

void VerificaInfra() {
  if (estadoInfra) {
      //Serial.println("Preto (ou nada)");
      temporizacaoInfraPreto = millis();
      if (alguemUsou == 1 && millis() - temporizacaoInfraBranco > tempoParaDescarga) { //Se fizer tempoParaDescarga que não é branco
        Serial.println("Descarga"); 
        EmiteSom();
        alguemUsou = 0;
      }
    } else { //Quando ficar branco
      if (millis() - temporizacaoInfraPreto > tempoParaUso) { //Se ficar branco constante por mais de tempoParaUso
        //Espere até ficar preto ou nada por 2 segundos e dê discarga
        Serial.println("Alguém usando");
        temporizacaoInfraBranco = millis();
        if (alguemUsou != 1){
          alguemUsou = 1;
        }
      }
    }
    delayMicroseconds(50);
}

void EmiteSom() {
    buf = 1;
    myDFPlayer.play(buf);
}
