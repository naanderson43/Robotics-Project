/*  Robot arm control program for
 *  ECE 5397: Introduction to Robotics
 *  University of Houston, Fall 2017
 *  Authors: Nick Anderson & Sikender Shahid
 *  Course Instructor: Dr. Aaron Becker
 *  
 *  NOTE: Code that has been commented out was intended to be used for interfacing with 
 *        a MatLAB program that would process images from two cameras and send commands
 *        to the robot to direct it to the object to be picked up. This portion of the
 *        code was not completed due to time constraints and other setbacks.
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
  
// Motor control pins
int gripper = 2;        //Gripper control Pin
int wrist = 3;          //Wrist control pin
int elbow = 4;          //Elbow control pin
int shoulderLift = 5;   //Shoulder control pin
int shoulderRot = 6;    //Shoulder control pin

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
#define GRIPPER_OPEN 400
#define GRIPPER_CLOSED 575
#define WRIST_UP 965
#define WRIST_DOWN 130
#define ELBOW_UP 890
#define ELBOW_DOWN 150
#define SHOULDER_UP 325
#define SHOULDER_DOWN 925
#define SHOULDER_LEFT 150
#define SHOULDER_RIGHT 925

// Home position pot values
#define SHOULDER_ROT_HOME 500
#define SHOULDER_LIFT_HOME 500
#define ELBOW_HOME 350
#define WRIST_HOME 250
#define GRIPPER_HOME 460

// Position tolerances
#define GRIPPER_TOL 50
#define WRIST_TOL 40
#define ELBOW_TOL 20
#define SH_LIFT_TOL 20
#define SH_ROT_TOL 10

#define PWM_MAX 255


//-------------------------------------------------------------------------------
// Prototypes
//-------------------------------------------------------------------------------
void Gripper(int t, int pwm);
void Wrist(int t, int pwm);
void Elbow(int t, int pwm);
void ShoulderLift(int t, int pwm);
void ShoulderRot(int t, int pwm);
void Rest(void);
void Home(void);
void Pull(void);
void LeftTurn(void);
void RightTurn(void);
void PickUp(void);
void clearOuts(void);


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

  // Initialize serial and wait for port to open:
//  Serial.begin(9600);
//  while(!Serial){} // Wait for serial port to connect
//  // FIXME: need to wait here until MatLAB program signals that it is ready
//}

//-------------------------------------------------------------------------------
// loop()
//
// Runs control program
//-------------------------------------------------------------------------------
void loop() {
  int i;

  delay(5000);

  Home();
  
  for(i = 1; i <= 4; i++){
    LeftTurn();
  }

  Pull();

  RightTurn();

  for(i = 1; i <= 2; i++){
    Pull();
  }


  PickUp();

  delay(3000);

  Rest();
  
  while(1){}
  

  
//  static int i = 0;
//  int j;
//  static char inStr[4];
//  char c;
//
//  while(Serial.available() > 0){
//    c = Serial.read();
//    inStr[i] = c;
//
//    if(c == 13){
//      inStr[i] = '\0';
//      
//      // Commands used in MatLAB program
//      if(!strcmp(inStr, "LT")){LeftTurn();}
//      if(!strcmp(inStr, "RT")){RightTurn();}
//      if(!strcmp(inStr, "PF")){Pull();}
//      if(!strcmp(inStr, "HM")){Home();}
//      if(!strcmp(inStr, "RS")){Rest();}
//      if(!strcmp(inStr, "GO")){Gripper(GRIPPER_OPEN, 128);}
//      if(!strcmp(inStr, "GG")){Gripper(GRIPPER_OPEN + 60, 128);}
//      if(!strcmp(inStr, "PU")){PickUp();}
//      
//      
//      for(j = 0; j < (sizeof(inStr)/(sizeof(inStr[0]))); j++){
//        inStr[j] = NULL;
//      }
//
//      i = 0;
//    }
//    else{i++;}
//  }
}

//-------------------------------------------------------------------------------
// Gripper()
//
// Adjusts gripper opening to within the defined
// tolerance of the desired setting
//-------------------------------------------------------------------------------
void Gripper(int t, int pwm){
  while(analogRead(gripperPot) > (t + GRIPPER_TOL) || analogRead(gripperPot) < (t - GRIPPER_TOL)){
    if(analogRead(gripperPot) > (t + GRIPPER_TOL)){
      digitalWrite(gripper, LOW);
      analogWrite(enGripper, pwm);
      while(analogRead(gripperPot) > (t + GRIPPER_TOL)){}
      digitalWrite(enGripper, LOW);
      delay(700);
    }
    
    if(analogRead(gripperPot) < (t - GRIPPER_TOL)){
      digitalWrite(gripper, HIGH);
      analogWrite(enGripper, pwm);
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
void Wrist(int t, int pwm){
  while(analogRead(wristPot) < (t - WRIST_TOL) || analogRead(wristPot) > (t + WRIST_TOL)){
    if(analogRead(wristPot) < (t - WRIST_TOL)){
      digitalWrite(wrist, HIGH);
      analogWrite(enWrist, pwm);
      while(analogRead(wristPot) < (t - WRIST_TOL)){}
      digitalWrite(enWrist, LOW);
      delay(700);
    }
    
    if(analogRead(wristPot) > (t + WRIST_TOL)){
      digitalWrite(wrist, LOW);
      analogWrite(enWrist, pwm);
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
void Elbow(int t, int pwm){
  while(analogRead(elbowPot) < (t - ELBOW_TOL) || analogRead(elbowPot) > (t + ELBOW_TOL)){
    if(analogRead(elbowPot) < (t - ELBOW_TOL)){
      digitalWrite(elbow, HIGH);
      analogWrite(enElbow, pwm);
      while(analogRead(elbowPot) < (t - ELBOW_TOL)){}
      digitalWrite(enElbow, LOW);
      delay(700);
    }

    if(analogRead(elbowPot) > (t + ELBOW_TOL)){
      digitalWrite(elbow, LOW);
      analogWrite(enElbow, pwm);
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
void ShoulderLift(int t, int pwm){
  while(analogRead(shLiftPot) > (t + SH_LIFT_TOL) || analogRead(shLiftPot) < (t - SH_LIFT_TOL)){
    if(analogRead(shLiftPot) > (t + SH_LIFT_TOL)){
      digitalWrite(shoulderLift, LOW);
      analogWrite(enShoulderLift, pwm);
      while(analogRead(shLiftPot) > (t + SH_LIFT_TOL)){}
      digitalWrite(enShoulderLift, LOW);
      delay(700);
    }

    if(analogRead(shLiftPot) < (t - SH_LIFT_TOL)){
      digitalWrite(shoulderLift, HIGH);
      analogWrite(enShoulderLift, pwm);
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
void ShoulderRot(int t, int pwm){
  while(analogRead(shRotPot) < (t - SH_ROT_TOL) || analogRead(shRotPot) > (t + SH_ROT_TOL)){
    if(analogRead(shRotPot) < (t - SH_ROT_TOL)){
      digitalWrite(shoulderRot, HIGH);
      analogWrite(enShoulderRot, pwm);
      while(analogRead(shRotPot) < (t - SH_ROT_TOL)){}
      digitalWrite(enShoulderRot, LOW);
      delay(700);
    }

    if(analogRead(shRotPot) > (t + SH_ROT_TOL)){
      digitalWrite(shoulderRot, LOW);
      analogWrite(enShoulderRot, pwm);
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
  ShoulderRot(SHOULDER_ROT_HOME, PWM_MAX / 2);
  Elbow((ELBOW_DOWN + ELBOW_UP) / 2, PWM_MAX);
  ShoulderLift(SHOULDER_UP, PWM_MAX / 2);
  Elbow(ELBOW_DOWN, PWM_MAX);
  Wrist(WRIST_DOWN, 115);
  Gripper(GRIPPER_OPEN + 70, 125);
}

//-------------------------------------------------------------------------------
// Home()
//
// Moves robot arm to the "Home" position
//-------------------------------------------------------------------------------
void Home(void){
  ShoulderLift(SHOULDER_LIFT_HOME, 128);
  Elbow(ELBOW_HOME, PWM_MAX);
  Wrist(WRIST_HOME, 100);
  ShoulderRot(SHOULDER_ROT_HOME, 100);
  Gripper(GRIPPER_OPEN, 125);
}

//-------------------------------------------------------------------------------
// Pull()
//
// Robot arm pulls itself forward
//-------------------------------------------------------------------------------
void Pull(void){
  Home();
  Elbow(510, 128);
  ShoulderLift(885,255);
  digitalWrite(shoulderLift, LOW);
  digitalWrite(elbow, LOW);
  analogWrite(enElbow, 255);
  while(analogRead(elbowPot) > (ELBOW_DOWN + 100 + ELBOW_TOL)){
    analogWrite(enShoulderLift, 255);
    delay(250);
    digitalWrite(enShoulderLift, LOW);
    delay(550);
  }
  digitalWrite(enElbow, LOW);
  digitalWrite(enShoulderLift, LOW);
  Home();
}

//-------------------------------------------------------------------------------
// LeftTurn()
//
// Robot arm turns left
//-------------------------------------------------------------------------------
void LeftTurn(void){
  ShoulderLift(SHOULDER_LIFT_HOME, 128);
  ShoulderRot(SHOULDER_ROT_HOME, 100);
  Gripper(GRIPPER_CLOSED, 128);
  Elbow(250, 128);
  Wrist(250, 100);
  ShoulderLift(660, 255);
  ShoulderRot(565, 150);
  ShoulderLift(SHOULDER_LIFT_HOME, 128);
  ShoulderRot(SHOULDER_ROT_HOME, 100);
}

//-------------------------------------------------------------------------------
// RightTurn()
//
// Robot arm turns right
//-------------------------------------------------------------------------------
void RightTurn(void){
  ShoulderLift(SHOULDER_LIFT_HOME, 128);
  ShoulderRot(SHOULDER_ROT_HOME, 100);
  Gripper(GRIPPER_CLOSED, 128);
  Elbow(250, 128);
  Wrist(250, 100);
  ShoulderLift(660, 255);
  ShoulderRot(435, 150);
  ShoulderLift(SHOULDER_LIFT_HOME, 128);
  ShoulderRot(SHOULDER_ROT_HOME, 100);
}

//-------------------------------------------------------------------------------
// PickUp()
//
// Robot arm picks up object
//-------------------------------------------------------------------------------
void PickUp(void){
  Home();
  Elbow(450, 128);
  ShoulderLift(750,100);
  Gripper(GRIPPER_OPEN + 70, 100);
  ShoulderLift(SHOULDER_LIFT_HOME, 200);
  Wrist(500, 128);
  Elbow(510, 128);
}


//-------------------------------------------------------------------------------
// clearOuts()
//
// Sets all digital output pins to LOW
//-------------------------------------------------------------------------------
void clearOuts(void){
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
