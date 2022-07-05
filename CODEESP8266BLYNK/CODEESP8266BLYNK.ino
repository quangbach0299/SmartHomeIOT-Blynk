#include <DNSServer.h>
//#include <ESP8266WebServer.h>
#include <WiFiManager.h>         //https://github.com/tzapu/WiFiManager

#include <Ticker.h>
Ticker ticker;

#define BLYNK_TEMPLATE_ID "TMPLHP81Cggp"
#define BLYNK_DEVICE_NAME "SMART HOME"
#define BLYNK_AUTH_TOKEN "yB7DSMINyKKDxIfvON8NFqV-qpcIPmX3"
#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
char auth[] = "yB7DSMINyKKDxIfvON8NFqV-qpcIPmX3";
char ssid[] = "VIETTEL";
char pass[] = "24682468";
#include <ArduinoJson.h>
#include <SoftwareSerial.h> // 2.5.0 
const byte TX = D6;
const byte RX = D5;
SoftwareSerial mySerial = SoftwareSerial(RX, TX);
#include <NTPClient.h>
#include <WiFiUdp.h>
WiFiUDP u;
NTPClient timeClient(u, "2.asia.pool.ntp.org", 7 * 3600 );

unsigned long times = millis();
#define LED_IO2 2
void YeuCauSTM32SendData(void);

WidgetLED led_connect(V0);
WidgetLED led_MQ2(V11);
WidgetLED led_DOOR(V12);
String DataBlynk = "123";
unsigned long LastSend = 0;
/*
  LED CONNECT V0 INTERGER
  BUTTON ĐÈN PHÒNG KHÁCH  V1 INTERGER
  BUTTON ĐÈN PHÒNG NGỦ V2 INTERGER
  BUTTON ĐÈN PHÒNG ĂN V3 INTERGER
  BUTTON ĐÈN TOLET V4 INTERGER
  BUTTON QUẠT PHÒNG KHÁCH V5 INTERGER
  BUTTON QUẠT PHÒNG NGỦ V6 INTERGER
  BUTTON QUẠT PHÒNG ĂN V7 INTERGER

  NHIỆT ĐỘ V8 INTERGER
  ĐỘ ẨM V9 INTERGER
  GA V10 INTERGER
  LED BÁO CHÁY V11 INTERGER
  LED CỬA V12  INTERGER --- TT_RFID
  CẢNH BÁO CỬA V13 INTERGER -- TT_RFID
  CẢNH BÁO GA V14 INTERGER
  BUTTON ĐIỀU KHIỂN CỬA  V15 INTERGER

*/
/*
  tài khoản blynk
  nguoidungblynk2@gmail.com
  @Aa123456789
*/
String Time = "";
int ND = 0;
int DA = 0;
int MQ2 = 0;
int LEDMQ2 = 0;
int LEDDOOR = 0;
int TT_DEN1 = 0;
int TT_DEN2 = 0;
int TT_DEN3 = 0;
int TT_DEN4 = 0;
int TT_QUAT1 = 0;
int TT_QUAT2 = 0;
int TT_QUAT3 = 0;
int TT_COI = 0;
int TT_IR = 0; // cam bien hong ngoai
int TT_MODE = 0;// led cua va canh bao cua cu
static int TT_CUA  = 0;// trang thai dong mo cua

int TT_DEN11 = 0;
int TT_DEN22 = 0;
int TT_DEN33 = 0;
int TT_DEN44 = 0;
int TT_QUAT11 = 0;
int TT_QUAT22 = 0;
int TT_QUAT33 = 0;
int TT_MODEE  = 0;
bool DataSendControl = false;

//----------MODE CANH BAO-------------------------
int timerID1, timerID2;
BlynkTimer timer;
boolean buttonState = HIGH;
boolean runMode = 0; //Bật tắt chế độ cảnh báo
boolean canhbao = 0;
boolean canhbaoState = 0;
//----------MODE CANH BAO-------------------------

/*
  BLYNK_CONNECTED()
  {
  //Blynk.syncAll();
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2);
  Blynk.syncVirtual(V3);
  Blynk.syncVirtual(V4);
  Blynk.syncVirtual(V5);
  Blynk.syncVirtual(V6);
  Blynk.syncVirtual(V7);
  }
*/

