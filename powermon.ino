

/***************************************************************************
* Example sketch for the ADS1115_WE library
* https://github.com/wollewald/ADS1115_WE
* https://wolles-elektronikkiste.de/ads1115 (German)
* https://wolles-elektronikkiste.de/en/ads1115-a-d-converter-with-amplifier (English)
***************************************************************************/

#include<ADS1115_WE.h> 
#include<Wire.h>
#define I2C_ADDRESS 0x48


//per spiffs_vars
#include "FS.h"
#include "SPIFFS.h"
#define FORMAT_SPIFFS_IF_FAILED true

//per json
#include <ArduinoJson.h>
JsonDocument doc;

//per wifi e OTA
#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiManager.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <base64.h>

String jsonFileVarName = "jfv";

const char* deviceName = "ESP32_Power_Mon";

ADS1115_WE adc = ADS1115_WE(I2C_ADDRESS);
ADS1115_MUX ch[]={ADS1115_COMP_0_GND,ADS1115_COMP_1_GND,ADS1115_COMP_2_GND,ADS1115_COMP_3_GND};
ADS1115_MUX ch_actual=ch[0];

unsigned long last_second=0;

extern float avg_long[4];
extern float vrms[4]; //prometto di definire questa variabile in altro file
extern float f_sampling[4];


/////////////////////////////////////////
bool in_calibration[4]={false,false,false,false};             ///
bool erase_config=true;              /// <--- ho inserito i dati di calibrazione direttamente nel codice
/////////////////////////////////////////

#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h> //https://github.com/me-no-dev/ESPAsyncWebServer/tree/master
AsyncWebServer server(80);
bool reboot=false;

void setup() {
  Serial.begin(115200);
  // Inizializza WiFiManager
  WiFiManager wifiManager;
  
  // Imposta il timeout per la configurazione del WiFi
  wifiManager.setConfigPortalTimeout(180);
  
  // Tenta di connettere al WiFi salvato, se fallisce avvia il portale di configurazione
  if (!wifiManager.autoConnect(deviceName)) {
    Serial.println("Failed to connect and hit timeout");
    delay(3000);
    ESP.restart();
  }

  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP().toString());


  Wire.begin();
  
  if(!adc.init()){
    Serial.println("ADS1115 not connected!");
  }

  adc.setVoltageRange_mV(ADS1115_RANGE_6144); //comment line/change parameter to change range
  //adc.setCompareChannels(ADS1115_COMP_0_GND); //comment line/change parameter to change channel
  adc.setMeasureMode(ADS1115_CONTINUOUS); //comment line/change parameter to change mode

  spiffs_vars_begin();
  json_file_setup(erase_config);//true =erase calibration

  setupOTA();

  campiona_setup();

  wserver_setup();

}

void loop() {
  ArduinoOTA.handle();
  campiona_all();
  respond_serial();

  if ((millis()/1000) > last_second+9){
    last_second=millis()/1000;
    for (int i=0;i<4;i++){
      if (in_calibration[i]){
        float p=get_live_data("Pgrid");
        calibration_run(vrms[i],p, i);
        String  s  = "loop(): ";
        s += "f_sampling[i]="+String(f_sampling[i],4)+"\t";
        s += "vrms[i]="+String(vrms[i],4)+"\t";
        s += "p_mis="+String(p,5)+"\t";
        s += "p_calc[i]="+String(getPowerFromCalibration(vrms[i], i) ,4)+"\t";
        Serial.println(s);
      }
    }
    
    
  }
  //json_file_update(false); spostata nel loop sopra
  //delay(10000);
}


void respond_serial() {
  if (Serial.available() > 0) {
    while(Serial.available()) {
      Serial.read();
    }
    Serial.println(getFileVarString(jsonFileVarName));
    Serial.print("f_sampling[]="); for (int i=0;i<4;i++) Serial.print(String(f_sampling[i],0)+"\t");
    Serial.print("avg_long[]="); for (int i=0;i<4;i++) Serial.print(String(avg_long[i],3)+"\t");
    Serial.print("\r\n");
  }
}


