import processing.serial.*;

XML xml;
Serial myPort;
String Serial_port;
int[] readData = new int[4];
int read_count = 0,time_count = 0;
boolean read_finish_flag = false;
boolean change_flag = false;
int InteractiveFlag = 0,InteractiveCount = 0;
String[] words = {"Neil", "Anne", "Emily", "Greg", "Jill", "Zhang Wei", "Kexin", "Mohamed", "Jamal Jones", "Wangfang", "LIGHT"};
String a = "ERROR";
String[] recName = {"Neil", "Anne", "Emily", "Greg","Greg", "Jill", "Zhang Wei", "Kexin", "Mohamed", "Jamal Jones", "Wangfang", "LIGHT","UNKNOWN","UNKNOWN","UNKNOWN","UNKNOWN","UNKNOWN","UNKNOWN","UNKNOWN","UNKNOWN"};
String[] correctName = {"Neil", "Anne", "Emily", "Greg", "Jill"};
String input_name = "";
byte[] send = {(byte)0xff,(byte)0x00,(byte)0x01,(byte)0x0b};

void setup()
{
  noStroke();
  fullScreen(JAVA2D);
  surface.setSize(1920, 1080);
  surface.setLocation(0, 0);
  background(255);
  /****************Serial Config********************/
  xml = loadXML("setting.xml");
  XML[] children = xml.getChildren("serial");
  for(int i = 0; i < children.length; i++){
    if(i == 0) Serial_port = children[i].getString("port");
  }
  println(Serial_port);
  try{
    String portName = Serial.list()[5];
    myPort=new Serial(this,portName,115200);
    //myPort = new Serial(this, Serial_port, 115200);
  }catch(Exception e){
    println("open serial fill!");
    println(e);
  }
  HeartConfig();
  frameRate(12);
}

void draw(){
   if(read_finish_flag == true){
     println(readData);
     if(readData[0] == 0xff && readData[3] == 0x0b){
       if(readData[1] == 0x01) {InteractiveFlag = 1;}
       else if(readData[1] == 0x00) {InteractiveFlag = 2;}
       else if(readData[1] == 0x02) {InteractiveFlag = 0;background(#FFFFFF);}
     }
     read_finish_flag = false;
   }
   switch(InteractiveFlag){
     case 0:{
       Nomal(input_name);
       InteractiveCount = 0;
     }
     break;
     case 1:{
       InteractiveCount++;
       if(InteractiveCount < 20){
         Nomal(recName[readData[2]-1]);
       }else if(InteractiveCount == 20){
         //background(346,50,99);
       }
       else{
         InteractiveCount = 20;
         HeartBeat();
       }
     }
     break;
     case 2:{
       InteractiveCount++;
       if(InteractiveCount < 20){
         Nomal(recName[readData[2]-1]);
       }else if(InteractiveCount == 20){
       background(0);
       }else{
         InteractiveCount = 20;
         Wrong();
       }
       
     }
     break;
     default:break;
   }
}



void Nomal(String indata){

  fill(#FFFFFF, 20);
  rect(0,0,width,height-80);
  //stroke(#FFFFFF);
  //line(0,0,720,int(random(480))); 
  
  //stroke(#000000);
  //line(0,0,int(random(720)),480); 
  fill(#000000);
  textSize(random(20, 200));
  
  text(words[int(random(11))], random(width), random((height-80)));
  noStroke();
  fill(#D1D1D1);
  rect(0,height-80,width,80);
  fill(#000000);
  textSize(50);
  time_count++;
  if(indata.length()==0){
    if(time_count < 8){
      text("|",width/2,height-20);
    }else if(time_count < 16){
      text("",width/2-indata.length()*18,height-20);
    }else{
      time_count = 0;
    }
  }
  
  text(indata,width/2-indata.length()*18,height-20);
}

void Wrong(){
  textSize(60);
  int x = (int)random(0,width);
  int y = (int)random(0,height);
  
  stroke(#FF0000);
  fill(0);
  rect(x - textWidth(a) / 2,y,textWidth(a),-60);
  fill(#FF0000);
  text(a,x - textWidth(a) / 2,y - 10); 
}

void serialEvent(Serial myPort) {
  readData[read_count] = myPort.read();
  read_count++;
  
  if(read_count>=4){
    read_count = 0;
    read_finish_flag = true;
  }
  
}

void keyPressed() {
  if(InteractiveFlag == 0){
    //println("pressed " + int(key) + " " + keyCode);
    char temp = key;
    if((int(key) >= 65 && int(key) <= 90) || (int(key) >= 97 && int(key) <= 122)){
      input_name += temp;
    }
    else if(int(key) == 8){
      if(input_name.length() > 0){
        input_name = input_name.substring(0,input_name.length()-1);
      }
    }
    else if(int(key) == 127){
      input_name = "";
    }
    else if(int(key) == 10){//确认键
      println(input_name);
      //{"Neil", "Anne", "Emily", "Greg", "Jill"};
      if(input_name.equals(correctName[0])||input_name.equals(correctName[1])||input_name.equals(correctName[2])||input_name.equals(correctName[3])||input_name.equals(correctName[4])){ 
        send[1] = (byte)0x01;
        myPort.write(send);  
      }else{
        send[1] = (byte)0x00;
        myPort.write(send);  
      }
      input_name = "";
    }
  }
}
