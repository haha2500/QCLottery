//
//  MFCClass.cpp
//  Graph
//
//  Created by 武 孙 on 12-5-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "MFCClass.h"

CDWordArray::CDWordArray()
{
    m_dwSize = 0;
    m_lpBuf = NULL;
}

CDWordArray::~CDWordArray()
{
    RemoveAll();
}

void CDWordArray::SetSize(DWORD dwCount)
{
    if (dwCount == 0)
    {
        RemoveAll();
        return;
    }
    
    if (dwCount <= m_dwSize)
    {
        m_dwSize = dwCount;
        return;
    }
    
    LPDWORD lpNewBuf = new DWORD[dwCount];
    if (m_dwSize > 0)
    {
        memcpy(lpNewBuf, m_lpBuf, m_dwSize * sizeof(DWORD));
        delete []m_lpBuf;
    }
   
    m_lpBuf = lpNewBuf;
    m_dwSize = dwCount;
}

void CDWordArray::SetAt(DWORD dwIndex, DWORD dwValue)
{
    ASSERT(dwIndex < m_dwSize);
    
    m_lpBuf[dwIndex] = dwValue;
}

DWORD CDWordArray::GetAt(DWORD dwIndex)
{
    ASSERT(dwIndex < m_dwSize);
    
    return m_lpBuf[dwIndex];
}

void CDWordArray::Add(DWORD dwValue)
{
    LPDWORD lpNewBuf = new DWORD[m_dwSize + 1];
    memcpy(lpNewBuf, m_lpBuf, m_dwSize * sizeof(DWORD));
    lpNewBuf[m_dwSize] = dwValue;
    
    delete []m_lpBuf;
    m_lpBuf = lpNewBuf;
    
    m_dwSize ++;
}

void CDWordArray::RemoveAt(DWORD dwIndex)
{
    ASSERT(dwIndex < m_dwSize);

    m_dwSize --;
    
    for(DWORD i=dwIndex; i<m_dwSize; i++)
    {
        m_lpBuf[i] = m_lpBuf[i + 1];
    }
}

void CDWordArray::RemoveAll()
{
    DELETE_BUFFER(m_lpBuf);
    m_dwSize = 0;
}

void CDWordArray::Copy(CDWordArray &cSrcByteArray)
{
    RemoveAll();
    
    m_dwSize = cSrcByteArray.m_dwSize;
    if(m_dwSize > 0)
    {
        m_lpBuf = new DWORD[m_dwSize];
        memcpy(m_lpBuf, cSrcByteArray.m_lpBuf, m_dwSize * sizeof(DWORD));
    }
}

#pragma mark -

CByteArray::CByteArray()
{
    m_dwSize = 0;
    m_lpBuf = NULL;
}

CByteArray::~CByteArray()
{
    RemoveAll();
}

void CByteArray::SetSize(DWORD dwCount)
{
    if (dwCount == 0)
    {
        RemoveAll();
        return;
    }
    
    if (dwCount <= m_dwSize)
    {
        m_dwSize = dwCount;
        return;
    }
    
    LPBYTE lpNewBuf = new BYTE[dwCount];
    if (m_dwSize > 0)
    {
        memcpy(lpNewBuf, m_lpBuf, m_dwSize * sizeof(BYTE));
        delete []m_lpBuf;
    }
    
    m_lpBuf = lpNewBuf;
    m_dwSize = dwCount;
}

void CByteArray::SetAt(DWORD dwIndex, BYTE btValue)
{
    ASSERT(dwIndex < m_dwSize);
    
    m_lpBuf[dwIndex] = btValue;
}

BYTE CByteArray::GetAt(DWORD dwIndex)
{
    ASSERT(dwIndex < m_dwSize);
    
    return m_lpBuf[dwIndex];
}

void CByteArray::RemoveAll()
{
    if(m_dwSize == 0)
    {
        return ;
    }
    
    DELETE_BUFFER(m_lpBuf);
    m_dwSize = 0;
}

void CByteArray::Copy(CByteArray &cSrcByteArray)
{
    RemoveAll();
    
    m_dwSize = cSrcByteArray.m_dwSize;
    if(m_dwSize > 0)
    {
        m_lpBuf = new BYTE[m_dwSize];
        memcpy(m_lpBuf, cSrcByteArray.m_lpBuf, m_dwSize * sizeof(BYTE));
    }
}

#pragma mark - 
CTime::CTime()
{
    m_wYear = 0;
    m_btMonth = m_btDay = m_btHour = m_btMinute = m_btSecond = 0;
}

CTime::CTime(int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSecond, int nRes)
{
    ASSERT(nYear >= 1900 && nYear < 2100);
    ASSERT(nMonth >= 1 && nMonth <= 12);
    ASSERT(nDay >= 1 && nDay <= 31);
    ASSERT(nHour >= 0 && nHour <= 23);
    ASSERT(nMinute >= 0 && nMinute <= 59);
    ASSERT(nSecond >= 0 && nSecond <= 59);
    
    m_wYear = (WORD)nYear;
    m_btMonth = (BYTE)nMonth;
    m_btDay = (BYTE)nDay;
    m_btHour = (BYTE)nHour;
    m_btMinute = (BYTE)nMinute;
    m_btSecond = (BYTE)nSecond;
}

//   CTime(DWORD dwYearMonthDay);
//  CTime(DWORD dwYearMonthDayHourMinute);
CTime::CTime(NSDate *date)
{
    SetNSDate(date);
}

CTime::~CTime()
{
    
}

DWORD CTime::GetYearMonthDayHourMinute()
{
    return (m_wYear - 2000) * 100000000 + m_btMonth * 1000000 + m_btDay * 10000 + m_btHour * 100 + m_btSecond;
}

DWORD CTime::GetYearMonthDay()
{
    return m_wYear * 10000 + m_btMonth * 100 + m_btDay;
}

DWORD CTime::GetHourMinute()
{
    return m_btHour * 100 + m_btMinute;
}

void CTime::SetNSDate(NSDate *date)
{
    ASSERT(date != nil);
    
    NSCalendar *gregorian = [[NSCalendar alloc]initWithCalendarIdentifier:NSGregorianCalendar];
    NSInteger flag = NSYearCalendarUnit | NSMonthCalendarUnit | NSDayCalendarUnit | NSHourCalendarUnit | NSMinuteCalendarUnit | NSSecondCalendarUnit;
    NSDateComponents *dateComponents = [gregorian components:flag fromDate:date];
    
    m_wYear = (WORD)dateComponents.year;
    m_btMonth = (BYTE)dateComponents.month;
    m_btDay = (BYTE)dateComponents.day;
    m_btHour = (BYTE)dateComponents.hour;
    m_btMinute = (BYTE)dateComponents.minute;
    m_btSecond = (BYTE)dateComponents.second;
}

NSDate *CTime::GetNSDate()
{
    NSDateComponents *comps = [[NSDateComponents alloc] init];
    [comps setDay:m_btDay];
    [comps setMonth:m_btMonth];
    [comps setYear:m_wYear];
    NSCalendar *gregorian = [[NSCalendar alloc]initWithCalendarIdentifier:NSGregorianCalendar];
    NSDate *date = [gregorian dateFromComponents:comps];
    
    return date;
}

// 向前nTSDays>0）或往后（nTSDays>0）指定天数
void CTime::TimeSpan(int nTSDays)
{
    NSDate *date = GetNSDate();
    if (date == nil)
    {
        ASSERT(FALSE);
        return ;
    }
    
    NSDate *newDate = [date dateByAddingTimeInterval:nTSDays * 24 * 3600];
    SetNSDate(newDate);
}