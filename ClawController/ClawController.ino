/**
 * ME4242 GROUP 5 ROBOTIC CLAW MACHINE CONTROL CODE
 * FOR ARDUINO UNO + DFR0008 Joystick Module
 */
#include <Servo.h>
//#include <SPI.h>
//#include <MFRC522.h>

// JOYSTICK MODULE - FROM DOCUMENTATION DFR0008
#define UP_BUTTON 8 // Activate the claw
#define DOWN_BUTTON A1 // Deactivate the claw
#define LEFT_BUTTON 9 // TODO: NEED TO DEACTIVATE THIS! TO USE RST
#define RIGHT_BUTTON 12 // TODO : NEED TO DEACTIVATE THIS! TO USE MISO
#define JOYSTICK_BUTTON A0
#define X_AXIS_JOYSTICK A3 // Used to move the Gripper Left-Right
#define Y_AXIS_JOYSTICK A2 // Used to move the servo pulley up and down
#define DEBOUNCE_DELAY 20

int xAxis = 0;
int yAxis = 0;
boolean grab = 0;
boolean letgo = 0;
char state = 'L';

// DC MOTOR & L298N PINS
#define ENB_L298N 5 // PWM
#define IN3_L298N 2 // Direction Change for DC motor (TODO: LEFT/RIGHT? HAVE TO CHECK)
#define IN4_L298N 4 // Direction Change for DC motor (TODO: LEFT/RIGHT? HAVE TO CHECK)

int motorSpeed = 0;

// SERVO PULLEY
#define SERVO_PIN 3

Servo pulley;
int servoPos = 0;
int prevPos = 0;
//
//// RFID READER
//#define SS_PIN 10
//#define RST_PIN 9

//MFRC522 mfrc522(SS_PIN, RST_PIN);

 
void setup() {
  Serial.begin(115200);
  // Init joystick
  pinMode(UP_BUTTON, INPUT);
  pinMode(DOWN_BUTTON, INPUT);
  pinMode(LEFT_BUTTON, INPUT);
  pinMode(RIGHT_BUTTON, INPUT);
  pinMode(JOYSTICK_BUTTON, INPUT);
  pinMode(X_AXIS_JOYSTICK, INPUT);
  pinMode(Y_AXIS_JOYSTICK, INPUT);
  // Init motor and L298N
  pinMode(ENB_L298N, OUTPUT);
  pinMode(IN3_L298N, OUTPUT);
  pinMode(IN4_L298N, OUTPUT);
  analogWrite(ENB_L298N, 0);
  // Init servo
  pulley.attach(SERVO_PIN);
  pulley.write(servoPos);
  // Init RFID
}

void loop() {
  xAxis = analogRead(X_AXIS_JOYSTICK);
  yAxis = analogRead(Y_AXIS_JOYSTICK);
  grab = isButtonPressed(UP_BUTTON);
  letgo = isButtonPressed(DOWN_BUTTON);

  // SEND MESSAGE TO FLUIDIC CONTROL BOARD
  if (grab || letgo) {
    state = letgo ? 'L' : 'G';
    Serial.write(state);
  }

  // X AXIS USED FOR LEFT-RIGHT CONTROL
  if (xAxis < 470) { 
    moveLeft();
  } else if (xAxis > 550) {
    moveRight();
  } else {
    stopMotor();
  }
  analogWrite(ENB_L298N, motorSpeed);

  // Y AXIS USED FOR UP-DOWN CONTROL
  if (yAxis < 470) {
    moveDown();
  } else if (yAxis > 550) {
    moveUp();
  } else {
    stopServo();
  }
//  servoPos = map(yAxis, 0, 1023, 180, 0);
//  pulley.write(servoPos); 

  
}

void moveLeft() {
  digitalWrite(IN3_L298N, HIGH);
  digitalWrite(IN4_L298N, LOW);
  motorSpeed = map(xAxis, 470, 0, 0, 255);
}

void moveRight() {
  digitalWrite(IN3_L298N, LOW);
  digitalWrite(IN4_L298N, HIGH);
  motorSpeed = map(xAxis, 550, 1023, 0, 255);
}

void stopMotor() {
  digitalWrite(IN3_L298N, LOW);
  digitalWrite(IN4_L298N, LOW);
  motorSpeed = 0;
}

void moveDown() {
  pulley.attach(SERVO_PIN);
  servoPos = map(yAxis, 470, 0, 0, 180);
  pulley.write(servoPos);  
}

void moveUp() {
  pulley.attach(SERVO_PIN);
  servoPos = map(yAxis, 550, 1023, 180, 0);
  pulley.write(servoPos);
}

void stopServo() {
  pulley.detach();
}

boolean isButtonPressed(int button) {
  if (digitalRead(button) == LOW) {
    delay(DEBOUNCE_DELAY);
    if (digitalRead(button) == LOW) {
      return true;
    }
  }
  return false;
}
