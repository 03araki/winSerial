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

	BOOL setup(int comPort, int baudrate);	// シリアルポートのセットアップ
	BOOL close();							// シリアルポートの終了処理
	int available();						// 受信可能データ数の確認
	BOOL flush();							// 送受信データの削除
	int writeBytes(char* buf, int length);	// データ送信
	int write(char byte);					// 1 バイト送信
	int readBytes(char* buf, int length);	// データ受信
	char read();							// 1 バイト受信
};

#endif // __Serial_H_