//
//  NumbersUndo.h
//  Graph
//
//  Created by 武 孙 on 12-5-10.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "INumbers.h"

#define		DATACOUNT_SEP			1000000		// 数据个数分隔，当大于等于这个值时，只能撤销恢复16次，否则为32次

class CNumbersUndo  
{
public:
	void Destroy();
	BOOL Create(DWORD dwCount, INumbers *pIFilterNumbers);
    
	CNumbersUndo();
	virtual ~CNumbersUndo();
    
private:
	DWORD	m_dwCount;				// 数据总数
	LPBYTE	m_lpData;				// 数据缓冲区
    
	BYTE	m_btMaxUndoCount;		// 最大可撤销次数
	BYTE	m_btCanUndoCount;		// 可撤销次数（0到m_btMaxUndoCount）
	BYTE	m_btCanRedoCount;		// 可恢复次数（0到m_btMaxUndoCount）
	BYTE	m_btCurUndoFlagIndex;	// 当前撤销标志索引（0到m_btMaxUndoCount-1）
    
	INumbers	*m_pINumbers;
public:
	// 保存撤销信息
	virtual void SaveUndo();
	// 撤销
	virtual void Undo();
	// 恢复
	virtual void Redo();
	// 是否可以撤销
	virtual BOOL CanUndo();
	// 是否可以恢复
	virtual BOOL CanRedo();
    
private:
	void _ResetAllFlag();
	BOOL _IsSameWithPrev();
	void _CopyUndoData();
};
