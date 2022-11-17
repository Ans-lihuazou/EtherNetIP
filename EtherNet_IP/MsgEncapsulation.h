#pragma once
#pragma pack(1)//强制内存对齐,系统不在填补

#include <vector>
#include <string>
#include <string.h>
#include <stdint.h>
#include <iostream>
#include <ctime>
#include <set>

namespace Tool {
	
	static std::set<uint16_t> setSerialNumber;

	static void initRand() {
		srand(time(NULL));
	}

	static uint32_t getRand() {
		
		return rand();
	}

	static uint16_t getConnectionSerialNumber() {
		
		uint16_t unNumber;
		while (true) {
			unNumber = getRand()&(0xFFFF);
			if (0 == setSerialNumber.count(unNumber)) {
				setSerialNumber.insert(unNumber);
				break;
			}
		}
		
		return unNumber;
	}

};

typedef enum omron_data_type {

	OMRON_BOOL_TYPE = 0xc1,
	OMRON_SINT_TYPE = 0xc2,
	OMRON_INT_TYPE = 0xc3,
	OMRON_DINT_TYPE = 0xc4,
	OMRON_LINT_TYPE = 0xc5,
	OMRON_USINT_TYPE = 0xc6,
	OMRON_UINT_TYPE = 0xc7,
	OMRON_UDINT_TYPE = 0xc8,
	OMRON_ULINT_TYPE = 0xc9,
	OMRON_REAL_TYPE = 0xca,
	OMRON_LREAL_TYPE = 0xcb,
	OMRON_BYTE_TYPE = 0xd1,
	OMRON_WORD_TYPE = 0xd2,
	OMRON_DWORD_TYPE = 0xd3,
	OMRON_LWORD_TYPE = 0xd4,
	OMRON_ENUM_TYPE = 0x07,
	OMRON_STRING_TYPE = 0xd0,
}OMRON_DATA_TYPE;



typedef enum eip_request_cmd {

	/* RegisterSession commands */
	EIP_REGISTER_CMD = 0x65,
	/* SendRRData commands */
	EIP_SENDRRDATA_CMD = 0x6f,
	/* SendUnitData commands */
	EIP_SENDUNITDATA_CMD = 0x70,
}EIP_REQUEST_CMD;

typedef enum cip_service_code {

	CIP_FORWARD_CLOSE_CODE = 0x4e,
	CIP_LARGE_FORWARD_OPEN_CODE = 0x5b,
	CIP_READ_CODE = 0x4c,
	CIP_WRITE_CODE = 0x4d,
}CIP_SERVICE_CODE;

typedef enum cip_request_types {

	CIP_READ_REQUEST_TYPE,
	CIP_WRITE_REQUEST_TYPE,
}CIP_REQUEST_TYPES;

typedef enum cip_response_types {

	CIP_READ_RESPONSE_TYPE,
	CIP_WRITE_RESPONSE_TYPE,
}CIP_RESPONSE_TYPES;


/*
*Defined in EtherNet-IP(Chinese)_Edition1.2.pdf P18:2-3.1
*Encapsulation header
*Fixed size is 24 bytes
**/
typedef struct tagEIPHEADER_T {

	uint16_t usEIPCommand;		/* 2 bytes, Encapsulate commands */
	uint16_t usDataLength;		/* 2 bytes, Length of data part */
	uint32_t unSessionHandle;	/* 4 bytes, conversation handle */
	uint32_t unStatus;			/* 4 bytes, Status code, valid as 0 */
	uint64_t ullSenderContext;	/* 8 bytes, Sender Context */
	uint32_t unOptions;			/* 4 bytes, Option, valid is 0 */
}EIPHEADER_T;

/*
*Defined in EtherNet-IP(Chinese)_Edition1.2.pdf P33:2-4.8
*Encapsulation data when there is no connection
*Fixed size is 16 bytes
**/
typedef struct tagUNCONNECT_EIPDATA_T {

	uint32_t unInterfaceHandl;		/* 4 bytes, Interface handle, CIP pattern is 0*/
	uint16_t usTimeOut;				/* 2 bytes, Timeout setting, 0 uses the default */

	/* P36:2-6.1 */
	uint16_t usItemCounts;			/* 2 bytes, item count, Usually an address item and a data item */
	uint16_t usAddressItemTypeID;	/* 2 bytes, address type id is 0 when there is no connection */
	uint16_t usAddressItemLength;	/* 2 bytes, address length is 0 when there is no connection */
	uint16_t usDataItemTypeID;		/* 2 bytes, data type id is 0xB2 when there is no connection */
	uint16_t usCIPMessageLength;	/* 2 bytes, CIP message length when there is no connection */
}UNCONNECT_EIPDATA_T;

