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
      break;
    case 4: 
      setup4();
      break;
    // ...
  }
}
void draw() { // or mousePressed(), keyPressed(), etc.
  switch( state ){
    case 0:
      draw0();
      break;
    case 1:
      draw0();
      break;
    case 2:
      draw0(); 
      break;
    case 3:
      draw3(); 
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
  dispense = new Button(100, 400, 150, 50, color(128, 244, 66), color(13, 15, 11), "-->");
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
  dispense2 = new Button(200, 10, 100, 40, color(128, 244, 66), color(13, 15, 11), "-->");
  dispense2.draw();
  
}

void setup2(){
  GFX_fill(theme.background); 
  String s[] = {"SUN"};
  window_selection = new Nav_bar(10, 10, 20, 30, s, 'v', true);
  window_selection.draw(); // draw window_selection nav_bar
  keyboard = new Keyboard();
  keyboard.draw();
  textSize(20);
  text("Set your alarms", width-180, height-390); 
  morning = new Button(20, 120, 100, 40, color(128, 244, 66), color(13, 15, 11), "Morn");
  morning.draw();
//text("Morning: ", width-250, height-350); 
  fill(230);
  rect(150,120,100,40);
  fill(230);
  //text("Afternoon: ", width-250, height-300); 
  fill(230);
  rect(150,170,100,40);
  afternoon = new Button(20, 170, 100, 40, color(128, 244, 66), color(13, 15, 11), "Noon");
  afternoon.draw();
  fill(230);
  //text("Evening: ", width-250, height-230); 
  fill(230);
  rect(150,220,100,40);
  evening = new Button(20, 220, 100, 40, color(128, 244, 66), color(13, 15, 11), "Evening");
  evening.draw();
  fill(230);
  //text("Night: ", width-250, height-170); 
  fill(230);
  rect(150,270,100,40);
  night = new Button(20, 270, 100, 40, color(128, 244, 66), color(13, 15, 11), "Night");
  night.draw();
  fill(230);
  dispense3 = new Button(270, 70, 50, 40, color(128, 244, 66), color(13, 15, 11), "->");
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

void setup4(){
  
  GFX_fill(theme.background);    // fill background
  //keyboard = new Keyboard();
  //keyboard.draw();
  
  String s[] = {"MAIN", "ADMIN"};
  window_selection = new Nav_bar(45, 10, 20, 30, s, 'h', true);
  window_selection.draw(); // draw window_selection nav_bar
  clock = new Clock_display(width/2,height/2);
  clock.draw();
  fill(244, 98, 66);
  text("It's time to take your meds!", width/2, 380); 
  text("AFTERNOON", 450, 175); //example: can show AFTERNOON, MORNING, EVENING
  text("Press button to dispense pills!", 160, 400); 
  dispense = new Button(width/4, 420, 150, 40, color(128, 244, 66), color(13, 15, 11), "Press this!");
  dispense.draw();
  
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
   for (int j = 0; j < 4; j ++){
   //System.out.println(allTimes[0][0].get(Calendar.HOUR_OF_DAY));
     if (allTimes[0][j] != null){
        if (hour() == allTimes[0][j].get(Calendar.HOUR_OF_DAY) && minute() == allTimes[0][j].get(Calendar.MINUTE)){
          System.out.println("alarm interrupt");
          state = 4; 
          startState(state);
        }
     }
   }
  
  sec = psec;
}

void draw3(){
  int sec = second();
  boolean WindowChangeFlag = false;
  
  // set WindowChangeFlag flag
  if(touch.peek() != null){
    PVector v = touch.poll();
    //WindowChangeFlag = window_selection.check(v);
  }
  
  DateFormat dateFormat = new SimpleDateFormat("HH:mm");
  currentTime = Calendar.getInstance();
   for (int j = 0; j < 4; j ++){
   //System.out.println(allTimes[0][0].get(Calendar.HOUR_OF_DAY));
     if (allTimes[0][j] != null){
        if (hour() == allTimes[0][j].get(Calendar.HOUR_OF_DAY) && minute() == allTimes[0][j].get(Calendar.MINUTE)){
          System.out.println("alarm interrupt");
          state = 4; 
          startState(state);
        }
     }
   }
  
  
  
  if(sec != psec){
    clock.draw();
  }
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
            rect(10,320,width-20,30);
            fill(18);
            text(pin, 150, 330);
          
          }
          else if (pin.length() < 4){
             pin += b.title;
             fill(230);
             rect(10,320,width-20,30);
             fill(18);
             text(pin, 150, 330);
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
            rect(10,320,width-20,25);
            fill(18);
            text(current, 150, 330);
          
          }
          else if (current.length() < 5){

              if (b.title.equals("X")){
                current = "N/A"; 
                fill(230);
                rect(10,320,width-20,30);
                fill(18);
                text(current, 150, 330);
              }
              else{
                current += b.title;
                System.out.println(current);
                fill(230);
                rect(10,320,width-20,30);
                fill(18);
                text(current, 150, 330);
                
              }          
           }
        }
      }
    }
    
    if (morning.check(new PVector(mouseX, mouseY)) && (current.length() == 5 || current.equals("N/A"))) {
       morningtext = current; 
       System.out.println(morningtext);
       fill(255);
       rect(150,120,100,40);
       fill(18);
       text(morningtext, 200, 142);
       
       if (!morningtext.equals("N/A")){
         int hours = Integer.parseInt(morningtext.substring(0,2)); 
         int min = Integer.parseInt(morningtext.substring(3));
         boolean valid = morningtext.substring(2,3).equals(":") && (hours >= 0 && hours < 24) && (min >= 0 && min < 60);
         if (valid){
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
         System.out.println(allTimes[0][0].get(Calendar.HOUR_OF_DAY)); 
         System.out.println(allTimes[0][0].get(Calendar.MINUTE));
         }
         else {
           
         }
       }
       else {
         allTimes[0][0] = null;
       }
    }
    
    if (afternoon.check(new PVector(mouseX, mouseY)) && (current.length() == 5 || current.equals("N/A"))) {
       afternoontext = current; 
       fill(255);
       rect(150,170,100,40);
       fill(18);
       text(afternoontext, 200, 192);
       
        if (!afternoontext.equals("N/A")){
         DateFormat sdf = new SimpleDateFormat("hh:mm");
         Calendar time = Calendar.getInstance();
         try{
         Date date = sdf.parse(afternoontext);
         time.setTime(date);
         }
         catch(Exception e){
          System.out.println(e); 
         }
         allTimes[0][1] = time; 
         System.out.println(allTimes[0][0].get(Calendar.HOUR_OF_DAY)); 
         System.out.println(allTimes[0][0].get(Calendar.MINUTE));
       }
       else {
         allTimes[0][1] = null;
       }
    }
    
    if (evening.check(new PVector(mouseX, mouseY)) && (current.length() == 5 || current.equals("N/A"))) {
       eveningtext = current; 
       fill(255);
       rect(150,220,100,40);
       fill(18);
       text(eveningtext, 200, 243);
       
        if (!eveningtext.equals("N/A")){
         DateFormat sdf = new SimpleDateFormat("hh:mm");
         Calendar time = Calendar.getInstance();
         try{
         Date date = sdf.parse(eveningtext);
         time.setTime(date);
         }
         catch(Exception e){
          System.out.println(e); 
         }
         allTimes[0][2] = time; 
         System.out.println(allTimes[0][0].get(Calendar.HOUR_OF_DAY)); 
         System.out.println(allTimes[0][0].get(Calendar.MINUTE));
       }
       else {
         allTimes[0][2] = null;
       }
    }
    
    if (night.check(new PVector(mouseX, mouseY)) && (current.length() == 5 || current.equals("N/A"))) {
       nighttext = current; 
       fill(255);
       rect(150,270,100,40);
       fill(18);
       text(nighttext, 200, 285);
       
        if (!nighttext.equals("N/A")){
         DateFormat sdf = new SimpleDateFormat("hh:mm");
         Calendar time = Calendar.getInstance();
         try{
         Date date = sdf.parse(nighttext);
         time.setTime(date);
         }
         catch(Exception e){
          System.out.println(e); 
         }
         allTimes[0][3] = time; 
         System.out.println(allTimes[0][0].get(Calendar.HOUR_OF_DAY)); 
         System.out.println(allTimes[0][0].get(Calendar.MINUTE));
       }
       else {
         allTimes[0][3] = null;
       }
    }
    }
  //if (state == 0 && (mouseX < 247 && mouseX > 100) &&( mouseY > 400 && mouseY < 465)){
    if (state == 0 && dispense.check(new PVector(mouseX, mouseY))){
      state = 1;
      startState(state);
      System.out.println(dispense.title);
    }
    if (state == 1 && dispense2.check(new PVector(mouseX, mouseY)) && pin.length() == 4){
      state = 2;
      startState(state);
      System.out.println(pin);
    }
    if (state == 2 && dispense3.check(new PVector(mouseX, mouseY))){
      state = 3; 
      startState(state); 
      System.out.println("state is" + state); 
    }
    
    
   
  //}
  //System.out.println("x: " + mouseX); 
  //System.out.println("y: " + mouseY); 
  touch.add( new PVector(mouseX, mouseY));
  
}
