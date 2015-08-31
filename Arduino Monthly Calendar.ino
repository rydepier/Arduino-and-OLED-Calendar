/********************************************************
Calendar
by Chris Rouse Aug 2015

Displays a calendar for the current month

Connections:

RTC and OLED Display in parallel
Vcc to Arduino 5 volts
Gnd to Arduino Gnd
SDA to Arduino pin A4 (UNO) or Mega pin 20
SCL to Arduino A5 (UNO) or Mega pin 21

***************************************************************/

 #include "U8glib.h"
 #include <SPI.h>
 #include <Wire.h>
 #include "RTClib.h" 
 // setup u8g object
 U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);	// I2C 
 //
 RTC_DS1307 RTC;
 //
 int startDay = 0; // Sunday's value is 0, Saturday is 6
 String week1 ="";
 String week2 ="";
 String week3 ="";
 String week4 ="";
 String week5 ="";
 int newWeekStart = 0; // used to show start of next week of the month
 char monthString2[37]= {"JanFebMarAprMayJunJulAugSepOctNovDec"};
 int  monthIndex2[122] ={0,3,6,9,12,15,18,21,24,27,30,33};
 char monthName2[3]="";
 int monthLength = 0;
 //


void setup() {
 Serial.begin(9600);
 // Setup RTC
 Wire.begin();
 RTC.begin();
 if (! RTC.isrunning()) {
   Serial.println("RTC is NOT running!");
  }
  // following line sets the RTC to the date & time this sketch was compiled  
  // un REM the line below to set clock, then re REM it
  // and upload this sketch again
  //RTC.adjust(DateTime(__DATE__, __TIME__));
  //
}

void loop() {
  
  u8g.firstPage();      
  do {
    drawCalendar(); 
  } while( u8g.nextPage() ); 

}
//
 void drawCalendar(){
  // display a full month on a calendar 
  u8g.setFont(u8g_font_profont12);
  u8g.drawStr(2,9, "Su Mo Tu We Th Fr Sa"); 
  // display this month
  DateTime now = RTC.now();
  //
  // get number of days in month
  if (now.month() == 1 || now.month() == 3 || now.month() == 5 || now.month() == 7 || now.month() == 8 || now.month() == 10 || now.month() == 12){
    monthLength = 31;
  }
  else {monthLength = 30;}
  if(now.month() == 2){monthLength = 28;}
  startDay = startDayOfWeek(now.year(),now.month(),1); // Sunday's value is 0
  // now build first week string
  switch (startDay){
    case 0:
      // Sunday
      week1 = " 1  2  3  4  5  6  7";
      break;
    case 1:
      // Monday
      week1 = "    1  2  3  4  5  6";
      break;      
     case 2:
      // Tuesday
      week1 = "       1  2  3  4  5";
      break;           
     case 3:
      // Wednesday
      week1 = "          1  2  3  4";
      break;  
     case 4:
      // Thursday
      week1 = "             1  2  3";
      break; 
     case 5:
      // Friday
 if(monthLength == 28 || monthLength == 30){week1 = "                1  2";}      
      if(monthLength == 31){week1 = "31              1  2";}      
      break; 
     case 6:
      // Saturday
      if(monthLength == 28){week1 = "                   1";}
      if(monthLength == 30){week1 = "30                 1";}      
      if(monthLength == 31){week1 = "30 31              1";}       
      
      break;           
  } // end first week
  newWeekStart = (7-startDay)+1;
  const char* newWeek1 = (const char*) week1.c_str();  
  u8g.drawStr(2,19,newWeek1); 
  // display week 2
  week2 ="";
  for (int f = newWeekStart; f < newWeekStart + 7; f++){
    if(f<10){
      week2 = week2 +  " " + String(f) + " ";
    }  
    else{week2 = week2 + String(f) + " ";}    
  }
  const char* newWeek2 = (const char*) week2.c_str();  
  u8g.drawStr(2,29,newWeek2); 
  // display week 3
  newWeekStart = (14-startDay)+1; 
  week3 ="";
  for (int f = newWeekStart; f < newWeekStart + 7; f++){
    if(f<10){
      week3 = week3 +  " " + String(f) + " ";
    }  
    else{week3 = week3 + String(f) + " ";}    
  }
  const char* newWeek3 = (const char*) week3.c_str();  
  u8g.drawStr(2,39,newWeek3);     
  // display week 4
  newWeekStart = (21-startDay)+1; 
  week4 ="";
  for (int f = newWeekStart; f < newWeekStart + 7; f++){
    if(f<10){
      week4 = week4 +  " " + String(f) + " ";
    }  
    else{week4 = week4 + String(f) + " ";}    
    }
   const char* newWeek4 = (const char*) week4.c_str();  
   u8g.drawStr(2,49,newWeek4); 
   // do we need a fifth week
   week5="";
   newWeekStart = (28-startDay)+1;   
   // is is February?
   if(newWeekStart > 28 && now.month() == 2){
   // do nothing unless its a leap year
     if (now.year()==(now.year()/4)*4){ // its a leap year
       week5 = "29";
     }       
   }
   else{ // print up to 30 anyway
     if(now.month() == 2){  // its February
       for (int f = newWeekStart; f < 29; f++){
         week5 = week5 + String(f) + " ";  
       }  
       // is it a leap year
       if (now.year()==(now.year()/4)*4){ // its a leap year
         week5 = week5 + "29";
       }        
     }
     else{
       for (int f = newWeekStart; f < newWeekStart+7; f++){
         week5 = week5 + String(f) + " ";
       }
       // are there 31 days
       if (monthLength == 31 && week5.length() <7){
         week5 = week5 + "31"; 
       } 
     } 
   }
   const char* newWeek5 = (const char*) week5.c_str();  
   u8g.drawStr(2,59,newWeek5);
   // To print to Serial Monitor un REM the following
   Serial.println("Su Mo Tu We Th Fr Sa");
   Serial.println(week1);  
   Serial.println(week2);  
   Serial.println(week3); 
   Serial.println(week4);    
   Serial.println(week5);  
   while(1);  // wait indefinitly
 } 
/********************************************************/

// calculate first day of month
int startDayOfWeek(int y, int m, int d){
  static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
  y -= m < 3;
  return (y +y/4 -y/100 + y/400 + t[m-1] + d)% 7; 
} 

/********************************************************/ 
