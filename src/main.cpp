#include <Arduino.h>
#include "WiFi.h"

#define BLINKER_WIFI
#define BLINKER_MIOT_LIGHT
#include <Blinker.h>

char auth[] = "e351f69e58c6"; //换成APP获取到的密匙
char ssid[] = "kb123";
char pswd[] = "12345678";
// char ssid[] = "Xiaomi_A1C9_BOX";
// char pswd[] = "15854862130";
int led1 = 18;
int led2 = 19;
int netled=12;
bool wsState;

int Nuanguang = 100;
int Lengguang = 100;
float num = 2.2;

int freq = 500;   // 频率
int channel1 = 0; // 通道
int channel2 = 1;
int resolution = 8;  // 分辨�?
uint8_t colorW = 50; //默�?�亮度为50，防�?�?瞎眼�?

//#define BUTTON_1 "ButtonKey"
#define keyz "Keyz" //总开关键�?
#define nuanguang "ran-v37"
#define lengguang "ran-f33"

//BlinkerButton Button1(BUTTON_1);
BlinkerButton ZONG_KAIGUAN(keyz);
BlinkerButton NUANGUANG_HUAKUAI(nuanguang);
BlinkerButton LENGGUANG_HUAKUAI(lengguang);

uint8_t wsMode = BLINKER_CMD_MIOT_DAY;

void keyz_callback(const String &state) //总开关回�?
{
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    BLINKER_LOG("get button state: ", state);
    //当按下总开关时两组�?都亮
    if (state == BLINKER_CMD_ON)
    {
        BLINKER_LOG("Toggle on!");

        Serial.printf("on\n");
        Serial.printf("Nuanguang=%d\n", Nuanguang);
        Serial.printf("Lengguang=%d\n", Lengguang);
        ledcWrite(channel1, Nuanguang * num);
        ledcWrite(channel2, Lengguang * num);

        //Button1.icon("icon_56");
        ZONG_KAIGUAN.color("#1FFFFF");
        ZONG_KAIGUAN.text("ON");
        // Button1.text("Your button name", "describe");
        ZONG_KAIGUAN.print("on");
    }
    else if (state == BLINKER_CMD_OFF)
    {
        BLINKER_LOG("Toggle off!");

        Serial.printf("off\n");
        ledcWrite(channel1, 0);
        ledcWrite(channel2, 0);
        // Button1.icon("icon_32");
        ZONG_KAIGUAN.color("#FFF0FF");
        ZONG_KAIGUAN.text("OFF");
        // Button1.text("Your button name", "describe");
        ZONG_KAIGUAN.print("off");
    }
}

void nuanguang_huakuai_callback(const String &state)
{
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    BLINKER_LOG("Get user setting: ", state);

    Nuanguang = state.toInt();

    Serial.printf("Nuanguang=%d\n", Nuanguang);
    ledcWrite(channel2, Nuanguang * num);
    Serial.printf("nuanguang on\n");
    // Button1.icon("icon_1");
    // Button1.color("#FFFFFF");
    //Button1.text("Your button name or describe");
    // Button1.text("Your button name", "describe");
}

void lengguang_huakuai_callback(const String &state)
{
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    BLINKER_LOG("Get user setting: ", state);

    Lengguang = state.toInt();

    Serial.printf("Lengguang=%d\n", Lengguang);

    ledcWrite(channel1, Lengguang * num);
    Serial.printf("lengguang on\n");
    // Button1.icon("icon_1");
    // Button1.color("#FFFFFF");
    //Button1.text("Your button name or describe");
    // Button1.text("Your button name", "describe");
}
// void button1_callback(const String & state)
// {
//     digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
//     BLINKER_LOG("get button state: ", state);

//     if (state == BLINKER_CMD_BUTTON_TAP) {
//         BLINKER_LOG("Button tap!");
//         Serial.printf("tap\n");
//         Button1.icon("icon_1");
//         Button1.color("#FFFFFF");
//         Button1.text("切换");
//         Button1.print();
//     }
//     else if (state == BLINKER_CMD_BUTTON_PRESSED) {
//         BLINKER_LOG("Button pressed!");

//         Serial.printf("press\n");
//         // Button1.icon("icon_1");
//         // Button1.color("#FFFFFF");
//         // Button1.text("Your button name or describe");
//         Button1.print();
//     }
//     else if (state == BLINKER_CMD_BUTTON_RELEASED) {
//         BLINKER_LOG("Button released!");

//         Serial.printf("pressup\n");

