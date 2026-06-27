#include <AFMotor.h>
#include <NewPing.h>
#include <Servo.h>

// Sensor Pins
#define RIGHT A2
#define LEFT A3
#define TRIGGER_PIN A1
#define ECHO_PIN A0
#define MAX_DISTANCE 100

// Initialize Ultrasonic Sensor
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);

// Initialize Motors via L293D Shield
AF_DCMotor Motor1(1, MOTOR12_1KHZ);
AF_DCMotor Motor2(2, MOTOR12_1KHZ);
AF_DCMotor Motor3(3, MOTOR34_1KHZ);
AF_DCMotor Motor4(4, MOTOR34_1KHZ);

Servo myservo;
int pos = 0;

void setup() {
  Serial.begin(9600);
  
  // Attach Servo to pin 10
  myservo.attach(10);
  
  // Servo Initialization Sweep (Look Left to Right)
  for(pos = 90; pos <= 180; pos += 1) {
    myservo.write(pos);
    delay(15);
  }
  for(pos = 180; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(15);
  }
  for(pos = 0; pos <= 90; pos += 1) {
    myservo.write(pos);
    delay(15);
  }
  
  // Set IR pins as inputs
  pinMode(RIGHT, INPUT);
  pinMode(LEFT, INPUT);
}

void loop() {
  delay(50);
  
  // Read distance from Ultrasonic Sensor
  unsigned int distance = sonar.ping_cm();
  
  // Read values from IR Sensors
  int Right_Value = digitalRead(RIGHT);
  int Left_Value = digitalRead(LEFT);

  // --- Following Logic ---
  
  // Target is straight ahead
  if((Right_Value == 1) && (Left_Value == 1)) {
    if(distance > 1 && distance < 15) {
      // Too close, stop moving
      stopRobot();
    } else if(distance >= 15 && distance <= 35) {
      // Target in range, move forward
      moveForward();
    }
  } 
  // Target is drifting to the Right
  else if((Right_Value == 0) && (Left_Value == 1)) {
    turnRight();
  } 
  // Target is drifting to the Left
  else if((Right_Value == 1) && (Left_Value == 0)) {
    turnLeft();
  } 
  // Target is lost or completely out of bounds
  else if((Right_Value == 0) && (Left_Value == 0)) {
    stopRobot();
  }
}

// --- Movement Helper Functions ---

void moveForward() {
  Motor1.setSpeed(150); Motor1.run(FORWARD);
  Motor2.setSpeed(150); Motor2.run(FORWARD);
  Motor3.setSpeed(150); Motor3.run(FORWARD);
  Motor4.setSpeed(150); Motor4.run(FORWARD);
}

void turnRight() {
  Motor1.setSpeed(150); Motor1.run(FORWARD);
  Motor2.setSpeed(150); Motor2.run(FORWARD);
  Motor3.setSpeed(150); Motor3.run(BACKWARD);
  Motor4.setSpeed(150); Motor4.run(BACKWARD);
}

void turnLeft() {
  Motor1.setSpeed(150); Motor1.run(BACKWARD);
  Motor2.setSpeed(150); Motor2.run(BACKWARD);
  Motor3.setSpeed(150); Motor3.run(FORWARD);
  Motor4.setSpeed(150); Motor4.run(FORWARD);
}

void stopRobot() {
  Motor1.setSpeed(0); Motor1.run(RELEASE);
  Motor2.setSpeed(0); Motor2.run(RELEASE);
  Motor3.setSpeed(0); Motor3.run(RELEASE);
  Motor4.setSpeed(0); Motor4.run(RELEASE);
}
