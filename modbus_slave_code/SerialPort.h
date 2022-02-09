#ifndef SERIALPORT_H
#define SERIALPORT_H

#endif // SERIALPORT_H
#pragma once
/*
* Windows Serial Port for Windows API
*
* Copyright (c) 2016 Ondrej Sterba <osterba@inbox.com>
*
* https://github.com/embedded-tools/WindowsSerialPort
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.
* It is provided "as is" without express or implied warranty.
*
*/

#ifndef SERIALPORT___H
#define SERIALPORT___H

#include <windows.h>


class TSerialPort
{
private:
    HANDLE m_portHandle;
    HANDLE m_workingThread;
    DWORD  m_workingThreadId;

    int    m_timeoutMilliSeconds;
    int    m_maxPacketLength;

    void (*m_OnDataReceivedHandler)(const unsigned char* pData, int dataLength);
    void (*m_OnDataSentHandler)(void);

    CRITICAL_SECTION m_criticalSectionRead;
    CRITICAL_SECTION m_criticalSectionWrite;

    int __ReadBuffer(unsigned char* pData, int dataLength, int timeOutMS = -1);
    int __WriteBuffer(const unsigned char* pData, int dataLength);


public:
    TSerialPort();
    ~TSerialPort();

    int GetMaxTimeout();
    void* GetDataReceivedHandler();
    void* GetDataSentHandler();

    bool Open(int comPortNumber, int baudRate, int timeoutMS = 1000);

    bool OpenAsync(int comPortNumber, int baudRate,
        void (*OnDataReceivedHandler)(const unsigned char* pData, int dataLength),
        void (*OnDataSentHandler)(void),
        int timeoutMS = 100
    );

    void Close();
    bool IsOpen();

    int ReadBuffer(unsigned char* pData, int dataLength, int timeOutMS = -1);
    int WriteBuffer(const unsigned char* pData, int dataLength);

    int ReadLine(char* pLine, int maxBufferSize, int timeOutMS = -1);
    int WriteLine(char* pLine, bool addCRatEnd = true);


};

DWORD WINAPI SerialPort_WaitForData(LPVOID lpParam);
void    SerialPort_Initialize(void);
void    SerialPort_Uninitialize(void);
int     SerialPort_GetMaxTimeout();
int     SerialPort_GetDataReceivedHandler();
int     SerialPort_GetDataSentHandler();
BOOL    SerialPort_OpenAsync(int comPortNumber,
    int baudRate,
    void (*OnDataReceivedHandler)(const unsigned char* pData, int dataLength),
    void (*OnDataSentHandler)(void),
    int timeoutMS);
BOOL    SerialPort_Open(int comPortNumber, int baudRate, int timeoutMS);
void    SerialPort_Close();
BOOL    SerialPort_IsOpen();
int     SerialPort_ReadBuffer(unsigned char* pData, int dataLength, int timeOutMS);
int     SerialPort_WriteBuffer(const unsigned char* pData, int dataLength);
int     SerialPort_WriteLine(char* pLine, BOOL addCRatEnd);
int     SerialPort_ReadLine(char* pLine, int maxBufferSize, int timeOutMS);



#endif
