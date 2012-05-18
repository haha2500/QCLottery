//
//  Numbers.cpp
//  Graph
//
//  Created by 武 孙 on 12-5-10.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "Numbers.h"

//////////////////////////////////////////////////////////////////////
#define GROUPTOSINGLE_SETDATA(nNumberCount)	\
            if(m_btNumberCount == nNumberCount){ \
                memcpy(&lpNew[nSinIndex], btSinData, m_btNumberCount); \
                nSinIndex += m_btNumberCount; \
                memcpy(&lpNew[nSinIndex], &lpData[m_btNumberCount], m_btNumberCount2); \
                nSinIndex += m_btNumberCount2; \
                nSinIndex += ms_btCDSize; \
                continue; \
            }

///////////////////////////////////////////////////////////////////////////////////////////////////
BYTE CNumbers::ms_btWholeNumberCount = 0;
BOOL CNumbers::ms_bAscSortData = TRUE;
BYTE CNumbers::ms_btCDSize = sizeof(DWORD);

///////////////////////////////////////////////////////////////////////////////////////////////////

CNumbers::CNumbers()
{
	m_lpDataItemArray = NULL;
	m_dwCount = 0;
	m_btDataSource = m_btItemSize = 0;
	m_btDataType = m_btMaxNumber = m_btMinNumber = m_btNumberCount = 0;
	m_btDataType2 = m_btMaxNumber2 = m_btMinNumber2 = m_btNumberCount2 = 0;
	ZeroMemory(m_szDataString, sizeof(m_szDataString));
	ZeroMemory(&m_stNumsSep, sizeof(m_stNumsSep));
	ZeroMemory(&m_stNumsFilterSep, sizeof(m_stNumsFilterSep));
	m_btNumber2Mode = 0;
	m_btNumber2Array.RemoveAll();
	m_caX = 0;
	m_pNumbersUndo = NULL;
}

CNumbers::~CNumbers()
{
	DELETE_POINTER(m_pNumbersUndo);
	Destroy();
}

BOOL CNumbers::Create(DWORD dwCount, BYTE btDataSource, BYTE btNumberCount, BYTE btNumberCount2)
{
	Destroy();	// 先销毁以前的
    
	// 赋值
	m_dwCount = dwCount;
	m_btDataSource = btDataSource;
    
	// 初始化相关变量
	LPLOTTERYPROPERTY lpLotteryProperty = g_pIData->GetLotteryProperty();
	switch(btDataSource)
	{
        case DATA_SOURCE_INIT:			// 初始数据
		{
			m_btDataType = lpLotteryProperty->stSectionInfo[0].btDataType;
			m_btMaxNumber = lpLotteryProperty->stSectionInfo[0].btMaxNumber;
			m_btMinNumber = lpLotteryProperty->stSectionInfo[0].btMinNumber;
			m_btNumberCount = lpLotteryProperty->stSectionInfo[0].btNumberCount;
            
			m_btDataType2 = lpLotteryProperty->stSectionInfo[1].btDataType;
			m_btMaxNumber2 = lpLotteryProperty->stSectionInfo[1].btMaxNumber;
			m_btMinNumber2 = lpLotteryProperty->stSectionInfo[1].btMinNumber;
			m_btNumberCount2 = lpLotteryProperty->stSectionInfo[1].btNumberCount;
		} break;
            
        case DATA_SOURCE_ORG:			// 原始数据
		{
			m_btDataType = g_pIData->GetDataType(DATA_SOURCE_ORG);
			m_btMaxNumber = g_pIData->GetMaxNumber(DATA_SOURCE_ORG);
			m_btMinNumber = g_pIData->GetMinNumber(DATA_SOURCE_ORG);
			m_btNumberCount = (btNumberCount == 0) ? g_pIData->GetNumberCount(DATA_SOURCE_ORG) : btNumberCount;
            
			if(btNumberCount2 > 0)
			{
				m_btDataType2 = lpLotteryProperty->stSectionInfo[1].btDataType;
				m_btMaxNumber2 = lpLotteryProperty->stSectionInfo[1].btMaxNumber;
				m_btMinNumber2 = lpLotteryProperty->stSectionInfo[1].btMinNumber;
			}
			else
			{
				m_btDataType2 = m_btMaxNumber2 = m_btMinNumber2 = 0;
			}
			m_btNumberCount2 = btNumberCount2;
		} break;
            
        case DATA_SOURCE_CUR:			// 当前数据
		{
			m_btDataType = g_pIData->GetDataType(DATA_SOURCE_CUR);
			m_btMaxNumber = g_pIData->GetMaxNumber(DATA_SOURCE_CUR);
			m_btMinNumber = g_pIData->GetMinNumber(DATA_SOURCE_CUR);
			m_btNumberCount = (btNumberCount == 0) ? g_pIData->GetNumberCount(DATA_SOURCE_CUR) : btNumberCount;
            
			m_btDataType2 = m_btMaxNumber2 = m_btMinNumber2 = m_btNumberCount2 = 0;
		} break;
            
        default: ASSERT(FALSE); break;
	}
    
	// 计算所有号码个数
	m_btWholeNumberCount = (BYTE)(m_btNumberCount + m_btNumberCount2);
    
	// 计算单元数据长度
	m_btItemSize = (BYTE)(sizeof(BYTE) * (m_btWholeNumberCount) + ms_btCDSize);
    
	// 设置缺省分隔符信息
	SetSeparator(NULL);	// 从系统配置获取分割信息
    
	// 创建数据缓冲区
	if(m_dwCount > 0)
	{
		m_lpDataItemArray = new BYTE[m_dwCount * m_btItemSize];
		if(m_lpDataItemArray == NULL)
		{
			ASSERT(FALSE);
			Destroy();
			return FALSE;
		}
        
		// 初始化，将所有号码设为可删除的，即无效的
		ZeroMemory(m_lpDataItemArray, m_dwCount * m_btItemSize);
		DWORD dwIndex = 0;
		for(DWORD i=0; i<m_dwCount; i++)
		{
			m_lpDataItemArray[dwIndex] = DELETE_ITEM_FLAG;
			dwIndex += m_btItemSize;
		}
	}
	
	return TRUE;
}


int CNumbers::LoadFromString(LPCSTR lpszData, BOOL bAppend, BOOL bRemoveSame)
{
	if(m_btNumberCount == 0)
	{
		Create(0, DATA_SOURCE_ORG);	// 如果不存在，则按原始方式创建
	}
    
	CNumbers cNumbersTemp;
    cNumbersTemp.Create(0, m_btDataSource, m_btNumberCount, m_btNumberCount2);
	
	switch(m_btDataType & DATA_TYPE_NUM_MASK)
	{
        case DATA_TYPE_NUM_SZ: _LoadFromString_SZ(lpszData, &cNumbersTemp); break;
        case DATA_TYPE_NUM_LT:
            if(m_btDataType & DATA_TYPE_ORDERED)
            {
                _LoadFromString_LT(lpszData, &cNumbersTemp);
            }
            else
            {
                _LoadFromString_POSLT(lpszData, &cNumbersTemp);
            }
            break;
        default: ASSERT(FALSE); break;
	}	
    
	if(cNumbersTemp.GetCount() == 0)
	{
		return 0;	// no data
	}
    
	if(bAppend)
	{
		Append((INumbers *)&cNumbersTemp, bRemoveSame);
	}
	else
	{
		if(bRemoveSame)		// 去重复
			cNumbersTemp.RemoveSameData();
        
		Copy((INumbers *)&cNumbersTemp);
	}
	
	return cNumbersTemp.GetCount();
}

int CNumbers::AutoComboNumbers(BOOL bOnlyCalcCount)
{
	ASSERT(m_btNumberCount > 0);
	ASSERT(m_btNumberCount2 == 0);	// 不允许有第二个号码类型
    
	switch(m_btDataType & DATA_TYPE_NUM_MASK)
	{
        case DATA_TYPE_NUM_SZ: return _AutoComboNums_SZ(bOnlyCalcCount); break;
        case DATA_TYPE_NUM_LT:
            if(m_btDataType & DATA_TYPE_ORDERED)
            {
                return _AutoComboNums_LT(bOnlyCalcCount);
            }
            else
            {	
                return _AutoComboNums_POSLT(bOnlyCalcCount);
            } break;
        default: ASSERT(FALSE); break;
	}
	
	ASSERT(FALSE);
	return -1;
}

void CNumbers::Destroy()
{
	RemoveAllData();
    
	m_btDataSource = m_btItemSize = 0;
	m_btDataType = m_btMaxNumber = m_btMinNumber = m_btNumberCount = 0;
	m_btDataType2 = m_btMaxNumber2 = m_btMinNumber2 = m_btNumberCount2 = 0;
    
	ZeroMemory(m_szDataString, sizeof(m_szDataString));
	m_btNumber2Mode = 0;
	m_btNumber2Array.RemoveAll();
}

void CNumbers::SetCount(DWORD dwCount)
{
	if(dwCount == m_dwCount)
	{
		return ;	// 和当前大小相同，则不处理
	}
    
	if(dwCount == 0)
	{
		RemoveAllData();
	}
	else
	{
		DWORD dwBufSize = dwCount * m_btItemSize;
		LPBYTE lpNew = new BYTE[dwBufSize];
		if(lpNew == NULL)
		{
			ASSERT(FALSE);
			return ;
		}
		
		// 赋值当前数据
		if(dwCount < m_dwCount)
		{
			memcpy(lpNew, m_lpDataItemArray, dwBufSize);
		}
		else
		{
			if(m_dwCount > 0)
			{
				memcpy(lpNew, m_lpDataItemArray, m_dwCount * m_btItemSize);
			}
            
			// 初始化多余的数据空间，并将号码设为可删除的，即无效的
			DWORD dwIndex = m_dwCount * m_btItemSize;
			ZeroMemory(&lpNew[dwIndex], (dwCount - m_dwCount) * m_btItemSize);
			for(DWORD i=m_dwCount; i<dwCount; i++)
			{
				lpNew[dwIndex] = DELETE_ITEM_FLAG;
				dwIndex += m_btItemSize;
			}
		}
        
		RemoveAllData();
        
		m_lpDataItemArray = lpNew;
		m_dwCount = dwCount;
	}
}

BOOL CNumbers::Append(INumbers *pSrc, BOOL bRemoveSame)
{
	ASSERT(pSrc != NULL);
	
	if(pSrc->GetCount() == 0 || pSrc->GetNumberCount() == 0)
	{
		return TRUE;	// 没有数据，不需要添加
	}
    
	if(m_dwCount == 0 || (pSrc->GetNumberCount() != m_btNumberCount || pSrc->GetNumberCount2() != m_btNumberCount2))
	{
		Copy(pSrc);
	}
	else
	{
		// 创建新的缓冲区
		DWORD dwSrcCount = pSrc->GetCount();
		m_dwCount += dwSrcCount;
		
		int nTemp = m_dwCount * m_btItemSize;
		LPBYTE lpNew = new BYTE[nTemp];
		if(lpNew == NULL)
		{
			ASSERT(FALSE);
			return FALSE;
		}
		
		// 复制旧的数据
		int nOld = (m_dwCount - dwSrcCount) * m_btItemSize;
		memcpy(lpNew, m_lpDataItemArray, nOld);
		
		// 复制源数据
		memcpy(&lpNew[nOld], pSrc->GetBuffer(), dwSrcCount * m_btItemSize);
		
		delete []m_lpDataItemArray;
		m_lpDataItemArray = lpNew;
	}
    
	if(bRemoveSame)
		RemoveSameData();
	
	return TRUE;
}

void CNumbers::Copy(INumbers *pSrc)
{
	ASSERT(pSrc != NULL);
    
	DWORD dwSrcCount = pSrc->GetCount();
    
	// 如果个数不同，则先销毁当前，以便重新创建，否则则可直接使用
	if(m_dwCount != dwSrcCount || pSrc->GetDataSource() != m_btDataSource || \
       pSrc->GetNumberCount() != m_btNumberCount || pSrc->GetNumberCount2() != m_btNumberCount2)
	{
		Destroy();
	}
    
	if(m_lpDataItemArray == NULL)
	{
		// 如果当前没有创建，则创建新的
		if(!Create(dwSrcCount, pSrc->GetDataSource(), pSrc->GetNumberCount(), pSrc->GetNumberCount2()))
		{
			return ;
		}
	}
    
	ASSERT(pSrc->GetNumberCount() == m_btNumberCount && pSrc->GetNumberCount2() == m_btNumberCount2);
    
	// 复制新数据
	memcpy(m_lpDataItemArray, pSrc->GetBuffer(), m_dwCount * m_btItemSize);
}

BOOL CNumbers::IsSame(INumbers *pSrc)
{
	ASSERT(pSrc != NULL);
    
	if(m_dwCount != pSrc->GetCount())
	{
		return FALSE;
	}
    
	const LPBYTE lpSrcBuf = pSrc->GetBuffer();
	int nIndex = 0;
	for(DWORD i=0; i<m_dwCount; i++)
	{
		if(memcmp(&lpSrcBuf[nIndex], &m_lpDataItemArray[nIndex], m_btWholeNumberCount))
		{
			return FALSE;	// 数据不同
		}
		nIndex += m_btItemSize;
	}
    
	return TRUE;
}

void CNumbers::SetSeparator(LPNUMBERSSEPARATOR lpNumbersSep)
{
	if(lpNumbersSep == NULL)	// 使用系统分隔符设置
	{
        _GetNumbersSeparator(&m_stNumsSep, m_btDataSource);
        _GetNumbersSeparator(&m_stNumsFilterSep, DATA_SOURCE_CUR);
	}
	else
	{
		memcpy(&m_stNumsSep, lpNumbersSep, sizeof(m_stNumsSep));
	}
}

