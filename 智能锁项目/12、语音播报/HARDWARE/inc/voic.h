#ifndef _VOIC_H
#define _VOIC_H

	#include "sys.h"
	
	#define   VO_DATA_H     GPIO_SetBits(GPIOB,GPIO_Pin_0)
	#define   VO_DATA_L     GPIO_ResetBits(GPIOB,GPIO_Pin_0)
	
	#define   VO_BUSY_HL    GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)
	
	/*语音对应的标号*/
	#define CHANGE_ADMIN_PASSWORD	   1 //修改管理员密码
	#define SET_DOOR_PASSWORD	   		 2 //设置开门密码
	#define REGISTER_FINGER					 3 //登记指纹
	#define REGISTER_CARD					   4 //登记卡片
	#define CHANGE_TIME					   	 5 //修改时间
	#define DELETE_ALLFINGER				 6 //删除所有指纹
	#define DELETE_ASSIGN_FINGER		 7 //删除指定指纹
	#define RECOVER_ALL							 8 //恢复出厂设置
	#define CHANGE_VOLUME						 9 //设置音量
	#define DELETE_ALL_CARD				   10 //删除所有门卡
	#define DELETE_ASSIGN_CARD			 11//删除指定门卡
	#define INPUT_ADMIN_PASSWORD     12//请输入管理员密码  
	#define INPUT_NEW_PASSWORD 			 13//请输入新密码       
	#define INPUT_NEW_PASSWORD_AGAIN 14//请再次输入新密码 
	#define PASSWORD_ERROR     			 15//验证失败
	#define PASSWORD_INCONFORMITY		 16//密码不一致
	#define SETTING_SUCCESS					 17//操作成功
	#define PASSWORD_REPETITION			 18//密码重复
	#define PUTCARD 								 19//请放置卡片
	#define CARD_REGISTERED 				 20//卡重复             
	#define DELETE_ALL_FIGNER		 		 21//是否删除
	#define DELETE_ASSIGNFIGNER		 	 22//请放手指
	#define FINGER_REPETITION 		 	 23//指纹重复
	#define DOOROPEN_SUCCESS				 24//欢迎回家
	#define DOOROPEN_FAIL					 	 25//开门失败
	#define CONNECT_SUCCESS          26//蓝牙连接成功
	#define TIPS                     27//按#号确认，按*号退出
	#define REPETE                   28//请重新设置
	#define Di 											 29//嘟                
	#define MenLing 					       30//叮咚 叮咚       
	#define Warm 							       31//报警声    

	void Voic_Init(void);
	void Voic_SendData(u8 data);

#endif



