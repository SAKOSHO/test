/**********************************************************************
 * File         :spi.h                       
 * Date         :2020/8/4
 * Summary      :Use for spi communication
 * CPU type     :PIC16F886
 * Compiler     :XC8
 * Author       :Akaru Oishi
* ***********************************************************************/



#ifndef __SPI_H
#define __SPI_H

/*---Define---*/
#define _SSPSTAT_CONFIG                     0b01000000                          //CKE�ibit6�j��1�i�����オ��ő��M�j
#define _SSPCON_CONFIG                      0b00100000                          //SPI�}�X�^���[�h�A�N���b�N��Fosc/4�A�N���b�N��Low�ŃA�C�h���A    
#define _XTAL_FREQ                          20000000

/*--Prototype--*/
void spi_init(void);
char spi_putc(char c);

#endif