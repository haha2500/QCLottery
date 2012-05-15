//
//  QuickAddString.h
//  Graph
//
//  Created by 武 孙 on 12-5-10.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

class CQuickAddString  
{
public:
	CQuickAddString();
	CQuickAddString(LPCSTR lpszText);
	virtual ~CQuickAddString();
    
	void Add(LPCSTR lpszText);
    
	LPCSTR GetText(){return (m_lpBuf == NULL) ? "" : m_lpBuf;};
	int GetLength(){return m_dwCurTextLen;};
    
	void TrimRight(LPCSTR lpszTargetList);
	void TrimRight(int nCount = 1);
    
	// 清空，如果bClean为TRUE，则释放空间
	void Empty(BOOL bClean = FALSE);
	// 是否为空，如果bClean为TRUE，则还需检测空间是否被释放
	BOOL IsEmpty(BOOL bClean = FALSE);
    
public:
	operator LPCSTR() const
	{
		return LPCSTR(m_lpBuf);
	};
    
	const CQuickAddString & operator += (LPCSTR lpszText)
	{
		Add(lpszText);
		return *this;
	};
    
	const CQuickAddString & operator += (char caText)
	{
		char szText[2] = {caText, 0};
		Add(szText);
		return *this;
	};
    
	const CQuickAddString & operator = (LPCSTR lpszText)
	{
		Empty();
		Add(lpszText);
		return *this;
	};
    
private:
	char		*m_lpBuf;
	DWORD		m_dwBufLen;
	DWORD		m_dwCurTextLen;
};
