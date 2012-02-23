#include <Configuration.h>
#include <Debug.h>
#include <ParsedStream.h>
#include <SpiUart.h>
#include <WiFly.h>
#include <WiFlyClient.h>
#include <WiFlyDevice.h>
#include <WiFlyServer.h>
#include <_Spi.h>

/*
 * Web Server
 *
 * (Based on Ethernet's WebServer Example)
 *
 * A simple web server that shows the value of the analog input pins.
 */

#include "Credentials.h"


WiFlyServer server(80);

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  
  Serial.println("begin");
  WiFly.setUart(&Serial1);
  Serial.println("uart set");
  WiFly.begin();
  Serial.println("began");

  WiFly.join(ssid, passphrase);
  /*
  if (!WiFly.join(ssid, passphrase)) {
    while (1) {
      Serial.println("hung");
    }
  }
  */
  
  Serial.print("IP: ");
  Serial.println(WiFly.ip());
  
  server.begin();
}

void loop() {
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
  }
}
