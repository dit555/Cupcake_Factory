#include "rims.h"

//Project 3
//Written By: Dumitru Chiriac
//Email: dchir002@ucr.edu
//SID: 862126186
//Write Up Link: https://docs.google.com/document/d/1yjGMqxG7OIXT2gt7yG7NmgEq0XVJbKdojhbsXmxVXAg/edit?usp=sharing
//Demo Link: https://youtu.be/Csj1Fns6hfs

//For project 3 I will build a cupcake baking 'assembly line'
//I.E. A cupcake will travel through a conveyer and get assembled
//First the wrapper is placed, then it is filled with batter,
//then it is baked, frosted, and topped with sprinkles,
//and finally assembled with 8 other cupcakes in a tray

volatile int timerISR = 0;
void TimerISR(){
    timerISR = 1;
}

//cupcake struct
typedef struct cupcake{
    int wrapper;    //1 if wrapper placed
    int batter;     //1 if batter placed
    int baked;      //1 if baked
    int frosted;    //1 if frosted
    int sprinkle;  //1 if sprkles are on
    int packaged;   //1 if packaged
    int pos;        //postition index on conveyer belt by section
    int time;       //how long its spent in section
    
} cupcake;

//conveyer vars
//conveyer is split in 8 spections
//numbers mean length of conveyer in each section
//each station has a index on the converyer
//I.E at index 0 is the wrapper and the length
//of the converyer there is 1, whereas for the
//baker the index if 3 and the conveyer there
//is 20 long.
//any non indexed parts are empty

/*conveyer sections:
   conveyer[0] = 1;
   conveyer[1] = 1;
   conveyer[2] = 3;
   conveyer[3] = 10;
   conveyer[4] = 3;
   conveyer[5] = 1;
   conveyer[6] = 1;
   conveyer[7] = 1;
   */
const int conLength = 21;
const int conSections = 8;
const int wrIndex = 0;
const int batIndex = 1;
const int bakIndex = 3;
const int frIndex = 5;
const int spIndex = 6;
const int paIndex = 7;


//cupcake array
//converyer can only hold has many cupcakes
//as it is long so we can only have that
//many cupcakes on a converyer at a time
cupcake cup[21];
int completeCup = 0;
int packageTot = 0;

//SM States
enum WR_States {WR_Start, WR_Off, WR_Wait, WR_Place} WR_State; //wrapper
enum BAT_States {BAT_Start, BAT_Off, BAT_Wait, BAT_Fill} BAT_State; //batter-er
enum BAK_States {BAK_Start, BAK_Off, BAK_Heat, BAK_Ready} BAK_State; //baker
enum FR_States {FR_Start, FR_Off, FR_Wait, FR_Frost} FR_State; //froster
enum SP_States {SP_Start, SP_Off, SP_Wait, SP_Sprinkle} SP_State; //sprinkler
enum PA_State {PA_Start, PA_Off, PA_Package} PA_State;
enum CON_States {CON_Start, CON_Off, CON_Move} CON_State; //conveyer
enum DP_States {DP_Start, DP_Disp} DP_State; //display

//tick functions
void WRtickFCT();
void BATtickFCT();
void BAKtickFCT();
void FRtickFCT();
void SPtickFCT();
void PAtickFCT();
void CONtickFCT();
void DPtickFCT();

int ready = 0; //makes sure all stations are ready
void readyCheck(){
    if(!A0)
         ready = 1; //if all stations are on then ready
    else
         ready = 0; //if all stations are off then not ready
}
void main(){
    
   
   //cup init
   int i = 0;
   for(i = 0; i < conLength; i++){
       cup[i].wrapper = 0;
       cup[i].batter = 0;
       cup[i].baked = 0;
       cup[i].frosted = 0;
       cup[i].sprinkle = 0;
       cup[i].packaged = 0;
       cup[i].pos = -1; //-1 means not on conveyer
       cup[i].time = 0;
   }
   
   

   //SM init
   WR_State = WR_Start;
   BAT_State = BAT_Start;
   BAK_State = BAK_Start;
   FR_State = FR_Start;
   SP_State = SP_Start;
   PA_State = PA_Start;
   CON_State = CON_Start;
   DP_State = DP_Start;
   
   TimerSet(1000);
   TimerOn();
   
   while (1){
      readyCheck();
      while(!timerISR)
      timerISR = 0;
      WRtickFCT();
      BATtickFCT();
      BAKtickFCT();
      FRtickFCT();
      SPtickFCT();
      DPtickFCT();
      PAtickFCT();
      CONtickFCT();
   }
}

