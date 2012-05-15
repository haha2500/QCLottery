//
//  QuickAddString.cpp
//  Graph
//
//  Created by 武 孙 on 12-5-10.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "QuickAddString.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuickAddString::CQuickAddString()
{
	m_lpBuf = NULL;
	m_dwBufLen = m_dwCurTextLen = 0;
}

CQuickAddString::CQuickAddString(LPCSTR lpszText)
{
	m_lpBuf = NULL;
	m_dwBufLen = m_dwCurTextLen = 0;
	Add(lpszText);
}


CQuickAddString::~CQuickAddString()
{
	Empty(TRUE);
}

void CQuickAddString::Add(LPCSTR lpszText)
{
	if(lpszText == NULL)
	{
		return ;
	}
    
	int nLen = strlen(lpszText);
	if(nLen == 0)
	{
		return ;
	}
    
	DWORD dwNeedLen = m_dwCurTextLen + nLen;
	if(dwNeedLen >= m_dwBufLen)	// 超长，则需要重新创建缓冲区
	{
		if(m_dwBufLen == 0)
		{
			m_dwBufLen = fmax(2 * nLen, 1024);	// 初始化长度
		}
		else
		{
			m_dwBufLen *= 2;
			if(m_dwBufLen < m_dwCurTextLen + nLen)
			{
				m_dwBufLen = 2 * (m_dwCurTextLen + nLen);
			}
		}
		char *pNewBuf = new char[m_dwBufLen];
		if(m_dwCurTextLen > 0)
		{
			memcpy(pNewBuf, m_lpBuf, m_dwCurTextLen);
			pNewBuf[m_dwCurTextLen] = 0;
		}
		delete []m_lpBuf;
		m_lpBuf = pNewBuf;
	}
	else	// 长度足够
	{
		if(2 * dwNeedLen < m_dwBufLen && m_dwBufLen > 1024)	//  需要释放一些空间
		{
			m_dwBufLen = fmax(2 * dwNeedLen, 1024);
			char *pNewBuf = new char[m_dwBufLen];
			memcpy(pNewBuf, m_lpBuf, m_dwCurTextLen);
			delete []m_lpBuf;
			m_lpBuf = pNewBuf;
		}
	}
    
	memcpy(&m_lpBuf[m_dwCurTextLen], lpszText, nLen);
	m_dwCurTextLen += nLen;
	m_lpBuf[m_dwCurTextLen] = 0;
}

void CQuickAddString::TrimRight(LPCSTR lpszTargetList)
{
	ASSERT(lpszTargetList != NULL);
    
	while(m_dwCurTextLen > 0)
	{
		if(strchr(lpszTargetList, m_lpBuf[m_dwCurTextLen-1]) == NULL)
		{
			m_lpBuf[m_dwCurTextLen] = 0;
			return ;
		}
        
		m_dwCurTextLen --;
	}
}

void CQuickAddString::TrimRight(int nCount)
{
	if(m_dwCurTextLen < (DWORD)nCount)
		m_dwCurTextLen = 0;
	else
		m_dwCurTextLen -= nCount;
    
	m_lpBuf[m_dwCurTextLen] = 0;
}

void CQuickAddString::Empty(BOOL bClean)
{
	m_dwCurTextLen = 0;
	if(m_dwBufLen > 0)
	{
		m_lpBuf[0] = 0;
	}
    
	if(bClean || m_dwBufLen > 1024 * 2)
	{
		if(m_lpBuf != NULL)
		{
			delete []m_lpBuf;
			m_lpBuf = NULL;
		}
		m_dwBufLen = 0;
	}
}

BOOL CQuickAddString::IsEmpty(BOOL bClean)
{
	return bClean ? (m_dwBufLen == 0) : (m_dwCurTextLen == 0);
}


