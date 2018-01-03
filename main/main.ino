#include <LiquidCrystal.h>
LiquidCrystal lcd (11, 10, 9, 8, 7, 6); 

#include <NewPing.h>

long unsigned time; 
long unsigned btnTimer = 0; 
double seconds = 0; 
int minutes = 0, hours = 0; 
int minutesA = 0, hoursA = 0; 
String secondsS, minutesS, hoursS;
String minutesSA, hoursSA; 
String meridiem = "AM", meridiemA = "AM";  
String timer, alarm; 
boolean twelveHour;  
int buttonState = LOW;    
boolean turnedLOW = false; 
boolean timeSet = false; 
boolean alarmSet = false; 
String alarmStatus = "OFF"; 
int counter = 0; 
boolean hold = false; 
int numMenu = 4; 
int subMenu = 10; 
int minuteCorrector = 0; 
long distanceTime = 800; 
long distanceSeconds = 0; 
long duration = 0; 

int buttonPin1 = 2; 
int motor1A = 4; 
int motor2A = 3; 
int motor12EN = 5; 
int speaker = 14; 
int trig = 12; 
int echo = 13;   
#define MAX_DISTANCE 200
 
NewPing sonar(trig, echo, 100);

int readButton ()
{ 
  if (digitalRead (buttonPin1) == HIGH && buttonState == LOW)
   { 
     buttonState = HIGH;
   }
   
   else if (digitalRead (buttonPin1) == LOW && buttonState == HIGH)
   { 
    buttonState = LOW; 
    turnedLOW = true; 
   }

   return buttonState; 
}
void setup() 
{
  Serial.begin (9600); 
  
  pinMode (buttonPin1, INPUT); 
  pinMode (speaker, OUTPUT); 
  pinMode (motor1A, OUTPUT); 
  pinMode (motor2A, OUTPUT); 
  pinMode (motor12EN, OUTPUT);
  pinMode (trig, OUTPUT); 
  pinMode (echo, INPUT);
   
  lcd.begin (16, 2); 
  lcd.setCursor(0, 0);   
  time = millis(); 
  twelveHour = true; 
}

int calculateDistance ()
{
   duration = sonar.ping_cm(); 
   return (duration); 
}

void openWindow ()
{
  int distance = sonar.ping_cm(); 
   
    if (distance > 11 && seconds < 10)
    { 
        digitalWrite (motor1A, 1); 
        digitalWrite (motor2A, 0); 
        analogWrite (motor12EN, 180); 
    } 
    else if (seconds > 10 && seconds < 13)
    {
        digitalWrite (motor1A, 0); 
        digitalWrite (motor2A, 1); 
        analogWrite (motor12EN, 115); 
    }
    else
    { 
      digitalWrite (motor12EN, LOW); 
    }
}

