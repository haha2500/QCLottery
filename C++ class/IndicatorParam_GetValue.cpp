//
//  IndicatorParam_GetValue.cpp
//  QCLottery
//
//  Created by 武 孙 on 12-5-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "IndicatorParam.h"

///////////////////////////////////////////////////////////////////////////////////////////////
LPCDTVALUE CIndicatorParam::GetValue(const CDTIPID &cdtIPID, ICondition *pICdtParent, DWORD dwCustomData, LPBYTE lpFltNumsData, DWORD dwIssueIndex, BYTE /*btType*/)
{
	ASSERT(pICdtParent != NULL);
	
	LPCDTVALUE lpParentValue = pICdtParent->GetValueEx(lpFltNumsData, dwIssueIndex);
    
	_RemoveAllValue();
    
	if(lpParentValue->stValueItems[0].nValue == CDTVALUE_INVALID)	// 条件无效值
	{
		_AddValue(CDTVALUE_INVALID);
		return m_lpValueTemp;
	}
    
	if(cdtIPID.btType & CDTIPID_TYPE_CUSTOM)		// 自定义指标参数
	{
		return _GetCustomValue(cdtIPID, pICdtParent, dwCustomData, lpParentValue);
	}
	else	// 常规指标参数
	{
		switch(cdtIPID.dwSystemIPID)
		{
            case IPID_NONE: break;
                
            case IPID_S_ODDEVEN: 
			{
				for(int i=0; i<lpParentValue->btItemCount; i++)
				{
					_AddValue(abs(lpParentValue->stValueItems[i].nValue) % 2, lpParentValue->stValueItems[i].btCount);
				}
			} break;
                
            case IPID_S_BIGSMALL:	// 大小作为二分区
			{
				int nMinValue = 0, nMaxValue = 0;
				_GetValueRangeForPosArea(nMinValue, nMaxValue, pICdtParent);
				for(int i=0; i<lpParentValue->btItemCount; i++)
				{
					int nValue = theApp.GetSubAreaIndex(2, lpParentValue->stValueItems[i].nValue, nMinValue, nMaxValue) - 1;
					_AddValue(nValue, lpParentValue->stValueItems[i].btCount);
				}
			} break;
                
            case IPID_S_DZX:	// 大中小作为三分区
			{
				int nMinValue = 0, nMaxValue = 0;
				_GetValueRangeForPosArea(nMinValue, nMaxValue, pICdtParent);
				for(int i=0; i<lpParentValue->btItemCount; i++)
				{
					int nValue = theApp.GetSubAreaIndex(3, lpParentValue->stValueItems[i].nValue, nMinValue, nMaxValue) - 1;
					_AddValue(nValue, lpParentValue->stValueItems[i].btCount);
				}
                
			} break;
                
            case IPID_S_012: 
			{
				for(int i=0; i<lpParentValue->btItemCount; i++)
				{
					_AddValue(abs(lpParentValue->stValueItems[i].nValue) % 3, lpParentValue->stValueItems[i].btCount);
				}
			}
                break;
                
            case IPID_S_PRIMECOMPOSITE: 
			{
				for(int i=0; i<lpParentValue->btItemCount; i++)
				{
					ASSERT(lpParentValue->stValueItems[i].nValue >= -999 && lpParentValue->stValueItems[i].nValue <= 999);
					_AddValue(theApp.m_btPrimeFlag[abs(lpParentValue->stValueItems[i].nValue)], lpParentValue->stValueItems[i].btCount);
				}
			}break;
                
            case IPID_S_TAIL: 
			{
				for(int i=0; i<lpParentValue->btItemCount; i++)
				{
					_AddValue(abs(lpParentValue->stValueItems[i].nValue) % 10, lpParentValue->stValueItems[i].btCount);
				}
			}break;
                
            case IPID_S_ABS: 
			{
				for(int i=0; i<lpParentValue->btItemCount; i++)
				{
					_AddValue(abs(lpParentValue->stValueItems[i].nValue), lpParentValue->stValueItems[i].btCount);
				}
			}break;
                
            case IPID_S_ABS_TAIL: 
			{
				for(int i=0; i<lpParentValue->btItemCount; i++)
				{
					_AddValue(abs(lpParentValue->stValueItems[i].nValue) % 10, lpParentValue->stValueItems[i].btCount);
				}
			}break;
                
            case IPID_S_SUM: _AddValue(_GetSumValue(lpParentValue)); break;
            case IPID_S_SUM_TAIL: _AddValue(_GetSumValue(lpParentValue, TRUE)); break;
            case IPID_S_SUM_N: _AddValue(_GetSumNValue(lpParentValue)); break;
            case IPID_S_SPAN: _AddValue(_GetSpanValue(lpParentValue)); break;
            case IPID_S_SPAN_TAIL: _AddValue(_GetSpanValue(lpParentValue, TRUE)); break;
            case IPID_S_AVG: _AddValue(_GetAvgValue(lpParentValue)); break;
            case IPID_S_AVG_TAIL: _AddValue(_GetAvgValue(lpParentValue, TRUE)); break;
            case IPID_S_MIN: _AddValue(_GetMinValue(lpParentValue)); break;
            case IPID_S_MIN_TAIL: _AddValue(_GetMinValue(lpParentValue, TRUE)); break;
            case IPID_S_MID: _AddValue(_GetMidValue(lpParentValue)); break;
            case IPID_S_MID_TAIL: _AddValue(_GetMidValue(lpParentValue, TRUE)); break;
            case IPID_S_MAX: _AddValue(_GetMaxValue(lpParentValue)); break;
            case IPID_S_MAX_TAIL: _AddValue(_GetMaxValue(lpParentValue, TRUE)); break;
                
            case IPID_D_DISTANCE: 
            case IPID_D_DISTANCE_TAIL: 
			{
				ASSERT(lpParentValue->btItemCount == 1);
                
				int nValue = _GetDistanceValue(pICdtParent, lpParentValue->stValueItems[0].nValue, dwIssueIndex, cdtIPID.dwSystemIPID); 
				_AddValue(nValue, lpParentValue->stValueItems[0].btCount);
			}break;
                
            case IPID_D_CIRCLE_DISTANCE: 
            case IPID_D_CIRCLE_DISTANCE_TAIL: 
			{
				ASSERT(lpParentValue->btItemCount == 1);
				
				int nValue = _GetCircleDistanceValue(pICdtParent, lpParentValue->stValueItems[0].nValue, dwIssueIndex, cdtIPID.dwSystemIPID); 
				_AddValue(nValue, lpParentValue->stValueItems[0].btCount);
			}break;
                
            case IPID_D_ADJACENT_SUB:
            case IPID_D_ADJACENT_SUM:
            case IPID_D_ADJACENT_SUMTAIL:
			{
				ASSERT(lpParentValue->btItemCount == 1);
				int nValue = _GetAdjacentValue(pICdtParent, lpParentValue->stValueItems[0].nValue, dwIssueIndex, cdtIPID.dwSystemIPID);
				_AddValue(nValue, lpParentValue->stValueItems[0].btCount);
			} break;
                
            case IPID_D_SPJ:
			{
				ASSERT(lpParentValue->btItemCount == 1);
				int nValue = _GetSPJValue(pICdtParent, lpParentValue->stValueItems[0].nValue, dwIssueIndex);
				_AddValue(nValue, lpParentValue->stValueItems[0].btCount);
			} break;
                
            case IPID_D_AXIS_SUB:
            case IPID_D_AXIS_DISTANCE:
            case IPID_D_AXIS_DISTANCE_TAIL:
			{
				ASSERT(lpParentValue->btItemCount == 1);
				int nValue = _GetAxisValue(pICdtParent, lpParentValue->stValueItems[0].nValue, dwIssueIndex, cdtIPID.dwSystemIPID);
				_AddValue(nValue, lpParentValue->stValueItems[0].btCount);
			} break;
                
            case IPID_D_SKIP:
            case IPID_D_SKIP_TAIL:
			{
				ASSERT(lpParentValue->btItemCount == 1);
				int nValue = _GetSkipValue(pICdtParent, lpParentValue->stValueItems[0].nValue, dwIssueIndex, cdtIPID.dwSystemIPID);
				_AddValue(nValue, lpParentValue->stValueItems[0].btCount);
			} break;
                
            case IPID_D_TESTNUMS_COMPARE:
            case IPID_D_TESTNUMS_DISTANCE:
            case IPID_D_TESTNUMS_DISTANCE_TAIL:
            case IPID_D_TESTNUMS_SUB:
            case IPID_D_TESTNUMS_CIRSUB:
            case IPID_D_TESTNUMS_CIRSUB_TAIL: _AddValue(_GetTestNumsXXXValue(cdtIPID.dwSystemIPID, pICdtParent, lpParentValue->stValueItems[0].nValue, dwIssueIndex)); break;
                
            default: 
			{
				if(cdtIPID.dwSystemIPID >= IPID_S_REMAINDER_0 && cdtIPID.dwSystemIPID <= IPID_S_REMAINDER_MAX)	// 余数
				{
					int nDivisor = cdtIPID.dwSystemIPID - IPID_S_REMAINDER_0;
					for(int i=0; i<lpParentValue->btItemCount; i++)
					{
						_AddValue(lpParentValue->stValueItems[i].nValue % nDivisor, lpParentValue->stValueItems[i].btCount);
					}
				}
				else if(cdtIPID.dwSystemIPID >= IPID_S_SUBAREA_0 && cdtIPID.dwSystemIPID <= IPID_S_SUBAREA_MAX)		// 分区
				{
					int nAreaCount = cdtIPID.dwSystemIPID - IPID_S_SUBAREA_0;
					int nMinValue = 0, nMaxValue = 0;
					_GetValueRangeForPosArea(nMinValue, nMaxValue, pICdtParent);
					for(int i=0; i<lpParentValue->btItemCount; i++)
					{
						int nValue = theApp.GetSubAreaIndex(nAreaCount, lpParentValue->stValueItems[i].nValue, nMinValue, nMaxValue);
						_AddValue(nValue, lpParentValue->stValueItems[i].btCount);
					}
				}
				else
				{
					ASSERT(FALSE); 
				}
			} break;
		}
	}
    
	return m_lpValueTemp;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
int CIndicatorParam::_GetSumValue(LPCDTVALUE lpParentValue, BOOL bGetTail)
{
	ASSERT(lpParentValue != NULL);
	if(lpParentValue->stValueItems[0].nValue == CDTVALUE_INVALID)
	{
		return CDTVALUE_INVALID;
	}
	
	int nValue = 0;
	for(int i=0; i<lpParentValue->btItemCount; i++)
	{
		nValue += lpParentValue->stValueItems[i].nValue * lpParentValue->stValueItems[i].btCount;
	}
    
	if(bGetTail)
	{
		if(nValue < 0)
			nValue = 0 - nValue;
		nValue %= 10;
	}
    
	return nValue;
}

int CIndicatorParam::_GetSumNValue(LPCDTVALUE lpParentValue)
{
	ASSERT(lpParentValue != NULL);
	if(lpParentValue->stValueItems[0].nValue == CDTVALUE_INVALID)
	{
		return CDTVALUE_INVALID;
	}
	
	int nValue = 0, nTemp = 0;
	
	for(int i=0; i<lpParentValue->btItemCount; i++)
	{
		nValue += abs(lpParentValue->stValueItems[i].nValue) * lpParentValue->stValueItems[i].btCount;
	}
	
	while(nValue > 9)
	{
		nTemp = 0;
		while(nValue != 0)
		{
			nTemp += nValue % 10;
			nValue /= 10;
		}
		nValue = nTemp;
	}
	
	return nValue;
}

int CIndicatorParam::_GetSpanValue(LPCDTVALUE lpParentValue, BOOL bGetTail)
{
	ASSERT(lpParentValue != NULL);
	if(lpParentValue->stValueItems[0].nValue == CDTVALUE_INVALID)
	{
		return CDTVALUE_INVALID;
	}
    
	int nMin = lpParentValue->stValueItems[0].nValue, nMax = lpParentValue->stValueItems[0].nValue;
	for(int i=1; i<lpParentValue->btItemCount; i++)
	{
		if(lpParentValue->stValueItems[i].nValue > nMax)	nMax = lpParentValue->stValueItems[i].nValue;
		if(lpParentValue->stValueItems[i].nValue < nMin)	nMin = lpParentValue->stValueItems[i].nValue;
	}
    
	int nValue = nMax - nMin;
    
	if(bGetTail)
	{
		if(nValue < 0)
			nValue = 0 - nValue;
		nValue %= 10;
	}
    
	return nValue;
}

int CIndicatorParam::_GetAvgValue(LPCDTVALUE lpParentValue, BOOL bGetTail)
{
	ASSERT(lpParentValue != NULL);
	if(lpParentValue->stValueItems[0].nValue == CDTVALUE_INVALID)
	{
		return CDTVALUE_INVALID;
	}
	
	int nValue = 0, nTemp = 0, nCount = 0;
	
	for(int i=0; i<lpParentValue->btItemCount; i++)
	{
		nTemp += lpParentValue->stValueItems[i].nValue * lpParentValue->stValueItems[i].btCount;
		nCount += lpParentValue->stValueItems[i].btCount;
	}
    
	nValue = int(((double)nTemp / (double)nCount) + 0.5);
    
	if(bGetTail)
	{
		if(nValue < 0)
			nValue = 0 - nValue;
		nValue %= 10;
	}
	
	return nValue;
}

int CIndicatorParam::_GetMinValue(LPCDTVALUE lpParentValue, BOOL bGetTail)
{
	ASSERT(lpParentValue != NULL);
	if(lpParentValue->stValueItems[0].nValue == CDTVALUE_INVALID)
	{
		return CDTVALUE_INVALID;
	}
	
	int nValue = lpParentValue->stValueItems[0].nValue;
	for(int i=1; i<lpParentValue->btItemCount; i++)
	{
		if(lpParentValue->stValueItems[i].nValue < nValue)
			nValue = lpParentValue->stValueItems[i].nValue;
	}
    
	if(bGetTail)
	{
		if(nValue < 0)
			nValue = 0 - nValue;
		nValue %= 10;
	}
	
	return nValue;
}

int CIndicatorParam::_GetMidValue(LPCDTVALUE lpParentValue, BOOL bGetTail)
{
	ASSERT(lpParentValue != NULL);
	if(lpParentValue->stValueItems[0].nValue == CDTVALUE_INVALID)
	{
		return CDTVALUE_INVALID;
	}
	
	int nValue = 0;
    
	switch(lpParentValue->btItemCount)
	{
        case 1: nValue = lpParentValue->stValueItems[0].nValue; break;
        case 2: nValue = (lpParentValue->stValueItems[0].btCount == 2) ? lpParentValue->stValueItems[0].nValue : lpParentValue->stValueItems[1].nValue; break;
        case 3:
		{
			int nValues[3] = {lpParentValue->stValueItems[0].nValue, lpParentValue->stValueItems[1].nValue, lpParentValue->stValueItems[2].nValue};
			g_pICstPubFunc->QuickSort(nValues, 3, QSVT_INT);
			nValue = nValues[1];
		}
	}
    
	if(bGetTail)
	{
		if(nValue < 0)
			nValue = 0 - nValue;
		nValue %= 10;
	}
	
	return nValue;
}

int CIndicatorParam::_GetMaxValue(LPCDTVALUE lpParentValue, BOOL bGetTail)
{
	ASSERT(lpParentValue != NULL);
	if(lpParentValue->stValueItems[0].nValue == CDTVALUE_INVALID)
	{
		return CDTVALUE_INVALID;
	}
	
	int nValue = lpParentValue->stValueItems[0].nValue;
	for(int i=1; i<lpParentValue->btItemCount; i++)
	{
		if(lpParentValue->stValueItems[i].nValue > nValue)
			nValue = lpParentValue->stValueItems[i].nValue;
	}
    
	if(bGetTail)
	{
		if(nValue < 0)
			nValue = 0 - nValue;
		nValue %= 10;
	}
	
	return nValue;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
int CIndicatorParam::_GetDistanceValue(ICondition *pICdtParent, int nValueIn, DWORD dwIssueIndex, DWORD dwIPID)
{
	ASSERT(pICdtParent != NULL);
    
	// 获取上一期指标的值以便计算距离
	DWORD dwPrevIssueIndex = g_pIData->GetPrevDataIndex(dwIssueIndex);	
	if(dwPrevIssueIndex == DATA_INDEX_INVALID)
	{
		return CDTVALUE_INVALID;
	}
	
	LPCDTVALUE lpPrevValue = pICdtParent->GetValueEx(NULL, dwPrevIssueIndex);
    
	ASSERT(lpPrevValue->btItemCount == 1);	// 确保不是多值
    
	if(lpPrevValue->stValueItems[0].nValue == CDTVALUE_INVALID)
	{
		return CDTVALUE_INVALID;
	}
    
	int nValue = abs(nValueIn - lpPrevValue->stValueItems[0].nValue);
    
	if(dwIPID == IPID_D_DISTANCE_TAIL)
		nValue %= 10;
    
	return nValue;
}

int CIndicatorParam::_GetCircleDistanceValue(ICondition *pICdtParent, int nValueIn, DWORD dwIssueIndex, DWORD dwIPID)
{
	ASSERT(pICdtParent != NULL);
	
	// 获取上一期指标的值以便计算环距
	DWORD dwPrevIssueIndex = g_pIData->GetPrevDataIndex(dwIssueIndex);	
	if(dwPrevIssueIndex == DATA_INDEX_INVALID)
	{
		return CDTVALUE_INVALID;
	}
    
	LPCDTVALUE lpPrevValue = pICdtParent->GetValueEx(NULL, dwPrevIssueIndex);
	
	ASSERT(lpPrevValue->btItemCount == 1);	// 确保不是多值
    
	if(lpPrevValue->stValueItems[0].nValue == CDTVALUE_INVALID)
	{
		return CDTVALUE_INVALID;
	}
    
	int nValue = (nValueIn - lpPrevValue->stValueItems[0].nValue);
	if(nValue < 0)
	{
		nValue += pICdtParent->GetValueRange()->nItemCount;
	}
	
	if(dwIPID == IPID_D_CIRCLE_DISTANCE_TAIL)
		nValue %= 10;
	
	return nValue;
}

int CIndicatorParam::_GetAdjacentValue(ICondition *pICdtParent, int nValueIn, DWORD dwIssueIndex, DWORD dwIPID)
{
	ASSERT(pICdtParent != NULL);
	
	// 获取上一期指标的值以便计算环距
	DWORD dwPrevIssueIndex = g_pIData->GetPrevDataIndex(dwIssueIndex);	
	if(dwPrevIssueIndex == DATA_INDEX_INVALID)
	{
		return CDTVALUE_INVALID;
	}
    
	LPCDTVALUE lpPrevValue = pICdtParent->GetValueEx(NULL, dwPrevIssueIndex);
	
	ASSERT(lpPrevValue->btItemCount == 1);	// 确保不是多值
    
	if(lpPrevValue->stValueItems[0].nValue == CDTVALUE_INVALID)
	{
		return CDTVALUE_INVALID;
	}
    
	int nValue = 0;
    
	if(dwIPID == IPID_D_ADJACENT_SUB)	// 邻期差
	{
		nValue = nValueIn - lpPrevValue->stValueItems[0].nValue;
	}
	else	// 邻期和（合）
	{
		nValue = nValueIn + lpPrevValue->stValueItems[0].nValue;
        
		if(dwIPID == IPID_D_ADJACENT_SUMTAIL)
			nValue %= 10;
	}
	
	return nValue;
}

int CIndicatorParam::_GetTestNumsXXXValue(DWORD dwIPID, ICondition *pICdtParent, int nValueIn, DWORD dwIssueIndex)
{
	// 获取试机号
	LPBYTE lpTestNums = g_pIData->GetItemTestNums(dwIssueIndex);
	if(lpTestNums == NULL)
	{
		return CDTVALUE_INVALID;	// 没有试机号，则返回无效条件值
	}
    
	ASSERT(!pICdtParent->IsMultipleValue());
    
	// 和开奖号的值进行比较
	int nValueTestNums = pICdtParent->GetValue(lpTestNums);
	int nRtnValue = 0;
    
	switch(dwIPID)
	{
        case IPID_D_TESTNUMS_COMPARE: if(nValueIn == nValueTestNums) nRtnValue = 1; break;
        case IPID_D_TESTNUMS_DISTANCE: nRtnValue = abs(nValueIn - nValueTestNums); break;
        case IPID_D_TESTNUMS_DISTANCE_TAIL: nRtnValue = abs(nValueIn - nValueTestNums) % 10; break;
        case IPID_D_TESTNUMS_SUB: nRtnValue = nValueIn - nValueTestNums; break;
        case IPID_D_TESTNUMS_CIRSUB: 
            nRtnValue = nValueIn - nValueTestNums; 
            if(nRtnValue < 0)
                nRtnValue += pICdtParent->GetValueRange()->nItemCount;
            break;
        case IPID_D_TESTNUMS_CIRSUB_TAIL:
            nRtnValue = nValueIn - nValueTestNums; 
            if(nRtnValue < 0)
                nRtnValue += pICdtParent->GetValueRange()->nItemCount;
            nRtnValue %= 10;
            break;
        default: ASSERT(FALSE); break;
	}
    
	return nRtnValue;
}

int CIndicatorParam::_GetSPJValue(ICondition *pICdtParent, int nValueIn, DWORD dwIssueIndex)
{
	ASSERT(pICdtParent != NULL);
	
	// 获取上一期指标的值以便计算
	DWORD dwPrevIssueIndex = g_pIData->GetPrevDataIndex(dwIssueIndex);	
	if(dwPrevIssueIndex == DATA_INDEX_INVALID)
	{
		return CDTVALUE_INVALID;
	}
	
	LPCDTVALUE lpPrevValue = pICdtParent->GetValueEx(NULL, dwPrevIssueIndex);
	
	ASSERT(lpPrevValue->btItemCount == 1);	// 确保不是多值
    
	if(lpPrevValue->stValueItems[0].nValue == CDTVALUE_INVALID)
	{
		return CDTVALUE_INVALID;
	}
	
	if(lpPrevValue->stValueItems[0].nValue < nValueIn)
		return 0;	// 升
	else if(lpPrevValue->stValueItems[0].nValue > nValueIn)
		return 2;	// 降
	else
		return 1;	// 平
}

int CIndicatorParam::_GetAxisValue(ICondition *pICdtParent, int nValueIn, DWORD dwIssueIndex, DWORD dwIPID)
{
	ASSERT(pICdtParent != NULL);
	
	// 获取上一期指标的值以便计算轴差
	DWORD dwPrevIssueIndex = g_pIData->GetPrevDataIndex(dwIssueIndex);	
	if(dwPrevIssueIndex == DATA_INDEX_INVALID)
	{
		return CDTVALUE_INVALID;
	}
	
	LPCDTVALUE lpPrevValue = pICdtParent->GetValueEx(NULL, dwPrevIssueIndex);
	
	ASSERT(lpPrevValue->btItemCount == 1);	// 确保不是多值
    
	if(lpPrevValue->stValueItems[0].nValue == CDTVALUE_INVALID)
	{
		return CDTVALUE_INVALID;
	}
    
	int nValue = nValueIn - lpPrevValue->stValueItems[0].nValue;
	if(nValue == 0)
	{
		return 0;
	}
    
	int nValue2 = 0, nValueAbs = 0, nValue2Abs = 0;
	if(nValue > 0)
	{
		nValueAbs = nValue;
		nValue2 = nValue - pICdtParent->GetValueRange()->nItemCount;
		nValue2Abs = 0 - nValue2;
	}
	else
	{
		nValueAbs = 0 - nValue;
		nValue2 = nValue + pICdtParent->GetValueRange()->nItemCount;
		nValue2Abs = nValue2;
	}
    
	switch(dwIPID)
	{
        case IPID_D_AXIS_SUB: return (nValueAbs <= nValue2Abs) ? nValue : nValue2; break;
        case IPID_D_AXIS_DISTANCE: return (nValueAbs <= nValue2Abs) ? nValueAbs : nValue2Abs; break;
        case IPID_D_AXIS_DISTANCE_TAIL: return ((nValueAbs <= nValue2Abs) ? nValueAbs : nValue2Abs) % 10; break;
	}
	
	ASSERT(FALSE);
	return 0;
}

int CIndicatorParam::_GetSkipValue(ICondition *pICdtParent, int nValueIn, DWORD dwIssueIndex, DWORD dwIPID)
{
	ASSERT(pICdtParent != NULL);
	
	DWORD dwPrevIssueIndex = g_pIData->GetPrevDataIndex(dwIssueIndex);	
    
	int i, nSkipValue = 0;
	LPCDTVALUE lpPrevValue = NULL;
	BOOL bSkipEnd = FALSE;
    
	// 从后往前查询当前值的遗漏
	while(dwPrevIssueIndex != DATA_INDEX_INVALID)
	{
		lpPrevValue = pICdtParent->GetValueEx(NULL, dwPrevIssueIndex);
		for(i=0; i<lpPrevValue->btItemCount; i++)
		{
			if(lpPrevValue->stValueItems[i].nValue == nValueIn)
			{
				bSkipEnd = TRUE;
				break;	// 遗漏结束
			}
		}
        
		if(bSkipEnd)
		{
			break;
		}
        
		nSkipValue ++;
		dwPrevIssueIndex = g_pIData->GetPrevDataIndex(dwPrevIssueIndex);
	}
    
	if(dwIPID == IPID_D_SKIP_TAIL)
		nSkipValue %= 10;
    
	return nSkipValue;
}

LPCDTVALUE CIndicatorParam::_GetCustomValue(const CDTIPID &cdtIPID, ICondition * /*pICdtParent*/, DWORD dwCustomData, LPCDTVALUE lpParentValue)
{
	ASSERT(cdtIPID.btType & CDTIPID_TYPE_CUSTOM);	// 自定义指标参数
    
	if(cdtIPID.btType & CDTIPID_TYPE_DATAAREA)		// 号码分区指标参数
	{
		LPDATAAREA lpDataArea = (LPDATAAREA)dwCustomData;
		ASSERT(lpDataArea != NULL);
		int nValue = 0;
		for(int i=0; i<lpParentValue->btItemCount; i++)
		{
			ASSERT(lpParentValue->stValueItems[i].nValue >= g_pIData->GetMinNumber() && lpParentValue->stValueItems[i].nValue <= g_pIData->GetMaxNumber());
			nValue = lpDataArea->btSubAreaValues[lpParentValue->stValueItems[i].nValue];
			_AddValue(nValue, lpParentValue->stValueItems[i].btCount);
		}
	}
	else
	{
		LPCIP_HEAD lpCustomIP = (LPCIP_HEAD)dwCustomData;
		switch(lpCustomIP->wFlag)
		{
            case CIPHF_AREA:
			{
				LPCIP_CUSTOMAREA lpCustomArea = (LPCIP_CUSTOMAREA)lpCustomIP->lpDataBuf;
				ASSERT(lpCustomArea != NULL);
				int nValue = 0;
				for(int i=0; i<lpParentValue->btItemCount; i++)
				{
					ASSERT(lpParentValue->stValueItems[i].nValue >= lpCustomArea->nMinValue && lpParentValue->stValueItems[i].nValue <= lpCustomArea->nMaxValue);
					nValue = lpCustomArea->GetSubAreaIndex(lpParentValue->stValueItems[i].nValue);
					_AddValue(nValue, lpParentValue->stValueItems[i].btCount);
				}
			} break;
            case CIPHF_REMAINDER:
			{
				LPCIP_CUSTOMREMAINDER lpCustomRem = (LPCIP_CUSTOMREMAINDER)lpCustomIP->lpDataBuf;
				ASSERT(lpCustomRem != NULL);
				for(int i=0; i<lpParentValue->btItemCount; i++)
				{
					ASSERT(lpParentValue->stValueItems[i].nValue >= lpCustomRem->nMinValue && lpParentValue->stValueItems[i].nValue <= lpCustomRem->nMaxValue);
					_AddValue(lpParentValue->stValueItems[i].nValue % lpCustomRem->nDivisor, lpParentValue->stValueItems[i].btCount);
				}
			} break;
            default: ASSERT(FALSE); break;
		}
	}
    
	return m_lpValueTemp;
}
