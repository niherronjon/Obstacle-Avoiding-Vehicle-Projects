//Obstacle Avoiding robot
//Author : Niher Ronjon Pramanik

#include <Servo.h>
​
// Ultrasonic Sensor Pins
const int trigPin = 9;
const int echoPin = 10;
​
// Motor Pins
const int motor1Pin1 = 3;
const int motor1Pin2 = 4;
const int motor2Pin1 = 5;
const int motor2Pin2 = 6;
​
// Servo Pin
Servo myServo;
​
// Buzzer Pin
const int buzzerPin = 12;
​
// Distance Threshold
const int distanceThreshold = 15; // in cm
​
void setup() {
  // Motor Pins
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
​
  // Ultrasonic Sensor Pins
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
​
  // Buzzer Pin
  pinMode(buzzerPin, OUTPUT);
​
  // Servo Setup
  myServo.attach(11);
  myServo.write(90); // Start at center position
​
  Serial.begin(9600); // For debugging
}
​
long readDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
​
  long duration = pulseIn(echoPin, HIGH);
  return duration * 0.034 / 2; // Convert to cm
}
​
void moveForward() {
  analogWrite(motor1Pin1, 255); // Full speed
  digitalWrite(motor1Pin2, LOW);
  analogWrite(motor2Pin1, 255); // Full speed
  digitalWrite(motor2Pin2, LOW);
}
​
void stopMotors() {
  digitalWrite(motor1Pin1, LOW);
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  digitalWrite(motor2Pin2, LOW);
}
​
void turnLeft() {
  analogWrite(motor1Pin1, 128); // 50% speed
  digitalWrite(motor1Pin2, LOW);
  digitalWrite(motor2Pin1, LOW);
  analogWrite(motor2Pin2, 128); // 50% speed
}
​
void turnRight() {
  digitalWrite(motor1Pin1, LOW);
  analogWrite(motor1Pin2, 128); // 50% speed
  analogWrite(motor2Pin1, 128); // 50% speed
  digitalWrite(motor2Pin2, LOW);
}
​
void loop() {
  long distance = readDistance();
  Serial.println(distance);
​
  if (distance < distanceThreshold) { // Obstacle detected
    stopMotors();
    tone(buzzerPin, 1000); // Turn on the buzzer with a 1 kHz tone
    delay(500); // Pause briefly
​
    // Scan left and right for a clear path
    myServo.write(60); // Look left
    delay(500);
    long leftDistance = readDistance();
​
    myServo.write(120); // Look right
    delay(500);
    long rightDistance = readDistance();
​
    myServo.write(90); // Return to center
    delay(500);
​
    // Decide turn direction
    if (leftDistance > rightDistance) {
      turnLeft();
    } else {
      turnRight();
    }
    delay(400); // Reduce turn duration
    stopMotors(); // Stop after turning
  } else {
    noTone(buzzerPin); // Turn off the buzzer
    moveForward(); // No obstacle, move forward
  }
​
  delay(100); // Small delay to stabilize sensor readings
}
