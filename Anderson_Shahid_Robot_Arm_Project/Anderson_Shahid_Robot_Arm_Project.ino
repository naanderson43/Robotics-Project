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

#define MOTOR_NUM 5
#define DEFAULT_SPEED 100

struct Motor{
  int _pin; 
  int _enable; 
  int _pot;
  int _tol; //tolerance 
};

enum MotorName{
  GRIPPER, 
  WRIST, 
  ELBOW, 
  SHOULDER_LIFT, 
  SHOULDER_ROT
};

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

void motor_write(int motor, int speed, int direction){
  digitalWrite(motors[motor]._pin, direction); 
  analogWrite(motors[motor]._enable, speed); 
}

void motor_stop(){
  for(int i = 0; i < MOTOR_NUM ; i++){
    analogWrite(motors[i]._enable, 0); 
    digitalWrite(motors[i]._pin, LOW); 
  }
}

void MotorControl(int motor, int position, int speed){
    while(analogRead(motors[motor]._pot) < (position - motors[motor]._tol) || analogRead(motors[motor]._pot) > (position + motors[motor]._tol)){
    if(analogRead(motors[motor]._pot) < (position - motors[motor]._tol)){
      motor_write(motor, speed, HIGH); 
      while(analogRead(motors[motor]._pot) < (position - motors[motor]._tol)){}
      motor_stop();
      delay(700);
    }

    if(analogRead(motors[motor]._pot) > (position + motors[motor]._tol)){
      motor_write(motor, speed, LOW); 
      while(analogRead(motors[motor]._pot) > (position + motors[motor]._tol)){}
      motor_stop();
      delay(700);
    }
  }
}

struct motor_state{
  int position[5];
  int speed[5]; 
};

struct motor_state move_forward[3]; 
void move_forward_setup(){
  for(int i = 0 ; i < MOTOR_NUM ; i++){
    move_forward[0].speed[i] = DEFAULT_SPEED; 
  }
  // initial state
  move_forward[0].position[GRIPPER] = 595; 
  move_forward[0].position[WRIST] = 549; 
  move_forward[0].position[ELBOW] = 219; 
  move_forward[0].position[SHOULDER_LIFT] = 551; 
  move_forward[0].position[SHOULDER_ROT] = (SHOULDER_LEFT+SHOULDER_RIGHT)/2; 

  //place state
  move_forward[0].position[GRIPPER] = 594; 
  move_forward[0].position[WRIST] = 557; 
  move_forward[0].position[ELBOW] = 218; 
  move_forward[0].position[SHOULDER_LIFT] = 600; 
 // move_forward[0].position[SHOULDER_ROT] = 0;
 
  //pull state
  move_forward[0].position[GRIPPER] = 0; 
  move_forward[0].position[WRIST] = 86; 
  move_forward[0].position[ELBOW] = 0; 
  move_forward[0].position[SHOULDER_LIFT] = 0; 
  move_forward[0].position[SHOULDER_ROT] = 0;
}

//-------------------------------------------------------------------------------
// Home()
//
// Moves robot arm to the "Home" position
//-------------------------------------------------------------------------------

struct motor_state home; 
void home_setup(void){
  for (int i = 0 ; i < MOTOR_NUM ; i++){
    home.speed[i] = DEFAULT_SPEED; 
  }
  home.position[GRIPPER] = GRIPPER_CLOSED; 
  home.position[ELBOW] = 350; 
  home.position[WRIST] = 500; 
  home.position[SHOULDER_LIFT] = 500; 
  home.position[SHOULDER_ROT] = 500; 
// rest positions 
//   ShoulderRot(SHOULDER_ROT_HOME);
//   Elbow((ELBOW_DOWN + ELBOW_UP) / 2);
//   ShoulderLift(SHOULDER_UP);
//   Elbow(ELBOW_DOWN);
//   Wrist(WRIST_DOWN);
//   Gripper(GRIPPER_CLOSED);

//   // Home position pot values
// #define SHOULDER_ROT_HOME 500
// #define SHOULDER_LIFT_HOME 500
// #define ELBOW_HOME 350
// #define WRIST_HOME 500
// #define GRIPPER_HOME 460
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
  motor_stop(); 
}

void loop() {
  MotorControl(GRIPPER, home.position[GRIPPER], home.speed[GRIPPER]);
  MotorControl(WRIST, home.position[WRIST], home.speed[WRIST]); 
  MotorControl(ELBOW, home.position[ELBOW], home.speed[ELBOW]); 
  MotorControl(SHOULDER_LIFT, home.position[SHOULDER_LIFT], home.speed[SHOULDER_LIFT]); 
  MotorControl(SHOULDER_ROT, home.position[SHOULDER_ROT], home.speed[SHOULDER_ROT]); 
  while(1);
}
