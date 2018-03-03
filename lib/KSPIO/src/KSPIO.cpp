
#include "KSPIO.h"

// flash leds
void KSPIO::initLEDS() {
    pinMode(GLED, OUTPUT);
    digitalWrite(GLED, HIGH);
    delay(1000);
}

// darken all leds
void KSPIO::zeroIndicators() {
    digitalWrite(GLED, LOW);
}

void KSPIO::initPackets() {
    HPacket.id = 0;  
    CPacket.id = 101;
}

void KSPIO::initControls() {
    pinMode(4, INPUT_PULLUP);
}

// input is FROM KSP...
int KSPIO::input() {
    int returnValue = -1;

    now = millis();

    if (ReceiveData()){
        deadtimeOld = now;
        returnValue = id;
        switch(id) {
            case 0: //Handshake packet
                KSPIO::Handshake();
                break;
            case 1:
                indicators();
                break;
        }

        //We got some data, turn the green led on
        digitalWrite(GLED,HIGH);
        Connected = true;
    } else {
        deadtime = now - deadtimeOld; 
        if (deadtime > IDLETIMER)
        {
        deadtimeOld = now;
        Connected = false;
        zeroIndicators();
        }    
    }

    return returnValue;
}

void KSPIO::output() {

}


void KSPIO::indicators() {

  caution = 0;
  warning = 0;

  caution += VData.G > GCAUTION;
  warning += VData.G > GWARN;
  caution += VData.LiquidFuelS/VData.LiquidFuelTotS*100 < FUELCAUTION;
  warning += VData.LiquidFuelS/VData.LiquidFuelTotS*100 < FUELWARN;

//   if (caution != 0)
//     digitalWrite(YLED,HIGH);
//   else
//     digitalWrite(YLED,LOW);

//   if (warning != 0)
//     digitalWrite(RLED,HIGH);
//   else
//     digitalWrite(RLED,LOW);
    
  digitalWrite(SASLED,ControlStatus(AGSAS));
//   digitalWrite(RCSLED,ControlStatus(AGRCS));
//   digitalWrite(CG1LED,ControlStatus(AGCustom01));
//   digitalWrite(CG2LED,ControlStatus(AGCustom02));
//   digitalWrite(CG3LED,ControlStatus(AGCustom03));
//   digitalWrite(CG4LED,ControlStatus(AGCustom04));
//   digitalWrite(CG5LED,ControlStatus(AGCustom05));
//   digitalWrite(CG6LED,ControlStatus(AGCustom06));
//   digitalWrite(CG7LED,ControlStatus(AGCustom07));
//   digitalWrite(CG8LED,ControlStatus(AGCustom08));
//   digitalWrite(CG9LED,ControlStatus(AGCustom09));
//   digitalWrite(CG10LED,ControlStatus(AGCustom10));
}


bool KSPIO::ReceiveData() {
  if ((rx_len == 0)&&(Serial.available()>3)){
    while(Serial.read()!= 0xBE) {
      if (Serial.available() == 0)
        return false;  
    }
    if (Serial.read() == 0xEF){
      rx_len = Serial.read();   
      id = Serial.read(); 
      rx_array_inx = 1;

      switch(id) {
      case 0:
        structSize = sizeof(HPacket);   
        address = (uint16_t*)&HPacket;     
        break;
      case 1:
        structSize = sizeof(VData);   
        address = (uint16_t*)&VData;     
        break;
      }
    }

    //make sure the binary structs on both Arduinos are the same size.
    if(rx_len != structSize){
      rx_len = 0;
      return false;
    }   
  }

  if(rx_len != 0){
    while(Serial.available() && rx_array_inx <= rx_len){
      buffer[rx_array_inx++] = Serial.read();
    }
    buffer[0] = id;

    if(rx_len == (rx_array_inx-1)){
      //seem to have got whole message
      //last uint8_t is CS
      calc_CS = rx_len;
      for (int i = 0; i<rx_len; i++){
        calc_CS^=buffer[i];
      } 

      if(calc_CS == buffer[rx_array_inx-1]){//CS good
        memcpy(address,buffer,structSize);
        rx_len = 0;
        rx_array_inx = 1;
        return true;
      }
      else{
        //failed checksum, need to clear this out anyway
        rx_len = 0;
        rx_array_inx = 1;
        return false;
      }
    }
  }

  return false;
}

void KSPIO::SendData(uint8_t * address, uint8_t len) {
  uint8_t CS = len;
  Serial.write(0xBE);
  Serial.write(0xEF);
  Serial.write(len);
  
  for(int i = 0; i<len; i++){
    CS^=*(address+i);
    Serial.write(*(address+i));
  }
  
  Serial.write(CS);
}