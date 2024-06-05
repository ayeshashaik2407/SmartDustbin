#include <ESP32Servo.h>
#include <NewPing.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

#define SERVO_PIN1     26 
#define ECHO_PIN1      19 //servo
#define TRIGGER_PIN1   18
#define MAX_DISTANCE   200
#define ECHO_PIN2      21
#define TRIGGER_PIN2   5

NewPing sonar1(TRIGGER_PIN1, ECHO_PIN1, MAX_DISTANCE); 
NewPing sonar2(TRIGGER_PIN2, ECHO_PIN2, MAX_DISTANCE); 
Servo servoMotor1;

// WiFi network station credentials
#define WIFI_SSID "XXXXXXXXXX"
#define WIFI_PASSWORD "XXXXXXXX"

// Telegram BOT Token (Get from Botfather)
#define BOT_TOKEN "XXXXXXXX:XXXXXXXXXXXXXX"
bool flag1=0;
WiFiClientSecure secured_client;
UniversalTelegramBot bot(BOT_TOKEN, secured_client);

// Variable to track if the initial message has been sent
bool initialMessageSent = false;
bool filled = false;

void setup() 
{
    Serial.begin(9600);
    servoMotor1.attach(SERVO_PIN1);
    
    // Connect to WiFi
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    secured_client.setCACert(TELEGRAM_CERTIFICATE_ROOT);

    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(500);
    }

    Serial.println("\nWiFi connected");
     servoMotor1.write(0);
}

void loop() 
{
    
   
   // servoMotor2.write(0);
    delay(50);
    Serial.println("Sonar 1: ");
    Serial.println(sonar1.ping_cm()); 
    Serial.println("Sonar 2: ");
    Serial.println(sonar2.ping_cm()); 

    // Ultra sonic code for opening and closing door 
    if (sonar1.ping_cm() <= 30 &&(sonar1.ping_cm()>0))
    {
        delay(500);
        if(!flag1)
        {
          servoMotor1.write(120);
          flag1=1;
          delay(2000);
        }
    }
    else
    {
        flag1=0;
        servoMotor1.write(0);
        //servoMotor2.write(0);
        Serial.print("close");
        delay(2000);
    }
    
    // Ultrasonic code for detecting waste
    if (sonar2.ping_cm() <=30)
    {
        if ((sonar2.ping_cm() != 0) && (!flag1))
        {
            // Send initial message quickly
            if (!initialMessageSent)
            {
                bot.sendMessage("XXXXXXX", "please clean me");
                Serial.println("please clean me (initial message)");
                initialMessageSent = false;
                filled =true;
            }
        }
        
    }

    // Delay for 5 minutes (300,000 milliseconds) after the initial message
  
    else if(sonar2.ping_cm() >30)
    {
        
        if(filled)
        {
                bot.sendMessage("XXXXXXXXXXXXX", "Thanks for cleaning");
                initialMessageSent = false;
                filled=false;
            //}

        }
    }
    // 
}