void CNumbers::SetXChar(char caX)
{
	m_caX = caX;
}

char CNumbers::GetXChar()
{
	return m_caX;
}

DWORD CNumbers::StatAllItems(BOOL bSortByCount, BOOL bAsc, LPDWORD lpMinCount)
{
	if(m_dwCount == 0)
	{
		return 0;			// 没有数据
	}
    
	ResetAllCustomData();	// 重置所有自定义数据值
	
	// 号码排序
	Sort();
	
	// 统计相同个数到自定义数据
	DWORD dwIndex = m_btItemSize, dwMaxCount = 0, dwMinCount = m_dwCount;
	LPDWORD lpCustomData = (LPDWORD)&m_lpDataItemArray[m_btWholeNumberCount];
	
	(*lpCustomData) = 1;	// 将第一个号码的自定义数据初始化为1
	
	for(int i=1; i<(int)m_dwCount; i++)
	{
		if(!memcmp(&m_lpDataItemArray[dwIndex], &m_lpDataItemArray[dwIndex - m_btItemSize], m_btWholeNumberCount))
		{
			// 如果和前一个相同，则将第一个值的自定义数据加1
			(*lpCustomData) ++;
		}
		else
		{
			// 如果不相同，则将该值设为第一个值，并将标志初始化为1
			lpCustomData = (LPDWORD)&m_lpDataItemArray[dwIndex + m_btWholeNumberCount];
			(*lpCustomData) = 1;
		}
		dwIndex += m_btItemSize;
	}	
	
	// 将自定义数据为0的置删除标志，以便删除
	dwIndex = m_btWholeNumberCount;
	DWORD dwItemDataCount = 0;	// 号码出现次数
	for(int i=0; i<(int)m_dwCount; i++)
	{
		dwItemDataCount = (*(LPDWORD)&m_lpDataItemArray[dwIndex]);
		if(dwItemDataCount == 0)
		{
			m_lpDataItemArray[dwIndex - m_btWholeNumberCount] |= DELETE_ITEM_FLAG;
		}
		else
		{
			// 分析最大和最小值
			if(dwMaxCount < dwItemDataCount) dwMaxCount = dwItemDataCount;
			if(dwMinCount > dwItemDataCount) dwMinCount = dwItemDataCount;
		}
		dwIndex += m_btItemSize;
	}
	
	// 删除无用的数据
	RemoveData();
	
	if(bSortByCount)
	{
		// 按统计值排序
		Sort(bAsc);
	}
    
	if(lpMinCount != NULL)
	{
		(*lpMinCount) = dwMinCount;
	}
    
	return dwMaxCount;
}

LPSTR CNumbers::GetStatString(int nColumn, BOOL bSortByCount, BOOL bAsc)
{
	ASSERT(nColumn >= 0);
    
	if(m_lpDataItemArray == NULL || m_dwCount == 0)
	{
		return (LPSTR)"";
	}
    
	// 统计次数并获取最小重复号码个数
	DWORD dwMinSame = 0;
	StatAllItems(bSortByCount, bAsc, &dwMinSame);
    
	// 组返回的字符串	
	int nIndex = 0, nSameCount = 0, nItemStringLen = strlen(GetItemString(0)), nNumsSepLen = strlen(m_stNumsSep.szBetweenNums);
    
	// 创建缓冲区
	char szDetails[64] = {0};
	int nLen = (nItemStringLen + (nNumsSepLen > 2 ? nNumsSepLen : 2)) * m_dwCount;
	char *pBuf = new char[nLen + 1];
	if(pBuf == NULL)
	{
		return (LPSTR)"";
	}
    
	CQuickAddString strRtn;
	LPDWORD lpCustomData = NULL;
	DWORD dwCurSame = dwMinSame, dwItemCDIndex = m_btWholeNumberCount;
	for(int i=0; i<(int)m_dwCount; i++)
	{
		lpCustomData = (LPDWORD)&m_lpDataItemArray[dwItemCDIndex];
		if(dwCurSame == (*lpCustomData))
		{
			nSameCount ++;
		}
		else
		{
			sprintf(szDetails, "出现%lu次的（%d注）：\r\n", dwCurSame, nSameCount);
			pBuf[nIndex] = 0;
			strRtn += szDetails;
			strRtn += pBuf;
			strRtn.TrimRight();
			if(nNumsSepLen > 0)
				strRtn.TrimRight(m_stNumsSep.szBetweenNums);
            
			strRtn += "\r\n\r\n";
			
			nIndex = 0;
			nSameCount = 1;
			dwCurSame = (*lpCustomData);
		}
		memcpy(&pBuf[nIndex], GetItemString(i), nItemStringLen);
		nIndex += nItemStringLen;
		if(nColumn > 0 && (nSameCount % nColumn == 0))
		{
			// 换行
			pBuf[nIndex ++] = '\r';	
			pBuf[nIndex ++] = '\n';	
		}
		else
		{
			// 添加号码分割符
			if(nNumsSepLen > 0)
			{
				memcpy(&pBuf[nIndex], m_stNumsSep.szBetweenNums, nNumsSepLen);
				nIndex += nNumsSepLen;
			}
		}
		dwItemCDIndex += m_btItemSize;
	}
    
	if(nSameCount > 0)
	{
		sprintf(szDetails, "出现%lu次的号码（%d注）：\r\n", dwCurSame, nSameCount);
		pBuf[nIndex] = 0;
		strRtn += szDetails;
		strRtn += pBuf;
		strRtn.TrimRight();
		if(nNumsSepLen > 0)
			strRtn.TrimRight(m_stNumsSep.szBetweenNums);
	}
    
	delete []pBuf;
    
	ResetAllCustomData();
    
	// 创建返回的缓冲区
	nLen = strRtn.GetLength();
	LPSTR lpRtn = new char[nLen + 1];
	memcpy(lpRtn, strRtn, nLen);
	lpRtn[nLen] = 0;
    
	return lpRtn;
}

void CNumbers::ReleaseStatString(LPSTR lpszStatString)
{
	if(lpszStatString != NULL && lpszStatString[0] != 0)
	{
		delete []lpszStatString;
	}
}

LPCSTR CNumbers::GetAllDataString(int nColCount, DWORD dwType, WORD wAutoAddSpaceRow)
{
	ASSERT(nColCount >= 0);
	
	if(!m_strAllDataString.IsEmpty())
	{
		m_strAllDataString.Empty();
	}
    
	if(dwType == GADST_NUMSTYPE)
	{
		_GetAllDataStringByNumsType(m_strAllDataString, nColCount);	// 按号码类型分类
	}
	else
	{
		_GetAllDataStringByCustomData(m_strAllDataString, nColCount, wAutoAddSpaceRow, MAXDWORD);
	}
    
	return m_strAllDataString.GetText();
}

void CNumbers::ReleaseAllDataString()
{
	m_strAllDataString.Empty(TRUE);
}

void CNumbers::SetItemData(DWORD dwIndex, LPBYTE lpData)
{
	ASSERT(m_lpDataItemArray != NULL && lpData != NULL);
	ASSERT(dwIndex < m_dwCount);
	
	memcpy(&m_lpDataItemArray[dwIndex * m_btItemSize], lpData, m_btWholeNumberCount);
}

int CNumbers::AddItemData(LPBYTE lpData, BOOL bNoAddIfExist)
{
	ASSERT(lpData != NULL && m_btNumberCount > 0);	// 必须调用了Create
	
	DWORD dwIndex = 0;
    
	if(bNoAddIfExist)
	{
		// 查询是否存在相同数据
		for(DWORD i=0; i<m_dwCount; i++)
		{
			if(!memcmp(&m_lpDataItemArray[dwIndex], lpData, m_btWholeNumberCount))
			{
				return i;		// find same
			}
			dwIndex += m_btItemSize;
		}
	}
    
	// 添加到最后
	dwIndex = m_dwCount * m_btItemSize;
	LPBYTE lpNew = new BYTE[dwIndex + m_btItemSize];
    
	memcpy(&lpNew[dwIndex], lpData, m_btWholeNumberCount);
	ZeroMemory(&lpNew[dwIndex + m_btWholeNumberCount], ms_btCDSize);
    
	// 复制并删除旧的数据
	if(m_lpDataItemArray != NULL)
	{
		memcpy(lpNew, m_lpDataItemArray, dwIndex);
		delete []m_lpDataItemArray;
	}
	
	m_lpDataItemArray = lpNew;
	m_dwCount ++;
	
	return m_dwCount - 1;
}

BOOL CNumbers::ModifyItemData(DWORD dwIndex, LPBYTE lpData, BOOL bFailIfExists)
{
	if(dwIndex >= m_dwCount)
	{
		ASSERT(FALSE);
		return FALSE;
	}
    
	if(bFailIfExists)
	{
		// 查询是否存在相同数据
		DWORD dwFindIndex = 0;
		for(DWORD i=0; i<m_dwCount; i++)
		{
			if(!memcmp(&m_lpDataItemArray[dwFindIndex], lpData, m_btWholeNumberCount))
			{
				return FALSE;		// find same
			}
			dwFindIndex += m_btItemSize;
		}
	}
    
	// 覆盖数据
	memcpy(&m_lpDataItemArray[dwIndex * m_btItemSize], lpData, m_btWholeNumberCount);
    
	return TRUE;
}

void CNumbers::SetItemCustomData(DWORD dwIndex, DWORD dwCustomData)
{
	ASSERT(m_lpDataItemArray != NULL);
	ASSERT(dwIndex < m_dwCount);
	
	*((LPDWORD)&m_lpDataItemArray[dwIndex * m_btItemSize + m_btWholeNumberCount]) = dwCustomData;
}

void CNumbers::ResetAllCustomData(DWORD dwCustomData)
{
	DWORD dwIndex = m_btWholeNumberCount;
    
	for(DWORD i=0; i<m_dwCount; i++)
	{
		*((LPDWORD)&m_lpDataItemArray[dwIndex]) = dwCustomData;
		dwIndex += m_btItemSize;
	}
}

LPBYTE CNumbers::GetItemData(DWORD dwIndex, BYTE btGetFlag)
{
	ASSERT(m_lpDataItemArray != NULL);
	ASSERT(dwIndex < m_dwCount);
	
	LPBYTE lpItemData = &m_lpDataItemArray[dwIndex * m_btItemSize];
	
	if(lpItemData[0] == MAXBYTE)
	{
		return NULL;		// 只有试机号没有开奖号
	}
    
	ASSERT(ISVALIDITEM(lpItemData));	// 确保单元数据有效
    
	if(m_btDataSource == DATA_SOURCE_ORG)		// 原始数据
	{
		if(btGetFlag == GF_CUR)	// 需要进行数据变换
		{
			if(g_pIData->DataTransTable_IsOrgTrans())	// 原始数据变换，则直接使用分析方式下的数据
			{
				GetItemData(dwIndex, GF_AM);
			}
			else	// 非原始数据变换，则需要获取变换后的数据
			{
				memcpy(m_btCurData, g_pIData->DataTransTable_GetTransData(GetItemValue(dwIndex, GF_AM)), g_pIData->GetNumberCount());
			}
			// 数据排列
			g_pIData->DataTransTable_OrderData(m_btCurData, g_pIData->GetNumberCount());
			return m_btCurData;
		}
		else if(btGetFlag == GF_AM)	// 需要进行分析方式转换
		{
			BYTE btTempData[100] = {0};
			memcpy(btTempData, lpItemData, m_btNumberCount);
			LPANALYSISMODEITEM lpAMItem = g_pIData->GetCurrentAnalysisMode();
			ASSERT(lpAMItem != NULL);
			
			// 前处理
			_TransAMDataByFlag(lpAMItem->btFlagPrev, btTempData, m_btNumberCount);
			
			// 位置转换
			for(int i=0; i<lpAMItem->btCount; i++)
			{
				m_btCurData[i] = btTempData[lpAMItem->btPosArray[i]];
			}
			m_btCurData[lpAMItem->btCount] = 0;
			
			// 后处理
			_TransAMDataByFlag(lpAMItem->btFlagNext, m_btCurData, lpAMItem->btCount);
            
			return m_btCurData;
		}
	}
    
	return lpItemData;	// 原始数据
}

DWORD CNumbers::GetItemCustomData(DWORD dwIndex)
{
	ASSERT(m_lpDataItemArray != NULL);
	ASSERT(dwIndex < m_dwCount);
	
	return *((LPDWORD)&m_lpDataItemArray[dwIndex * m_btItemSize + m_btWholeNumberCount]);
}

