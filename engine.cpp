#include "engine.h"

#define ENABLEA   3
#define PINA1     4
#define PINA2     2
#define ENABLEB   11
#define PINB1     6
#define PINB2     5
#define SPEED     80

Engine::Engine(Logger* l) {
  pinMode (ENABLEA, OUTPUT);
  pinMode (PINA1, OUTPUT);
  pinMode (PINA2, OUTPUT);

  pinMode (ENABLEB, OUTPUT);
  pinMode (PINB1, OUTPUT);
  pinMode (PINB2, OUTPUT); 


  this->logger = l;
}

void Engine::forward (int speed, int duration) {
  this->forward(speed);
  delay (duration);
  this->disableMotors();
}

void Engine::forward (int speed) {
  this->enableMotors(speed);
  this->motorAforward();
  this->motorBforward();
}

void Engine::stop () {
  logger->debug("stop()");
  this->disableMotors();
}

void Engine::backward (int speed, int duration) {
  this->backward(speed);
  delay (duration);
  this->disableMotors();
}

void Engine::backward (int speed) {
  this->enableMotors(speed);
  this->motorAbackward();
  this->motorBbackward();
}

void Engine::right (int speed) {
  this->enableMotors(speed);
  this->motorAbackward();
  this->motorBforward();
}

void Engine::right (int speed, int duration) {
  this->right(speed);
  delay (duration);
  this->disableMotors();
}

void Engine::left (int speed) {
  this->enableMotors(speed);
  this->motorAforward();
  this->motorBbackward();
}

void Engine::left (int speed, int duration) {
  this->left(speed);
  delay (duration);
  this->disableMotors();
}

void Engine::backwardLeft (int speed) {
  this->enableMotors(speed);
  this->motorAbackward();
  this->motorBoff();
}

void Engine::backwardRight (int speed) {
  this->enableMotors(speed);
  this->motorAoff();
  this->motorBbackward();
}

void Engine::forwardLeft(int speed) {
  this->enableMotors(speed);
  this->motorAforward();
  this->motorBoff();  
}

void Engine::forwardRight(int speed) {
  this->enableMotors(speed);
  this->motorAoff();
  this->motorBforward();  
}

void Engine::motorAforward() {
 logger->debug("motorAforward()");
 
 digitalWrite (PINA1, HIGH);
 digitalWrite (PINA2, LOW);
}

void Engine::motorBforward() {
 logger->debug("motorBforward()");

 digitalWrite (PINB1, LOW);
 digitalWrite (PINB2, HIGH);
}

void Engine::motorAbackward() {
 digitalWrite (PINA1, LOW);
 digitalWrite (PINA2, HIGH);
}
void Engine::motorBbackward() {
 digitalWrite (PINB1, HIGH);
 digitalWrite (PINB2, LOW);
}

void Engine::disableMotors() {
  logger->debug("disableMotors()");
  this->motorAoff();
  this->motorBoff();
}

void Engine::enableMotors(int speed) {
 logger->debug("enableMotors()");
 //this->motorAon();
 //this->motorBon();
 this->setSpeed(speed);
}

void Engine::setSpeed(int speed) {
 analogWrite(ENABLEA, speed);
 analogWrite(ENABLEB, speed);  
}

void Engine::motorAon() {
 //digitalWrite (ENABLEA, HIGH);
}
void Engine::motorBon() {
 //digitalWrite (ENABLEB, HIGH);
}
void Engine::motorAoff() {
 digitalWrite (ENABLEA, LOW);
}
void Engine::motorBoff() {
 digitalWrite (ENABLEB, LOW);
}

void Engine::avoid(int speed)
{
    this->logger->debug("avoid()");
    this->logger->ident();

    this->backward(speed, 250);
    if(random(2)) {
      this->right(speed, 360);
    } else {
      this->left(speed, 360);
    }   
    logger->unident();
}
