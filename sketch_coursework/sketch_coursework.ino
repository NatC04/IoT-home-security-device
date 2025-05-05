#include <Keypad.h>

#define echoPin 11 // Attach pin 11 Arduino to pin Echo of Ultrasonic sensor
#define trigPin 12 // Attach pin 12 Arduino to pin Trig of Ultrasonic sensor

// Define variables
const int buzzer = 13; // Buzzer to ardunio pin 13
long duration; // Variable for the duration of sound wave travel
int distance; // Variable for the distance measurement
unsigned long startTime = 0;
unsigned long onOffStartTime = 0;
bool timerStarted = false;
bool onOffTimerStarted = false;
String enteredPassword = "";
const String correctPassword = "1234";  // Change this as needed
bool accessGranted = false;
bool onOffAccessGranted = false;
float baselineDistance = 0;
bool baselineSet = false;
bool currentDistanceSet = false;
const int button = 2;
bool deviceOn = false;
bool onOffPass = false;

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
//define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

byte colPins[COLS] = {7, 8, 9, 10}; //connect to the column pinouts of the keypad
byte rowPins[ROWS] = {3, 4, 5, 6}; //connect to the row pinouts of the keypad

//initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

void setup() {
  pinMode(button, INPUT_PULLUP);
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzer, OUTPUT);
  
  // Take baseline reading
  delay(1000);
  baselineDistance = getDistance();
  baselineSet = true;

  Serial.print("Baseline distance set to: ");
  Serial.print(baselineDistance);
  Serial.println(" cm");
}


void loop() {
  int buttonState = digitalRead(button);
  // Sets buzzer to off state
  noTone(buzzer);
  if (buttonState == 0) {
    delay(200);  // Delay to stop multiple presses
    if (digitalRead(button) == 0) {  // Confirm it's still pressed
      Serial.println("Enter Password.");
      onOffStartTime = millis();
      onOffTimerStarted = true;
      onOffPass = true;
      onOffAccessGranted = false;
      // Wait for button release to avoid double toggle
      while (digitalRead(button) == 0) {
        delay(10);
      }
    }
  }

  // Password input while timer is running
  if (onOffTimerStarted && onOffPass) {
    char key = customKeypad.getKey();
    if (key) {
      if (key == '*') {
        enteredPassword = "";
        Serial.println("Password cleared");
      } else if (key == '#') {
        if (enteredPassword == correctPassword) {
          Serial.println("Correct Password");
          deviceOn = !deviceOn;  // Activate system
          Serial.println(deviceOn ? "System activated" : "System deactivated");
          onOffAccessGranted = true;
          enteredPassword = "";
          onOffPass = false;
          Serial.println("Resetting...");
          delay(10000);
          onOffTimerStarted = false;
          setup();  // Reset baseline
        } else {
          Serial.println("Incorrect Password");
          enteredPassword = "";
        }
      } else {
        enteredPassword += key;
        Serial.print("Entered so far: ");
        Serial.println(enteredPassword);
      }
    }
    // Time expired
    if ((millis() - onOffStartTime > 30000) && !onOffAccessGranted) {
      Serial.println("Time expired. Sounding buzzer.");
      tone(buzzer, 1000);
      delay(3000);
      noTone(buzzer);
      onOffTimerStarted = false;
      enteredPassword = "";
      onOffPass = false;
      setup();  // Reset baseline
    }
  }

  if(deviceOn) {
    // Gets the current distance from the ultrasonic sensor
    float currentDistance = getDistance();
    // Displays the distance on the Serial Monitor for testing
    if (!timerStarted) {
      Serial.print("Distance: ");
      Serial.print(distance);
      Serial.println(" cm");
    }
    // Ensures everything is running correctly
    if (!timerStarted && baselineSet && currentDistance > 0) {
      // Activates when the distance changes from the baseline by too much
      if (abs(currentDistance - baselineDistance) > 10) {
        Serial.println("TAKE_PICTURE");
        Serial.println("Motion detected - starting timer.");
        startTime = millis();
        timerStarted = true;
        accessGranted = false;
        enteredPassword = "";
      }
    }
    
    // Password input while timer is running
    if (timerStarted) {
      char key = customKeypad.getKey();
      if (key) {
        if (key == '*') {
          enteredPassword = "";
          Serial.println("Password cleared");
        } else if (key == '#') {
          if (enteredPassword == correctPassword) {
            Serial.println("Correct Password");
            accessGranted = true;
            enteredPassword = "";
            Serial.println("Resetting...");
            delay(10000);
            timerStarted = false;
            setup();  // Reset baseline
          } else {
            Serial.println("Incorrect Password");
            enteredPassword = "";
          }
        } else {
          enteredPassword += key;
          Serial.print("Entered so far: ");
          Serial.println(enteredPassword);
        }
      }
      // Time expired
      if ((millis() - startTime > 30000) && !accessGranted) {
        Serial.println("Time expired. Sounding buzzer.");
        tone(buzzer, 1000);
        delay(3000);
        noTone(buzzer);
        timerStarted = false;
        enteredPassword = "";
        setup();  // Reset baseline
      }
    }
    delay(100);
  }
}

  
float getDistance() {
  // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); // Sets the trigPin HIGH for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH);// Calculating the distance
  return distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (there and back)
}