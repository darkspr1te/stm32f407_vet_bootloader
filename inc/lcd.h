#ifndef __LCD_H__
#define __LCD_H__

#include "stm32f4xx.h"

#define LCD_REG      (*((volatile unsigned short *) 0x60000000)) 
#define LCD_RAM      (*((volatile unsigned short *) 0x6003FFFE)) 
//LCD_RAM = data;
//LCD_REG = command;
#define  ILI9325

#define Lcd_Light_ON    PDout(11)=1
#define Lcd_Light_OFF   PDout(11)=0
#define Lcd_SET()         PDout(6)=1
#define Lcd_RESET()       PDout(6)=0

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

#define I8    signed char
#define U8  unsigned char     /* unsigned 8  bits. */
#define I16   signed short    /*   signed 16 bits. */
#define U16 unsigned short    /* unsigned 16 bits. */
#define I32   signed long   /*   signed 32 bits. */
#define U32 unsigned long   /* unsigned 32 bits. */
#define I16P I16              /*   signed 16 bits OR MORE ! */
#define U16P U16              /* unsigned 16 bits OR MORE ! */

/* Private typedef -----------------------------------------------------------
typedef struct
{
  u16 LCD_REG;
  u16 LCD_RAM;
} LCD_TypeDef;
*/

/* LCD is connected to the FSMC_Bank1_NOR/SRAM4 and NE4 is used as chip select signal */
//**#define LCD_BASE        ((u32)(0x6C000000 | 0x0000007E))	//skyblue       //A6,NE4
//**#define LCD_BASE           ((uint32_t)(0x60000000 | 0x0C000000))                //A0,NE4
#define LCD_BASE           ((uint32_t)(0x60000000 | 0x0003FFFE))              //A17,NE1

//#define LCD         ((LCD_TypeDef *) LCD_BASE)

uint16_t DeviceCode;

void init_lcd(void);
void init_FSMC(void);
void LCD_setWindowArea(uint16_t StartX, uint16_t StartY, uint16_t Width, uint16_t Height);
void ili9320_Clear(u16 Color);





/* LCD color */
#ifndef 	White
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0
#endif

/* LCD Registers */
#ifndef     R0 

#define R0             0x00
#define R1             0x01
#define R2             0x02
#define R3             0x03
#define R4             0x04
#define R5             0x05
#define R6             0x06
#define R7             0x07
#define R8             0x08
#define R9             0x09
#define R10            0x0A
#define R12            0x0C
#define R13            0x0D
#define R14            0x0E
#define R15            0x0F
#define R16            0x10
#define R17            0x11
#define R18            0x12
#define R19            0x13
#define R20            0x14
#define R21            0x15
#define R22            0x16
#define R23            0x17
#define R24            0x18
#define R25            0x19
#define R26            0x1A
#define R27            0x1B
#define R28            0x1C
#define R29            0x1D
#define R30            0x1E
#define R31            0x1F
#define R32            0x20
#define R33            0x21
#define R34            0x22
#define R36            0x24
#define R37            0x25
#define R40            0x28
#define R41            0x29
#define R43            0x2B
#define R45            0x2D
#define R48            0x30
#define R49            0x31
#define R50            0x32
#define R51            0x33
#define R52            0x34
#define R53            0x35
#define R54            0x36
#define R55            0x37
#define R56            0x38
#define R57            0x39
#define R59            0x3B
#define R60            0x3C
#define R61            0x3D
#define R62            0x3E
#define R63            0x3F
#define R64            0x40
#define R65            0x41
#define R66            0x42
#define R67            0x43
#define R68            0x44
#define R69            0x45
#define R70            0x46
#define R71            0x47
#define R72            0x48
#define R73            0x49
#define R74            0x4A
#define R75            0x4B
#define R76            0x4C
#define R77            0x4D
#define R78            0x4E
#define R79            0x4F
#define R80            0x50
#define R81            0x51
#define R82            0x52
#define R83            0x53
#define R96            0x60
#define R97            0x61
#define R106           0x6A
#define R118           0x76
#define R128           0x80
#define R129           0x81
#define R130           0x82
#define R131           0x83
#define R132           0x84
#define R133           0x85
#define R134           0x86
#define R135           0x87
#define R136           0x88
#define R137           0x89
#define R139           0x8B
#define R140           0x8C
#define R141           0x8D
#define R143           0x8F
#define R144           0x90
#define R145           0x91
#define R146           0x92
#define R147           0x93
#define R148           0x94
#define R149           0x95
#define R150           0x96
#define R151           0x97
#define R152           0x98
#define R153           0x99
#define R154           0x9A
#define R157           0x9D
#define R192           0xC0
#define R193           0xC1
#define R229           0xE5
#endif


#define HyalineBackColor 0x0001  // Lihao 透明背景

#ifndef Line0
#define Line0          0
#define Line1          24
#define Line2          48
#define Line3          72
#define Line4          96
#define Line5          120
#define Line6          144
#define Line7          168
#define Line8          192
#define Line9          216


#define Horizontal     0x00
#define Vertical       0x01
#endif



#define  HDP  799  //Horizontal Display Period     //**
#define  HT   1000 //Horizontal Total
#define  HPS  51  //LLINE Pulse Start Position
#define  LPS  3   //	Horizontal Display Period Start Position
#define  HPW  8   //	LLINE Pulse Width


#define  VDP  479	//Vertical Display Period
#define  VT   530	//Vertical Total
#define  VPS  24	//	LFRAME Pulse Start Position
#define  FPS  23	//Vertical Display Period Start Positio
#define  VPW  3 	// LFRAME Pulse Width     //**

#define MAX_HZ_POSX HDP+1
#define MAX_HZ_POSY VDP+1 


//LCD
#define SSD_HOR_RESOLUTION		800		//LCD
#define SSD_VER_RESOLUTION		480		//LCD
//LCD
#define SSD_HOR_PULSE_WIDTH		1		//
#define SSD_HOR_BACK_PORCH		46		//
#define SSD_HOR_FRONT_PORCH		210		//

#define SSD_VER_PULSE_WIDTH		1		//
#define SSD_VER_BACK_PORCH		23		//
#define SSD_VER_FRONT_PORCH		22		//
//
#define SSD_HT	(SSD_HOR_RESOLUTION+SSD_HOR_BACK_PORCH+SSD_HOR_FRONT_PORCH)
#define SSD_HPS	(SSD_HOR_BACK_PORCH)
#define SSD_VT 	(SSD_VER_RESOLUTION+SSD_VER_BACK_PORCH+SSD_VER_FRONT_PORCH)
#define SSD_VPS (SSD_VER_BACK_PORCH)
#endif