/* sensorbox Example

   This example code is Contains the GPIO, buzzer,WIFI,18b20 hc-sr04,laser,relay,function test.

   According to the following start each test module:
   1.By default, after power on RGBled began to shine,then according to the following test sequence
   2.RGB GPIO->relay->buzzer->hc-ssr04->ds18b20->wifi->RGB GPIO.
   3.When the hc-ssr04 working properly, a red light will flash.
   4.When the temperature sensor ds18b20 working properly, a green light will flash.
   5.When the wifi connect to ap, a blue light will flash.
*/

#include <DS18B20.h>
#include <Ticker.h>
#include <Ultrasonic.h>
#include <ESP8266WiFi.h>

#define ONE_WIRE_BUS 16

DS18B20 ds18b20(ONE_WIRE_BUS);
Ticker debounce_handle;
Ticker fun_handle;

uint8_t key_status = 1;
uint8_t key_backup = 1;
uint8_t fun_choice = 0;

void ds18b20_init(void)
{
    if(ds18b20.is_ds18b20()==0)
    {
       ds18b20.start_ds18b20_tran();
       Serial.print("18b20 is ready ok\r\n");
    }
    else
    {
       Serial.print("18b20 is no ready\r\n");
    }
}

void printMacAddress() {
  // the MAC address of your Wifi shield
      byte mac[6];
    
      // print your MAC address:
      WiFi.macAddress(mac);
      Serial.print("MAC: ");
      Serial.print(mac[5], HEX);
      Serial.print(":");
      Serial.print(mac[4], HEX);
      Serial.print(":");
      Serial.print(mac[3], HEX);
      Serial.print(":");
      Serial.print(mac[2], HEX);
      Serial.print(":");
      Serial.print(mac[1], HEX);
      Serial.print(":");
      Serial.println(mac[0], HEX);
}

void scan_wifi_init(void)
{
    if(WiFi.status() == WL_NO_SHIELD)
    {
        Serial.println("WiFi shield not present");
    }

    //String fv = WiFi.firmwareVersion();
    //Serial.println(fv);

    printMacAddress();
}

void printEncryptionType(int thisType) {
  // read the encryption type and print out the name:
  switch (thisType) {
    case ENC_TYPE_WEP:
      Serial.println("WEP");
      break;
    case ENC_TYPE_TKIP:
      Serial.println("WPA");
      break;
    case ENC_TYPE_CCMP:
      Serial.println("WPA2");
      break;
    case ENC_TYPE_NONE:
      Serial.println("None");
      break;
    case ENC_TYPE_AUTO:
      Serial.println("Auto");
      break;
  }
}

int connect_to_wifi(char *ssid,char *password)
{ 
      int out_time = 10;
      int status = 0;

      //Serial.println(ssid);
      //Serial.println(password);
      status = WiFi.begin(ssid,password);
      //Serial.print("status:");
      //Serial.println(status);
      while(1)
      {
          if(WiFi.status() == WL_CONNECTED)
          {
              return 1;
          }
          else
          {
              if(out_time == 0)
              {
                  return 0;
              }
              else
              {
                  delay(1000);
                  out_time--;
              }
          }
      }
}
#if 0
int wifi_test(void)
{
    int ret = 0, i = 0;
    char ssid_s[30] = {0};

    for(i=0;i<3;i++)
    {
        #if 1
        int numSsid = WiFi.scanNetworks();
        if (numSsid == -1) 
        {
            Serial.println("Couldn't get a wifi connection");
            return 0;
        }
        Serial.print("number of available networks:");
        Serial.println(numSsid);
        #endif
        
        for (int thisNet = 0; thisNet < numSsid; thisNet++)  
        {
          #if 0
            Serial.print(thisNet);
            Serial.print(") ");
            Serial.print(WiFi.SSID(thisNet));
            Serial.print("\tSignal: ");
            Serial.print(WiFi.RSSI(thisNet));
            Serial.print(" dBm");
            Serial.print("\tEncryption: ");
            printEncryptionType(WiFi.encryptionType(thisNet));
           #endif
    
           if(WiFi.SSID(thisNet).substring(0) == "8DB0839D")
          //if(1)
           {
                Serial.println("find wifi:8DB0839D");
                ret = connect_to_wifi((char *)"8DB0839D",(char*)"094FAFE8");
                if(ret == 1)
                {
                    Serial.println("Connect to 8DB0839D OK");
                    Serial.print("Signal: ");
                    //Serial.print(WiFi.RSSI(thisNet));
                    Serial.println(" dBm");
                    WiFi.disconnect();
                    Serial.println("Close to 8DB0839D OK");
                    RGB_led_open(1);
                    delay(1000);
                    RGB_led_open(100);
                    delay(100);
                    return 1;
                }
                else
                {
                    Serial.println("Connect to 8DB0839D Failed");
                }
           }
        }
    } 
}
#endif 

int wifi_test(void)
{
    int ret = 0, i = 0;
    char ssid_s[30] = {0};

    for(i=0;i<3;i++)
    {
        #if 1
         ret = connect_to_wifi((char *)"8DB0839D",(char*)"094FAFE8");
         if(ret == 1)
         {
               Serial.println("Connect to 8DB0839D OK");
               WiFi.disconnect();
               Serial.println("Close to 8DB0839D OK");
               RGB_led_open(1);
               delay(1000);
               RGB_led_open(100);
               delay(2000);
               return 1;
         }
         else
         {
               Serial.println("Connect to 8DB0839D Failed");
         }
         #else

         ret = connect_to_wifi((char *)"YangHuaiMi",(char*)"TianShuiTeChan");
         if(ret == 1)
         {
               Serial.println("Connect to YangHuaiMi OK");
               WiFi.disconnect();
               Serial.println("Close to YangHuaiMi OK");
               RGB_led_open(1);
               delay(1000);
               RGB_led_open(100);
               delay(2000);
               return 1;
         }
         else
         {
               Serial.println("Connect to YangHuaiMi Failed");
         }
         #endif
    } 
}

