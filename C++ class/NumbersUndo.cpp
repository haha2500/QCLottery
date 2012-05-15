//
//  NumbersUndo.cpp
//  Graph
//
//  Created by 武 孙 on 12-5-10.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "NumbersUndo.h"
#include "Numbers.h"

const DWORD UNDO_FLAG[] = {0x00000001, 0x00000002, 0x00000004, 0x00000008, 0x00000010, 0x00000020, 0x00000040, 0x00000080,
    0x00000100, 0x00000200, 0x00000400, 0x00000800, 0x00001000, 0x00002000, 0x00004000, 0x00008000,
    0x00010000, 0x00020000, 0x00040000, 0x00080000, 0x00100000, 0x00200000, 0x00400000, 0x00800000,
    0x01000000, 0x02000000, 0x04000000, 0x08000000, 0x10000000, 0x20000000, 0x40000000, 0x80000000};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CNumbersUndo::CNumbersUndo()
{
	m_dwCount = 0;
	m_lpData = NULL;
    
	m_btMaxUndoCount = m_btCanUndoCount = m_btCanRedoCount = m_btCurUndoFlagIndex = 0;
    
	m_pINumbers = NULL;
}

CNumbersUndo::~CNumbersUndo()
{
	Destroy();
}

BOOL CNumbersUndo::Create(DWORD dwCount, INumbers *pIFilterNumbers)
{
	ASSERT(m_dwCount == 0 && m_lpData == NULL && pIFilterNumbers != NULL);
	ASSERT(dwCount > 0);
    
	m_dwCount = dwCount;
    
	DWORD dwBufSize = 0;
    
	if(m_dwCount >= DATACOUNT_SEP)
	{
		m_lpData = (LPBYTE)new WORD[m_dwCount];
		dwBufSize = m_dwCount * sizeof(WORD);
		m_btMaxUndoCount = 16;
	}
	else
	{
		m_lpData = (LPBYTE)new DWORD[m_dwCount];
		dwBufSize = m_dwCount * sizeof(DWORD);
		m_btMaxUndoCount = 32;
	}
    
	if(m_lpData == NULL)
	{
		return FALSE;
	}
    
	ZeroMemory(m_lpData, dwBufSize);
    
	m_pINumbers = pIFilterNumbers;
    
	return TRUE;
}

void CNumbersUndo::Destroy()
{
	if(m_lpData != NULL)
	{
		if(m_dwCount >= DATACOUNT_SEP)
		{
			delete [](LPWORD)m_lpData;
		}
		else
		{
			delete [](LPDWORD)m_lpData;
		}
		
		m_lpData = NULL;
	}
    
	m_dwCount = 0;
	m_btMaxUndoCount = m_btCanUndoCount = m_btCanRedoCount = m_btCurUndoFlagIndex = 0;
}

void CNumbersUndo::SaveUndo()
{
	// 当前撤销标志索引后移
	m_btCurUndoFlagIndex ++;
	if(m_btCurUndoFlagIndex == m_btMaxUndoCount)
	{
		m_btCurUndoFlagIndex = 0;	// 移动超出最后，则复位
	}
    
	// 重新设置所有数据该标志位信息
	_ResetAllFlag();
    
	// 检查是否和上一次撤销的数据完全一样
	if(_IsSameWithPrev())
	{
		// 如果和前一次撤销的数据完全相同，则当前撤销标志前移，以便忽略本次撤销信息
		if(m_btCurUndoFlagIndex == 0)
		{
			m_btCurUndoFlagIndex = (BYTE)(m_btMaxUndoCount - 1);
		}
		else
		{
			m_btCurUndoFlagIndex --;
		}
	}
	else
	{
		// 可撤销总数处理
		if(m_btCanUndoCount < m_btMaxUndoCount - 1)
		{
			m_btCanUndoCount ++;
			m_btCanRedoCount = 0;
		}
	}
}

void CNumbersUndo::Undo()
{
	if(m_btCanUndoCount == 0)
	{
		return ;		// 没有可撤销的信息
	}
    
	// 当前撤销标志前移
	if(m_btCurUndoFlagIndex == 0)
	{
		m_btCurUndoFlagIndex = (BYTE)(m_btMaxUndoCount - 1);
	}
	else
	{
		m_btCurUndoFlagIndex --;
	}
	
	// 复制数据
	_CopyUndoData();
    
	// 重置撤销和恢复个数
	m_btCanUndoCount --;	
	m_btCanRedoCount ++;
}

void CNumbersUndo::Redo()
{
	if(m_btCanRedoCount == 0)
	{
		return ;		// 没有可恢复的信息
	}
    
	// 当前撤销标志后移
	if(m_btCurUndoFlagIndex == m_btMaxUndoCount - 1)
	{
		m_btCurUndoFlagIndex = 0;
	}
	else
	{
		m_btCurUndoFlagIndex ++;
	}
    
	// 复制数据
	_CopyUndoData();
    
	m_btCanRedoCount --;
	m_btCanUndoCount ++;
}

