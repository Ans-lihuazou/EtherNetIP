#include "EtherNetIP.h"

int main() {

	Tool::initRand();
	CEtherNetIP netIp;

#ifdef _WIN32
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	/* test WSA */
	if (WSAStartup(sockVersion, &data) != 0) {

		std::cout << "error: WSAStartup\n";
		return 0;
	}

	/* CREATE SOCKET */
	SOCKET sClient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sClient == INVALID_SOCKET) {

		std::cout << "invalid socket!\n";
		return 0;
	}

	/* CONNECT SERVER */
	sockaddr_in serAddr;
	/* IPV4 */
	serAddr.sin_family = AF_INET;
	/* PORT */
	serAddr.sin_port = htons(44818);
	/* IP */
	inet_pton(AF_INET, "192.168.56.126", &serAddr.sin_addr.s_addr);
	if (connect(sClient, (sockaddr *)&serAddr, sizeof(serAddr)) == SOCKET_ERROR) {

		std::cout << "connect error !" << std::endl;
		closesocket(sClient);
		return 0;
	}

	netIp.setSocket(sClient);

#elif __linux__

	struct sockaddr_in addr;
	int nSocket;

	/* initialize variable */
	memset(&addr, 0, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(44818);
	addr.sin_addr.s_addr = inet_addr("192.168.56.126");


	/* creat socket */
	nSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (nSocket < 0) {
		std::cout << "error: create socket is fail\n";
		return -1;
	}

	/* connect server */
	if (connect(nSocket, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
		std::cout << "error: connect is fail\n";
		return -1;
	}

	netIp.setSocket(nSocket);

#endif

	std::cout << "init return value is " << netIp.init() << "\n";

	std::cout << "read return value is " << netIp.read("dwordTest") << "\n";

	//int(16),word(16),byte(8),sint(8),dword(32)
	//using dataType = typename uint16_t;
	typedef uint16_t dataType;
	std::string tag = "wordTest";
	dataType value = 0xff;
	uint16_t type = OMRON_WORD_TYPE;
	std::cout << "write return value is " << netIp.write<dataType>(tag, value, type) << "\n";

	return 0;
}