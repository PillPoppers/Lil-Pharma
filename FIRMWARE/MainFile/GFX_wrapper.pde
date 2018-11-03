import java.awt.Graphics;


void GFX_fill(int r, int g, int b){
  background(r,g,b);
}

void GFX_fill(int a){
  background(a);
}

class Button{
  int x, y, dx, dy; // defines location and size of button
  color bg, txt;    // defines background and text color of button
  String title = "Button"; // what the button says
  
  Button(int x, int y, int dx, int dy, color bg, color txt, String title){
    this.x = x;   this.y = y;
    this.dx = dx; this.dy = dy;
    this.bg = bg; this.txt = txt;
    this.title = title;
  }
  
  // redraws the button. Should never need to use it after button is created.
  void draw(){
    noStroke();
    fill(bg);
    rect(x,y,dx,dy,4);
    textAlign(CENTER, CENTER);
    textSize(25);
    fill(txt);
    text(title, x + dx/2,y + dy/2);
  }
  
  // gets should get called when an event occurs. Returns whether button was pressed.
  boolean check(PVector a){
    return ( (a.x > x) && (a.x < (x+dx)) && (a.y > y) && (a.y < (y+dy)));
  }
  
  boolean checkWithTitle(PVector a, String title){
    return ( (a.x > x) && (a.x < (x+dx)) && (a.y > y) && (a.y < (y+dy)) && this.title.equals(title));
  }
}

class Nav_bar{
  /******************************************/
  /* "Look and feel" variables and defualts */
  /******************************************/
  PVector p = new PVector(0,0);      // position of Nav_Bar 
  PVector size = new PVector(-1,-1); // Calculated based on elements within. Speficies the region on the screen that is used for rendering the Nav_bar.
  int padding = 10;  // padding. Default is 10.
  char orientation = 'h'; // 'h' is horazontal, 'v' is vertical.
  boolean fixed = true;   // is the spacing fixed, or is it based on the length of the character? (Has no effect when orientation is vertical.)
  boolean banner = false; // if true, the navBar will be drawn as a banner that streches across the screen. p.x or p.y gets ignored depending on orientation.
  color bg = color(80,80,80);
  color txt = color(255);
  int textSize = 12;
  int selectedElement = 0; // assume first element is selected (this is basically like a radio button selection.)
  
  
  
  /********************************************/
  /* variables used internally for rendering: */
  /********************************************/
  // several elements make up a Nav_bar. [hello][world] would have 2 elements, for instance. 'hello' and 'world.'
  // this structure just holds infomation needed to render the Element. May be expanded upon as needed.
  class Element{
    int dx = 0; // width of element
    int dy = 0; // height of element
    String title = "hello";
  } Element elements[]; // creates an array of Elements, collectively making up a NavBar.
  
  //TODO: Test every combo of orientation and fixed. (4 total.)
  Nav_bar(int x, int y, int size, int padding, String element_titles[], char orientation, boolean fixed){
    this.p.x = x;
    this.p.y = y;
    this.orientation = orientation;
    this.fixed = fixed;
    this.textSize = size;
    elements = new Element[element_titles.length]; // java equivelent to C malloc(sizeof(element_titles.length))
    textSize(textSize);
    
    // create the elements, then define their properties. This one is tricky.
    // this.elements = new Element[element_titles.length];
    for(int i = 0; i < element_titles.length; i++) {
      elements[i] = new Element();
      this.elements[i].title = element_titles[i];
    }
    
    switch(orientation){
      case('h'):
        // if the orientation is fixed, then use equal spacing between all the elements.
        // this may be a hard one to port, since textWidth may need to be implemented. Calculates length of a string in Pixels.
        // round function only needed because max returns float instead of int
        if(fixed){
          
          int max_element_width  = 0;
          
          for(int i = 0; i < elements.length; i++){
            max_element_width = round( max(max_element_width, textWidth(elements[i].title)) ); // find maximum length
          }
          
          max_element_width += padding; // adds a bit of padding so the length of elements so text fits comfortably inside.
        
          for(int i = 0; i < elements.length; i++){
            elements[i].dx = max_element_width;
            elements[i].dy = textSize + padding;
          }
          
          this.size.x = max_element_width * elements.length; // sum of the lengths (this case we can just multiply for O(1).
        }
        else{ // !fixed
        
          for(int i = 0; i < elements.length; i++){
            elements[i].dx = round(textWidth(elements[i].title) + padding);
            elements[i].dy = textSize + padding;
            this.size.x += elements[i].dx; // sum of the lengths
          }
        }
        this.size.y = textSize + padding;
        break;
        
      case('v'): // vertical stacking: fixed has no effect (it doesn't make sense since the height of each block does not vary with the Title.)
        int max_element_width  = 0;
        
        for(int i = 0; i < elements.length; i++){ // find maximum length
          max_element_width = round( max( max_element_width, textWidth(elements[i].title) ) );
        }
        
        max_element_width += padding; // adds a bit of padding so the length of elements so text fits comfortably inside.
        
        for(int i = 0; i < elements.length; i++){
          elements[i].dx = max_element_width;
          elements[i].dy = textSize + padding;
        }
        break;
      default:
        println("Nav_bar error: orientation not valid.");
    }
  }
  
  
  void draw(){
    textAlign(CENTER, CENTER);
    textSize(textSize);
    fill(txt);
    
    int _x = round(p.x); // round not needed for when ported to C/C++
    int _y = round(p.y);
    for(int i = 0; i < elements.length; i++){
      
      noStroke();
      fill( (i == selectedElement) ?  theme.selected : theme.notselected );
      rect(_x, _y, elements[i].dx,  elements[i].dy); // vertical width is based on the the size of the text
      
      fill(txt);
      text(elements[i].title, _x + elements[i].dx/2, _y +  elements[i].dy/2);
      stroke(bg);
      switch(orientation){
      case 'v':
        _y += elements[i].dy;
        break;
      default: // TODO: Should I assume equal spacing for this?
        _x += elements[i].dx;
        break;
      }
    }
  }
  
