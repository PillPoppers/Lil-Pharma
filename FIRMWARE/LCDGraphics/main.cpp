// Driverlib includes
#include "hw_types.h"

// Custom libraries
#include "lcd.h"
#include "util.h"
#include "pinmux.h"
#include "graphicsClasses.h"
#include <vector>
#include <string>

void SetDC();
void InitializeLCD();
void ResetDC();
void welcomeScreen();
void setUpPin();
void adminScreen();
void enterPin();
void setUpAlarms(char* day);
void homePage();
void alarmRinging();

static void
BoardInit(void)
{
    PRCMCC3200MCUInit();
}

void delay(){
    for (int i = 0; i < 1000; i++);
}


void main(){
    // Initialize Board configurations
    BoardInit();

    // Mind the order: 1) PIN 2) LCDComm 3) LCD (this is dealt by nested calls)

    // Dependent on
    SPIPinMuxConfig();
    InitializeLCDCommunication();
    InitializeLCD();


    //fillScreen(WHITE);
    //Button b(0, 0, 200, 100, WHITE, BLACK, "buttons");
    //b.draw();
    //fillScreen(WHITE);
    //Keyboard k;
    //k.draw();
    //setup0();
    setUpAlarms("SUN");
    //alarmRinging();
    //std::vector<char*> vec;
    //vec.push_back("Hello");
    //Nav_bar bar(0, 0, 30, vec, 'h', true);
    //bar.draw();


    //fillRect(0,0,310, 200, RED);
    //char array[] = "Blue";
    //writeText(50, 50, 10, array, BLACK, WHITE);

    while(1)
    {
        //writeCommand(DISPOFF);
        //writeParameter(0x07);
       SetDC();
       //UtilsDelay(1000);
       delay();
       ResetDC();
       delay();
      // UtilsDelay(1000);


    }

}

void welcomeScreen(){

  fillRect(0,0,100,320,TEAL);
  fillRect(100,0,100,320,TEAL);
  fillRect(200,0,100,320,TEAL);
  fillRect(300,0,100,320,TEAL);
  fillRect(400,0,80,320,TEAL);

  writeText(50, 100, 6, "Welcome", BLACK, TEAL);
  writeText(50, 150, 3, "Press CONTINUE to", BLACK, TEAL);
  writeText(50, 170, 3, "configure your device.", BLACK, TEAL);
  Button b(330, 230, 150, 40 , MAGENTA, BLACK, "CONTINUE");
  b.draw();
}

void setUpPin(){

  fillRect(0,0,100,320,TEAL);
  fillRect(100,0,100,320,TEAL);
  fillRect(200,0,100,320,TEAL);
  fillRect(300,0,100,320,TEAL);
  fillRect(400,0,80,320,TEAL);
  writeText(55, 70, 3, "Welcome", BLACK, TEAL);
  writeText(55, 100, 3, "Setup your admin pin!", BLACK, TEAL);
  fillRect(70,140,200,50,WHITE);
  Button b(280, 140, 170, 50 , MAGENTA, BLACK, "CONTINUE");
  b.draw();

  Keyboard k;
  k.draw();

}

void adminScreen(){

  fillRect(0,0,100,320,TEAL);
  fillRect(100,0,100,320,TEAL);
  fillRect(200,0,100,320,TEAL);
  fillRect(300,0,100,320,TEAL);
  fillRect(400,0,80,320,TEAL);
  writeText(55, 70, 5, "Admin Page", BLACK, TEAL);

  Button b(20, 150, 230, 50 , GREEN, BLACK, "UNLOCK");
  Button c(260, 150, 230, 50 , GREEN, BLACK, "EDIT ALARMS");
  b.draw();
  c.draw();


}

void enterPin(){

  fillRect(0,0,100,320,TEAL);
  fillRect(100,0,100,320,TEAL);
  fillRect(200,0,100,320,TEAL);
  fillRect(300,0,100,320,TEAL);
  fillRect(400,0,80,320,TEAL);
  writeText(55, 70, 3, "Enter Pin", BLACK, TEAL);
  fillRect(70,140,200,50,WHITE);
  Button b(280, 140, 170, 50 , MAGENTA, BLACK, "CONTINUE");
  b.draw();

  Keyboard k;
  k.draw();

}

void setUpAlarms(char* day){

    fillRect(0,0,100,320,TEAL);
    fillRect(100,0,100,320,TEAL);
    fillRect(200,0,100,320,TEAL);
    fillRect(300,0,100,320,TEAL);
    fillRect(400,0,80,320,TEAL);

    //writeText(10, 10, 3, "Set up alarms!", BLACK, TEAL);
    fillRect(10,10,250,40,WHITE);
    writeText(10, 10, 3, "Set up alarms!", BLACK, WHITE);
    Button b(20, 100, 100, 40 , GREEN, BLACK, "MORN");
    b.draw();
    fillRect(130,100,100,40,WHITE);
    Button c(240, 100, 100, 40, GREEN, BLACK, "NOON");
    c.draw();
    fillRect(350,100,100,40,WHITE);
    Button d(20, 150, 100, 40 , GREEN, BLACK, "EVE");
    d.draw();
    fillRect(130,150,100,40,WHITE);
    Button e(240, 150, 100, 40, GREEN, BLACK, "NIGHT");
    e.draw();
    fillRect(350,150,100,40,WHITE);
    Button g(290, 0, 200, 50 , MAGENTA, BLACK, "CONTINUE");
    g.draw();
    Keyboard k(1);
    k.drawColonKeyboard();
    char* days[] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
    Nav_bar n(13, days);
    n.draw(day);
}

void homePage(){
   fillRect(0,0,100,320,TEAL);
   fillRect(100,0,100,320,TEAL);
   fillRect(200,0,100,320,TEAL);
   fillRect(300,0,100,320,TEAL);
   fillRect(400,0,80,320,TEAL);

  Button b(20, 150, 100, 40 , GREEN, BLACK, "MORN");
  b.draw();
  fillRect(130,150,100,40,WHITE);
  Button c(240, 150, 100, 40, GREEN, BLACK, "NOON");
  c.draw();
  fillRect(350,150,100,40,WHITE);
  Button d(20, 200, 100, 40 , GREEN, BLACK, "EVE");
  d.draw();
  fillRect(130,200,100,40,WHITE);
  Button e(240, 200, 100, 40, GREEN, BLACK, "NIGHT");
  e.draw();
  fillRect(350,200,100,40,WHITE);
  Button g(290, 0, 200, 50 , MAGENTA, BLACK, "CONTINUE");
  g.draw();
  Button h(100, 0, 150, 50 , MAGENTA, BLACK, "ADMIN");
  h.draw();


}

void alarmRinging(){
   fillRect(0,0,100,320,TEAL);
   fillRect(100,0,100,320,TEAL);
   fillRect(200,0,100,320,TEAL);
   fillRect(300,0,100,320,TEAL);
   fillRect(400,0,80,320,TEAL);


  Button g(290, 0, 200, 50 , MAGENTA, BLACK, "HOME");
  g.draw();
  Button h(100, 0, 150, 50 , MAGENTA, BLACK, "ADMIN");
  h.draw();
  writeText(55, 130, 4, "Time to take", BLACK, TEAL);
  writeText(55, 170, 4, "your pills!", BLACK, TEAL);
  Button i(100, 230, 200, 70 , GREEN, BLACK, "DISPENSE");
  i.draw();
}


