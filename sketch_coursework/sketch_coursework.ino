#define echoPin 4 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 5 //attach pin D3 Arduino to pin Trig of HC-SR04

// defines variables
const int buzzer = 9; // buzzer to ardunio pin 9
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600); // Serial Communication is starting with 9600 of baudrate speed
  pinMode(buzzer, OUTPUT); // Set buzzer pin 9 as an output

}
void loop() {
  noTone(buzzer);
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); // Sets the trigPin HIGH for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);// Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (there and back)
  // Displays the distance on the Serial Monitor - Not required in final product
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  if (distance < 50) {
    //tone(buzzer, 1000);
    delay(5000);
    Serial.println("TAKE_PICTURE");
    delay(10000);
  }
  delay(100);
}
