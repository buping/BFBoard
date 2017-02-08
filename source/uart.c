/************************************
FileName:uart.c//uart驱动函数
Revision By:
Revised on 2014.02.01
Comments: Create
Innover Confidential
************************************/


#include "uart.h"
#include "Common.h"

INT8U  USART1_DMA_TX_Buf[USART1_DMA_TX_BUF_LEN] = {0};
INT8U  USART1_DMA_RX_Buf[USART1_DMA_RX_BUF_LEN] = {0x00};

INT8U  USART2_DMA_TX_Buf[USART2_DMA_TX_BUF_LEN] = {0};
INT8U  USART2_DMA_RX_Buf[USART2_DMA_RX_BUF_LEN] = {0x00};

void initUart(void)
{
  USART_InitTypeDef USART_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  if(gDeviceType == DEVICE_ENTRY) 
  {
  	USART_InitStructure.USART_BaudRate = RATE_115200;
  }
  else if(gDeviceType == DEVICE_CAR)
  {
  	USART_InitStructure.USART_BaudRate = RATE_19200;
  }
  else if(gDeviceType == DEVICE_EXIT)
  {
  	USART_InitStructure.USART_BaudRate = RATE_115200;
  }
  else
  {
    USART_InitStructure.USART_BaudRate = RATE_115200;
  }
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  STM_EVAL_COMInit(COM1, &USART_InitStructure);
  
  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //#if(!URAT_DMA_EN)
  NVIC_Init(&NVIC_InitStructure);
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
  //#endif  	
  USART_InitStructure.USART_BaudRate = RATE_57600;
  STM_EVAL_COMInit(COM2, &USART_InitStructure);
  
  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //#if(!URAT_DMA_EN)
  NVIC_Init(&NVIC_InitStructure);
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
  //#endif  
  
#if(PLC_MOTOR_UART_EN)
  USART_InitStructure.USART_BaudRate = RATE_115200;
  STM_EVAL_COMInit(COM3, &USART_InitStructure);
#endif
  
  /* Enable the USARTx Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  //#if(!URAT_DMA_EN)
  NVIC_Init(&NVIC_InitStructure);
  //#endif
  
#if(PLC_MOTOR_UART_EN)
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
#endif
  
#if(URAT_DMA_EN)
	DMA_Uart_Init(COM1);
	DMA_Uart_Init(COM2);
	initUartDMA();
#endif
	
}

void DMA_Uart_Init(INT8U usartNo)
{
  DMA_InitTypeDef DMA_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  
  if(usartNo == COM1)
  {
    /* DMA clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); // 开启DMA1时钟
		/*USART1 DMA Config ---*/
    DMA_Cmd(USART1_Tx_DMA_Channel, DISABLE);                           // 关DMA通道
    DMA_DeInit(USART1_Tx_DMA_Channel);                                 // 恢复缺省值
    DMA_InitStructure.DMA_PeripheralBaseAddr = (INT32U)(&USART1->TDR);// 设置串口发送数据寄存器
    DMA_InitStructure.DMA_MemoryBaseAddr = (INT32U)USART1_DMA_TX_Buf;         // 设置发送缓冲区首地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                      // 设置外设位目标，内存缓冲区 -> 外设寄存器
    DMA_InitStructure.DMA_BufferSize = 0;                     // 需要发送的字节数，这里其实可以设置为0，因为在实际要发送的时候，会重新设置次值
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        // 外设地址不做增加调整，调整不调整是DMA自动实现的
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 // 内存缓冲区地址增加调整
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 外设数据宽度8位，1个字节
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         // 内存数据宽度8位，1个字节
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           // 单次传输模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                 // 优先级设置
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                            // 关闭内存到内存的DMA模式
    DMA_Init(USART1_Tx_DMA_Channel, &DMA_InitStructure);               // 写入配置
    DMA_ClearFlag(USART1_Tx_DMA_FLAG);                                 // 清除DMA所有标志
    DMA_Cmd(USART1_Tx_DMA_Channel, DISABLE); // 关闭DMA
    DMA_ITConfig(USART1_Tx_DMA_Channel, DMA_IT_TC, ENABLE);            // 开启发送DMA通道中断
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
		
    /* Enable the DMA Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_Tx_DMA_IRQ;   // 发送DMA通道的中断配置
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // 优先级设置
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    //
    //USART DMA接收使能
    //
    DMA_InitTypeDef DMA_InitStructure;  
    /*--- COM_ZK_Rx_DMA_Channel DMA Config ---*/
    DMA_Cmd(USART1_Rx_DMA_Channel, DISABLE);                           // 关DMA通道  
    DMA_DeInit(USART1_Rx_DMA_Channel);                                 // 恢复缺省值
    DMA_InitStructure.DMA_PeripheralBaseAddr = (INT32U)(&USART1->RDR);// 设置串口接收数据寄存器
    DMA_InitStructure.DMA_MemoryBaseAddr = (INT32U)USART1_DMA_RX_Buf;         // 设置接收缓冲区首地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                      // 设置外设为数据源，外设寄存器 -> 内存缓冲区
    DMA_InitStructure.DMA_BufferSize = USART1_DMA_RX_BUF_LEN;                     // 需要最大可能接收到的字节数
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        // 外设地址不做增加调整，调整不调整是DMA自动实现的
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 // 内存缓冲区地址增加调整
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 外设数据宽度8位，1个字节
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         // 内存数据宽度8位，1个字节
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           // 单次传输模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                 // 优先级设置
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                            // 关闭内存到内存的DMA模式
    DMA_Init(USART1_Rx_DMA_Channel, &DMA_InitStructure);               // 写入配置
    DMA_ClearFlag(USART1_Rx_DMA_FLAG);                                 // 清除DMA所有标志
    //DMA_ITConfig(USART1_Rx_DMA_Channel, DMA_IT_TE, ENABLE);    
    //DMA_Cmd(USART1_Rx_DMA_Channel, ENABLE);                            // 开启接收DMA通道，等待接收数据    
    
    /* Enable the DMA Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_Rx_DMA_IRQ;   // 接收DMA通道的中断配置
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // 优先级设置
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
  }
  else if(usartNo == COM2)
  {
    /* DMA clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); // 开启DMA1时钟
		/*USART1 DMA Config ---*/
    DMA_Cmd(USART2_Tx_DMA_Channel, DISABLE);                           // 关DMA通道
    DMA_DeInit(USART2_Tx_DMA_Channel);                                 // 恢复缺省值
    DMA_InitStructure.DMA_PeripheralBaseAddr = (INT32U)(&USART2->TDR);// 设置串口发送数据寄存器
    DMA_InitStructure.DMA_MemoryBaseAddr = (INT32U)USART2_DMA_TX_Buf;         // 设置发送缓冲区首地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                      // 设置外设位目标，内存缓冲区 -> 外设寄存器
    DMA_InitStructure.DMA_BufferSize = 0;                     // 需要发送的字节数，这里其实可以设置为0，因为在实际要发送的时候，会重新设置次值
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        // 外设地址不做增加调整，调整不调整是DMA自动实现的
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 // 内存缓冲区地址增加调整
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 外设数据宽度8位，1个字节
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         // 内存数据宽度8位，1个字节
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           // 单次传输模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                 // 优先级设置
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                            // 关闭内存到内存的DMA模式
    DMA_Init(USART2_Tx_DMA_Channel, &DMA_InitStructure);               // 写入配置
    DMA_ClearFlag(USART2_Tx_DMA_FLAG);                                 // 清除DMA所有标志
    DMA_Cmd(USART2_Tx_DMA_Channel, DISABLE); // 关闭DMA
    DMA_ITConfig(USART2_Tx_DMA_Channel, DMA_IT_TC, ENABLE);            // 开启发送DMA通道中断
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
		
    /* Enable the DMA Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_Tx_DMA_IRQ;   // 发送DMA通道的中断配置
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // 优先级设置
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    //
    //USART DMA接收使能
    //
    DMA_InitTypeDef DMA_InitStructure;  
    /*--- COM_ZK_Rx_DMA_Channel DMA Config ---*/
    DMA_Cmd(USART2_Rx_DMA_Channel, DISABLE);                           // 关DMA通道  
    DMA_DeInit(USART2_Rx_DMA_Channel);                                 // 恢复缺省值
    DMA_InitStructure.DMA_PeripheralBaseAddr = (INT32U)(&USART2->RDR);// 设置串口接收数据寄存器
    DMA_InitStructure.DMA_MemoryBaseAddr = (INT32U)USART2_DMA_RX_Buf;         // 设置接收缓冲区首地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                      // 设置外设为数据源，外设寄存器 -> 内存缓冲区
    DMA_InitStructure.DMA_BufferSize = USART2_DMA_RX_BUF_LEN;                     // 需要最大可能接收到的字节数
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        // 外设地址不做增加调整，调整不调整是DMA自动实现的
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 // 内存缓冲区地址增加调整
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // 外设数据宽度8位，1个字节
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         // 内存数据宽度8位，1个字节
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           // 单次传输模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                 // 优先级设置
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                            // 关闭内存到内存的DMA模式
    DMA_Init(USART2_Rx_DMA_Channel, &DMA_InitStructure);               // 写入配置
    DMA_ClearFlag(USART2_Rx_DMA_FLAG);                                 // 清除DMA所有标志
    
    //DMA_ITConfig(USART2_Rx_DMA_Channel, DMA_IT_TE, ENABLE);    
    //DMA_Cmd(USART2_Rx_DMA_Channel, ENABLE);                            // 开启接收DMA通道，等待接收数据    
    
    
    
    /* Enable the DMA Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_Rx_DMA_IRQ;   // 接收DMA通道的中断配置
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // 优先级设置
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
  }
  
	
}


void initUartDMA(void)
{
	//USART1-TX   DMA1-Channel 4
	//USART1-RX   DMA1-Channel 5
	//USART3-TX   DMA1-Channel 2
	//USART3-RX   DMA1-Channel 3
  /* Enable USARTy DMA TX request */
  USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);  // 开启串口1 DMA发送			    
  USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);  // 开启串口1 DMA接收	
  
  USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);  // 开启串口2 DMA发送			    
  USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);  // 开启串口2 DMA接收
}

