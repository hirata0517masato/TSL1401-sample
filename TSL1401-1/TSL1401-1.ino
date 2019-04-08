#define TSL_SI 7
#define TSL_CLK 8
#define TSL_AD A0

#define   LineStart   27    /* カメラで見る範囲*/
#define   LineStop    100

/*======================================*/
/* グローバル変数の宣言                 */
/*======================================*/
unsigned long   cnt =  0;

/* カメラ関連 */
int   ImageData[128];                 /* カメラの値        */
int   Max = 0,Min;        /*カメラ読み取り最大値、最小値*/


void setup() {
  Serial.begin(9600);       // 通信速度は9600bps
  
  pinMode(TSL_SI,OUTPUT);
  pinMode(TSL_CLK,OUTPUT);
  pinMode(TSL_AD,INPUT);
}

void loop() {
  while(1){
    ImageCapture(LineStart,LineStop);     //イメージキャプチャー

    cnt++;
    if(cnt>50){
      for(int i = LineStart; i <= LineStop; i++){
         Serial.print(ImageData[i]);
         Serial.print(",");
      }
      Serial.println(" ");
      Serial.print(Max);
      Serial.print(" ");
      Serial.println(Min);
      
      cnt=0;
    }
  }
}


/************************************************************************/
/* イメージキャプチャ                                                   */
/************************************************************************/
void ImageCapture(int linestart, int linestop){   
  
  unsigned char i;

  Max = 0,Min = 1024;

  digitalWrite( TSL_SI, HIGH );
  digitalWrite( TSL_CLK, HIGH );
  digitalWrite( TSL_SI, LOW );
  digitalWrite( TSL_CLK, LOW );
  
  for(i = 0; i < linestart; i++) {    
    digitalWrite( TSL_CLK, HIGH ); 
    digitalWrite( TSL_CLK, LOW );
  }

  for(i = linestart; i <= linestop; i++) {        
    digitalWrite( TSL_CLK, HIGH ); 
    ImageData[i] = analogRead(TSL_AD);  
    digitalWrite( TSL_CLK, LOW );
    
    if(Max < ImageData[i]){
      Max = ImageData[i];
    }     
    if(Min > ImageData[i]){
      Min = ImageData[i];
    }
  }
 
  for(i = linestop+1; i < 128; i++) {   
    digitalWrite( TSL_CLK, HIGH ); 
    digitalWrite( TSL_CLK, LOW );
  }
}
