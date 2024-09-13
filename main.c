//
// LCD_Game_FlappyBird : emulate FlappyBird game on LCD
//
// EVB : Nu-LB-NUC140
// MCU : NUC140VE3CN
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "NUC100Series.h"
#include "MCU_init.h"
#include "SYS_init.h"
#include "LCD.h"
#include "Scankey.h"
#include "Seven_Segment.h"

#define Bird_Xsize  16
#define Bird_Ysize  16

// bitmap is in upside down order
unsigned char bmp_Bird[16*2] = {
	0xF8,0x08,0x08,0x08,0x1C,0x34,0xE4,0x04,0x74,0x9C,0x6C,0x6C,0xE8,0xF8,0xF0,0xC0,
	0x00,0x07,0x0E,0x1A,0x33,0x21,0x20,0x2E,0x3A,0x33,0x35,0x29,0x2B,0x3A,0x1F,0x0D
};

unsigned char GameTitle[128*8] ={
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x80,0xC0,0xE0,0x60,0x70,0x30,0x30,0x30,0x70,0x60,0x60,0xE0,0xC0,0xC0,0xE0,0xE0,0x70,0x38,0x38,0x1C,0x1C,0x0C,0x0E,0x0E,0x0E,0x06,0x06,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x87,0xC6,0x66,0x6E,0x2E,0x3E,0x3C,0x3C,0x3C,0x38,0x38,0x78,0x70,0xF0,0xE0,0xC0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0xFC,0xFF,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x03,0x07,0x0E,0x1C,0x38,0xF0,0xE0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF8,0xBE,0x03,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0xC1,0xC3,0x87,0x0F,0x3E,0xFC,0xF0,0xE0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x20,0xE0,0xE0,0x20,0x20,0x20,0x20,0x60,0xE0,0x00,0x00,0x10,0xF0,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x01,0x3F,0xFE,0xE0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x9F,0xFE,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x1F,0x70,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0xFF,0xF9,0xF0,0xF0,0xF1,0xFE,0x00,0x00,0x03,0xFF,0xFF,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xFF,0xFF,0x84,0x04,0x04,0x0E,0x00,0x00,0x00,0x00,0x80,0xFF,0xFF,0x80,0x00,0x00,0x70,0xFB,0x89,0x89,0x49,0xFF,0xFE,0x80,0x00,0x00,0x01,0xFF,0xFF,0x42,0x81,0x81,0xC3,0x7E,0x3C,0x00,0x00,0x01,0xFF,0xFF,0x42,0x81,0x81,0xC3,0x7E,0x3C,0x00,0x01,0x07,0x1F,0x79,0xE0,0x60,0x19,0x07,0x01,0x00,0x00,
	0x00,0x00,0x00,0x00,0x01,0x03,0x07,0xFE,0xFC,0xB8,0x30,0x30,0x70,0x70,0x60,0x60,0x70,0x30,0x30,0x30,0x18,0x18,0x0C,0x0C,0x06,0x03,0x01,0x00,0x00,0x00,0x00,0x80,0xC0,0xC0,0xE0,0x70,0x70,0x30,0x39,0x3B,0x1E,0x1C,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x38,0x39,0x33,0x31,0x71,0x60,0xE0,0xE0,0xF8,0x9F,0x8F,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x80,0x80,0x80,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04,0x07,0x07,0x44,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x04,0x07,0x07,0x04,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x06,0x02,0x01,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x07,0x1F,0x3E,0x78,0xF0,0xC0,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3E,0x7F,0xF3,0xC1,0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x08,0x10,0x10,0x10,0x30,0x30,0x30,0x20,0x60,0x60,0x60,0x60,0xE0,0xE0,0x60,0x61,0x63,0x63,0x7F,0x3E,0x18,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x10,0x10,0x10,0x1F,0xF7,0xE0,0x00,0x00,0x04,0xFD,0xFD,0x00,0x00,0x00,0x04,0xFC,0xFC,0x08,0x04,0x0C,0x0C,0x00,0xF0,0xF8,0x0C,0x04,0x04,0x08,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x03,0x07,0x0F,0x0E,0x1C,0x1C,0x38,0x38,0x70,0x70,0x70,0x60,0x60,0xE0,0xE0,0xE0,0xE0,0xE0,0xE0,0x61,0x63,0x67,0x67,0x76,0x3E,0x1C,0x1C,0x18,0x18,0x38,0x38,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x30,0x18,0x18,0x0F,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x03,0x03,0x02,0x02,0x02,0x02,0x03,0x01,0x00,0x00,0x02,0x03,0x03,0x02,0x00,0x00,0x02,0x03,0x03,0x02,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x02,0x02,0x01,0x03,0x03,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00	
};

unsigned char bmp_Alien1[16] = {
	0x00,0x70,0x18,0x7D,0x36,0x34,0x3C,0x3C,0x3C,0x3C,0x34,0x36,0x7D,0x18,0x70,0x00};

	
void Buzz(int8_t no)
{
	while(no!=0) {
		PB11=0;
		CLK_SysTickDelay(50000);
		PB11=1;
		CLK_SysTickDelay(50000);
	  no--;
	}
}

void Init_Buzz(void)
{
	GPIO_SetMode(PB, BIT11, GPIO_PMD_OUTPUT);
	PB11=1;
}
	
int check_bound(int8_t bird_x,int8_t bird_y){
	if(bird_x<0 || bird_x>110) return 0;
	if(bird_y<0 || bird_y>48) return 0;
	else return 1;
}

