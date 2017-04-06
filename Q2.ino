#include <Servo.h>

int tempPin = 0;
int servoPin = 3;
int pos = 0;
float temperature = 0;
float temp_v = 0.f;

Servo myservo;

void setup() {
  // put your setup code here, to run once:
  myservo.attach(servoPin);
  pinMode(tempPin, INPUT);
  Serial.begin(9600);
  myservo.write(0);
}

void loop()
{
  temp_v = (analogRead(tempPin) * 5.f) / 1024.f;
  temperature = ((temp_v - 0.5f)*100.f);
  pos = map(temperature, 10, 40, 0, 180);
  // room temp varied beteen 10 - 40 deg C, so the temp scale was chosen to
  // show this range to make the servo movement more apparent
  Serial.print("Temperature (C): ");
  Serial.print(temperature);
  Serial.print("\n");
  Serial.print("Servo Position: ");
  Serial.print(pos);
  Serial.print("\n");
  Serial.print("\n");
  myservo.write(pos);
  delay(500);
}