LPCSTR CNumbers::GetItemString(DWORD dwIndex, BYTE btGetFlag)
{
	LPBYTE lpItemData = GetItemData(dwIndex, btGetFlag);
	if(lpItemData == NULL)
	{
		return "";
	}
    
	int nIndex = 0;
	BYTE btDataType = m_btDataType;
	BYTE btNumberCount = m_btNumberCount, btNumberCount2 = m_btNumberCount2;
	LPNUMBERSSEPARATOR lpNumsSep = &m_stNumsSep;
    
	if(btGetFlag != GF_ORG && m_btDataSource == DATA_SOURCE_ORG)
	{
		// 需要将原始数据源转换为当前数据源
		btNumberCount = g_pIData->GetNumberCount();
		btNumberCount2 = 0;		// 分析方式下只有一种类型的号码
		btDataType = g_pIData->GetDataType();
		lpNumsSep = &m_stNumsFilterSep;
	}
	
	LPCSTR lpszInSection0 = lpNumsSep->szInSection[0];
    
	// 先处理第一个号码
	int nSepLen = strlen(lpszInSection0);
	switch(btDataType & DATA_TYPE_NUM_MASK)
	{
        case DATA_TYPE_NUM_SZ:
            nIndex = _ByteToChar_SZ(m_szDataString, lpItemData, btNumberCount, lpszInSection0, nSepLen); break;
            
        case DATA_TYPE_NUM_LT:
            nIndex = _ByteToChar_LT(m_szDataString, lpItemData, btNumberCount, lpszInSection0, nSepLen); break;
            
        default: ASSERT(FALSE); break;
	}
    
	// 再处理第二个号码
	if(btNumberCount2 > 0)
	{
		nSepLen = strlen(m_stNumsSep.szBetweenSection);
		if(nSepLen > 0)
		{
			// 添加分割符
			memcpy(&m_szDataString[nIndex], lpNumsSep->szBetweenSection, nSepLen);
			nIndex += nSepLen;
		}
		nSepLen = strlen(m_stNumsSep.szInSection[1]);
		switch(m_btDataType2 & DATA_TYPE_NUM_MASK)
		{
            case DATA_TYPE_NUM_SZ:		// 数字型
                nIndex += _ByteToChar_SZ(&m_szDataString[nIndex], &lpItemData[btNumberCount], btNumberCount2, lpNumsSep->szInSection[1], nSepLen); break;
                               
            case DATA_TYPE_NUM_LT:
                nIndex += _ByteToChar_LT(&m_szDataString[nIndex], &lpItemData[btNumberCount], btNumberCount2,lpNumsSep->szInSection[1], nSepLen); break;
                
            default: ASSERT(FALSE); break;
		}
	}
    
	ASSERT(nIndex < sizeof(m_szDataString));
	m_szDataString[nIndex] = 0;
    
	return m_szDataString;
}

DWORD CNumbers::GetItemValue(DWORD dwIndex, BYTE btGetFlag)
{
	ASSERT(m_lpDataItemArray != NULL);
	ASSERT(dwIndex < m_dwCount);
	
	LPBYTE lpItemData = GetItemData(dwIndex, btGetFlag);
	ASSERT(lpItemData != NULL);
    
	BYTE btNumberCount = m_btNumberCount, btMinNumber = m_btMinNumber;
	int nRatio = m_btMaxNumber - m_btMinNumber + 1;
    
	if(btGetFlag != GF_ORG && m_btDataSource == DATA_SOURCE_ORG)
	{
		// 需要将原始数据源转换为当前数据源
		if(btGetFlag == GF_AM)
			btNumberCount = g_pIData->GetCurrentAnalysisMode()->btCount;
		else
			btNumberCount = g_pIData->GetNumberCount();
		btMinNumber = g_pIData->GetMinNumber();
		if(g_pIData->GetDataType() & DATA_TYPE_NUM_SZ)
		{
			nRatio = 10;	// 数字分析方式
		}
	}
	else
	{
		switch(m_btDataType & DATA_TYPE_NUM_MASK)
		{
            case DATA_TYPE_NUM_LT:
                if(m_btDataType & DATA_TYPE_ORDERED)
                {
                    return _GetItemValue_LT(lpItemData);
                }
                else
                {
                    return _GetItemValue_POSLT(lpItemData);
                }
                break;
            default: break;
		}
	}
    
	DWORD dwValue = 0, dwRatio = 1;
	for(int i=btNumberCount-1; i>=0; i--)
	{
		dwValue += (lpItemData[i] - btMinNumber) * dwRatio;
		dwRatio *= nRatio;
	}
    
	return dwValue;
}

void CNumbers::DeleteItemData(DWORD dwIndex)
{
	ASSERT(m_lpDataItemArray != NULL);
	ASSERT(dwIndex < m_dwCount);
    
	m_lpDataItemArray[dwIndex * m_btItemSize] |= DELETE_ITEM_FLAG;	
}

int CNumbers::RemoveData(INumbers *pRemovedData)
{
	DWORD dwDeleteCount = 0, dwIndex = 0, dwTemp = 0, i = 0;
	
	// 计算需要删除的单元个数
	for(i=0; i<m_dwCount; i++)
	{
		if(m_lpDataItemArray[dwIndex] & DELETE_ITEM_FLAG)
		{
			dwDeleteCount ++;
		}
		dwIndex += m_btItemSize;
	}
	
	// 存入待删除数据
	if(pRemovedData != NULL)
	{
		pRemovedData->Create(dwDeleteCount, m_btDataSource, m_btNumberCount, m_btNumberCount2);
		dwTemp = dwIndex = 0;
		
		for(i=0; i<m_dwCount; i++)
		{
			if(m_lpDataItemArray[dwIndex] & DELETE_ITEM_FLAG)
			{
				m_lpDataItemArray[dwIndex] &= ~DELETE_ITEM_FLAG;
				pRemovedData->SetItemData(dwTemp++, &m_lpDataItemArray[dwIndex]);
				m_lpDataItemArray[dwIndex] |= DELETE_ITEM_FLAG;
			}
			dwIndex += m_btItemSize;
		}
	}
    
	if(dwDeleteCount == 0)
	{
		return 0;	// 没有需要删除的数据
	}
	
	// 清除需要删除的数据
	if(dwDeleteCount < m_dwCount)	// 没有删除全部数据
	{
		DWORD dwCount = m_dwCount - dwDeleteCount;
		LPBYTE lpNew = new BYTE[dwCount * m_btItemSize];
		ZeroMemory(lpNew, dwCount * m_btItemSize);
		dwTemp = dwIndex = 0;
        
		for(i=0; i<m_dwCount; i++)
		{
			if(!(m_lpDataItemArray[dwIndex] & DELETE_ITEM_FLAG))
			{
				memcpy(&lpNew[dwTemp], &m_lpDataItemArray[dwIndex], m_btItemSize);
				dwTemp += m_btItemSize;
			}
			dwIndex += m_btItemSize;
		}
        
		delete []m_lpDataItemArray;
		m_lpDataItemArray = lpNew;
		m_dwCount = dwCount;
	}
	else				// 删除全部数据
	{
		RemoveAllData();
	}
    
	return dwDeleteCount;
}

void CNumbers::UnRemoveData()
{
	DWORD i, dwIndex = 0;
	for(i=0; i<m_dwCount; i++)
	{
		m_lpDataItemArray[dwIndex] &= ~DELETE_ITEM_FLAG;
		dwIndex += m_btItemSize;
	}
}

void CNumbers::RemoveAllData()
{
	DELETE_BUFFER(m_lpDataItemArray);
	m_dwCount = 0;
}

void CNumbers::ReverseAllData(BOOL bSingle)
{
	// 组全部号码
	CNumbers cNumbers;
	cNumbers.Create(0, m_btDataSource, m_btNumberCount, m_btNumberCount2);
	cNumbers.AutoComboNumbers();
    
	if(!bSingle)	// 需要转换为组选
	{
		cNumbers.SinToGro();
		SinToGro();
	}
    
	// 添加到当前号码
	Append((CNumbers *)&cNumbers, FALSE);
    
	// 排序
	ResetAllCustomData(0);
	Sort(TRUE);
    
	// 有相同的，则是需要排除的
	DWORD dwIndex = m_btItemSize;		
	for(int i=1; i<(int)m_dwCount; i++)
	{
		if(!memcmp(&m_lpDataItemArray[dwIndex], &m_lpDataItemArray[dwIndex - m_btItemSize], m_btWholeNumberCount))
		{
			// 如果和前一个相同，则需要排除
			m_lpDataItemArray[dwIndex] |= DELETE_ITEM_FLAG;
			m_lpDataItemArray[dwIndex - m_btItemSize] |= DELETE_ITEM_FLAG;
		}
		dwIndex += m_btItemSize;
	}	
    
	// 删除无用的数据
	RemoveData();
}

void CNumbers::Sort(BOOL bAsc)
{
	ms_btWholeNumberCount = m_btWholeNumberCount;
	ms_bAscSortData = bAsc;
	
	if(bAsc)
		qsort((void*)m_lpDataItemArray, m_dwCount, sizeof(BYTE) * m_btItemSize, _AscendSortNums);
	else
		qsort((void*)m_lpDataItemArray, m_dwCount, sizeof(BYTE) * m_btItemSize, _DescendSortNums);	
}

void CNumbers::SortEx(BOOL bAscData, BOOL bAscNums)
{
	ms_btWholeNumberCount = m_btWholeNumberCount;
	ms_bAscSortData = bAscData;
    
	if(bAscNums)
		qsort((void*)m_lpDataItemArray, m_dwCount, sizeof(BYTE) * m_btItemSize, _AscendSortNums);
	else
		qsort((void*)m_lpDataItemArray, m_dwCount, sizeof(BYTE) * m_btItemSize, _DescendSortNums);	
}

void CNumbers::RemoveSameData()
{
	Sort();
	
	// 去重复
	DWORD dwIndex = m_btItemSize;
	for(int i=1; i<(int)m_dwCount; i++)
	{
		if(!memcmp(&m_lpDataItemArray[dwIndex], &m_lpDataItemArray[dwIndex - m_btItemSize], m_btWholeNumberCount))
		{
			// 如果与上一个号码数据相同，则需要将上一个号码数据删除
			m_lpDataItemArray[dwIndex - m_btItemSize] |= DELETE_ITEM_FLAG;
		}
		dwIndex += m_btItemSize;
	}
	
	RemoveData();
}

void CNumbers::SortItemData(DWORD dwIndex, BOOL bAsc)
{
	LPBYTE lpItemData = GetItemData(dwIndex);
	ASSERT(lpItemData != NULL);
    
	if(bAsc)
		qsort(lpItemData, m_btNumberCount, 1, _AscendSortItem);		// 号码正向快速排序	
	else
		qsort(lpItemData, m_btNumberCount, 1, _DescendSortItem);	// 号码反向快速排序	
}

void CNumbers::SinToGro(BOOL bRemoveSaveNums)
{
	DWORD dwIndex = 0;
	LPBYTE lpData = NULL;
    
	// 将每个号码变为组选
	for(DWORD i=0; i<m_dwCount; i++)
	{
		lpData = &m_lpDataItemArray[dwIndex];
		qsort(lpData, m_btNumberCount, 1, _AscendSortItem);	// 号码快速排序
		dwIndex += m_btItemSize;
	}
	
	if(bRemoveSaveNums)
	{
		// 去重复
		RemoveSameData();
	}
}

void CNumbers::GroToSin(BOOL bRemoveSaveNums)
{
	SinToGro(bRemoveSaveNums);		// 先进行单变组，以尽量减少原始数据个数
    
	if(m_dwCount == 0)
	{
		return ;		// no data
	}
    
	// 计算变为单选号码以后的总数：n!
	int i = 0, nSinCount = m_dwCount, nIndex = 0;
	for(i=m_btNumberCount; i>1; i--)
	{
		nSinCount *= i;
	}
	
	if(nSinCount > 10000000)	// 数据量大于1千万的，则使用全组号码后排除的算法
	{
		ASSERT(bRemoveSaveNums);
        
		CNumbers cNewData;
		cNewData.Create(0, m_btDataSource, m_btNumberCount, m_btNumberCount2);
		cNewData.AutoComboNumbers();
		
		ASSERT(m_btNumberCount < 32);
		BYTE btData[32] = {0};
		for(i=cNewData.GetCount()-1; i>=0; i--)
		{
			memcpy(btData, cNewData.GetItemData(i), m_btNumberCount);
			qsort(btData, m_btNumberCount, 1, _AscendSortItem);	// 号码快速排序
			if(-1 == FindItem(btData, TRUE))
			{
				cNewData.DeleteItemData(i);
			}
		}
		cNewData.RemoveData();
		Copy((CNumbers *)&cNewData);
	}
	else	// 数据量小于等于1千万的，则使用直接转换的算法
	{
		// 重新创建缓冲区
		LPBYTE lpNew = new BYTE[nSinCount * m_btItemSize], lpData = NULL;
		ASSERT(lpNew != NULL);
		ZeroMemory(lpNew, nSinCount * m_btItemSize);
		
		int a, b, c, d, e, f, g, nSinIndex = 0;
		BYTE btSinData[8] = {0};
		
		// 将每一个号码转为单选号码
		for(i=0; i<(int)m_dwCount; i++)
		{
			lpData = &m_lpDataItemArray[nIndex];
			
			for(a=0; a<m_btNumberCount; a++)
			{
				btSinData[0] = lpData[a];
				GROUPTOSINGLE_SETDATA(1);	// 设置相关数据
				for(b=0; b<m_btNumberCount; b++)
				{
					if(b == a) continue;
					btSinData[1] = lpData[b];
					GROUPTOSINGLE_SETDATA(2);	// 设置相关数据
					
					for(c=0; c<m_btNumberCount; c++)
					{
						if(c == a || c == b) continue;
						btSinData[2] = lpData[c];
						GROUPTOSINGLE_SETDATA(3);	// 设置相关数据
						
						for(d=0; d<m_btNumberCount; d++)
						{
							if(d == a || d == b || d == c) continue;
							btSinData[3] = lpData[d];
							GROUPTOSINGLE_SETDATA(4);	// 设置相关数据
							
							for(e=0; e<m_btNumberCount; e++)
							{
								if(e == a || e == b || e == c || e == d) continue;
								btSinData[4] = lpData[e];
								GROUPTOSINGLE_SETDATA(5);	// 设置相关数据
								
								for(f=0; f<m_btNumberCount; f++)
								{
									if(f == a || f == b || f == c || f == d || f == e) continue;
									btSinData[5] = lpData[f];
									GROUPTOSINGLE_SETDATA(6);	// 设置相关数据
									
									for(g=0; g<m_btNumberCount; g++)
									{
										if(g == a || g == b || g == c || g == d || g == e || g == f) continue;
										btSinData[6] = lpData[g];
										GROUPTOSINGLE_SETDATA(7);	// 设置相关数据
										
									}
								}
							}
						}
					}
				}
			}
			
			nIndex += m_btItemSize;
		}
		
		// 删除原来的，并设置新的
		delete []m_lpDataItemArray;
		m_lpDataItemArray = lpNew;
		m_dwCount = nSinCount;
		
		if(bRemoveSaveNums)
		{
			// 去重复
			RemoveSameData();
		}
	}
}