void WRtickFCT(){
    int i;
    switch(WR_State){//transitions
         case WR_Start: WR_State = WR_Off; break;
         case WR_Off: WR_State = (ready) ? WR_Wait : WR_Off;break; 
         case WR_Wait:
            if(!ready){
                WR_State = WR_Off; break;     
            }
            for(i = 0; i < conLength; i++){
                if(cup[i].pos == -1){
                    WR_State = WR_Place;
                    break;
                }
                else
                    WR_State = WR_Wait;
            }
            break;
         case WR_Place: WR_State = (ready) ? WR_Wait : WR_Off; break;
         default: WR_State = WR_Start; break;
    }
    
    switch(WR_State){//actions
        case WR_Place: cup[i].pos = 0; cup[i].wrapper = 1; break;   
    }
}

void BATtickFCT(){
    int i;
    switch(BAT_State){//transitions
         case BAT_Start: BAT_State = BAT_Off; break;
         case BAT_Off: BAT_State = (ready) ? BAT_Wait : BAT_Off;break; 
         case BAT_Wait:
            if(!ready){
                BAT_State = BAT_Off; break;     
            }
            for(i = 0; i < conLength; i++){
                if(cup[i].pos == batIndex){
                    BAT_State = BAT_Fill;
                    break;
                }
                else
                    BAT_State = BAT_Wait;
            }
            break;
         case BAT_Fill: BAT_State = (ready) ? BAT_Wait : BAT_Off; break;
         default: BAT_State = BAT_Start; break;
    }
    
    switch(BAT_State){//actions
        case BAT_Fill: cup[i].batter = 1;break;   
    }
}
void BAKtickFCT(){
    int i;
    switch(BAK_State){//transitions
         case BAK_Start: BAK_State = BAK_Off; break;
         case BAK_Off: BAK_State = (ready) ? BAK_Heat : BAK_Off;break; 
         case BAK_Heat: BAK_State = (ready) ? BAK_Ready : BAK_Off; break;
         case BAK_Ready: BAK_State = BAK_Ready;break;
         default: BAK_State = BAK_Start; break;
    }
    
    switch(BAK_State){//actions
        case BAK_Ready: 
            for(i = 0; i < conLength;i++){
               if(cup[i].pos == bakIndex && cup[i].time == 9){
                  cup[i].baked = 1; break; 
               }
            }
    }
}
void FRtickFCT(){
    int i = 0;
    switch(FR_State){
        case FR_Start: FR_State = FR_Off; break;
        case FR_Off: FR_State = (ready) ? FR_Wait : FR_Off; break;
        case FR_Wait:
            if(!ready){
                FR_State = FR_Off; break;     
            }
            for(i = 0; i < conLength; i++){
                if(cup[i].pos == frIndex){
                    FR_State = FR_Frost;
                    break;
                }
                else
                    FR_State = FR_Wait;
            }
            break;
         case FR_Frost: FR_State = FR_Wait; break;
         default: FR_State = FR_Start; break;
    }
    
    switch(FR_State){
        case FR_Frost: cup[i].frosted = 1; break;
    }
}
void SPtickFCT(){
    int i = 0;
    switch(SP_State){
        case SP_Start: SP_State = SP_Off; break;
        case SP_Off: SP_State = (ready) ? SP_Wait : SP_Off; break;
        case SP_Wait:
            if(!ready){
                SP_State = SP_Off; break;     
            }
            for(i = 0; i < conLength; i++){
                if(cup[i].pos == spIndex){
                    SP_State = SP_Sprinkle;
                    break;
                }
                else
                    SP_State = SP_Wait;
            }
            break;
         case SP_Sprinkle: SP_State = SP_Wait; break;
         default: SP_State = SP_Start; break;
    }
    
    switch(SP_State){
        case SP_Sprinkle: cup[i].sprinkle = 1; break;
    }
}

