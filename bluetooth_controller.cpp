#include "bluetooth_controller.h"

BluetoothController::BluetoothController(Logger* l, Engine* e) {
  this->logger = l;
  this->engine = e;
  this->bluetoothSerial = new SoftwareSerial(12,13);
  this->bluetoothSerial->begin(9600); 
  this->autoModeEnabled = 0;
  this->iSpeed = 0;
}

BluetoothController::~BluetoothController() {
  delete this->bluetoothSerial;
}

int BluetoothController::getAutoMode() {
  return this->autoModeEnabled;
}

int BluetoothController::getSpeed() {
  return this->iSpeed;
}

void BluetoothController::followCommand() {
  if (this->bluetoothSerial->available()) {
    this->val = this->bluetoothSerial->read();

    if (this->val == this->last_val) {
      return;
    }

    char val_str[1];
    sprintf(val_str, "%c", this->val);
    this->logger->debug(val_str);

    this->last_val = this->val;

    if (this->val == 'S') { // Stop Forward
        this->engine->stop();
    }
    else if (this->val == 'F') { // Forward
      this->engine->forward(this->iSpeed);
    }
    else if (this->val == 'B') { // Backwards
      this->engine->backward(this->iSpeed);
    }
    else if (this->val == 'R') { // Right
      this->engine->right(this->iSpeed);
    }
    else if (this->val == 'L') { // Left
      this->engine->left(this->iSpeed);
    }
    else if (this->val == 'G') { // Forward Left
      this->engine->forwardLeft(this->iSpeed);
    }
    else if (this->val == 'I') { // Forward Right
      this->engine->forwardRight(this->iSpeed);
    }
    else if (this->val == 'H') { // Backward Left
      this->engine->backwardLeft(this->iSpeed);
    }
    else if (this->val == 'J') { // Backward Right
      this->engine->backwardRight(this->iSpeed);
    }
    else if (int(this->val) >= 49 && int(this->val) <= 57) { //set speed
      this->iSpeed = (int(this->val)-48)*26;
      Serial.println("Speed set to: " + this->iSpeed);
    }
    else if (this->val == 'q') { //set speed
      this->iSpeed = 255;
      Serial.println("Speed set to: " + this->iSpeed);
    }
    else if (this->val == 'U') { //set speed
      this->autoModeEnabled = 1;
      Serial.println("Automode enabled!");
    }
    else if (this->val == 'u') { //set speed
      this->autoModeEnabled = 0;
      Serial.println("Automode disabled!");
    }
  }
}