BOOL CNumbers::SaveData(LPCSTR lpszFilename)
{
    // TEST
/*	ASSERT(m_lpDataItemArray != NULL && lpszFilename != NULL);
    
	CFile cSaveFile;
	if(!cSaveFile.Open(lpszFilename, CFile::modeCreate|CFile::modeWrite))
		return FALSE;
    
	DWORD dwVersion = MAXDWORD;
	cSaveFile.Write(&dwVersion, sizeof(dwVersion));
	cSaveFile.Write(&m_dwCount, sizeof(m_dwCount));
	cSaveFile.Write(&m_btNumberCount, sizeof(m_btNumberCount));
	cSaveFile.Write(&m_btNumberCount2, sizeof(m_btNumberCount2));
	
	DWORD i, dwIndex = 0;
	
	for(i=0; i<m_dwCount; i++)
	{
		cSaveFile.Write(&m_lpDataItemArray[dwIndex], m_btWholeNumberCount);
		dwIndex += m_btItemSize;
	}
	cSaveFile.Close();
    */
	return TRUE;
}

BOOL CNumbers::LoadData(LPCSTR lpszFilename)
{
    // TEST
	ASSERT(lpszFilename != NULL);
    
/*	CFile cOpenFile(lpszFilename, CFile::modeRead);
	if(cOpenFile.GetLength() == 0)
	{
		cOpenFile.Close();
		return FALSE;
	}
    
	DWORD i, dwCount = 0, dwVersion = 0;
    
	m_btNumberCount2 = 0;
	
	try{
		cOpenFile.Read(&dwVersion, sizeof(dwVersion));
		if(dwVersion == MAXDWORD)	// 当前版本
		{
			cOpenFile.Read(&dwCount, sizeof(dwCount));
			cOpenFile.Read(&m_btNumberCount, sizeof(m_btNumberCount));
			cOpenFile.Read(&m_btNumberCount2, sizeof(m_btNumberCount2));
		}
		else	// 以前版本
		{
			dwCount = dwVersion;
			cOpenFile.Read(&m_btNumberCount, sizeof(m_btNumberCount));
		}
        
		Create(dwCount, DATA_SOURCE_ORG, m_btNumberCount, m_btNumberCount2);
		
		DWORD dwIndex = 0;
		for(i=0; i<dwCount; i++)
		{
			if(dwVersion == MAXDWORD)	// 当前版本
			{
				cOpenFile.Read(&m_lpDataItemArray[dwIndex], m_btNumberCount);
			}
			else
			{
				cOpenFile.Read(&m_lpDataItemArray[dwIndex], m_btWholeNumberCount);
			}
			dwIndex += m_btItemSize;
		}
	}catch(...)
	{
		cOpenFile.Close();
		return FALSE;
	}
    
	cOpenFile.Close();
 */
	return TRUE;
}

BOOL CNumbers::ImportData(LPBYTE lpDataBuf, int nBufLen)
{
	ASSERT(lpDataBuf != NULL && nBufLen > 0);
	
	DWORD i = 0, j = 0, dwCount = 0, dwIndex = 0;
	
	m_btNumberCount2 = 0;
	
	memcpy(&dwCount, &lpDataBuf[j], sizeof(dwCount));
	j += sizeof(m_dwCount);
	memcpy(&m_btNumberCount, &lpDataBuf[j], sizeof(m_btNumberCount));
	j += sizeof(m_btNumberCount);
	memcpy(&m_btNumberCount2, &lpDataBuf[j], sizeof(m_btNumberCount2));
	j += sizeof(m_btNumberCount2);
	
	Create(dwCount, DATA_SOURCE_ORG, m_btNumberCount, m_btNumberCount2);
	
	for(i=0; i<dwCount; i++)
	{
		memcpy(&m_lpDataItemArray[dwIndex], &lpDataBuf[j], m_btNumberCount);
		j += m_btWholeNumberCount;
		dwIndex += m_btItemSize;
	}
    
	ASSERT(j == (DWORD)nBufLen);
	return TRUE;
}

int CNumbers::ExportData(LPBYTE lpDataBuf, int nBufLen)
{
	// 计算长度
	int nLen = sizeof(m_dwCount) + sizeof(m_btNumberCount) + sizeof(m_btNumberCount2) + m_btWholeNumberCount * m_dwCount;
    
	if(lpDataBuf != NULL)
	{
		if(nBufLen < nLen)
		{
			ASSERT(FALSE);
			return 0;
		}
        
		DWORD i = 0, j = 0, dwIndex = 0;
		memcpy(&lpDataBuf[j], &m_dwCount, sizeof(m_dwCount));
		j += sizeof(m_dwCount);
		memcpy(&lpDataBuf[j], &m_btNumberCount, sizeof(m_btNumberCount));
		j += sizeof(m_btNumberCount);
		memcpy(&lpDataBuf[j], &m_btNumberCount2, sizeof(m_btNumberCount2));
		j += sizeof(m_btNumberCount2);
		
		for(i=0; i<m_dwCount; i++)
		{
			memcpy(&lpDataBuf[j], &m_lpDataItemArray[dwIndex], m_btWholeNumberCount);
			j += m_btWholeNumberCount;
			dwIndex += m_btItemSize;
		}
		
		ASSERT(j == (DWORD)nLen);
	}
    
    
	return nLen;
}

void CNumbers::FillRandomData(INumbers *pDestNumbers, BOOL bNeedSeed)
{
	ASSERT(pDestNumbers != NULL);
	DWORD dwDestCount = pDestNumbers->GetCount(), dwNeedFlag = 0, dwRandCount = 0;
	
	ASSERT(dwDestCount > 0 && dwDestCount <= m_dwCount);
	
	if(bNeedSeed)
		srand(time(NULL));		// 种子
	
	if(dwDestCount > (m_dwCount / 2))	// 随机号码数超过一半
	{
		ResetAllCustomData(1);
		dwRandCount = m_dwCount - dwDestCount;
		dwNeedFlag = 0;
	}
	else	// 随机号码数未超过一半
	{
		ResetAllCustomData(0);
		dwRandCount = dwDestCount;
		dwNeedFlag = 1;
	}
	
	DWORD i = 0, dwIndex = 0;
	LPDWORD lpCustomData = NULL;
	int nLastDivCount =  m_dwCount % RAND_MAX, nDivIndex = 0;
	int nDivCount = m_dwCount / RAND_MAX + (nLastDivCount > 0 ? 1 : 0);
	
	// 取随机号码
	while(i < dwRandCount)
	{
		if(nDivCount == 1)
		{
			dwIndex = (DWORD)(rand() % m_dwCount);
		}
		else
		{
			nDivIndex = rand() % nDivCount;
			if(nDivIndex == nDivCount - 1)
				dwIndex = (DWORD)(rand() % nLastDivCount + nDivIndex * RAND_MAX);
			else
				dwIndex = (DWORD)(rand() + nDivIndex * RAND_MAX);
		}
		
		ASSERT(dwIndex < m_dwCount);
		
		dwIndex *= m_btItemSize;
		lpCustomData = (LPDWORD)&m_lpDataItemArray[dwIndex + m_btWholeNumberCount];
		if((*lpCustomData) != dwNeedFlag)
		{
			(*lpCustomData) = dwNeedFlag;			// 置标志
			i ++;
		}
	}
	
	// 赋值到目标
	dwIndex = m_btWholeNumberCount;
	DWORD dwDestIndex = 0;
    
	for(i=0; i<m_dwCount; i++)
	{
		lpCustomData = (LPDWORD)&m_lpDataItemArray[dwIndex];
		if((*lpCustomData) == 1)
		{
			pDestNumbers->SetItemData(dwDestIndex++, &m_lpDataItemArray[dwIndex - m_btWholeNumberCount]);
			(*lpCustomData) = 0;
		}
		dwIndex += m_btItemSize;
	}
}

void CNumbers::TreatRandomData(DWORD dwRandomCount, BOOL bNeedSeed)
{
	ASSERT(dwRandomCount <= m_dwCount);
	
	DWORD dwIndex = 0;
	
	if(bNeedSeed)
		srand(time(NULL));
	
	DWORD i = 0;
	BOOL bNeedKeep = TRUE;	// 是否需要保留
    
	if(dwRandomCount > m_dwCount / 2)	// 如果需保留的超过一半，则删除小部分
	{
		dwRandomCount = m_dwCount - dwRandomCount;
		bNeedKeep = FALSE;
	}
    
	int nLastDivCount =  m_dwCount % RAND_MAX, nDivIndex = 0;
	int nDivCount = m_dwCount / RAND_MAX + (nLastDivCount > 0 ? 1 : 0);
    
	// 置删除标记
	while(i < dwRandomCount)
	{
		if(nDivCount == 1)
		{
			dwIndex = (DWORD)(rand() % m_dwCount);
		}
		else
		{
			nDivIndex = rand() % nDivCount;
			if(nDivIndex == nDivCount - 1)
				dwIndex = (DWORD)(rand() % nLastDivCount + nDivIndex * RAND_MAX);
			else
				dwIndex = (DWORD)(rand() + nDivIndex * RAND_MAX);
		}
        
		ASSERT(dwIndex < m_dwCount);
        
		dwIndex *= m_btItemSize;
		
		if(!(m_lpDataItemArray[dwIndex] & DELETE_ITEM_FLAG))
		{
			m_lpDataItemArray[dwIndex] |= DELETE_ITEM_FLAG;
			i ++;
		}
	}
    
	// 处理需保留时的情况
	if(bNeedKeep)
	{
		dwIndex = 0;
		for(i=0; i<m_dwCount; i++)
		{
			if(m_lpDataItemArray[dwIndex] & DELETE_ITEM_FLAG)
			{
				m_lpDataItemArray[dwIndex] &= ~DELETE_ITEM_FLAG;
			}
			else
			{
				m_lpDataItemArray[dwIndex] |= DELETE_ITEM_FLAG;
			}
			dwIndex += m_btItemSize;
		}
	}
	
	RemoveData();
}

DWORD CNumbers::GetMatchCount(LPBYTE lpMatchData, LPDWORD lpNDCountArray, BOOL bSingle)
{
	ASSERT(lpMatchData != NULL && lpNDCountArray != NULL);
	ASSERT(m_btNumberCount2 == 0);	// 目前不支持附加号码
	
	ZeroMemory(lpNDCountArray, sizeof(DWORD) * m_btNumberCount);
	
	DWORD i = 0, dwWholeMatchCount = 0;
	LPBYTE lpData = NULL;
	BYTE j = 0, btBitMatchCount = 0;
	
	for(i=0; i<m_dwCount; i++)
	{
		lpData = &m_lpDataItemArray[i * m_btItemSize];
		btBitMatchCount = 0;
		
		if(bSingle)		// 单选方式
		{
			for(j=0; j<m_btNumberCount; j++)
			{
				if(lpData[j] == lpMatchData[j])
				{
					btBitMatchCount ++;
				}
			}
		}
		else			// 组选方式
		{
			int nFlag[10] = {0};
			for(j=0; j<m_btNumberCount; j++)
			{
				nFlag[lpMatchData[j]] ++;
			}
			for(j=0; j<m_btNumberCount; j++)
			{
				if(--nFlag[lpData[j]] >= 0)
				{
					btBitMatchCount ++;
				}
			}
		}
		
		if(btBitMatchCount >= m_btNumberCount)
			dwWholeMatchCount ++;
		else
			lpNDCountArray[btBitMatchCount] ++;
	}
	
	return dwWholeMatchCount;
}

void CNumbers::SaveUndo()
{
	ASSERT(m_btNumberCount > 0);
    
	if(m_btNumberCount != g_pIData->GetNumberCount(DATA_SOURCE_ORG))
	{
		return ;	// 只能保存和原始数据相同号码个数的撤销恢复信息
	}
    
	if(m_pNumbersUndo == NULL)
	{
		m_pNumbersUndo = new CNumbersUndo;
		if(m_pNumbersUndo == NULL)
		{
			return ;
		}
        
		// 计算号码总数
		DWORD dwTotal = 0;
		switch(m_btDataType & DATA_TYPE_NUM_MASK)
		{
            case DATA_TYPE_NUM_SZ: 
            case DATA_TYPE_NUM_PK: dwTotal = (DWORD)pow((m_btMaxNumber - m_btMinNumber + 1), m_btNumberCount); break;
            case DATA_TYPE_NUM_LT:
                if(m_btDataType & DATA_TYPE_ORDERED)
                {			
                    dwTotal = _Comb_LT(m_btMaxNumber, m_btNumberCount);
                }
                else
                {
                    dwTotal = _Comb_POSLT(m_btMaxNumber, m_btNumberCount);
                }
                break;
            default: ASSERT(FALSE); break;
		}
        
		// 创建对象
		m_pNumbersUndo->Create(dwTotal, (CNumbers *)this);
	}
    
	m_pNumbersUndo->SaveUndo();
}

