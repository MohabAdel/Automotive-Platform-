







#ifndef UDS_H_
#define UDS_H_

#include "tp_transiver.h"
//all frames here are 7 bytes only
typedef enum ServiceID{
	Session_Control=0x10,
	Tester_Present = 0x3E,
	ECU_Reset = 0x11,
	Comm_Control = 0x28,
	Read_by_ID =0x22,
	Write_by_ID = 0x2E,
	Routine_Control = 0x31,
	
	Read_DTC =0x19,
	Clear_DTC = 0x14,
	Control_DTC = 0x85,
	
	DOWNLOAD =0x34,
	TRANSFER =0x36,
	EXIT =0x37
	
}g_ServiceType_t;


typedef enum sub_function{
	HARD_RESET=0x01,
	ON_KEY=0x02,
	SW_RESET=0x03,
	R_DTC_NUMS_BY_SM=0x01,
	R_DTC_IDS_BY_SM=0x02,
	R_SNAP_SHOOT_DATA=0x04,
	R_EXTENDED_DATA_RECORD=0x06,
	R_SUPPORTED_DTC=0x0A,
	DEFAULT_SESSION=0x01,
	PROGRAMMING_SESSION=0x02,
	EXTENDED_SESSION=0x03
}g_SubFunction_t;

typedef enum NRC{
	READ_ID_NRC_INCORRECT_MSG_LEN_OR_INVALID_FORMAT=0x13,
	READ_ID_NRC_RESPONSE_TOO_LONG=0x14,
	READ_ID_NRC_CONDITIONS_NOT_CORRECT=0x22,
	READ_ID_NRC_REQUEST_OUT_OF_RANGE=0x31,
	READ_ID_NRC_SECURITY_ACCESS_DENIED=0x33
}g_NRC_t;


typedef struct DiagRequest{
uint8 ServiceID;
uint8 SubFunctionID;
uint8 RemBytes[5]; 	
}g_DiaRequest_t;

typedef struct DiagPosRes{
	uint8 ServiceID; //ServiceID+0x40
  uint8 sub_function; 	
	uint8 Data[5];
}g_DiagPosRes_t;

typedef struct DiagNegRes{
	uint8 NegativeID;
	uint8 ServiceID;
	uint8 NRC;
	uint8 RemByte[4];
}g_DiagNegRes_t;


/*
typedef struct DiagReadIDRequest{

	uint8 ServiceID;
	uint16 ElementID;
	uint8 RemaBytes[4];
}g_DiagReadIDRequest_t; //7 byte
*/

typedef struct DiagReadIDRequest{

	uint8 ServiceID;
	uint8 ElementID_HighByte;
	uint8 ElementID_LowByte;
	uint8 RemaBytes[4];
}g_DiagReadIDRequest_t;

typedef struct DiagPosReadIDRes{
	uint8 ServiceID;
	uint8 ElementID_HighByte;
	uint8 ElementID_LowByte;
	uint8 Data[4];
}g_DiagPosReadIDRes_t;



typedef struct DiagPosWriteIDRes{
	uint8 ServiceID;
	uint8 ElementID_HighByte;
	uint8 ElementID_LowByte;
	uint8 Data[4];
}g_DiagPosWriteIDRes_t;

typedef struct DiagWriteIDRequest{
	uint8 ServiceID;
	uint8 ElementID_HighByte;
	uint8 ElementID_LowByte;
	uint8 Data[4];
}g_DiagWriteIDRequest_t;


typedef struct VARS_ID{
	uint16 ID;
	uint32* Pvar;
	uint16 varLengthInBytes;
}g_VarsID_t;

typedef enum ERROR_UDS{
	PASS_UDS , FAIL , ID_ELEMENTS_FULL, ID_Not_Found
	
}g_errorUDS_t;
void UDS_init(void);
g_errorUDS_t UDS_Add_Vars_ID(g_VarsID_t Data);
void UDS_Handle_Request_Message(void);

#endif
