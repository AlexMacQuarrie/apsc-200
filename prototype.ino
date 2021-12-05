#include <SoftwareSerial.h>

SoftwareSerial ble(11, 10);

int val = 0;
bool pillTaken = true;

int hour, minute;
int targetHour, targetMinute;

unsigned int lastTime = millis();
unsigned int delayBetween = 500;
//Should be 1000 for 1 second, might want to change to 60*1000 to increment every minute instead

void setup()
{
  pinMode(A0, INPUT);
  pinMode(13, OUTPUT);
  ble.begin(9600);

  ble.println("Welcome to the HoldSmart Medical Edition Demo!");
  ble.println("For the purposes of the demo, the clock will run significantly faster\n\n");

  ble.print("Enter the target hour (24h): ");
  while (ble.available() == 0) {}
  targetHour = ble.parseInt();

  while (ble.available() == 0) {}
  ble.parseInt();

  ble.print("Enter the target minute: ");
  while (ble.available() == 0) {}
  targetMinute = ble.parseInt();

  while (ble.available() == 0) {}
  ble.parseInt();

  ble.print("Target time is : ");
  ble.print(targetHour);
  ble.print(":");
  ble.println(targetMinute);
  ble.println();

  ble.print("Enter the current hour (24h): ");
  while (ble.available() == 0) {}
  hour = ble.parseInt();
  
  while (ble.available() == 0) {}
  ble.parseInt();

  ble.print("Enter the current minute: ");
  while (ble.available() == 0) {}
  minute = ble.parseInt();
}

void loop()
{
  unsigned int timeNow = millis();
  if (timeNow - lastTime > delayBetween)
  {
    if (minute == 60)
    {
      hour++;
      minute = 0;
    }
    if (hour == 4)   hour = 0;

    val = analogRead(A0);
    
    ble.print("Time is: ");
    ble.print(hour);
    ble.print(":");
    ble.print(minute);
    ble.print("\t");
    ble.println(val);

    if (pillTaken == 0 && val == 0)
    {
        ble.println("Thank you for taking your medication!");
        digitalWrite(13, LOW);
        pillTaken = true;
    }
    if (pillTaken == 0 && val != 0)
    {
        ble.println("Please take your medication as soon as possible");
        digitalWrite(13, HIGH);
    }


    if (hour == targetHour && minute == targetMinute)
    {   
        if (val > 0)
        {
          ble.println("Please take your medication");
          digitalWrite(13, HIGH);
          pillTaken = false;
          
        }
        else
        {
          ble.println("You have already taken your medication!");
          digitalWrite(13, LOW);
          pillTaken = true;
        }
    }
    
    minute++;
    lastTime = timeNow;
  }
}
