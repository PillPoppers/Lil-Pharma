import java.util.Queue; 
import java.util.LinkedList; // this should be a circular buffer of fixed size when ported to C/C++

Queue<PVector> touch = new LinkedList(); // touch stores PVector where user tapped the screen.
Button buttons[] = new Button[7];
Nav_bar window_selection;  // navigation bar is rendered when displaying the window_selections.
Clock_display clock;
Button dispense, dispense2;
Keyboard keyboard;
String pin = "";
int counter = 0;
int state;
void setup(){
  size( 320, 480 );
  state = 0;
  startState( state );
}
void setState( int newState ){
  endState();
  state = newState;
  startState(state);
}
void endState(){
  switch( state ){
    case 0:
      //cleanup0();
      break;
    case 1:
      //cleanup1();
      break;
    // ...
  }
}
void startState( int state ){
  switch( state ){
    case 0:
      setup0();
      break;
    case 1:
      setup1();
      break;
    case 2:
      setup2(); 
      break;
    // ...
  }
}
void draw() { // or mousePressed(), keyPressed(), etc.
  switch( state ){
    case 0:
      draw0(); // or mousePressed0(), keyPressed0(), etc.
      break;
    case 1:
      draw0(); // or mousePressed1(), keyPressed1(), etc.
      break;
    // ...
  }
}

void setup0(){  
  //size(320, 480); // resolution of LCD. Our LCD is going to be lower res. All graphic objects must be scalled appropriatly.
  //noSmooth();     // disable AA to better emulate the appearance on LCD
  GFX_fill(theme.background);    // fill background
  //keyboard = new Keyboard();
  //keyboard.draw();
  
  String s[] = {"MAIN"};
  textSize(26);
  text("Welcome!", 100, height/4); 
  text("Configure your device!", 20, 200); //example: can show AFTERNOON, MORNING, EVENING
  dispense = new Button(100, 400, 150, 70, color(128, 244, 66), color(13, 15, 11), "Continue..");
  dispense.draw();
  
}

void setup1(){  
  //size(320, 480); // resolution of LCD. Our LCD is going to be lower res. All graphic objects must be scalled appropriatly.
  //noSmooth();     // disable AA to better emulate the appearance on LCD
  GFX_fill(theme.background);    // fill background
  keyboard = new Keyboard();
  keyboard.draw();
  
  String s[] = {"MAIN"};
  text("Welcome!", width/2, height/4); 
  text("Set up your admin pin.", width/2, 170); //example: can show AFTERNOON, MORNING, EVENING
  dispense2 = new Button(150, 20, 150, 50, color(128, 244, 66), color(13, 15, 11), "Continue..");
  dispense2.draw();
  
}

void setup2(){
  GFX_fill(theme.background); 
  String s[] = {"SUN"};
  window_selection = new Nav_bar(10, 10, 20, 30, s, 'v', true);
  window_selection.draw(); // draw window_selection nav_bar
  keyboard = new Keyboard();
  keyboard.draw();
  text("Set your alarms", width-180, height-390); 
  text("Morning: ", width-250, height-350); 
  fill(230);
  rect(10,210,width-20,40); 
  
}

void setup3(){
  
}



int psec = second();
void draw0(){
  int sec = second();
  boolean WindowChangeFlag = false;
  
  // set WindowChangeFlag flag
  if(touch.peek() != null){
    PVector v = touch.poll();
    //WindowChangeFlag = window_selection.check(v);
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
  System.out.println("here");

   if (state == 1){
    for(Button b: keyboard.keys){
      //System.out.println(b);
       
      if (b != null){
        if (b.check(new PVector(mouseX, mouseY))){
          if (b.title.equals("←")){
            pin = pin.substring(0, pin.length()-1);
            System.out.println(pin);
            fill(255);
            rect(10,210,width - 20, 40);
            fill(18);
            text(pin, 60, 225);
          
          }
          else if (pin.length() < 4){
             pin += b.title;
             fill(230);
             rect(10,210,width-20,40);
             fill(18);
             text(pin, 60, 225);
           }
        }
      }
    }
    }
  //if (state == 0 && (mouseX < 247 && mouseX > 100) &&( mouseY > 400 && mouseY < 465)){
    if (state == 0 && dispense.check(new PVector(mouseX, mouseY))){
      state = 1;
      startState(state);
      System.out.println(dispense.title);
    }
    if (state == 1 && dispense2.check(new PVector(mouseX, mouseY))){
      state = 2;
      startState(state);
      System.out.println(dispense.title);
    }
    
    
   
  //}
  //System.out.println("x: " + mouseX); 
  //System.out.println("y: " + mouseY); 
  touch.add( new PVector(mouseX, mouseY));
  
}
