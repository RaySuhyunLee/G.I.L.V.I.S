#include <SoftwareSerial.h>
#include <MP3Player_KT403A.h>

#include <DS1302.h>
DS1302 rtc(9, 8, 7);

SoftwareSerial mp3(3, 2);

#define BRIGHTNESS_THRESHOLD 500

//#define DEBUG
#define RESET_TIME    // Uncomment this after setting time.

void setup()
{
    rtc.halt(false);
    rtc.writeProtect(false);
    
    mp3.begin(9600);
#ifdef DEBUG
    Serial.begin(9600); 
#endif
    delay(100);
    
    SelectPlayerDevice(0x02);       // Select SD card as the player device.
    SetVolume(0x13);                // 0x00 ~ 0x1E
   
#ifdef RESET_TIME
    rtc.setDOW(TUESDAY);
    rtc.setTime(0, 4, 0);
    rtc.setDate(1, 11, 2017);
#endif
}

void loop()
{
    Time t = rtc.getTime();

#ifdef DEBUG
    Serial.print(t.hour);
    Serial.print(" ");
    Serial.print(t.min);
    Serial.print(" ");
    Serial.println(t.sec);
#endif
    
    static int prev_brightness = BRIGHTNESS_THRESHOLD - 200;
    int brightness = analogRead(0);

    static int prev_dow = MONDAY;
    static int prev_dow2 = MONDAY;

    static boolean wakeupEvent = false;
    Serial.print("hi"); Serial.println(t.date);
    if (isLightTurnedOn(brightness) && !isLightTurnedOn(prev_brightness) && prev_dow != t.dow) { // light just turned on
      Serial.println("Light turned on!");
      prev_dow = t.dow;
      wakeupEvent = true;
      if (t.hour >= 6 && t.hour < 7) {
        SpecifyMusicPlay(7);
      } else if (t.hour == 7 && t.min < 30) {
        SpecifyMusicPlay(8);
      } else if (t.hour >= 7 && t.hour < 9) {
        SpecifyMusicPlay(9);
      } else if (t.hour >= 9 && t.hour < 11) {
        SpecifyMusicPlay(10);
      } else {
        wakeupEvent = false;
      }
    } else if (!isLightTurnedOn(brightness) && isLightTurnedOn(prev_brightness) && prev_dow2 != t.dow) {
      Serial.println("Light turned off!");
      prev_dow2 = t.dow;
      if (t.hour >= 23) {
        SpecifyMusicPlay(11);
      } else if (t.hour < 1) {
        SpecifyMusicPlay(12);
      } else if (t.hour < 3) {
        SpecifyMusicPlay(13);
      }
    } else if (!isLightTurnedOn(brightness)) {
      if (t.hour == 7 && t.min == 0 && t.sec == 0) {
        SpecifyMusicPlay(1);
      } else if (t.hour == 7 && t.min == 30 && t.sec == 0) {
        SpecifyMusicPlay(2);
      } else if (t.hour == 9 && t.min == 0 && t.sec == 0) {
        SpecifyMusicPlay(3);
      }
    } else if (isLightTurnedOn(brightness)) {
      if (t.hour == 23 && t.min == 0 && t.sec == 0) {
        SpecifyMusicPlay(4);
      } else if (t.hour == 0 && t.min == 0 && t.sec == 0) {
        SpecifyMusicPlay(5);
      } else if (t.hour == 1 && t.min == 0 && t.sec == 0) {
        SpecifyMusicPlay(6);
      }
    }

    if (wakeupEvent == true) {
      delay(6000);
      SpecifyMusicPlay(14);
      wakeupEvent = false;
    }
    
    prev_brightness = brightness;
    delay(1000);
}

boolean isLightTurnedOn(int brightness) {
  if (brightness > BRIGHTNESS_THRESHOLD) {
    return false;
  } else {
    return true;
  }
}
