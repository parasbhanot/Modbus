#include <SoftwareSerial.h>

#include <UIPEthernet.h>
#include <utility/logging.h>
#include <SPI.h>

unsigned char reader[12]; 

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF
};

IPAddress ip(192, 168, 1,40);
EthernetServer server(80);

void setup() {

   Serial.begin(9600);
 
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

        //unsigned char reader[12];
        //client.readBytes(reader,12);
        
        //Serial.println(reader[0]);  

        if(client.available()){

            client.readBytes(reader,12);

            for(int i = 0;i<12;i++){

               Serial.print(reader[i],HEX); 
               Serial.print(" ");  
                        
            }

            
              client.write(reader[0]); // tid 
              client.write(reader[1]);
      
              client.write((byte)0x00);  // pid 
              client.write((byte)0x00);
      
              client.write((byte)0x00); // message length
              client.write(0x08);
      
              client.write(0x11); //did 
      
               ///---------------------------
      
               client.write(0x01); // F_code
               client.write(0x05); //N_bytes
      
              client.write(0xCD); // data
              client.write(0x6B);
              client.write(0xB2);
              client.write(0x0E);
              client.write(0x1B);

        }
              
    }

    client.stop();
    Serial.println("client disconnected");    
  }

}
