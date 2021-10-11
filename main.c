/* 
 * File:   main.c
 * Author: 佐々木翔
 *
 * Created on 2021/08/07, 12:37
 * Discription delayで1s間隔でRA0をON/OFF
 * 
 */

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "spi.h"
#include "MCP2515.h"
#include "CSS.h"
#include "fram.h"
#include "usart.h"

// CONFIG1
#pragma config FOSC  = HS       // Oscillator Selection bits (HS oscillator: High-speed crystal/resonator on RA6/OSC2/CLKOUT and RA7/OSC1/CLKIN)
#pragma config WDTE  = OFF      // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = ON       // RE3/MCLR pin function select bit (RE3/MCLR pin function is MCLR)
#pragma config CP    = OFF      // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD   = OFF      // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = ON       // Brown Out Reset Selection bits (BOR enabled)
#pragma config IESO  = OFF      // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP   = OFF      // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR21V   // Brown-out Reset Selection bit (Brown-out Reset set to 2.1V)
#pragma config WRT   = OFF      // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#define _XTAL_FREQ      20000000                                                //PICの周波数を20MHzに設定
#define _CAN_BAUDRATE   2                                                       //ボーレート


/*Prtotype*/
void CONFIG();                                                                  //初期設定
char BtoD(char data);                                                           //2進数を10進数に変換
void interrupt isr();                                                           //割り込み処理

char end_flag = 0;          
char *rx_data;                                                                  //受信データ格納変数
char *fram_data;                                                                //framのデータ格納変数
char rx_status;                                                                 //受信状態
char rx_int;                                                                    //受信割り込み
char mode = _ChargeMode;                                                        //初期モード
char size;                                                                      //サイズ変数


void main(void)
{
    int data[3][3] = {{1,2,3}, {4,5,6}, {7,8,9}};
    int i,j;
    
    CONFIG();                                                                   //初期設定
    __delay_ms(100);
    
    while(1)
    {
        RA0 = 0;
       __delay_ms(1000);
       
       RA0 = 1;
        
    }
}

void CONFIG()
{
    OSCCON = 0b01101000;
    ANSEL  = 0b00000000;
    ANSELH = 0b00000000;
    TRISA  = 0b00000000; 
    TRISB  = 0b00000000;
    TRISC  = 0b00000000;
    PORTA  = 0b00000000;
    PORTB  = 0b00000000;
    PORTC  = 0b00000000;
    
    spi_init();
    __delay_ms(100);
    
    MCP2515_init(_CAN_BAUDRATE);                                                //とりあえず，動作している2にした．理解はまだ
    Write(_TXB0DLC , 0b00001000);                                               //メッセージサイズ8byte
    MCP2515_Open(1);                                                            //とりあえず，0にした．理解はまだ
}

char BtoD(char data)
{
    char  binary;
    char decimal = 0;
    char bas = 1;
            
    binary = data & 0b00001111;
    
    while(binary>0)
    {
        decimal = decimal + (binary % 10) * bas;
        binary = binary / 10;
        bas = bas * 2;
    }
    
    return decimal;
}

void interrupt isr()
{
    rx_int = Read(_CANINTF);                                                    //割り込みフラグ確認
        
    if((rx_int & _Flagbit0) == 0b00000001)                                      //受信処理
    {   
        Write(_CANINTF, 0b00000000);                                            //割り込みフラグのクリア
        
        Write(_TXB0DLC , 0b00001000);                                           //メッセージサイズ8byte
        Load_TX_ID(_F_TXB0SIDH, 0, 0b00001000, 0, 0);                           //送信識別子（拡張フレーム）の設定
        Load_TX_Data(_F_TXB0D0, 8, 0);                                          //送信データ格納
        RTS0_CSS(_CAN_BAUDRATE);                                                //送信要求
    }
    
    INTCONbits.INTF = 0;                                                        //割り込みフラグクリア
    
}

void putch(unsigned char c)
{
    while(!TXIF);
    TXREG = c;
}