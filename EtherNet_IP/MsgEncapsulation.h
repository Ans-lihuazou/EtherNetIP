#ifndef MESSAGE_ENCAPSULATION_H__20221117
#define MESSAGE_ENCAPSULATION_H__20221117

#include "GlobalTool.h"

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
	int unPackReadResponse(uint8_t* pDate, const uint32_t& c_unLength);


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

#endif /* MESSAGE_ENCAPSULATION_H__20221117 */