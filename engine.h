
#include "logger.h"
#include <arduino.h>

#ifndef Engine_H
#define Engine_H

class Engine {
public:
  Engine(Logger*);
  void backward();
  void backward(int);
  void backwardLeft();
  void backwardRight();
  void forward();
  void forward(int);
  void forwardLeft();
  void forwardRight();
  void left();
  void left(int);
  void right();
  void right(int);
  void stop();
private:
  Logger* logger;
  void motorAbackward();
  void motorBbackward();
  void motorAforward();
  void motorBforward();
  void disableMotors();
  void enableMotors();
  void motorAon();
  void motorBon();
  void motorAoff();
  void motorBoff();
};

#endif


