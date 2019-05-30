//Include the SoftwareSerial library
#include "SoftwareSerial.h"
#include <Servo.h>

//Create a new software  serial
SoftwareSerial bluetooth(0, 1); //TX, RX (Bluetooth)

int incomingByte;      // a variable to read incoming serial data into

int botaoPin = 12;
int botao;

int sensorPIRPin = 2;
unsigned char sensorPIR;

int led1Pin = 4;
int led2Pin = 5;

int servoPin = 3;
Servo servo;

bool estaSegurandoBotao = false;
bool clicouBotao = false;
bool soltouBotao = true;

unsigned long temporizacaoLeds;
unsigned long tempoLeds = 15000; // Time leds ON in milliseconds  
unsigned char contandoLeds;

void setup()
{
  //Serial.begin(2400);
  bluetooth.begin(9600);
  pinMode(botaoPin, INPUT);
  pinMode(sensorPIRPin, INPUT);
  //pinMode(infraPin, INPUT);
  pinMode(led1Pin, OUTPUT);
  pinMode(led2Pin, OUTPUT);
  //pinMode(infraOutPin, OUTPUT);
  servo.attach(servoPin);
  servo.write(0);
}

void loop()
{
  botao = digitalRead(botaoPin);
  sensorPIR = digitalRead(sensorPIRPin);
  //estadoInfra = digitalRead(infraPin);
  
  //VerificaInfra();
  VerificaPiR();
  VerificaBluetooth();
  VerificaBotao();
}

void VerificaBluetooth() {
  if (bluetooth.available() > 0) {
    // read the oldest byte in the serial buffer:
    incomingByte = bluetooth.read();
    if (incomingByte == 'P') {
      clicouBotao?clicouBotao=false:clicouBotao=true;
    }
  }
}

void VerificaBotao() {
  if (botao == HIGH) {
    if (!estaSegurandoBotao && soltouBotao) {
        clicouBotao?clicouBotao=false:clicouBotao=true;
      soltouBotao = false;
    }
    estaSegurandoBotao = true;
  } else {
    estaSegurandoBotao = false;
    soltouBotao = true;
  }
  
  AbrirFecharPorta(servo.read(), clicouBotao);
  delayMicroseconds(100); //Tempo para executar de novo para não sbrecarregar o sistema
}

void AbrirFecharPorta(int grauAtual, bool estadoPorta) {
  if (estadoPorta) {
    if (grauAtual < 180) {
      servo.write(grauAtual+=1);
    }
  } else {
    if (grauAtual > 0) {
      servo.write(grauAtual-=1);
    }
  }
  delayMicroseconds(5500); //Tempo para executar de novo para não sbrecarregar o sistema
}

void VerificaPiR() {
  if (sensorPIR == HIGH) {
    temporizacaoLeds = millis();
    LigarLeds();
    contandoLeds = 1;
  } else if (contandoLeds == 1 && (millis() - temporizacaoLeds > tempoLeds)) {
    DesligarLeds();
    contandoLeds = 0;
  }
  delayMicroseconds(70); //Tempo para executar de novo para não sbrecarregar o sistema
}

void LigarLeds() {
  digitalWrite(led1Pin, HIGH);  // turn LED ON
  digitalWrite(led2Pin, HIGH);  // turn LED ON
}

void DesligarLeds() {
  digitalWrite(led1Pin, LOW); // turn LED OFF
  digitalWrite(led2Pin, LOW);  // turn LED OFF
}