void USART1_DMA_Tx(INT16U size)                                                                      
{   
	USART1_Tx_DMA_Channel->CNDTR = (INT16U)size; // 设置要发送的字节数目   
  //USART1_Tx_DMA_Channel->CMAR = (INT32U)addr;
  //USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);  // 开启串口1 DMA发送
  DMA_Cmd(USART1_Tx_DMA_Channel, ENABLE);        //开始DMA发送                                             
}                                           

void USART1_DMA_Tx_Over(void)                                                                                
{                                                                                                                 
  DMA_ClearFlag(USART1_Tx_DMA_FLAG);         // 清除标志                                                   
  DMA_Cmd(USART1_Tx_DMA_Channel, DISABLE);   // 关闭DMA通道  
}

void USART1_DMA_Rx_Over(void)
{
  DMA_Cmd(USART1_Rx_DMA_Channel, DISABLE);                           // 关DMA通道  	
  //DMA_SetCurrDataCounter(USART1_Rx_DMA_Channel, USART1_DMA_RX_BUF_LEN);
  DMA_ClearFlag(USART1_Rx_DMA_FLAG);                                 // 清除DMA所有标志	
  DMA_Cmd(USART1_Rx_DMA_Channel, ENABLE);                            // 开启接收DMA通道，等待接收数据    	
  //gUSART_SIG_DMA_Last_Len = 0;
}

void USART2_DMA_Tx(INT16U size)                                                                      
{   
	USART2_Tx_DMA_Channel->CNDTR = (INT16U)size; // 设置要发送的字节数目   
  DMA_Cmd(USART2_Tx_DMA_Channel, ENABLE);        //开始DMA发送                                             
}                                           

void USART2_DMA_Tx_Over(void)                                                                                
{                                                                                                                 
  DMA_ClearFlag(USART2_Tx_DMA_FLAG);         // 清除标志                                                   
  DMA_Cmd(USART2_Tx_DMA_Channel, DISABLE);   // 关闭DMA通道  
}

void USART2_DMA_Rx_Over(void)
{
  DMA_Cmd(USART2_Rx_DMA_Channel, DISABLE);                           // 关DMA通道  	
  //DMA_SetCurrDataCounter(USART2_Rx_DMA_Channel, USART2_DMA_RX_BUF_LEN);
  DMA_ClearFlag(USART2_Rx_DMA_FLAG);                                 // 清除DMA所有标志	
  DMA_Cmd(USART2_Rx_DMA_Channel, ENABLE);                            // 开启接收DMA通道，等待接收数据    	
  //gUSART_SIG_DMA_Last_Len = 0;
}


