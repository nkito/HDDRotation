const int MOTOR_1   = 2;
const int MOTOR_2   = 3;
const int MOTOR_3   = 4;

const int LED_BOARD = 13;
const int LED_R     = 12;
const int LED_G     = 10;
const int LED_B     = 11;


const int start_delay  = 30000;
const int stable_delay = 800; // max 800

void setup() {
  pinMode(MOTOR_1, OUTPUT);
  pinMode(MOTOR_2, OUTPUT);
  pinMode(MOTOR_3, OUTPUT);

  pinMode(LED_R    , OUTPUT);
  pinMode(LED_G    , OUTPUT);
  pinMode(LED_B    , OUTPUT);
  pinMode(LED_BOARD, OUTPUT);
  
  digitalWrite( MOTOR_1,  LOW);
  digitalWrite( MOTOR_2,  LOW);
  digitalWrite( MOTOR_3,  LOW);

  digitalWrite( LED_R,  LOW);
  digitalWrite( LED_G,  LOW);
  digitalWrite( LED_B,  LOW);
}



void motorctl(int rot){
  switch(rot){
   case 1:
    digitalWrite( MOTOR_1, HIGH);
    digitalWrite( MOTOR_2,  LOW);
    digitalWrite( MOTOR_3,  LOW);
    break;
   case 2:
    digitalWrite( MOTOR_1,  LOW);
    digitalWrite( MOTOR_2, HIGH);
    digitalWrite( MOTOR_3,  LOW);
    break;
   default:
    digitalWrite( MOTOR_1,  LOW);
    digitalWrite( MOTOR_2,  LOW);
    digitalWrite( MOTOR_3, HIGH);
  }
}
void ledctl(int col){
  digitalWrite( LED_R, (col&0x4) ? HIGH : LOW );
  digitalWrite( LED_G, (col&0x2) ? HIGH : LOW );
  digitalWrite( LED_B, (col&0x1) ? HIGH : LOW);
}


void spinup(void){
  int s=start_delay;
  int d, i;

  do{
    for(i=1; i<=3; i++){
      motorctl(i);
      if( d < 10 ){
        delayMicroseconds(s);
      }else{
        delay(d);
      }
    }
    if(s >10000){
      s = s - 1000;
    }else if(s > 5000){
      s = s - 100;
    }else if(s > 2000){
      s = s - 10;
    }else if(s - 10 >= stable_delay ){
      s = s - 1;
    }else{
      s = stable_delay;
    }
    d = s/1000;
  }while( s > stable_delay );
}

void stable_rot1(int d, int count){
  int i, s, loop = 0;
  unsigned long start = 0, current;
  
  while(1){
    current = millis();
    if(start == 0) start = current;
    s = (current - start) / d;
    for(i=0; i<3*6; i++){
      motorctl((i%3)+1);
      delayMicroseconds(stable_delay);
      ledctl(i + s);
    }
    if( s == count ){
      return;
    }
  }
}

void stable_rot2(int fgcolor, int bgcolor, int d, int count){
  int i, s, t, rad = 0, loop = 0;
  unsigned long start = 0, current;

  while(1){
    current = millis();
    if( start == 0 ) start = current;
    rad = (current - start) / d;
    for(i=0; i<3*6; i++){
      motorctl((i%3)+1);
      delayMicroseconds(stable_delay);
      if( i < rad ){
        ledctl(fgcolor);
      }else{
        ledctl(bgcolor);
      }
    }
    if(rad == 3*6+2){
      if(++loop >= count){
        return;
      }
      start = current;
      i = fgcolor;
      fgcolor = bgcolor;
      bgcolor = i;
    }
  }
}

void loop() {
  spinup();
  int color=1;
  while(1){
    stable_rot1(250, 18);
    stable_rot2(color, 0, 50, 4);
    if( ++color == 8 ){ color = 1; }
  }
}
