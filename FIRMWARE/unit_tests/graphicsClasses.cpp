#include "LCD.h"
#include <string>
#include <vector>
#include <ctime>
#include <iostream>

// library-specific includes:
#include "graphicsClasses.h"
#include "fonts.h"


using namespace std;

PVector::PVector(int x, int y){
    this -> x = x;
    this -> y = y;
}

Button::Button(int x, int y, int dx, int dy, unsigned long colorBackground, unsigned long colorText, char* title){
    this -> x = x;   this -> y = y;
    this -> dx = dx; this -> dy = dy;
    this -> title = title;
    this -> background = colorBackground;
    this -> text = colorText;
};


// redraws the button. Should never need to use it after button is created.
void Button::draw(){

    //fillScreen(background);
    fillRect(x,y,dx,dy,background);
    writeText(x+10, y+10, 3, this->title, HX8357_BLACK, this-> background);

    // textAlign(CENTER, CENTER);
    // textSize(25);
    //fill(txt);
    // text(title, x + dx/2,y + dy/2);
}

  // gets should get called when an event occurs. Returns whether button was pressed.
bool Button::check(Point a){
    return ( (a.x > x) && (a.x < (x+dx)) && (a.y > y) && (a.y < (y+dy)));
}

bool Button::checkWithTitle(Point a, string title){
    return ( (a.x > x) && (a.x < (x+dx)) && (a.y > y) && (a.y < (y+dy)) && this->title == title);
}
/*
Element::Element(){
    this->title = title;
    this->dx = 0;
    this->dy = 0;
}

*/
Keyboard::Keyboard(){
    //int nextLineIndex
    vector< vector<string> > keyTitleLower;//= {{ "1", "2", "3", "4", "5", ":"}, {"6", "7", "8", "9", "0", "Ã¢â€ Â�", "X"}};
    char* firstLine[6];
    char* secondLine[6];
    firstLine[0] = "0";
    firstLine[1] = "1";
    firstLine[2] = "2";
    firstLine[3] = "3";
    firstLine[4] = "4";
    firstLine[5] = "5";
    secondLine[0] = "6";
    secondLine[1] = "7";
    secondLine[2] = "8";
    secondLine[3] = "9";
    secondLine[4] = "X";
    secondLine[5] = "<-";

    //keyTitleLower.push_back(firstLine);
    // keyTitleLower.push_back(secondLine);

    //hardcoded in width and height!
   int n = 0;
   for(int i = 0; i < 6; i++){
        Button b( 55+i*65+(55-35), 210, 45, 50, RED, WHITE, firstLine[i]);
        keys[n] = b;
        n++;
        //b.draw();
        //writeText(b.x, b.y, 4, (char*)b.title.c_str(), HX8357_BLACK, RED);
        //cout << "THIS IS THE X VALUE OF THE KEY" << keys[i].x;
   }

   for(int i = 0; i < 6; i++){
       Button b( 55+i*65+(55-35), 270, 45, 50, RED, WHITE, secondLine[i]);
       keys[n] = b;
       n++;
       //keys[6+i]  = Button(i*45+(55-35), 260+55*1+100, 35, 45, 0x8080, 0x0, keyTitleLower[1][i]);
   }
}

Keyboard::Keyboard(int i){
    //int nextLineIndex
    vector< vector<string> > keyTitleLower;//= {{ "1", "2", "3", "4", "5", ":"}, {"6", "7", "8", "9", "0", "Ã¢â€ Â�", "X"}};
    char* firstLine[7];
    char* secondLine[6];
    firstLine[0] = "0";
    firstLine[1] = "1";
    firstLine[2] = "2";
    firstLine[3] = "3";
    firstLine[4] = "4";
    firstLine[5] = "5";
    firstLine[6] = ":";
    secondLine[0] = "6";
    secondLine[1] = "7";
    secondLine[2] = "8";
    secondLine[3] = "9";
    secondLine[4] = "X";
    secondLine[5] = "<-";

    //keyTitleLower.push_back(firstLine);
    // keyTitleLower.push_back(secondLine);

    //hardcoded in width and height!
    int n = 0;
    for(int i = 0; i < 7; i++){

        Button b( i*60+(55-35), 210, 45, 50, RED, HX8357_WHITE, firstLine[i]);
        keys[n] = b;
        n++;
        //b.draw();
        //writeText(b.x, b.y, 4, (char*)b.title.c_str(), HX8357_BLACK, RED);
        //cout << "THIS IS THE X VALUE OF THE KEY" << keys[i].x;
    }

    for(int i = 0; i < 6; i++){
        Button b( i*60+(55-35), 270, 45, 50, RED, HX8357_WHITE, secondLine[i]);
        keys[n] = b;
        n++;
        //keys[6+i]  = Button(i*45+(55-35), 260+55*1+100, 35, 45, 0x8080, 0x0, keyTitleLower[1][i]);
    }

}

