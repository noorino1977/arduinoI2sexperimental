#include "filter_def.h"
#include "owncodec.h"
#include "SI5351Noor.h"
#include "i2cNoor.h"

Si5351 si5351;
int_fast32_t freq_manual =  7060000;   

int rxbuf[buffers], txbuf[buffers];
float l_in[buffers/2], r_in[buffers/2];
float l_out[buffers/2], r_out[buffers/2];

void prosesAudio( void *pvParameters );

float val_gain_input = 200.0f;
float val_gain_out = 1.0f;
byte modeSSBz = 0; //bypass ..change  it by serial  for experiment
byte filtermode = 0;//bypass ..change  it by serial  for experiment

void setup() {
  Serial.begin(115200); 
  parameter_I2S();
  mclk_pin_select(0); 
  xTaskCreatePinnedToCore(prosesAudio, "prosesAudio", 4096 , NULL, 10,  NULL, 0);

  bool i2c_found;
  i2c_found = si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);
  if(!i2c_found){ Serial.println("Device not found on I2C bus!");}
  if(i2c_found){ Serial.println("Device found");}
  si5351.drive_strength(SI5351_CLK0, SI5351_DRIVE_2MA);
  si5351.drive_strength(SI5351_CLK1, SI5351_DRIVE_2MA);
  si5351.set_freq(freq_manual*100, SI5351_CLK0);
  si5351.set_freq(freq_manual*100, SI5351_CLK1);
  si5351.set_phase(SI5351_CLK0, 0); 
  si5351.set_phase(SI5351_CLK1, 90); 
  si5351.update_status();
  delay(500);
  Serial.println(freq_manual);
}

void prosesAudio(void *pvParameters){
  (void) pvParameters;
  float sample = 0;  
  for (;;) {
    size_t readsize = 0;  
          
    esp_err_t rxfb = i2s_read(I2S_NUM_0, &rxbuf[0], buffers * 2, &readsize, 1000);                   
                                
    if (rxfb == ESP_OK && readsize==buffers * 2) {                   
           /*split L an R input*/ 
          int y=0;
          for (int i=0; i<buffers; i+=2) {
            r_out[y] = (float) rxbuf[i];
            l_out[y] = (float) rxbuf[i+1];
            y++;
          }
          
           /*gain input   */      
          for (int i=0; i<buffers/2; i++) {  
            r_out[i] = val_gain_input * r_out[i];         
            l_out[i] = val_gain_input * l_out[i];
                         
          }   
                               
          /*Phase shift */    
          if (modeSSBz==1){
              for (int i=0; i<buffers/2; i++) {          
                r_out[i] =   firP45(r_out[i]);
                l_out[i] =   firM45(l_out[i]);                          
              }  
          } 
          else if (modeSSBz==2){
               for (int i=0; i<buffers/2; i++) {          
                r_out[i] =   firM45(r_out[i]);
                l_out[i] =   firP45(l_out[i]);                          
              }             
          }
          else if (modeSSBz==3){
              for (int i=0; i<buffers/2; i++) {          
                r_out[i] =   firP45(r_out[i]);
                l_out[i] =   firM45(l_out[i]);                          
              }             
          }
          else if (modeSSBz==4){
               for (int i=0; i<buffers/2; i++) {          
                r_out[i] =   firM45(r_out[i]);
                l_out[i] =   firP45(l_out[i]);                          
              }             
          }
           
          /*bandpass*/
          if (filtermode==1){
              for (int i=0; i<buffers/2; i++) {          
                r_out[i] =   iirbp1(r_out[i]);
                l_out[i] =   iirbp1(l_out[i]);                          
              } 
          }  
          else if (filtermode==2){
               for (int i=0; i<buffers/2; i++) {          
                r_out[i] =   iirbp2(r_out[i]);
                l_out[i] =   iirbp2(l_out[i]);                          
              } 
          }  
          else if (filtermode==3){
               for (int i=0; i<buffers/2; i++) {          
                r_out[i] =   iirbp3(r_out[i]);
                l_out[i] =   iirbp3(l_out[i]);                          
              }  
          }  
          else if (filtermode==4){
               for (int i=0; i<buffers/2; i++) {          
                r_out[i] =   iirbp4(r_out[i]);
                l_out[i] =   iirbp4(l_out[i]);                          
              }  
          }  
          else if (filtermode==5){
               for (int i=0; i<buffers/2; i++) {          
                r_out[i] =   iirbp5(r_out[i]);
                l_out[i] =   iirbp5(l_out[i]);                          
              }  
          }    
          else if (filtermode==6){
               for (int i=0; i<buffers/2; i++) {          
                r_out[i] =   iirbp6(r_out[i]);
                l_out[i] =   iirbp6(l_out[i]);                          
              }  
          }    
           /*lowpass*/
          else if (filtermode==7){
               for (int i=0; i<buffers/2; i++) {          
                r_out[i] =   iirlp(r_out[i]);
                l_out[i] =   iirlp(l_out[i]);                         
              }  
          }                                
          /*gain  */
          for (int i=0; i<buffers/2; i++) {  
            r_out[i] = val_gain_out * r_out[i];         
            l_out[i] = val_gain_out * l_out[i];
                         
          }     
                        

          /*combine to mono
          for (int i=0; i<buffers/2; i++) {          
                r_out[i] =   r_out[i]+l_out[i];
                l_out[i] =   r_out[i]+l_out[i];                          
          }          
          */     
         /*combine output*/
          y=0;
          for (int i=0;i<buffers/2;i++) {
          txbuf[y]   = (int) l_out[i];
          txbuf[y+1] = (int) r_out[i];
          y=y+2;
          }   
             
         i2s_write(I2S_NUM_0, &txbuf[0], buffers * 2, &readsize, 1000);
    }                   
  } 
  vTaskDelete(NULL); 
}

