#include "ASR.h"


void setup() {
    unsigned char cleck = 0xff;
    unsigned char asr_version = 0;
    Wire.begin();
    Wire.setClock(100000);
    Serial.begin(115200);  //涓插彛娉㈢壒鐜囪缃紝鎵撳嵃鏁版嵁鏃朵覆鍙ｉ渶瑕侀�夋嫨鍜岃繖閲屼竴鏍风殑娉㈢壒鐜�
    pinMode(7,OUTPUT);//椋庢墖杈撳嚭绔彛


    WireReadData(FIRMWARE_VERSION,&asr_version,1);
    Serial.print("asr_version is ");
    Serial.println(asr_version);
#if 1
    I2CWrite(ASR_CLEAR_ADDR,0x40);//娓呴櫎鎺夌數淇濆瓨鍖�,褰曞叆鍓嶉渶瑕佹竻闄ゆ帀鐢典繚瀛樺尯
    BusyWait();
    Serial.println("clear flash is ok");   
    I2CWrite(ASR_MODE_ADDR,0);//璁剧疆妫�娴嬫ā寮�  
    BusyWait();
    Serial.println("mode set is ok");              
    AsrAddWords(1,"hong deng");
    BusyWait();    
    AsrAddWords(2,"lv deng");
    BusyWait();
    AsrAddWords(3,"lan deng");
    BusyWait();
    AsrAddWords(4,"guan deng");
    BusyWait();
    AsrAddWords(5,"kai feng shang");
    BusyWait();
    AsrAddWords(6,"guan feng shang");
    BusyWait();
    while(cleck != 6)
    {
      WireReadData(ASR_NUM_CLECK,&cleck,1);
      Serial.println(cleck);
      delay(100);
      }
    Serial.println("cleck is ok");     
#endif

    I2CWrite(ASR_REC_GAIN,0x40);  //璇嗗埆鐨勭伒鏁忓害锛屽缓璁�0x40-0x55
    I2CWrite(ASR_VOICE_FLAG,1);  //璇嗗埆缁撴灉鎻愮ず闊冲紑鍏宠缃�
    I2CWrite(ASR_BUZZER,1);  //寮�鍚渹楦ｅ櫒
    RGB_Set(255,255,255);//璁剧疆妯″潡鐨凴GB鐏负鐧借壊
    delay(500);
    I2CWrite(ASR_BUZZER,0);  //鍏抽棴铚傞福鍣�
    RGB_Set(0,0,0);//鍏抽棴RGB鐏�    
    
          
}

void loop() {
        unsigned char result;
        WireReadData(ASR_RESULT,&result,1);//璇诲彇璇嗗埆搴忓彿鍊硷紝骞惰祴鍊肩粰result锛岄粯璁ゆ槸0xff
        delay(100);
        Serial.println(result);
  
        if(result == 5)//鍒ゆ柇璇诲彇鐨勮瘑鍒簭鍙峰�兼槸鍚︽槸寮�鍚鎵�
        {
          digitalWrite(7,1);
        }
        else if(result == 6)//鍒ゆ柇璇诲彇鐨勮瘑鍒簭鍙峰�兼槸鍚︽槸鍏抽棴椋庢墖
        {
            digitalWrite(7,0);
        }

}

