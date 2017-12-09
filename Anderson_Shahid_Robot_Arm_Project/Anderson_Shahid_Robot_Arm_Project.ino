/*  Robot arm control program for
 *  ECE 5397: Introduction to Robotics
 *  University of Houston, Fall 2017
 *  Authors: Nick Anderson & Sikender Shahid
 *  Course Instructor: Dr. Aaron Becker
 */
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

#define MOTOR_NUM 5

struct Motor{
  int _pin; 
  int _enable; 
  int _pot;
  int _tol; //tolerance 
}

enum MotorName{
  GRIPPER, 
  WRIST, 
  ELBOW, 
  SHOULDER_LIFT, 
  SHOULDER_ROT
}

struct Motor motors[MOTOR_NUM]; 

void MotorSetup(){
  for(int i = 0 ; i < MOTOR_NUM ; i++){
    motors[i]._pin = 2 + i; 
    motors[i]._enable = 8 + i; 
    motors[i]._pot = i; 
  }
  motors[GRIPPER]._tol = 50; 
  motors[WRIST]._tol = 40; 
  motors[ELBOW]._tol = 20; 
  motors[SHOULDER_LIFT]._tol = 20; 
  motors[SHOULDER_ROT]._tol = 5;
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
// struct motor_state{
//   int *motor; 
//   int *position;
// }
// struct motor_state* move_forward; 
// void move_forward_setup(){
//   struct motor_state * move_forward= (struct motor_s*) 3; 
//   move_forward[0]->motor = (int*)malloc((5)*sizeof(int)); 
//   move_forward[0]->position = (int*)malloc((5)*sizeof(int)); 
// }


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