void loop(){
  if (Serial.available() > 0)  {
    char key = Serial.read();
    switch (key)    {
    case 'a':
      filtermode+=1; if(filtermode>7) filtermode=0; Serial.printf("Filter mode = %d\n", filtermode);
      delay(50);     
      break;
    case 'z':
      filtermode-=1; if(filtermode<0) filtermode=6; Serial.printf("Filter mode = %d\n", filtermode);
      delay(50);     
      break;
    case 's':
      modeSSBz+=1; if(modeSSBz>4) modeSSBz=0; Serial.printf("modeSSBz = %d\n", modeSSBz);
      delay(50);     
      break;
    case 'x':
      modeSSBz-=1; if(modeSSBz<0) modeSSBz=4; Serial.printf("modeSSBz = %d\n", modeSSBz);
      delay(50);     
      break;  
    case 'd':
      val_gain_input+=1.0f; Serial.printf("Gain Input + %f\n", val_gain_input);
      delay(50);     
      break;
    case 'c':
      val_gain_input-=1.0f;if(val_gain_input<0) val_gain_input=0;Serial.printf("Gain Input + %f\n", val_gain_input);
      delay(50);     
      break;
    case 'f':
      val_gain_out+=1.0f; Serial.printf("Gain Out + %f\n", val_gain_out);
      delay(50);     
      break;
    case 'v':
      val_gain_out-=1.0f; if(val_gain_out<0) val_gain_out=0; Serial.printf("Gain Out + %f\n", val_gain_out);
      delay(50);     
      break; 
    case 'g':
      freq_manual+=1000;
      si5351.set_freq(freq_manual*100, SI5351_CLK0);
      si5351.set_freq(freq_manual*100, SI5351_CLK1);
      si5351.set_phase(SI5351_CLK0, 0); 
      si5351.set_phase(SI5351_CLK1, 90); 
      Serial.println(freq_manual);
      delay(50);     
      break;
    case 'b':
      freq_manual-=1000;
      si5351.set_freq(freq_manual*100, SI5351_CLK0);
      si5351.set_freq(freq_manual*100, SI5351_CLK1);
      si5351.set_phase(SI5351_CLK0, 0); 
      si5351.set_phase(SI5351_CLK1, 90); 
      Serial.println(freq_manual);
      delay(50);     
      break; 
    case 'h':
      freq_manual+=100;
      si5351.set_freq(freq_manual*100, SI5351_CLK0);
      si5351.set_freq(freq_manual*100, SI5351_CLK1);
      si5351.set_phase(SI5351_CLK0, 0); 
      si5351.set_phase(SI5351_CLK1, 90); 
      Serial.println(freq_manual);
      delay(50);     
      break; 
     case 'n':     
      freq_manual-=100;
      si5351.set_freq(freq_manual*100, SI5351_CLK0);
      si5351.set_freq(freq_manual*100, SI5351_CLK1);
      si5351.set_phase(SI5351_CLK0, 0); 
      si5351.set_phase(SI5351_CLK1, 90); 
      Serial.println(freq_manual);
      delay(50);     
      break; 
     case 'm':       
      si5351.init(SI5351_CRYSTAL_LOAD_8PF, 0, 0);
      si5351.drive_strength(SI5351_CLK0, SI5351_DRIVE_2MA);
      si5351.drive_strength(SI5351_CLK1, SI5351_DRIVE_2MA);
      si5351.set_freq(freq_manual*100, SI5351_CLK0);
      si5351.set_freq(freq_manual*100, SI5351_CLK1);
      si5351.set_phase(SI5351_CLK0, 0); 
      si5351.set_phase(SI5351_CLK1, 90); 
      si5351.update_status();
      delay(50);     
     break;

          
    }
  }  
}