/*
* Defined in EtherNet-IP(Chinese)_Edition1.2.pdf P33:2-4.8
* Encapsulation data when connected
* Fixed size is 22 bytes
* The definition here is not universal,  only for read and write operations
**/
typedef struct tagCONNECT_EIPDATA_T {

	uint32_t unInterfaceHandl;		/* 4 bytes, Interface handle, CIP pattern is 0*/
	uint16_t usTimeOut;				/* 2 bytes, Timeout setting, 0 uses the default */

	/* EtherNet-IP(Chinese)_Edition1.2.pdf P36:2-6.1 */
	uint16_t usItemCounts;			/* 2 bytes, item count, Usually an address item and a data item */
	uint16_t usAddressItemTypeID;	/* 2 bytes, address type id is 0xA1 when connected */
	uint16_t usAddressItemLength;	/* 2 bytes, address length is 4 when connected */
	uint32_t unConectionID;			/* 4 bytes, connect id */
	uint16_t usDataItemTypeID;		/* 2 bytes, data type id is 0xB1 when connected */
	uint16_t usCIPMessageLength;	/* 2 bytes, CIP message length when connected */
	uint16_t usCIPSquenceCount;		/* 2 bytes, CIP Serial Number */
}CONNECT_EIPDATA_T;


/*
* use in LARGE FORWARD OPEN REQUEST and FORWARD CLOSE REQUEST
* Put at the end of the message
* The relevant definitions are in CommonIndustrialProtocol-V1-3.3_2007.pdf P1228 C-1.4
**/
typedef struct tagCONNECTIONPATH_T {

	/* port */
	uint8_t ucPathSegment1;	/* 1 bytes */
	uint8_t ucLinkAddress;	/* 1 bytes */
	/* class */
	uint8_t ucPathSegment2;	/* 1 bytes */
	uint8_t ucClass;		/* 1 bytes */
	/* istance */
	uint8_t ucPathSegment3;	/* 1 bytes */
	uint8_t ucIstance;		/* 1 bytes */
}CONNECTIONPATH_T;


typedef struct tagUNCONNECT_COMMOMINDUSTRIALPROTOCOL_T {

	uint8_t ucServiceCode;
	uint8_t ucPathSize;
	/* class */
	uint8_t ucPathSegment1;	/* 1 bytes */
	uint8_t ucClass;		/* 1 bytes */
	/* istance */
	uint8_t ucPathSegment2;	/* 1 bytes */
	uint8_t ucIstance;		/* 1 bytes */
}UNCONNECT_COMMOMINDUSTRIALPROTOCOL_T;



/*
* use in FORWARD CLOSE REQUEST
* CLOASE REQUSET = CONNECT_EIPDATA_T + UNCONNECT_EIPDATA_T + UNCONNECT_COMMOMINDUSTRIALPROTOCOL_T + CLOSE_CIPCONNECTIOPNMANAGER_T
**/
typedef struct tagCLOSE_CIPCONNECTIOPNMANAGER_T {

	/*
	*2bytes
	*first bytes is tick time
	*second bytes is time-out tick
	*ActualTimeOut = 2^(tick time) * time-out tick
	**/
	uint16_t usActualTimeOut;
	/* 2bytes, the last two bits of the local IP  */
	uint16_t usConnectionSerialNumber;
	/* 2bytes, fixed value is 0x30 */
	uint16_t usOriginatorVendorID;
	/* 4 bytes, the meaning of the last two bytes is unknown, the first two bytes are the first two bits of the IP */
	uint32_t unOriginatorSerialNumber;
	/* 1 bytes, the count of connectionPath */
	uint8_t ucConnectionPathSize;
	/* 1 bytes, reserved field, usually 0 */
	uint8_t ucReserved;
	/* 2 bytes is port, 2 bytes is class, 2 bytes is istance */
	CONNECTIONPATH_T connectionPath;
}CLOSE_CIPCONNECTIOPNMANAGER_T;