//         Button1.icon("icon_1");
//         Button1.color("#FFFFFF");
//         Button1.text("Your button name or describe");
//         // Button1.text("Your button name", "describe");
//         Button1.print();
//     }
//     else if (state == BLINKER_CMD_ON) {
//         BLINKER_LOG("Toggle on!");

//         Serial.printf("on\n");
//         ledcWrite(channel1, 200);
//         ledcWrite(channel2,200);

//         //Button1.icon("icon_56");
//         Button1.color("#1FFFFF");
//         Button1.text("ON");
//         // Button1.text("Your button name", "describe");
//         Button1.print("on");
//     }
//     else if (state == BLINKER_CMD_OFF) {
//         BLINKER_LOG("Toggle off!");

//         Serial.printf("off\n");
//          ledcWrite(channel1, 0);
//         ledcWrite(channel2,0);
//        // Button1.icon("icon_32");
//         Button1.color("#FFF0FF");
//         Button1.text("OFF");
//         // Button1.text("Your button name", "describe");
//         Button1.print("off");
//     }
//     else {
//         BLINKER_LOG("Get user setting: ", state);

//         Serial.printf("state:%s\n",state);
//         Button1.icon("icon_1");
//         Button1.color("#FFFFFF");
//         Button1.text("Your button name or describe");
//         // Button1.text("Your button name", "describe");
//         Button1.print();
//     }
// }

void miotPowerState(const String &state)
{
    BLINKER_LOG("need set power state: ", state);

    if (state == BLINKER_CMD_ON)
    {
        digitalWrite(LED_BUILTIN, HIGH);

        BlinkerMIOT.powerState("on");
        BlinkerMIOT.print();
        Serial.printf("ceshideng on\n");
        ledcWrite(channel1, Nuanguang * num);
        ledcWrite(channel2, Lengguang * num);
    }
    else if (state == BLINKER_CMD_OFF)
    {
        digitalWrite(LED_BUILTIN, LOW);
        Serial.printf("ceshideng off\n");

        ledcWrite(channel1, 0);
        ledcWrite(channel2, 0);

        BlinkerMIOT.powerState("off");
        BlinkerMIOT.print();
    }
}

// void miotColor(int32_t color)
// {
//     BLINKER_LOG("need set color: ", color);

//     colorR = color >> 16 & 0xFF;
//     colorG = color >>  8 & 0xFF;
//     colorB = color       & 0xFF;

//     BLINKER_LOG("colorR: ", colorR, ", colorG: ", colorG, ", colorB: ", colorB);
//     Serial.printf("color:%d\n",color);

//     ledcWrite(channel1, colorB*colorW*0.041); //colorw为亮度值，colorB为蓝�?数值（0-255）esp8266的pwm�?0-1024，换算一下公�?
//     ledcWrite(channel2, colorR*colorW*0.041);

//     BlinkerMIOT.color(color);
//     BlinkerMIOT.print();
// }

void miotMode(uint8_t mode)
{
    BLINKER_LOG("need set mode: ", mode);

    if (mode == BLINKER_CMD_MIOT_DAY)
    {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_NIGHT)
    {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_COLOR)
    {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_WARMTH)
    {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_TV)
    {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_READING)
    {
        // Your mode function
    }
    else if (mode == BLINKER_CMD_MIOT_COMPUTER)
    {
        // Your mode function
    }

    wsMode = mode;

    BlinkerMIOT.mode(mode);
    BlinkerMIOT.print();
}

void miotBright(const String &bright)
{
    BLINKER_LOG("need set brightness: ", bright);
    colorW = bright.toInt();
    BLINKER_LOG("now set brightness: ", colorW);
    Serial.printf("colorW:%d!\n", colorW);
    Nuanguang = Lengguang = colorW;
    Serial.printf("Nuanguang:%d!\n", Nuanguang);
    Serial.printf("Lengguang:%d!\n", Lengguang);
    Serial.printf("Lengguang&Nuanguang all on\n");
    ledcWrite(channel1, Nuanguang * num);
    ledcWrite(channel2, Lengguang * num);

    BlinkerMIOT.brightness(colorW);
    BlinkerMIOT.print();
}

