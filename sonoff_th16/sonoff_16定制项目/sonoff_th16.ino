/* sonoff_th16 Example

   This example code is Contains the GPIO,relay,WIFIfunction test.

   According to the following start each test module:
   1.By default, after power on test GPIO4 and GPIO14 ,if functional put LED-GPIO13 normally on, or flashes 2 times.
   2.After try connect to ap,if succeed,put out the LED-GPIO13.
   3.Then press the button-GPIO0, LED-GPIO12 and relay will reverse it.
*/
#include <Ticker.h>
#include <Ultrasonic.h>
#include <ESP8266WiFi.h>

Ticker debounce_handle;
Ticker led_flashing;

int key_status = 1;
int key_backup = 1;

int down_time = 0;
int up_time = 0;

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
               return 1;
         }
         else
         {
               Serial.println("Connect to YangHuaiMi Failed");
         }
         #endif
    } 

    return 0;
}

int check(unsigned char a,unsigned char b)
{
  
  pinMode(a,OUTPUT);
  pinMode(b,INPUT);
  digitalWrite(a,HIGH);
  if(digitalRead(b) == LOW)
    return 0;
  digitalWrite(a,LOW);
  if(digitalRead(b) == HIGH)
    return 0;
    
  pinMode(b,OUTPUT);
  pinMode(a,INPUT);
  digitalWrite(b,HIGH);
  if(digitalRead(a) == LOW)
    return 0;
  digitalWrite(b,LOW);
  if(digitalRead(a) == HIGH)
    return 0;
  
    return 1;
}

void key_debounch_handle()
{
    if(digitalRead(0)==0)
    {
        down_time++;
    }
    else
    {
        up_time++;
    }

    if(down_time > 20)
    {
        down_time = 0;
        key_status = 0;
    }
    if(up_time > 20)
    {
        up_time = 0;
        key_status = 1;
    }
}

void setup() {
    Serial.begin(9600);
    pinMode(13,OUTPUT);//led pwm1
    pinMode(12,OUTPUT);//led pwm0
    pinMode(0,INPUT);//key
//    pinMode();
    debounce_handle.attach_ms(2,key_debounch_handle);
    #if 1
    if(check(14,4)==1)
    {
        digitalWrite(13,LOW);
        //digitalWrite(12,HIGH);
    }
    else
    {
        digitalWrite(13,LOW);
        //digitalWrite(12,HIGH);
        delay(500);
        digitalWrite(13,HIGH);
        //digitalWrite(12,LOW);
        delay(500);
        digitalWrite(13,LOW);
        //digitalWrite(12,HIGH);
        delay(500);
        digitalWrite(13,HIGH);
        //digitalWrite(12,LOW);
        delay(500);
        digitalWrite(13,LOW);
        //digitalWrite(12,HIGH);
    }

    if(wifi_test()==1)
    {
        digitalWrite(13,HIGH);
    }
    #endif
}

void loop() {
  int sta = 0;

  while(1)
  {
      if(key_status != key_backup)
      {
          if(key_status == 0)
          {
              sta = ~sta;
              digitalWrite(12,sta);
          }
          key_backup = key_status;
      }
      delay(1);
  }
}
