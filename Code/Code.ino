/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#define BLYNK_TEMPLATE_ID "TMPLu9mteHzu"
#define BLYNK_DEVICE_NAME "Mobile Robotic Arm"
#define BLYNK_AUTH_TOKEN "M06VJ8gb_LFeImP_va8tCfD8u3_2HzZX"

#include <ESP32Servo.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "M06VJ8gb_LFeImP_va8tCfD8u3_2HzZX";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "NAMAN";
char pass[] = "password";


//-----------
WidgetTerminal terminal(V2);

BlynkTimer timer;
//------------

//-----------HCSRO4 DEF
#define echoPinIN 32 
#define trigPinIN 33 
#define echoPinOUT 25
#define trigPinOUT 26 

long duration;
int FrontDistance;
int RearDistance;
int RightDistance;
int LeftDistance;

//-----END OF------HCSRO4 DEF


//-----------------
Servo myservo1; 
Servo myservo2;
Servo myservo3;
Servo myservo4;
Servo myservo5;
Servo myservo6;
//-----------------
void SensorServoOperate(int sen = 90)
{
  sen = map(sen, 0, 180, 0, 147); 
  myservo6.write(sen); //ROTATE CLAW
}
void ClawServoOperate(int claw = 160)
{
  myservo4.write(claw); //ROTATE CLAW
}
void BaseServoOperate(int rota =90)
{
  rota = map(rota, 0, 180, 0, 147); 
  myservo1.write(rota); //BASE
}
void WristServoOperate(int armrota=90)
{
  armrota = map(armrota, 0, 180, 0, 147); 
  myservo5.write(armrota); //ROTATE ARM
}

void ArmServoOperate(int t1, int t2)
{
  t1 = map(t1, 0, 180, 0, 147); 
  t2 = map(t2, 0, 180, 0, 147); 
  myservo2.write(t1); //1ST ARM
    myservo3.write(t2); //2ND ARM
    
}
    
void Home()
{
  WristServoOperate(90);
  ClawServoOperate(130);
  ArmServoOperate(20, 30);
  delay(100);
  BaseServoOperate(180);
}
int FrontDist()
{
  digitalWrite(trigPinIN, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinIN, LOW);
  duration = pulseIn(echoPinIN, HIGH);
  return (duration * 0.034 / 2);
}
int BackDist()
{
  digitalWrite(trigPinOUT, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPinOUT, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinOUT, LOW);
  duration = pulseIn(echoPinOUT, HIGH);
  return (duration * 0.034 / 2);
}
void GetSensorData()
{
  //---FRONT
  FrontDistance = FrontDist(); 
  
  SensorServoOperate(90);
  delay(1000);

  //---REAR
  RearDistance =BackDist(); 
  delay(500);

  //---LEFT
  SensorServoOperate(0);
  delay(1000);
  LeftDistance = BackDist(); 

  
  //--RIGHT
  SensorServoOperate(180);
  delay(1000);
  RightDistance = BackDist(); 

  delay(500);
  SensorServoOperate(90);

  Serial.println("Distance: ");
  Serial.println(FrontDistance);
  Serial.println(RearDistance);
  Serial.println(LeftDistance );
  Serial.println(RightDistance);
  
}


//-------------------Motor Control Functions
void forward()
{
  digitalWrite(23, HIGH);
  digitalWrite(22, HIGH);
  
  digitalWrite(13, LOW);
  digitalWrite(12, HIGH);
  digitalWrite(14, HIGH);
  digitalWrite(27, LOW);
}
void backward()
{
  digitalWrite(23, HIGH);
  digitalWrite(22, HIGH);
  
  digitalWrite(13, HIGH);
  digitalWrite(12, LOW);
  digitalWrite(14, LOW);
  digitalWrite(27, HIGH);
}
void right()
{
  digitalWrite(23, HIGH);
  digitalWrite(22, HIGH);
  
  digitalWrite(13, LOW);
  digitalWrite(12, HIGH);
  digitalWrite(14, LOW);
  digitalWrite(27, HIGH);
}
void left()
{
  digitalWrite(23, HIGH);
  digitalWrite(22, HIGH);
  
  digitalWrite(13, HIGH);
  digitalWrite(12, LOW);
  digitalWrite(14, HIGH);
  digitalWrite(27, LOW);
}
void Brake()
{
  digitalWrite(23, LOW);
  digitalWrite(22, LOW);
}


