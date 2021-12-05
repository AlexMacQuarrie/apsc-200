#include <SoftwareSerial.h>     //Library used for bluetotth serial communication

SoftwareSerial ble(11, 10);     //Sets pins 11 & 10 as TX and RX serial pins

int val = 0;                    //Declaration of global varaibles
bool pillTaken = true;

int hour, minute;
int targetHour, targetMinute;

unsigned int lastTime = millis();     //Used for clock timing
unsigned int delayBetween = 60*1000;  //Sets a 1 minute delay for the clock

void setup()    //Runs once
{
  pinMode(A0, INPUT);     //Sets pins and starts bluetooth serial communication
  pinMode(13, OUTPUT);
  ble.begin(9600);

  ble.println("Welcome to the HoldSmart Medical Edition");

  ble.print("Enter the target hour (24h): ");     //Prompts user to input the targetted hour
  while (ble.available() == 0) {}                 //Gets user input from bluetooth terminal
  targetHour = ble.parseInt();

  while (ble.available() == 0) {}                 //Fixes bluetooth input duplication bug
  ble.parseInt();

  ble.print("Enter the target minute: ");
  while (ble.available() == 0) {}                 //Gets user input from bluetooth terminal
  targetMinute = ble.parseInt();

  while (ble.available() == 0) {}                 //Fixes bluetooth input duplication bug
  ble.parseInt();

  ble.print("Target time is : ");                 //Displays the inputted target time
  ble.print(targetHour);
  ble.print(":");
  ble.println(targetMinute);
  ble.println();

  ble.print("Enter the current hour (24h): ");
  while (ble.available() == 0) {}                 //Gets user input from bluetooth terminal
  hour = ble.parseInt();
  
  while (ble.available() == 0) {}                 //Fixes bluetooth input duplication bug
  ble.parseInt();

  ble.print("Enter the current minute: ");
  while (ble.available() == 0) {}                 //Gets user input from bluetooth terminal
  minute = ble.parseInt();
}

void loop()   //Loops infinetly
{
  unsigned int timeNow = millis();                //Gets the current time
  
  if (timeNow - lastTime > delayBetween)          //Ensures the loop runs only once every minute
  {
    if (minute == 60)                             //Makes times like x:60 become (x+1):0 for correct clock implementation
    {
      hour++;
      minute = 0;
    }
    if (hour == 24)   hour = 0;                   //Ensures hour runs from 0 to 24 

    val = analogRead(A0);                         //Reads from the force sensor as a 10 but value

    if (pillTaken == 0 && val == 0)                                     //Runs if the medication was not taken and still has not been taken
    {
      ble.println("Thank you for taking your medication!");             //Tells the user that they have now taken the medication
      digitalWrite(13, LOW);                                            //Turns the LED off
      pillTaken = true;                                                 //Sets that the medication has now been taken
    }
    if (pillTaken == 0 && val != 0)                                     //Runs if the medication was not taken but has now been taken
    {
      ble.println("Please take your medication as soon as possible");   //Prompts the user to take their medication
      digitalWrite(13, HIGH);                                           //Turns the LED on
    }

    if (hour == targetHour && minute == targetMinute) //Runs if it is the targetted time
    {
      if (val > 0)                                    //Runs if there is weight on the force sensor
      {
        ble.println("Please take your medication");   //Prompts user to take their medication
        digitalWrite(13, HIGH);                       //Turns the LED on
        pillTaken = false;                            //Sets that the medication has not been taken
      }
      else                                            //Runs if there is no weight on the force sensor
      {
        ble.println("You have already taken your medication!");   //Tells user they have already taken their medication
        digitalWrite(13, LOW);                                    //Turns off the LED
        pillTaken = true;                                         //Sets that the medication has been taken
      }
    }
    
    minute++;                 //Increments the minute
    lastTime = timeNow;       //Sets the time the last interation ran at
  }
}
