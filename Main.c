///***********************************************************************///
///                    >>  Line Following Robot  <<                      ///
///     Program Description: This program is for the PIC microcontroller ///
///       to control a line-following robot.                             ///
///       Created by: Saeed Soukiah                                      ///
///***********************************************************************///
#include <xc.h>
#define _XTAL_FREQ 20000000 
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)
void main()
{
TRISB2 = 1; TRISB3 = 1; //Bath the IR sensor pins are declared as input
TRISB4 = 0; TRISB5 = 0; //Motor 1 pins declared as output
TRISB6 = 0; TRISB7 = 0; //Motor 2 pins declared as output
while(1)
{
if (RB2==1 && RB3==1) //Both sensor not over balck line
{
RB4=0; RB5=1; //Motor 1 forward
RB6=1; RB7=0; //Motor 2 forward
}
else if (RB2==0 && RB3==1) //Left sensor is over black line
{
RB4=1; RB5=1; //Motor 1 stop
RB6=1; RB7=0; //Motor 2 forward
} 
else if (RB2==1 && RB3==0) //Right sensor is over black line
{
RB4=0; RB5=1; //Motor 1 forward
RB6=1; RB7=1; //Motor 2 stop
} 
else //Both Sensor over black line
{
RB4=1; RB5=1; //Motor 1 stop
RB6=1; RB7=1; //Motor 2 stop 
} 
}
}