void loop() 
{                
    turnedLOW = false;

    if (toAlarm() != -1 && alarmSet == false)
    { 
      if (toAlarm () == 0)
      { 
        soundAlarm(); 
        readButton (); 
        
        if (buttonState == LOW && turnedLOW == true)
        { 
          alarmStatus = "OFF"; 
          noTone (speaker);
        } 
      }
      else if (toAlarm() == 5)
      { 
        openWindow(); 
      }
      
    }   

    //If user is setting the time
    if (counter >= subMenu && alarmSet == false)
    { 
      timeSet = true; 
    }
    //User is not setting the time
    else 
    { 
      timeSet = false; 
    }
    
    //Comparing time to see if a second passed 
      if (millis() - time > 999)
      { 
        seconds++; 
        time += 1000;
      }
  
      //Increase minute after 60s pass
      if (timeSet == false)
      { 
        if (seconds >= 60)
        { 
          seconds = 0; 
          minutes++; 
          minuteCorrector++; 

          if (minuteCorrector % 16 == 0 && seconds == 0)
          { 
            seconds++; 
          }
        }
      } 
      
      //Increase hour after 60min pass
      if (minutes > 59)
      { 
        hours++;
        minutes = 0;  
      }
   
    if (hours == 12 && timeSet == false)
    { 
      meridiem = "PM"; 
    }
    else if (hours == 24)
    { 
      meridiem = "AM"; 
    }

    //Set hour back to 0 after 24
    if (hours > 24)
    { 
      hours = 1; 
      meridiem = "AM"; 
    }

    if (hoursA > 24)
    { 
      hoursA = 1; 
    }

    //Formatting output
    secondsS = String (int(seconds)); 
    minutesS = String (minutes); 
    minutesSA = String (minutesA); 

    if (hours >= 13)
    { 
      hoursS = String(hours - 12);
    } 
    else 
    { 
       hoursS = String(hours);
    }

    if (hoursA >= 13)
    { 
      hoursSA = String(hoursA - 12);
    } 
    else 
    { 
       hoursSA = String(hoursA);
    }
    
    if (secondsS.length() < 2)
    { 
      secondsS = "0" + secondsS; 
    }

    if (minutesS.length() < 2)
    { 
      minutesS = "0" + minutesS; 
    }

    if (minutesSA.length() < 2)
    { 
      minutesSA = "0" + minutesSA; 
    }

    timer = hoursS + ":" + minutesS + ":" + secondsS + " " + meridiem; 
    alarm = hoursSA + ":" + minutesSA + " " + meridiemA;  
    
   buttonState = readButton();

   if (buttonState == LOW && turnedLOW == true && counter < 10 && !hold)
   { 
    counter++;
   }
    
    if (counter > numMenu -1 && counter < 10)
     { 
       counter = 0; 
     }

   if (buttonState == LOW)
   { 
      btnTimer = millis(); 
      hold = false; 
   }

   //If button is held down for 1 seconds
   if ((millis() - btnTimer) > 1000)
     { 
        hold = true; 
        btnTimer = millis(); 

        if (counter == 2)
        { 
          alarmSet = true;
          counter = 10;  
          hoursA = 0; 
        }

        else if (counter == 3)
        { 
          if (alarmStatus == "ON ")
          { 
            alarmStatus = "OFF"; 
          }
          else 
          {
            alarmStatus = "ON "; 
          }
        }
        else if (counter == 1)
        { 
          alarmSet = false; 
          counter = 10;  
          hours = 0; 
        }

        else if (counter == 10)
        { 
          counter = 11;
        }

        else if (counter == 11)
        { 
          counter = 12; 
        }

        else if (counter == 12)
        { 
          if (alarmSet == false)
          { 
            if (meridiem == "PM" && hours != 12)
            {
               hours += 12; 
            }
            else if (meridiem == "AM" && hours == 12)
            { 
              hours+=12; 
            }
          } 

          else
          { 
             if (meridiemA == "PM" && hoursA != 12)
            {
               hoursA += 12; 
            }
            else if (meridiemA == "AM" && hoursA == 12)
            { 
              hoursA+=12; 
            }
          } 
          if (alarmSet == false)
          { 
            seconds = 0;
          } 
          counter = 0;
          alarmSet = false; 
        }
     }

   //Output to the LCD screen
   switch (counter)
   { 
      case 0: 
        lcd.setCursor (0,0); 
        lcd.print ("Shahzada Khoso <"); 
        lcd.setCursor (0, 1); 
        lcd.print (timer + "      ");
        break; 

      case 1: 
        lcd.setCursor (0,0); 
        lcd.print ("Shahzada Khoso    "); 
        lcd.setCursor (0, 1);
        if (hoursS.length () < 2)
        {  
          lcd.print (timer + "     <"); 
        } 
        else 
        { 
          lcd.print (timer + "    <"); 
        }
        break; 

      case 2:
        lcd.setCursor (0, 0); 
        lcd.print (timer + "     ");
        lcd.setCursor (0, 1); 
        lcd.print ("Alarm: " + alarm);   
        break;

      case 3:
        lcd.setCursor (0, 0);  
        lcd.print ("Alarm: " + alarm);   
        lcd.setCursor (0, 1); 
        lcd.print ("Alarm: " + alarmStatus + "     ");  
        break;

       case 10: 
       lcd.setCursor (0, 0); 
       lcd.print ("Hour            "); 
       lcd.setCursor (0, 1);

       if (!alarmSet)
       { 
         lcd.print (hoursS + "                "); 
       }
       else 
       { 
         lcd.print (hoursSA + "                "); 
       }
       setHour(alarmSet); 
       break; 

       case 11: 
       lcd.setCursor (0, 0); 
       lcd.print ("Minute            "); 
       lcd.setCursor (0, 1); 

       if (!alarmSet)
       { 
        lcd.print (minutesS + "                "); 
       } 
       else 
       { 
        lcd.print (minutesSA + "                ");
       }
       setMinute (alarmSet); 
       break; 

       case 12:
       lcd.setCursor (0, 0); 
       lcd.print ("AM or PM            "); 
       lcd.setCursor (0, 1);
       if (!alarmSet)
       {  
        lcd.print (meridiem + "                ");
       } 
       else 
       { 
         lcd.print (meridiemA + "                ");
       }
       setMeridiem (alarmSet); 
       break;  
   }    
}

   void setHour (boolean alarm)
   { 
     buttonState = readButton();   
    
    if (buttonState == LOW && turnedLOW == true)
    { 
      if (!alarmSet && !hold)
      { 
        hours++; 
        
        if (hours > 12)
        { 
          hours = 1; 
        }
      }
      else if (!hold)
      { 
        hoursA++;
         
        if (hoursA > 12)
        { 
          hoursA = 1; 
        }
      }

    }
   }

   void setMinute (boolean alarm)
   { 
     buttonState = readButton();   
    
    if (buttonState == LOW && turnedLOW == true)
    { 
       if (!alarm && !hold)
      { 
        minutes++;

         if (minutes > 59)
        { 
          minutes = 0; 
        }
      }
      else if (!hold)
      { 
        minutesA++; 

        if (minutesA > 59)
        { 
          minutesA = 0; 
        }
      }
    }
   }

   void setMeridiem (boolean alarm)
   { 
     buttonState = readButton();   
    
    if (buttonState == LOW && turnedLOW == true)
    { 
       if (!alarm && !hold)
       { 
        if (meridiem == "AM")
          { 
            meridiem = "PM"; 
          }
          else
          { 
            meridiem = "AM"; 
          }
       }
       else if (!hold)
       { 
        if (meridiemA == "AM")
          { 
            meridiemA = "PM"; 
          }
          else
          { 
            meridiemA = "AM"; 
          }
      }
    }
   }

   int toAlarm ()
   { 
      if (alarmStatus == "ON ")
      { 
       int timeToAlarm; 
       if (hoursA - hours < 0)
       { 
         timeToAlarm = (24 - (hours - hoursA))*60; 
       }
       else 
       { 
         timeToAlarm = (hoursA - hours)*60; 
       }

       timeToAlarm += minutesA - minutes; 

       return timeToAlarm; 
      } 
      else
      { 
        return -1;  
      }
   }

   void soundAlarm ()
   { 
      if (int (seconds) % 2 == 0)
      { 
        tone (speaker, 300); 
      }
      else
      { 
        noTone(speaker); 
       }
   } 