void CNumbers::Undo()
{
	if(m_pNumbersUndo == NULL)
	{
		return ;
	}
    
	m_pNumbersUndo->Undo();
}

void CNumbers::Redo()
{
	if(m_pNumbersUndo == NULL)
	{
		return ;
	}
	
	m_pNumbersUndo->Redo();
}

BOOL CNumbers::CanUndo()
{
	if(m_pNumbersUndo == NULL)
	{
		return FALSE;
	}
    
	if(m_btNumberCount != g_pIData->GetNumberCount(DATA_SOURCE_ORG))
	{
		return FALSE;	// 不是原始数据则没有撤销恢复信息
	}
    
	return m_pNumbersUndo->CanUndo();
}

BOOL CNumbers::CanRedo()
{
	if(m_pNumbersUndo == NULL)
	{
		return FALSE;
	}
	
	if(m_btNumberCount != g_pIData->GetNumberCount(DATA_SOURCE_ORG))
	{
		return FALSE;	// 不是原始数据则没有撤销恢复信息
	}
    
	return m_pNumbersUndo->CanRedo();	
}

void CNumbers::TransCurToOrg(INumbers *pIOrgNumbers)
{
	ASSERT(m_btDataSource != DATA_SOURCE_INIT);		// 当前数据源不能为初始数据源
    
	if(m_btDataSource == DATA_SOURCE_ORG || g_pIData->IsOrgData(TRUE))
	{
		// 当前为原始数据源，则不需要进行转换
		if(pIOrgNumbers != NULL)
		{
			pIOrgNumbers->Copy((CNumbers *)this);		// 直接复制
		}
		m_btDataSource = DATA_SOURCE_ORG;
		return ;
	}
    
	// 设置临时数据
	INumbers *pITempData = pIOrgNumbers;
	if(pIOrgNumbers == NULL)
	{
		pITempData = (INumbers *)new CNumbers;	// 创建临时数据
	}
    
	// 组所有原始数据
	pITempData->Create(0, DATA_SOURCE_ORG);
	pITempData->AutoComboNumbers();
    
	// 初始化数据标志缓冲区
	int i = 0, nFlagCount = (int)pow(m_btMaxNumber - m_btMinNumber + 1, m_btNumberCount);
	LPBYTE lpFlag = new BYTE[nFlagCount];
	ZeroMemory(lpFlag, nFlagCount * sizeof(BYTE));
	
	for(i=0; i<(int)m_dwCount; i++)
	{
		lpFlag[GetItemValue(i)] = 1;
	}
    
	// 根据当前数据查询并删除不需要的原始数据
	for(i=pITempData->GetCount()-1; i>=0; i--)
	{
		if(lpFlag[pITempData->GetItemValue(i, GF_CUR)] == 0)
		{
			pITempData->DeleteItemData(i);
		}
	}
    
	delete []lpFlag;
	
	pITempData->RemoveData();
    
	if(pIOrgNumbers == NULL)
	{
		// 复制并删除临时数据
		Copy(pITempData);
		delete (CNumbers *)pITempData;
	}
}

void CNumbers::TransOrgToCur(INumbers *pICurNumbers)
{
	ASSERT(m_btDataSource != DATA_SOURCE_INIT);		// 当前数据源不能为初始数据源
    
	if(m_btDataSource == DATA_SOURCE_CUR || g_pIData->IsOrgData(TRUE))
	{
		// 如果已经是当前数据，则不需要进行转换
		if(pICurNumbers != NULL)
		{
			// 直接复制
			pICurNumbers->Copy((CNumbers *)this);
		}
		m_btDataSource = DATA_SOURCE_CUR;
		return ;
	}
    
	// 设置临时数据
	INumbers *pITempData = pICurNumbers;
	if(pICurNumbers == NULL)
	{
		pITempData = (INumbers *)new CNumbers;	// 创建临时数据
	}
    
	// 创建分析方式下数据
	pITempData->Create(m_dwCount, DATA_SOURCE_CUR);
	for(DWORD i=0; i<m_dwCount; i++)
	{
		pITempData->SetItemData(i, GetItemData(i, TRUE));
	}
	
	pITempData->RemoveSameData();
    
	if(pICurNumbers == NULL)
	{
		// 复制并删除临时数据
		Copy(pITempData);
		delete (CNumbers *)pITempData;
	}
}

BOOL CNumbers::GetTextFileFlagString(LPSTR lpszTextOut)
{
    // TEST
	/*if((g_pIData->GetCurrentPlayType()->btFlagPrev == LPIF_ORG) && m_btDataSource == DATA_SOURCE_ORG && (((m_btDataType & DATA_TYPE_NUM_LT) || (g_pIData->GetLotteryProperty()->btSectionCount > 1)) && g_pIData->GetCurrentSectionIndex() == 0))
	{
		// 如果是原始数据，并且是乐透型彩票或者存在两种及其以上号码类型，并且当前是第一号码类型
		ASSERT(lpszTextOut != NULL);
		sprintf(lpszTextOut, "%s%08d", SAVETXTFILEFLAG, MAKEWORD(m_btNumberCount2, m_btNumberCount));
		return TRUE;
	}
    */
	return FALSE;
}

void CNumbers::ReduceComplex()
{
	if((DATA_TYPE_NUM_LT | DATA_TYPE_ORDERED) != (m_btDataType & (DATA_TYPE_NUM_LT | DATA_TYPE_ORDERED)))
	{
		return ;	// 目前只支持乐透型的复式转为单式
	}
    
	BYTE btSingleNumberCount = g_pIData->GetNumberCount(DATA_SOURCE_ORG);
	if(m_btNumberCount == btSingleNumberCount)
	{
		return ;	// 当前数据为单式，则不需要处理
	}
    
	switch(m_btDataType & DATA_TYPE_NUM_MASK)
	{
        case DATA_TYPE_NUM_LT:
            _ReduceComplex_LT(btSingleNumberCount);
            break;
        default: ASSERT(FALSE); break;
	}
}

void CNumbers::SetNumber2Data(CByteArray *pNumber2DataArray, BYTE btMode)
{
	if(pNumber2DataArray == NULL || pNumber2DataArray->GetSize() == 0)	// 清除第2个号码区段数据
	{
		_RemoveNumber2Data();
		return ;
	}
    
	/////////////////////////////////////////////////////////////////////////////////
	int nData2Count = pNumber2DataArray->GetSize();
	ASSERT(nData2Count > 0 && nData2Count < 10000);
	
	LPLOTTERYSECTIONITEM lpLSItem = &g_pIData->GetLotteryProperty()->stSectionInfo[1];
    
	BYTE btNumberCount2 = (btMode == N2DM_TOGETHER ? (BYTE)nData2Count : lpLSItem->btNumberCount);
	LPBYTE lpDataBuf = m_lpDataItemArray;
	BYTE btNewItemSize = m_btItemSize;
	BYTE btNewWholeNumberCount = m_btWholeNumberCount;
	DWORD i = 0, dwIndex = 0,  dwNewIndex = 0;
    
	nData2Count /= btNumberCount2;
    
	// 前处理
	if(btNumberCount2 != m_btNumberCount2)
	{
		// 需要重新创建数据
		btNewWholeNumberCount = (BYTE)(m_btNumberCount + btNumberCount2);
		btNewItemSize = (BYTE)(btNewWholeNumberCount + ms_btCDSize);
		lpDataBuf = new BYTE[btNewItemSize * m_dwCount];
        
		for(i=0; i<m_dwCount; i++)
		{
			// 复制第一类型数据
			memcpy(&lpDataBuf[dwNewIndex], &m_lpDataItemArray[dwIndex], m_btNumberCount);
			
			dwNewIndex += btNewWholeNumberCount;
			dwIndex += m_btWholeNumberCount;
            
			// 复制自定义数据
			memcpy(&lpDataBuf[dwNewIndex], &m_lpDataItemArray[dwIndex], ms_btCDSize);
			dwNewIndex += ms_btCDSize;
			dwIndex += ms_btCDSize;
		}
	}
    
	// 设置第二类型数据
	switch(btMode)
	{
        case N2DM_TOGETHER:		// 所有的一起添加
            dwIndex = m_btNumberCount;
            for(i=0; i<m_dwCount; i++)
            {
                memcpy(&lpDataBuf[dwIndex], pNumber2DataArray->GetData(), btNumberCount2);
                dwIndex += btNewItemSize;
            }
            break;
        case N2DM_CIRCLE:		// 循环添加每一个
            dwIndex = m_btNumberCount;
            for(i=0; i<m_dwCount; i++)
            {
                memcpy(&lpDataBuf[dwIndex], &(pNumber2DataArray->GetData()[(i % nData2Count) * btNumberCount2]), btNumberCount2);
                dwIndex += btNewItemSize;
            }
            break;
        case N2DM_RANDOM:		// 随机添加每一个dwIndex = m_btNumberCount;
            dwIndex = m_btNumberCount;
            srand(time(NULL));
            for(i=0; i<m_dwCount; i++)
            {
                memcpy(&lpDataBuf[dwIndex], &(pNumber2DataArray->GetData()[(rand() % nData2Count) * btNumberCount2]), btNumberCount2);
                dwIndex += btNewItemSize;
            }
            break;
        default: ASSERT(FALSE); break;
	}
    
	// 后处理
	if(btNumberCount2 != m_btNumberCount2)
	{
		delete []m_lpDataItemArray;
        
		m_lpDataItemArray = lpDataBuf;
		m_btNumberCount2 = btNumberCount2;
		m_btWholeNumberCount = btNewWholeNumberCount;
		m_btItemSize = btNewItemSize;
	}
	m_btDataType2 = lpLSItem->btDataType;
	m_btMaxNumber2 = lpLSItem->btMaxNumber;
	m_btMinNumber2 = lpLSItem->btMinNumber;
    
	// 保存设置
	m_btNumber2Mode = btMode;
	m_btNumber2Array.Copy(*pNumber2DataArray);
}

BYTE CNumbers::GetNumber2Data(CByteArray *pNumber2DataArray)
{
	if(pNumber2DataArray != NULL)
	{
		pNumber2DataArray->Copy(m_btNumber2Array);
	}
    
	return m_btNumber2Mode;
}

int CNumbers::FindItem(LPBYTE lpData, BOOL bHalfFind)
{
	if(bHalfFind)	// 折半查找
	{
		// 使用折半查找
		int nCompareResult = 0;
		int nMid = m_dwCount / 2, nFirst = 0, nLast = m_dwCount;
		
		// 折半查找
		while(nFirst <= nLast && nMid < (int)m_dwCount)
		{
			nCompareResult = memcmp(lpData, &m_lpDataItemArray[nMid * m_btItemSize], m_btNumberCount);
			if(nCompareResult == 0)
			{
				return nMid;	// find
			}
			else if(nCompareResult < 0)		// 如果小于当前元素值，则往前折半查找
			{
				nLast = nMid - 1;
			}
			else							// 如果大于当前元素值，则往后折半查找
			{
				nFirst = nMid + 1;
			}
			nMid = (nFirst + nLast) / 2;
		}
	}
	else			// 顺序查找
	{
		DWORD dwDataIndex = 0;
		for(DWORD i=0; i<m_dwCount; i++)
		{
			if(!memcmp(lpData, &m_lpDataItemArray[dwDataIndex], m_btNumberCount))
			{
				return i;	// find
			}
			dwDataIndex += m_btItemSize;
		}
	}
    
	return -1;	// not find
}

/////////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -- 私有函数开始
int CNumbers::_AscendSortNums(const void *arg1, const void *arg2)
{
	LPBYTE p1 = LPBYTE(arg1);
	LPBYTE p2 = LPBYTE(arg2);
	
	// 先比较自定义数据
	int dwCD1 = *((LPINT)&p1[ms_btWholeNumberCount]);
	int dwCD2 = *((LPINT)&p2[ms_btWholeNumberCount]);
	
	if(ms_bAscSortData)		// 正向排序
	{
		if(dwCD1 < dwCD2)
			return -1;
		else if(dwCD1 > dwCD2)
			return 1;
	}
	else					// 反向排序
	{
		if(dwCD1 < dwCD2)
			return 1;
		else if(dwCD1 > dwCD2)
			return -1;
	}
    
	// 再比较号码数据
	for(BYTE i=0; i<ms_btWholeNumberCount; i++)
	{
		if(p1[i] < p2[i])
			return -1;
		else if(p1[i] > p2[i])
			return 1;
	}
	
	return 0;
}

int CNumbers::_DescendSortNums(const void *arg1, const void *arg2)
{
	LPBYTE p1 = LPBYTE(arg1);
	LPBYTE p2 = LPBYTE(arg2);
	
	// 先比较自定义数据
	int dwCD1 = *((LPINT)&p1[ms_btWholeNumberCount]);
	int dwCD2 = *((LPINT)&p2[ms_btWholeNumberCount]);
    
	if(ms_bAscSortData)		// 正向排序
	{
		if(dwCD1 > dwCD2)
			return 1;
		else if(dwCD1 < dwCD2)
			return -1;
	}
	else					// 反向排序
	{
		if(dwCD1 > dwCD2)
			return -1;
		else if(dwCD1 < dwCD2)
			return 1;
	}
	
	// 再在比较号码数据
	for(BYTE i=0; i<ms_btWholeNumberCount; i++)
	{
		if(p1[i] > p2[i])
			return -1;
		else if(p1[i] < p2[i])
			return 1;
	}
	
	return 0;
}

