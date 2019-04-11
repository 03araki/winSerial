#include "WinSerial.h"
#include <stdio.h>

CWinSerial::CWinSerial()
{
	m_hComPort = NULL;
}

CWinSerial::CWinSerial(int comPort, int baudrate)
{
	CWinSerial();
	if (!setup(comPort, baudrate)) m_hComPort = NULL;
}

CWinSerial::~CWinSerial()
{
	close();
}


BOOL CWinSerial::setup(int comPort, int baudrate)
{
	char cComPortStr[256];
	sprintf_s(cComPortStr, 256, "\\\\.\\COM%d", comPort);

	// COM ポートのオープン
	m_hComPort = CreateFile(cComPortStr, GENERIC_READ | GENERIC_WRITE,
		0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hComPort == INVALID_HANDLE_VALUE) return FALSE;

	// 送信設定
	DCB dcb;
	ZeroMemory(&dcb, sizeof(DCB));
	sprintf_s(cComPortStr, 256, "COM%d: baud=%d parity=N data=8 stop=1", comPort, baudrate);
	if (!BuildCommDCB(cComPortStr, &dcb)) {
		close();
		return FALSE;
	}
	if (!SetCommState(m_hComPort, &dcb)) {
		close();
		return FALSE;
	}

	// タイムアウト設定
	COMMTIMEOUTS timeout;
	timeout.ReadIntervalTimeout = 1;
	timeout.ReadTotalTimeoutMultiplier = 0;
	timeout.ReadTotalTimeoutConstant = 1;
	timeout.WriteTotalTimeoutMultiplier = 0;
	timeout.WriteTotalTimeoutConstant = 1;
	if (!SetCommTimeouts(m_hComPort, &timeout)) {
		close();
		return FALSE;
	}
	
	return TRUE;
}


BOOL CWinSerial::close() {
	if (m_hComPort == NULL) return TRUE;

	return CloseHandle(m_hComPort);
}

int CWinSerial::available() {
	if (m_hComPort == NULL) return 0;
	ClearCommError(m_hComPort, &m_dwError, &m_comStat);

	return (int)m_comStat.cbInQue;
}

BOOL CWinSerial::flush()
{
	ReadFile(m_hComPort, NULL, available(), &m_dwRecvSize, NULL);
	PurgeComm(m_hComPort, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);

	return TRUE;
}

int CWinSerial::writeBytes(char * buf, int length)
{
	if (m_hComPort == NULL) return 0;
	WriteFile(m_hComPort, buf, length, &m_dwSendSize, NULL);
	return m_dwSendSize;
}

int CWinSerial::write(char byte)
{
	return writeBytes(&byte, 1);
}

int CWinSerial::readBytes(char * buf, int length)
{
	if (m_hComPort == NULL) return 0;
	if (!ReadFile(m_hComPort, buf, length, &m_dwRecvSize, NULL)) return 0;
	return m_dwRecvSize;
}

char CWinSerial::read()
{
	char buf;
	readBytes(&buf, 1);
	return buf;
}


