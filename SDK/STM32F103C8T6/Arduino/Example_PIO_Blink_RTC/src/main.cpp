#include <Arduino.h>
#include <STM32RTC.h>
STM32RTC &rtc = STM32RTC::getInstance();
uint8_t lastSecond = 0;

void setup()
{
  pinMode(PB2, OUTPUT);
  pinMode(PA0, INPUT_PULLDOWN);

  digitalWrite(PB2, HIGH);

  rtc.setClockSource(STM32RTC::LSE_CLOCK);
  rtc.begin();
  if (!rtc.isConfigured())
  {
    rtc.setTime(0, 0, 0);
    rtc.setDate(1, 1, 25);
  }

  digitalWrite(PB2, LOW);

  Serial.begin(115200);
}

// the loop function runs over and over again forever
void loop()
{
  uint8_t second = rtc.getSeconds();

  if (digitalRead(PA0) == HIGH)
  {
    digitalWrite(PB2, HIGH);
    delay(25);
    digitalWrite(PB2, LOW);
    delay(175);
  }
  else
  {
    if (second != lastSecond)
    {
      lastSecond = second;
      Serial.print(rtc.getDay());
      Serial.print("/");
      Serial.print(rtc.getMonth());
      Serial.print("/");
      Serial.print(rtc.getYear());
      Serial.print(" ");
      Serial.print(rtc.getHours());
      Serial.print(":");
      Serial.print(rtc.getMinutes());
      Serial.print(":");
      Serial.print(rtc.getSeconds());
      Serial.println(" - Hello World! - hello WeAct Studio!");

      digitalWrite(PB2, HIGH);
      delay(25);
      digitalWrite(PB2, LOW);
    }
  }
}