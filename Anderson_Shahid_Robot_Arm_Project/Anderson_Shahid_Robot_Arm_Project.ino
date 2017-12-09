/*  Robot arm control program for
 *  ECE 5397: Introduction to Robotics
 *  University of Houston, Fall 2017
 *  Authors: Nick Anderson & Sikender Shahid
 *  Course Instructor: Dr. Aaron Becker
 */
//-------------------------------------------------------------------------------
// Globals
//-------------------------------------------------------------------------------

// Motor enable pins
int enGripper = 8;       //Enable gripper motor
int enWrist = 9;         //Enable wrist motor
int enElbow = 10;        //Enable elbow motor
int enShoulderLift = 11; //Enable shoulder lift motor
int enShoulderRot = 12;  //Enable shoulder rotation motor
int enLight =13; 
  
// Motor control pins
int gripper = 2;        //Gripper close Pin
int wrist = 3;          //Wrist up pin
int elbow = 4;          //Elbow up pin
int shoulderLift = 5;   //Shoulder down pin
int shoulderRot = 6;    //Shoulder right pin
int light = 7;

// Potentiometer analog input pins
int gripperPot = 0;
int wristPot = 1;
int elbowPot = 2;
int shLiftPot = 3;
int shRotPot = 4;

//-------------------------------------------------------------------------------
// Defines
//-------------------------------------------------------------------------------
// PWM for ENABLE WRITE
int PWM_ENABLE_ON = 100;
#define PWM_ENABLE_OFF 0

// Pot value limits
#define GRIPPER_OPEN 460
#define GRIPPER_CLOSED 677
#define WRIST_UP 960
#define WRIST_DOWN 125
#define ELBOW_UP 1000
#define ELBOW_DOWN 130
#define SHOULDER_UP 325
#define SHOULDER_DOWN 925
#define SHOULDER_LEFT 150
#define SHOULDER_RIGHT 925

// Home position pot values
#define SHOULDER_ROT_HOME 500
#define SHOULDER_LIFT_HOME 500
#define ELBOW_HOME 350
#define WRIST_HOME 500
#define GRIPPER_HOME 460

// Position tolerances
#define GRIPPER_TOL 50
#define WRIST_TOL 40
#define ELBOW_TOL 20
#define SH_LIFT_TOL 20
#define SH_ROT_TOL 5

//-------------------------------------------------------------------------------
// Prototypes
//-------------------------------------------------------------------------------
void Gripper(int t);
void Wrist(int t);
void Elbow(int t);
void ShoulderLift(int t);
void ShoulderRot(int t);
void Rest(void);
void Home(void);
void MotorStop(); 


/*
turning arm pressure down 
picking up the base
rotating the base <Camera>
*/

/*
moving forward
*/

/*
moving backward
*/
#define MOTOR_NUM 5

struct Motor{
  int _pin; 
  int _enable; 
  int _pot;
  int _tol; //tolerance 
}

Motor motors[MOTOR_NUM]; 

void MotorSetup(){
  for(int i = 0 ; i < MOTOR_NUM ; i++){
    motors[i]._pin = 2 + i; 
    motors[i]._enable = 8 + i; 
    motors[i]._pot = i; 
  }
  motors[0]._tol = 50; 
  motors[1]._tol = 40; 
  motors[2]._tol = 20; 
  motors[3]._tol = 20; 
  motors[4]._tol = 5;
}

void Motor(int motor, int speed, int direction){
  digitalWrite(motors[motor]._pin, direction); 
  analogWrite(motors[motor]._enable, speed); 
}

void MotorStop(){
  for(int i = 0; i < MOTOR_NUM ; i++){
    analogWrite(motors[i]._enable, 0); 
    digitalWrite(motors[i]._pin, LOW); 
  }
}

void MotorControl(int motor, int speed, int position){
    while(analogRead(motors[motor]._pot) < (position - motors[motor]._tol) || analogRead(motors[motor]._pot) > (position + motors[motor]._tol)){
    if(analogRead(motors[motor]._pot) < (position - motors[motor]._tol)){
      Motor(motor, enable, speed, HIGH); 
      while(analogRead(motors[motor]._pot) < (position - motors[motor]._tol)){}
      MotorStop();
      delay(700);
    }

    if(analogRead(motors[motor]._pot) > (position + motors[motor]._tol)){
      Motor(motor, enable, speed, LOW); 
      while(analogRead(motors[motor]._pot) > (position + motors[motor]._tol)){}
      MotorStop();
      delay(700);
    }
  }
}

//-------------------------------------------------------------------------------
// setup()
//
// Configures digital pins and clears outputs
//-------------------------------------------------------------------------------
void setup() {
  for(int i = 0 ; i < MOTOR_NUM ; i++){
    pinMode(motors[i]._enable, OUTPUT); 
    pinMode(motors[i]._pin, OUTPUT);  
  }
  MotorStop(); 
}