  void erase(){
    println("not implemented"); // TODO: implement me
  }
  
  // checks which (if any) element in Nav_Bar was pressed, and returns the current selected element (index of the Title array passed in)
  boolean check(PVector a){
    
    int _x = round(p.x); // round not needed for when ported to C/C++
    int _y = round(p.y);
    for(int i = 0; i < elements.length; i++){
      // optimization: We can improve performance a bit by not checking redundant things and checking only the button that's closest to the touch.
      
      if( (a.x > _x) && (a.x < (_x + elements[i].dx)) && (a.y > _y) && (a.y < (_y+elements[i].dy))){
        selectedElement = i; // set i equal to the new selected value
        draw();      // automatically redraw element that was pressed. (optimization: may be optimized to only redraw the 2 Elements that change color. May provide significant I/O performance boost.)
        return true; // return true if an element was pressed. You can read the new 
      }
      switch(orientation){
      case 'v':
        _y += elements[i].dy;
        break;
      default: // TODO: Should I assume equal spacing for this?
        _x += elements[i].dx;
        break;
      }
      
    }
    return false; // return false if no element was pressed.
  }
}


class Clock_display{
  int hour,  minute,  second,
     phour, pminute, psecond;
     
  int x, y;
  
  Clock_display(int x, int y){
    this.x = x;
    this.y = y;
    draw();
    
    // draw clock face
    stroke(255);
    strokeWeight(1);
    ellipse(x, y, 250, 250);
    for(int i = 0; i < 12; i++) {
      stroke(0); // make it RED :)
      strokeWeight(5); // set stroke weight to 5
      line(x + 100*cos( (2*PI/12) * i), y + 100*sin(2*PI/12 * i), x + 110*cos( (2*PI/12) * i), y + 110*sin(2*PI/12 * i));
    }
  }
  
  int draw(){
    hour   = hour();
    minute = minute();
    second = second();
    phour  =  (hour - 1) % 12;
    pminute = (minute - 1) % 60;
    psecond = (second - 1) % 60;
    
    // erase old minute hand by masking them by drawing on top of the old hands
    // TODO: Hour hand will jump in descrete steps. May not look like a real analog clock
    stroke(255,255,255); 
    strokeWeight(5); line(x, y, x + 40*sin( (2*PI/12) * phour   ), y - 40*cos(2*PI/12 * phour   ));
    strokeWeight(2); line(x, y, x + 80*sin( (2*PI/60) * pminute ), y - 80*cos(2*PI/60 * pminute ));
    strokeWeight(1); line(x, y, x + 90*sin( (2*PI/60) * psecond ), y - 90*cos(2*PI/60 * psecond ));
    
    // draw new hands where they belong
    stroke(255,0,0);
    strokeWeight(5); line(x, y, x + 40*sin( (2*PI/12) * hour  ), y - 40*cos(2*PI/12 * hour  ));
    strokeWeight(2); line(x, y, x + 80*sin( (2*PI/60) * minute), y - 80*cos(2*PI/60 * minute));
    strokeWeight(1); line(x, y, x + 90*sin( (2*PI/60) * second), y - 90*cos(2*PI/60 * second));
    
    return 0; // return error code in case of error
  }
  