void Keyboard::draw(){

    fillRect(55,200,400,150,HX8357_BLACK);
    //fillRect(0,0,320,200, HX8357_BLACK);

    for(int i = 0; i < 12; i++){
      //if(keys[i] != NULL){ // safety check
        keys[i].draw();
      //}
    }

}

void Keyboard::drawColonKeyboard(){

    fillRect(10,200,430,150,HX8357_BLACK);
    //fillRect(0,0,320,200, HX8357_BLACK);

    for(int i = 0; i < 13; i++){
      //if(keys[i] != NULL){ // safety check
        keys[i].draw();
      //}
    }

}

Clock_display::Clock_display(int x, int y){
    this->x = x;
    this->y = y;
    // draw clock face
    //stroke(255);
    //strokeWeight(1);
    //TODO
    //ellipse(x, y, 250, 250);
    for(int i = 0; i < 12; i++) {
      //stroke(0); // make it RED :)
      //strokeWeight(5); // set stroke weight to 5
      //TODO line(x + 100*cos( (2*PI/12) * i), y + 100*sin(2*PI/12 * i), x + 110*cos( (2*PI/12) * i), y + 110*sin(2*PI/12 * i));
    }
}


Nav_bar::Nav_bar(int size, char* element_titles[]){

    //this->p->x = x;
    //this->p->y = y;
    this->textSize = size;


    // create the elements, then define their properties. This one is tricky.

    for(int i = 0; i < 7; i++) {
      //elements[i] = new Element();
      Element e;
      e.title = element_titles[i];
      elements[i] = e;
    }

    createNavBar();

 }

void Nav_bar::createNavBar(){
    int max_element_width  = 0;

     for(int i = 0; i < 7; i++){
       //max_element_width = round( max(max_element_width, textWidth(elements[i]->title)) ); // find maximum length
       max_element_width = 45;
     }

     max_element_width += padding; // adds a bit of padding so the length of elements so text fits comfortably inside.

     for(int i = 0; i < 7; i++){
       elements[i].dx = max_element_width;
       elements[i].dy = this->textSize + this->padding;
     }

     this->size->x = max_element_width * 7; // sum of the lengths (this case we can just multiply for O(1).
}

void Nav_bar::draw(char* day){
    int _x = 0; // round not needed for when ported to C/C++
    int _y = p->y;
   for(int i = 0; i <7; i++){

     //noStroke();
     //fill( (i == selectedElement) ?  theme.selected : theme.notselected );
     //fillRect(_x, _y, elements[i]->dx,  elements[i]->dy, RED); // vertical width is based on the the size of the text

     //fill(txt);
     //text(elements[i].title, _x + elements[i].dx/2, _y +  elements[i].dy/2);
     if (elements[i].title == day){
         writeText(_x, 60, 3,this->elements[i].title, HX8357_BLACK, HX8357_YELLOW);
     }
     else{
         writeText(_x, 60, 3,this->elements[i].title, HX8357_BLACK, HX8357_WHITE);
     }
     //stroke(bg);
     _x += elements[i].dx;

   }

}

bool Nav_bar::check(Point a){

   int _x = a.x; // round not needed for when ported to C/C++
   int _y = a.y;
   for(int i = 0; i < 7; i++){
     // optimization: We can improve performance a bit by not checking redundant things and checking only the button that's closest to the touch.

     if( (a.x > _x) && (a.x < (_x + elements[i].dx)) && (a.y > _y) && (a.y < (_y+elements[i].dy))){
       selectedElement = i; // set i equal to the new selected value
       //draw();      // automatically redraw element that was pressed. (optimization: may be optimized to only redraw the 2 Elements that change color. May provide significant I/O performance boost.)
       return true; // return true if an element was pressed. You can read the new
     }
     /*
     switch(orientation){
     case 'v':
       _y += elements[i].dy;
       break;
     default: // TODO: Should I assume equal spacing for this?
       _x += elements[i].dx;
       break;
     }
     */

   }
   return false; // return false if no element was pressed.
 }

