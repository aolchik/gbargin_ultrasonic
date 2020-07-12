#include <Servo.h>
#include "logger.h"
#include "engine.h"
#include "bluetooth_controller.h"

// NEXT
// - Improve speed regulation (speeds 1 and 2 are not strong enough)
// - Use wi-fi module
// - Fix radars and open space to camera
// - Issue: frontal left motor sometimes fails
// - Add lights
// - Add buzzer
// - Remove old methods in this file
// - Remove warnings
// - Radar flickering
// - Improve responsiveness for bluetooth control
// - Different strategies for obstacles on te right and on the left
// - Add start stop command (hand before sensores for 5 seconds?)
// - No movement detection. Is it possible?

// @WIP: Moving to engine.cpp
#define ENABLEA 3
#define PINA1   4
#define PINA2   2
#define ENABLEB   11
#define PINB1     6
#define PINB2     5

int servposnum = 0;
int servpos = 0;

int distance();

// Class Radar
#define ECHOPIN   9
#define MAX_SERVO_ANGLE   2600
#define MIN_SERVO_ANGLE   1200
#define MINIMUM_DISTANCE  15
#define SERVO_DELAY       100
#define SERVO_STEPS       10 
#define SERVOPIN  10
#define TRIGPIN   8

class Radar {
public:
  Radar(Logger*);
  int obstacle();

private:
  Logger* logger;
  Servo myservo;
  int servo_step;
  int mid_point;
  int serv_pos;
};

Radar::Radar(Logger* l) {
  this->logger = l;

  //configure pin modes for the ultrasonic sensor
  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);

  //Servo pins
  this->myservo.attach(SERVOPIN);

  this->servo_step = (MAX_SERVO_ANGLE - MIN_SERVO_ANGLE)/SERVO_STEPS;
  this->mid_point = (MAX_SERVO_ANGLE + MIN_SERVO_ANGLE)/2;
  char msg[255];
  sprintf(msg,"servo_step: %d",this->servo_step);
  this->logger->debug(msg);
  this->serv_pos = this->mid_point;
  this->myservo.writeMicroseconds(serv_pos);
}

//FIXME: Refactor
int Radar::obstacle() {
  logger->debug("obstacle()");
  logger->ident();
  
  int obstacle_found = false;
  if(servo_step > 0) {
    if (serv_pos > MIN_SERVO_ANGLE && !obstacle_found) {
      serv_pos = serv_pos - this->servo_step;
      this->myservo.writeMicroseconds(serv_pos);
      obstacle_found = distance() <= MINIMUM_DISTANCE;
      char msg[255];
      sprintf(msg,"servo_pos: %d",serv_pos);
      logger->debug(msg);
      delay(SERVO_DELAY);
    } else {
      servo_step = -servo_step;
    }
  }

  if(servo_step < 0) {
    if(serv_pos < MAX_SERVO_ANGLE && !obstacle_found) {
      serv_pos = serv_pos - this->servo_step;
      this->myservo.writeMicroseconds(serv_pos);
      obstacle_found = distance() <= MINIMUM_DISTANCE;
      char msg[255];
      sprintf(msg,"servo_pos: %d",serv_pos);
      logger->debug(msg);
      delay(SERVO_DELAY);
    } else {
      servo_step = -servo_step;
    }
  }
  
  logger->unident();
  return obstacle_found;
}

Logger* logger;
Radar* radar;
Engine* engine;
BluetoothController* btcontrol;

void setup() {
  logger = new Logger();
  logger->set_level(DEBUG);
  logger->debug("Oi!");
  logger->debug("Iniciando o robô...");

  engine = new Engine(logger);
  btcontrol = new BluetoothController(logger, engine);
  radar = new Radar(logger);
}

void loop() {
  btcontrol->followCommand();  
  if(btcontrol->getAutoMode()) {
    logger->debug("In auto mode...");
    if(!radar->obstacle()) { 
      engine->forward(btcontrol->getSpeed(),200);
     } else {
      engine->stop();
      engine->avoid(btcontrol->getSpeed());
    }
  }
}

//Defining functions so it's more easy
//motor functions
void motorAforward() {
 logger->debug("motorAforward()");
 
 digitalWrite (PINA1, HIGH);
 digitalWrite (PINA2, LOW);
}
void motorBforward() {
 logger->debug("motorBforward()");

 digitalWrite (PINB1, LOW);
 digitalWrite (PINB2, HIGH);
}
void motorAbackward() {
 digitalWrite (PINA1, LOW);
 digitalWrite (PINA2, HIGH);
}
void motorBbackward() {
 digitalWrite (PINB1, HIGH);
 digitalWrite (PINB2, LOW);
}
void motorAstop() {
 digitalWrite (PINA1, HIGH);
 digitalWrite (PINA2, HIGH);
}
void motorBstop() {
 digitalWrite (PINB1, HIGH);
 digitalWrite (PINB2, HIGH);
}
void motorAcoast() {
 digitalWrite (PINA1, LOW);
 digitalWrite (PINA2, LOW);
}
void motorBcoast() {
 digitalWrite (PINB1, LOW);
 digitalWrite (PINB2, LOW);
}
void motorAon() {
 digitalWrite (ENABLEA, HIGH);
}
void motorBon() {
 digitalWrite (ENABLEB, HIGH);
}
void motorAoff() {
 digitalWrite (ENABLEA, LOW);
}
void motorBoff() {
 digitalWrite (ENABLEB, LOW);
}

// Movement functions
void forward (int duration) {
  char msg[255];
  sprintf(msg,"forward(): %d",duration);
  logger->debug(msg);
  
  motorAforward();
  motorBforward();
  delay (duration);
}

void backward (int duration) {
  char msg[255];
  sprintf(msg,"backward(): %d",duration);
  logger->debug(msg);

  motorAbackward();
  motorBbackward();
  delay (duration);
}

void right (int duration) {
 logger->debug("right()");
 motorAbackward();
 motorBforward();
 delay (duration);
}
void left (int duration) {
 logger->debug("left()");
 motorAforward();
 motorBbackward();
 delay (duration);
}
void coast (int duration) {
 motorAcoast();
 motorBcoast();
 delay (duration);
}
void breakRobot (int duration) {
 motorAstop();
 motorBstop();
 delay (duration);
}
void disableMotors() {
  logger->debug("disableMotors()");
  motorAoff();
  motorBoff();
}
void enableMotors() {
 logger->debug("enableMotors()");
 motorAon();
 motorBon();
}

int distance() {
  int duration, distance;
  digitalWrite(TRIGPIN, HIGH);
  delayMicroseconds(1000);
  digitalWrite(TRIGPIN, LOW);
  duration = pulseIn(ECHOPIN, HIGH);
  distance = (duration/2) / 29.1;
  
  char msg[255];
  sprintf(msg,"distance(): %d",distance);
  logger->debug(msg);
  
  return distance;
}

void avoid()
{
    logger->debug("avoid()");
    logger->ident();

    enableMotors();
    backward(250);
    if(random(2)) {
      right(360);
    } else {
      left(360);
    }
    disableMotors();
    
    logger->unident();
}