int ds18b20_test()
{
      int ret,temp,intT,decT;
      int i = 0 ,cnt = 0;

     for(i=0;i<3;i++)
     {
        ret = ds18b20.Get_18b20_temp(&temp);
        if(ret)
        {
              intT = temp >> 4;
              decT = temp & 0x0f;
              Serial.print("Temperature for the device 1 (index 0) is: ");
              Serial.print(intT); 
              Serial.print(".");
              if(decT<10)
              {
                  Serial.print(0);
              }
              Serial.println(decT); 
              if(intT > 0 && intT < 50)
              {
                      RGB_led_open(12);
                      delay(1000);
                      RGB_led_open(100);
                      delay(1000);
                      return 1;
              }
        }
        ds18b20.start_ds18b20_tran();
        delay(1000);
     }
}

void buzzer_test(void)
{
    digitalWrite(5, 1); 
    delay(1000);
    digitalWrite(5, 0); 
}

void relay_test(void)
{
   int sta = 1;
   int i = 0;

   digitalWrite(14, 1); 
   delay(1000); 
   digitalWrite(14, 0); 
   delay(1000); 
   digitalWrite(14, 1); 
   delay(1000); 
   digitalWrite(14, 0); 
   delay(500);
}

void HC_start()
{
     digitalWrite(0,HIGH);
     Serial.println("IO10 high"); 
     delayMicroseconds(10);
     digitalWrite(0,LOW);
     Serial.println("IO10 low");
}

int HC_SR04_test()
{
    unsigned long duration;
    float distance;
    int cnt = 0;

     for(int i=0;i<3;i++)
     {
         HC_start();
         duration = pulseIn(4,HIGH);
         if(duration > 117 && duration < 26470)
         {
              distance = (float)duration*340/2/1000000;
              Serial.print("ditance:");
              Serial.print(distance);
              Serial.println("m");
              if(duration > 292 && duration < 1755)//5-30cm
              {       
                      RGB_led_open(15);
                      delay(1000);
                      RGB_led_open(100);
                      delay(1000);
                      return 1;
              }
         }
         delay(1000);
     }
     
}

void RGB_led_open(int num)
{
    switch(num)
    {
        case 15:
          digitalWrite(15,HIGH);
          digitalWrite(12,LOW);
          digitalWrite(1,LOW);
          break;
        case 12:
          digitalWrite(15,LOW);
          digitalWrite(12,HIGH);
          digitalWrite(1,LOW);
          break;
        case 1:
          digitalWrite(15,LOW);
          digitalWrite(12,LOW);
          digitalWrite(1,HIGH);
          break;
        case 0:
          digitalWrite(15,HIGH);
          digitalWrite(12,HIGH);
          digitalWrite(1,HIGH);
          break;
        default:
          digitalWrite(15,LOW);
          digitalWrite(12,LOW);
          digitalWrite(1,LOW);
          break;
    }
}

void RGB_test()
{
    //delay(1000);
    RGB_led_open(15);
    laser_test();
    delay(1000);
    RGB_led_open(12);
    laser_test();
    delay(1000);
    RGB_led_open(1);
    laser_test();
    delay(1000);
}

void laser_test()
{
     static int sta = 0;

    digitalWrite(2, sta); 
    sta = ~sta;
}

void setup() {
  //Serial.begin(9600);
  pinMode(5, OUTPUT); //buzzer
  pinMode(0, OUTPUT); //trig
  pinMode(4, INPUT);  //echo

  pinMode(15, OUTPUT); //red
  pinMode(12, OUTPUT);//green
  pinMode(16, OUTPUT); //blue,tx
  pinMode(14, OUTPUT);//relay
  pinMode(2,OUTPUT);//laser
  pinMode(1,OUTPUT);//
  digitalWrite(13,HIGH);
  pinMode(13,OUTPUT);
  ds18b20_init();
}

void loop() 
{
    while(1)
    {
        if(fun_choice==0)
        {
            RGB_test();
            RGB_led_open(100);
            fun_choice++;
        }
        else if(fun_choice==1)
        {
            relay_test();
            fun_choice++;
        }
        else if(fun_choice==2)
        {
            buzzer_test();
            fun_choice++;
        }
        else if(fun_choice==3)
        {
            HC_SR04_test();
            fun_choice++;
        }
        else if(fun_choice==4)
        {
            ds18b20_test();
            fun_choice++;
        }
        else if(fun_choice==5)
        {
            wifi_test();
            fun_choice++;
        }
        else if(fun_choice==6)
        {
             RGB_led_open(0);
             delay(3000);
             digitalWrite(0,HIGH);
             digitalWrite(13,LOW);
             delay(500);
        }
    }           
}

void check_time()
{
    static uint8_t start_flag = 0;
    Serial.println("come in");
    #if 0
    if(start_flag == 0)
    {
         if(digitalRead(4)==1)
         {
            
            start_flag = 1;
         }
    }
    else
    {
      
    }
    #endif
}

