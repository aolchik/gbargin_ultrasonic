
#include "logger.h"
#include "engine.h"
#include <SoftwareSerial.h>

#ifndef BluetContr_H
#define BluetContr_H

class BluetoothController {
public:
  BluetoothController(Logger*, Engine*);
  ~BluetoothController();
  void followCommand();

private:
  Logger* logger;
  Engine* engine;
  SoftwareSerial* bluetoothSerial;
  char val;
};

#endif

