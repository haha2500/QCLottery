//
//  MFCClass.h
//  Graph
//
//  Created by 武 孙 on 12-5-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

class CDWordArray 
{
private:
    DWORD       m_dwSize;
    LPDWORD     m_lpBuf;
    
public:
    CDWordArray();
    virtual ~CDWordArray();
    
    void SetSize(DWORD dwCount);
    
    DWORD GetSize(){return m_dwSize;};
    LPDWORD GetData(){return m_lpBuf;};
    
    void SetAt(DWORD dwIndex, DWORD dwValue);
    DWORD GetAt(DWORD dwIndex);
    
    void Add(DWORD dwValue);
    void RemoveAt(DWORD dwIndex);
    
    void RemoveAll();
    
    void Copy(CDWordArray &cSrcByteArray);
};

#pragma mark - 
class CByteArray 
{
private:
    BYTE       m_dwSize;
    LPBYTE     m_lpBuf;
    
public:
    CByteArray();
    virtual ~CByteArray();
    
   void SetSize(DWORD dwCount);
    
    DWORD GetSize(){return m_dwSize;};
    LPBYTE GetData(){return m_lpBuf;};
    
    void SetAt(DWORD dwIndex, BYTE btValue);
    BYTE GetAt(DWORD dwIndex);
    
    void RemoveAll();
    
    void Copy(CByteArray &cSrcByteArray);
};

#pragma mark -
class CTime
{
public:
    CTime();
    CTime(int nYear, int nMonth, int nDay, int nHour = 0, int nMinute = 0, int nSecond = 0, int nRes = 0);
 //   CTime(DWORD dwYearMonthDay);
  //  CTime(DWORD dwYearMonthDayHourMinute);
    CTime(NSDate *date);
    virtual ~CTime();
    
public:
    int GetYear(){return m_wYear;};
    int GetMonth(){return m_btMonth;};
    int GetDay(){return m_btDay;};
    int GetHour(){return m_btHour;};
    int GetMinute(){return m_btMinute;};
    int GetSecond(){return m_btSecond;};
    
    DWORD GetYearMonthDayHourMinute();  // 年只用后两位
    DWORD GetYearMonthDay();
    DWORD GetHourMinute();
    
    // 获取星期几，暂时未实现
    int GetDayOfWeek(){return 1;};
    
    void SetNSDate(NSDate *date);
    NSDate *GetNSDate();
    
    // 向前nTSDays>0）或往后（nTSDays>0）指定天数
    void TimeSpan(int nTSDays);
    
private:
    WORD m_wYear;
    BYTE m_btMonth;
    BYTE m_btDay;
    BYTE m_btHour;
    BYTE m_btMinute;
    BYTE m_btSecond;
};

