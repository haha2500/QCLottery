//
//  Data_Private.cpp
//  Graph
//
//  Created by 武 孙 on 12-5-10.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "Data.h"
#include "Def_ConditionValue.h"

#define		BEGIN_SET_LOTTERYPRIZEINFO() \
            { \
                BYTE btPrizeItemIndex = 0; \
                LPLOTTERYPRIZEINFO lpPrizeInfo = &m_stLotteryProperty.stPrizeInfo; \
                lpPrizeInfo->btPrizeGroupCount = 0; \
                LPLOTTERYPRIZEGROUP lpPrizeGroup = NULL; \
                LPLOTTERYPRIZEITEM lpPrizeItem = NULL;

#define		BEGIN_ADD_LOTTERYPRIZEGROUP(szGroupNameIn, btNumberCountIn, btSectionIndexIn, btTypeIn) \
                ASSERT(btSectionIndexIn >= 0 && btSectionIndexIn < 2); \
                ASSERT(lpPrizeInfo->btPrizeGroupCount < (sizeof(lpPrizeInfo->stPrizeGroupArray) / sizeof(LOTTERYPRIZEGROUP))); \
                lpPrizeGroup = &lpPrizeInfo->stPrizeGroupArray[lpPrizeInfo->btPrizeGroupCount]; \
                strcpy(lpPrizeGroup->szName, szGroupNameIn); \
                lpPrizeGroup->btSectionIndex = btSectionIndexIn; \
                lpPrizeGroup->btNumberCount = btNumberCountIn; \
                lpPrizeGroup->btPrizeItemBeginIndex = btPrizeItemIndex; \
                lpPrizeGroup->btType = btTypeIn; \
                lpPrizeGroup->btPrizeItemCount = 0;

#define		ADD_PRIZEITEM(btLevelIn, dwCompareDataIn, dwMoneyIn, szNameIn, dwTypeDataIn) \
                ASSERT(btPrizeItemIndex < (sizeof(lpPrizeInfo->stPrizeItemArray) / sizeof(LOTTERYPRIZEITEM))); \
                lpPrizeItem = &lpPrizeInfo->stPrizeItemArray[btPrizeItemIndex]; \
                lpPrizeItem->btLevel = btLevelIn; \
                lpPrizeItem->dwCompareData = dwCompareDataIn; \
                lpPrizeItem->dwMoney = dwMoneyIn; \
                strcpy(lpPrizeItem->szName, szNameIn); \
                lpPrizeItem->dwTypeData = dwTypeDataIn; \
                btPrizeItemIndex ++;

#define		ADD_PRIZEGROUP_DETAILS(szDetailsPart) \
                strcat(lpPrizeGroup->szDetails, szDetailsPart); \
                strcat(lpPrizeGroup->szDetails, "\r\n"); \
                ASSERT(strlen(lpPrizeGroup->szDetails) < sizeof(lpPrizeGroup->szDetails) - 1);

#define		END_ADD_LOTTERYPRIZEGROUP() \
                lpPrizeGroup->btPrizeItemCount = (BYTE)(btPrizeItemIndex - lpPrizeGroup->btPrizeItemBeginIndex); \
                lpPrizeInfo->btPrizeGroupCount ++;

#define		END_SET_LOTTERYPRIZEINFO() \
            }

#define		DATA_TYPE_COMPLEX_SZ		DATA_TYPE_NUM_SZ | DATA_TYPE_REPEATABLE
#define		DATA_TYPE_COMPLEX_LT		DATA_TYPE_NUM_LT | DATA_TYPE_ORDERED
#define		DATA_TYPE_COMPLEX_POSLT		DATA_TYPE_NUM_LT

////////////////////////////////////////////////////////////////////////////////
void CData::_InitLotteryProperty()
{
    ZeroMemory(&m_stLotteryProperty, sizeof(m_stLotteryProperty));
	m_stLotteryProperty.btOpenFlag = CSTLPOF_ALL;
	m_stLotteryProperty.btProperty = CSTLPP_TESTNUMS;
	m_stLotteryProperty.dwLotteryID = LOTTERYID_FC3D;
	strcpy(m_stLotteryProperty.szLotteryName, "福彩3D");
	m_stLotteryProperty.btIssueType = ISSUE_TYPE_YYYYXXX;
	m_stLotteryProperty.btSectionCount = 1;
	m_stLotteryProperty.stSectionInfo[0].btPosIndex = 0;
	m_stLotteryProperty.stSectionInfo[0].btMinNumber = 0;
	m_stLotteryProperty.stSectionInfo[0].btMaxNumber = 9;
	m_stLotteryProperty.stSectionInfo[0].btNumberCount = 3;
	m_stLotteryProperty.stSectionInfo[0].btDataType = DATA_TYPE_COMPLEX_SZ;
	strcpy(m_stLotteryProperty.stSectionInfo[0].szName, "开奖号码");
	m_stLotteryProperty.stSectionInfo[0].btFlag = CSTLTF_NEED;
	
	m_stLotteryProperty.stDownloadDataInfo.btEnableUDP = 1;
	strcpy(m_stLotteryProperty.stDownloadDataInfo.szURL, "http://software.pinble.com/CstData2010/Debug/CstData_SZ.asp");
	m_stLotteryProperty.stDownloadDataInfo.dwLastDate = 0;
	m_stLotteryProperty.stDownloadDataInfo.dwLastTime = 0;
    
	BEGIN_SET_LOTTERYPRIZEINFO()
        BEGIN_ADD_LOTTERYPRIZEGROUP("单选", 3, 0, LPIT_SINGLE)
            ADD_PRIZEITEM(1, 3000, 1000, "单选", 0)
            ADD_PRIZEGROUP_DETAILS("单选：中奖金额为每注1000元。")
        END_ADD_LOTTERYPRIZEGROUP()
        BEGIN_ADD_LOTTERYPRIZEGROUP("组选", 3, 0, LPIT_GROUP_MORETYPE)
            ADD_PRIZEITEM(1, 3000, 160, "组选6", 111)
            ADD_PRIZEITEM(2, 3000, 320, "组选3", 12)
            ADD_PRIZEITEM(3, 3000, 1000, "单选", 3)
            ADD_PRIZEGROUP_DETAILS("单选：中奖金额为每注1000元。")
            ADD_PRIZEGROUP_DETAILS("组选3：中奖金额为每注320元。")
            ADD_PRIZEGROUP_DETAILS("组选6：中奖金额为每注160元。")
        END_ADD_LOTTERYPRIZEGROUP()
    
        BEGIN_ADD_LOTTERYPRIZEGROUP("单选", 2, 0, LPIT_SINGLE)
            ADD_PRIZEITEM(1, 2000, 98, "单选", 0)
            ADD_PRIZEGROUP_DETAILS("单选：中奖金额为每注98元。")
        END_ADD_LOTTERYPRIZEGROUP()
	END_SET_LOTTERYPRIZEINFO()
}

BYTE CData::_GetValueFromSpecialChar(char caSpecialChar)
{
	if(caSpecialChar >= '0' && caSpecialChar <= '9')
		return BYTE(caSpecialChar - '0');
	
	if(caSpecialChar >= 'A' && caSpecialChar <= 'Z')
		return BYTE(caSpecialChar - 'A' + 10);
	
	if(caSpecialChar >= 'a' && caSpecialChar <= 'z')
		return BYTE(caSpecialChar - 'a' + 36);
	
	return 0;
}

