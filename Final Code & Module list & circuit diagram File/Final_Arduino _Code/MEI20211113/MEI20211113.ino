#include <DFRobot_DF1201S.h>
#include <SoftwareSerial.h>
#include "ASR.h"
#include <Adafruit_NeoPixel.h>
#include <MsTimer2.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define PIN        6 // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS 64 // Popular NeoPixel ring size

#define PINled       7 // On Trinket or Gemma, suggest changing this to 1
#define NUMPIXELS2 60 // Popular NeoPixel ring size

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels2(NUMPIXELS2, PINled, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 500 // Time (in milliseconds) to pause between pixels
SoftwareSerial DF1201SSerial(2, 3);  //RX  TX
DFRobot_DF1201S DF1201S;
int recieve_data[20];
int recieve_count = 0;
int keyState = 0; 
int keyFlag = 0;
boolean recieve_finish = false;

byte senddata[4] = {0xff,0x02,0x00,0x0b};
boolean busyflag = false;
unsigned char result;
int time_count = 0,interactive = 0;
void flash(){
  if(busyflag == true){
    time_count++;
  }
}

void setup() {
  
  Serial.begin(115200);
  DF1201SSerial.begin(115200);
  pinMode(5, INPUT);
  unsigned char cleck = 0xff;
  unsigned char asr_version = 0;
  Wire.begin();
  Wire.setClock(100000);
  WireReadData(FIRMWARE_VERSION,&asr_version,1);
//  Serial.print("asr_version is ");
//  Serial.println(asr_version);

#if 0
    I2CWrite(ASR_CLEAR_ADDR,0x40);
    BusyWait();
    Serial.println("clear flash is ok");   
    I2CWrite(ASR_MODE_ADDR,0);
    BusyWait();
    Serial.println("mode set is ok");              
    AsrAddWords(1,"niu er");//Neil
    BusyWait();    
    AsrAddWords(2,"an en");//Anne
    BusyWait();
    AsrAddWords(3,"e me li");//Emily
    BusyWait();
    AsrAddWords(4,"gui");//Greg
    BusyWait();
    AsrAddWords(5,"ge wei");//Greg
    BusyWait();
    AsrAddWords(6,"jiu");//Jill
    BusyWait();
    AsrAddWords(7,"zhang wei");//Zhang Wei
    BusyWait();
    AsrAddWords(8,"ke xin");//Kexin
    BusyWait();
    AsrAddWords(9,"mo ha mi");//Mohamed
    BusyWait();
    AsrAddWords(10,"jiang mo jiong");//Jamal Jones
    BusyWait();
    AsrAddWords(11,"wang fang");//Wangfang
    BusyWait();
    AsrAddWords(12,"lai ti");//LIGHT
    BusyWait();
    AsrAddWords(13,"a a a");//UNKNOWN
    BusyWait();
    AsrAddWords(14,"wu wu");//UNKNOWN
    BusyWait();
    AsrAddWords(15,"e e");//UNKNOWN
    BusyWait();
    AsrAddWords(16,"ni hao");//UNKNOWN
    BusyWait();
    AsrAddWords(17,"o ho");//UNKNOWN
    BusyWait();
    AsrAddWords(18,"wang wang");//UNKNOWN
    BusyWait();
    AsrAddWords(19,"liu liu liu");//UNKNOWN
    BusyWait();
    AsrAddWords(20,"guan deng");//UNKNOWN
    BusyWait();
    while(cleck != 20)
    {
      WireReadData(ASR_NUM_CLECK,&cleck,1);
//      Serial.println(cleck);
      delay(100);
      }
//    Serial.println("cleck is ok");     
#endif
  I2CWrite(ASR_REC_GAIN,0x40);  //0x40-0x55语音识别灵敏度
  I2CWrite(ASR_VOICE_FLAG,1);  //
  I2CWrite(ASR_BUZZER,1);  //
  RGB_Set(255,255,255);//
  delay(500);
  I2CWrite(ASR_BUZZER,0);  //
  RGB_Set(0,0,0);//

  
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
  #endif
   pixels.begin();
   pixels.clear(); // Set all pixel colors to 'off'

   pixels2.begin();
   pixels2.clear();
//设置MP3
  while(!DF1201S.begin(DF1201SSerial)){
//    Serial.println("Init failed, please check the wire connection!");
    delay(1000);
  }
  /*Set volume to 20*/
  DF1201S.setVol(/*VOL = */20);//设置声音大小
//  Serial.print("VOL:");
  /*Enter music mode*/
  DF1201S.switchFunction(DF1201S.MUSIC);
  delay(2000);
  /*Set playback mode to "repeat all"*/
  DF1201S.setPlayMode(DF1201S.ALLCYCLE);
  
  MsTimer2::set(100, flash); // 100ms period
  MsTimer2::start();
  delay(5000);
  WireReadData(ASR_RESULT,&result,1);
  result = 255;
}

void loop() {
  
  keyState = digitalRead(5);

  if(keyState == HIGH && keyFlag == 0){
    keyFlag = 1;
    
  }else if(keyState == LOW && keyFlag == 1){
    keyFlag = 2;
    
  }else if(keyState == HIGH && keyFlag == 2){
    keyFlag = 3;
  }else if(keyState == LOW && keyFlag == 3){
    keyFlag = 0;
  }
  if(keyFlag == 2){
    for(int i = 0;i < NUMPIXELS2 ;i++){
      pixels2.setPixelColor(i, pixels2.Color(50, 50, 0));
       
    }
    pixels2.show();  
  }else if(keyFlag == 0){
    pixels2.clear(); 
    pixels2.show();  
  }
  
  WireReadData(ASR_RESULT,&result,1);//
  if(busyflag == false){

    if(recieve_finish == true){
      if(recieve_data[0] == 0xff && recieve_data[3] == 0x0b){
        
        if(recieve_data[1] == 0x01){
          result = 1;
        }else{
          result = 14;
        }
      }
      recieve_finish = false;
    }



    
    if(result < 7) {
      busyflag = true;
      interactive = 1;
      
      for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
        if(i == 2||i == 3||i == 4||i == 5||i == 9||i == 10||i == 13||i == 14||i == 16||i == 17||i == 22||i == 23||i == 24||i == 31||i == 32||i == 39||i == 40||i == 41||i == 46||i == 47||i == 49||i == 50||i == 53||i == 54||i == 58||i == 59||i == 60||i == 61){
          pixels.setPixelColor(i, pixels.Color(0, 50, 0));
          pixels.show();   // Send the updated pixel colors to the hardware.
        }
//        if(i < NUMPIXELS2){
//          pixels2.setPixelColor(i, pixels.Color(50, 50, 0));
//          pixels2.show(); 
//        }
      }
      time_count = 0;
      senddata[1] = 0x01;
      senddata[2] = (byte)result;
      Serial.write(senddata,4);
      DF1201S.start();
      DF1201S.playFileNum(/*File Number = */1);
      
    }
    else if(result >= 7 && result < 21) {
      busyflag = true;
      interactive = 2;
      
      for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
        if(i == 0||i == 7||i == 9||i == 14||i == 18||i == 21||i == 27||i == 28||i == 35||i == 36||i == 42||i == 45||i == 49||i == 54||i == 56||i == 63){
          pixels.setPixelColor(i, pixels.Color(50, 0, 0));
          pixels.show();   // Send the updated pixel colors to the hardware.
          
        }
//        if(i < NUMPIXELS2){
//          pixels2.setPixelColor(i, pixels.Color(50, 50, 0));
//          pixels2.show(); 
//        }
      }
      time_count = 0;
      senddata[1] = 0x00;
      senddata[2] = (byte)result;
      Serial.write(senddata,4);
      DF1201S.start();
      DF1201S.playFileNum(/*File Number = */2);
      
    }
  }
  if(busyflag == true){
    switch(interactive){
      case 0:{
        
      }break;
      case 1:{
//        for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
//          if(i == 2||i == 3||i == 4||i == 5||i == 9||i == 10||i == 13||i == 14||i == 16||i == 17||i == 22||i == 23||i == 24||i == 31||i == 32||i == 39||i == 40||i == 41||i == 46||i == 47||i == 49||i == 50||i == 53||i == 54||i == 58||i == 59||i == 60||i == 61){
//            pixels.setPixelColor(i, pixels.Color(0, 50, 0));
//            pixels.show();   // Send the updated pixel colors to the hardware.
//          }
//        }
        if(time_count > 110){
          senddata[1] = 0x02;
          senddata[2] = 0x00;
          Serial.write(senddata,4);
          result = 255;
          time_count = 0;
          DF1201S.pause();
          pixels.clear(); 
          pixels.show();
//          pixels2.clear(); 
//          pixels2.show();
          interactive = 0;
          busyflag = false;
        }
      }break; 
      case 2:{
//        for(int i=0; i<NUMPIXELS; i++) { // For each pixel...
//        if(i == 0||i == 7||i == 9||i == 14||i == 18||i == 21||i == 27||i == 28||i == 35||i == 36||i == 42||i == 45||i == 49||i == 54||i == 56||i == 63){
//          pixels.setPixelColor(i, pixels.Color(50, 0, 0));
//          pixels.show();   // Send the updated pixel colors to the hardware.
//          
//        }
//      }
        if(time_count > 205){
          senddata[1] = 0x02;
          senddata[2] = 0x00;
          Serial.write(senddata,4);
          result = 255;
          time_count = 0;
          DF1201S.pause();
          pixels.clear(); 
          pixels.show();
//          pixels2.clear(); 
//          pixels2.show();
          interactive = 0;
          busyflag = false;
        }
      }break;
      default:break;
    }
    
  }
  
  
  delay(100);

  
  
}

void serialEvent() {
  while (Serial.available()) {
    recieve_data[recieve_count] = Serial.read();
    recieve_count++;
  }
  recieve_count = 0;
  recieve_finish = true;
}
