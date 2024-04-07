#include <QMC5883LCompass.h>
#include <Servo.h>

QMC5883LCompass compass;

float average;
const float strength = 0.93; 

const int BUTTON_PIN = A0;
const int LED_PIN = D4; 
bool turnRight = false;
bool shouldTurn = false;
int loopTime = 0;
int progTime = 0;
int timeOnDoorChange = 0;
int i = 0;
int x, y, z, a, b;
char myArray[3];
int buttonValue;
int timeOnStopTurn = 0;
bool beenInStop = false;

Servo myservo;

void setup() {
  Serial.begin(115200);
  compass.init();
  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  myservo.attach(D5); // attach the servo to our servo object
  myservo.write(90); // stop the motor

  average = 0.0;
  for( int i=0; i<10; i++)
  {
    compass.read();
    average += compass.getZ();;
    delay(10);
  }
  average /= 10.0;
}

void loop() {
  progTime = millis();

  if ((progTime - loopTime) > 5){
    
    loopTime = millis();
    
    compass.read();
    
    x = compass.getX();
    y = compass.getY();
    z = compass.getZ();
    
    a = compass.getAzimuth();
  
    //Serial.print("X: ");
    //Serial.print(x);
  
    //Serial.print(" Y: ");
    //Serial.print(y);
  
    //Serial.print(" Z: ");
    //Serial.print(z);
  
    //Serial.print(" Azimuth: ");
    //Serial.print(a);
  
    //Serial.print(myArray[0]);
    //Serial.print(myArray[1]);
    //Serial.print(myArray[2]);
  
    //Serial.println();

    float newValue = z;
    average = (strength * average) + ((1.0-strength) * newValue);
    float result = newValue - average;
  
    //Serial.print(" Z: ");
    //Serial.print(z);
  
    //Serial.println();
  
    Serial.println(average);

    Serial.print(" Result: ");
    Serial.print(result);
    Serial.println();

    buttonValue = analogRead(BUTTON_PIN);

    if (buttonValue > 1010 && turnRight == false){
      if ((progTime - timeOnDoorChange) > 5000) {
        Serial.println("Locking door");
        turnRight = true;
        shouldTurn = true;
        digitalWrite(LED_PIN, HIGH);
        timeOnDoorChange = millis();
      }
    }
    else if (buttonValue > 1010 && turnRight == true){
      if ((progTime - timeOnDoorChange) > 5000) {
        Serial.println("Unlocking door");
        turnRight = false;
        shouldTurn = true;
        digitalWrite(LED_PIN, LOW);
        timeOnDoorChange = millis();
      }
    }


    if (shouldTurn == true && turnRight == true){
      myservo.write(43);
    }
  
      
    else if (shouldTurn == true && turnRight == false){
      myservo.write(110);
    }
  
    if (abs(result) <= 5) {
      timeOnStopTurn = millis();
      if ((timeOnStopTurn - timeOnDoorChange) > 2000) {
        if (beenInStop == true){
          Serial.println("shouldTurn False");
          Serial.println();
          Serial.print("stop turning triggered at: ");
          Serial.print(timeOnStopTurn - timeOnDoorChange);
          myservo.write(90);
          shouldTurn = false;
        }
        else{
          delay(50);
          beenInStop = true;
        }
        
      }
    }
    else {
      beenInStop = false;
    }
  
    if (z >=2000) {
      Serial.println("Door open");
    }
    else {
      Serial.println("Door closed");
    }
  }
  yield();
}


    /*
  
  
    if (z >=2000) {
      Serial.println("Door open");
    }
    else {
      Serial.println("Door closed");
    }
    loopTime = millis();
  }

  //Här ska göras när result är större / mindre än +-50, måste fortsätta läsa in med rätt intervall under tiden
  
  if(analogRead(BUTTON_PIN) > 1010 && turnRight == false){
    if ((progTime - timeOnDoorChange) > 5000) {
      Serial.println("Locking door");
      i = 0;
      while (i < 40) {
        myservo.write(2);
        delay(50);
        i++;
        yield();
      }
      turnRight = true;
      digitalWrite(LED_PIN, HIGH);
      timeOnDoorChange = millis();
    }
  }
  else if (analogRead(BUTTON_PIN) > 1010 && turnRight == true){
    if ((progTime - timeOnDoorChange) > 5000) {
      Serial.println("Unlocking door");
      i = 0;
      while (i < 40) {
        myservo.write(110);
        delay(50);
        i++;
        yield();
      }
      turnRight = false;
      digitalWrite(LED_PIN, LOW);
      timeOnDoorChange = millis();
    }
  }
}

*/