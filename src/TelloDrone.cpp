#include <TelloDrone.h>
#include <WiFi.h>
#include <WiFiUdp.h>
#include <string.h>



        const char* networkName = "";
        const char* networkPswd = "";

        WiFiUDP udp;
        
        boolean connected;
        
        const char* udpAddress = "192.168.10.1";
        const int udpPort = 8889;

        TelloDrone::TelloDrone()
        {
        }

        void TelloDrone::connect(const char* ssid, const char* password)
        {
                // WiFi network name and password:
                this->networkName = ssid;
                this->networkPswd = password;
        
         WiFi.disconnect(true);
 
                IPAddress myIP(192,168,10,2);
                IPAddress gateway(192,168,10,1);
                IPAddress subnet(255,255,255,0); 
                
                Serial.print("Connection to: ");
                Serial.println(networkName);
                
                WiFi.disconnect();

                // Set eventlistener for wifi                
                WiFi.onEvent([this](WiFiEvent_t event, system_event_info_t info) {this->WiFiEvent(event, info); });


                // Configure wifi
                WiFi.config(myIP,gateway,subnet); 
                
                //Initiate connection
                // WiFi.begin(networkName, networkPswd);
                WiFi.begin(ssid, password);
                Serial.println("Waiting for WIFI connection");
                udp.begin(udpPort);
        }

        //Wifi event handler
        void TelloDrone::WiFiEvent(WiFiEvent_t event, system_event_info_t info){
                switch(event) {
                        case SYSTEM_EVENT_STA_GOT_IP:
                                //When connected set
                                Serial.print("WiFi connected! IP address: ");
                                Serial.println(WiFi.localIP());
                                //Initializes the UDP state
                                udp.begin(WiFi.localIP(),udpPort);
                                 
                                this->connected = true;
                                delay(1000);       
                                sendCommand("command");
                                delay(1000);
                               
                                break;
                        case SYSTEM_EVENT_STA_DISCONNECTED:
                                Serial.println("WiFi lost connection");
                                this->connected = false;
                                break;
                        case SYSTEM_EVENT_AP_STOP:
                                Serial.println("Lost connection to Drone");
                                this->connected = false;
                                break;
                }
        } 

        void TelloDrone::sendCommand(string message)  {
                string out = "Sending command '" + message + "'";
                Serial.println(out.c_str());
                //Send a packet
                udp.beginPacket(udpAddress,udpPort);
                udp.printf(message.c_str());
                udp.endPacket();
               
        }



