/*
 * MP3_Play_Test.ino
 * A quick start example for Grove-Serial MP3 Player V2.0
 * Note: The MP3 chip of Grove-Serial MP3 Player V2.0 is different from Grove-Serial MP3 Player V1.0
 * Description: This demo let you can send instruction 1-8 to control the Grove-Serial MP3 Player, via the serial port.
 *
 * Copyright (c) 2015 seeed technology inc.
 * Website    : www.seeed.cc
 * Author     : Wuruibin
 * Created Time: Dec 2015
 * Modified Time:
 * 
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <SoftwareSerial.h>
#include <MP3Player_KT403A.h>

#include <DS1302.h>
DS1302 rtc(9, 8, 7);

// Note: You must define a SoftwareSerial class object that the name must be mp3, 
//       but you can change the pin number according to the actual situation.
SoftwareSerial mp3(3, 2);

#define BRIGHTNESS_THRESHOLD 500

void setup()
{
    rtc.halt(false);
    rtc.writeProtect(false);
    
    mp3.begin(9600);
    Serial.begin(9600); 
    delay(100);
    
    SelectPlayerDevice(0x02);       // Select SD card as the player device.
    SetVolume(0x13);                // Set the volume, the range is 0x00 to 0x1E.


    
    rtc.setDOW(TUESDAY);
    rtc.setTime(0, 4, 0);
    rtc.setDate(1, 11, 2017);
    
}

void loop()
{
    /*
    Serial.print(rtc.getDOWStr());
    Serial.print(" ");
    Serial.print(rtc.getDateStr());
    Serial.print(" -- ");
    Serial.println(rtc.getTimeStr());
    */
    
    char recvChar = 0;
    while(Serial.available())
    {
        recvChar = Serial.read() - '0';
    }
    Serial.print("Send: ");
    Serial.println( recvChar );

    /*
    if (recvChar >= 0) {
      SpecifyMusicPlay(recvChar);
    }*/

    /*
    switch (recvChar)
    {
        case '1':
            SpecifyMusicPlay(0);
            Serial.println("Specify the music index to play");
            break;
        case '2':
            PlayPause();
            Serial.println("Pause the MP3 player");
            break;
        case '3':
            PlayResume();
            Serial.println("Resume the MP3 player");
            break;
        case '4':
            PlayNext();
            Serial.println("Play the next song");
            break;
        case '5':
            PlayPrevious();
            Serial.println("Play the previous song");
            break;
        case '6':
            PlayLoop();
            Serial.println("Play loop for all the songs");
            break;
        case '7':
            IncreaseVolume();
            Serial.println("Increase volume");
            break;
        case '8':
            DecreaseVolume();
            Serial.println("Decrease volume");
            break;
        default:
            break;
    }*/

    Time t = rtc.getTime();

    Serial.print(t.hour);
    Serial.print(" ");
    Serial.print(t.min);
    Serial.print(" ");
    Serial.println(t.sec);
    
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
    
    //SpecifyMusicPlay(1);

    prev_brightness = brightness;
    delay(1000);
    
//    printReturnedData();
}

boolean isLightTurnedOn(int brightness) {
  if (brightness > BRIGHTNESS_THRESHOLD) {
    return false;
  } else {
    return true;
  }
}