/*
* OPEN REQUEST = CONNECT_EIPDATA_T + UNCONNECT_EIPDATA_T + UNCONNECT_COMMOMINDUSTRIALPROTOCOL_T + OPEN_CIPCONNECTIOPNMANAGER_T
*/
typedef struct tagOPEN_CIPCONNECTIOPNMANAGER_T {

	/*
	*2bytes
	*first bytes is tick time
	*second bytes is time-out tick
	*ActualTimeOut = 2^(tick time) * time-out tick
	**/
	uint16_t usActualTimeOut;

	/*
	* O->T T->O : in CommonIndustrialProtocol-V1-3.3_2007.pdf	P28 1-8
	* O->T: Originator to Target (used to describe packets that are sent from the originator to the target)
	* T->O: Target to Originator (used to describe packets that are sent from the target to the originator)
	*/

	/* 4 bytes, O->T connection identifier */
	uint32_t unOTNetworkConnectionID;
	/* 4 bytes, T->O connection identifier */
	uint32_t unTONetworkConnectionID;
	/* 2bytes, the last two bits of the local IP  */
	uint16_t usConnectionSerialNumber;
	/* 2bytes, fixed value is 0x30 */
	uint16_t usOriginatorVendorID;
	/* 4 bytes, the meaning of the last two bytes is unknown, the first two bytes are the first two bits of the IP */
	uint32_t unOriginatorSerialNumber;

	/*
	* 1 byte is Connection Time Out Multiplier, 3 bytes is Reserved
	* Connection Time Out Multiplier: Defined in CommonIndustrialProtocol-V1-3.3_2007.pdf P99 3-5.5.1.5
	* Reserved: reserved field, usually 0x000000
	*/
	uint32_t unConnectionTimeOutMulAndReserved;

	/* Network Connection Parameters: Defined in CommonIndustrialProtocol-V1-3.3_2007.pdf P95 3-5.5.1.1 */
	/* RPI: Defined in CommonIndustrialProtocol-V1-3.3_2007.pdf P104 3-5.5.2 Forward Open and Large_Forward_Open */

	uint32_t unOTRPI;
	uint32_t unOTNetworkConnectionParam;
	uint32_t unTORPI;
	uint32_t unTONetworkConnectionParam;

	/*
	* 1 bytes
	* Defined in CommonIndustrialProtocol-V1-3.3_2007.pdf P53 3-4.4.3
	* The eighth is  Direction bit, 0: the target end is the server; 1: the target end is the client
	* The fourth to seventh places are the value of Production Trigger:
	*		0: Cyclic
	*		1: Change Of State
	*		2: Application Object Triggered
	*		other: Reserved by CIP
	* The first to fourth places are the transportation category
	*/
	uint8_t ucTrigger;

	/* 1 bytes, count of connectionPath */
	uint8_t ucConnectionPathSize;
	CONNECTIONPATH_T connectionPath;
}OPEN_CIPCONNECTIOPNMANAGER_T;

typedef struct tagREAD_COMMOMINDUSTRIALPROTOCOL_T {

	uint8_t ucServiceCode;
	uint8_t ucPathSize;
	uint8_t ucPathSegmentType;
	uint8_t ucDataSize;
	std::string strANSISymbol;
}READ_COMMOMINDUSTRIALPROTOCOL_T;


#pragma pack()  /* close */ 

class CMsgEncapsulation {

public:

	CMsgEncapsulation();
	virtual ~CMsgEncapsulation();

	/* set */
	void setEHCommond(const uint16_t& c_usCommond);
	void setEHLength(const uint16_t& c_usLength);
	void setEHSessionHandle(const uint32_t& c_unHandle);

	void setCSDConnectID(const uint32_t& c_unID);
	void setCSDDataLength(const uint16_t& c_usLength);

	void setCIPServiceCode(const uint8_t& c_ucCode);

	void addPath(const uint16_t& c_usPath);
	void initPath();

	void setConnectionSerialNumber(const uint16_t& c_usIp);
	void setOriginatorSerialNumber(const uint16_t& c_usIp);

	void setANSISymbol(const std::string& c_strSymbol);

	/* void setValue(const uint32_t& c_usValue); */
	void setValue(void* c_usValue);
	void setDataType(const uint16_t& c_usDataType);

	void setOptionCount(const uint16_t& c_unOptCount);

	/* encapsulation message segment */
	int encapsulationEncapsulationHeader(uint8_t* pData, const uint32_t& c_unLength);
	int encapsulationUnConnectCommandSpecificData(uint8_t* pData, const uint32_t& c_unLength);
	int encapsulationCommonIndustrialProtocol(uint8_t* pData, const uint32_t& c_unLength);
	int encapsulationCIPConnectionManager(uint8_t* pData, const uint32_t& c_unLength);
	int encapsulationConnectCommandSpecificData(uint8_t* pData, const uint32_t& c_unLength);
	/* read */
	int encapsulationReadCommonIndustrialProtocol(uint8_t* pData, const uint32_t& c_unLength);
	int encapsulationReadCIPClassGeneric(uint8_t* pData, const uint32_t& c_unLength);
	/* write */
	int encapsulationWriteCIPClassGeneric(uint8_t* pData, const uint32_t& c_unLength);
	int encapsulationWriteCommonIndustrialProtocol(uint8_t* pData, const uint32_t& c_unLength);
	template<class T>
	int encapsulationWriteCIPClassGeneric(uint8_t* pData, const uint32_t& c_unLength);

