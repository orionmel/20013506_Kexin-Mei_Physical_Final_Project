int NUM =150;
int backcol =255;
//ハートの座標を配列で管理
float[]posX=new float[NUM];
float[]posY=new float[NUM];

//スピード
float[]speedX=new float[NUM];
float[]speedY=new float[NUM];

//ハートの幅
float[]widthHH=new float[NUM];
//ハートの高さ
float[]heightHH=new float[NUM];
//色
float[]col=new float [NUM];

//ゆっくりと近づく時間
float easing=0.1;


void HeartConfig(){
  for(int i=0; i<NUM;i++){
    posX[i]=random(width);
    posY[i]=random(height);
    speedX[i]=random(-5,5);
    speedY[i]=random(-5,5);
    widthHH[i]=random(10,40);
    heightHH[i]=random(10,40);
    col[i]=(int)random(360);
  }
}

//ハートの形
void heart(float centerX, float centerY, float widthH, float heightH){
  final float WIDTH = widthH / 2 * 0.85;
  final float HEIGHT = heightH / 2;
  final float OFFSET = centerY - (HEIGHT / 6 * 5);
  beginShape();
  for(int i = 0; i < 30; i++){
  float tx = abs(sin(radians(i * 12))) * (1 + cos(radians(i * 12))) * sin(radians(i * 12)) * WIDTH + centerX;
  float ty = (0.8 + cos(radians(i * 12))) * cos(radians(i * 12)) * HEIGHT + OFFSET;
  vertex(tx, ty);
  }
  endShape(CLOSE);
}

void HeartBeat(){
  noStroke();
  background(346,50,99);
  colorMode(HSB,360,100,100,100);
  for(int i=0; i<NUM;i++){
    fill(col[i],100,100,100);
    
    //XYの座標にスピードを足す
    posX[i]=posX[i]+speedX[i];
    posY[i]=posY[i]+speedY[i];
    
    //跳ね返り
    if(posX[i]>width||posX[i]<0){
      speedX[i]=speedX[i]*-1;
    }
    if(posY[i]>width||posY[i]<0){
      speedY[i]=speedY[i]*-1;
    }  
   heart(posX[i], posY[i], widthHH[i], heightHH[i]);  
  }
}