  void deinit(){
    fill(0); // background fill
    noStroke();
    rect(x-126, y-126, 252, 252);
    // Java does not support this because it has built-in garbage collection and memory managment. 
  }
}


/*********************************************/
/* Basic full screen keyboard. The screen is */
/* fairly small, so this keyboard must take  */
/* up the entirety of the screen, providing  */ 
/* text box as well to enter a password.     */
/*********************************************/



class Keyboard{
  
  Button keys[] = new Button[64]; // allocate the memory
  /* KEYBOARD LAYOUT
     ` 1 2 3 4 5 6 7 8 9 0 - = [back]
  [tab] q w e r t y u i o p [ ] \
  [caps] a s d f g h j k l ; ' \n
  [shift]  z x c v b n m , . / 
  
     ~ ! @ # $ % ^ & * ( ) - = [back]
  [tab] Q W E R T Y U I O P { } |
  [caps] A S D F G H J K L : " \n
  [shift]  Z X C V B N M < > ? 
  */
  
  Keyboard(){
    
    String[][] keyTitleLower = {{ "1", "2", "3", "4", "5", ":"}, 
                                {"6", "7", "8", "9", "0", "←", "X"}};
                               /*{"TAB", "q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "[", "]", "\\"  },*/
                               /*{"CAPS", "a", "s", "d", "f", "g", "h", "j", "k", "l", ";", "'", "ENTER"},*/
                               /*{"SPACE", "!", "z", "x", "c", "v", "b", "n", "m", ",", ".", "/"}};*/
                                
    String[][] keyTitleUpper = {{"~", "!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "_", "+", "←"},
                                {"TAB", "Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "{", "}", "|"  },
                                {"CAPS",  "A", "S", "D", "F", "G", "H", "J", "K", "L", ":", "'", "ENTER"},
                                {"SPACE", "Z", "X", "C", "V", "B", "N", "M", "<", ">", "?"}};
    
    // draw keyboard body
    fill(theme.notselected);
    rect(0,350,width,height-400);
    // keys [`] through [←] :
    for(int i = 0; i < keyTitleLower[0].length; i++){
      keys[i] = new Button( 5+i*45+(55-35), 260+100, 35, 45, color(100,200,255), color(255), keyTitleLower[0][i]);
    }
    
    for(int i = 0; i < keyTitleLower[1].length; i++){
      keys[6+i]  = new Button(i*45+(55-35), 260+55*1+100, 35, 45, color(100,200,255), color(255), keyTitleLower[1][i]);
    }
    /*
    // special case for tab:
    keys[14]  = new Button( 10, 260+55*1, 55, 45, color(100,200,255), color(255), keyTitleLower[1][0]);
    
    for(int i = 1; i < 13; i++){
      keys[14+i]  = new Button( 10+i*45+(55-35), 260+55*1, 35, 45, color(100,200,255), color(255), keyTitleLower[1][i]);
    }
    
    // special case for caps lock:
    keys[14+14] = new Button( 10, 260+55*2, 35*2+10, 45, color(100,200,255), color(255), keyTitleLower[2][0]);
    
    for(int i = 1; i < 13; i++){
      keys[14+14+i]    = new Button( 10+i*45+45, 260+55*2, 35, 45, color(100,200,255), color(255), keyTitleLower[2][i]);
    }
    // special case for spacebar
    keys[14+14+12]  = new Button( 10, 260+55*3, 100, 45, color(100,200,255), color(255), keyTitleLower[3][0]);
    for(int i = 1; i < 11; i++){
      keys[14+14+12+i] = new Button( 10+i*45 + (100-45)+10, 260+55*3, 35, 45, color(100,200,255), color(255), keyTitleLower[3][i]);
    }
    */
  }
  
  boolean check(){
    return false;
  }
  
  
  void draw(){
    
    // draw keyboard body
    noStroke();
    fill(theme.notselected);
    rect(0,350,width,200);
    // text box
    fill(230);
    
    
    for(int i = 0; i < keys.length; i++){
      if(keys[i] != null){ // safety check
        keys[i].draw();
      }
    }
    
  }
  
}
