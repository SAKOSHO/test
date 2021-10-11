/*************************************************************************
 * File         :spi.c                       				 
 * Date         :2020/8/4					 
 * Summary      :Use for spi communication			
 * CPU type     :PIC16F886						 
 * Compiler     :XC8							 
 * Author       :Akaru Oishi						 
 * Description  :		 
 *              :				 
 *              :		 
 *              :			 
 * ***********************************************************************

/*---File Include---*/
#include <xc.h>
#include <stdio.h>
#include <string.h>
#include "spi.h"

/*---Initiation---*/
void spi_init(void){
    SSPCON = _SSPCON_CONFIG;
    SSPSTAT = _SSPSTAT_CONFIG;
    INTCONbits.GIE = 1;                                                         //���荞�ݐݒ�
    INTCONbits.PEIE = 1;                                                        //���荞�ݐݒ�
    PIE1bits.SSPIE = 0;                                                         //SSP���荞�݂��C�l�[�u��
    PIR1bits.SSPIF = 0;                                                         //SSP���荞�݃t���O���N���A
    TRISAbits.TRISA4 = 0;                                                       //CS�s�����o�͐ݒ�
    TRISAbits.TRISA5 = 0;                                                       //CS�s�����o�͐ݒ�
    TRISCbits.TRISC3 = 0;                                                       //CLK�s�����o�͐ݒ�
    TRISCbits.TRISC4 = 1;                                                       //DI�s������͐ݒ�
    TRISCbits.TRISC5 = 0;                                                       //DO�s�����o�͐ݒ�
    PORTAbits.RA4 = 1;                                                          //CS�s����High�őҋ@
    PORTAbits.RA5 = 1;                                                          //CS�s����High�őҋ@
}

/*---Communication---*/
char spi_putc(char c){
    char dumy;                                                                  //�_�~�[���[�h�p�ϐ��錾
    char data;                                                                  //�f�[�^�i�[�p�ϐ��錾
    
    dumy = SSPBUF;                                                              //SSP�o�b�t�@�_�~�[���[�h                                                           
    if(!SSPSTATbits.BF){                                                        //���M�p���W�X�^�t���O�`�F�b�N�i���0�j
        SSPBUF = c;                                                             
    }
    while(!SSPSTATbits.BF);                                                     //���M�i��M�j�����҂�
    data = SSPBUF;                                                              //�o�b�t�@���[�h
    
    //__delay_us(1);
    return data;
}