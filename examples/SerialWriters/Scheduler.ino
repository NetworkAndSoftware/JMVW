/*
  SerialWriters.ino

  This is a demo of the MultiLoop library. It sets up three different independent loops.

  Open the serial port monitor while you have this running and make sure your serial port 
  is set to 115200 baud. You'll see that the text Hello is printed at a rate different from 
  the text World. Meanwhile the LED is blinking.
*/

#include <arduino.h>
#include <MultiLoop.h>

void setup()
{
  Serial.begin(115200);
delay(1000);
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // Add three different loops. The maximum amount of loops is 8 by default.
  MultiLoop::AddLoop(loopBlink);
  MultiLoop::AddLoop(loopHello);
  MultiLoop::AddLoop(loopWorld);
}

void loopBlink()
{
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  MultiLoop::delay(250);             // wait for a second. 
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  MultiLoop::delay(250);              
}

void loopHello()
{
  Serial.println("Hello");
  MultiLoop::delay(500);            // Meanwhile shorter other loops can run.
}

void loopWorld()
{
  Serial.println("World!");
  MultiLoop::delay(5000);
}
