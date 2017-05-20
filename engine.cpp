#include "engine.h"

#define ENABLEA   2
#define PINA1     4
#define PINA2     3
#define ENABLEB   7
#define PINB1     6
#define PINB2     5
#define SPEED     150

Engine::Engine(Logger* l) {
  pinMode (ENABLEA, OUTPUT);
  pinMode (PINA1, OUTPUT);
  pinMode (PINA2, OUTPUT);

  pinMode (ENABLEB, OUTPUT);
  pinMode (PINB1, OUTPUT);
  pinMode (PINB2, OUTPUT); 


  this->logger = l;
}

void Engine::forward (int duration) {
  this->forward();
  delay (duration);
  this->disableMotors();
}

void Engine::forward () {
  this->enableMotors();
  this->motorAforward();
  this->motorBforward();
}

void Engine::stop () {
  this->disableMotors();
}

void Engine::backward (int duration) {
  this->backward();
  delay (duration);
  this->disableMotors();
}

void Engine::backward () {
  this->enableMotors();
  this->motorAbackward();
  this->motorBbackward();
}

void Engine::right () {
  this->enableMotors();
  this->motorAbackward();
  this->motorBforward();
}

void Engine::right (int duration) {
  this->right();
  delay (duration);
  this->disableMotors();
}

void Engine::left () {
  this->enableMotors();
  this->motorAforward();
  this->motorBbackward();
}

void Engine::left (int duration) {
  this->left();
  delay (duration);
  this->disableMotors();
}

void Engine::backwardLeft () {
  this->enableMotors();
  this->motorAbackward();
  this->motorBoff();
}

void Engine::backwardRight () {
  this->enableMotors();
  this->motorAoff();
  this->motorBbackward();
}

void Engine::forwardLeft() {
  this->enableMotors();
  this->motorAforward();
  this->motorBoff();  
}

void Engine::forwardRight() {
  this->enableMotors();
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

void Engine::enableMotors() {
 logger->debug("enableMotors()");
 //this->motorAon();
 //this->motorBon();
 this->setSpeed();
}

void Engine::setSpeed() {
 analogWrite(ENABLEA, SPEED);
 analogWrite(ENABLEB, SPEED);  
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

void Engine::avoid()
{
    this->logger->debug("avoid()");
    this->logger->ident();

    backward(250);
    if(random(2)) {
      right(360);
    } else {
      left(360);
    }   
    logger->unident();
}

