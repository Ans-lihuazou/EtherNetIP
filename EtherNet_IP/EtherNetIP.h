#pragma once

#ifdef _WIN32

#include <WinSock2.h>
#include <WS2tcpip.h>

#elif __linux__
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#endif

#include "MsgEncapsulation.h"

class CEtherNetIP {
public:

	CEtherNetIP();
	virtual ~CEtherNetIP();

#ifdef _WIN32
	
	int init(SOCKET socket);
#elif __linux__

	int init(int nSocket);
#endif

	int read(const std::string& request, const uint32_t& c_unCount = 1);

	template<class T>
	int write(const std::string& c_strTag, T& c_usValue, const uint16_t& c_usDataType);

private:

	CMsgEncapsulation m_msgEncapsulation;

	uint32_t m_unOTNetworkConnectionID;
	uint32_t m_unTONetworkConnectionID;

#ifdef _WIN32
	SOCKET m_sClient;

#elif __linux__

	int m_sClient;
#endif

};

template<class T>
inline int CEtherNetIP::write(const std::string& c_strTag, T& c_usValue, const uint16_t& c_usDataType) {
	EIPHEADER_T EIPHeader;
	uint8_t szWriteRequest[1024] = { 0 };
	int len = 0;
	m_msgEncapsulation.setCSDConnectID(m_unOTNetworkConnectionID);
	m_msgEncapsulation.setANSISymbol(c_strTag);
	//m_msgEncapsulation.setValue(c_unValue);
	void* temp = static_cast<void*>(&c_usValue);
	m_msgEncapsulation.setValue(temp);
	m_msgEncapsulation.setDataType(c_usDataType);
	m_msgEncapsulation.setOptionCount(1);
	len = m_msgEncapsulation.encapsulationWriteMessage<T>(szWriteRequest, sizeof(szWriteRequest));
	//std::cout << " request len is " << len << "\n";
	if (len <= 0) {

		std::cout << "error: message\n";
		return -1;
	}

	send(m_sClient, (char*)szWriteRequest, len, 0);

	recv(m_sClient, (char*)&EIPHeader, 24, 0);

	if (EIPHeader.unStatus != 0) {
		return -2;
	}

	if (EIPHeader.usDataLength == 0) {
		return -3;
	}

	memset(szWriteRequest, 0, sizeof(szWriteRequest));

	int recvLength = recv(m_sClient, (char*)szWriteRequest, EIPHeader.usDataLength, 0);

	if (recvLength != EIPHeader.usDataLength) {
		return -4;
	}

	if (szWriteRequest[24] != 0) {
		return -5;
	}

	return 0;
}