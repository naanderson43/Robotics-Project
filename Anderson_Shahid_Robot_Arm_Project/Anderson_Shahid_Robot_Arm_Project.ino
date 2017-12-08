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
void StopMotor(); 


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


void Motor(int motor, int enable, int speed, int direction){
  digitalWrite(motor, direction); 
  analogWrite(enable, speed); 
}

void MotorControl(int motor, int enable, int speed, int pot, int position, int tolerance){
    while(analogRead(pot) < (position - tolerance) || analogRead(pot) > (position + tolerance)){
    if(analogRead(pot) < (position - tolerance)){
      Motor(motor, enable, speed, HIGH); 
      while(analogRead(pot) < (position - tolerance)){}
      analogWrite(enable, 0);
      delay(700);
    }

    if(analogRead(motor) > (position + tolerance)){
      Motor(motor, enable, speed, LOW); 
      while(analogRead(pot) > (position + tolerance)){}
      analogWrite(enable, 0);
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
  // Set enable pins as outputs
  pinMode(enShoulderLift, OUTPUT);
  pinMode(enElbow, OUTPUT);
  pinMode(enWrist, OUTPUT);
  pinMode(enGripper, OUTPUT);
  pinMode(enShoulderRot, OUTPUT);

  // Set motor pins as outputs
  pinMode(shoulderLift, OUTPUT);
  pinMode(elbow, OUTPUT);
  pinMode(wrist, OUTPUT);
  pinMode(gripper, OUTPUT);
  pinMode(shoulderRot, OUTPUT);

  // Set all outputs low
  StopMotor();
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
// StopMotor()
//
// Sets all digital output pins to LOW
//-------------------------------------------------------------------------------
void StopMotor(){
  analogWrite(enShoulderLift, PWM_ENABLE_OFF);
  analogWrite(enElbow, PWM_ENABLE_OFF);
  analogWrite(enWrist, PWM_ENABLE_OFF);
  analogWrite(enGripper, PWM_ENABLE_OFF);
  analogWrite(enShoulderRot, PWM_ENABLE_OFF);
  digitalWrite(shoulderLift, LOW);
  digitalWrite(elbow, LOW);
  digitalWrite(wrist, LOW);
  digitalWrite(gripper, LOW);
  digitalWrite(shoulderRot, LOW);
}
