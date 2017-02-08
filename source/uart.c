/************************************
FileName:uart.c//uart��������
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
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); // ����DMA1ʱ��
		/*USART1 DMA Config ---*/
    DMA_Cmd(USART1_Tx_DMA_Channel, DISABLE);                           // ��DMAͨ��
    DMA_DeInit(USART1_Tx_DMA_Channel);                                 // �ָ�ȱʡֵ
    DMA_InitStructure.DMA_PeripheralBaseAddr = (INT32U)(&USART1->TDR);// ���ô��ڷ������ݼĴ���
    DMA_InitStructure.DMA_MemoryBaseAddr = (INT32U)USART1_DMA_TX_Buf;         // ���÷��ͻ������׵�ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                      // ��������λĿ�꣬�ڴ滺���� -> ����Ĵ���
    DMA_InitStructure.DMA_BufferSize = 0;                     // ��Ҫ���͵��ֽ�����������ʵ��������Ϊ0����Ϊ��ʵ��Ҫ���͵�ʱ�򣬻��������ô�ֵ
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        // �����ַ�������ӵ�����������������DMA�Զ�ʵ�ֵ�
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 // �ڴ滺������ַ���ӵ���
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // �������ݿ��8λ��1���ֽ�
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         // �ڴ����ݿ��8λ��1���ֽ�
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           // ���δ���ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                 // ���ȼ�����
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                            // �ر��ڴ浽�ڴ��DMAģʽ
    DMA_Init(USART1_Tx_DMA_Channel, &DMA_InitStructure);               // д������
    DMA_ClearFlag(USART1_Tx_DMA_FLAG);                                 // ���DMA���б�־
    DMA_Cmd(USART1_Tx_DMA_Channel, DISABLE); // �ر�DMA
    DMA_ITConfig(USART1_Tx_DMA_Channel, DMA_IT_TC, ENABLE);            // ��������DMAͨ���ж�
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
		
    /* Enable the DMA Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_Tx_DMA_IRQ;   // ����DMAͨ�����ж�����
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // ���ȼ�����
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    //
    //USART DMA����ʹ��
    //
    DMA_InitTypeDef DMA_InitStructure;  
    /*--- COM_ZK_Rx_DMA_Channel DMA Config ---*/
    DMA_Cmd(USART1_Rx_DMA_Channel, DISABLE);                           // ��DMAͨ��  
    DMA_DeInit(USART1_Rx_DMA_Channel);                                 // �ָ�ȱʡֵ
    DMA_InitStructure.DMA_PeripheralBaseAddr = (INT32U)(&USART1->RDR);// ���ô��ڽ������ݼĴ���
    DMA_InitStructure.DMA_MemoryBaseAddr = (INT32U)USART1_DMA_RX_Buf;         // ���ý��ջ������׵�ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                      // ��������Ϊ����Դ������Ĵ��� -> �ڴ滺����
    DMA_InitStructure.DMA_BufferSize = USART1_DMA_RX_BUF_LEN;                     // ��Ҫ�����ܽ��յ����ֽ���
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        // �����ַ�������ӵ�����������������DMA�Զ�ʵ�ֵ�
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 // �ڴ滺������ַ���ӵ���
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // �������ݿ��8λ��1���ֽ�
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         // �ڴ����ݿ��8λ��1���ֽ�
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           // ���δ���ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                 // ���ȼ�����
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                            // �ر��ڴ浽�ڴ��DMAģʽ
    DMA_Init(USART1_Rx_DMA_Channel, &DMA_InitStructure);               // д������
    DMA_ClearFlag(USART1_Rx_DMA_FLAG);                                 // ���DMA���б�־
    //DMA_ITConfig(USART1_Rx_DMA_Channel, DMA_IT_TE, ENABLE);    
    //DMA_Cmd(USART1_Rx_DMA_Channel, ENABLE);                            // ��������DMAͨ�����ȴ���������    
    
    /* Enable the DMA Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_Rx_DMA_IRQ;   // ����DMAͨ�����ж�����
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // ���ȼ�����
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
  }
  else if(usartNo == COM2)
  {
    /* DMA clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); // ����DMA1ʱ��
		/*USART1 DMA Config ---*/
    DMA_Cmd(USART2_Tx_DMA_Channel, DISABLE);                           // ��DMAͨ��
    DMA_DeInit(USART2_Tx_DMA_Channel);                                 // �ָ�ȱʡֵ
    DMA_InitStructure.DMA_PeripheralBaseAddr = (INT32U)(&USART2->TDR);// ���ô��ڷ������ݼĴ���
    DMA_InitStructure.DMA_MemoryBaseAddr = (INT32U)USART2_DMA_TX_Buf;         // ���÷��ͻ������׵�ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;                      // ��������λĿ�꣬�ڴ滺���� -> ����Ĵ���
    DMA_InitStructure.DMA_BufferSize = 0;                     // ��Ҫ���͵��ֽ�����������ʵ��������Ϊ0����Ϊ��ʵ��Ҫ���͵�ʱ�򣬻��������ô�ֵ
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        // �����ַ�������ӵ�����������������DMA�Զ�ʵ�ֵ�
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 // �ڴ滺������ַ���ӵ���
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // �������ݿ��8λ��1���ֽ�
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         // �ڴ����ݿ��8λ��1���ֽ�
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           // ���δ���ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                 // ���ȼ�����
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                            // �ر��ڴ浽�ڴ��DMAģʽ
    DMA_Init(USART2_Tx_DMA_Channel, &DMA_InitStructure);               // д������
    DMA_ClearFlag(USART2_Tx_DMA_FLAG);                                 // ���DMA���б�־
    DMA_Cmd(USART2_Tx_DMA_Channel, DISABLE); // �ر�DMA
    DMA_ITConfig(USART2_Tx_DMA_Channel, DMA_IT_TC, ENABLE);            // ��������DMAͨ���ж�
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
		
    /* Enable the DMA Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_Tx_DMA_IRQ;   // ����DMAͨ�����ж�����
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // ���ȼ�����
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    //
    //USART DMA����ʹ��
    //
    DMA_InitTypeDef DMA_InitStructure;  
    /*--- COM_ZK_Rx_DMA_Channel DMA Config ---*/
    DMA_Cmd(USART2_Rx_DMA_Channel, DISABLE);                           // ��DMAͨ��  
    DMA_DeInit(USART2_Rx_DMA_Channel);                                 // �ָ�ȱʡֵ
    DMA_InitStructure.DMA_PeripheralBaseAddr = (INT32U)(&USART2->RDR);// ���ô��ڽ������ݼĴ���
    DMA_InitStructure.DMA_MemoryBaseAddr = (INT32U)USART2_DMA_RX_Buf;         // ���ý��ջ������׵�ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;                      // ��������Ϊ����Դ������Ĵ��� -> �ڴ滺����
    DMA_InitStructure.DMA_BufferSize = USART2_DMA_RX_BUF_LEN;                     // ��Ҫ�����ܽ��յ����ֽ���
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        // �����ַ�������ӵ�����������������DMA�Զ�ʵ�ֵ�
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 // �ڴ滺������ַ���ӵ���
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; // �������ݿ��8λ��1���ֽ�
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         // �ڴ����ݿ��8λ��1���ֽ�
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           // ���δ���ģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                 // ���ȼ�����
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;                            // �ر��ڴ浽�ڴ��DMAģʽ
    DMA_Init(USART2_Rx_DMA_Channel, &DMA_InitStructure);               // д������
    DMA_ClearFlag(USART2_Rx_DMA_FLAG);                                 // ���DMA���б�־
    
    //DMA_ITConfig(USART2_Rx_DMA_Channel, DMA_IT_TE, ENABLE);    
    //DMA_Cmd(USART2_Rx_DMA_Channel, ENABLE);                            // ��������DMAͨ�����ȴ���������    
    
    
    
    /* Enable the DMA Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = USART2_Rx_DMA_IRQ;   // ����DMAͨ�����ж�����
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;     // ���ȼ�����
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
  USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);  // ��������1 DMA����			    
  USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);  // ��������1 DMA����	
  
  USART_DMACmd(USART2, USART_DMAReq_Tx, ENABLE);  // ��������2 DMA����			    
  USART_DMACmd(USART2, USART_DMAReq_Rx, ENABLE);  // ��������2 DMA����
}

void USART1_DMA_Tx(INT16U size)                                                                      
{   
	USART1_Tx_DMA_Channel->CNDTR = (INT16U)size; // ����Ҫ���͵��ֽ���Ŀ   
  //USART1_Tx_DMA_Channel->CMAR = (INT32U)addr;
  //USART_DMACmd(USART1, USART_DMAReq_Tx, ENABLE);  // ��������1 DMA����
  DMA_Cmd(USART1_Tx_DMA_Channel, ENABLE);        //��ʼDMA����                                             
}                                           

void USART1_DMA_Tx_Over(void)                                                                                
{                                                                                                                 
  DMA_ClearFlag(USART1_Tx_DMA_FLAG);         // �����־                                                   
  DMA_Cmd(USART1_Tx_DMA_Channel, DISABLE);   // �ر�DMAͨ��  
}

void USART1_DMA_Rx_Over(void)
{
  DMA_Cmd(USART1_Rx_DMA_Channel, DISABLE);                           // ��DMAͨ��  	
  //DMA_SetCurrDataCounter(USART1_Rx_DMA_Channel, USART1_DMA_RX_BUF_LEN);
  DMA_ClearFlag(USART1_Rx_DMA_FLAG);                                 // ���DMA���б�־	
  DMA_Cmd(USART1_Rx_DMA_Channel, ENABLE);                            // ��������DMAͨ�����ȴ���������    	
  //gUSART_SIG_DMA_Last_Len = 0;
}

void USART2_DMA_Tx(INT16U size)                                                                      
{   
	USART2_Tx_DMA_Channel->CNDTR = (INT16U)size; // ����Ҫ���͵��ֽ���Ŀ   
  DMA_Cmd(USART2_Tx_DMA_Channel, ENABLE);        //��ʼDMA����                                             
}                                           

void USART2_DMA_Tx_Over(void)                                                                                
{                                                                                                                 
  DMA_ClearFlag(USART2_Tx_DMA_FLAG);         // �����־                                                   
  DMA_Cmd(USART2_Tx_DMA_Channel, DISABLE);   // �ر�DMAͨ��  
}

void USART2_DMA_Rx_Over(void)
{
  DMA_Cmd(USART2_Rx_DMA_Channel, DISABLE);                           // ��DMAͨ��  	
  //DMA_SetCurrDataCounter(USART2_Rx_DMA_Channel, USART2_DMA_RX_BUF_LEN);
  DMA_ClearFlag(USART2_Rx_DMA_FLAG);                                 // ���DMA���б�־	
  DMA_Cmd(USART2_Rx_DMA_Channel, ENABLE);                            // ��������DMAͨ�����ȴ���������    	
  //gUSART_SIG_DMA_Last_Len = 0;
}


