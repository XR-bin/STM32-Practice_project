ALIENTEK STM32开发板SYSTEM文件夹，采用V3.5库的头文件(stm32f10x.h、system_stm32f10x.h和
core_cm3.h等几个文件放在:SYSTEM\sys 文件夹里面)，使得代码可以完美移植到库函数里面使用，
不过需要注意，在新建工程的时候，请在：Options for Target-->C/C++ 选项卡里面 Preprocessor
 Symbols 栏定义你的STM32芯片容量。比如:ALIENTEK战舰STM32开发板用户，使用的是大容量的STM32
芯片，则在Define栏输入：STM32F10X_HD 

对于STM32F103系列芯片，设置原则如下：
16KB≤FLASH≤32KB       选择：STM32F10X_LD
64KB≤FLASH≤128KB      选择：STM32F10X_MD
256KB≤FLASH≤512KB     选择：STM32F10X_HD

                                                   正点原子@ALIENTEK
                                                   广州市星翼电子科技有限公司 
                                                   传真：020-36773971
                                                   电话：020-38271790
                                                   网址：www.alientek.com
                                                   论坛：www.openedv.com
 