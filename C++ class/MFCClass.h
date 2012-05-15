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

