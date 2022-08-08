#include<Servo.h>

#define ESC_PIN 4

Servo esc;

void setup() 
{
  Serial.begin(115200);
  esc.attach(ESC_PIN,  1000, 2000);
}

void loop() 
{
  int joystickValue = analogRead(A1);
  joystickValue = map(joystickValue, 550, 1023, 0, 180);
  if (joystickValue < 0)
    joystickValue = 0;
  Serial.println(joystickValue);
  esc.write(joystickValue);
  //delay(1000);
}
