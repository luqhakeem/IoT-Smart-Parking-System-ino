//Screeen
#include <ILI9341_kbv.h>
ILI9341_kbv tft;
#define vali0 0   





//COLORS
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void setup()
          {
    
            tft.begin();
            tft.setRotation(1);// Landscape
            tft.fillScreen(TFT_PINK);// set color iitial screen 
 
          }
          



 
        
void loop(){

  //Some text to see on the screen 
      tft.setCursor(0, 0);
      tft.fillScreen(TFT_BLUE);
      tft.setTextColor(YELLOW); 
      tft.setTextSize(2);
      tft.print("YELLOW");
      tft.setTextSize(4);
      tft.setTextColor(RED); 
      tft.setTextSize(3);
      tft.print("RED");
      tft.setCursor(50, 50);
      tft.setTextColor(BLACK);    
      tft.setTextSize(8);
      tft.print("TEST");
      delay(3000);


  

}
