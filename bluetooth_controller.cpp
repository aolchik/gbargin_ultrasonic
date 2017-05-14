#include "bluetooth_controller.h"

BluetoothController::BluetoothController(Logger* l, Engine* e) {
  this->logger = l;
  this->engine = e;
  this->bluetoothSerial = new SoftwareSerial(12,13);
  this->bluetoothSerial->begin(9600); 
}

BluetoothController::~BluetoothController() {
  delete this->bluetoothSerial;
}

void BluetoothController::followCommand() {
  if (this->bluetoothSerial->available()) { //check if bluetooth command available
    this->val = this->bluetoothSerial->read();
    //this->bluetoothSerial->write(this->val);
    this->logger->debug(this->val);
  }
  
  if (this->val == 'F') { // Forward
    this->engine->forward();
  }
  else if (this->val == 'S') { // Stop Forward
    this->engine->stop();
  }
  else if (this->val == 'B') { // Backwards
    this->engine->backward();
  }
  else if (this->val == 'R') { // Right
    this->engine->right();
  }
  else if (this->val == 'L') { // Left
    this->engine->left();
  }
  else if (this->val == 'G') { // Forward Left
    this->engine->forwardLeft();
  }
  else if (this->val == 'I') { // Forward Right
    this->engine->forwardRight();
  }
  else if (this->val == 'H') { // Backward Left
    this->engine->backwardLeft();
  }
  else if (this->val == 'J') { // Backward Right
    this->engine->backwardRight();
  }

//  else if (this->val == 's') { // Stop, not used though
//    stopp(10 ) ;
//  }
//  else if (int(this->val) >= 49 && int(this->val) <= 57) { //set speed
//    iSpeed = (int(this->val)-48)*26;
//    Serial.println("Speed set to: " + iSpeed); 
//  }
//  else if (this->val == 'q') { //set speed
//    iSpeed = 255;
//    digitalWrite(LedPin,HIGH);   
//    Serial.println("Speed set to: " + iSpeed);  
//  }
//  else if (this->val == 'W') {
//    digitalWrite(LedPin,HIGH);
//  }
//  else if (this->val == 'w') {
//    digitalWrite(LedPin,LOW);
//  }
}