void miotColoTemp(int32_t colorTemp)
{
    BLINKER_LOG("need set colorTemperature: ", colorTemp);

    Serial.printf("colorTemp:%d!\n", colorTemp);
    if (colorTemp > 5000 && colorTemp <= 10000)
    {
        ledcWrite(channel2, 0);
        ledcWrite(channel1, Nuanguang * num);
        //  delay(1000);
        //  for(int i=0;i<512;i++)
        //  {
        //       ledcWrite(channel1, i);
        //       delay(10);
        //       Serial.printf("i=%d",i);
        //  }

        //  ledcWrite(channel1, Nuanguang);
        //ledcWrite(channel2,0);
        Serial.printf("lengguang on,nuanguang off\n");
    }
    else if ((colorTemp >= 1000 && colorTemp <= 4999))
    {

        ledcWrite(channel2, Lengguang * num);
        ledcWrite(channel1, 0);
        Serial.printf("nuanguang on,lenguang off\n");
    }

    BlinkerMIOT.colorTemp(colorTemp);
    BlinkerMIOT.print();
}

void miotQuery(int32_t queryCode)
{
    BLINKER_LOG("MIOT Query codes: ", queryCode);

    switch (queryCode)
    {
    case BLINKER_CMD_QUERY_ALL_NUMBER:
        BLINKER_LOG("MIOT Query All");
        BlinkerMIOT.powerState(wsState ? "on" : "off");
        BlinkerMIOT.color(0);
        BlinkerMIOT.mode(0);
        BlinkerMIOT.colorTemp(1000);
        BlinkerMIOT.brightness(1);
        BlinkerMIOT.print();
        break;
    case BLINKER_CMD_QUERY_POWERSTATE_NUMBER:
        BLINKER_LOG("MIOT Query Power State");
        BlinkerMIOT.powerState(wsState ? "on" : "off");
        BlinkerMIOT.print();
        break;
    case BLINKER_CMD_QUERY_COLOR_NUMBER:
        BLINKER_LOG("MIOT Query Color");
        BlinkerMIOT.color(0);
        BlinkerMIOT.print();
        break;
    case BLINKER_CMD_QUERY_MODE_NUMBER:
        BLINKER_LOG("MIOT Query Mode");
        BlinkerMIOT.mode(0);
        BlinkerMIOT.print();
        break;
    case BLINKER_CMD_QUERY_COLORTEMP_NUMBER:
        BLINKER_LOG("MIOT Query ColorTemperature");
        BlinkerMIOT.colorTemp(1000);
        BlinkerMIOT.print();
        break;
    case BLINKER_CMD_QUERY_BRIGHTNESS_NUMBER:
        BLINKER_LOG("MIOT Query Brightness");
        BlinkerMIOT.brightness(1);
        BlinkerMIOT.print();
        break;
    default:
        BlinkerMIOT.powerState(wsState ? "on" : "off");
        BlinkerMIOT.color(0);
        BlinkerMIOT.mode(0);
        BlinkerMIOT.colorTemp(1000);
        BlinkerMIOT.brightness(1);
        BlinkerMIOT.print();
        break;
    }
}

void dataRead(const String &data)
{
    BLINKER_LOG("Blinker readString: ", data);

    Blinker.vibrate();

    uint32_t BlinkerTime = millis();

    Blinker.print("millis", BlinkerTime);
}
void mySmartWifiConfig()
{
  WiFi.mode(WIFI_MODE_STA);
  Serial.println("������������:");
  WiFi.beginSmartConfig();
  while (1)
  {
    Serial.print(".");
    delay(500);
    if (WiFi.smartConfigDone())
    {
      Serial.println("�����ɹ�");
      Serial.printf("SSID:%s", WiFi.SSID().c_str());
      Serial.printf("PSW:%s", WiFi.psk().c_str());
      break;
    }
  }
}

bool autoConfig()
{
  WiFi.disconnect(true,true);
  WiFi.begin();
  for (size_t i = 0; i < 20; i++)
  {
    int wifiStatus = WiFi.status();
    if (wifiStatus == WL_CONNECTED)
    {
      Serial.println("�Զ����ӳɹ�!");
      return 1;
    }
    else
    {
      delay(1000);
      Serial.println("�ȴ��Զ�������...");
    }
  }
  Serial.println("�޷��Զ�����!");
  return 0;
}

