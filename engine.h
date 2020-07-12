
#include "logger.h"
#include <arduino.h>

#ifndef Engine_H
#define Engine_H

class Engine {
public:
  Engine(Logger*);
  void avoid(int);
  void backward(int);
  void backward(int, int);
  void backwardLeft(int);
  void backwardRight(int);
  void forward(int);
  void forward(int, int);
  void forwardLeft(int);
  void forwardRight(int);
  void left(int);
  void left(int, int);
  void right(int);
  void right(int, int);
  void setSpeed(int);
  void stop();
private:
  Logger* logger;
  void motorAbackward();
  void motorBbackward();
  void motorAforward();
  void motorBforward();
  void disableMotors();
  void enableMotors(int);
  void motorAon();
  void motorBon();
  void motorAoff();
  void motorBoff();
};

#endif