//-------END OF------------Motor Control Functions


//------------------ CALCULATE THETA AND OPERATE SERVOS
double ya,z;
int theta1,theta2;
String Serialt1="";
String Serialt2="";
String Serialt3="";
void calcTheta(int dist)
{
  int calibration=5;
  int x = dist + calibration;

  //-------for theta1
  if(x<=15)
  {
    ya = -0.00005*pow(x,5) + 0.0026*pow(x,4) - 0.0495*pow(x,3) + 0.324*pow(x,2) + 1.6995*x + 67.123;
  }
  else if(x>15)
  {
    ya = -0.0288*pow(x,3) + 1.5939*pow(x,2) - 30.017*x + 281.15;

  }
  theta1= (int)ya;
  theta1 = 180 - theta1;
  
  //-------for theta2
 // z = 0.000007*pow(x,6) - 0.0006*pow(x,5) + 0.0189*pow(x,4) - 0.3108*pow(x,3) + 2.8174*pow(x,2) - 11.677*x + 82.814;
z= 0.0089*pow(x,3) - 0.2351*pow(x,2) + 4.1228*x + 49.064;
  theta2= (int)z;
  
  Serialt1 = String(theta1);
  Serialt2 = String(theta1);
  Serialt3 = String(x);
  Serial.println("D= "+Serialt3);
  Serial.println("Theta1= "+Serialt1);
  Serial.println("Theta2= "+Serialt2);
  ArmServoOperate(theta1,theta2);
}
//---------END OF--------- CALCULATE THETA AND OPERATE SERVOS

//-------------------Auto Turn
void autoTurn()
{
  int perpendicularTurnTime = 600;
  terminal.clear();
  GetSensorData();
  if( FrontDistance>=20 && RearDistance>=20 && LeftDistance>=20 && RightDistance>=20)
  {
    //finding greater distance
    if(RightDistance>=LeftDistance)
    {
      terminal.println("Turning Right");
      right();
      delay(perpendicularTurnTime);
      Brake();
    }
    else
    {
      terminal.println("Turning Left");
      left();
      delay(perpendicularTurnTime);
      Brake();
    }
  }
  else
  {
    terminal.println("Insufficient Space!!!");
    Serial.println("Insufficient Space!!!");
  }
  terminal.flush();
}

//----------END OF---------Auto Turn

//--------Base Servo
int baseValue=0;
BLYNK_WRITE(V0)
{
  baseValue = param.asInt(); 
  baseValue = map(baseValue, 0, 100, 130, 50); 
  BaseServoOperate(baseValue);
}

//---END OF-----Base Servo

//------------------------VertSlider for dist to angle
double vertValueD;
int vertValue;
BLYNK_WRITE(V4)    //VertSlider for dist to angle
{
  vertValueD = param.asDouble(); 
  vertValueD *= 100;
  vertValue = (int)vertValueD;
  vertValue = map(vertValue, 0, 100, 0, 23); 
  if(baseValue>=50 && baseValue<=130)
  {
  calcTheta(vertValue);
  }
  else
  {
  BaseServoOperate(90);
  delay(500);
  calcTheta(vertValue);
  }

  Serial.println(vertValue);
}
//-------END OF-----------------VertSlider for dist to angle



//--------Wrist Servo
int wristValue;
BLYNK_WRITE(V5)
{
  wristValue = param.asInt(); 
  wristValue = map(wristValue, 0, 100, 0, 180); 
  WristServoOperate(wristValue);
}

//---END OF-----Wrist Servo


//--------Claw Servo
int ClawValue;
BLYNK_WRITE(V6)
{
  ClawValue = param.asInt(); 
  ClawValue = map(ClawValue, 0, 100, 180, 120); 
  ClawServoOperate(ClawValue);
}