	/* encapsulation request message */
	int encapsulationRegisterMessage(uint8_t* pData, const uint32_t& c_unLength);
	int encapsulationCloseMessage(uint8_t* pData, const uint32_t& c_unLength);
	int encapsulationOpenMessage(uint8_t* pData, const uint32_t& c_unLength);
	int encapsulationReadMessage(uint8_t* pData, const uint32_t& c_unLength);
	int encapsulationWriteMessage(uint8_t* pData, const uint32_t& c_unLength);

	template<class T>
	int encapsulationWriteMessage(uint8_t* pData, const uint32_t& c_unLength);

	/* Unpack, Suspend temporarily */
	int unPackReadResponse(uint8_t* pDate,const uint32_t& c_unLength);


private:

	/* Encapsulation Header */
	uint16_t m_usEHCommond;
	uint16_t m_usEHLength;
	uint32_t m_unEHSessionHandle;

	/* Command Specific Data */
	/* address item */
	uint32_t m_unCSDConnectID;
	/* data item */
	uint16_t m_usCSDDataLength;

	/* open Common Industrial Protocol */
	uint8_t m_ucCIPServiceCode;
	std::string m_strCIPPathSegment;

	/* Command Specific Data */
	uint16_t m_usConnectionSerialNumber;
	uint32_t m_usOriginatorSerialNumber;

	/* read Common Industrial Protocol */
	std::string m_strANSISymbol;
	//uint32_t m_usValue;
	void* m_pValue;
	uint16_t m_usDataType;

	uint16_t m_usOptionCount;
};

template<class T>
int CMsgEncapsulation::encapsulationWriteCIPClassGeneric(uint8_t * pData, const uint32_t & c_unLength) {

	int len = 0;
	if (len + sizeof(this->m_usDataType) > c_unLength) {
		return -1;
	}
	memcpy(pData, &this->m_usDataType, sizeof(this->m_usDataType));
	len += sizeof(this->m_usDataType);

	if (len + sizeof(this->m_usOptionCount) > c_unLength) {
		return -2;
	}
	memcpy(pData + len, &this->m_usOptionCount, sizeof(this->m_usOptionCount));
	len += sizeof(this->m_usOptionCount);

	if (len + sizeof(this->m_pValue) > c_unLength) {
		return -3;
	}

	T value = *(static_cast<T*>(this->m_pValue));
	memcpy(pData + len, &value, sizeof(value));
	len += sizeof(value);

	setCSDDataLength(len);

	return len;
}

template<class T>
inline int CMsgEncapsulation::encapsulationWriteMessage(uint8_t * pData, const uint32_t & c_unLength) {

	uint8_t szEncapsulationHeader[1024];
	uint8_t szCommandSpecificData[1024];
	uint8_t szCommonIndustrialProtocol[1024];
	uint8_t szCIPClassGeneric[1024];
	uint32_t len1, len2, len3, len4, len;

	len1 = len2 = len3 = len4 = len = 0;

	setEHCommond(EIP_SENDUNITDATA_CMD);
	setCIPServiceCode(CIP_WRITE_CODE);

	if ((len1 = encapsulationWriteCIPClassGeneric<T>(szCIPClassGeneric, sizeof(szCIPClassGeneric))) < 0) {
		return -1;
	}
	if ((len2 = encapsulationWriteCommonIndustrialProtocol(szCommonIndustrialProtocol, sizeof(szCommonIndustrialProtocol))) < 0) {
		return -2;
	}
	if ((len3 = encapsulationConnectCommandSpecificData(szCommandSpecificData, sizeof(szCommandSpecificData))) < 0) {
		return -3;
	}
	if ((len4 = encapsulationEncapsulationHeader(szEncapsulationHeader, sizeof(szEncapsulationHeader))) < 0) {
		return -4;
	}
	if (len + len4 > c_unLength) {
		return -5;
	}

	memcpy(pData, szEncapsulationHeader, len4);
	len += len4;

	if (len + len3 > c_unLength) {
		return -6;
	}
	memcpy(pData + len, szCommandSpecificData, len3);
	len += len3;

	if (len + len2 > c_unLength) {
		return -7;
	}
	memcpy(pData + len, szCommonIndustrialProtocol, len2);
	len += len2;

	if (len + len1 > c_unLength) {
		return -8;
	}
	memcpy(pData + len, szCIPClassGeneric, len1);
	len += len1;
	//std::cout << " read len : " << len1 << " " << len2 << " " << len3 << " " << len4 << "\n";

	return len;
}
