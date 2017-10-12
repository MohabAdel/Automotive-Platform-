#include "UDS.h"

#define ARRAY_VARS_ID_SIZE   3





static  uint8 tp_message[7];
static  uint8 message_len;
 g_VarsID_t ELEMENTS_UDS[ARRAY_VARS_ID_SIZE];

#define NEGATIVE_ID    0x7F
#define POSITIVE_ServiceID_OFFSET 0x40
#define POSITIVE_RES_LEN	0x03
#define WRITE_BY_ID_DATA_SIZE  4
#define none    0
#define Negative_Response_Len		3

/*
 *this file receive requests from tp layer for to performe tasks like read ID , write ID , Read DTC , 
 */
 
 
 
/****************************** Function Prototypes ******************************************/
//g_ServiceType_t UDS_check_requested_service(void);
g_errorUDS_t static UDS_Read_Value_by_ID(uint16 ID_requested,uint32* pValue,uint16* pLength);
g_errorUDS_t static UDS_Write_Value_by_ID(uint16 ID_requested,uint32 value);
 
void UDS_init(void){
  tp_Init();
}	


void UDS_Handle_Request_Message(void){ //Interract with TP layer
//	g_ServiceType_t Service_Type;
	tp_receiver(tp_message,&message_len);
	if(message_len!=0){
		//Service_Type=check_requested_service();  //decodes the request
		g_DiaRequest_t* RequestMessage;
		RequestMessage =(g_DiaRequest_t*)tp_message;
		if(RequestMessage->ServiceID == Read_by_ID){
			g_DiagReadIDRequest_t* ReadIDRequest;
			ReadIDRequest = (g_DiagReadIDRequest_t*)tp_message ;
			uint32 Element_Value=0;
			uint16 varLength=0;
			uint16 ElementID =((ReadIDRequest->ElementID_HighByte)<<8)|(ReadIDRequest->ElementID_LowByte);
		  g_errorUDS_t res;
			res=UDS_Read_Value_by_ID(ElementID,&Element_Value,&varLength); //this function searches the array of Structs (Element_VAlue&ID)
			if(res==ID_Not_Found){
			//send -ve Response with NRC => 
				g_DiagNegRes_t NegRes;
				NegRes.NegativeID=NEGATIVE_ID;
				NegRes.ServiceID=Read_by_ID;
				NegRes.NRC=READ_ID_NRC_REQUEST_OUT_OF_RANGE;
				for(int i=0;i<4;i++){
				 NegRes.RemByte[i]=0;
				}
				tp_sender((uint8*)&NegRes,Negative_Response_Len);
			}
			
			else if(res==PASS_UDS){
			//	
				g_DiagPosReadIDRes_t PosRes;
				PosRes.ServiceID=Read_by_ID + POSITIVE_ServiceID_OFFSET;
				PosRes.ElementID_HighByte=ReadIDRequest->ElementID_HighByte;
				PosRes.ElementID_LowByte=ReadIDRequest->ElementID_LowByte;
				for(int i=0;i<varLength;i++){
					PosRes.Data[i]=(Element_Value& (0x0FF<<(i*8)))>>(i*8);
				}
			//	PosRes.Data=Element_Value;
				tp_sender((uint8*)&PosRes,POSITIVE_RES_LEN+varLength);
			}
		}
		
		
		
		else if(RequestMessage->ServiceID == Write_by_ID){
			g_DiagWriteIDRequest_t* WriteIDRequest;
			WriteIDRequest = (g_DiagWriteIDRequest_t*)tp_message;
			
			uint16 ElementID =((WriteIDRequest->ElementID_HighByte)<<8)|(WriteIDRequest->ElementID_LowByte);
			uint32 value=0;
			for(int i=0 ; i<WRITE_BY_ID_DATA_SIZE;i++ ){
			value |= ((WriteIDRequest->Data[i])<<(i*8));
			}
			
			g_errorUDS_t res=UDS_Write_Value_by_ID(ElementID,value);

			 if(res== ID_Not_Found){
				//send -ve frame with NRC
				g_DiagNegRes_t NegRes;
				NegRes.NegativeID=NEGATIVE_ID;
				NegRes.ServiceID=Write_by_ID;
				NegRes.NRC=READ_ID_NRC_REQUEST_OUT_OF_RANGE;
				for(int i=0;i<4;i++){
				 NegRes.RemByte[i]=0;
				}
				tp_sender((uint8*)&NegRes,Negative_Response_Len);
			}
			else if(res==PASS_UDS){
				//send +ve frame
				g_DiagPosWriteIDRes_t PosRes;
				PosRes.ServiceID=Write_by_ID + POSITIVE_ServiceID_OFFSET;
				PosRes.ElementID_HighByte=WriteIDRequest->ElementID_HighByte;
				PosRes.ElementID_LowByte=WriteIDRequest->ElementID_LowByte;
	      
			//	PosRes.Data=Element_Value;
				tp_sender((uint8*)&PosRes,POSITIVE_RES_LEN);
				
			}
		}
	
		
	}
	else if(message_len >7){
		//generate Neg Res with NRC length not supported
	}
	
}

g_errorUDS_t UDS_Add_Vars_ID(g_VarsID_t Data){
	g_errorUDS_t res= PASS_UDS;
	static uint8 count=0;
	if(count == ARRAY_VARS_ID_SIZE){
	res = ID_ELEMENTS_FULL;
	}
	else{
		ELEMENTS_UDS[count]=Data;
		count ++;
	}
	
	
	return res;
	
	
}
/**** Another Approach of design ****/
/*
 *Design A function return with the address of the  Element  to the coresponding ID to use in Read ID / Write ID Tasks 
 */

g_errorUDS_t static UDS_Write_Value_by_ID(uint16 ID_requested,uint32 value){
	g_errorUDS_t res = ID_Not_Found;
	for(int i=0;i< ARRAY_VARS_ID_SIZE;i++){
		if(ELEMENTS_UDS[i].ID==ID_requested){
			(*(ELEMENTS_UDS[i].Pvar))=value;
			res = PASS_UDS;
			break;
		}
	}
			
	
return res;
	
}

g_errorUDS_t static UDS_Read_Value_by_ID(uint16 ID_requested,uint32* pValue,uint16* pLength){
	  g_errorUDS_t res=ID_Not_Found;
	  
	  
	  
	for(int i=0;i< ARRAY_VARS_ID_SIZE;i++){
		if(ELEMENTS_UDS[i].ID==ID_requested){
			*pValue=(*(ELEMENTS_UDS[i].Pvar));
			*pLength = ((ELEMENTS_UDS[i].varLengthInBytes));
			res = PASS_UDS;
			break;
		}
	}
	
	

  return res;
}



/*
g_ServiceType_t UDS_check_requested_service(void){
	g_ServiceType_t ServiceType;
	
	if(tp_message[0]==Read_by_ID){
		ServiceType=Read_by_ID;
	}
	else if(tp_message[0]==Write_by_ID){
		ServiceType=Write_by_ID;
	}
	else if(tp_message[0]==Read_DTC){
		ServiceType=Read_DTC;
	}
	
	
	return ServiceType;
}



*/
