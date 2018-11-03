import java.util.Queue; 
import java.util.LinkedList; // this should be a circular buffer of fixed size when ported to C/C++
import java.util.Calendar;
import java.util.Date;
import java.text.SimpleDateFormat;
import java.text.DateFormat;

Queue<PVector> touch = new LinkedList(); // touch stores PVector where user tapped the screen.
Button buttons[] = new Button[7];
Nav_bar window_selection;  // navigation bar is rendered when displaying the window_selections.
Clock_display clock;
Button dispense, dispense2, dispense3;
Button morning, afternoon, evening, night;
String morningtext = "", afternoontext = "", eveningtext = "", nighttext = ""; 
String current = "";
Keyboard keyboard;
String pin = "";
int counter = 0;
int state;
int index = 0;
Calendar currentTime = null;
Calendar[][] allTimes = new Calendar[7][4]; 

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
    case 3:
      setup3(); 
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
  dispense = new Button(100, 400, 150, 70, color(128, 244, 66), color(13, 15, 11), "Continue to pin");
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
  dispense2 = new Button(200, 10, 200, 70, color(128, 244, 66), color(13, 15, 11), "Continue to alarms");
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
  morning = new Button(10, 115, 100, 30, color(128, 244, 66), color(13, 15, 11), "Morning");
  morning.draw();
//text("Morning: ", width-250, height-350); 
  fill(230);
  rect(150,115,100,40);
  fill(230);
  //text("Afternoon: ", width-250, height-300); 
  fill(230);
  rect(150,170,100,40);
  afternoon = new Button(10, 170, 120, 30, color(128, 244, 66), color(13, 15, 11), "Afternoon");
  afternoon.draw();
  fill(230);
  //text("Evening: ", width-250, height-230); 
  fill(230);
  rect(150,230,100,40);
  evening = new Button(10, 230, 100, 30, color(128, 244, 66), color(13, 15, 11), "Evening");
  evening.draw();
  fill(230);
  //text("Night: ", width-250, height-170); 
  fill(230);
  rect(150,275,100,40);
  night = new Button(10, 280, 100, 30, color(128, 244, 66), color(13, 15, 11), "Night");
  night.draw();
  fill(230);
  dispense3 = new Button(100, 10, 50, 70, color(128, 244, 66), color(13, 15, 11), "Continue to alarms");
  dispense3.draw();
 
}

void setup3(){
  //noSmooth();     // disable AA to better emulate the appearance on LCD
  GFX_fill(theme.background);    // fill background
  //keyboard = new Keyboard();
  //keyboard.draw();
  
  String s[] = {"MAIN", "ADMIN"};
  window_selection = new Nav_bar(45, 10, 20, 30, s, 'h', true);
  window_selection.draw(); // draw window_selection nav_bar
  Date todayDate = new Date(); 
  SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd"); 
  String date = dateFormat.format(todayDate.getTime()); 
  text(date, width/2, 90); //will indicate AFTERNOON, MORNING, OR NIGHT
  clock = new Clock_display(width/2,height/2);
  clock.draw();
  text("Pill Status ", width/2, 380); //will indicate AFTERNOON, MORNING, OR NIGHT
  text("Morning: ", width/4, 400); //will indicate AFTERNOON, MORNING, OR NIGHT
  text("Afternoon: ", width/4, 420); //will indicate AFTERNOON, MORNING, OR NIGHT
  text("Evening: ", width/4, 440); //will indicate AFTERNOON, MORNING, OR NIGHT
  text("Night: ", width/4, 460); //will indicate AFTERNOON, MORNING, OR NIGHT
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
  
  DateFormat dateFormat = new SimpleDateFormat("HH:mm");
  currentTime = Calendar.getInstance();
  //dateFormat.format(currentTime);
  System.out.println(allTimes[0][0]);
  if (allTimes[0][0] != null){
  System.out.println("reaches here");
  if (currentTime.HOUR_OF_DAY == allTimes[0][0].HOUR_OF_DAY && currentTime.MINUTE == allTimes[0][0].MINUTE){
    System.out.println("alarm interrupt");
  }
  System.out.println(currentTime.HOUR_OF_DAY == allTimes[0][0].HOUR_OF_DAY); 
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
    
    else if (state == 2){
    for(Button b: keyboard.keys){
      //System.out.println(b);
       
      if (b != null){
        if (b.check(new PVector(mouseX, mouseY))){
          if (b.title.equals("←")){
            current = current.substring(0, current.length()-1);
            System.out.println(current);
            fill(255);
            rect(10,330,width-20,25);
            fill(18);
            text(current, 60, 340);
          
          }
          else if (current.length() < 5){

              if (b.title.equals("X")){
                current = "N/A"; 
              }
              else{
                current += b.title;
                System.out.println(current);
                fill(230);
                rect(10,330,width-20,25);
                fill(18);
                text(current, 60, 340);
                
              }          
           }
        }
      }
    }
    
    if (morning.check(new PVector(mouseX, mouseY)) && current.length() == 5) {
       morningtext = current; 
       System.out.println(morningtext);
       fill(255);
       rect(150,115,100,40);
       fill(18);
       text(morningtext, 200, 130);
       
       DateFormat sdf = new SimpleDateFormat("hh:mm");
       Calendar time = Calendar.getInstance();
       try{
       Date date = sdf.parse(morningtext);
       time.setTime(date);
       }
       catch(Exception e){
        System.out.println(e); 
       }
       allTimes[0][0] = time; 
    }
    
    if (afternoon.check(new PVector(mouseX, mouseY)) && current.length() == 5) {
       afternoontext = current; 
       System.out.println(afternoontext);
       text(afternoontext, 200, 190);
    }
    
    if (evening.check(new PVector(mouseX, mouseY)) && current.length() == 5) {
       eveningtext = current; 
       System.out.println(eveningtext);
       text(eveningtext, 200, 250);
    }
    
    if (night.check(new PVector(mouseX, mouseY)) && current.length() == 5) {
       nighttext = current; 
       //System.out.println(nighttext);
       text(nighttext, 200, 300);
    }
    }
  //if (state == 0 && (mouseX < 247 && mouseX > 100) &&( mouseY > 400 && mouseY < 465)){
    if (state == 0 && dispense.checkWithTitle(new PVector(mouseX, mouseY), "Continue to pin")){
      state = 1;
      startState(state);
      System.out.println(dispense.title);
    }
    if (state == 1 && dispense2.checkWithTitle(new PVector(mouseX, mouseY), "Continue to alarms") && pin.length() == 4){
      state = 2;
      startState(state);
      System.out.println(dispense2.title);
    }
    if (state == 2 && dispense3.check(new PVector(mouseX, mouseY))){
      state = 3; 
      startState(state); 
    }
    
    
   
  //}
  //System.out.println("x: " + mouseX); 
  //System.out.println("y: " + mouseY); 
  touch.add( new PVector(mouseX, mouseY));
  
}