int CNumbers::_AscendSortItem(const void *arg1, const void *arg2)
{
	BYTE a = *LPBYTE(arg1);
	BYTE b = *LPBYTE(arg2);
	
	if(a > b)
		return 1;
	if(a < b)
		return -1;
	
	return 0;
}

int CNumbers::_DescendSortItem(const void *arg1, const void *arg2)
{
	BYTE a = *LPBYTE(arg1);
	BYTE b = *LPBYTE(arg2);
	
	if(a > b)
		return -1;
	if(a < b)
		return 1;
	
	return 0;
}

void CNumbers::_GetNumbersSeparator(LPNUMBERSSEPARATOR lpNumsSep, BYTE btDataType)
{
    LPLOTTERYPROPERTY lpProperty = g_pIData->GetLotteryProperty();
    
	// 两组号码之间的分隔符
	strcpy(lpNumsSep->szBetweenNums, btDataType & DATA_TYPE_NUM_SZ ? " " : ", ");
    
	// 相邻区段之间分割符
	if(lpProperty->btSectionCount > 1)
	{
		if(btDataType & DATA_TYPE_NUM_LT || (lpProperty->stSectionInfo[1].btDataType & DATA_TYPE_NUM_LT))
		{
			strcpy(lpNumsSep->szBetweenSection, " + ");
		}
		else
		{
			strcpy(lpNumsSep->szBetweenSection, "+");
		}
	}
    
	// 各区段内单个号码间的分割符
	if(btDataType & DATA_TYPE_NUM_LT)
	{
		strcpy(lpNumsSep->szInSection[0], " ");
	}
	if(lpProperty->stSectionInfo[1].btDataType & DATA_TYPE_NUM_LT)
	{
		strcpy(lpNumsSep->szInSection[1], " ");
	}
}

void CNumbers::_GetAllDataStringByNumsType(CQuickAddString &strTextOut, int nColCount)
{
    // TEST
    /*
	// 目前只用于数字三
	ASSERT(m_btNumberCount == 3 && m_btDataType & DATA_TYPE_NUM_SZ);
	
	// 保存当前号码信息到临时对象
	CNumbers cTempNumbers;
	cTempNumbers.Copy((CNumbers *)this);
    
	DWORD dwType = 0;
	ICondition *pICondition = g_pIConditionMan->GetICondition_System(IID_STC_GENRE_NORMAL, IPID_NONE, IPID_NONE);
    
	// 按号码类型排序
	for(DWORD i=0; i<m_dwCount; i++)
	{
		dwType = pICondition->GetValue(GetItemData(i));
		if(dwType > DT_AAB)	dwType = DT_ACE;
		SetItemCustomData(i, dwType);	
	}
	Sort();
    
	CQuickAddString strTypeContext;
	CString strTypeTitle;
    
	// 按类型组字符串
	int nTypeCount = 0;
	_GetAllDataStringByCustomData(strTypeContext, nColCount, 0, DT_AAA, &nTypeCount);
	if(nTypeCount > 0)
	{
		strTypeTitle.Format("豹子（%s注）\r\n", g_pICstPubFunc->GetSBCcaseString(nTypeCount));
		strTextOut += strTypeTitle + strTypeContext + "\r\n\r\n";
	}
    
	_GetAllDataStringByCustomData(strTypeContext, nColCount, 0, DT_AAB, &nTypeCount);
	if(nTypeCount > 0)
	{
		strTypeTitle.Format("组三（%s注）\r\n", g_pICstPubFunc->GetSBCcaseString(nTypeCount));
		strTextOut += strTypeTitle + strTypeContext + "\r\n\r\n";
	}
    
	_GetAllDataStringByCustomData(strTypeContext, nColCount, 0, DT_ACE, &nTypeCount);
	if(nTypeCount > 0)
	{
		strTypeTitle.Format("组六（%s注）\r\n", g_pICstPubFunc->GetSBCcaseString(nTypeCount));
		strTextOut += strTypeTitle + strTypeContext + "\r\n\r\n";
	}
    
	// 恢复号码信息
	Copy((CNumbers *)&cTempNumbers); */
}

void CNumbers::_GetAllDataStringByCustomData(CQuickAddString &strTextOut, int nColCount, WORD wAutoAddSpaceRow, DWORD dwCustomData, int *pCountOut)
{
	if(m_dwCount == 0)
	{
		return ;
	}
	
	strTextOut.Empty();
	int nValidCount = 0, nNumsSepLen = strlen(m_stNumsSep.szBetweenNums);
	int nAutoAddRow = (HIBYTE(wAutoAddSpaceRow) & 0X7F), nAutoAddRow2 = LOBYTE(wAutoAddSpaceRow), nRowIndex = 0;
	if(!(wAutoAddSpaceRow & 0X8000))
	{
		nAutoAddRow = 0;
	}
	
	// 组数据串
	for(int i=0; i<(int)m_dwCount; i++)
	{
		if(dwCustomData != MAXDWORD && dwCustomData != GetItemCustomData(i))
		{
			continue;	// 如果自定义数据不匹配，则忽略
		}
        
		strTextOut += GetItemString(i);
		
		if(nColCount > 0 && (nValidCount + 1) % nColCount == 0)
		{
			strTextOut += "\r\n";
			if(nAutoAddRow > 0)		// 每个M行增加N空行
			{
				nRowIndex ++;
				if(nRowIndex == nAutoAddRow)
				{
					for(int j=0; j<nAutoAddRow2; j++)
					{
						strTextOut += "\r\n";
					}
					nRowIndex = 0;
				}
			}
		}
		else if(nNumsSepLen > 0)
		{
			strTextOut += m_stNumsSep.szBetweenNums;
		}
        
		nValidCount ++;
	}
    
	if(nValidCount > 0)
	{
		if(nColCount > 0 && (nValidCount) % nColCount == 0)
		{
			// 去除最后的回车换行
			strTextOut.TrimRight("\r\n");
		}
		else
		{
			// 去除最后的分割符
			strTextOut.TrimRight(nNumsSepLen);
		}
	}
    
	if(pCountOut != NULL)
	{
		*pCountOut = nValidCount;
	}
}

BOOL CNumbers::_IsNumsDigital(char caNumber)
{
	if(caNumber >= '0' && caNumber <= '9')
	{
		return TRUE;
	}
    
	if(m_btDataType & DATA_TYPE_NUM_PK)
	{
		switch(caNumber)
		{
            case 'A':
            case 'a':
            case 'T':
            case 't':
            case 'J':
            case 'j':
            case 'Q':
            case 'q':
            case 'K':
            case 'k':
                return TRUE; 
		}
	}
    
	return FALSE;
}

void CNumbers::_TransAMDataByFlag(BYTE btFlag, LPBYTE lpNums, BYTE btNumCount)
{
    // TEST
    /*
	BYTE i = 0;
	
	switch(btFlag)
	{
        case LPIF_ORDER: g_pICstPubFunc->QuickSort(lpNums, btNumCount, QSVT_BYTE); break;
        case LPIF_TAIL:
            for(i=0; i<btNumCount; i++)
            {
                lpNums[i] %= 10;
            }
            break;
        case LPIF_ORDER_TAIL: 
            g_pICstPubFunc->QuickSort(lpNums, btNumCount, QSVT_BYTE);
            for(i=0; i<btNumCount; i++)
            {
                lpNums[i] %= 10;
            }
            break;
        case LPIF_TAIL_ORDER: 
            for(i=0; i<btNumCount; i++)
            {
                lpNums[i] %= 10;
            }
            g_pICstPubFunc->QuickSort(lpNums, btNumCount, QSVT_BYTE);
	}*/
}

void CNumbers::_RemoveNumber2Data()
{
	if(m_btNumberCount2 == 0)
	{
		return ;	// 当前没有，则忽略
	}
	m_btNumber2Mode = m_btDataType2 = m_btMaxNumber2 = m_btMinNumber2 = m_btNumberCount2 = 0;
	m_btNumber2Array.RemoveAll();
	
	// 需要重新创建数据
	BYTE btNewWholeNumberCount = m_btNumberCount;
	BYTE btNewItemSize = (BYTE)(btNewWholeNumberCount + ms_btCDSize);
	LPBYTE lpDataBuf = new BYTE[btNewItemSize * m_dwCount];
	DWORD dwNewIndex = 0, dwIndex = 0;
	
	for(DWORD i=0; i<m_dwCount; i++)
	{
		// 复制第一类型数据
		memcpy(&lpDataBuf[dwNewIndex], &m_lpDataItemArray[dwIndex], m_btNumberCount);	
		dwNewIndex += btNewWholeNumberCount;
		dwIndex += m_btWholeNumberCount;
		
		// 复制自定义数据
		memcpy(&lpDataBuf[dwNewIndex], &m_lpDataItemArray[dwIndex], ms_btCDSize);
		dwNewIndex += ms_btCDSize;
		dwIndex += ms_btCDSize;
	}
	
	delete []m_lpDataItemArray;
	
	m_lpDataItemArray = lpDataBuf;
	m_btWholeNumberCount = btNewWholeNumberCount;
	m_btItemSize = btNewItemSize;
}

#pragma mark -- 以下是数字型彩票相关函数
#define		AUTOCOMBONUMS_SZ_SETITEMDATA(nNC)	\
                if(m_btNumberCount == nNC){ \
                    SetItemData(nIndex++, btData); \
                    continue; \
                }
#define		EXPANDXITEM_SETITEMDATA(nNC) \
                if((lpXData[nNC-1] != 0xFF) && (lpXData[nNC-1] != lpData[nNC-1])) \
                    continue; \
                    if(m_btNumberCount == nNC){ \
                        SetItemData(nIndex++, lpData); \
                        continue; \
                }

int CNumbers::_AutoComboNums_SZ(BOOL bOnlyCalcCount)
{	
	DWORD dwTotalCount = (DWORD)pow((m_btMaxNumber - m_btMinNumber + 1), m_btNumberCount);
    
	if(bOnlyCalcCount)
	{
		return dwTotalCount;
	}
    
	SetCount(dwTotalCount);
    
	BYTE btData[8] = {0};
	int nIndex = 0;
    
	for(btData[0]=m_btMinNumber; btData[0]<=m_btMaxNumber; btData[0]++)
	{
		AUTOCOMBONUMS_SZ_SETITEMDATA(1)
        
		for(btData[1]=m_btMinNumber; btData[1]<=m_btMaxNumber; btData[1]++)
		{
			AUTOCOMBONUMS_SZ_SETITEMDATA(2)
            
			for(btData[2]=m_btMinNumber; btData[2]<=m_btMaxNumber; btData[2]++)
			{
				AUTOCOMBONUMS_SZ_SETITEMDATA(3)
                
				for(btData[3]=m_btMinNumber; btData[3]<=m_btMaxNumber; btData[3]++)
				{
					AUTOCOMBONUMS_SZ_SETITEMDATA(4)
                    
					for(btData[4]=m_btMinNumber; btData[4]<=m_btMaxNumber; btData[4]++)
					{
						AUTOCOMBONUMS_SZ_SETITEMDATA(5)
                        
						for(btData[5]=m_btMinNumber; btData[5]<=m_btMaxNumber; btData[5]++)
						{
							AUTOCOMBONUMS_SZ_SETITEMDATA(6)
                            
							for(btData[6]=m_btMinNumber; btData[6]<=m_btMaxNumber; btData[6]++)
							{
								AUTOCOMBONUMS_SZ_SETITEMDATA(7)
                                
								for(btData[7]=m_btMinNumber; btData[7]<=m_btMaxNumber; btData[7]++)
								{
									SetItemData(nIndex++, btData);
								}
							}
						}
					}
				}
			}
		}
	}
    
	ASSERT(nIndex == (int)dwTotalCount);
    
	return dwTotalCount;
}