//---END OF-----Claw Servo

//---------HOME
BLYNK_WRITE(V7)
{
  int HomebuttonState = param.asInt();
  if(HomebuttonState==1)
  {
    baseValue=0;
    Home();
  }
}
//---END OF-----
//-----autoTurnBtn
BLYNK_WRITE(V8)
{
  int autoBtnState;
  autoBtnState = param.asInt(); 
  if(autoBtnState==1)
  {
    autoTurn();
  }
}
//--END OF---autoTurnBtn


//-----ArmToObject
BLYNK_WRITE(V9)
{
  int armBtnState;
  int frontDist;
  armBtnState = param.asInt(); 
  if(armBtnState==1)
  {
    //Calculate Front Distance
    frontDist = FrontDist(); 
    
    //IF 5<=front dist<=23
    if(frontDist>=5 && frontDist<=23)
    {
      //calc theta and move but first check if the arm is at correct position or not
      if(baseValue>=50 && baseValue<=130)
      {
        ClawServoOperate(180);
        calcTheta(frontDist);
        }
        else
        {
          BaseServoOperate(90);
          delay(500);
          ClawServoOperate(180);
          calcTheta(frontDist);
          }
    }
    else
    {
      terminal.println("Out Of Arm Reach");
    }
    terminal.flush();
  }
}
//--END OF---ArmToObject

//-------------------Lift Object
BLYNK_WRITE(V10)
{
  int liftBtnState;
  liftBtnState = param.asInt(); 
  if(liftBtnState==1)
  {
    ArmServoOperate(10, 50);
  }

}
//-------END OF------------Lift Object


BLYNK_WRITE(V2)
{


  if (String("sensor") == param.asStr()) {
    terminal.println("Finding Sensor data") ;
    GetSensorData();
    terminal.println("Front") ;
    terminal.println(FrontDistance) ;
    terminal.println("Rear") ;
    terminal.println(RearDistance) ;
    terminal.println("Left") ;
    terminal.println(LeftDistance) ;
    terminal.println("Right") ;
    terminal.println(RightDistance) ;
  }
  else {

    // Send it back
    terminal.print("You said:");
    terminal.write(param.getBuffer(), param.getLength());
    terminal.println();
  }

  // Ensure everything is sent
  terminal.flush();
}


//-------------------------- Joystick motor control
BLYNK_WRITE(V3) {
  int x = param[0].asInt();
  int y = param[1].asInt();
  int fDist;

  // Do something with x and y
  Serial.print("X = ");
  Serial.print(x);
  Serial.print("; Y = ");
  Serial.println(y);
// for front and back movement
if(y>=200)
{
  fDist=FrontDist();
  delay(100);
  terminal.println(fDist);
  if(fDist<=30)
  {
    Brake();
    Serial.print("Brake!! Obstacle in Front");
    terminal.println("Brake!! Obstacle in Front");
  }
  else
  {
    forward();
    Serial.print("Forward");
  }

}
else if(y<=50)
{
  backward();
  Serial.print("backward");
}

// for right left movement
if(x>=200)
{
  right();
  Serial.print("right");
}
else if(x<=50)
{
  left();
  Serial.print("left");
}

if(x==128 and y==128)
{
  Brake();
  Serial.print("brake");
}

}
//----------END OF---------------- Joystick motor control






void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
//-------------motor setup
  pinMode(13, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(27, OUTPUT);
  pinMode(23, OUTPUT);
  pinMode(22, OUTPUT);
//------------end of motor setup
//----------HCSRO4 setup
  pinMode(trigPinIN, OUTPUT); 
  pinMode(trigPinOUT, OUTPUT); 
  pinMode(echoPinIN, INPUT); 
  pinMode(echoPinOUT, INPUT);
//----END OF------HCSRO4 setup

  myservo1.attach(15); 
  myservo2.attach(2);
  myservo3.attach(4);
  myservo4.attach(5);
  myservo5.attach(18);
  myservo6.attach(19);

}

void loop()
{
  Blynk.run();
  timer.run();
//Serial.println("Working");  
}
