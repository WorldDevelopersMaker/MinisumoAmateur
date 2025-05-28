#include <NewPing.h>

// Pines de configuración
#define DIP4 3
#define DIP3 12
#define DIP2 13
#define PULSO1 2

#define TRIGGER_PIN 17 // TODOS JUNTOS  
#define ECHO_PIN_1 18  // FRENTE 
#define ECHO_PIN_2 16  // izquierdo
#define ECHO_PIN_3 19 // DErecho 
#define MAX_DISTANCE 100

#define Piso_Izquierda 8
#define Piso_Derecha 14

// Pines del motor A (izquierdo)
const int pinAIN1 = 9;
const int pinAIN2 = 10;
const int pinPWMA = 5;

// Pines del motor B (derecho)
const int pinBIN1 = 11;
const int pinBIN2 = 7;
const int pinPWMB = 6;

// Variables
NewPing sonar1(TRIGGER_PIN, ECHO_PIN_1, MAX_DISTANCE);
NewPing sonar2(TRIGGER_PIN, ECHO_PIN_2, MAX_DISTANCE);
NewPing sonar3(TRIGGER_PIN, ECHO_PIN_3, MAX_DISTANCE);

int pulso = 0;
int Dip2 = 0, Dip3 = 0, Dip4 = 0;
int speed = 255;  // Velocidad de giro

const int pinMotorB[3] = { pinPWMA, pinAIN2, pinAIN1 };
const int pinMotorA[3] = { pinPWMB, pinBIN1, pinBIN2 };

enum moveDirection { forward, backward };
enum turnDirection { Right, Left };

void setup() {
  Serial.begin(9600);

  pinMode(DIP2, INPUT);
  pinMode(DIP3, INPUT);
  pinMode(DIP4, INPUT);

  pinMode(Piso_Derecha, INPUT);
  pinMode(Piso_Izquierda, INPUT);
  pinMode(PULSO1, INPUT);

  pinMode(pinAIN2, OUTPUT);
  pinMode(pinAIN1, OUTPUT);
  pinMode(pinPWMA, OUTPUT);
  pinMode(pinBIN1, OUTPUT);
  pinMode(pinBIN2, OUTPUT);
  pinMode(pinPWMB, OUTPUT);
}

void loop() {
  // Lectura sensores ultrasonido
  unsigned int distance1 = sonar1.ping_cm();
  unsigned int distance2 = sonar2.ping_cm();
  unsigned int distance3 = sonar3.ping_cm();

  Serial.print("Sensor 1: "); Serial.print(distance1); Serial.println(" cm");
  Serial.print("Sensor 2: "); Serial.print(distance2); Serial.println(" cm");
  Serial.print("Sensor 3: "); Serial.print(distance3); Serial.println(" cm");

  // Lectura infrarrojos
  bool pisoI = digitalRead(Piso_Izquierda);
  bool pisoD = digitalRead(Piso_Derecha);

  Serial.print("INFRA 1: "); Serial.println(pisoI);
  Serial.print("INFRA 2: "); Serial.println(pisoD);
  Serial.println("----------------------");

  // Lógica sensores infrarrojos
  if (pisoI == 0 && pisoD == 0) {
    fullStop();
  } else if (pisoI == 1 && pisoD == 1) {
    move(backward, 255);
    delay(600);
  } else if (pisoI == 1 && pisoD == 0) {
    turn(Left, 200);
    delay(400);
  } else if (pisoI == 0 && pisoD == 1) {
    turn(Right, 200);
    delay(400);
  }

  // Lógica detección de oponente
  if (distance1 > 0 && distance1 < 15) {
    move(forward, 150);
  } else if (distance2 > 0 && distance2 < 15) {
    turn(Left, 200);
    delay(400);
  } else if (distance3 > 0 && distance3 < 15) {
    turn(Right, 200);
    delay(400);
  }

  delay(100); // Para lectura de monitor serial
}

// Lectura de switches
void sensores() {
  pulso = digitalRead(PULSO1);
  Dip3 = digitalRead(DIP3);
  Dip2 = digitalRead(DIP2);
  Dip4 = digitalRead(DIP4);
}

// Estado en monitor serial
void monitores() {
  Serial.println("PULSO"); Serial.println(pulso);
  Serial.println("Dip1"); Serial.println(Dip4);
  Serial.println("Dip2"); Serial.println(Dip2);
  Serial.println("Dip3"); Serial.println(Dip3);
}

// Movimiento general
void move(int direction, int speed) {
  if (direction == forward) {
    moveMotorForward(pinMotorB, speed);
    moveMotorForward(pinMotorA, speed);
  } else if (direction == backward) {
    moveMotorBackward(pinMotorB, speed);
    moveMotorBackward(pinMotorA, speed);
  }
}

// Giros
void turn(int direction, int speed) {
  if (direction == Left) {
    moveMotorBackward(pinMotorB, speed);
    moveMotorForward(pinMotorA, speed);
  } else if (direction == Right) {
    moveMotorForward(pinMotorB, speed);
    moveMotorBackward(pinMotorA, speed);
  }
}

// Detener ambos motores
void fullStop() {
  stopMotor(pinMotorA);
  stopMotor(pinMotorB);
}

// Control de motores
void moveMotorForward(const int pinMotor[3], int speed) {
  digitalWrite(pinMotor[1], HIGH);
  digitalWrite(pinMotor[2], LOW);
  analogWrite(pinMotor[0], speed);
}

void moveMotorBackward(const int pinMotor[3], int speed) {
  digitalWrite(pinMotor[1], LOW);
  digitalWrite(pinMotor[2], HIGH);
  analogWrite(pinMotor[0], speed);
}

void stopMotor(const int pinMotor[3]) {
  digitalWrite(pinMotor[1], LOW);
  digitalWrite(pinMotor[2], LOW);
  analogWrite(pinMotor[0], 0);
}
