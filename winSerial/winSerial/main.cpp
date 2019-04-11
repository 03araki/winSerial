#include "WinSerial.h"
#include <stdio.h>
#include <conio.h>

BOOL g_bThreadFlag = TRUE;
DWORD WINAPI ThreadFunc(LPVOID arg);

int main()
{
	CWinSerial serial;

	if (!serial.setup(3, 115200)) {
		printf("Port Open Error.\n");
		printf("\nPress any key to exit ...");
		_getch();
		exit(1);
	}
	printf("Port Open Success.\n");

	serial.flush();
	serial.write('2');	// マイコンへの処理開始命令


	// 読み取り用スレッドの生成
	DWORD dwThreadID;
	HANDLE hThread = CreateThread(NULL, 0, ThreadFunc, LPVOID(&serial), 0, &dwThreadID);

	_getch();	// 入力待ち（キー入力でスレッド終了処理）
	g_bThreadFlag = FALSE;
	serial.write(' ');	// マイコンへ測定終了を指令
	WaitForSingleObject(hThread, INFINITE);	// スレッド終了待ち（スレッドが終了するまでプログラム停止）

	serial.close();

	printf("\nPress any key to continue ...");
	_getch();

	return 0;
}

// スレッド処理で呼び出される関数
DWORD WINAPI ThreadFunc(LPVOID arg) {
	CWinSerial* pSerial = (CWinSerial*)arg;

	char RxBuf[1024];
	int iReadSize;

	while (g_bThreadFlag) {
		if (pSerial->available() != 0) {
			// 受け取った文字列をそのまま表示
			iReadSize = pSerial->readBytes(RxBuf, 1024);
			RxBuf[iReadSize] = '\0';
			printf("%s", RxBuf);
		}
	}

	return 0;
}