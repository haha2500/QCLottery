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