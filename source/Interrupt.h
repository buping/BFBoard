#ifndef	_INTERRUPT_H
#define _INTERRUPT_H

#include "Common.h"

typedef struct	InitVectTab
{
	INT16U	INTx;
	INT16U	INTy[12];
}INTVECTTAB;

#define	INT1_MASK		0x0001
#define INT2_MASK		0x0002
#define	INT3_MASK		0x0004
#define INT4_MASK		0x0008
#define	INT5_MASK		0x0010
#define INT6_MASK		0x0020
#define	INT7_MASK		0x0040
#define INT8_MASK		0x0080
#define	INT9_MASK		0x0100
#define INT10_MASK		0x0200
#define	INT11_MASK		0x0400
#define INT12_MASK		0x0800
#define	INT13_MASK		0x1000
#define INT14_MASK		0x2000
#define	RTOSINT_MASK	0x4000
#define DLOGINT_MASK	0x8000


#define NMI_MASK		0x0001

#define	SOFT_INT1_MASK  0
#define SOFT_INT2_MASK	0
#define	SOFT_INT3_MASK  0
#define SOFT_INT4_MASK	0
#define	SOFT_INT5_MASK  0
#define SOFT_INT6_MASK	0
#define	SOFT_INT7_MASK  0
#define SOFT_INT8_MASK	0
#define	SOFT_INT9_MASK  0
#define SOFT_INT10_MASK	0
#define	SOFT_INT11_MASK  0
#define SOFT_INT12_MASK	0


#define	EN_T2PINT		{ SETBIT(IER,INT3_MASK);SETBIT(PIEIER3,INT1_MASK);}
#define DIS_T2PINT      { CLRBIT(IER,INT3_MASK);CLRBIT(PIEIER3,INT1_MASK);}

#define EN_XINT1		{ SETBIT(IER,INT1_MASK);SETBIT(PIEIER3,INT4_MASK);}		
#define DIS_XINT1		{ CLRBIT(IER,INT1_MASK);CLRBIT(PIEIER3,INT4_MASK);}		
// Non-Peripheral Interrupts:
interrupt void INT13_ISR(void);     // XINT13 or CPU-Timer 1
interrupt void INT14_ISR(void);     // CPU-Timer2
interrupt void DATALOG_ISR(void);   // Datalogging interrupt
interrupt void RTOSINT_ISR(void);   // RTOS interrupt
interrupt void EMUINT_ISR(void);    // Emulation interrupt
interrupt void NMI_ISR(void);       // Non-maskable interrupt
interrupt void ILLEGAL_ISR(void);   // Illegal operation TRAP
interrupt void USER1_ISR(void);     // User Defined trap 1
interrupt void USER2_ISR(void);     // User Defined trap 2
interrupt void USER3_ISR(void);     // User Defined trap 3
interrupt void USER4_ISR(void);     // User Defined trap 4
interrupt void USER5_ISR(void);     // User Defined trap 5
interrupt void USER6_ISR(void);     // User Defined trap 6
interrupt void USER7_ISR(void);     // User Defined trap 7
interrupt void USER8_ISR(void);     // User Defined trap 8
interrupt void USER9_ISR(void);     // User Defined trap 9
interrupt void USER10_ISR(void);    // User Defined trap 10
interrupt void USER11_ISR(void);    // User Defined trap 11
interrupt void USER12_ISR(void);    // User Defined trap 12
//interrupt void OSCtxSw(void);    // User Defined trap 12

// Group 1 PIE Interrupt Service Routines:
interrupt void  PDPINTA_ISR(void);   // EV-A
interrupt void  PDPINTB_ISR(void);   // EV-B
interrupt void  XINT1_ISR(void);     
interrupt void  XINT2_ISR(void);
interrupt void  ADCINT_ISR(void);    // ADC
interrupt void  TINT0_ISR(void);     // Timer 0
interrupt void  WAKEINT_ISR(void);   // WD

// Group 2 PIE Interrupt Service Routines:
interrupt void CMP1INT_ISR(void);   // EV-A
interrupt void CMP2INT_ISR(void);   // EV-A
interrupt void CMP3INT_ISR(void);   // EV-A
interrupt void T1PINT_ISR(void);    // EV-A
interrupt void T1CINT_ISR(void);    // EV-A
interrupt void T1UFINT_ISR(void);   // EV-A
interrupt void T1OFINT_ISR(void);   // EV-A
      
// Group 3 PIE Interrupt Service Routines:
interrupt void T2PINT_ISR(void);    // EV-A
interrupt void T2CINT_ISR(void);    // EV-A
interrupt void T2UFINT_ISR(void);   // EV-A
interrupt void T2OFINT_ISR(void);   // EV-A
interrupt void CAPINT1_ISR(void);   // EV-A
interrupt void CAPINT2_ISR(void);   // EV-A
interrupt void CAPINT3_ISR(void);   // EV-A
      
// Group 4 PIE Interrupt Service Routines:
interrupt void CMP4INT_ISR(void);   // EV-B
interrupt void CMP5INT_ISR(void);   // EV-B
interrupt void CMP6INT_ISR(void);   // EV-B
interrupt void T3PINT_ISR(void);    // EV-B
interrupt void T3CINT_ISR(void);    // EV-B
interrupt void T3UFINT_ISR(void);   // EV-B
interrupt void T3OFINT_ISR(void);   // EV-B
     
// Group 5 PIE Interrupt Service Routines:
interrupt void T4PINT_ISR(void);    // EV-B
interrupt void T4CINT_ISR(void);    // EV-B
interrupt void T4UFINT_ISR(void);   // EV-B
interrupt void T4OFINT_ISR(void);   // EV-B
interrupt void CAPINT4_ISR(void);   // EV-B
interrupt void CAPINT5_ISR(void);   // EV-B
interrupt void CAPINT6_ISR(void);   // EV-B

// Group 6 PIE Interrupt Service Routines:
interrupt void SPIRXINTA_ISR(void);   // SPI
interrupt void SPITXINTA_ISR(void);    // SPI
interrupt void MRINTA_ISR(void);    // McBSP
interrupt void MXINTA_ISR(void);    // McBSP
 
   
// Group 9 PIE Interrupt Service Routines:
interrupt void SCIRXINTA_ISR(void); // SCI-A
interrupt void SCITXINTA_ISR(void); // SCI-A
interrupt void SCIRXINTB_ISR(void); // SCI-B
interrupt void SCITXINTB_ISR(void); // SCI-B
interrupt void ECAN0INTA_ISR(void); // eCAN
interrupt void ECAN1INTA_ISR(void); // eCAN


// Catch-all for Reserved Locations For testing purposes:
interrupt void PIE_RESERVED(void);       // Reserved for test
interrupt void rsvd_ISR(void);           // for test
#endif