void PAtickFCT(){
    int i = 0;
    switch(PA_State){
        case PA_Start: PA_State = PA_Off; break;
        case PA_Off: PA_State = (ready) ? PA_Package : PA_Off; break;
        case PA_Package: PA_State = (ready) ? PA_Package : PA_Off; break;
        default: PA_State = PA_Start; break;
    }
    
    switch(PA_State){
        case PA_Package:
            for(i = 0; i < conLength; i++){
                if (cup[i].pos == paIndex){
                     if(cup[i].wrapper != 1){
                          printf("cupcake is missing a wrapper\n");
                          break;
                     }
                     if(cup[i].batter != 1){
                          printf("cupcake is missing batter\n");
                          break;
                     }
                     if(cup[i].baked != 1){
                          printf("cupcake is not baked\n");
                          break;
                     }
                     if(cup[i].frosted != 1){
                          printf("cupcake is not frosted\n");
                          break;
                     }
                     if(cup[i].sprinkle != 1){
                          printf("cupcake is missing sprinkles\n");
                          break;
                     }
                     completeCup++;
                     //printf("good cupcake number: %d\n", completeCup);
                     
                }
            }
            if (completeCup >= 8){
                completeCup -= 8;
                packageTot++;
                printf("%d packaged completed\n", packageTot);
            }
   }
}

void CONtickFCT(){
    int i = 0;
    switch(CON_State){//transitions
        case CON_Start: CON_State = CON_Off; break;
        case CON_Off: CON_State = (ready) ? CON_Move : CON_Off; break;
        case CON_Move: CON_State = (ready) ? CON_Move : CON_Off; break;
        default: CON_State = CON_Start; break;
    }
    
    
    switch(CON_State){
        case CON_Move: //move the cupcakes
            for(i = 0; i < conLength; i++){
                if(cup[i].pos != -1){
                    //printf("case: %d\n", cup[i].pos);
                    switch(cup[i].pos){
                        case 0: cup[i].pos = 1; break;
                        case 1: cup[i].pos = 2; break;
                        case 2:
                           if (cup[i].time < 2)
                              cup[i].time++;
                           else{
                               cup[i].pos = 3;
                               cup[i].time = 0;
                           }
                           break;
                        case 3:
                           if (cup[i].time < 9)
                              cup[i].time++;
                           else{
                               cup[i].pos++;
                               cup[i].time = 0;
                           }
                           break;
                        case 4:
                           if (cup[i].time < 2)
                              cup[i].time++;
                           else{
                               cup[i].pos++;
                               cup[i].time = 0;
                           }
                           break;
                        case 5:
                        case 6: cup[i].pos++; break;
                        case 7: 
                           cup[i].wrapper = 0;
                           cup[i].batter = 0;
                           cup[i].baked = 0;
                           cup[i].frosted = 0;
                           cup[i].sprinkle = 0;
                           cup[i].packaged = 0;
                           cup[i].pos = -1;
                           cup[i].time = 0;
                           break;
                        default:
                           printf("default\n");
                    }
                }
            }
            break;
         }
}
void DPtickFCT(){
    char display = 0x00;
    int i = 0;
    int j = 0;
    
    switch(DP_State){//transitions
         case DP_Start: DP_State = DP_Disp; break;
         case DP_Disp: DP_State = DP_Disp; break;
         default: DP_State = DP_Start; break;
    }
    
    
    switch(DP_State){//actions
        case DP_Disp:
            
            for(i = 0; i < conLength; i++){
                  if(cup[i].pos == 0) {display |= 0x01;}
                  if(cup[i].pos == 1) {display |= 0x02;}
                  if(cup[i].pos == 2) {display |= 0x04;}
                  if(cup[i].pos == 3) {display |= 0x08;}
                  if(cup[i].pos == 4) {display |= 0x10;}
                  if(cup[i].pos == 5) {display |= 0x20;}
                  if(cup[i].pos == 6) {display |= 0x40;}
                  if(cup[i].pos == 7) {display |= 0x80;}
            }
            break;
        }
        B = display;
}
