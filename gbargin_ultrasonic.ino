#include <Servo.h>

// NEXT
// Add start stop command (hand before sensores for 5 seconds?)

Servo myservo;
#define ENABLEA 2
#define PINA1   4
#define PINA2   3

int servposnum = 0;
int servpos = 0;

#define ENABLEB   7
#define PINB1     6
#define PINB2     5

#define TRIGPIN   8
#define ECHOPIN   9
#define SERVOPIN  10

// Logger Class
// FIXME: move to another file
#define NONE  0
#define DEBUG 2
class Logger {
public:
  Logger();
  void set_level(int);
  void disable();
  void debug(char []);
  void ident();
  void print_identation();
  void unident();

private:
  int level;
  int ident_level;
};

Logger::Logger() {
  this->level = NONE;
  this->ident_level = 0;
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

Logger logger; 

void setup() {
  // put your setup code here, to run once:
  //configure pin modes for the drive motors
  pinMode (ENABLEA, OUTPUT);
  pinMode (PINA1, OUTPUT);
  pinMode (PINA2, OUTPUT);

  pinMode (ENABLEB, OUTPUT);
  pinMode (PINB1, OUTPUT);
  pinMode (PINB2, OUTPUT); 

  //configure pin modes for the ultrasonci se3nsor
  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);

  Serial.begin(9600);
  logger.set_level(DEBUG);
  logger.debug("Oi!");
  logger.debug("Iniciando o robô...");

  //Servo pins
  myservo.attach(SERVOPIN);

}

void loop() {
  car();
  avoid();    
}

//Defining functions so it's more easy
//motor functions
void motorAforward() {
 logger.debug("motorAforward()");
 
 digitalWrite (PINA1, HIGH);
 digitalWrite (PINA2, LOW);
}
void motorBforward() {
 logger.debug("motorBforward()");

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
  logger.debug(msg);
  
  motorAforward();
  motorBforward();
  delay (duration);
}

void backward (int duration) {
  char msg[255];
  sprintf(msg,"backward(): %d",duration);
  logger.debug(msg);

  motorAbackward();
  motorBbackward();
  delay (duration);
}

void right (int duration) {
 logger.debug("right()");
 motorAbackward();
 motorBforward();
 delay (duration);
}
void left (int duration) {
 logger.debug("left()");
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
  logger.debug("disableMotors()");
  motorAoff();
  motorBoff();
}
void enableMotors() {
 logger.debug("enableMotors()");
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
  logger.debug(msg);
  
  return distance;
}

#define MIN_SERVO_ANGLE   1200
#define MAX_SERVO_ANGLE   2600
#define SERVO_STEPS       10 
#define MINIMUM_DISTANCE  15
#define SERVO_DELAY       100

int obstacle_ahead() {
  logger.debug("obstacle_ahead()");
  logger.ident();
  
  int servo_step = (MAX_SERVO_ANGLE - MIN_SERVO_ANGLE)/SERVO_STEPS;
  int mid_point = (MAX_SERVO_ANGLE + MIN_SERVO_ANGLE)/2;
  char msg[255];
  sprintf(msg,"servo_step: %d",servo_step);
  logger.debug(msg);
  
  int obstacle_found = false;
  for(int serv_pos = mid_point; serv_pos > MIN_SERVO_ANGLE && !obstacle_found; serv_pos = serv_pos - servo_step) {
    myservo.writeMicroseconds(serv_pos);
    obstacle_found = distance() <= MINIMUM_DISTANCE;
    char msg[255];
    sprintf(msg,"servo_pos: %d",serv_pos);
    logger.debug(msg);
    delay(SERVO_DELAY);
  }

  for(int serv_pos = mid_point; serv_pos < MAX_SERVO_ANGLE && !obstacle_found; serv_pos = serv_pos + servo_step) {
    myservo.writeMicroseconds(serv_pos);
    obstacle_found = distance() <= MINIMUM_DISTANCE;
    char msg[255];
    sprintf(msg,"servo_pos: %d",serv_pos);
    logger.debug(msg);
    delay(SERVO_DELAY);
  }
  
  myservo.writeMicroseconds(mid_point);
  delay(SERVO_DELAY);
  
  logger.unident();
  return obstacle_found;
}


void car() {
  logger.debug("car(): beginning...");
  logger.ident();
  
  while(!obstacle_ahead()) {
    enableMotors();
    forward(300);   
    disableMotors(); 
    //delay(100);   
  }
  breakRobot(0);
  logger.unident();


}
void avoid()
{
    logger.debug("avoid()");
    logger.ident();

    enableMotors();
    backward(250);
    if(random(2)) {
      right(360);
    } else {
      left(360);
    }
    disableMotors();
    
    logger.unident();
}