//-------------------------------------------------------------------------------
// loop()
//
// Runs control program
//-------------------------------------------------------------------------------
void loop() {
 while(1){ 
  //init state
  ShoulderLift(551);
  Gripper(595); 
  Wrist(549); 
  Elbow(219); 
  
  delay(500); 
  
  //place state 
  Gripper(594); 
  Wrist(557); 
  Elbow(218); 
  ShoulderLift(600);
  delay(500); 
  
  //pull state 
  Wrist(86);

  delay(500);  
}
  
  
  while(1); 
 //Working pick up 
  Elbow(ELBOW_UP - ((ELBOW_UP - ELBOW_DOWN) / 2)); 
  ShoulderRot((SHOULDER_LEFT+SHOULDER_RIGHT)/2);  
  while(0); 
  Gripper(593); 
  Wrist(366); 
  Elbow(245);
  ShoulderLift(635);
  while(1){
   ShoulderRot(SHOULDER_LEFT+200); 
   ShoulderRot(SHOULDER_RIGHT-200); 
    
  } 


  //MotorControl(int motor, int enable, int speed, int pot, int position, int tolerance)
  //
  //Home(); 
  //Gripper(GRIPPER_CLOSED - ((GRIPPER_CLOSED - GRIPPER_OPEN)/4)); 
  //ShoulderLift(SHOULDER_DOWN - ((SHOULDER_DOWN - SHOULDER_UP) / 6));
  
  // Start from "home" position
  // Home();
  // delay(2000);

  // // Move to pick up location and pick up object
  // ShoulderRot(((SHOULDER_RIGHT - SHOULDER_LEFT) / 4) + SHOULDER_LEFT);
  // Elbow(ELBOW_UP - ((ELBOW_UP - ELBOW_DOWN) / 2));
  // ShoulderLift(SHOULDER_DOWN - ((SHOULDER_DOWN - SHOULDER_UP) / 6));
  // Wrist(WRIST_DOWN);
  // Gripper(GRIPPER_CLOSED - ((GRIPPER_CLOSED - GRIPPER_OPEN) / 4));
  // Wrist(WRIST_UP);

  // // Move to drop-off location and drop off object
  // ShoulderRot(SHOULDER_ROT_HOME);
  // ShoulderLift(analogRead(shLiftPot) - 70);
  // Elbow(analogRead(elbowPot) - 50);
  // //  ShoulderLift(SHOULDER_DOWN - ((SHOULDER_DOWN - SHOULDER_UP) / 4));
  // Wrist(WRIST_DOWN);
  // Gripper(GRIPPER_OPEN);

  // // Return to "home" position
  // Home();

  // End program
  while(true){}
}

//-------------------------------------------------------------------------------
// Gripper()
//
// Adjusts gripper opening to within the defined
// tolerance of the desired setting
//-------------------------------------------------------------------------------
void Gripper(int t){
  while(analogRead(gripperPot) > (t + GRIPPER_TOL) || analogRead(gripperPot) < (t - GRIPPER_TOL)){
    if(analogRead(gripperPot) > (t + GRIPPER_TOL)){
      digitalWrite(gripper, HIGH);
      analogWrite(enGripper, PWM_ENABLE_ON);
      while(analogRead(gripperPot) > (t + GRIPPER_TOL)){}
      analogWrite(enGripper, PWM_ENABLE_OFF);
      delay(700);
    }
    
    if(analogRead(gripperPot) < (t - GRIPPER_TOL)){
      digitalWrite(gripper, LOW);
      analogWrite(enGripper, PWM_ENABLE_ON);
      while(analogRead(gripperPot) < (t - GRIPPER_TOL)){}
      analogWrite(enGripper, PWM_ENABLE_OFF);
      delay(700);
    }
  }
}

//-------------------------------------------------------------------------------
// Wrist()
//
// Adjusts wrist angle to within the defined
// tolerance of the desired setting
//-------------------------------------------------------------------------------
void Wrist(int t){
  while(analogRead(wristPot) < (t - WRIST_TOL) || analogRead(wristPot) > (t + WRIST_TOL)){
    if(analogRead(wristPot) < (t - WRIST_TOL)){
      digitalWrite(wrist, HIGH);
      analogWrite(enWrist, PWM_ENABLE_ON);
      while(analogRead(wristPot) < (t - WRIST_TOL)){}
      analogWrite(enWrist, PWM_ENABLE_OFF);
      delay(700);
    }  
    
    if(analogRead(wristPot) > (t + WRIST_TOL)){
      digitalWrite(wrist, LOW);
      analogWrite(enWrist, PWM_ENABLE_ON);
      while(analogRead(wristPot) > (t + WRIST_TOL)){}
      analogWrite(enWrist, PWM_ENABLE_OFF);
      delay(700);
    }
  }  
}

