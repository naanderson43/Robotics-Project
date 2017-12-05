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
  clearOuts();
}

//-------------------------------------------------------------------------------
// loop()
//
// Runs control program
//-------------------------------------------------------------------------------
void loop() {

  // Start from "home" position
  Home();
  delay(2000);

  // Move to pick up location and pick up object
  ShoulderRot(((SHOULDER_RIGHT - SHOULDER_LEFT) / 4) + SHOULDER_LEFT);
  Elbow(ELBOW_UP - ((ELBOW_UP - ELBOW_DOWN) / 2));
  ShoulderLift(SHOULDER_DOWN - ((SHOULDER_DOWN - SHOULDER_UP) / 6));
  Wrist(WRIST_DOWN);
  Gripper(GRIPPER_CLOSED - ((GRIPPER_CLOSED - GRIPPER_OPEN) / 4));
  Wrist(WRIST_UP);

  // Move to drop-off location and drop off object
  ShoulderRot(SHOULDER_ROT_HOME);
  ShoulderLift(analogRead(shLiftPot) - 70);
  Elbow(analogRead(elbowPot) - 50);
//  ShoulderLift(SHOULDER_DOWN - ((SHOULDER_DOWN - SHOULDER_UP) / 4));
  Wrist(WRIST_DOWN);
  Gripper(GRIPPER_OPEN);

  // Return to "home" position
  Home();

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
      digitalWrite(enGripper, HIGH);
      while(analogRead(gripperPot) > (t + GRIPPER_TOL)){}
      digitalWrite(enGripper, LOW);
      delay(700);
    }
    
    if(analogRead(gripperPot) < (t - GRIPPER_TOL)){
      digitalWrite(gripper, LOW);
      digitalWrite(enGripper, HIGH);
      while(analogRead(gripperPot) < (t - GRIPPER_TOL)){}
      digitalWrite(enGripper, LOW);
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
      digitalWrite(enWrist, HIGH);
      while(analogRead(wristPot) < (t - WRIST_TOL)){}
      digitalWrite(enWrist, LOW);
      delay(700);
    }
    
    if(analogRead(wristPot) > (t + WRIST_TOL)){
      digitalWrite(wrist, LOW);
      digitalWrite(enWrist, HIGH);
      while(analogRead(wristPot) > (t + WRIST_TOL)){}
      digitalWrite(enWrist, LOW);
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
      digitalWrite(enElbow, HIGH);
      while(analogRead(elbowPot) < (t - ELBOW_TOL)){}
      digitalWrite(enElbow, LOW);
      delay(700);
    }

    if(analogRead(elbowPot) > (t + ELBOW_TOL)){
      digitalWrite(elbow, LOW);
      digitalWrite(enElbow, HIGH);
      while(analogRead(elbowPot) > (t + ELBOW_TOL)){}
      digitalWrite(enElbow, LOW);
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
      digitalWrite(enShoulderLift, HIGH);
      while(analogRead(shLiftPot) > (t + SH_LIFT_TOL)){}
      digitalWrite(enShoulderLift, LOW);
      delay(700);
    }

    if(analogRead(shLiftPot) < (t - SH_LIFT_TOL)){
      digitalWrite(shoulderLift, LOW);
      digitalWrite(enShoulderLift, HIGH);
      while(analogRead(shLiftPot) < (t - SH_LIFT_TOL)){}
      digitalWrite(enShoulderLift, LOW);
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
      digitalWrite(enShoulderRot, HIGH);
      while(analogRead(shRotPot) < (t - SH_ROT_TOL)){}
      digitalWrite(enShoulderRot, LOW);
      delay(700);
    }

    if(analogRead(shRotPot) > (t + SH_ROT_TOL)){
      digitalWrite(shoulderRot, LOW);
      digitalWrite(enShoulderRot, HIGH);
      while(analogRead(shRotPot) > (t + SH_ROT_TOL)){}
      digitalWrite(enShoulderRot, LOW);
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
// clearOuts()
//
// Sets all digital output pins to LOW
//-------------------------------------------------------------------------------
void clearOuts(){
  digitalWrite(enShoulderLift, LOW);
  digitalWrite(enElbow, LOW);
  digitalWrite(enWrist, LOW);
  digitalWrite(enGripper, LOW);
  digitalWrite(enShoulderRot, LOW);
  digitalWrite(shoulderLift, LOW);
  digitalWrite(elbow, LOW);
  digitalWrite(wrist, LOW);
  digitalWrite(gripper, LOW);
  digitalWrite(shoulderRot, LOW);
}