BLYNK_WRITE(V1)
{
  int denphongkhach = param.asInt();
  Serial.print("denphongkhach:");
  Serial.println(denphongkhach);
  mySerial.println("{\"D1\":\"" + String(denphongkhach) + "\"}");
  mySerial.flush();
  TT_DEN11 = denphongkhach;
  DataSendControl = true;
  LastSend = millis();
}
BLYNK_WRITE(V2)
{
  int denphongngu = param.asInt();
  Serial.print("denphongngu:");
  Serial.println(denphongngu);
  mySerial.println("{\"D2\":\"" + String(denphongngu) + "\"}");
  mySerial.flush();
  TT_DEN22 = denphongngu;
  DataSendControl = true;
  LastSend = millis();
}
BLYNK_WRITE(V3)
{
  int denphongan = param.asInt();
  Serial.print("denphongan:");
  Serial.println(denphongan);
  mySerial.println("{\"D3\":\"" + String(denphongan) + "\"}");
  mySerial.flush();
  TT_DEN33 = denphongan;
  DataSendControl = true;
  LastSend = millis();
}
BLYNK_WRITE(V4)
{
  int dentolet = param.asInt();
  Serial.print("dentolet:");
  Serial.println(dentolet);
  mySerial.println("{\"D4\":\"" + String(dentolet) + "\"}");
  mySerial.flush();
  TT_DEN44 = dentolet;
  DataSendControl = true;
  LastSend = millis();
}
BLYNK_WRITE(V5)
{
  int quatphongkhach = param.asInt();
  Serial.print("quatphongkhach:");
  Serial.println(quatphongkhach);
  mySerial.println("{\"Q1\":\"" + String(quatphongkhach) + "\"}");
  mySerial.flush();
  TT_QUAT11 = quatphongkhach;
  DataSendControl = true;
  LastSend = millis();
}
BLYNK_WRITE(V6)
{
  int quatphongngu = param.asInt();
  Serial.print("quatphongngu:");
  Serial.println(quatphongngu);
  mySerial.println("{\"Q2\":\"" + String(quatphongngu) + "\"}");
  mySerial.flush();
  TT_QUAT22 = quatphongngu;
  DataSendControl = true;
  LastSend = millis();
}
BLYNK_WRITE(V7)
{
  int quatphongan = param.asInt();
  Serial.print("quatphongan:");
  Serial.println(quatphongan);
  mySerial.println("{\"Q3\":\"" + String(quatphongan) + "\"}");
  mySerial.flush();
  TT_QUAT33 = quatphongan;
  DataSendControl = true;
  LastSend = millis();
}
BLYNK_WRITE(V15)
{
  int dieukhiencua = param.asInt();
  Serial.print("dieukhien door:");
  Serial.println(dieukhiencua);
  mySerial.println("{\"OPENCLOSE\":\"" + String(dieukhiencua) + "\"}");
  mySerial.flush();
  LastSend = millis();
}



/*
  tài khoản blynk
  nguoidungblynk2@gmail.com
  @Aa123456789
*/

void tick()
{
  //toggle state
  int state = digitalRead(LED_IO2);  // get the current state of GPIO1 pin
  digitalWrite(LED_IO2, !state);     // set pin to the opposite state
}
void configModeCallback (WiFiManager *myWiFiManager)
{
  Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
  ticker.attach(0.2, tick);
}


void setup()
{
  Serial.begin(115200);
  mySerial.begin(9600);
  //WiFi.disconnect();
  pinMode(LED_IO2, OUTPUT);

  ticker.attach(0.2, tick);
  WiFiManager wifiManager;
  wifiManager.EC_begin();
  wifiManager.EC_read();
  wifiManager.setAPCallback(configModeCallback);
  wifiManager.autoConnect("ESPWIFICONFIG", "12345678");
  Blynk.begin(auth, ssid, pass);
  ticker.detach();
  timeClient.begin();
  //----------------------------------
  timerID1 = timer.setInterval(1000L, handleTimerID1);
  timerID2 = timer.setInterval(10000L, handleTimerID2);
  timer.disable(timerID2);
  //----------------------------------
  LastSend = millis();
  times = millis();

}

void loop()
{

  //----------MODE CANH BAO-------------------------
  timer.run();

  //----------MODE CANH BAO-------------------------
  Blynk.run();
  YeuCauSTM32SendData();
  Read_STM32();
  BLynkConnect();

}


//----------MODE CANH BAO-------------------------

