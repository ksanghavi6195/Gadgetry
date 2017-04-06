// Initialize and Define DC Motor variables
#define encoder0PinA 2
#define encoder0PinB 3

// Variables for reading the encoder's position
volatile unsigned int encoderPos_f = 0;
volatile unsigned int encoderPos_f_old = 0;

//Initialize and Define Stepper Motor
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"


// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Initialize the stepper motor with 200 steps per rev
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);


void setup() {

  pinMode(encoder0PinA, INPUT); 
  pinMode(encoder0PinB, INPUT); 
  
// encoder pin on interrupt 0 (pin 2)
  attachInterrupt(0, doEncoderA, CHANGE);
  
// encoder pin on interrupt 1 (pin 3)
  attachInterrupt(1, doEncoderB, CHANGE);  

  Serial.begin (9600);
  AFMS.begin();
// Set the rotational speed of the stepper motor
  myMotor->setSpeed(20);
}

void loop() {
  Serial.print("Encoder Position: ");
  Serial.println(encoderPos_f);
  Serial.print("Encoder Position Old: ");
  Serial.println(encoderPos_f_old);

  if (encoderPos_f != encoderPos_f_old) {
    // Find the absolute value of the difference between the encoder's new and old positions
    int difference = encoderPos_f - encoderPos_f_old;
    difference = abs(difference);
    
    // Get the proportional number of steps needed to match the encoder
    // DC motor Rev = 465, Stepper motor Rev = 200
    int stepsToTake = map(difference, 0, 465, 0, 200);
    
    // If the encoder value increases
    if (encoderPos_f > encoderPos_f_old) {
      // move stepper CW at a rate that's proportional to the DC motor     
      // Tell the stepper motor to move that many steps
      myMotor->step(stepsToTake, FORWARD, SINGLE);

    }

    else if (encoderPos_f < encoderPos_f_old) {
      // move stepper CCW at rate that's proportional to the DC motor
      myMotor->step(stepsToTake, BACKWARD, SINGLE);
    }
  }
  
  // Reset the old value of the encoder
  encoderPos_f_old = encoderPos_f;
  
  // Allows time for stepper motor to rotate
  delay(1000);
}


// Code for Encoder in DC Motor
void doEncoderA(){
  // look for a low-to-high on channel A
  if (digitalRead(encoder0PinA) == HIGH) { 
    // check channel B to see which way encoder is turning
    if (digitalRead(encoder0PinB) == LOW) {  
      
        encoderPos_f = encoderPos_f + 1;         // CW Rotation
    } 
    else {
        encoderPos_f = encoderPos_f - 1;        // CCW Rotation
    }
  }
  else   // must be a high-to-low edge on channel A                                       
  { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(encoder0PinB) == HIGH) {   
      
        encoderPos_f = encoderPos_f + 1;          // CW Rotation
    } 
    else {
        encoderPos_f = encoderPos_f - 1;          // CCW Rotation
    }
  }
}

void doEncoderB(){
  // look for a low-to-high on channel B
  if (digitalRead(encoder0PinB) == HIGH) {   
   // check channel A to see which way encoder is turning
    if (digitalRead(encoder0PinA) == HIGH) {  
       
        encoderPos_f = encoderPos_f + 1;         // CW Rotation
    } 
    else {
        encoderPos_f = encoderPos_f - 1;         // CCW Rotation
    }
  }
  // Look for a high-to-low on channel B
  else { 
    // check channel B to see which way encoder is turning  
    if (digitalRead(encoder0PinA) == LOW) {   
        encoderPos_f = encoderPos_f + 1;          // CW Rotation
    } 
    else {
        encoderPos_f = encoderPos_f - 1;         // CCW Rotation
    }
  }
}
