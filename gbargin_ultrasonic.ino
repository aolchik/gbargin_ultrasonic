#include <Servo.h>
Servo myservo;
int enableA = 1;
int pinA1 = 3;
int pinA2 = 2;

int servposnum = 0;
int servpos = 0;

int enableB = 6;
int pinB1 = 5;
int pinB2 = 4;
#define trigPin 7
#define echoPin 8

// Logger Class
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
  pinMode (enableA, OUTPUT);
  pinMode (pinA1, OUTPUT);
  pinMode (pinA2, OUTPUT);

  pinMode (enableB, OUTPUT);
  pinMode (pinB1, OUTPUT);
  pinMode (pinB2, OUTPUT); 

  //configure pin modes for the ultrasonci se3nsor
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  Serial.begin(9600);
  logger.set_level(DEBUG);
  logger.debug("Oi!");
  logger.debug("Iniciando o robô...");

  //Servo pins
  myservo.attach(9);
}

void loop() {
  // put your main code here, to run repeatedly:
  car();
  avoid();    
}

//Defining functions so it's more easy
//motor functions
void motorAforward() {
 logger.debug("motorAforward()");
 
 digitalWrite (pinA1, HIGH);
 digitalWrite (pinA2, LOW);
}
void motorBforward() {
 logger.debug("motorAforward()");

 digitalWrite (pinB1, LOW);
 digitalWrite (pinB2, HIGH);
}
void motorAbackward() {
 digitalWrite (pinA1, LOW);
 digitalWrite (pinA2, HIGH);
}
void motorBbackward() {
 digitalWrite (pinB1, HIGH);
 digitalWrite (pinB2, LOW);
}
void motorAstop() {
 digitalWrite (pinA1, HIGH);
 digitalWrite (pinA2, HIGH);
}
void motorBstop() {
 digitalWrite (pinB1, HIGH);
 digitalWrite (pinB2, HIGH);
}
void motorAcoast() {
 digitalWrite (pinA1, LOW);
 digitalWrite (pinA2, LOW);
}
void motorBcoast() {
 digitalWrite (pinB1, LOW);
 digitalWrite (pinB2, LOW);
}
void motorAon() {
 digitalWrite (enableA, HIGH);
}
void motorBon() {
 digitalWrite (enableB, HIGH);
}
void motorAoff() {
 digitalWrite (enableA, LOW);
}
void motorBoff() {
 digitalWrite (enableB, LOW);
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
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
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
    forward(100);   
    disableMotors(); 
    delay(100);   
  }
  breakRobot(0);
  logger.unident();


}
void avoid()
{
    logger.debug("avoid()");
    logger.ident();
    
    backward(250);
    if(random(2)) {
      right(360);
    } else {
      left(360);
    }
    
    logger.unident();
}



