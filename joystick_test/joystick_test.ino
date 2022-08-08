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

#define ESC_PIN_1 4

#define outputA 9
#define outputB 10

float AccerlerationFactor = 0.1f;

int joystickCenterX;
int joystickCenterY;

int aLastState, aState, counter = 0;

Servo esc_1;

void setup() 
{
  Serial.begin(115200);
  esc_1.attach(ESC_PIN_1, 1000, 2000);

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

  // Create new value based on joystick value and last value
  int new_esc_1_value = counter+joystickValueX;

  if (new_esc_1_value < 0)
    new_esc_1_value = 0;
  if (new_esc_1_value > 180)
    new_esc_1_value = 180;
  if (joystickValueX > 0 && counter == 0)
    new_esc_1_value = 0;

  // Update Value:
  esc_1.write(new_esc_1_value);

  Serial.print("MotorSpeed: ");
  Serial.println(new_esc_1_value);

  delay(10);

}