void handleTimerID1() {
  String s ;
  if (TT_CUA == 0) {
    s = "CỬA ĐANG ĐÓNG!";
  }
  else if (TT_CUA == 1)
  {
    s = "CỬA ĐANG MỞ!";
  }
  
  Blynk.virtualWrite(V16, s);
  //Serial.println("Trạng thái cửa: " + String(TT_CUA));
  if (runMode == 1) {
    if (TT_IR == 1) {
      if (canhbao == 0) {
        canhbaoState = !canhbaoState;
        Blynk.virtualWrite(V18, canhbaoState);
        timer.enable(timerID2);
        timer.restartTimer(timerID2);
        canhbao = 1;
      }
    }
  }
}

void handleTimerID2() {
  canhbaoState = !canhbaoState;
  Blynk.virtualWrite(V18, canhbaoState);
}
BLYNK_WRITE(V17) {
  runMode = param.asInt();
  Serial.print("RunMode:");
  Serial.println(runMode);
  mySerial.println("{\"MODE\":\"" + String(runMode) + "\"}");
  mySerial.flush();
  TT_MODEE = runMode;
  DataSendControl = true;

  if (runMode == 0) {
    timer.disable(timerID2);
  }
  else
  {
    canhbao = 0;
  }
  LastSend = millis();
}
BLYNK_WRITE(V18) {
  canhbaoState = param.asInt();
}
BLYNK_CONNECTED() {
  Blynk.syncVirtual(V17, V18);
}


//----------MODE CANH BAO-------------------------


void YeuCauSTM32SendData(void)
{
  if (millis() -  LastSend >= 4000)
  {
    if (Blynk.connected())
    {
      Serial.println("Send Lệnh!!!");
      mySerial.println("{\"S\":\"1\"}");
      mySerial.flush();
      Serial.println("======================================================");
    }
    LastSend = millis();
  }

}
void BLynkConnect(void)
{
  if (millis() - times > 1000)
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      if (Blynk.connected())
      {
        //ND = random(0, 100);
        //DA = random(0, 100);
        //MQ2 = random(0, 1000);
        //Blynk.virtualWrite(V8, ND);
        //Blynk.virtualWrite(V9, DA);
        //Blynk.virtualWrite(V10, MQ2);
        blinkLedBlynk();
        ControlLed();
        delay(10);
      }
    }
    times = millis();
  }
}

