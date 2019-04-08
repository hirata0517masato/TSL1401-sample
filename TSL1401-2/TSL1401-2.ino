#define TSL_SI 7
#define TSL_CLK 8
#define TSL_AD A0

#define   Line_Max  450   /* ライン白色MAX値の設定 */

#define   LineStart   27    /* カメラで見る範囲*/
#define   LineStop    100

/*======================================*/
/* グローバル変数の宣言                 */
/*======================================*/
unsigned long   cnt =  0;

/* カメラ関連 */
long  EXPOSURE_timer = 10000;      /* 露光時間   */
int   ImageData[128];               /* カメラの値        */
int   Max = 0,Min;                  /*カメラ読み取り最大値、最小値*/


void setup() {
  Serial.begin(9600);       // 通信速度は9600bps
  
  pinMode(TSL_SI,OUTPUT);
  pinMode(TSL_CLK,OUTPUT);
  pinMode(TSL_AD,INPUT);
}

void loop() {
  while(1){
    expose();                             //露光時間 
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
      Serial.print(Min);
      Serial.print(" ");
      Serial.println(EXPOSURE_timer);
      
      cnt=0;
    }
  }
}

/************************************************************************/
/* 露光時間調整                                                         */
/* 引数　 なし                                                          */
/* 戻り値 なし　　　　　                                                */
/************************************************************************/
void expose( void )
{
  long i;
   
  if(Line_Max - Max < 0){
    EXPOSURE_timer -= 100;
  }else{
    EXPOSURE_timer += 100;
  }
  if( EXPOSURE_timer > 100000000) EXPOSURE_timer = 100000000;
  else if( EXPOSURE_timer <= 0 ) EXPOSURE_timer = 0;
  
  for(i=0;i<EXPOSURE_timer;i++)asm("nop");
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
