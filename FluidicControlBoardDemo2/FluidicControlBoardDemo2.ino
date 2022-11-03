/**
 * ME4242 GROUP 5 ROBOTIC CLAW MACHINE CONTROL CODE
 * FOR FLUIDIC CONTROL KIT
 */
//Pin Assigement 
#define VALVE_CTRL_0  3
#define VALVE_CTRL_1  9
#define VALVE_CTRL_2  10
#define VALVE_CTRL_3  11
#define PUMP_CTRL     5
#define PRESSURE_0    A0
#define PRESSURE_1    A1
#define PRESSURE_2    A2
#define PRESSURE_3    A3

//User Variables
int PRESSURE[1] ={0};
char state = 'L';
#define PUMP_IN 255 // MAX for now

//define pump ON/OFF control

void pumpOn(int val) { // val can be between 0 to 255
  analogWrite(PUMP_CTRL, val);
}

void pumpOff() {
  digitalWrite(PUMP_CTRL, LOW);
}

void valveOn(byte VALVE_INDEX){
  digitalWrite(VALVE_INDEX, HIGH);
}

void valveOff(byte VALVE_INDEX){
  digitalWrite(VALVE_INDEX, LOW);
}

void readPressure(){
  PRESSURE[0]=analogRead(PRESSURE_0);
  PRESSURE[1]=analogRead(PRESSURE_1);
  PRESSURE[2]=analogRead(PRESSURE_2);
  PRESSURE[3]=analogRead(PRESSURE_3);
}
void setup() {
  Serial.begin(115200);
  pinMode(VALVE_CTRL_0, OUTPUT);
  pinMode(VALVE_CTRL_1, OUTPUT);
  pinMode(VALVE_CTRL_2, OUTPUT);
  pinMode(VALVE_CTRL_3, OUTPUT);
  pinMode(PUMP_CTRL, OUTPUT);
  valveOff(VALVE_CTRL_0);
  valveOff(VALVE_CTRL_1);
  valveOff(VALVE_CTRL_2);
  valveOff(VALVE_CTRL_3);
  pumpOff();
}

void loop() {
  readPressure();
  // Serial.println(PRESSURE[0]);

  if (Serial.available() > 1) {
    state = Serial.read();
    state = (char)state;
  }

  if (state == 'G') {
    valveOn(VALVE_CTRL_0);
    pumpOn(PUMP_IN);
    delay(5000);
    pumpOn(PUMP_IN/2); // let it deflate a bit to prevent major leakage
    delay(200);
  } else {
    valveOff(VALVE_CTRL_0);
    pumpOff();
  }
}
