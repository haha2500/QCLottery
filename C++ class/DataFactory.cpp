//
//  DataFactory.cpp
//  Graph
//
//  Created by 武 孙 on 12-5-10.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

// DataFactory.cpp: implementation of the CDataFactory class.
//
//////////////////////////////////////////////////////////////////////

#include "DataFactory.h"
#include "IData.h"
#include "Numbers.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDataFactory::CDataFactory()
{
	m_dwCurUsedDataItemCount = m_dwCountOfDataItems = 0;
	m_lpDataItems = NULL;
	m_cExtDataItemArray.RemoveAll();
	m_dwBatchDeleteCount = 0;
	m_bIsCurData = FALSE;
	ZeroMemory(m_szTempBuf, sizeof(m_szTempBuf));
	ZeroMemory(&m_stForcastItem, sizeof(m_stForcastItem));
	m_pNumbers = NULL;
}

CDataFactory::~CDataFactory()
{
	Destroy();
    
    if(m_pNumbers != NULL)
    {
        delete (CNumbers *)m_pNumbers;
        m_pNumbers = NULL;
    }
}

BOOL CDataFactory::Create(DWORD dwCount, BOOL bCurData)
{
	Destroy();
    
	m_bIsCurData = bCurData;
    
    if(dwCount > 0)
    {
        NEW_BUFFER(m_lpDataItems, DATAITEM, dwCount);
    }
    
	m_dwCountOfDataItems = dwCount;
	m_dwCurUsedDataItemCount = dwCount;
    
	m_dwBatchDeleteCount = 0;
    
	if(m_pNumbers == NULL)
	{
		m_pNumbers = new CNumbers;
	}
	m_pNumbers->Create(1, bCurData ? DATA_SOURCE_CUR : DATA_SOURCE_INIT);
	return TRUE;
}

void CDataFactory::Destroy()
{
	if(m_lpDataItems == NULL)
	{
		return ;
	}
    
	DeleteAll();
    //	m_pNumbers->Destroy();
}

void CDataFactory::OnNumberCountChanged()
{
	ASSERT(m_bIsCurData);		// 只用于当前数据
    
	m_pNumbers->Destroy();
	m_pNumbers->Create(1, DATA_SOURCE_CUR);
}

DWORD CDataFactory::GetCount(BOOL bIgnoreOnlyTest)
{
	if(m_bIsCurData && bIgnoreOnlyTest)	// 当前数据并且忽略只含有试机号的数据
	{
		if(m_dwCurUsedDataItemCount > 0 && _GetDataItem(DATA_INDEX_ONLYTESTNUMS) != NULL)
		{
			return m_dwCurUsedDataItemCount - 1;	// 最后一期只有试机号没有开奖号
		}
	}
    
	return m_dwCurUsedDataItemCount;
}

void CDataFactory::SetCount(DWORD dwCount)
{
	if(dwCount == 0)
	{
		DeleteAll();
		return ;
	}
	
	if(dwCount == m_dwCurUsedDataItemCount)
	{
		return ;	// 如果和设置前的总数相同，则不处理，直接使用原来的
	}
	
	if(dwCount < m_dwCurUsedDataItemCount)
	{
		// 如果小于设置前的总数，则删除原来的多余的，然后直接使用
		int nDeleteCount = m_dwCurUsedDataItemCount - dwCount;
		for(int i=0; i<nDeleteCount; i++)
		{
			DeleteLast();
		}
		return ;
	}
	
	// 如果大于设置前的总数，则需要创建新的，并把以前的数据都复制过来
	LPDATAITEM lpOldArray = m_lpDataItems;
    
	m_lpDataItems = NULL;
	NEW_BUFFER(m_lpDataItems, DATAITEM, dwCount);
	memcpy(m_lpDataItems, lpOldArray, sizeof(DATAITEM) * m_dwCurUsedDataItemCount);
    
	for(int i=0; i<m_cExtDataItemArray.GetSize(); i++)
	{
		if(m_dwCurUsedDataItemCount + i >= dwCount)
		{
			break;
		}
		memcpy(&m_lpDataItems[m_dwCurUsedDataItemCount + i], (LPDATAITEM)m_cExtDataItemArray.GetAt(i), sizeof(DATAITEM));
	}
	
	m_dwCurUsedDataItemCount = m_dwCountOfDataItems = dwCount;
	
	// 删除原来的
	DELETE_BUFFER(lpOldArray);
	
	// 删除额外的单元
	for(int i=m_cExtDataItemArray.GetSize()-1; i>=0; i--)
	{
		delete (LPDATAITEM)m_cExtDataItemArray.GetAt(i);
	}
	m_cExtDataItemArray.RemoveAll();
}

