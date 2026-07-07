#include <Servo.h>

Servo tracker;
const int LDR_PIN = A0;
const int PIR_L = 2, PIR_C = 3, PIR_R = 4;
const int TRIG = 10, ECHO = 11, LED = 12;

int lastDistance = 999; 
int nightThreshold = 500; // Adjust slider in Tinkercad to test

void setup() {
  tracker.attach(9);
  pinMode(LED, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  Serial.begin(9600);
}

void loop() {
  int lightValue = analogRead(LDR_PIN);
  
  // SIMULATION TIP: Slide the LDR value to 'Dark' in Tinkercad
  if (lightValue < nightThreshold) {
    bool motion = false;

    // 1. Tracking Logic
    if (digitalRead(PIR_L)) { tracker.write(45); motion = true; }
    else if (digitalRead(PIR_C)) { tracker.write(90); motion = true; }
    else if (digitalRead(PIR_R)) { tracker.write(135); motion = true; }

    if (motion) {
      delay(500); // Wait for servo to move
      int dist = getDistance();
      
      // 2. Proximity Warning Logic (Moving Closer)
      // Trigger if current distance is significantly less than the last check
      if (dist < lastDistance - 15 && dist < 200) { 
        flashLED();
      }
      lastDistance = dist;
    }
  } else {
    // It's "Day" - System Standby
    digitalWrite(LED, LOW);
  }
  delay(1000);
}

int getDistance() {
  digitalWrite(TRIG, LOW); delayMicroseconds(2);
  digitalWrite(TRIG, HIGH); delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  return pulseIn(ECHO, HIGH) * 0.034 / 2;
}

void flashLED() {
  for(int i=0; i<3; i++) {
    digitalWrite(LED, HIGH); delay(100);
    digitalWrite(LED, LOW); delay(100);
  }
}