#include <arduino.h>
#include "logger.h"

Logger::Logger() {
  this->level = NONE;
  this->ident_level = 0;
  Serial.begin(9600);
}

void Logger::set_level(int new_level) {
  this->level = new_level;
}

void Logger::debug(char message[]) {
  if(this->level == DEBUG) {
    this->print_identation();
    Serial.println(message); 
  }
}

// FIXME: mover sprintf para dentro do metodo
//void Logger::debug(const char *format, ...) {
//  if(this->level == DEBUG) {
//    this->print_identation();
//    
//    va_list v_List;
//    va_start( v_List, format );
//    char buffer[255];
//    sprintf( buffer, format, v_List );
//    va_end( v_List );
//    
//    Serial.println(buffer); 
//  }
//}


void Logger::ident() { this->ident_level = this->ident_level + 1; }

void Logger::print_identation() {
  for(int i = 0; i < this->ident_level; i++) {
    Serial.print(" ");
  }
}

void Logger::unident() { this->ident_level = this->ident_level - 1; }

