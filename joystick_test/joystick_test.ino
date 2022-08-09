/*
Joystick:
Power connect: 5V + Gnd
VRx: A1
VRy: A2

Potentiometer:
DT: D9
CLK: D10

Esc_1: D4
Esc_2: D5

Power: 5V + Gnd
*/

#include<Servo.h>

#define ESC_PIN_FL 4 // front left
#define ESC_PIN_FR 5 // front right
#define ESC_PIN_BL 6 // back left
#define ESC_PIN_BR 7 // back right

#define outputA 9
#define outputB 10

float AccerlerationFactor = 0.05f;

int joystickCenterX;
int joystickCenterY;

int aLastState, aState, counter = 0;

Servo esc_FL;
Servo esc_FR;
Servo esc_BL;
Servo esc_BR;

int proofNewValue(int value)
{
  if (value < 0)
    return 0;
  if (value > 180)
    return 180;
  if (value > 0 && counter == 0) // idle protection
    return 0;
  return value;
}

void setup() 
{
  Serial.begin(115200);
  esc_FL.attach(ESC_PIN_FL, 1000, 2000);
  esc_FR.attach(ESC_PIN_FR, 1000, 2000);
  esc_BL.attach(ESC_PIN_BL, 1000, 2000);
  esc_BR.attach(ESC_PIN_BR, 1000, 2000);

  // Get center coords of joystick
  joystickCenterX = analogRead(A1);
  joystickCenterY = analogRead(A2);

  // setup potentiometer
  pinMode(outputA, INPUT);
  pinMode(outputB, INPUT);
  aLastState = digitalRead(outputA);
}
  
void loop() 
{
  // For X Axes
  int joystickValueX = analogRead(A1);
  int joystickValueY = analogRead(A2);
  int poti = digitalRead(A4);
  
  // limit values
  joystickValueX = constrain(joystickValueX, 0, joystickCenterX*2);
  joystickValueY = constrain(joystickValueY, 0, joystickCenterY*2);
  
  // Convert joystick value to writeable value
  joystickValueX = map(joystickValueX, joystickCenterX, joystickCenterX*2, 0, 180*AccerlerationFactor);
  joystickValueY = map(joystickValueY, joystickCenterY, joystickCenterY*2, 0, 180*AccerlerationFactor);
 
  // Debugging
  aState = digitalRead(outputA);
  if (aState != aLastState)
  {
    if (digitalRead(outputB) != aState)
      counter++;
    else
      counter--;
  }
  if (counter < 0)
    counter = 0;

  aLastState = aState;

  // Update values:
  int newEscValue_FL = counter-joystickValueX+joystickValueY;
  int newEscValue_FR = counter-joystickValueX-joystickValueY;
  int newEscValue_BL = counter+joystickValueX+joystickValueY;
  int newEscValue_BR = counter+joystickValueX-joystickValueY;

  newEscValue_FL = proofNewValue(newEscValue_FL);
  newEscValue_FR = proofNewValue(newEscValue_FR);
  newEscValue_BL = proofNewValue(newEscValue_BL);
  newEscValue_BR = proofNewValue(newEscValue_BR);

  // Write values to ESCs
  esc_FL.write(newEscValue_FL);
  esc_FR.write(newEscValue_FR);
  esc_BL.write(newEscValue_BL);
  esc_BR.write(newEscValue_BR);

  // Debugging
  Serial.print("FL: ");
  Serial.print(newEscValue_FL);
  Serial.print(" FR: ");
  Serial.print(newEscValue_FR);
  Serial.print(" BL: ");
  Serial.print(newEscValue_BL);
  Serial.print(" BR: ");
  Serial.println(newEscValue_BR);
}