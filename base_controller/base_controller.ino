/* base_controler Example

   This example code is Contains the GPIO, buzzer,WIFI,uart,18b20 function test.

   According to the following start each test module:
   1.By default, after power on RGBled began to shine,then according to the following test sequence
   2.RGB GPIO->BUZZER->nextion power->nextion uart->18b20->wifi->soft_reset->RGB GPIO.
   3.When the nextion uart working properly, a red light will flash.
   4.When the temperature sensor working properly, a green light will flash.
   5.When the wifi connect to ap, a blue light will flash.
*/
#include <DS18B20.h>
#include <Ticker.h>
#include <ESP8266WiFi.h>

DS18B20 ds18b20(4);

void RGB_led_open(int num)
{
    switch(num)
    {
        case 2:
          digitalWrite(2,HIGH);
          digitalWrite(14,LOW);
          digitalWrite(16,LOW);
          break;
        case 14:
          digitalWrite(2,LOW);
          digitalWrite(14,HIGH);
          digitalWrite(16,LOW);
          break;
        case 16:
          digitalWrite(2,LOW);
          digitalWrite(14,LOW);
          digitalWrite(16,HIGH);
          break;
        case 0:
          digitalWrite(2,HIGH);
          digitalWrite(14,HIGH);
          digitalWrite(16,HIGH);
          break;
        default:
          digitalWrite(2,LOW);
          digitalWrite(14,LOW);
          digitalWrite(16,LOW);
          break;
    }
}

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

int read_respond(char *rece_buf, int len)
{
  long timeout = 0;
  int read_cnt = 0;
  int addr_diff = 0;
  int position = 0;
  
  while (1)
  { 
    if (Serial.available() > 0)
    {
      timeout = 0;
      rece_buf[position++] = Serial.read();
      //for(int i=0;i<10000;i++);
    } 
    else
    {
      timeout++;
      delay(1);
      if (timeout > 100)
      {
        if (rece_buf[0] == 0)
        {
          //Serial.write("no data read\n");
          Serial.flush();
          return 0;
        }
          return position;
      }
    }
  }
}

void RGB_test()
{
    RGB_led_open(2);
    delay(1000);
    RGB_led_open(14);
    delay(1000);
    RGB_led_open(16);
    delay(1000);
    RGB_led_open(100);
    delay(1000);
}

void buzzer_test(void)
{
    digitalWrite(5, 1); 
    delay(1000);
    digitalWrite(5, 0);
    delay(1000); 
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
                      RGB_led_open(14);
                      delay(1000);
                      RGB_led_open(100);
                      delay(2000);
                      return 1;
              }
        }
        ds18b20.start_ds18b20_tran();
        delay(1000);
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
               RGB_led_open(16);
               delay(1000);
               RGB_led_open(100);
               delay(2000);
               return 1;
         }
         else
         {
               Serial.println("Connect to 8DB0839D Failed");
         }
         //#else
         if(ret==0)
         {
                     ret = connect_to_wifi((char *)"YangHuaiMi",(char*)"TianShuiTeChan");
                   if(ret == 1)
                   {
                         Serial.println("Connect to YangHuaiMi OK");
                         WiFi.disconnect();
                         Serial.println("Close to YangHuaiMi OK");
                         RGB_led_open(14);
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
}

void nextion_power_test()
{
    digitalWrite(12,LOW);
    delay(1000);
    digitalWrite(12,HIGH);
    delay(2000);
}

void reset_test()
{
    RGB_led_open(0);
    delay(2000);
    digitalWrite(13,LOW);
    delay(500);
}

int nextion_uart_test()
{
    uint8_t tail[3] = {0xFF,0xFF,0xFF};
    char read_buf[20] = {0};
    int len = 0;

    for(int i=0;i<6;i++)
    {
        Serial.flush();
        Serial.print("get baud");
        Serial.write((const char*)tail,3);
        Serial.flush();
    
        len = read_respond(read_buf,20);
        if(len > 0)
        {
            if((read_buf[1] == 0x80) && (read_buf[2]==0x25))
            {
                Serial.print("nextion uart test OK");
                RGB_led_open(2);
                delay(1000);
                RGB_led_open(100);
                delay(2000);
                return 1;
            }
            else if(len == 8)
            {
                return 1;
            }
        }
        delay(1000);
    }
}

void setup() {
    Serial.begin(9600);
    pinMode(2,OUTPUT);  //red
    pinMode(14,OUTPUT); //green
    pinMode(16,OUTPUT); //blue
    pinMode(5,OUTPUT);//buzzer

    //pinMode(12,INPUT);
    pinMode(12,OUTPUT);//nextion power
    digitalWrite(12,HIGH);
    digitalWrite(13,HIGH);
    pinMode(13,OUTPUT);//reset
    
    ds18b20_init();
}

void loop() 
{
    while(1)
    {
         RGB_test();
         buzzer_test();
         nextion_power_test();  
         nextion_uart_test();
         ds18b20_test();
         wifi_test();
         reset_test();
         delay(2000);  
    }
}
