#pragma once
#ifndef __WinSerial_H_
#define __WinSerial_H_

#include <Windows.h>

class CWinSerial {
private:
	HANDLE	m_hComPort;
	DWORD	m_dwError;
	COMSTAT	m_comStat;
	DWORD	m_dwSendSize;
	DWORD	m_dwRecvSize;

public:
	CWinSerial();
	CWinSerial(int comPort, int baudrate);
	~CWinSerial();

	BOOL setup(int comPort, int baudrate);	// �V���A���|�[�g�̃Z�b�g�A�b�v
	BOOL close();							// �V���A���|�[�g�̏I������
	int available();						// ��M�\�f�[�^���̊m�F
	BOOL flush();							// ����M�f�[�^�̍폜
	int writeBytes(char* buf, int length);	// �f�[�^���M
	int write(char byte);					// 1 �o�C�g���M
	int readBytes(char* buf, int length);	// �f�[�^��M
	char read();							// 1 �o�C�g��M
};

#endif // __Serial_H_