int check_hit(int8_t bx,int8_t by,int8_t mx,int8_t my){
	int8_t t1 = abs(bx-mx);
	int8_t t2 = abs(by-my);
	if( t1<=10 && t2<=10) return 1;
	else return 0;
}
void Display_7seg(uint16_t value)
{
  uint8_t digit;
	digit = value / 1000;
	CloseSevenSegment();
	ShowSevenSegment(3,digit);
	CLK_SysTickDelay(5000);
			
	value = value - digit * 1000;
	digit = value / 100;
	CloseSevenSegment();
	ShowSevenSegment(2,digit);
	CLK_SysTickDelay(5000);

	value = value - digit * 100;
	digit = value / 10;
	CloseSevenSegment();
	ShowSevenSegment(1,digit);
	CLK_SysTickDelay(5000);

	value = value - digit * 10;
	digit = value;
	CloseSevenSegment();
	ShowSevenSegment(0,digit);
	CLK_SysTickDelay(5000);
	CloseSevenSegment();
}

int main(void)
{
	int8_t final;
	int8_t bird_x, bird_y;
	int8_t mx, my;
	int8_t tx, ty;
	int temp;
	char Text[25];
	int8_t j;
	int8_t x,y;
	int8_t i,keyin;
	int8_t hit;
	uint16_t score, life;
	int8_t reme;
	int speed = 1;
	SYS_Init();
	Init_Buzz();  
	life = 4;
	score=0;
	final = 0;
	
	SYS_Init();
	OpenSevenSegment();
	init_LCD();
	clear_LCD();
  OpenKeyPad();// initialize 3x3 keypad
	//draw_LCD(GameTitle); // draw Game Title scene
	print_Line(0, "Gungry Bird");
	CLK_SysTickDelay(6000000);
	clear_LCD();
	CLK_SysTickDelay(5000);
	
	//led
		GPIO_SetMode(PC, BIT12, GPIO_MODE_OUTPUT);
		GPIO_SetMode(PC, BIT13, GPIO_MODE_OUTPUT);
		GPIO_SetMode(PC, BIT14, GPIO_MODE_OUTPUT);
		GPIO_SetMode(PC, BIT15, GPIO_MODE_OUTPUT);
		PC12 =0; // turn on LED
		PC13 =0;
		PC14 =0;
		PC15 =0;
	
	
	for (i=0;i<3;i++) CLK_SysTickDelay(1000000);
	clear_LCD();	
	
	keyin=ScanKey();
  while(keyin==0) { keyin=ScanKey();}
	clear_LCD();
	
	// set initial location of bird
	bird_x=0; bird_y=24;
	mx = 40, my = 50;
	
	while(1) {
		Display_7seg(score);
	  // Display Score & Life
			//sprintf(Text,"SCORE %3d        LIFE %3d",score, life);	
			x=0; y=0; printS_5x7(x,y,Text);			
			if (hit==0){}
				draw_Bmp16x16(bird_x, bird_y, FG_COLOR,BG_COLOR, bmp_Bird);
				draw_Bmp16x8(mx,my,FG_COLOR,BG_COLOR,bmp_Alien1);
				// Display Score & Life
				//sprintf(Text,"SCORE %3d        LIFE %3d",score, life);	
				x=0; y=0; printS_5x7(x,y,Text);			
			
				// Delay for Vision
				CLK_SysTickDelay(5000);
			
				draw_Bmp16x16(bird_x, bird_y, BG_COLOR,BG_COLOR, bmp_Bird); 		
				draw_Bmp16x8(mx,my,BG_COLOR,BG_COLOR,bmp_Alien1);
				
				keyin=ScanKey(); 
				CLK_SysTickDelay(5000);
				if(keyin == 0) keyin = reme;
				
				// moving bird
				if(keyin == 1){
						bird_x-=speed;
						bird_y-=speed;
						reme = 1;
				}
				else if(keyin == 3){
						bird_x+=speed;
						bird_y-=speed;
						reme = 3;
				}
				else if(keyin == 7){
						bird_x-=speed;
						bird_y+=speed;
						reme = 7;
				}
				else if(keyin == 9){
						bird_x+=speed;
						bird_y+=speed;
						reme = 9;
				}
				
				else if (keyin == 2) {
						bird_y-=speed;
						reme = 2;
				} 
				else if (keyin == 4) {
						bird_x-=speed;
						reme = 4;
				} 
				else if (keyin == 6) {
						bird_x+=speed;
						reme = 6;
				} 
				else if (keyin == 8) {
						bird_y+=speed;
						reme = 8;
				}
				
				// check the bird eat the moster or not , if yes score add one 
				if(check_hit(bird_x,bird_y,mx,my)==1){
					tx = bird_x;
					ty = bird_y;
					score++;
					final++;
					Buzz(1);
					/*print_Line(0, "score add one");
					CLK_SysTickDelay(600000);
					clear_LCD();					
					CLK_SysTickDelay(600000);*/
					do{
							mx = rand()%100 + 10;
							my = rand()%40 + 10;
					}while(mx == tx && my == ty);
					
					// if score largger than every 5 times it will speed up 
					if(final%5==0){
						speed++;
					}
				}
				// check the bird hit the bound or not , if yes life minus one
				if(check_bound(bird_x,bird_y) == 0){
					if(PC12==0){
								PC12 =1;
					}
					else if(PC13==0){
							PC13=1;
					}
					else if(PC14==0){
							PC14=1;
					}
					else if(PC15==0){
							PC15=1;
					}
					life--;
					if(life == 0){
						print_Line(0, "you die BYE BYE"); 
						for(j=0;j<1000;j++) Display_7seg(score);
						break;
					}					
					final = 0;
					speed = 1;
					print_Line(0, "life minus one"); 
					CLK_SysTickDelay(600000);
					clear_LCD();
					CLK_SysTickDelay(5000);
					bird_x = 20;
					bird_y = 20;
					reme = 0;
				}
			}
	
	}
	
	