void setup()
{

    Serial.begin(115200);
    // BLINKER_DEBUG.stream(Serial);
    //BLINKER_DEBUG.debugAll();

    if (!autoConfig())
    {
        mySmartWifiConfig();
    }
   
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    pinMode(netled, OUTPUT);
    digitalWrite(netled, LOW);
    
    ledcSetup(channel1, freq, resolution); // 设置通道
    ledcAttachPin(led1, channel1);         // 将通道与�?�应的引脚连�?
    ledcSetup(channel2, freq, resolution); // 设置通道
    ledcAttachPin(led2, channel2);         // 将通道与�?�应的引脚连�?

    Blinker.begin(auth, WiFi.SSID().c_str(), WiFi.psk().c_str());
    //Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);

    BlinkerMIOT.attachPowerState(miotPowerState);
    //BlinkerMIOT.attachColor(miotColor);
    BlinkerMIOT.attachMode(miotMode);
    BlinkerMIOT.attachBrightness(miotBright);
    BlinkerMIOT.attachColorTemperature(miotColoTemp);
    BlinkerMIOT.attachQuery(miotQuery);
    //Button1.attach(button1_callback);
    ZONG_KAIGUAN.attach(keyz_callback);
    NUANGUANG_HUAKUAI.attach(nuanguang_huakuai_callback);
    LENGGUANG_HUAKUAI.attach(lengguang_huakuai_callback);
    ledcWrite(channel1, Nuanguang * num);
    ledcWrite(channel2, Lengguang * num);
    //WiFi.softAPsetHostname("esp32");

    // Serial.print("������:");
    // Serial.println(WiFi.softAPgetHostname());
    // Serial.print("����IP:");
    // Serial.println(WiFi.softAPIP());
    // Serial.print("����IPV6:");
    // Serial.println(WiFi.softAPIPv6());
    // Serial.print("����SSID:");
    // Serial.println(WiFi.SSID());
    // Serial.print("�����㲥IP:");
    // Serial.println(WiFi.softAPBroadcastIP());
    // Serial.print("����mac��ַ:");
    // Serial.println(WiFi.softAPmacAddress());
    // Serial.print("�������Ӹ���:");
    // Serial.println(WiFi.softAPgetStationNum());
    // Serial.print("��������ID:");
    // Serial.println(WiFi.softAPNetworkID());
    // Serial.print("����״̬:");
    // Serial.println(WiFi.status());
    // delay(1000);

    // Serial.print("�Ƿ�����:");
    // Serial.println(WiFi.isConnected());
    // Serial.print("����IP:");
    // Serial.println(WiFi.localIP());
    // Serial.print("����IPv6:");
    // Serial.println(WiFi.localIPv6());
    // Serial.print("mac��ַ:");
    // Serial.println(WiFi.macAddress());
    // Serial.print("����ID:");
    // Serial.println(WiFi.networkID());
    // Serial.print("��Ϣ:");
    // Serial.println(WiFi.getSleep());
    // Serial.print("��ȡ״̬��:");
    // Serial.println(WiFi.getStatusBits());
    // Serial.print("getTxPower:");
    // Serial.println(WiFi.getTxPower());
    // Serial.print("�Ƿ��Զ�����:");
    // Serial.println(WiFi.getAutoConnect());
    // Serial.print("�Ƿ��Զ�����:");
    // Serial.println(WiFi.getAutoReconnect());
    // Serial.print("��ȡģʽ:");
    // Serial.println(WiFi.getMode());
    // Serial.print("��ȡ������:");
    // Serial.println(WiFi.getHostname());
    // Serial.print("��ȡ����IP:");
    // Serial.println(WiFi.gatewayIP());
    // Serial.print("dnsIP:");
    // Serial.println(WiFi.dnsIP());
    // Serial.print("״̬:");
    // Serial.println(WiFi.status());
    // delay(1000);
}

void loop()
{
    if (!WiFi.isConnected())
  {
    digitalWrite(netled, LOW);
    Serial.println("����δ����");
    delay(1000);
  }
  else digitalWrite(netled, HIGH);
    Blinker.run();
}

// /**********************************************************************************************************/
// // int freq = 2000;    // 频率
// // int channel = 0;    // 通道
// // int resolution = 8;   // 分辨�?

// // const int led = 18;
// // void setup()
// // {

// //   ledcSetup(channel, freq, resolution); // 设置通道
// //   ledcAttachPin(led, channel);  // 将通道与�?�应的引脚连�?
// // }

// // void loop()
// // {
// //   // 逐渐变亮
// //   for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle = dutyCycle + 1)
// //   {
// //     ledcWrite(channel, dutyCycle);  // 输出PWM
// //     delay(20);
// //   }

// //   // 逐渐变暗
// //   for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle = dutyCycle - 1)
// //   {
// //     ledcWrite(channel, dutyCycle);  // 输出PWM
// //     delay(20);
// //   }
// // }