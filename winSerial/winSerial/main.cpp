#include "WinSerial.h"
#include <stdio.h>

BOOL g_bThreadFlag = TRUE;
DWORD WINAPI ThreadFunc(LPVOID arg);
char TxBuf[20];


// Threading function
DWORD WINAPI ThreadFunc(LPVOID arg) {
	CWinSerial* pSerial = (CWinSerial*)arg;

	char RxBuf[4096];
	int iReadSize;

	// data received by serial communication
	while (g_bThreadFlag) {
		if (pSerial->available() != 0) {
			// Display received strings
			iReadSize = pSerial->readBytes(RxBuf, 4096);
			RxBuf[iReadSize] = '\0';
			printf("%s", RxBuf);
		}
	}

	return 0;
}


int main()
{
	CWinSerial serial;

	if (!serial.setup(10, 57600)) {
		printf("Port Open Error.\n");
		exit(1);
	}
	printf("Port Open Success.\n");
	printf("Press [Enter] key to exit ... ");

	serial.flush();

	// Generate thread for serial data receive
	DWORD dwThreadID;
	HANDLE hThread = CreateThread(NULL, 0, ThreadFunc, LPVOID(&serial), 0, &dwThreadID);

	while (true) {
		// serial.writeBytes(TxBuf, 20); // write data
		if (GetAsyncKeyState(VK_RETURN)) {
			g_bThreadFlag = FALSE;
			WaitForSingleObject(hThread, INFINITE);	// Waiting for thread to close
			break;
		}
	}

	serial.close();

	return 0;
}