//-------------------------------------------------------------------------------
// Elbow()
//
// Adjusts elbow angle to within the defined
// tolerance of the desired setting
//-------------------------------------------------------------------------------
void Elbow(int t){
  while(analogRead(elbowPot) < (t - ELBOW_TOL) || analogRead(elbowPot) > (t + ELBOW_TOL)){
    if(analogRead(elbowPot) < (t - ELBOW_TOL)){
      digitalWrite(elbow, HIGH);
      analogWrite(enElbow, PWM_ENABLE_ON);
      while(analogRead(elbowPot) < (t - ELBOW_TOL)){}
      analogWrite(enElbow, PWM_ENABLE_OFF);
      delay(700);
    }

    if(analogRead(elbowPot) > (t + ELBOW_TOL)){
      digitalWrite(elbow, LOW);
      analogWrite(enElbow, PWM_ENABLE_ON);
      while(analogRead(elbowPot) > (t + ELBOW_TOL)){}
      analogWrite(enElbow, PWM_ENABLE_OFF);
      delay(700);
    }
  }
}

//-------------------------------------------------------------------------------
// ShoulderLift()
//
// Adjusts shoulder lift angle to within the defined
// tolerance of the desired setting
//-------------------------------------------------------------------------------
void ShoulderLift(int t){
  while(analogRead(shLiftPot) > (t + SH_LIFT_TOL) || analogRead(shLiftPot) < (t - SH_LIFT_TOL)){
    if(analogRead(shLiftPot) > (t + SH_LIFT_TOL)){
      digitalWrite(shoulderLift, HIGH);
      analogWrite(enShoulderLift, PWM_ENABLE_ON);
      while(analogRead(shLiftPot) > (t + SH_LIFT_TOL)){}
      analogWrite(enShoulderLift, PWM_ENABLE_OFF);
      delay(700);
    }

    if(analogRead(shLiftPot) < (t - SH_LIFT_TOL)){
      digitalWrite(shoulderLift, LOW);
      analogWrite(enShoulderLift, PWM_ENABLE_ON);
      while(analogRead(shLiftPot) < (t - SH_LIFT_TOL)){}
      analogWrite(enShoulderLift, PWM_ENABLE_OFF);
      delay(700);
    }
  }
}

//-------------------------------------------------------------------------------
// ShoulderRot()
//
// Adjusts shoulder rotation angle to within the defined
// tolerance of the desired setting
//-------------------------------------------------------------------------------
void ShoulderRot(int t){
  while(analogRead(shRotPot) < (t - SH_ROT_TOL) || analogRead(shRotPot) > (t + SH_ROT_TOL)){
    if(analogRead(shRotPot) < (t - SH_ROT_TOL)){
      digitalWrite(shoulderRot, HIGH);
      analogWrite(enShoulderRot, PWM_ENABLE_ON);
      while(analogRead(shRotPot) < (t - SH_ROT_TOL)){}
      analogWrite(enShoulderRot, PWM_ENABLE_OFF);
      delay(700);
    }

    if(analogRead(shRotPot) > (t + SH_ROT_TOL)){
      digitalWrite(shoulderRot, LOW);
      analogWrite(enShoulderRot, PWM_ENABLE_ON);
      while(analogRead(shRotPot) > (t + SH_ROT_TOL)){}
      analogWrite(enShoulderRot, PWM_ENABLE_OFF);
      delay(700);
    }
  }
}

//-------------------------------------------------------------------------------
// Rest()
//
// Moves robot arm to the "rest" position
//-------------------------------------------------------------------------------
void Rest(void){
  ShoulderRot(SHOULDER_ROT_HOME);
  Elbow((ELBOW_DOWN + ELBOW_UP) / 2);
  ShoulderLift(SHOULDER_UP);
  Elbow(ELBOW_DOWN);
  Wrist(WRIST_DOWN);
  Gripper(GRIPPER_CLOSED);
}

//-------------------------------------------------------------------------------
// Home()
//
// Moves robot arm to the "Home" position
//-------------------------------------------------------------------------------
void Home(void){
  ShoulderLift(SHOULDER_LIFT_HOME);
  Elbow(ELBOW_HOME);
  Wrist(WRIST_HOME);
  Gripper(GRIPPER_HOME);
  ShoulderRot(SHOULDER_ROT_HOME);
}

//-------------------------------------------------------------------------------
// MotorStop()
//
// Sets all digital output pins to LOW
//-------------------------------------------------------------------------------

