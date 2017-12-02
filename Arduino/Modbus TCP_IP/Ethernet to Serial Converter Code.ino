#include <SoftwareSerial.h>

#include <UIPEthernet.h>
#include <utility/logging.h>
#include <SPI.h>

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF
};

IPAddress ip(192, 168, 1,40);
EthernetServer server(80);

void setup() {

   Serial.begin(9600);
   Serial1.begin(9600);
   Serial2.begin(9600);
   Serial3.begin(9600);
 
   Ethernet.begin(mac,ip);
   server.begin();
   Serial.print("server is at ");
   Serial.println(Ethernet.localIP());
   
}

void loop() {
  
   EthernetClient client = server.available();

    if (client) {
    Serial.println("new client");

    while (client.connected()) {

      if (client.available()) {
        
        char c = client.read();
        Serial1.write(c);
        Serial2.write(c);
        Serial3.write(c);
   
      }

         if(Serial3.available()){
          
           //working  3 bytes or termiate 
            char reader[30] ={0};
            Serial3.readBytesUntil('\n',reader,30);
            client.println(reader);
            Serial.println(reader);
        }


         if(Serial2.available()){
          
           //working  3 bytes or termiate 
            char reader[30] ={0};
            Serial2.readBytesUntil('\n',reader,30);
            client.println(reader);
            Serial.println(reader);
        }

        
         if(Serial1.available()){
          
           //working  3 bytes or termiate 
            char reader[30] ={0};
            Serial1.readBytesUntil('\n',reader,30);
            client.println(reader);
            Serial.println(reader);
        }
      
    }

    client.stop();
    Serial.println("client disconnected");    
  }

}



