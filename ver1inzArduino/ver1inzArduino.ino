#define TIMEOUT 5000
#include <AFMotor.h>
#include <Servo.h>

Servo horizontal;
Servo vertical;

unsigned long time_now1 = 0;
unsigned long time_now2 = 0;

int period = 500;
int buzzer_period;
int buzzer_on_time;

int packetLength;

int anglehor;
int anglever;
int motorspeed;

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);
AF_DCMotor motor3(3);
AF_DCMotor motor4(4);


unsigned int EchoPin = 28;

unsigned int TrigPin = 29;

unsigned long Time_Echo_us = 0;

unsigned long Len_mm_X100 = 0;

unsigned long Len_Integer = 0; //

unsigned int Len_Fraction = 0;

void setup() {


  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial3.begin(115200);
  Serial3.setTimeout(100);


  pinMode(EchoPin, INPUT);
  pinMode(TrigPin, OUTPUT);

  pinMode(A3, OUTPUT);
  pinMode(A15, OUTPUT);
  pinMode(A8, OUTPUT);
  digitalWrite(A8, LOW);

  motor1.setSpeed(200);
  motor1.run(RELEASE);
  motor2.setSpeed(200);
  motor2.run(RELEASE);
  motor3.setSpeed(200);
  motor3.run(RELEASE);
  motor4.setSpeed(200);
  motor4.run(RELEASE);


  horizontal.attach(9);
  vertical.attach(10);
  horizontal.write(90);
  vertical.write(100);


  Serial.write("gotowe\n");

  //    SendCommand("AT+RST", "invalid");
  //    delay(3000);
  //    SendCommand("AT+CWMODE=2","OK");
  //    SendCommand("AT+CIFSR", "OK");
  //    SendCommand("AT+CIPMUX=1","OK");
  //    SendCommand("AT+CIPSERVER=1,80","OK");
  //    SendCommand("AT+CIPSTO=0","OK");
  digitalWrite(A15, HIGH);

}

void loop() {
  anglehor = horizontal.read();
  anglever = vertical.read();


  String IncomingString = "";
  boolean StringReady = false;
  while (Serial3.available()) {
    //Serial3.find(": /");
    //IncomingString=Serial3.readString();


    IncomingString = Serial3.readStringUntil('\n');
    IncomingString.trim();
    Serial.println(IncomingString.length());
    Serial.println(IncomingString);

    StringReady = true;



  }




  if (StringReady == true) {

    if (IncomingString.length() == 1) {

      Serial.println("Received String: " + IncomingString);
      //Serial.println(IncomingString);


      if (IncomingString == "f") {
        Forward();
      }

      if (IncomingString == "s") {
        Release();
      }

      if (IncomingString == "d") {
        Backward();
      }

      if (IncomingString == "r") {
        TurnRight();
      }

      if (IncomingString == "l") {
        TurnLeft();
      }



    } else if (IncomingString.length() > 1) {
      char abc = IncomingString[0];

      String ang = "";
      
      for (int i = 1; i < IncomingString.length(); i++) {
        ang += (char)IncomingString[i];
      }
      Serial.println(ang);
      if (abc == 'h') {
        anglehor = ang.toInt();
        anglehor = 90 - anglehor;
        horizontal.write(anglehor);
      } else if (abc == 'v') {
        anglever = ang.toInt();
        anglever = 90 - anglever;
        vertical.write(anglever);
      } else if (abc=='s'){
        motorspeed = ang.toInt();
        motor1.setSpeed(motorspeed);
        motor1.run(RELEASE);
        motor2.setSpeed(motorspeed);
        motor2.run(RELEASE);
        motor3.setSpeed(motorspeed);
        motor3.run(RELEASE);
        motor4.setSpeed(motorspeed);
        motor4.run(RELEASE);
      }
    }
    //Serial.println(abc);
  }
  



  if (millis() > time_now1 + period) {
    time_now1 = millis();

    digitalWrite(TrigPin, HIGH);

    delayMicroseconds(50);

    digitalWrite(TrigPin, LOW);

    Time_Echo_us = pulseIn(EchoPin, HIGH);

    if ((Time_Echo_us < 60000) && (Time_Echo_us > 1))

    {

      Len_mm_X100 = (Time_Echo_us * 34) / 2;

      Len_Integer = Len_mm_X100 / 100;

      Len_Fraction = Len_mm_X100 % 100;

      Serial.print("Present Length is: ");

      Serial.print(Len_Integer, DEC);

      Serial.print(".");

      if (Len_Fraction < 10)

        Serial.print("0");

      Serial.print(Len_Fraction, DEC);

      Serial.println("mm");

      //delay(1000);

    }
  }
  if (Len_Integer < 500) {
    buzzer_period = Len_Integer;
    buzzer_on_time = Len_Integer / 2;
    //Serial.print(buzzer_on_time);
    if (millis() > time_now2 + buzzer_period) {
      time_now2 = millis();
      Serial.print(buzzer_period);
      tone(A3, 1000, buzzer_on_time);
    }

  } else {
    noTone(A3);
  }

}

boolean SendCommand(String cmd, String ack) {
  Serial3.println(cmd); // Send "AT+" command to module
  if (!echoFind(ack)) // timed out waiting for ack string
    return true; // ack blank or ack found
}

boolean echoFind(String keyword) {
  byte current_char = 0;
  byte keyword_length = keyword.length();
  long deadline = millis() + TIMEOUT;
  while (millis() < deadline) {
    if (Serial3.available()) {
      char ch = Serial3.read();
      Serial.write(ch);
      if (ch == keyword[current_char])
        if (++current_char == keyword_length) {
          Serial.println();
          return true;
        }
    }
  }
  return false; // Timed out
}
void Forward() {
  motor1.run(BACKWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(BACKWARD);
}

void Backward() {
  motor1.run(FORWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(FORWARD);
}

void Release() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
  motor3.run(RELEASE);
  motor4.run(RELEASE);
}

void TurnRight() {
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
  motor3.run(FORWARD);
  motor4.run(BACKWARD);
}

void TurnLeft() {
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
  motor3.run(BACKWARD);
  motor4.run(FORWARD);

}
