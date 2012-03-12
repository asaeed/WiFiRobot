
#include "WiFly.h"
#include "MotorDriver.h"

#define motorAPWM 11
#define motorAIN1 12
#define motorAIN2 13
#define motorBPWM 10
#define motorBIN1 9
#define motorBIN2 8

MotorDriver leftMotor = MotorDriver(motorAIN1, motorAIN2, motorAPWM);
MotorDriver rightMotor = MotorDriver(motorBIN1, motorBIN2, motorBPWM);

int inByte= -1;                   // incoming byte from serial RX
const int inStringLength = 50;    // max length is 19 and then 1 for the termination 0?
char inString[inStringLength];    // string for incoming serial data
int stringPos = 0;                // string index counter

WiFlyServer server(80);

void setup()
{
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.println("setup begin");
  
  WiFly.setUart(&Serial1);
  Serial.println("uart set");
  WiFly.begin();
   
  //WiFly.join("SecondFloor", "firstfloor99");
  
  Serial.print("IP: ");
  //Serial.println(WiFly.ip());
  
  server.begin();
  
  Serial.println("setup end");
}

void loop()
{
  /*
  for (int i=-20; i<20; i++) {
    leftMotor.setSpeed(i);
    rightMotor.setSpeed(i);
    delay(20);
  }
  
  leftMotor.setSpeed(0);
  rightMotor.setSpeed(0);
  delay(1000);
  */
  
  //handleSerial();
  
  
  while(Serial1.available() > 0) {
    Serial.write(Serial1.read());
  }
  
  while(Serial.available()) { // Outgoing data
    Serial1.write(Serial.read());
  }
  
}


void handleSerial(){
  WiFlyClient client = server.available();
  if (client) {
    // an http request ends with a blank line
    boolean current_line_is_blank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        // if we've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so we can send a reply
        if (c == '\n' && current_line_is_blank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          
          // output the value of each analog input pin
          for (int i = 0; i < 6; i++) {
            client.print("analog input ");
            client.print(i);
            client.print(" is ");
            client.print(analogRead(i));
            client.println("<br />");
          }
          break;
        }
        if (c == '\n') {
          // we're starting a new line
          current_line_is_blank = true;
        } else if (c != '\r') {
          // we've gotten a character on the current line
          current_line_is_blank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(100);
    client.stop();
    client.stop();
  }
}
