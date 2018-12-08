

// this class will combine GFX, inputs, and define UI elements.


class App {
  int currentWindow = 0;
  
  // 2 edge cases need to be considered: When I have entered a particular state, and when I am about to leave a particular state.
  // In these cases I have to init and deinit the UI elements for the given state.
  void run(){
    switch(currentWindow){
      
      case 0: // MAIN DISPLAY
      
      break;
        
      case 1: // SET ALARMS
      // TODO implement code here
      
      break;
        
      case 2: // SET 
      // TODO: implement code here
        
        
      break;
        
        
      default:
        println("not valid state");
    }
  }
} App app = new App();