String ReadTime()
{
  timeClient.update();
  Time = timeClient.getFormattedTime();
  return Time;
}
void Read_STM32(void)
{
  while (mySerial.available())
  {
    LastSend = millis();
    const size_t capacity = JSON_OBJECT_SIZE(2) + 256;
    DynamicJsonDocument JSON(capacity);
    DeserializationError error = deserializeJson(JSON, mySerial);
    if (error)
    {
      return;
    }
    else
    {
      Serial.println("Data nhận được STM32 là:");
      serializeJsonPretty(JSON, Serial);
      Serial.println();
      // ND , DA , D1 , D2 , D3 , D4 , Q1 , Q2 , Q3 , COI , CUA, IR, MODE(canh bao)
      if (JSON.containsKey("ND"))
      {
        String Data_ND = JSON["ND"];
        Serial.print("ND:");
        Serial.println(Data_ND);
        ND = Data_ND.toInt();
      }
      if (JSON.containsKey("DA"))
      {
        String Data_DA = JSON["DA"];
        Serial.print("DA:");
        Serial.println(Data_DA);
        DA = Data_DA.toInt();
      }
      if (JSON.containsKey("D1"))
      {
        String Data_D1 = JSON["D1"];
        Serial.print("Data_D1:");
        Serial.println(Data_D1);
        TT_DEN1 = Data_D1.toInt();
      }
      if (JSON.containsKey("D2"))
      {
        String Data_D2 = JSON["D2"];
        Serial.print("Data_D2:");
        Serial.println(Data_D2);
        TT_DEN2 = Data_D2.toInt();
      }
      if (JSON.containsKey("D3"))
      {
        String Data_D3 = JSON["D3"];
        Serial.print("Data_D3:");
        Serial.println(Data_D3);
        TT_DEN3 = Data_D3.toInt();
      }
      if (JSON.containsKey("D4"))
      {
        String Data_D4 = JSON["D4"];
        Serial.print("Data_D4:");
        Serial.println(Data_D4);
        TT_DEN4 = Data_D4.toInt();
      }
      if (JSON.containsKey("Q1"))
      {
        String Data_Q1 = JSON["Q1"];
        Serial.print("Data_Q1:");
        Serial.println(Data_Q1);
        TT_QUAT1 = Data_Q1.toInt();
      }
      if (JSON.containsKey("Q2"))
      {
        String Data_Q2 = JSON["Q2"];
        Serial.print("Data_Q2:");
        Serial.println(Data_Q2);
        TT_QUAT2 = Data_Q2.toInt();
      }
      if (JSON.containsKey("Q3"))
      {
        String Data_Q3 = JSON["Q3"];
        Serial.print("Data_Q3:");
        Serial.println(Data_Q3);
        TT_QUAT3 = Data_Q3.toInt();
      }
      if (JSON.containsKey("COI"))
      {
        String Data_COI = JSON["COI"];
        Serial.print("Data_COI:");
        Serial.println(Data_COI);
        TT_COI = Data_COI.toInt();
      }
      if (JSON.containsKey("CUA"))
      {
        String Data_CUA = JSON["CUA"];
        Serial.print("Data_CUA:");
        Serial.println(Data_CUA);
        TT_CUA = Data_CUA.toInt();
      }
      if (JSON.containsKey("IR"))
      {
        String Data_IR = JSON["IR"];
        Serial.print("Data_IR:");
        Serial.println(Data_IR);
        TT_IR = Data_IR.toInt();
      }
      if (JSON.containsKey("MODE"))
      {
        String Data_MODE = JSON["MODE"];
        Serial.print("Data_MODE:");
        Serial.println(Data_MODE);
        TT_MODE = Data_MODE.toInt();
      }


      if (JSON.containsKey("MQ2"))
      {
        String Data_MQ2 = JSON["MQ2"];
        Serial.print("Data_MQ2:");
        Serial.println(Data_MQ2);
        MQ2 = Data_MQ2.toInt();
      }

      if (Blynk.connected())
      {
        if (DataSendControl)
        {
          Blynk.virtualWrite(V1, TT_DEN11);
          Blynk.virtualWrite(V2, TT_DEN22);
          Blynk.virtualWrite(V3, TT_DEN33);
          Blynk.virtualWrite(V4, TT_DEN44);
          Blynk.virtualWrite(V5, TT_QUAT11);
          Blynk.virtualWrite(V6, TT_QUAT22);
          Blynk.virtualWrite(V7, TT_QUAT33);
          Blynk.virtualWrite(V8, ND);
          Blynk.virtualWrite(V9, DA);
          Blynk.virtualWrite(V10, MQ2);
          Blynk.virtualWrite(V11, TT_COI);// led chay
          Blynk.virtualWrite(V12, TT_IR);//led cua
          Blynk.virtualWrite(V13, TT_IR);//canh bao cua
          Blynk.virtualWrite(V14, TT_COI);//canh bao chay
          Blynk.virtualWrite(V17, TT_MODEE);
          Serial.println("======================================================");
          Serial.println("SEND Data BLYNK OK!!!");
          Serial.println("======================================================");
          DataSendControl = false;
        }
        else
        {
          Blynk.virtualWrite(V1, TT_DEN1);
          Blynk.virtualWrite(V2, TT_DEN2);
          Blynk.virtualWrite(V3, TT_DEN3);
          Blynk.virtualWrite(V4, TT_DEN4);
          Blynk.virtualWrite(V5, TT_QUAT1);
          Blynk.virtualWrite(V6, TT_QUAT2);
          Blynk.virtualWrite(V7, TT_QUAT3);
          Blynk.virtualWrite(V8, ND);
          Blynk.virtualWrite(V9, DA);
          Blynk.virtualWrite(V10, MQ2);
          Blynk.virtualWrite(V11, TT_COI);// led chay
          Blynk.virtualWrite(V12, TT_IR);//led cua
          Blynk.virtualWrite(V13, TT_IR);//canh bao cua
          Blynk.virtualWrite(V14, TT_COI);//canh bao chay
          Blynk.virtualWrite(V17, TT_MODE);
          Serial.println("======================================================");
          Serial.println("SEND Data NEW BLYNK OK!!!");
          Serial.println("======================================================");
          DataSendControl = false;
        }


      }

      JSON.clear();
      LastSend = millis();
    }
  }

}

void blinkLedBlynk()
{
  if (led_connect.getValue())
  {
    led_connect.off();
  }
  else
  {
    led_connect.on();
  }
}

void ControlLed()
{
  digitalWrite(LED_IO2, !digitalRead(LED_IO2));
}
