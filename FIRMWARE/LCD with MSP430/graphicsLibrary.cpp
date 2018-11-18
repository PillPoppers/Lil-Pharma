

class Button{
    int x, y, dx, dy;
    color bg, txt;    // defines background and text color of button
    string title = "Button"; // what the button says

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


};