void CDataFactory::Copy(CDataFactory *pSrcDataFactory)
{
	ASSERT(pSrcDataFactory != NULL);
    
	if(m_dwCountOfDataItems != pSrcDataFactory->m_dwCountOfDataItems)
	{
		DELETE_BUFFER(m_lpDataItems);
		m_dwCountOfDataItems = pSrcDataFactory->m_dwCountOfDataItems;
		m_lpDataItems = new DATAITEM[m_dwCountOfDataItems];
	}
    
	m_bIsCurData = pSrcDataFactory->m_bIsCurData;
	memcpy(m_lpDataItems, pSrcDataFactory->m_lpDataItems, m_dwCountOfDataItems * sizeof(DATAITEM));
	m_cExtDataItemArray.Copy(pSrcDataFactory->m_cExtDataItemArray);
    
	m_dwCurUsedDataItemCount = pSrcDataFactory->m_dwCurUsedDataItemCount;
	m_dwBatchDeleteCount = pSrcDataFactory->m_dwBatchDeleteCount;
	strcpy(m_szTempBuf, pSrcDataFactory->m_szTempBuf);
	memcpy(&m_stForcastItem, &pSrcDataFactory->m_stForcastItem, sizeof(m_stForcastItem));
    
	m_pNumbers->Destroy();
	m_pNumbers->Create(1, m_bIsCurData ? DATA_SOURCE_CUR : DATA_SOURCE_INIT);
}

BOOL CDataFactory::SetDataFileItem(DWORD dwDataIndex, LPDATAFILEITEM lpDataFileItem)
{
 	ASSERT(!m_bIsCurData);	// 只能用于初始数据

    return SetItemInfo(dwDataIndex, lpDataFileItem->dwDateTime, lpDataFileItem->dwIssue, lpDataFileItem->btData);
}

