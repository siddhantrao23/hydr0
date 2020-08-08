#include <Wire.h>
#include <RTClib.h>
#include <MaxMatrix.h>

// RTC Module glab variables
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Relay pin
int relay  = 5;

// LED Matrix
int DIN = 8;
int CS = 7;
int CLK = 6;
int maxInUse = 1;
MaxMatrix m(DIN, CS, CLK, maxInUse);

// setup code
void setup () 
{
  Serial.begin(9600);
  delay(3000); // wait for console opening

  pinMode(relay, OUTPUT);
  digitalWrite(relay, HIGH);

  m.init();
  m.setIntensity(8);
  m.clear();
  
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
  
  // Comment out below lines once you set the date & time.
  // Following line sets the RTC to the date & time this sketch was compiled
    // rtc.adjust(DateTime(F(_DATE), F(TIME_)));
  }
}

void lightAll() {
    m.setColumn(0, B11111111);
    m.setColumn(1, B11111111);
    m.setColumn(2, B11111111);
    m.setColumn(3, B11111111);
    m.setColumn(4, B11111111);
    m.setColumn(5, B11111111);
    m.setColumn(6, B11111111);
    m.setColumn(7, B11111111);
}

void printDate()
{
  Serial.println("Current Date & Time: ");
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();  
}

void loop () 
{
  DateTime now = rtc.now();
  printDate();
  
  int sec=now.second();
  if(sec == 0) {
    // turn on pump and light
    digitalWrite(relay, LOW);
    lightAll();
  } else if(sec == 15) {
    // turn off pump
    digitalWrite(relay, HIGH);
  } else if(sec == 30) {
    // turn off lights
    m.clear();
  }

  delay(1000);
}