BOOL CNumbersUndo::CanUndo()
{
	return m_btCanUndoCount > 0 ? TRUE : FALSE;	
}

BOOL CNumbersUndo::CanRedo()
{
	return m_btCanRedoCount > 0 ? TRUE : FALSE;	
}

/////////////////////////////////////////////////////////////////////////////////////////////////
void CNumbersUndo::_ResetAllFlag()
{
	DWORD dwRemoveFlag = ~UNDO_FLAG[m_btCurUndoFlagIndex], dwAddFlag = UNDO_FLAG[m_btCurUndoFlagIndex];
	DWORD dwFDCount = m_pINumbers->GetCount();
	
	if(m_dwCount >= DATACOUNT_SEP)
	{
		// 先清除所有数据该标志位信息
		LPWORD lpFlag = NULL;
		for(DWORD i=0; i<m_dwCount; i++)
		{
			lpFlag = &((LPWORD)m_lpData)[i];
			(*lpFlag) &= (WORD)dwRemoveFlag;
		}
		
		// 设置可撤销数据的该标志位信息
        DWORD dwValue = 0;
		for(DWORD i=0; i<dwFDCount; i++)
		{
            dwValue = m_pINumbers->GetItemValue(i);
			lpFlag = &((LPWORD)m_lpData)[dwValue];
			(*lpFlag) |= (WORD)dwAddFlag;
		}
	}
	else
	{
		// 先清除所有数据该标志位信息
		LPDWORD lpFlag = NULL;
		for(DWORD i=0; i<m_dwCount; i++)
		{
			lpFlag = &((LPDWORD)m_lpData)[i];
			(*lpFlag) &= dwRemoveFlag;
		}
        
		// 设置可撤销数据的该标志位信息
        DWORD dwValue = 0;
		for(DWORD i=0; i<dwFDCount; i++)
		{
            dwValue = m_pINumbers->GetItemValue(i);
			lpFlag = &((LPDWORD)m_lpData)[dwValue];
			(*lpFlag) |= dwAddFlag;
		}
	}
}

BOOL CNumbersUndo::_IsSameWithPrev()
{
	// 检查是否和上一次撤销的数据完全一样
	BOOL bSame = TRUE, bPrevExist = FALSE, bCurExist = FALSE;
	DWORD dwPrevFlag = 0, dwAddFlag = UNDO_FLAG[m_btCurUndoFlagIndex];
    
	// 获取前一次撤销的标志
	if(m_btCurUndoFlagIndex == 0)
		dwPrevFlag = UNDO_FLAG[m_btMaxUndoCount - 1];
	else
		dwPrevFlag = UNDO_FLAG[m_btCurUndoFlagIndex - 1];
	
	// 比较所有数据的前一次撤销和当前撤销的标志
	if(m_dwCount >= DATACOUNT_SEP)
	{
		LPWORD lpFlag = NULL;
		for(DWORD i=0; i<m_dwCount; i++)
		{
			lpFlag = &((LPWORD)m_lpData)[i];
			bPrevExist = (*lpFlag) & (WORD)dwPrevFlag;
			bCurExist = (*lpFlag) & (WORD)dwAddFlag;
			if((!bPrevExist && bCurExist) || (bPrevExist && !bCurExist))
			{
				bSame = FALSE;	// 存在不一样的
				break;
			}
		}
	}
	else
	{
		LPDWORD lpFlag = NULL;
		for(DWORD i=0; i<m_dwCount; i++)
		{
			lpFlag = &((LPDWORD)m_lpData)[i];
			bPrevExist = (*lpFlag) & dwPrevFlag;
			bCurExist = (*lpFlag) & dwAddFlag;
			if((!bPrevExist && bCurExist) || (bPrevExist && !bCurExist))
			{
				bSame = FALSE;	// 存在不一样的
				break;
			}
		}
	}
    
	return bSame;
}

void CNumbersUndo::_CopyUndoData()
{
	ASSERT(m_pINumbers != NULL);
    
	// 组全部号码
	m_pINumbers->Create(0, DATA_SOURCE_ORG);
	m_pINumbers->AutoComboNumbers();
    
	DWORD i, dwItemCount = m_pINumbers->GetCount(), dwFlag = UNDO_FLAG[m_btCurUndoFlagIndex];
	
	// 比较标志，删除不是恢复的数据
	if(m_dwCount >= DATACOUNT_SEP)
	{
		LPWORD lpFlag = NULL;
		for(i=0; i<dwItemCount; i++)
		{
			lpFlag = &((LPWORD)m_lpData)[m_pINumbers->GetItemValue(i)];
			if(!((*lpFlag) & dwFlag))
			{
				m_pINumbers->DeleteItemData(i);
			}
		}
	}
	else
	{
		LPDWORD lpFlag = NULL;
		for(i=0; i<dwItemCount; i++)
		{
			lpFlag = &((LPDWORD)m_lpData)[m_pINumbers->GetItemValue(i)];
			if(!((*lpFlag) & dwFlag))
			{
				m_pINumbers->DeleteItemData(i);
			}
		}
	}
    
	m_pINumbers->RemoveData();
}
