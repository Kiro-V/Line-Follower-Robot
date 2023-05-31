

// X values will be read from pin 0 and Y from pin 1

int Right = 5;
int Left = 6;
int In1 = 2;
int In2 = 3;
int In3 = 4;
int In4 = 7;
int Sen1 = 8;
int Sen2 = 9;
int Sen3 = 10;
int Sen4 = 11;
int Sen5 = 12;
bool s1,s2,s3,s4,s5;

void setup() {
 // Start serial because we will observe values at serial monitor
 Serial.begin(9600);
 pinMode(Sen1,INPUT);
 pinMode(Sen2,INPUT);
 pinMode(Sen3,INPUT);
 pinMode(Sen4,INPUT);
 pinMode(Sen5,INPUT);
 pinMode(Right,OUTPUT);
 pinMode(Left,INPUT);
 pinMode(In1,OUTPUT);
 pinMode(In2,OUTPUT);
 pinMode(In3,OUTPUT);
 pinMode(In4,OUTPUT);
}
void ReadSensor()
{
  s1=digitalRead(Sen1);
  s2=digitalRead(Sen2);
  s3=digitalRead(Sen3);
  s4=digitalRead(Sen4);
  s5=digitalRead(Sen5);
}
int error = 0;
int I = 0, D = 0, errorLast = 0;
int PID()
{
  ReadSensor();
  //Propotional
  if (!s1 && !s2 && !s3 && !s4 && !s5)
  {
    return 301;
  }
  else if (s1 && s2 && s3 && s4 && s5)
  {
    if (errorLast < 0) error =-5;
    else error = +5;
  }
  else if (!s1 && !s2) error =-4;
  else if (!s4 && !s5) error =+4;
  else if (!s1) error =-3;
  else if (!s5) error =+3;
  else if (!s3 && !s2) error =-2;
  else if (!s3 && !s4) error =+2;
  else if (!s2) error = -1;
  else if (!s4) error =+1;
  else if (!s3) error =+0;
  //Derivative
  D = error - errorLast;
  errorLast = error;
  //Integral
  I = constrain(I + error,-500,500);
  return 50*error + I/4 + D*580;// Max propotion = +- 100 Max integral = +- 25 Max derivative = 2500 => PID Max = +- 180
}
//
void MotorRight(int speeda, bool f1, bool f2)
{
  digitalWrite(In1,f1);
  digitalWrite(In2,f2);
  analogWrite(Right,speeda);
}
void MotorLeft(int speedb, bool f4, bool f3)
{
  digitalWrite(In4,f4);
  digitalWrite(In3,f3);
  analogWrite(Left,speedb);
}
bool flag = true;
int ValuePID;
int inispeed = 100;
int effspeedLeft;
int effspeedRight;
void loop() 
{
  ValuePID = PID();
  if (ValuePID == 301)
  {
    delay(100);
    if (PID() != 301) flag = false;
  }
  else if (flag)
  {
  effspeedLeft = inispeed + ValuePID;
  effspeedRight = inispeed - ValuePID;
  if (effspeedLeft < 0 ) MotorLeft(constrain(abs(effspeedLeft),0,255), 1, 1);
  else MotorLeft(constrain(abs(effspeedLeft),0,255), 1, 0);
  if (effspeedRight < 0 ) MotorRight(constrain(abs(effspeedRight),0,255), 0, 1);
  else MotorRight(constrain(abs(effspeedRight),0,255), 1, 0);
  delay(10);
  }
  MotorLeft(0,0,0);
  MotorRight(0,0,0);
}
