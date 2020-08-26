#include <Servo.h>
Servo esc_signal;

#define Arduino_Pin 12
#define ESP8266_Pin 14 // Pin D5

void setup()
{
  esc_signal.attach(ESP8266_Pin);  //Specify here the pin number on which the signal pin of ESC is connected.
  esc_signal.write(30);   //ESC arm command. ESCs won't start unless input speed is less during initialization.
  delay(3000);            //ESC initialization delay.
}

void loop()
{
esc_signal.write(71);    //Vary this between 40-130 to change the speed of motor. Higher value, higher speed.
delay(15);
}