DWORD CData::_GetValueFromString(LPCSTR lpszString)
{
	DWORD dwDateTime = 0;
	
	for(int i=0; i<5; i++)
	{
		dwDateTime *= 100;
		dwDateTime += _GetValueFromSpecialChar(lpszString[i]);
	}
	
	return dwDateTime;
}

BOOL CData::_GetDataFromString(LPCSTR lpszData, BYTE btMaxNumber, LPBYTE lpData)
{
	// 字符串转换为数据
	ZeroMemory(lpData, MAX_NUMBER_COUNT);
	
	for(int i=0; i<MAX_NUMBER_COUNT; i++)
	{
		if(lpszData[i] == ' ')
		{
			lpData[i] = MAXBYTE;
		}
		else if(lpszData[i] <= '9')
		{
			lpData[i] = BYTE(lpszData[i] - '0');
		}
		else if(lpszData[i] >= 'A' && lpszData[i] <= 'Z')
		{
			lpData[i] = BYTE(lpszData[i] - 'A' + 10);
		}
		else
		{
			lpData[i] = BYTE(lpszData[i] - 'a' + 36);
		}
		
		if(lpData[i] > btMaxNumber)
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

DWORD CData::_GetValueFromIssueString(LPCSTR lpszString)
{
	ASSERT(lpszString != NULL);
	
	// 原串是数值按78进制进行转换，转换后为反向的，不足5位后补0
	DWORD dwRtn = 0, dwRatio = 1;
	
	for(int i=0; i<5; i++)
	{
		dwRtn += (lpszString[i] - '0') * dwRatio;
		dwRatio *= 78;
	}
	
	return dwRtn;
}

void CData::_RemoveAll()
{
	_ClearCurUsedData();	
}

void CData::_PreResetCurUsedData(DWORD dwFlag)
{
    // TEST
    /*
	if(dwFlag & RESETDATABY_ISSUERANGE)		// 因号码范围改变
	{
		g_pIConfig->GetConfig(CFGID_DATA_RANGE, (LPBYTE)&m_stDataRange, sizeof(m_stDataRange));
	}
    
	if(dwFlag & RESETDATABY_DIV)			// 因数据间隔改变
	{
		g_pIConfig->GetConfig(CFGID_DATA_DIV, (LPBYTE)&m_stDataDiv, sizeof(m_stDataDiv));
	}
    
	if(dwFlag & (RESETDATABY_MACHINE | RESETDATABY_BALL))		// 因机球号改变
	{
		int nMachineBall = g_pIConfig->GetConfig(CFGID_DATA_MACHINEBALL, NULL, 0);
		m_nMachineIndex = nMachineBall / 10;
		m_nBallIndex = nMachineBall % 10;
	}
    
	if(dwFlag & RESETDATABY_TESTNUMS)		// 因试机号设置改变
	{
		m_nTestNumsFlag = g_pIConfig->GetConfig(CFGID_DATA_TESTNUMSFLAG, NULL, 0);
	}
	*/
	if(dwFlag & RESETDATABY_DATARWL)			// 因热温冷设置改变
	{
		DELETE_BUFFER(m_lpRWLData);
	}
}

void CData::_ClearCurUsedData(BOOL bInludeNums)
{
	if(bInludeNums)
	{
		m_cCurDataFactory.Destroy();
	}
    
	DELETE_BUFFER(m_lpCTXData);
	DELETE_BUFFER(m_lpLGCData);
	DELETE_BUFFER(m_lpFGZData);
	DELETE_BUFFER(m_lpRWLData);
	DELETE_BUFFER(m_lpGroupSkipValueData);
	DELETE_BUFFER(m_lpGroupSkipValueData);
	DELETE_BUFFER(m_lpSingleSkipValueData);	
	DELETE_BUFFER(m_lpSingleSkipValueData);	
}

void CData::_InitDataToCurData()
{
	int nCount = m_cCurDataFactory.GetCount(FALSE);
    
	// 区段数据转换
	if(m_btSectionIndex > 0)	// 当前为第二区段
	{
		for(int i=0; i<nCount; i++)
		{
			m_cCurDataFactory.TransDataBySection(i, m_stLotteryProperty.stSectionInfo[m_btSectionIndex].btPosIndex, m_stLotteryProperty.stSectionInfo[m_btSectionIndex].btNumberCount);
		}
	}
	else	// 当前为第一区段
	{
		if(m_stLotteryProperty.stSectionInfo[1].btFlag & CSTLTF_JOIN)	// 特码作为第一区段的，需要排序处理
		{
			for(int i=m_cCurDataFactory.GetCount(FALSE)-1; i>=0; i--)
			{
				// 号码顺序排列处理
				m_cCurDataFactory.OrderItemData(i, DATA_ORDERTYPE_ASC, FALSE);
			}
		}
	}
    
	// 玩法数据转换
	if(!IsOrgPlayType())
	{
		for(int i=0; i<nCount; i++)
		{
			m_cCurDataFactory.TransDataByPosItem(i, &m_stPTItem, FALSE);
		}
	}
    
	// 分析方式数据转换
	if(!IsOrgAnalysisMode())
	{
		for(int i=0; i<nCount; i++)
		{
			m_cCurDataFactory.TransDataByPosItem(i, &m_stAMItem, TRUE);
		}
	}
}

void CData::_TreatDataRange()
{
	// 期数范围处理
	int i, nTemp = 0, nBeginIndex = 0, nEndIndex = m_cCurDataFactory.GetCount(FALSE) - 1;
	DWORD dwInitIndex = 0;
    
	switch(m_stDataRange.btRangeType)
	{
        case 0:				// 全部开奖号
            break;
            
        case 1:				// 最近一年的号码
            dwInitIndex = m_cCurDataFactory.GetItemInitIndex(nEndIndex);
            nTemp = m_pInitDataFactory->GetItemDate(dwInitIndex) / 10000;
            for(i=nEndIndex-1; i>=0; i--)
            {
                dwInitIndex = m_cCurDataFactory.GetItemInitIndex(i);
                if(int(m_pInitDataFactory->GetItemDate(dwInitIndex) / 10000) != nTemp)
                {
                    break;
                }
            }
            nBeginIndex = i + 1;
            
            if(m_stLotteryProperty.btOpenTimesPerDay > 1 && GetIssueType() != ISSUE_TYPE_YYXXXXXX)
            {
                // 特殊的高频彩需要排除上一天延续下来的期号
                int nDayIssueLen = GetIssueLen(TRUE), nModValue = (int)pow(10, nDayIssueLen);
                for(i=nBeginIndex; i <= nEndIndex; i++)
                {
                    dwInitIndex = m_cCurDataFactory.GetItemInitIndex(i);
                    if(m_pInitDataFactory->GetItemIssue(dwInitIndex) % nModValue == 1)		// 当天第一期
                    {
                        break;
                    }			
                }
            }
            break;
        case 2:				// 最近一天的号码
		{
			ASSERT(m_stLotteryProperty.btOpenTimesPerDay > 1);		// 只用于高频彩
			dwInitIndex = m_cCurDataFactory.GetItemInitIndex(nEndIndex);
			if(GetIssueType() == ISSUE_TYPE_YYXXXXXX)
			{
				nTemp = m_pInitDataFactory->GetItemDate(dwInitIndex);
				for(i=nEndIndex-1; i>=0; i--)
				{
					dwInitIndex = m_cCurDataFactory.GetItemInitIndex(i);
					if((int)m_pInitDataFactory->GetItemDate(dwInitIndex) != nTemp)
					{
						nBeginIndex = i + 1;
						break;
					}
				}
			}
			else	// 每天期号从1开始
			{
				int nDayIssueLen = GetIssueLen(TRUE), nModValue = (int)pow(10, nDayIssueLen);
				for(i=nEndIndex; i>=0; i--)
				{
					dwInitIndex = m_cCurDataFactory.GetItemInitIndex(i);
					if(m_pInitDataFactory->GetItemIssue(dwInitIndex) % nModValue == 1)		// 当天第一期
					{
						nBeginIndex = i;
						break;
					}
				}
			}
		} break; 
        case 3:
            if(m_stDataRange.nBeginIssue < 0)	// 起始序号到终止序号（暂时不用）
            {
                nBeginIndex = 0 - m_stDataRange.nBeginIssue;
                nEndIndex = m_stDataRange.nEndIssue;
                int nCount = m_cCurDataFactory.GetCount(FALSE);
                if(nBeginIndex < 0 || nBeginIndex >= nCount) nBeginIndex = 0;
                if(nEndIndex < 0 || nEndIndex >= nCount || nEndIndex < nBeginIndex) nEndIndex = nCount - 1;
            }
            else				// 起始期号到终止期号
            {
                if(m_stDataRange.nEndIssue > 0)
                {
                    for(i=m_cCurDataFactory.GetCount(FALSE)-1; i>=0; i--)
                    {
                        dwInitIndex = m_cCurDataFactory.GetItemInitIndex(i);
                        if(m_pInitDataFactory->GetItemIssue(dwInitIndex) <= (DWORD)m_stDataRange.nEndIssue)
                        {
                            nEndIndex = i;
                            break;
                        }
                    }
                }
                if(m_stDataRange.nFromEndCount <= 0)
                {
                    // 起始期号到终止期号，则需要查找起始期号索引
                    for(i=nEndIndex; i>=0; i--)
                    {
                        dwInitIndex = m_cCurDataFactory.GetItemInitIndex(i);
                        if(m_pInitDataFactory->GetItemIssue(dwInitIndex) <= (DWORD)m_stDataRange.nBeginIssue)
                        {
                            nBeginIndex = i;
                            if(m_pInitDataFactory->GetItemIssue(dwInitIndex) < (DWORD)m_stDataRange.nBeginIssue)
                            {
                                // 没有找到匹配的，但是已经超过起始期号了，则使用上一个索引
                                nBeginIndex ++;
                            }
                            break;
                        }
                    }
                }
                else
                {
                    // 终止期号前N期
                    nBeginIndex = nEndIndex - m_stDataRange.nFromEndCount + 1;
                    if(GetItemTestNums(DATA_INDEX_ONLYTESTNUMS) != NULL)
                    {
                        nBeginIndex --;
                    }
                    if(nBeginIndex < 0)
                    {
                        nBeginIndex = 0;
                    }
                }
            }
            break;
        case 4:				// 保留最近n期
            nBeginIndex = nEndIndex - m_stDataRange.nKeepLastIssueCount + 1;\
            if(GetItemTestNums(DATA_INDEX_ONLYTESTNUMS) != NULL)
            {
                nBeginIndex --;
            }
            break;
        default:
            ASSERT(FALSE);
            return ;
            break;
	}
    
	if(m_stDataRange.btRangeType != 3 && m_stDataRange.nReverseLastIssueCount > 0)
	{
		// 排除最近n期
		nEndIndex -= m_stDataRange.nReverseLastIssueCount;
		if(GetItemTestNums(DATA_INDEX_ONLYTESTNUMS) != NULL)
		{
			nEndIndex --;
		}
	}
	if(nEndIndex < nBeginIndex)
		nEndIndex = nBeginIndex - 1;
    
	// 批量删除不在指定范围内的数据
	m_cCurDataFactory.BeginBatchDelete();
	for(i=0; i<nBeginIndex; i++)
		m_cCurDataFactory.BatchDelete(i);
	for(i=m_cCurDataFactory.GetCount(FALSE)-1; i>nEndIndex; i--)
		m_cCurDataFactory.BatchDelete(i);
	m_cCurDataFactory.EndBatchDelete();
}

void CData::_TreatMachineBall()
{
	int nIndex = 0, nCount = m_pInitDataFactory->GetCount(FALSE);
    
	m_cCurDataFactory.SetCount(nCount);
    
	if(!(m_stLotteryProperty.btProperty & (CSTLPP_MACHINE | CSTLPP_BALL)))
	{
		// 没有机球信息
		for(int i=0; i<nCount; i++)
		{
			m_cCurDataFactory.CopyItem(i, i, m_pInitDataFactory);
		}
		return ;
	}
    
	// 处理机球匹配
	for(int i=0; i<nCount; i++)
	{
		if(m_stLotteryProperty.btProperty & CSTLPP_MACHINE)
		{
			if(m_nMachineIndex != 0)
			{
				if(m_pInitDataFactory->GetItemMachineIndex(i) != m_nMachineIndex)
				{
					continue;	// 机器号不匹配
				}
			}
		}
		if(m_stLotteryProperty.btProperty & CSTLPP_BALL)
		{
			if(m_nBallIndex != 0)
			{
				if(m_pInitDataFactory->GetItemBallIndex(i) != m_nBallIndex)
				{
					continue;	// 球号不匹配
				}
			}
		}
        
		m_cCurDataFactory.CopyItem(nIndex, i, m_pInitDataFactory);
		nIndex ++;
	}
    
	m_cCurDataFactory.SetCount(nIndex);
}

void CData::_TreatDataDiv()
{
	int i = 0, nCount = m_cCurDataFactory.GetCount(FALSE);
	int nDivCount = m_stDataDiv.nDivCount > nCount ? nCount : m_stDataDiv.nDivCount;
	DWORD dwInitIndex = 0;
    
	m_cCurDataFactory.BeginBatchDelete();
    
	if(m_stDataDiv.btDivType == 1)	// 隔期
	{
		if(GetItemNums(nCount - 1) == NULL)		// 最后一期只有试机号
		{
			nCount --;
			if(m_stDataDiv.btDivFrom == 1)		// 从最后一期开始间隔
			{
				m_cCurDataFactory.BatchDelete(nCount);
			}
		}
		for(i=1; i<=nDivCount; i++)
		{
			m_cCurDataFactory.BatchDelete(nCount-i-m_stDataDiv.btDivFrom);
		}
		nCount -= nDivCount;
		int nLastIndex = nCount - 1 - m_stDataDiv.btDivFrom;
		for(i=nLastIndex; i>=0; i--)
		{
			if((nLastIndex-i) % (nDivCount + 1) != 0)
			{
				m_cCurDataFactory.BatchDelete(i);
			}
		}
	}
	else if(m_stDataDiv.btDivType == 2)	// 按期号
	{
		if(m_stDataDiv.btIssueType == 0)	// 期号尾数
		{
			for(i=nCount-1; i>=0; i--)
			{
				dwInitIndex = m_cCurDataFactory.GetItemInitIndex(i);
				if(m_stDataDiv.btIssueTail[m_pInitDataFactory->GetItemIssue(dwInitIndex) % 10] == 0)
				{
					m_cCurDataFactory.BatchDelete(i);
				}
			}
		}
		else								// 期号余数
		{
			int nModValue = (int)pow(10, GetIssueLen(TRUE));
			for(i=nCount-1; i>=0; i--)
			{
				dwInitIndex = m_cCurDataFactory.GetItemInitIndex(i);
				if((m_pInitDataFactory->GetItemIssue(dwInitIndex) % nModValue) % m_stDataDiv.dwIssueRem[0] != m_stDataDiv.dwIssueRem[1])
				{
					m_cCurDataFactory.BatchDelete(i);
				}
			}
		}
	}
	else if(m_stDataDiv.btDivType == 3)	// 按日期
	{
        // TEST
/*		CTime tmDate = NULL;
		switch(m_stDataDiv.btDateType)
		{
            case 0:	// 按周
                for(i=nCount-1; i>=0; i--)
                {
                    dwInitIndex = m_cCurDataFactory.GetItemInitIndex(i);				
                    if(m_stDataDiv.btWeekDays[m_pInitDataFactory->GetItemDayOfWeek(dwInitIndex)] == 0)
                    {
                        m_cCurDataFactory.BatchDelete(i);
                    }
                }
                break;
            case 1: // 按月
                for(i=nCount-1; i>=0; i--)
                {
                    dwInitIndex = m_cCurDataFactory.GetItemInitIndex(i);	
                    m_pInitDataFactory->GetItemDateTime(dwInitIndex, tmDate);
                    if(m_stDataDiv.btMonthDays[tmDate.GetDay()-1] == 0)
                    {
                        m_cCurDataFactory.BatchDelete(i);
                    }
                }
                break;
            case 2: // 按天
			{
				// 得到时间
				if(m_stDataDiv.btTimesIndex >= m_stLotteryProperty.btOpenTimesPerDay)
					m_stDataDiv.btTimesIndex = (BYTE)(m_stLotteryProperty.btOpenTimesPerDay - 1);
				DWORD dwTime = m_stLotteryProperty.wOpenTimeArray[m_stDataDiv.btTimesIndex];
				for(i=nCount-1; i>=0; i--)
				{
					dwInitIndex = m_cCurDataFactory.GetItemInitIndex(i);	
					if(dwTime != m_pInitDataFactory->GetItemTime(dwInitIndex))
					{
						m_cCurDataFactory.BatchDelete(i);
					}
				}
			} break;
            case 3: // 每天间隔
			{
				LPBYTE lpValidFlag = new BYTE[2400];
				ASSERT(lpValidFlag != NULL);
				ZeroMemory(lpValidFlag, 2400);
                
				CTime tmBegin(2000, 1, 1, m_stDataDiv.wDayDivBeginTime / 100, m_stDataDiv.wDayDivBeginTime % 100, 0);
				WORD wLastTime = m_stLotteryProperty.wOpenTimeArray[m_stLotteryProperty.btOpenTimesPerDay - 1];
				CTime tmEnd(2000, 1, 1, wLastTime / 100, wLastTime % 100, 0);
				CTimeSpan ts(0, 0, m_stDataDiv.wDayDivMinutes, 0);
				while(tmBegin <= tmEnd)
				{
					ASSERT(tmBegin.GetHour() * 100 + tmBegin.GetMinute() < 2400);
					lpValidFlag[tmBegin.GetHour() * 100 + tmBegin.GetMinute()] = 1;
					tmBegin += ts;
				}
				for(i=nCount-1; i>=0; i--)
				{
					dwInitIndex = m_cCurDataFactory.GetItemInitIndex(i);	
					if(lpValidFlag[m_pInitDataFactory->GetItemTime(dwInitIndex)] != 1)
					{
						m_cCurDataFactory.BatchDelete(i);
					}
				}
				delete []lpValidFlag;
			} break;
            default: ASSERT(FALSE); break;
		}*/
	}
    
	m_cCurDataFactory.EndBatchDelete();
}

void CData::_TreatTestNums()
{
	if(m_nTestNumsFlag == TESTNUMSFLAG_JOIN)		// 将试机号视为开奖号
	{
		int nCount = m_cCurDataFactory.GetCount(TRUE);
		int nNewCount = nCount * 2;
		BOOL bHasOnlyTestNums = FALSE;
		if(m_cCurDataFactory.GetItemTestNums(DATA_INDEX_ONLYTESTNUMS) != NULL)
		{
			nNewCount ++;
			bHasOnlyTestNums = TRUE;
		}
        
		m_cCurDataFactory.SetCount(nNewCount);
		DWORD dwInitIndex = 0, dwIndex = nNewCount - 1;
        
		if(bHasOnlyTestNums)
		{
			if(m_cCurDataFactory.CopyItem(dwIndex, m_pInitDataFactory->GetCount(FALSE)-1, m_pInitDataFactory))
			{
				// 只有试机号的一期的试机号变的开奖号
				m_cCurDataFactory.ModifyItemData(dwIndex--, m_nTestNumsFlag);
			}
		}
		for(int i=nCount-1; i>=0; i--)
		{
			dwInitIndex = m_cCurDataFactory.GetItemInitIndex(i);
			m_cCurDataFactory.CopyItem(dwIndex--, dwInitIndex, m_pInitDataFactory);	// 原开奖号
            
			m_cCurDataFactory.CopyItem(dwIndex, dwInitIndex, m_pInitDataFactory);	// 试机号变的开奖号
			m_cCurDataFactory.ModifyItemData(dwIndex--, m_nTestNumsFlag);
		}
	}
	else if(m_nTestNumsFlag == TESTNUMSFLAG_SWAP)	// 试机号与开奖号互换
	{
		int nCount = m_cCurDataFactory.GetCount(FALSE);
		for(int i=0; i<nCount; i++)
		{
			m_cCurDataFactory.ModifyItemData(i, m_nTestNumsFlag);
		}
	}
}

void CData::_TreatDataOrder()
{
	if(m_btDataOrderType != DATA_ORDERTYPE_NONE)	// 需要排序
	{
		BOOL bHasTestNums = (m_stLotteryProperty.btProperty & CSTLPP_TESTNUMS) ? TRUE : FALSE;
		int i, nCount = m_cCurDataFactory.GetCount(FALSE);
		for(i=0; i<nCount; i++)
		{
			// 号码顺序排列处理
			m_cCurDataFactory.OrderItemData(i, m_btDataOrderType, bHasTestNums);
		}
	}
}

void CData::_TreatDataFilter()
{
    // TEST
/*	if(g_pIConfig->GetConfig(CFGID_DATA_FILTER, NULL, 0) == 0)
	{
		return ;	// 未启用筛选
	}
    
	IConditionData *pICdtDataRoot = NULL;
	g_pICstMain->CreateObject(CST_OBJID_IConditionData, (void **)&pICdtDataRoot);
	pICdtDataRoot->CreateRootGroup(NULL);
	
	CString strFilename;
	strFilename.Format("%sDataFilter_%s.cdt", g_pIConfig->GetPath(PATH_TEMP), g_pIData->GetCurrentDataDiffName(DDLM_GLOBAL));
	pICdtDataRoot->LoadData(strFilename);
    
	if(pICdtDataRoot->PrepareValidate(TRUE))
	{
		m_cCurDataFactory.BeginBatchDelete();
		for(int i=m_cCurDataFactory.GetCount(TRUE)-1; i>=0; i--)
		{
			if(0 != pICdtDataRoot->Validate(i))
			{
				m_cCurDataFactory.BatchDelete(i);
			}
		}
		pICdtDataRoot->FinishValidate();
		m_cCurDataFactory.EndBatchDelete();
	}
	g_pICstMain->DeleteObject((void **)&pICdtDataRoot);
 */
}

void CData::_ResetCurUsedData(DWORD dwFlag, BOOL bUseWholeData)
{
	_PreResetCurUsedData(dwFlag);
	
	_ClearCurUsedData();
	
	int nCount = m_pInitDataFactory->GetCount(FALSE);
	m_cCurDataFactory.Create(nCount, TRUE);
	if(bUseWholeData)
	{
		for(int i=0; i<nCount; i++)
		{
			m_cCurDataFactory.CopyItem(i, i, m_pInitDataFactory);
		}
	}
	else
	{
		// 机球匹配处理
		_TreatMachineBall();
	}
    
#ifndef _FREE	// 非免费版
	if(m_stLotteryProperty.btProperty & CSTLPP_TESTNUMS)
	{
		// 试机号和开奖号处理
		_TreatTestNums();
	}
#endif
	
	// 原始数据转换为当前使用的数据
	_InitDataToCurData();
	
	if(bUseWholeData)
	{
		return ;
	}
	
	// 期号范围处理
	_TreatDataRange();	
	
	// 隔期处理
	_TreatDataDiv();
	
#ifndef _FREE	// 非免费版
	
	// 数据变换处理
	_TreatDataTrans();
	
	// 号码顺序排列处理
	_TreatDataOrder();
	
	// 数据筛选处理
	_TreatDataFilter();
	
	// 防破解处理
	if(m_btIsCracked)
	{
        // TEST
		// 如果被破解，则修改历史数据中任意n期数据
	/*	srand(::GetTickCount());
		nCount = m_cCurDataFactory.GetCount(TRUE);
		for(int i=0; i<nCount; i++)
		{
			if(rand() % 52 == 6)
			{
				m_cCurDataFactory.RandItemData(i);
			}
		}
		
		// 将最后20期数据中任意10期数据修改
		if(nCount > 20)
		{
			for(i=0; i<15; i++)
			{
				m_cCurDataFactory.RandItemData(nCount - 2 - rand() % 10);
			}
		}*/
	}
#endif
}

////////////////////////////////////////////////////////////////////////////////////////////////////
/*nBallCount：开奖球个数，比如数字彩为10个，扑克彩为13个等*/
BYTE CData::_GetRWLType(int nCalcIssueCount, LPBYTE lpDataBuf, int nNumber, int nBallCount)
{
	int nExistCount = 0;
	for(int i=0; i<nCalcIssueCount; i++)
	{
		nExistCount += lpDataBuf[i * nBallCount + nNumber];
	}
	
	if(nExistCount > m_stRWLInfo.wWMMaxCount)
	{
		return 0;	// 热码
	}
	else if(nExistCount < m_stRWLInfo.wWMMinCount)
	{
		return 2;	// 冷码
	}
	else
	{
		return 1;	// 温码
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
LPBYTE CData::_CalcCXTData()
{
	if(m_lpCTXData != NULL)
	{
		return m_lpCTXData;	// 已经计算过，则直接返回
	}
    
	int i, j, nCount = GetItemCount(DATA_SOURCE_CUR);
	if(nCount < 2)
	{
		return NULL;
	}
    
	int nBallCount = m_btMaxNumber - m_btMinNumber + 1;
	m_lpCTXData = new BYTE[(nCount + 1) * nBallCount];
	if(m_lpCTXData == NULL)
	{
		return NULL;
	}
	memset(m_lpCTXData, 0xFF, sizeof(BYTE) * (nCount + 1) * nBallCount);
	BYTE btTemp[128] = {0}, btTempPrev[128] = {0};
	LPBYTE lpLotteryData = GetItemNums(1, DATA_SOURCE_CUR);
	for(i=0; i<m_btNumberCount; i++)
	{
		btTemp[lpLotteryData[i] - m_btMinNumber] = 1;
	}
	lpLotteryData = GetItemNums(0, DATA_SOURCE_CUR);
	for(i=0; i<m_btNumberCount; i++)
	{
		btTempPrev[lpLotteryData[i]] = 1;
	}
    
	for(i=2; i<=nCount; i++)
	{
		for(j=0; j<nBallCount; j++)
		{
			if(btTemp[j] == 1)
			{
				m_lpCTXData[i * nBallCount + j] = 0;	// 重码
			}
			else if((btTempPrev[(j+1)%nBallCount] == 1) || (btTempPrev[(j+nBallCount-1)%nBallCount] == 1))
			{
				m_lpCTXData[i * nBallCount + j] = 1;	// 斜码
			}
			else
			{
				m_lpCTXData[i * nBallCount + j] = 2;	// 跳码
			}
		}
        
		memcpy(btTempPrev, btTemp, sizeof(btTemp));
		ZeroMemory(btTemp, sizeof(btTemp));
		if(i < nCount)
		{
			lpLotteryData = GetItemNums(i, DATA_SOURCE_CUR);
			for(j=0; j<m_btNumberCount; j++)
			{
				btTemp[lpLotteryData[j] - m_btMinNumber] = 1;
			}
		}
	}
    
	return m_lpCTXData;
}

LPBYTE CData::_CalcLGCData()
{
	if(m_lpLGCData != NULL)
	{
		return m_lpLGCData;	// 已经计算过，则直接返回
	}
    
	int i, j, nCount = GetItemCount(DATA_SOURCE_CUR);
	if(nCount < 1)
	{
		return NULL;
	}
    
	int nBallCount = m_btMaxNumber - m_btMinNumber + 1;
	m_lpLGCData = new BYTE[(nCount + 1) * nBallCount];
	if(m_lpLGCData == NULL)
	{
		return NULL;
	}
	memset(m_lpLGCData, 0xFF, sizeof(BYTE) * (nCount + 1) * nBallCount);
	BYTE btTemp[128] = {0};
	LPBYTE lpLotteryData = GetItemNums(0, DATA_SOURCE_CUR);
	for(i=0; i<m_btNumberCount; i++)
	{
		btTemp[lpLotteryData[i] - m_btMinNumber] = 1;
	}
    
	for(i=1; i<=nCount; i++)
	{
		for(j=0; j<nBallCount; j++)
		{
			if(btTemp[j] == 1)
			{
				m_lpLGCData[i * nBallCount + j] = 2;	// 传码
			}
			else if((btTemp[(j+1)%nBallCount] == 1) || (btTemp[(j+nBallCount-1)%nBallCount] == 1))
			{
				m_lpLGCData[i * nBallCount + j] = 0;	// 邻码
			}
			else
			{
				m_lpLGCData[i * nBallCount + j] = 1;	// 孤码
			}
		}
        
		ZeroMemory(btTemp, sizeof(btTemp));
		if(i < nCount)
		{
			lpLotteryData = GetItemNums(i, DATA_SOURCE_CUR);
			for(j=0; j<m_btNumberCount; j++)
			{
				btTemp[lpLotteryData[j] - m_btMinNumber] = 1;
			}
		}
	}
    
	return m_lpLGCData;
}

LPBYTE CData::_CalcFGZData()
{
	if(m_lpFGZData != NULL)
	{
		return m_lpFGZData;	// 已经计算过，则直接返回
	}
    
	int i, j, nCount = GetItemCount(DATA_SOURCE_CUR);
	if(nCount < 2)
	{
		return NULL;
	}
    
	int nBallCount = m_btMaxNumber - m_btMinNumber + 1;
	m_lpFGZData = new BYTE[(nCount + 1) * nBallCount];
	if(m_lpFGZData == NULL)
	{
		return NULL;
	}
	memset(m_lpFGZData, 0xFF, sizeof(BYTE) * (nCount + 1) * nBallCount);
	BYTE btTemp[128] = {0}, btTempPrev[128] = {0};
	LPBYTE lpLotteryData = GetItemNums(1, DATA_SOURCE_CUR);
	for(i=0; i<m_btNumberCount; i++)
	{
		btTemp[lpLotteryData[i] - m_btMinNumber] = 1;
	}
	lpLotteryData = GetItemNums(0, DATA_SOURCE_CUR);
	for(i=0; i<m_btNumberCount; i++)
	{
		btTempPrev[lpLotteryData[i] - m_btMinNumber] = 1;
	}
    
	for(i=2; i<=nCount; i++)
	{
		for(j=0; j<nBallCount; j++)
		{
			if(btTemp[j] == 1)
			{
				m_lpFGZData[i * nBallCount + j] = 0;	// 复码
			}
			else if(btTempPrev[j] == 1)
			{
				m_lpFGZData[i * nBallCount + j] = 1;	// 隔码
			}
			else
			{
				m_lpFGZData[i * nBallCount + j] = 2;	// 中码
			}
		}
        
		memcpy(btTempPrev, btTemp, sizeof(btTemp));
		ZeroMemory(btTemp, sizeof(btTemp));
		if(i < nCount)
		{
			lpLotteryData = GetItemNums(i, DATA_SOURCE_CUR);
			for(j=0; j<m_btNumberCount; j++)
			{
				btTemp[lpLotteryData[j] - m_btMinNumber] = 1;
			}
		}
	}
    
	return m_lpFGZData;
}

LPBYTE CData::_CalcRWLData()
{
	if(m_lpRWLData != NULL)
	{
		return m_lpRWLData;	// 已经计算过，则直接返回
	}
	
	// 获取热温冷信息
// TEST	g_pIConfig->GetConfig(CFGID_DATA_RWL, LPBYTE(&m_stRWLInfo), sizeof(m_stRWLInfo));
    
	int i, j, nCount = GetItemCount(DATA_SOURCE_CUR);
    
	if(nCount <= m_stRWLInfo.wIssueCount)
	{
		return NULL;
	}
    
	int nBallCount = m_btMaxNumber - m_btMinNumber + 1;
	m_lpRWLData = new BYTE[(nCount + 1) * nBallCount];
	if(m_lpRWLData == NULL)
	{
		return NULL;
	}
	memset(m_lpRWLData, 0xFF, sizeof(BYTE) * (nCount + 1) * nBallCount);
	LPBYTE lpDataBuf = new BYTE[m_stRWLInfo.wIssueCount * nBallCount];
	if(lpDataBuf == NULL)
	{
		return NULL;
	}
	ZeroMemory(lpDataBuf, m_stRWLInfo.wIssueCount * nBallCount);
	LPBYTE lpLotteryData = NULL;
	for(i=0; i<m_stRWLInfo.wIssueCount; i++)
	{
		lpLotteryData = GetItemNums(i, DATA_SOURCE_CUR);
		for(j=0; j<m_btNumberCount; j++)
		{
			if(m_stRWLInfo.btRepeatToOne == 1)	// 多个算一个
				lpDataBuf[i * nBallCount + lpLotteryData[j] - m_btMinNumber] = 1;
			else
				lpDataBuf[i * nBallCount + lpLotteryData[j] - m_btMinNumber] ++;
		}
	}
    
	for(i=m_stRWLInfo.wIssueCount; i<=nCount; i++)
	{
		for(j=0; j<nBallCount; j++)
		{
			m_lpRWLData[i * nBallCount + j] = _GetRWLType(m_stRWLInfo.wIssueCount, lpDataBuf, j, nBallCount);
		}
        
		if(i < nCount)
		{
			for(j=0; j<m_stRWLInfo.wIssueCount-1; j++)
			{
				memcpy(&lpDataBuf[j * nBallCount], &lpDataBuf[(j + 1) * nBallCount], nBallCount);
			}
			ZeroMemory(&lpDataBuf[(m_stRWLInfo.wIssueCount - 1) * nBallCount], nBallCount);
			lpLotteryData = GetItemNums(i, DATA_SOURCE_CUR);
			for(j=0; j<m_btNumberCount; j++)
			{
				if(m_stRWLInfo.btRepeatToOne == 1)	// 多个算一个
					lpDataBuf[(m_stRWLInfo.wIssueCount - 1) * nBallCount + lpLotteryData[j] - m_btMinNumber] = 1;
				else
					lpDataBuf[(m_stRWLInfo.wIssueCount - 1) * nBallCount + lpLotteryData[j] - m_btMinNumber] ++;
			}
		}
	}
	delete []lpDataBuf;
    
	return m_lpRWLData;
}

int CData::_GetSPJData(DWORD dwFlag, DWORD dwIssueIndex, LPBYTE lpFilterNums)
{
	int i, nRtn = 0, nCount = m_cCurDataFactory.GetCount(TRUE);
	if(nCount < 1 || dwIssueIndex < 1)
		return CDTVALUE_INVALID;
    
	if(dwIssueIndex == DATA_INDEX_LAST) 
		dwIssueIndex = nCount - 1;
	else if(dwIssueIndex == DATA_INDEX_INVALID)
		dwIssueIndex = nCount;
    
	LPBYTE lpLotteryData = m_cCurDataFactory.GetItemNums(dwIssueIndex - 1);	// 上一期数据
	LPBYTE lpCurLotteryData = (lpFilterNums == NULL) ? m_cCurDataFactory.GetItemNums(dwIssueIndex) : lpFilterNums;
	int nTemp = 0, nTemp2 = 0;
    
	switch(dwFlag)
	{
        case GTSDF_ONE:
        case GTSDF_TWO:
        case GTSDF_THREE:
        case GTSDF_FOUR:
        case GTSDF_FIVE:
        case GTSDF_SIX:
        case GTSDF_SEVEN:
        case GTSDF_EIGHT:
            nTemp = lpLotteryData[dwFlag - GTSDF_ONE];
            nTemp2 = lpCurLotteryData[dwFlag - GTSDF_ONE];
            if(nTemp == nTemp2)
                nRtn = 1;	
            else if(nTemp > nTemp2)
                nRtn = 2;
            else
                nRtn = 0;
            break;
        case GTSDF_COUNT_0:
		{
			for(i=0; i<m_btNumberCount; i++)
			{
				if(lpCurLotteryData[i] > lpLotteryData[i])
					nRtn ++;
			}
		} break;
            
        case GTSDF_COUNT_1:
		{
			for(i=0; i<m_btNumberCount; i++)
			{
				if(lpCurLotteryData[i] == lpLotteryData[i])
					nRtn ++;
			}
		} break;
            
        case GTSDF_COUNT_2:
		{
			for(i=0; i<m_btNumberCount; i++)
			{
				if(lpCurLotteryData[i] < lpLotteryData[i])
					nRtn ++;
			}
		} break;
            
        case GTSDF_ARR_COUNT:
		{
			int nCount[3] = {0}; 
			for(i=0; i<m_btNumberCount; i++)
			{
				if(lpCurLotteryData[i] > lpLotteryData[i])
					nCount[0] ++;
				else if(lpCurLotteryData[i] == lpLotteryData[i])
					nCount[1] ++;
				else
					nCount[2] ++;
			}
			nRtn = nCount[0] * 100 + nCount[1] * 10 + nCount[2];
		} break;
            
        case GTSDF_ARR_SIN:
		{
			int j = 1, k = 0;
			for(i=m_btNumberCount-1; i>=0; i--)
			{
				if(lpCurLotteryData[i] > lpLotteryData[i])
					k = 0;
				else if(lpCurLotteryData[i] == lpLotteryData[i])
					k = 1;
				else
					k = 2;
				nRtn += k * j;
				j *= 10;
			}
		} break;
            
        case GTSDF_ARR_GRO:
		{
			int nRatio = 1;
			BYTE btTemp[MAX_NUMBER_COUNT] = {0};
			for(i=m_btNumberCount-1; i>=0; i--)
			{
				if(lpCurLotteryData[i] > lpLotteryData[i])
					btTemp[i] = 0;
				else if(lpCurLotteryData[i] == lpLotteryData[i])
					btTemp[i] = 1;
				else
					btTemp[i] = 2;
			}
// TEST			g_pICstPubFunc->QuickSort(btTemp, m_btNumberCount);
			for(i=m_btNumberCount-1; i>=0; i--)
			{
				nRtn += btTemp[i] * nRatio;
				nRatio *= 10;
			}
		} break;
            
        case GTSDF_ARR_SIN_VALUE:
		{
			int j = 1, k = 0;
			for(i=m_btNumberCount-1; i>=0; i--)
			{
				if(lpCurLotteryData[i] > lpLotteryData[i])
					k = 0;
				else if(lpCurLotteryData[i] == lpLotteryData[i])
					k = 1;
				else
					k = 2;
				nRtn += k * j;
				j *= 3;
			}
		} break;
            
        case GTSDF_SHAPE:
		{
			ASSERT(m_btNumberCount == 3);	// 3D专用
			int nCount[3] = {0}; 
			for(i=0; i<3; i++)
			{
				if(lpCurLotteryData[i] > lpLotteryData[i])
					nCount[0] ++;
				else if(lpCurLotteryData[i] == lpLotteryData[i])
					nCount[1] ++;
				else
					nCount[2] ++;
			}
			if(nCount[0] == 3 || nCount[1] == 3 || nCount[2] == 3)
			{
				nRtn = 0;		// 全路
			}
			else if(nCount[0] == 1 && nCount[1] == 1)
			{
				nRtn = 2;		// 111路
			}
			else
			{
				nRtn = 1;			// 断一路
			}
		} break;
            
        default:
            break;
	}
    
	return nRtn;
}

LPDWORD CData::_CalcSkipValueData_Single()
{
	if(m_lpSingleSkipValueData != NULL)
	{
		return m_lpSingleSkipValueData;	// 已经计算过，则直接返回
	}
    
	// 根据数据总数和最大号码创建缓冲区
	int nCount = m_cCurDataFactory.GetCount(TRUE);
    
	// m_lpSingleSkipValueData前面依次为每期号码的每位数字的遗漏，遗漏和值、遗漏合值的数据
	// 最后m_btNumberCount * m_btMaxNumber个为最后一期对应位置的开奖范围的数字的遗漏值
	int nDataBufLen = nCount * (m_btNumberCount + 2) + m_btNumberCount * (m_btMaxNumber + 1);
	NEW_BUFFER(m_lpSingleSkipValueData, DWORD, nDataBufLen); 
    
	DWORD dwSkipSum = 0, dwSkipValues[8][100] = {0};	// 最大8个数字，号码最大为99
	int i, j, k, nTemp, nIndex = 0;
	
	LPBYTE lpLotteryData = NULL;
    
	// 计算历史号码的单选遗漏值
	for(i=0; i<nCount; i++)
	{
		dwSkipSum = 0;
		lpLotteryData = m_cCurDataFactory.GetItemNums(i);
		for(j=0; j<m_btNumberCount; j++)
		{
			nTemp = lpLotteryData[j];
			m_lpSingleSkipValueData[nIndex++] = dwSkipValues[j][nTemp];
			dwSkipSum += dwSkipValues[j][nTemp];
			dwSkipValues[j][nTemp] = MAXDWORD;
		}
		m_lpSingleSkipValueData[nIndex++] = dwSkipSum;
		m_lpSingleSkipValueData[nIndex++] = dwSkipSum % 10;
        
		for(j=0; j<=m_btMaxNumber; j++)
		{
			for(k=0; k<m_btNumberCount; k++)
			{
				dwSkipValues[k][j] ++;
			}
		}
	}
    
	// 计算最后一期的所有数字的单选遗漏，用于缩水
	for(k=0; k<m_btNumberCount; k++)
	{
		for(i=0; i<=m_btMaxNumber; i++)
		{
			m_lpSingleSkipValueData[nIndex++] = dwSkipValues[k][i];
		}
	}
    
	ASSERT(nIndex == nDataBufLen);
    
	return m_lpSingleSkipValueData;
}

LPDWORD CData::_CalcSkipValueData_Group()
{
	if(m_lpGroupSkipValueData != NULL)
	{
		return m_lpGroupSkipValueData;	// 已经计算过，则直接返回
	}
    
	// 根据数据总数和最大号码创建缓冲区
	int nCount = m_cCurDataFactory.GetCount(TRUE);
    
	// m_lpGroupSkipValueData前面依次为每期号码的每位数字的遗漏，遗漏和值、遗漏合值的数据
	// 最后m_btNumberCount * m_btMaxNumber个为最后一期的开奖范围的数字的遗漏值
	int nDataBufLen = nCount * (m_btNumberCount + 2) + (m_btMaxNumber + 1);
	NEW_BUFFER(m_lpGroupSkipValueData, DWORD, nDataBufLen); 
    
	int nMissValue[100] = {0}, nMissValue2[100] = {0};
	int i, j, nTemp, nSkipSum = 0, nIndex = 0;
	LPBYTE lpLotteryData = NULL;
	
	// 计算历史号码的组选遗漏值
	for(i=0; i<nCount; i++)
	{
		nSkipSum = 0;
		lpLotteryData = m_cCurDataFactory.GetItemNums(i);
        
		for(j=0; j<m_btNumberCount; j++)
		{
			nTemp = lpLotteryData[j];
			if(nMissValue[nTemp] != -1)
			{
				nMissValue2[nTemp] = nMissValue[nTemp];
				nMissValue[nTemp] = -1;
			}
			m_lpGroupSkipValueData[nIndex++] = (DWORD)nMissValue2[nTemp];
			nSkipSum += nMissValue2[nTemp];
		}
		m_lpGroupSkipValueData[nIndex++] = nSkipSum;
		m_lpGroupSkipValueData[nIndex++] = nSkipSum % 10;
        
		for(j=0; j<=m_btMaxNumber; j++)
		{
			nMissValue[j] ++;
		}
	}
    
	// 计算最后一期的所有数字的组选遗漏，用于缩水
	for(i=0; i<=m_btMaxNumber; i++)
	{
		m_lpGroupSkipValueData[nIndex++] = nMissValue[i];
	}
	
	ASSERT(nIndex == nDataBufLen);
    
	return m_lpGroupSkipValueData;
}

void CData::_TreatDataTrans()
{
    // TEST
/*	if(m_stDataTrans.btType == DATATRANS_TYPE_ORG)
	{
		theApp.m_cDataTransTable.Destroy();
		return ;		// 原始数据，则不处理
	}
    
	DATATRANSSET stCurDataTrans = {0};
	memcpy(&stCurDataTrans, &m_stDataTrans, sizeof(stCurDataTrans));
    
	// 准备获取原始数据的条件
	g_pIConditionMan->PrepareGetOrgDataTransConditions();
    
	// 创建数据变换对应表，已被原始号码缩水等使用
	theApp.m_cDataTransTable.Create(&stCurDataTrans);
    
	int i, j, nCount = m_cCurDataFactory.GetCount(TRUE), nValue = 0;
	ICondition *pICondition[8] = {0};
	BYTE btFixValues[8] = {0};
    
	// 创建每位对应的条件
	for(i=0; i<stCurDataTrans.btItemCount; i++)
	{
		if(stCurDataTrans.cdtIDs[i].IsValid())	// 使用条件值
		{
			pICondition[i] = g_pIConditionMan->GetICondition(stCurDataTrans.cdtIDs[i]);
			ASSERT(!pICondition[i]->IsMultipleValue());
		}
		else	// 使用固定值
		{
			btFixValues[i] = (BYTE)stCurDataTrans.cdtIDs[i].cdtIID.dwSystemIID;
			if(btFixValues[i] < m_btMinNumber || btFixValues[i] > m_btMaxNumber)
			{
				ASSERT(FALSE);
				btFixValues[i] = m_btMinNumber;
			}
		}
	}
    
	// 获取每位变换后的数据
	int nAllNumCount = m_btMaxNumber - m_btMinNumber + 1, nItemIndex = 0;
	LPBYTE lpNums = NULL, lpTransDataBuf = new BYTE[MAX_NUMBER_COUNT * nCount];
    
	for(i=0; i<nCount; i++)
	{
		lpNums = m_cCurDataFactory.GetItemNums(i);
		for(j=0; j<stCurDataTrans.btItemCount; j++)
		{
			if(pICondition[j] == 0)
			{
				lpTransDataBuf[nItemIndex++] = btFixValues[j];
			}
			else
			{
				nValue = pICondition[j]->GetValue(NULL, i);
				if(nValue == CDTVALUE_INVALID)
				{
					nValue = 0;
				}
				else if(nValue < 0)
				{
					nValue = 0 - nValue;
				}
				lpTransDataBuf[nItemIndex++] = (BYTE)((nValue % nAllNumCount) + m_btMinNumber);
			}
		}
		for(j; j<MAX_NUMBER_COUNT; j++)
		{
			lpTransDataBuf[nItemIndex++] = lpNums[j];
		}
	}
    
	nItemIndex = 0;
	for(i=0; i<nCount; i++)
	{
		m_cCurDataFactory.SetItemData(i, &lpTransDataBuf[nItemIndex]);
		nItemIndex += MAX_NUMBER_COUNT;
	}
    
	delete []lpTransDataBuf;
    
	// 完成原始数据变换的条件获取
	g_pIConditionMan->FinishGetOrgataTransConditions();
    
	// 通知数据工厂号码个数改变
	m_cCurDataFactory.OnNumberCountChanged();
    
	_ClearCurUsedData(FALSE); */
}

DWORD CData::_GetValueFromPosItem(LPLOTTERYPOSITEM lpPosItem)
{
	ASSERT(lpPosItem != NULL);
    
	if(lpPosItem->btFlagPrev == LPIF_ORG)
	{
		return 0;		// 原始方式
	}
    
	// 计算位置值，因为最大号码个数是8，所以按8进制计算以节省空间
	DWORD dwValue = 0;
	DWORD dwTemp = 1;
	for(int i=lpPosItem->btCount-1; i>=0; i--)
	{
		dwValue += (lpPosItem->btPosArray[i] + 1) * dwTemp;
		dwTemp *= 8;
	}
    
	dwValue *= 100;		// 留出十个位保存取位标志值
    
	dwValue += lpPosItem->btFlagPrev * 10 + lpPosItem->btFlagNext;
    
	return dwValue;
}

void CData::_TreatPosItem(LPLOTTERYPOSITEM lpPosItem, BYTE &btDataType, BYTE &btMinNumber, BYTE &btMaxNumber)
{
	ASSERT(lpPosItem != NULL);
    
	if(lpPosItem->btFlagPrev == LPIF_ORG)
	{
		return ;	// 原始数据则不需要更改
	}
    
	// 前处理
	_TreatPosItemFlag(lpPosItem->btFlagPrev, btDataType, btMinNumber, btMaxNumber);
    
	// 位置处理
	BOOL bOrdered = TRUE, bRepeated = FALSE;
	for(BYTE i=1; i<lpPosItem->btCount; i++)
	{
		if(lpPosItem->btPosArray[i] <= lpPosItem->btPosArray[i - 1])
		{
			bOrdered = FALSE;
		}
		if(lpPosItem->btPosArray[i] <= lpPosItem->btPosArray[i - 1])
		{
			bRepeated = TRUE;
		}
	}
    
	if((btDataType & DATA_TYPE_ORDERED) && !bOrdered)
		btDataType &= ~DATA_TYPE_ORDERED;
    
	if(bRepeated)
		btDataType |= DATA_TYPE_REPEATABLE;
    
	// 后处理
	_TreatPosItemFlag(lpPosItem->btFlagNext, btDataType, btMinNumber, btMaxNumber);
}

void CData::_TreatPosItemFlag(BYTE btFlag, BYTE &btDataType, BYTE &btMinNumber, BYTE &btMaxNumber)
{
	switch(btFlag)
	{
        case LPIF_ORDER: btDataType |= DATA_TYPE_ORDERED; break;
        case LPIF_TAIL:	
            if((btDataType & DATA_TYPE_NUM_MASK) != DATA_TYPE_NUM_SZ)	// 非数字型
            {
                btDataType = DATA_TYPE_NUM_SZ | DATA_TYPE_REPEATABLE;
                btMinNumber = 0;
                btMaxNumber = 9;
            }
            break;
        case LPIF_ORDER_TAIL:
            if((btDataType & DATA_TYPE_NUM_MASK) == DATA_TYPE_NUM_SZ)	// 数字型
            {
                btDataType |= DATA_TYPE_ORDERED;
            }
            else	// 非数字型
            {
                btDataType = DATA_TYPE_NUM_SZ | DATA_TYPE_REPEATABLE;
                btMinNumber = 0;
                btMaxNumber = 9;
            }
            break;
        case LPIF_TAIL_ORDER:
            if((btDataType & DATA_TYPE_NUM_MASK) == DATA_TYPE_NUM_SZ)	// 数字型
            {
                btDataType |= DATA_TYPE_ORDERED;
            }
            else	// 非数字型
            {
                btDataType = DATA_TYPE_NUM_SZ | DATA_TYPE_REPEATABLE | DATA_TYPE_ORDERED;
                btMinNumber = 0;
                btMaxNumber = 9;
            }
            break;
	}
}

/*
BOOL CData::_GetGPLotteryNextTime(const CTime &tmDateTime, CTime &tmNextDateTime)
{
	int nCurTime = tmDateTime.GetHour() * 100 + tmDateTime.GetMinute();
    
	// 查询当前时间是哪一期，由于可能存在时间变动，故获取最接近的时间
	int nMinValue = abs(nCurTime - m_stLotteryProperty.wOpenTimeArray[0]), nIndex = 0;
	
	for(int i=1; i<m_stLotteryProperty.btOpenTimesPerDay; i++)
	{
		if(abs(nCurTime - m_stLotteryProperty.wOpenTimeArray[i]) < nMinValue)
		{
			nMinValue = abs(nCurTime - m_stLotteryProperty.wOpenTimeArray[i]);
			nIndex = i;
			if(nMinValue == 0)
			{
				break;		// 为0的话已经是最小误差了，则无需再进行比较了
			}
		}
	}
    
	int nNextTime = m_stLotteryProperty.wOpenTimeArray[(nIndex + 1) % m_stLotteryProperty.btOpenTimesPerDay];
	tmNextDateTime = CTime(tmDateTime.GetYear(), tmDateTime.GetMonth(), tmDateTime.GetDay(), nNextTime / 100, nNextTime % 100, 0);
    
	if(nNextTime < nCurTime)	// 跨天
	{
		tmNextDateTime += CTimeSpan(1, 0, 0, 0);
	}
    
	// 返回是否是当天第一期
	return (nIndex == m_stLotteryProperty.btOpenTimesPerDay - 1) ? TRUE : FALSE;
}*/