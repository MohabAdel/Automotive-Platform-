


#include <stdint.h>
//#include "CPX/PLL/PLL.h"
#include "CPX/UART/UART.h"
#include "SAL/UDS.h"
#include "os/os.h"
#include "SAL/tp_transiver.h"
//Interactive Task with TP layer
int count0,count1,count2,count3,count4,count5,count6;
#define THREADFREQ 1000
uint8 DataMain[40];
uint8 DataLen;


void Task0(){

			while(1){
				 count0++;
					/*							
				 tp_receiver(DataMain,&DataLen);
				 count0++;
				 if(DataLen !=0){
				 UART_OutStringLen((char*)DataMain,DataLen);
				 }	
				*/
				
				UDS_Handle_Request_Message();
	     }
}

//Idle Task
void Task1(){
	while(1){
		//tp_sender("ROUND 7 Test X", 15);
		count1++;
		
	}
}

void Task2(){
	while(1){
		count2++;
	}
}

void Task3(){
	while(1){
		count3++;
	}
}

void Task4(){
	while(1){
		count4++;
	}
}

void Task5(){
	while(1){
		count5++;
	}
}

void Task6(){
	while(1){
		count6++;
	}
}

/*
//---------------------OutCRLF---------------------
// Output a CR,LF to UART to go to a new line
// Input: none
// Output: none
void static OutCRLF(void){
  UART_OutChar(CR);
  UART_OutChar(LF);
}
*/
//debug code
 char ch;
int main(void){
   uint32 SteeringAngle =0x12345678 ,RaderY=0x76859432;
   uint32 RaderX=0x76894537;

	
	OS_Init();
  UDS_init();
	g_VarsID_t Dummy;

  Dummy.ID= 0x5151;
	Dummy.Pvar =&SteeringAngle;
	Dummy.varLengthInBytes=4;
	
  UDS_Add_Vars_ID(Dummy);
	Dummy.ID= 0xD001;
	Dummy.Pvar =&RaderX;
	Dummy.varLengthInBytes=4;
	UDS_Add_Vars_ID(Dummy);
	Dummy.ID= 0xD002;
	Dummy.Pvar =&RaderY;
	Dummy.varLengthInBytes=4;
	UDS_Add_Vars_ID(Dummy);	
	
//	UDS_Write_Value_by_ID(0x5151,0x20458798);
//  UDS_Write_Value_by_ID(0xD001,0x20458798);
//	UDS_Write_Value_by_ID(0xD002,0x20458798);
//	UDS_Read_Value_by_ID(0x5151);
	
  OS_AddThreads(&Task0,0,&Task1,0,&Task2,0,&Task3,0,&Task4,0,&Task5,0,&Task6,0);
		
//	UDS_init();
	OS_Launch(OS_Clock_GetFreq()/THREADFREQ); // doesn't return, interrupts enabled in here
	while(1){
  // tp_sender("Mohab Adel bla",15);
		

	}
}




