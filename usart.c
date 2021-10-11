/*************************************************************************
 * File         :usart.c                       				 
 * Date         :2020/1/15						 
 * Summary      :Use for usart(Serial communication)			
 * CPU type     :PIC16F886						 
 * Compiler     :XC8							 
 * Author       :Akaru Oishi						 
 * Description  :�g�p����INTCONbits��GIE,PEIE���Z�b�g���邱��		 
 *              :TRISC���o�͂Ɏw�肷�邱��				 
 *              :���荞�ݏ������[�`���ɉ��L�𒣂�t���邱��		 
 *              :���M���̑��M�f���~�^��LF���܂߂邱��			 
 * ***********************************************************************
 * ---���荞�ݎ�M�p�R�s�y---						 
 * if(PIR1bits.RCIF){							
 *       usart_save();							 
 * }									 
 ************************************************************************/


/*---File Include---*/
#include <xc.h>
#include <stdio.h>
#include <string.h>
#include "usart.h"


/*---Variable---*/
char strage[_RX_STRAGE_SIZE];


/*---Initiation---*/
void _usart_init(void)
{
    TXSTA = _USART_TX_CONFIG;                                                   //�V���A���ʐM�̑��M�ݒ�
    RCSTA = _USART_RX_CONFIG;                                                   //�V���A���ʐM�̎�M�ݒ�
    SPBRGH = _USART_BAUDRATE_H_9600;                                            //�{�[���[�g�̐ݒ�
    SPBRG = _USART_BAUDRATE_L_9600;                                             //�{�[���[�g�̐ݒ�
    BAUDCTLbits.BRG16 = 1;                                                      //SPBRG���W�X�^��16bit�w����C�l�[�u��
    INTCONbits.GIE = 1;                                                         //���荞�ݐݒ�
    INTCONbits.PEIE = 1;                                                        //���荞�ݐݒ�
    PIE1bits.RCIE  = 1;                                                         //��M���荞�݂��C�l�[�u��
    PIR1bits.RCIF = 0;                                                          //��M���荞�݃t���O�����Z�b�g�A���̍s���Ԃ񂢂��
}

/*---Transmission---*/
void _putch(unsigned char c)
{
    while(!TXIF);
    TXREG = c;
}

void usart_puts(char* s)
{
    while(*s){
        putch(*s);
        s++;
        __delay_ms(10);
    }

}


/*---Save---*/
void usart_save(void){
    if(!strchr(strage,'\n')){                                                   //strage��LF���Ȃ���Έ�������strage�ɒǉ�
        char* p1 = strage;
        char* p2 = strage;
        
        while(*p1++ != '\0'){
            p2++;
        }
        *p2 = RCREG;
    }
    else{                                                                       //�������strage��������&�V���b�Z�[�W
        memset(strage,'\0',_RX_STRAGE_SIZE);
        *strage = RCREG;
    }
}


/*---Get---*/
char* usart_get(void){
    if(!strchr(strage,'\n') || strage[0] == '\r' || strage[0] == '\n'){        //�ł����ݓr���iLF�����j�������͋�̃��b�Z�[�W�̏ꍇget���Ȃ�
        return 0;
    }
    else{
        return strage;
    }
}





