/*
 * graphicsClasses.h
 *
 *  Created on: Nov 12, 2018
 *      Author: Riya Simon
 */
#include <string>
#include <vector>
#ifndef GRAPHICSCLASSES_H_
#define GRAPHICSCLASSES_H_

class PVector{
public:
int x, y;
PVector(int x, int y);
};

class Button{
public:
  int x, y, dx, dy; // defines location and size of button
  unsigned long background, text;  // defines background and text color of button
  char* title = "Button"; // what the button says
  Button(int x, int y, int dx, int dy, unsigned long colorBackground, unsigned long colorText, char* title);
  void draw();
  bool check(PVector a);
  bool checkWithTitle(PVector a, std::string title);
};

class Element{
public:
    //Element(char* title);
    int dx = 0; // width of element
    int dy = 0; // height of element
    char* title = "hello";

};  // creates an array of Elements, collectively making up a NavBar.



class Nav_bar{
  /******************************************/
  /* "Look and feel" variables and defualts */
  /******************************************/
public:
  PVector* p = new PVector(0,25);      // position of Nav_Bar
  PVector* size = new PVector(-1,-1); // Calculated based on elements within. Speficies the region on the screen that is used for rendering the Nav_bar.
  int padding = 25;  // padding. Default is 10.
  //char orientation = 'h'; // 'h' is horazontal, 'v' is vertical.
  //bool fixed = true;   // is the spacing fixed, or is it based on the length of the character? (Has no effect when orientation is vertical.)
  bool banner = false; // if true, the navBar will be drawn as a banner that streches across the screen. p.x or p.y gets ignored depending on orientation.
  unsigned long bg = BLACK;
  unsigned long txt = WHITE;
  int textSize = 12;
  int selectedElement = 0; // assume first element is selected (this is basically like a radio button selection.)
  Element elements[7];
  Nav_bar(int size, char* element_titles[]);
  void createNavBar();
  void draw(char* day);
  void erase();
  bool check(PVector a);
};

class Keyboard{
  public:
   //Button keys = new Button[64]; // allocate the memory
  Button keys[];
  Keyboard();
  Keyboard(int i);
  void draw();
  void drawColonKeyboard();
};

class Clock_display{
   int hour,  minute,  second,
     phour, pminute, psecond;

   int x, y;

   Clock_display(int x, int y);
   void draw();
 };


#endif /* GRAPHICSCLASSES_H_ */
