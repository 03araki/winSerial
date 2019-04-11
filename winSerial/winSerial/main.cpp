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
	serial.write('2');	// �}�C�R���ւ̏����J�n����


	// �ǂݎ��p�X���b�h�̐���
	DWORD dwThreadID;
	HANDLE hThread = CreateThread(NULL, 0, ThreadFunc, LPVOID(&serial), 0, &dwThreadID);

	_getch();	// ���͑҂��i�L�[���͂ŃX���b�h�I�������j
	g_bThreadFlag = FALSE;
	serial.write(' ');	// �}�C�R���֑���I�����w��
	WaitForSingleObject(hThread, INFINITE);	// �X���b�h�I���҂��i�X���b�h���I������܂Ńv���O������~�j

	serial.close();

	printf("\nPress any key to continue ...");
	_getch();

	return 0;
}

// �X���b�h�����ŌĂяo�����֐�
DWORD WINAPI ThreadFunc(LPVOID arg) {
	CWinSerial* pSerial = (CWinSerial*)arg;

	char RxBuf[1024];
	int iReadSize;

	while (g_bThreadFlag) {
		if (pSerial->available() != 0) {
			// �󂯎��������������̂܂ܕ\��
			iReadSize = pSerial->readBytes(RxBuf, 1024);
			RxBuf[iReadSize] = '\0';
			printf("%s", RxBuf);
		}
	}

	return 0;
}