#ifndef _W25Q64_H
#define _W25Q64_H

  #include "sys.h"  
  
	//片选
  #define SPI_CS_H  GPIO_SetBits(GPIOA,GPIO_Pin_4)
  #define SPI_CS_L  GPIO_ResetBits(GPIOA,GPIO_Pin_4)
  
  void W25Q64_Init(void);
  void W25Q64_Write_Enable(void);
  u8 W25Q64_Read_Status(void);
  void W25Q64_Read_Bytes(u32 address,u32 num_byte,u8 *str);
  void W25Q64_Page_Write(u32 address,u16 num_byte,u8 *str);
  void W25Q64_Sector_Erase(u32 address);
  void W25Q64_Block_Erase(u32 address);
  void W25Q64_Chip_Erase(void);
  void W25Q64_Blocks_Erase(u32 address,u8 num_blcok);
	u16 W25Q64_Read_id(void);
#endif