BOOL CDataFactory::GetDataFileItem(DWORD dwDataIndex, LPDATAFILEITEM lpDataFileItem)
{
    ASSERT(!m_bIsCurData);	// 只能用于初始数据
    ASSERT(lpDataFileItem != NULL);
    
    LPDATAITEM lpDataItem = _GetAt(dwDataIndex);
	if(lpDataItem == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
    
    lpDataFileItem->dwDateTime = lpDataItem->dwDataTime;
    lpDataFileItem->dwIssue = lpDataItem->dwIssue;
    memcpy(lpDataFileItem->btData, lpDataItem->btData, sizeof(lpDataFileItem->btData));
    
    return TRUE;
}

BOOL CDataFactory::SetItemInfo(DWORD dwDataIndex, DWORD dwDataTime, DWORD dwIssue, LPBYTE lpData)
{
	LPDATAITEM lpDataItem = _GetAt(dwDataIndex);
	if(lpDataItem == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
    
	ASSERT(lpData != NULL);
    
	lpDataItem->dwDataTime = dwDataTime;
	lpDataItem->dwIssue = dwIssue;
    
	memcpy(lpDataItem->btData, lpData, sizeof(lpDataItem->btData));
    
	return TRUE;
}

BOOL CDataFactory::AddItemInfo(DWORD dwDataTime, DWORD dwIssue, LPBYTE lpData)
{
	ASSERT(!m_bIsCurData);	// 只能用于初始数据
    
    // 获取上一期数据并比较期号
    int nPrevIndex = GetCount(FALSE) - 1;
    if(nPrevIndex >= 0)
    {
        LPDATAITEM lpPrevItem = _GetAt(nPrevIndex);
        if (lpPrevItem->dwIssue == dwIssue) // 如果当期与上一期期号相同，则直接覆盖相关数据
        {
            lpPrevItem->dwDataTime = dwDataTime;
            memcpy(lpPrevItem->btData, lpData, MAX_NUMBER_COUNT);
            return TRUE;
        }
        else if(lpPrevItem->dwIssue >= dwIssue)
        {
            return FALSE;   // 期号不正确
        }
    }
    
	LPDATAITEM lpDataItem = _AddNew();
	if(lpDataItem == NULL)
	{
		ASSERT(FALSE);
		return FALSE;
	}
	
	ASSERT(lpData != NULL);
	
	lpDataItem->dwDataTime = dwDataTime;
	lpDataItem->dwIssue = dwIssue;
	
	memcpy(lpDataItem->btData, lpData, sizeof(lpDataItem->btData));
    
	return TRUE;
}

void CDataFactory::SetItemData(DWORD dwDataIndex, LPBYTE lpData)
{
	memcpy(_GetAt(dwDataIndex)->btData, lpData, MAX_NUMBER_COUNT);
}

BOOL CDataFactory::CopyItem(DWORD dwDataIndex, DWORD dwInitDataIndex, CDataFactory *pInitDataFactory)
{
	ASSERT(m_bIsCurData && pInitDataFactory != NULL);
	
	LPDATAITEM lpInitDataItem = pInitDataFactory->_GetAt(dwInitDataIndex);
	if(lpInitDataItem == NULL)
	{
		return FALSE;
	}
	
	SetItemInfo(dwDataIndex, dwInitDataIndex, DATAITEM_FLAG_NONE, lpInitDataItem->btData);
	return TRUE;
}

void CDataFactory::ModifyItemData(DWORD dwDataIndex, int nTestNumsFlag)
{
	ASSERT(m_bIsCurData);	// 必须是当前数据
	LPDATAITEM lpDataItem = _GetAt(dwDataIndex);
	if(lpDataItem == NULL)
	{
		return ;
	}
    
	if(nTestNumsFlag == TESTNUMSFLAG_JOIN)	// 试机号加入开奖号
	{
		memcpy(lpDataItem->btData, &lpDataItem->btData[5], 3);
		lpDataItem->dwFlag = DATAITEM_FLAG_TESTNUM;		// 置由试机号变来的标志
	}
	else	// 试机号和开奖号互换
	{
		BYTE btTemp[3] = {0};
		memcpy(btTemp, lpDataItem->btData, 3);
		memcpy(lpDataItem->btData, &lpDataItem->btData[5], 3);
		memcpy(&lpDataItem->btData[5], btTemp, 3);
	}
}

void CDataFactory::OrderItemData(DWORD dwDataIndex, BYTE btOrderType, BOOL bHasTestNums)
{
    // TEST
/*	LPBYTE lpItemData = _GetAt(dwDataIndex)->btData;
    
	if(bHasTestNums && (lpItemData[5] != MAXBYTE))
	{
		// 试机号快速排序
		g_pICstPubFunc->QuickSort(&lpItemData[5], m_pNumbers->GetNumberCount());
		
		// 分析试机号组三排列方式
		_OrderNumsByZ3Type(lpItemData, btOrderType, 5);
        
		if(lpItemData[0] == MAXBYTE)
		{
			return ;	// 没有开奖号
		}
	}
	
	// 快速排序开奖号
	g_pICstPubFunc->QuickSort(lpItemData, m_pNumbers->GetNumberCount());
	
	// 分析组三排列方式
	_OrderNumsByZ3Type(lpItemData, btOrderType, 0);*/
}

BOOL CDataFactory::DeleteLast()
{	
	if(m_dwCurUsedDataItemCount > m_dwCountOfDataItems)
	{
		int nIndex = m_dwCurUsedDataItemCount - m_dwCountOfDataItems - 1;
		ASSERT(nIndex == m_cExtDataItemArray.GetSize() - 1);
		
		delete (LPDATAITEM)m_cExtDataItemArray.GetAt(nIndex);
		m_cExtDataItemArray.RemoveAt(nIndex);
	}
    
	m_dwCurUsedDataItemCount --;
    
	return TRUE;
}

void CDataFactory::DeleteAll()
{
	DELETE_BUFFER(m_lpDataItems);
	m_dwCurUsedDataItemCount = m_dwCountOfDataItems = m_dwBatchDeleteCount = 0;
	
	for(int i=m_cExtDataItemArray.GetSize()-1; i>=0; i--)
	{
		delete (LPDATAITEM)m_cExtDataItemArray.GetAt(i);
	}
	m_cExtDataItemArray.RemoveAll();
}

void CDataFactory::BeginBatchDelete()
{
	ASSERT(m_dwBatchDeleteCount == 0);
    
	m_dwBatchDeleteCount = 0;
}

void CDataFactory::BatchDelete(DWORD dwDataIndex)
{
	LPDATAITEM lpDataItem = _GetAt(dwDataIndex);
    
	if(lpDataItem == NULL || lpDataItem->dwIssue == MAXDWORD)
	{
		ASSERT(FALSE);
		return ;
	}
    
	lpDataItem->dwIssue = MAXDWORD;
	m_dwBatchDeleteCount ++;
}

void CDataFactory::EndBatchDelete()
{
	if(m_dwBatchDeleteCount == 0)	// 没有需要删除的
	{
		return ;
	}
    
	if(m_dwCurUsedDataItemCount <= m_dwBatchDeleteCount)	// 完全删除
	{
		DeleteAll();
		return ;
	}
    
	// 创建新的缓冲区
	int nNewSize = m_dwCurUsedDataItemCount - m_dwBatchDeleteCount, nIndex = 0;
	LPDATAITEM lpNewDataItemArray = NULL, lpSrcItem = NULL;
	NEW_BUFFER(lpNewDataItemArray, DATAITEM, nNewSize);
	
	// 赋值
	for(DWORD i=0; i<m_dwCurUsedDataItemCount; i++)
	{
		lpSrcItem = _GetAt(i);
		if(lpSrcItem != NULL && lpSrcItem->dwIssue != MAXDWORD)
		{
			memcpy(&lpNewDataItemArray[nIndex], lpSrcItem, sizeof(DATAITEM));
			nIndex ++;
		}
	}
    
	ASSERT(nIndex == nNewSize);
    
	// 删除原来的
	DeleteAll();
    
	// 组成新的
	m_dwCurUsedDataItemCount = m_dwCountOfDataItems = nNewSize;
	m_lpDataItems = lpNewDataItemArray;
    
	m_dwBatchDeleteCount = 0;
}

DWORD CDataFactory::GetItemDataTimeAndIssue(DWORD dwDataIndex, DWORD &dwIssue)
{
	ASSERT(!m_bIsCurData);	// 必须是初始数据
    
	LPDATAITEM lpDataItem = _GetDataItem(dwDataIndex);
	if(lpDataItem == NULL)
	{
		return 0;
	}
    
	dwIssue = lpDataItem->dwIssue;
	return lpDataItem->dwDataTime;
}

DWORD CDataFactory::GetItemIssue(DWORD dwDataIndex)
{
	LPDATAITEM lpDataItem = _GetDataItem(dwDataIndex);
	if(lpDataItem == NULL)
	{
		return 0;
	}
    
	return lpDataItem->dwIssue;
}

LPCSTR CDataFactory::GetItemIssueString(DWORD dwDataIndex)
{
	LPDATAITEM lpDataItem = _GetDataItem(dwDataIndex);
	if(lpDataItem == NULL)
	{
		return "";
	}
    
	sprintf(m_szTempIssueStringBuf, "%0*lu", g_pIData->GetIssueLen(), lpDataItem->dwIssue);
    
	CHECK_STRING_STACK(m_szTempIssueStringBuf);
    
	return m_szTempIssueStringBuf;
}

// 开奖日期相关
DWORD CDataFactory::GetItemDate(DWORD dwDataIndex)
{
	ASSERT(!m_bIsCurData);	// 必须是初始数据
	
	LPDATAITEM lpDataItem = _GetDataItem(dwDataIndex);
	if(lpDataItem == NULL)
	{
		return 0;
	}
	
	return lpDataItem->dwDataTime / 10000 + 20000000;
}

LPCSTR CDataFactory::GetItemDateString(DWORD dwDataIndex)
{
	ASSERT(!m_bIsCurData);	// 必须是初始数据
	
	LPDATAITEM lpDataItem = _GetDataItem(dwDataIndex);
	if(lpDataItem == NULL)
	{
		return "";
	}
	
	DWORD dwDateTime = lpDataItem->dwDataTime / 10000;
	sprintf(m_szTempDataStringBuf, "20%02lu-%02lu-%02lu", dwDateTime / 10000, (dwDateTime / 100) % 100, dwDateTime % 100);
    
	CHECK_STRING_STACK(m_szTempDataStringBuf);
    
	return m_szTempDataStringBuf;
}

DWORD CDataFactory::GetItemTime(DWORD dwDataIndex)
{
	ASSERT(!m_bIsCurData);	// 必须是初始数据
	
	LPDATAITEM lpDataItem = _GetDataItem(dwDataIndex);
	if(lpDataItem == NULL)
	{
		return 0;
	}
	
	return lpDataItem->dwDataTime % 10000;
}

LPCSTR CDataFactory::GetItemTimeString(DWORD dwDataIndex)
{
	ASSERT(!m_bIsCurData);	// 必须是初始数据
	
	LPDATAITEM lpDataItem = _GetDataItem(dwDataIndex);
	if(lpDataItem == NULL)
	{
		return "";
	}
	
	DWORD dwDateTime = lpDataItem->dwDataTime % 10000;
	sprintf(m_szTempTimeStringBuf, "%02lu:%02lu", dwDateTime / 100, dwDateTime % 100);
    
	CHECK_STRING_STACK(m_szTempTimeStringBuf);
	
	return m_szTempTimeStringBuf;
}

int CDataFactory::GetItemDayOfWeek(DWORD dwDataIndex)
{
	ASSERT(!m_bIsCurData);	// 必须是初始数据
	
	LPDATAITEM lpDataItem = _GetDataItem(dwDataIndex);
	if(lpDataItem == NULL)
	{
		return 0;
	}
	
	DWORD dwDateTime = lpDataItem->dwDataTime;
	
	CTime tmDateTime = CTime(2000 + dwDateTime / 100000000, (dwDateTime / 1000000) % 100, (dwDateTime / 10000) % 100, 0, 0, 0);
    
	return tmDateTime.GetDayOfWeek() - 1;
}

LPCSTR CDataFactory::GetItemDayOfWeekString(DWORD dwDataIndex)
{
	static char DAYOFWEEKSTRING[][8] = {"日", "一", "二", "三", "四", "五", "六"};
    
	return DAYOFWEEKSTRING[GetItemDayOfWeek(dwDataIndex)];
}

void CDataFactory::GetItemDateTime(DWORD dwDataIndex, CTime &tmDateTime)
{
	ASSERT(!m_bIsCurData);	// 必须是初始数据
    
	LPDATAITEM lpDataItem = _GetDataItem(dwDataIndex);
	if(lpDataItem == NULL)
	{
		return ;
	}
	
	DWORD dwDateTime = lpDataItem->dwDataTime;
    
	tmDateTime = CTime(2000 + dwDateTime / 100000000, (dwDateTime / 1000000) % 100, (dwDateTime / 10000) % 100, 
                       (dwDateTime / 100) % 100, dwDateTime % 100, 0);
}

LPCSTR CDataFactory::GetItemDateTimeString(DWORD dwDataIndex)
{
	ASSERT(!m_bIsCurData);	// 必须是初始数据
	
	LPDATAITEM lpDataItem = _GetDataItem(dwDataIndex);
	if(lpDataItem == NULL)
	{
		return "";
	}
	
	DWORD dwDate = lpDataItem->dwDataTime / 10000;
	DWORD dwTime = lpDataItem->dwDataTime % 10000;
    
	sprintf(m_szTempDataStringBuf, "20%02lu-%02lu-%02lu %02lu:%02lu", dwDate / 10000, (dwDate / 100) % 100, dwDate % 100, dwTime / 100, dwTime % 100);
	
	CHECK_STRING_STACK(m_szTempDataStringBuf);
	
	return m_szTempDataStringBuf;
}

DWORD CDataFactory::GetItemInitIndex(DWORD dwDataIndex)
{
	ASSERT(m_bIsCurData);	// 必须是当前数据
    
	LPDATAITEM lpDataItem = _GetDataItem(dwDataIndex);
	if(lpDataItem == NULL)
	{
		return 0;
	}
	
	return lpDataItem->dwInitIndex;
}

LPBYTE CDataFactory::GetItemNums(DWORD dwDataIndex)
{
	LPDATAITEM lpItem = _GetDataItem(dwDataIndex);
	if(lpItem == NULL)
	{
		return NULL;
	}
    
	if(m_bIsCurData && lpItem->btData[0] == MAXBYTE)
	{
		return NULL;	// 只有试机号的一期
	}
    
	return lpItem->btData;
}

LPCSTR CDataFactory::GetItemNumsString(DWORD dwDataIndex)
{
	LPBYTE lpNums = GetItemNums(dwDataIndex);
	if(lpNums == NULL)
	{
		return "";
	}
	
	m_pNumbers->SetItemData(0, lpNums);
	return m_pNumbers->GetItemString(0);
}

DWORD CDataFactory::GetItemNumsValue(DWORD dwDataIndex)
{
	LPBYTE lpNums = GetItemNums(dwDataIndex);
	if(lpNums == NULL)
	{
		return 0;
	}
	
	m_pNumbers->SetItemData(0, lpNums);
	return m_pNumbers->GetItemValue(0);
}

LPBYTE CDataFactory::GetItem2Nums(DWORD dwDataIndex)
{
	ASSERT(!m_bIsCurData);	// 必须是初始数据
    
	LPDATAITEM lpDataItem = _GetDataItem(dwDataIndex);
	if(lpDataItem == NULL)
	{
		return NULL;
	}
    
	if(lpDataItem->btData[0] == MAXBYTE)
	{
		return NULL;	// 只有试机号的一期
	}
    
	if(g_pIData->GetCurrentSectionIndex() == 0)
		return &lpDataItem->btData[g_pIData->GetNumberCount(DATA_SOURCE_INIT)];	// 返回第二区段
	else
		return lpDataItem->btData;			// 返回第一区段
}

LPCSTR CDataFactory::GetItemNums2String(DWORD dwDataIndex)
{
	ASSERT(!m_bIsCurData);	// 必须是初始数据
    
	LPDATAITEM lpDataItem = _GetDataItem(dwDataIndex);
	if(lpDataItem == NULL)
	{
		return "";
	}
	
	if(lpDataItem->btData[0] == MAXBYTE)
	{
		return "";	// 只有试机号的一期
	}
	
	m_pNumbers->SetItemData(0, lpDataItem->btData);
	LPCSTR lpszItemString = m_pNumbers->GetItemString(0);
    
	// 查询区段分隔符“+”所在位置
	int nPos = -1, nLen = strlen(lpszItemString);
	for(int i=0; i<nLen; i++)
	{
		if(lpszItemString[i] == '+')
		{
			nPos = i;
			break;
		}
	}
    
	if(nPos == -1)
	{
		return lpszItemString;	// 没有找到，则只有一个区
	}
    
	if(g_pIData->GetCurrentSectionIndex() == 0) // 返回第二区段文本串
	{
		// 去除前面的空格
		nPos ++;
		while(lpszItemString[nPos] == ' ')
			nPos ++;
        
		strcpy(m_szTempBuf, &lpszItemString[nPos]);
	}
	else	// 返回第一区段文本串
	{
		memcpy(m_szTempBuf, lpszItemString, nPos);
		m_szTempBuf[nPos] = 0;
		// 去除后面的空格
		nPos --;
		while(m_szTempBuf[nPos] == ' ')
		{
			m_szTempBuf[nPos] = 0;
			nPos --;
		}
	}
    
	return m_szTempBuf;
}

DWORD CDataFactory::GetItemNums2Value(DWORD /*dwDataIndex*/)
{
	ASSERT(FALSE);		// 目前不提供该功能
	return 0;
}

LPBYTE CDataFactory::GetItemTestNums(DWORD dwDataIndex)
{
	if(dwDataIndex == DATA_INDEX_INVALID)
		dwDataIndex = DATA_INDEX_ONLYTESTNUMS;
    
	LPDATAITEM lpDataItem = _GetDataItem(dwDataIndex);
	if(lpDataItem == NULL)
	{
		return NULL;
	}
	
	// 目前只有3D有试机号信息
	return &(lpDataItem->btData[5]);
}

LPCSTR CDataFactory::GetItemTestNumsString(DWORD dwDataIndex)
{
	if(dwDataIndex == DATA_INDEX_INVALID)
		dwDataIndex = DATA_INDEX_ONLYTESTNUMS;
	
	LPDATAITEM lpDataItem = _GetDataItem(dwDataIndex);
	if(lpDataItem == NULL)
	{
		return "";
	}
	
	// 目前只有3D有试机号信息
	m_pNumbers->SetItemData(0, &(lpDataItem->btData[5]));
	return m_pNumbers->GetItemString(0);
}

DWORD CDataFactory::GetItemTestNumsValue(DWORD dwDataIndex)
{
	if(dwDataIndex == DATA_INDEX_INVALID)
		dwDataIndex = DATA_INDEX_ONLYTESTNUMS;
	
	LPDATAITEM lpDataItem = _GetDataItem(dwDataIndex);
	if(lpDataItem == NULL)
	{
		return 0;
	}
	
	// 目前只有3D有试机号信息
	m_pNumbers->SetItemData(0, &(lpDataItem->btData[5]));
	return m_pNumbers->GetItemValue(0);
}

BOOL CDataFactory::IsTestNumsItem(DWORD dwDataIndex)
{
	if(dwDataIndex == DATA_INDEX_INVALID)
		dwDataIndex = DATA_INDEX_ONLYTESTNUMS;
	
	LPDATAITEM lpDataItem = _GetDataItem(dwDataIndex);
	if(lpDataItem == NULL)
	{
		return FALSE;
	}
	
	return (lpDataItem->dwFlag & DATAITEM_FLAG_TESTNUM) ? TRUE : FALSE;
}

BYTE CDataFactory::GetItemMachineIndex(DWORD dwDataIndex)
{
	LPDATAITEM lpDataItem = _GetDataItem(dwDataIndex);
	if(lpDataItem == NULL)
	{
		return 0;
	}
	
	// 目前只有3D有机器信息
	return lpDataItem->btData[3];
}

BYTE CDataFactory::GetItemBallIndex(DWORD dwDataIndex)
{
	LPDATAITEM lpDataItem = _GetDataItem(dwDataIndex);
	if(lpDataItem == NULL)
	{
		return 0;
	}
	
	// 目前只有3D有球信息
	return lpDataItem->btData[4];
}

void CDataFactory::SetForecastNums(LPBYTE lpData)
{
	if(lpData == NULL)	// 清除预测行号码
	{
		ZeroMemory(&m_stForcastItem, sizeof(m_stForcastItem));
	}
	else	// 设置预测行号码
	{
		memcpy(m_stForcastItem.btData, lpData, MAX_NUMBER_COUNT);
		
		LPDATAITEM lpItem = _GetDataItem(DATA_INDEX_ONLYTESTNUMS);
		if(lpItem != NULL)
		{
			// 设置当期试机号数据，目前只有3D有
			memcpy(&m_stForcastItem.btData[3], &lpItem->btData[3], 5);
		}
		m_stForcastItem.dwIssue = 1;	// 只需不为0即可
	}
}

LPBYTE CDataFactory::GetForecastNums()
{
	if(m_stForcastItem.dwIssue == 0)
	{
		return NULL;
	}
    
	return m_stForcastItem.btData;
}

DWORD CDataFactory::GetNumsValue(LPBYTE lpNums)
{
	if(lpNums == NULL)
	{
		return 0;
	}
	
	m_pNumbers->SetItemData(0, lpNums);
	return m_pNumbers->GetItemValue(0);	
}

///////////////////////////////////////////////////////////////////////////////////
void CDataFactory::TransDataBySection(DWORD dwDataIndex, BYTE btBeginIndex, BYTE btCount)
{
	ASSERT(btBeginIndex > 0 && (btBeginIndex + btCount) <= MAX_NUMBER_COUNT);
    
	BYTE btTemp[MAX_NUMBER_COUNT] = {0};
	LPBYTE lpData = _GetAt(dwDataIndex)->btData;
	memcpy(btTemp, lpData, MAX_NUMBER_COUNT);
	
	memcpy(lpData, &btTemp[btBeginIndex], btCount);
	memcpy(&lpData[btCount], btTemp, MAX_NUMBER_COUNT - btCount);
}

void CDataFactory::TransDataByPosItem(DWORD dwDataIndex, LPLOTTERYPOSITEM lpPosItem, BOOL bForAM)
{
	ASSERT(lpPosItem != NULL);
    
	LPDATAITEM lpDataItem = _GetAt(dwDataIndex);
	if(lpDataItem == NULL)
	{
		return ;
	}
    
	BYTE i = 0, btOldData[MAX_NUMBER_COUNT] = {0};
	BYTE btNumberCount = g_pIData->GetNumberCount(bForAM ? DATA_SOURCE_ORG : DATA_SOURCE_INIT);
    
	memcpy(btOldData, lpDataItem->btData, MAX_NUMBER_COUNT);
	
	// 前处理
	_FormatDataByFlag(lpPosItem->btFlagPrev, btOldData, btNumberCount);
	// 位置转换
	for(i=0; i<lpPosItem->btCount; i++)
	{
		lpDataItem->btData[i] = btOldData[lpPosItem->btPosArray[i]];
	}
	// 后处理
	_FormatDataByFlag(lpPosItem->btFlagNext, lpDataItem->btData, lpPosItem->btCount);
    
	if(g_pIData->GetLotteryProperty()->btProperty & CSTLPP_TESTNUMS)	// 试机号处理
	{
		// 前处理
		_FormatDataByFlag(lpPosItem->btFlagPrev, &btOldData[5], btNumberCount);
		// 位置转换
		for(i=0; i<lpPosItem->btCount; i++)
		{
			lpDataItem->btData[i + 5] = btOldData[lpPosItem->btPosArray[i] + 5];
		}
		// 后处理
		_FormatDataByFlag(lpPosItem->btFlagNext, &lpDataItem->btData[5], lpPosItem->btCount);
	}
}

///////////////////////////////////////////////////////////////////////////////////
void CDataFactory::RandItemData(DWORD dwDataIndex)
{
	int nNumberCount = g_pIData->GetNumberCount();
	int nMinNumber = g_pIData->GetMinNumber();
	int nMaxNumber = g_pIData->GetMaxNumber();
	LPBYTE lpItemData = _GetAt(dwDataIndex)->btData;
    
	for(int i=nNumberCount-1; i>=0; i--)
	{
		lpItemData[i] = BYTE(rand() % (nMaxNumber - nMinNumber) + nMinNumber);
	}
}

///////////////////////////////////////////////////////////////////////////////////
CDataFactory::LPDATAITEM CDataFactory::_GetAt(DWORD dwDataIndex)
{
	ASSERT(dwDataIndex >= 0 && dwDataIndex < m_dwCurUsedDataItemCount);
    
	if(dwDataIndex < m_dwCountOfDataItems)
	{
		return &m_lpDataItems[dwDataIndex];
	}
	else
	{
		int nNewIndex = dwDataIndex - m_dwCountOfDataItems;
		ASSERT(nNewIndex >= 0 && nNewIndex < m_cExtDataItemArray.GetSize());
		
		return (LPDATAITEM)m_cExtDataItemArray.GetAt(nNewIndex);
	}
}

CDataFactory::LPDATAITEM CDataFactory::_GetDataItem(DWORD dwDataIndex)
{
	switch(dwDataIndex)
	{
        case DATA_INDEX_INVALID: ASSERT(FALSE); break;
        case DATA_INDEX_FORECAST:
		{
			if(m_stForcastItem.dwIssue == 0)
			{
				return NULL;
			}
			else
			{
				return &m_stForcastItem;
			}
		} break;
        case DATA_INDEX_ONLYTESTNUMS:
		{
			if(_GetAt(m_dwCurUsedDataItemCount-1)->btData[0] != MAXBYTE)
			{
				return NULL;	// 都有开奖号
			}
			return _GetAt(m_dwCurUsedDataItemCount - 1);
		} break;
        case DATA_INDEX_LAST: dwDataIndex = GetCount(TRUE) - 1; break;
	}
	
	if(dwDataIndex >= GetCount(FALSE))
	{
		return m_stForcastItem.dwIssue == 0 ? NULL : &m_stForcastItem;
	}
    
	LPDATAITEM lpDataItem = _GetAt(dwDataIndex);
    
	if((lpDataItem != NULL) && (lpDataItem->btData[0] == MAXBYTE))
	{
		return m_stForcastItem.dwIssue == 0 ? lpDataItem : &m_stForcastItem;
	}
    
	return lpDataItem;
}

CDataFactory::LPDATAITEM CDataFactory::_AddNew()
{
	if(m_dwCountOfDataItems == 0 || m_lpDataItems == NULL)
	{
		// 如果当前数据为空，则初始化一个新的。
		SetCount(1000);
		m_dwCurUsedDataItemCount = 1;
		return &m_lpDataItems[0];
	}
	
	// 当前数据不为空，在后面添加
	m_dwCurUsedDataItemCount ++;
	
	if(m_dwCurUsedDataItemCount <= m_dwCountOfDataItems)
	{
		return &m_lpDataItems[m_dwCurUsedDataItemCount - 1];
	}
	else
	{
		LPDATAITEM pNewDataItem = new DATAITEM;
		if(pNewDataItem == NULL)
		{
			m_dwCurUsedDataItemCount --;
			ASSERT(FALSE);
			return NULL;
		}
		
		m_cExtDataItemArray.Add((DWORD)pNewDataItem);
		
		return pNewDataItem;
	}
}

void CDataFactory::_FormatDataByFlag(BYTE btFlag, LPBYTE lpNums, BYTE btNumCount)
{
    // TEST
/*	BYTE i = 0;
    
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
	}
 */
}

void CDataFactory::_OrderNumsByZ3Type(LPBYTE lpNumsData, BYTE btOrderType, int nBeginIndex)
{
	ASSERT(lpNumsData != NULL);
	BYTE btTemp = 0;
	
	switch(btOrderType)
	{
        case DATA_ORDERTYPE_ASC_AAB:		// AAB
            if(lpNumsData[nBeginIndex+1] == lpNumsData[nBeginIndex+2])	// BAA
            {
                btTemp = lpNumsData[nBeginIndex];
                lpNumsData[nBeginIndex+0] = lpNumsData[nBeginIndex+2];
                lpNumsData[nBeginIndex+2] = btTemp;
            }
            break;
        case DATA_ORDERTYPE_ASC_ABA:		// ABA
            if(lpNumsData[nBeginIndex] == lpNumsData[nBeginIndex+1])  // AAB
            {
                btTemp = lpNumsData[nBeginIndex+1];
                lpNumsData[nBeginIndex+1] = lpNumsData[nBeginIndex+2];
                lpNumsData[nBeginIndex+2] = btTemp;
            } 
            else if(lpNumsData[nBeginIndex+1] == lpNumsData[nBeginIndex+2])	// BAA
            {
                btTemp = lpNumsData[nBeginIndex+1];
                lpNumsData[nBeginIndex+1] = lpNumsData[nBeginIndex];
                lpNumsData[nBeginIndex] = btTemp;
            }
            break;
        case DATA_ORDERTYPE_ASC_BAA:		// BAA
            if(lpNumsData[nBeginIndex] == lpNumsData[nBeginIndex+1])	// AAB
            {
                btTemp = lpNumsData[nBeginIndex];
                lpNumsData[nBeginIndex] = lpNumsData[nBeginIndex+2];
                lpNumsData[nBeginIndex+2] = btTemp;
            }
            break;
        default:	break;
	}
}