
#include "WiFly.h"
#include "Credentials.h"

WiFlyClient client("192.168.1.101", 800);

void setup() {
  
  Serial.begin(9600);
  Serial1.begin(9600);
  
  WiFly.setUart(&Serial1);
  WiFly.begin();
  
  Serial1.println("set comm idle 2");
  WiFly.join(ssid, passphrase);
  
}

void loop() {
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }
  
  if (!client.connected()) {
    Serial.println();
    Serial.println("disconnected.");
    //client.stop();
    //delay(2000);
    getData();
  }
}

void getData()
{
  Serial.println("connecting...");

  if (client.connect()) {
    Serial.println("connected");
    client.println("GET /IBM.UnicaEmailGenerator/index.php");
    client.println();
    //delay(100);
  } else {
    Serial.println("connection failed");
  }
   
}

