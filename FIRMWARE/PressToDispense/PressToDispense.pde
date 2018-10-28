import java.util.Queue; 
import java.util.LinkedList; // this should be a circular buffer of fixed size when ported to C/C++

Queue<PVector> touch = new LinkedList(); // touch stores PVector where user tapped the screen.
Button buttons[] = new Button[7];
Nav_bar window_selection;  // navigation bar is rendered when displaying the window_selections.
Clock_display clock;
Button dispense;
Keyboard keyboard;

void setup(){  
  size(640, 480); // resolution of LCD. Our LCD is going to be lower res. All graphic objects must be scalled appropriatly.
  //noSmooth();     // disable AA to better emulate the appearance on LCD
  GFX_fill(theme.background);    // fill background
  //keyboard = new Keyboard();
  //keyboard.draw();
  
  String s[] = {"MAIN", "ADMIN"};
  window_selection = new Nav_bar(45, 10, 20, 30, s, 'h', true);
  window_selection.draw(); // draw window_selection nav_bar
  clock = new Clock_display(width/4,height/2);
  clock.draw();
  fill(244, 98, 66);
  text("It's time to take your meds!", 450, height/4); 
  text("AFTERNOON", 450, 175); //example: can show AFTERNOON, MORNING, EVENING
  text("Press button to dispense pills!", 450, height/2); 
  dispense = new Button(350, 290, 200, 100, color(128, 244, 66), color(13, 15, 11), "Press this!");
  dispense.draw();
  
}




int psec = second();
void draw(){
  int sec = second();
  boolean WindowChangeFlag = false;
  
  // set WindowChangeFlag flag
  if(touch.peek() != null){
    PVector v = touch.poll();
    WindowChangeFlag = window_selection.check(v);
  }
  
  // run every second. Better to set up a timer that sets a flag in software instead
  //if(sec != psec){
  //  clock.draw();
  //}
  
  
  sec = psec;
}


// interrupt driven code. When the IRQ pin on the LCD triggers a GPIO pin, that triggers an an interrupt that does what is here.
// in Java/Proccessing, the closest thing is mousePressed.
void mousePressed(){
  touch.add( new PVector(mouseX, mouseY));
}