int CNumbers::_LoadFromString_SZ(LPCSTR lpszData, CNumbers *pNumbers)
{
	ASSERT(lpszData != NULL);
    
	int nFirstCount = pNumbers->GetNumberCount();
	int nLastCount = pNumbers->GetNumberCount2();
	int nNumberCount = nFirstCount + nLastCount;
    
	int nLen = strlen(lpszData), nIndex = 0;
	DWORD dwCount = nLen / nNumberCount, dwItemIndex = 0;
	if(dwCount == 0)
	{
		return 0;
	}
    
	// 创建号码数据对象
	pNumbers->SetCount(dwCount);
	LPBYTE lpData = new BYTE[nNumberCount];
	BOOL bIsSZLT = FALSE; // TEST m_pIData->GetDataType(DATA_SOURCE_INIT) == CSTDFF_TYPE_SZLT ? TRUE : FALSE;
	BYTE btLTNum[2] = {0}, btLTNumIndex = 0, bHasX = FALSE;
    
	CNumbers cXNumbers, cXNumbersTemp;	// 通配符处理使用
	cXNumbers.Create(0, DATA_SOURCE_CUR, 0, 0);
	cXNumbersTemp.Create(0, DATA_SOURCE_CUR, 0, 0);
    
	// 获取每个元素
	for(int i=0; i<nLen; i++)
	{
		if((lpszData[i] >= '0' && lpszData[i] <= '9') || lpszData[i] == m_caX)
		{
			if(nIndex < nFirstCount)
			{
				// 第一类型号码
				if(lpszData[i] == m_caX)	// 通配符
				{
					bHasX = TRUE;
					lpData[nIndex++] = 0xFF;
				}
				else
				{
					lpData[nIndex++] = (BYTE)(lpszData[i] - '0');
				}
			}
			else
			{
				// 第二类型号码
				if(lpszData[i] == m_caX)
				{
					continue;	// 第二类型号码不支持通配符，忽略
				}
                
				if(bIsSZLT)
				{
					btLTNum[btLTNumIndex++] = (BYTE)(lpszData[i] - '0');
					if(btLTNumIndex == 2)
					{
						lpData[nIndex] = (BYTE)(btLTNum[0] * 10 + btLTNum[1]);
						if(lpData[nIndex] < 1 || lpData[nIndex] > 12)
						{
							continue;			// 不合法，目前只支持生肖
						}
						btLTNumIndex = 0;
						nIndex ++;
					}
					else
					{
						continue;
					}
				}
				else
				{
					lpData[nIndex++] = (BYTE)(lpszData[i] - '0');
				}
			}
            
			if(nIndex == nNumberCount)
			{
				// 一个完整的号码数据
				if(bHasX) // 含有通配符，则加入相关列表
				{
					cXNumbersTemp._ExpandFromXItem_SZ(lpData);
					cXNumbers.Append((CNumbers *)&cXNumbersTemp);
				}
				else	// 正常数据
				{
					pNumbers->SetItemData(dwItemIndex++, lpData);
				}
				nIndex = 0;
			}
		}
	}
	delete []lpData;
    
	pNumbers->RemoveData();
    
	pNumbers->Append((CNumbers *)&cXNumbers);
    
	return 0;
}

int CNumbers::_ByteToChar_SZ(LPSTR lpString, LPBYTE lpBytes, int nByteCount, LPCSTR lpszSep, int nSepLen)
{
	ASSERT(lpString != NULL && lpBytes != NULL);
    
	int i, nCount = 0;
    
	for(i=0; i<nByteCount; i++)
	{
		lpString[nCount++] = char(lpBytes[i] + '0');
        
		if((nSepLen > 0) && (i < nByteCount - 1))
		{
			// 添加分割符
			memcpy(&lpString[nCount], lpszSep, nSepLen);
			nCount += nSepLen;
		}
	}
    
	return nCount;
}

void CNumbers::_ExpandFromXItem_SZ(LPBYTE lpXData)
{
	ASSERT(lpXData != NULL);
	
	RemoveAllData();
	
	// 获取通配符个数
	int nXCharCount = 0;
	for(BYTE i=0; i<m_btNumberCount; i++)
	{
		if(lpXData[i] == 0xFF)	// 通配符
		{
			nXCharCount ++;
		}
	}
	
	// 计算总个数，并初始化
	DWORD dwTotalCount = (DWORD)pow((m_btMaxNumber - m_btMinNumber + 1), nXCharCount);
	SetCount(dwTotalCount);
	
	// 展开
	LPBYTE lpData = new BYTE[m_btWholeNumberCount];
	memcpy(lpData, lpXData, m_btWholeNumberCount);
	
	int nIndex = 0;
	
	for(lpData[0]=m_btMinNumber; lpData[0]<=m_btMaxNumber; lpData[0]++)
	{
		EXPANDXITEM_SETITEMDATA(1)
        
		for(lpData[1]=m_btMinNumber; lpData[1]<=m_btMaxNumber; lpData[1]++)
		{
			EXPANDXITEM_SETITEMDATA(2)
            
			for(lpData[2]=m_btMinNumber; lpData[2]<=m_btMaxNumber; lpData[2]++)
			{
				EXPANDXITEM_SETITEMDATA(3)
                
				for(lpData[3]=m_btMinNumber; lpData[3]<=m_btMaxNumber; lpData[3]++)
				{
					EXPANDXITEM_SETITEMDATA(4)
                    
					for(lpData[4]=m_btMinNumber; lpData[4]<=m_btMaxNumber; lpData[4]++)
					{
						EXPANDXITEM_SETITEMDATA(5)
                        
						for(lpData[5]=m_btMinNumber; lpData[5]<=m_btMaxNumber; lpData[5]++)
						{
							EXPANDXITEM_SETITEMDATA(6)
							
							for(lpData[6]=m_btMinNumber; lpData[6]<=m_btMaxNumber; lpData[6]++)
							{
								EXPANDXITEM_SETITEMDATA(7)
								
								for(lpData[7]=m_btMinNumber; lpData[7]<=m_btMaxNumber; lpData[7]++)
								{
									EXPANDXITEM_SETITEMDATA(8);
								}
							}
						}
					}
				}
			}
		}
	}
	
	delete []lpData;
	
	ASSERT(nIndex == (int)dwTotalCount);
}

/////////////////////////////////////////////////////////////////////////////
#pragma mark -- 以下是乐透型彩票相关函数
#define		AUTOCOMBONUMSLT_SETITEMDATA(nNC) \
                if(m_btNumberCount == nNC){ \
                    SetItemData(nIndex++, btData); \
                    continue; \
                }

#define		REDUCECOMPLEXLT_SETITEMDATA(nNC) \
                if(btSingleNumberCount == nNC) { \
                    /* 第一类型数据赋值 */ \
                    for(j=0; j<btSingleNumberCount; j++) \
                    lpSingleData[j] = lpData[n[j]]; \
                    /* 第二类型数据赋值 */ \
                    memcpy(&lpSingleData[btSingleNumberCount], &lpData[m_btNumberCount], m_btNumberCount2); \
                    cSingleData.SetItemData(dwIndex++, lpSingleData); \
                    dwSingleIndex += btSingleItemSize;	/* 下一个数据 */ \
                    continue; \
                }

/////////////////////////////////////////////////////////////////////////////
int CNumbers::_AutoComboNums_LT(BOOL bOnlyCalcCount)
{
	// 计算号码个数
	DWORD dwTotalCount = _Comb_LT(m_btMaxNumber, m_btNumberCount);
    
	if(bOnlyCalcCount)
	{
		return dwTotalCount;
	}
    
	SetCount(dwTotalCount);
	
	BYTE btData[8] = {0};
	int nIndex = 0;
    
	for(btData[0]=m_btMinNumber; btData[0]<=m_btMaxNumber-m_btNumberCount+1; btData[0]++)
	{
		AUTOCOMBONUMSLT_SETITEMDATA(1)
        
		for(btData[1]=(BYTE)(btData[0]+1); btData[1]<=m_btMaxNumber-m_btNumberCount+2; btData[1]++)
		{
			AUTOCOMBONUMSLT_SETITEMDATA(2)
            
			for(btData[2]=(BYTE)(btData[1]+1); btData[2]<=m_btMaxNumber-m_btNumberCount+3; btData[2]++)
			{
				AUTOCOMBONUMSLT_SETITEMDATA(3)
                
				for(btData[3]=(BYTE)(btData[2]+1); btData[3]<=m_btMaxNumber-m_btNumberCount+4; btData[3]++)
				{
					AUTOCOMBONUMSLT_SETITEMDATA(4)
                    
					for(btData[4]=(BYTE)(btData[3]+1); btData[4]<=m_btMaxNumber-m_btNumberCount+5; btData[4]++)
					{
						AUTOCOMBONUMSLT_SETITEMDATA(5)
                        
						for(btData[5]=(BYTE)(btData[4]+1); btData[5]<=m_btMaxNumber-m_btNumberCount+6; btData[5]++)
						{
							AUTOCOMBONUMSLT_SETITEMDATA(6)
                            
							for(btData[6]=(BYTE)(btData[5]+1); btData[6]<=m_btMaxNumber-m_btNumberCount+7; btData[6]++)
							{
								AUTOCOMBONUMSLT_SETITEMDATA(7)
                                
								for(btData[7]=(BYTE)(btData[6]+1); btData[7]<=m_btMaxNumber-m_btNumberCount+8; btData[7]++)
								{
									SetItemData(nIndex++, btData);
								}
							}
						}
					}
				}
			}
		}
	}
    
	ASSERT(nIndex == (int)dwTotalCount);
    
	return dwTotalCount;
}

int CNumbers::_LoadFromString_LT(LPCSTR lpszData, CNumbers *pNumbers)
{
	ASSERT(lpszData != NULL);
	
	int nFirstCount = pNumbers->GetNumberCount();
	int nLastCount = pNumbers->GetNumberCount2();
	int nNumberCount = nFirstCount + nLastCount;
    
	int i, j, k = 0, nIndex = 0, nLen = strlen(lpszData), nNumIndex = 0;
	int nCount = nLen / (nNumberCount * 2);
	if(nCount == 0)
	{
		return 0;
	}
	
	pNumbers->SetCount(nCount);
    
	BOOL bGetFirst = TRUE;
	int nFirstMaxNumber = 33; // TESTg_pIData->GetMaxNumber(DATA_SOURCE_ORG);
    int nLastMaxNumber = 16; // g_pIData->GetLotteryProperty()->stSectionInfo[1].btMaxNumber;
	BYTE btNum = 0, btTemp[64] = {0}, btNums[2] = {0};
	LPBYTE lpValidData = new BYTE[nNumberCount];
    
	for(i=0; i<nLen; i++)
	{
		if(lpszData[i] >= '0' && lpszData[i] <= '9')
		{
			btNums[nNumIndex] = (BYTE)(lpszData[i] - '0');
			if(++nNumIndex < 2)
				continue;
		}
		else
		{
			continue;
		}
        
		nNumIndex = 0;
        
		btNum = (BYTE)(btNums[0] * 10 + btNums[1]);
		if(bGetFirst)	// 获取红球号码
		{
			if(btNum > 0 && btNum <= nFirstMaxNumber)
			{
				if(btTemp[btNum] == 1)
				{
					continue;		// 重复数字
				}
				else
				{
					btTemp[btNum] = 1;
                    
					k ++;
					if(k == nFirstCount)
					{
						// 红球获取完毕
						k = 0;
						for(j=1; j<=nFirstMaxNumber; j++)
						{
							if(btTemp[j] == 1)
							{
								lpValidData[k++] = (BYTE)(j);
							}
						}
                        
						ASSERT(nIndex < nCount);
						ZeroMemory(btTemp, sizeof(btTemp));
						k = 0;
						if(nLastCount > 0)
						{
							bGetFirst = FALSE;  // 需要得蓝球
						}
						else
						{
							pNumbers->SetItemData(nIndex, lpValidData);	// 号码完毕
							nIndex ++;
						}
					}
				}
			}
		}
		else	// 获取蓝球号码
		{
			if(btNum > 0 && btNum <= nLastMaxNumber)
			{
				if(btTemp[btNum] == 1)
				{
					continue;		// 重复数字
				}
				else
				{
					btTemp[btNum] = 1;
					k ++;
					if(k == nLastCount)
					{
						k = nFirstCount;
						for(j=1; j<=nLastMaxNumber; j++)
						{
							if(btTemp[j] == 1)
							{
								lpValidData[k++] = (BYTE)(j);
							}
						}
						ZeroMemory(btTemp, sizeof(btTemp));
						k = 0;
						bGetFirst = TRUE;
						pNumbers->SetItemData(nIndex, lpValidData);	// 号码完毕
						nIndex ++;
					}
				}
			}
		}
	}
	
	delete []lpValidData;
    
	pNumbers->RemoveData();
    
	return 0;
}

int CNumbers::_ByteToChar_LT(LPSTR lpString, LPBYTE lpBytes, int nByteCount, LPCSTR lpszSep, int nSepLen)
{
	ASSERT(lpString != NULL && lpBytes != NULL);
	
	int i, nCount = 0;
	
	for(i=0; i<nByteCount; i++)
	{
		lpString[nCount++] = char(lpBytes[i] / 10 + '0');
		lpString[nCount++] = char(lpBytes[i] % 10 + '0');
		
		if((nSepLen > 0) && (i < nByteCount - 1))
		{
			// 添加分割符
			memcpy(&lpString[nCount], lpszSep, nSepLen);
			nCount += nSepLen;
		}
	}
	
	return nCount;
}

void CNumbers::_ReduceComplex_LT(BYTE btSingleNumberCount)
{
	// 计算每一个复式可以分解成单式的个数
	int i, j, nCellCount = 1, nTemp = 1, nTemp2 = 1;
	for(i=1; i<=btSingleNumberCount; i++)
	{
		nTemp *= i;
		nTemp2 *= (m_btNumberCount - i + 1);
	}
	nCellCount = nTemp2 / nTemp;
    
	// 创建单式数据
	CNumbers cSingleData;
	cSingleData.Create(nCellCount * m_dwCount, m_btDataSource, btSingleNumberCount, m_btNumberCount2);
    
	DWORD dwIndex = 0, dwSingleIndex = 0;
	int n[8] = {0};
	BYTE btSingleItemSize = (BYTE)(btSingleNumberCount + m_btNumberCount2 + ms_btCDSize);
	LPBYTE lpData = m_lpDataItemArray, lpSingleData = new BYTE[btSingleItemSize];
    
	for(i=0; i<(int)m_dwCount; i++)
	{
		for(n[0]=0; n[0]<m_btNumberCount-(btSingleNumberCount-1); n[0]++)
		{
			REDUCECOMPLEXLT_SETITEMDATA(1);
            
			for(n[1]=n[0]+1; n[1]<m_btNumberCount-(btSingleNumberCount-2); n[1]++)
			{
				REDUCECOMPLEXLT_SETITEMDATA(2);
                
				for(n[2]=n[1]+1; n[2]<m_btNumberCount-(btSingleNumberCount-3); n[2]++)
				{
					REDUCECOMPLEXLT_SETITEMDATA(3);
                    
					for(n[3]=n[2]+1; n[3]<m_btNumberCount-(btSingleNumberCount-4); n[3]++)
					{
						REDUCECOMPLEXLT_SETITEMDATA(4);
                        
						for(n[4]=n[3]+1; n[4]<m_btNumberCount-(btSingleNumberCount-5); n[4]++)
						{
							REDUCECOMPLEXLT_SETITEMDATA(5);
                            
							for(n[5]=n[4]+1; n[5]<m_btNumberCount-(btSingleNumberCount-6); n[5]++)
							{
								REDUCECOMPLEXLT_SETITEMDATA(6);
                                
								for(n[6]=n[5]+1; n[6]<m_btNumberCount-(btSingleNumberCount-7); n[6]++)
								{
									REDUCECOMPLEXLT_SETITEMDATA(7);
                                    
									for(n[7]=n[6]+1; n[7]<m_btNumberCount-(btSingleNumberCount-8); n[7]++)
									{
										REDUCECOMPLEXLT_SETITEMDATA(8);
									}
								}
							}
						}
					}
				}
			}
		}
        
		lpData += m_btItemSize;		// 下一个数据
	}
    
	ASSERT(dwIndex == cSingleData.GetCount());
    
	delete []lpSingleData;
    
	// 排序，并去掉重复号码
	cSingleData.RemoveSameData();
    
	Copy(&cSingleData);
}

void CNumbers::_Initialize_LT_Prev4_ValueArray()
{
	if(m_dwPrev4LTValueArray.GetSize() > 0)
	{
		return ;	// 已经初始化过
	}
    
	BYTE btData[8] = {0};
	int nTotal = (int)pow(m_btMaxNumber, 4);
	m_dwPrev4LTValueArray.SetSize(nTotal);
	
	for(btData[0]=m_btMinNumber; btData[0]<=m_btMaxNumber-m_btNumberCount+1; btData[0]++)
	{
		for(btData[1]=(BYTE)(btData[0]+1); btData[1]<=m_btMaxNumber-m_btNumberCount+2; btData[1]++)
		{
			for(btData[2]=(BYTE)(btData[1]+1); btData[2]<=m_btMaxNumber-m_btNumberCount+3; btData[2]++)
			{
				for(btData[3]=(BYTE)(btData[2]+1); btData[3]<=m_btMaxNumber-m_btNumberCount+4; btData[3]++)
				{
					btData[4] = (BYTE)(btData[3] + 1);
					btData[5] = (BYTE)(btData[3] + 2);
					btData[6] = (BYTE)(btData[3] + 3);
					btData[7] = (BYTE)(btData[3] + 4);
					m_dwPrev4LTValueArray.SetAt(_GetItemValue_LT_Prev4_Index(btData), _LT_To_Index(btData, m_btNumberCount));
				}
			}
		}
	}
}

DWORD CNumbers::_LT_To_Index(LPBYTE lpData, BYTE btNumberCount)
{
	DWORD dwValue = 0;
	BYTE btMinNumber = m_btMinNumber;
    
	for(int i=0; i<btNumberCount-1; i++)
	{
		if(lpData[i] > btMinNumber)
			dwValue += _CalcXN(m_btMaxNumber-btMinNumber, btNumberCount - i - 1, lpData[i]-btMinNumber);
        
		btMinNumber = (BYTE)(lpData[i] + 1);
	}
    
	dwValue += lpData[btNumberCount-1] - btMinNumber;		// 最后一个号码变化个数
    
	return dwValue;
}

DWORD CNumbers::_GetItemValue_LT_Prev4_Index(LPBYTE lpData)
{
	DWORD dwValue = 0;
	int i = 0, j = 1, nRatio = (m_btMaxNumber - m_btNumberCount + 4);
	
	for(i=3; i>=0; i--)
	{
		dwValue += lpData[i] * j;
		j *= nRatio;
	}
	
	return dwValue;
}

DWORD CNumbers::_GetItemValue_LT(LPBYTE lpData)
{
	if(m_btNumberCount < 5)	// 4个及其以下的号码的则不需要优化，直接返回
	{
		return _LT_To_Index(lpData, m_btNumberCount);
	}
    
	// 大于等于5个号码需要优化
	DWORD dwValue = 0, dwIndex = 0;
	int i = 0, nTemp = 0;
	
	// 获取索引信息
	dwIndex = _GetItemValue_LT_Prev4_Index(lpData);
	
	// 初始化索引信息集合
	_Initialize_LT_Prev4_ValueArray();
    
	// 获取索引值
	dwValue = m_dwPrev4LTValueArray.GetAt(dwIndex);
	
	BYTE btMinNumber = (BYTE)(lpData[3] + 1);
	
	// 获取4位以后的值
	for(i=4; i<m_btNumberCount-1; i++)
	{
		nTemp = m_btNumberCount - i - 1;
		
		if(lpData[i] > btMinNumber)
			dwValue += _CalcXN(m_btMaxNumber-btMinNumber, nTemp, lpData[i]-btMinNumber);
		
		btMinNumber = (BYTE)(lpData[i] + 1);
	}
	
	dwValue += lpData[i] - btMinNumber;		// 最后一个号码变化个数
	
	return dwValue;
}

DWORD CNumbers::_Comb_LT(int m, int n)
{
	if(n <= 0 || m <= n)
	{
		return 1;
	}
    
	DWORD dwValue = m;
    
	for(int i=1; i<n; i++)
	{
		// 分开乘除，是为了防止连续相乘时溢出导致结果不正确
		dwValue *= (m - i);
		dwValue /= (i + 1);
	}
    
	return dwValue;
}

// 计算 C(m, n) + C(m-1, n) + ... + C(m-k, n)
// = (m(m-1)(m-2)...(m-n+1) + (m-1)(m-1-1)...(m-1-n+1) + ... + (m-k)(m-k-1)...(m-k-n+1)) / n!
// 令 x = m-n+1   y = m-k-n+1
// = (x(x+1)...(x+n) - y(y+1)...(y+n))/(n+1)!
DWORD CNumbers::_CalcXN(int m, int n, int k)
{
	if(k <= 0 || m < n || m < k)
	{
		return 0;
	}
	
	int x = m - n + 1, y = m - k - n + 1;
	
	DWORD dwValue = x, dwValue2 = y;
	
	for(int i=1; i<=n; i++)
	{
		// 分开乘除，是为了防止连续相乘时溢出导致结果不正确
		dwValue *= (x + i);
		dwValue /= (i + 1);
		
		dwValue2 *= (y + i);
		dwValue2 /= (i + 1);
	}
	
	return dwValue - dwValue2;
}

#pragma mark -- 以下是按位乐透型彩票相关函数
#define		AUTOCOMBONUMSPOSLT_SETITEMDATA(nNC)	\
                for(i=0; i<nNC-1; i++){ \
                    if(btData[i] == btData[nNC - 1]) break; \
                } \
                if(i != nNC - 1) continue; /* 和前面的号码重复，则忽略 */\
                if(m_btNumberCount == nNC){ \
                    SetItemData(nIndex++, btData); \
                    continue; \
                }

int CNumbers::_AutoComboNums_POSLT(BOOL bOnlyCalcCount)
{
	// 计算号码个数
	DWORD dwTotalCount = _Comb_POSLT(m_btMaxNumber, m_btNumberCount);
    
	if(bOnlyCalcCount)
	{
		return dwTotalCount;
	}
    
	SetCount(dwTotalCount);
	
	BYTE btData[8] = {0};
	int i = 0, nIndex = 0;
	
	for(btData[0]=m_btMinNumber; btData[0]<=m_btMaxNumber; btData[0]++)
	{
		AUTOCOMBONUMSPOSLT_SETITEMDATA(1)
        
		for(btData[1]=m_btMinNumber; btData[1]<=m_btMaxNumber; btData[1]++)
		{
			AUTOCOMBONUMSPOSLT_SETITEMDATA(2)
            
			for(btData[2]=m_btMinNumber; btData[2]<=m_btMaxNumber; btData[2]++)
			{
				AUTOCOMBONUMSPOSLT_SETITEMDATA(3)
                
				for(btData[3]=m_btMinNumber; btData[3]<=m_btMaxNumber; btData[3]++)
				{
					AUTOCOMBONUMSPOSLT_SETITEMDATA(4)
                    
					for(btData[4]=m_btMinNumber; btData[4]<=m_btMaxNumber; btData[4]++)
					{
						AUTOCOMBONUMSPOSLT_SETITEMDATA(5)
                        
						for(btData[5]=m_btMinNumber; btData[5]<=m_btMaxNumber; btData[5]++)
						{
							AUTOCOMBONUMSPOSLT_SETITEMDATA(6)
                            
							for(btData[6]=m_btMinNumber; btData[6]<=m_btMaxNumber; btData[6]++)
							{
								AUTOCOMBONUMSPOSLT_SETITEMDATA(7)
                                
								for(btData[7]=m_btMinNumber; btData[7]<=m_btMaxNumber; btData[7]++)
								{
									AUTOCOMBONUMSPOSLT_SETITEMDATA(8)
								}
							}
						}
					}
				}
			}
		}
	}
	
	ASSERT(nIndex == (int)dwTotalCount);
    
	return dwTotalCount;
}

int CNumbers::_LoadFromString_POSLT(LPCSTR lpszData, CNumbers *pNumbers)
{
	ASSERT(lpszData != NULL);
	
	int nFirstCount = pNumbers->GetNumberCount();
	int nLastCount = pNumbers->GetNumberCount2();
	int nNumberCount = nFirstCount + nLastCount;
    
	int i, j, k = 0, nIndex = 0, nLen = strlen(lpszData), nNumIndex = 0;
	int nCount = nLen / (nNumberCount * 2);
	if(nCount == 0)
	{
		return 0;
	}
	
	pNumbers->SetCount(nCount);
    
	BOOL bGetFirst = TRUE;
	int nFirstMaxNumber = g_pIData->GetMaxNumber(DATA_SOURCE_ORG), nLastMaxNumber = g_pIData->GetLotteryProperty()->stSectionInfo[1].btMaxNumber;
	BYTE btNum = 0, btTemp[64] = {0}, btNums[2] = {0};
	LPBYTE lpValidData = new BYTE[nNumberCount];
    
	for(i=0; i<nLen; i++)
	{
		if(lpszData[i] >= '0' && lpszData[i] <= '9')
		{
			btNums[nNumIndex] = (BYTE)(lpszData[i] - '0');
			if(++nNumIndex < 2)
				continue;
		}
		else
		{
			continue;
		}
        
		nNumIndex = 0;
        
		btNum = (BYTE)(btNums[0] * 10 + btNums[1]);
		if(bGetFirst)	// 获取第一类型的号码
		{
			if(btNum > 0 && btNum <= nFirstMaxNumber)
			{
				if(btTemp[btNum] == 1)
				{
					continue;		// 重复数字
				}
				else
				{
					btTemp[btNum] = 1;
					lpValidData[k++] = btNum;
                    
					if(k == nFirstCount)
					{
						// 号码获取完毕
                        
						ASSERT(nIndex < nCount);
						ZeroMemory(btTemp, sizeof(btTemp));
						k = 0;
						if(nLastCount > 0)
						{
							bGetFirst = FALSE;  // 需要得蓝球
						}
						else
						{
							pNumbers->SetItemData(nIndex, lpValidData);	// 号码完毕
							nIndex ++;
						}
					}
				}
			}
		}
		else	// 获取第二类型的号码
		{
			if(btNum > 0 && btNum <= nLastMaxNumber)
			{
				if(btTemp[btNum] == 1)
				{
					continue;		// 重复数字
				}
				else
				{
					btTemp[btNum] = 1;
					k ++;
					if(k == nLastCount)
					{
						k = nFirstCount;
						for(j=1; j<=nLastMaxNumber; j++)
						{
							if(btTemp[j] == 1)
							{
								lpValidData[k++] = (BYTE)(j);
							}
						}
						ZeroMemory(btTemp, sizeof(btTemp));
						k = 0;
						bGetFirst = TRUE;
						pNumbers->SetItemData(nIndex, lpValidData);	// 号码完毕
						nIndex ++;
					}
				}
			}
		}
	}
	
	delete []lpValidData;
    
	pNumbers->RemoveData();
    
	return 0;
}

DWORD CNumbers::_Comb_POSLT(int m, int n)
{
	if(n <= 0)
		return 1;
    
	DWORD dwValue = m;
	for(int i=1; i<n; i++)
	{
		dwValue *= (m - i);
	}
    
	return dwValue;
}

DWORD CNumbers::_GetItemValue_POSLT(LPBYTE lpData)
{
	int m = m_btMaxNumber - m_btMinNumber, i = 0, j = 0, nIgnoreCount = 0;
	DWORD dwValue = (lpData[i] - m_btMinNumber) * _Comb_POSLT(m, m_btNumberCount-1);;
    
	for(i=1; i<m_btNumberCount; i++)
	{
		nIgnoreCount = 0;
		for(j=0; j<i; j++)
		{
			if(lpData[i] > lpData[j])
				nIgnoreCount ++;		// 如果该数字大于前面的数字，则需要忽略该数字
		}
        
		m --;
		dwValue += (lpData[i] - m_btMinNumber - nIgnoreCount) * _Comb_POSLT(m, m_btNumberCount-i-1);
	}
    
	return dwValue;
}
