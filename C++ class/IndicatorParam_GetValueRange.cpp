//
//  IndicatorParam_GetValueRange.cpp
//  QCLottery
//
//  Created by 武 孙 on 12-5-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "IndicatorParam.h"

BOOL CIndicatorParam::GetValueRange(const CDTIPID &cdtIPID, ICondition *pICdtParent, DWORD dwCustomData, LPCDTVALUERANGE *lplpValueRange)
{
	ASSERT((*lplpValueRange) == NULL);
    
	m_lpValueRangeTemp->nItemCount = -1;			// 初始化
    
	if(cdtIPID.btType & CDTIPID_TYPE_CUSTOM)	// 自定义指标参数
	{
		if(cdtIPID.btType & CDTIPID_TYPE_DATAAREA)	// 号码分区指标参数
		{
			LPDATAAREA lpDataArea = (LPDATAAREA)dwCustomData;
			ASSERT(lpDataArea != NULL);
			_FillValueRange_DataArea(lpDataArea);
		}
		else
		{
			LPCIP_HEAD lpCustomIP = (LPCIP_HEAD)dwCustomData;
			ASSERT(lpCustomIP != NULL);
			_FillValueRange_CustomIP(lpCustomIP, pICdtParent);
		}
	}
	else	// 常规指标参数
	{
		switch(cdtIPID.dwSystemIPID)
		{
            case IPID_NONE: return FALSE;
            case IPID_S_ODDEVEN: 
			{
				BEGIN_FILLVALUERANGE(2)
                ADD_VALUERANGEITEM(1, "奇数", "奇", "J")
                ADD_VALUERANGEITEM(0, "偶数", "偶", "O")
				END_FILLVALUERANGE()
			} break;
                
            case IPID_S_BIGSMALL:
			{
				BEGIN_FILLVALUERANGE(2)
                ADD_VALUERANGEITEM(1, "大数", "大", "D")
                ADD_VALUERANGEITEM(0, "小数", "小", "X")
				END_FILLVALUERANGE()
			} break;
                
            case IPID_S_DZX:
			{
				BEGIN_FILLVALUERANGE(3)
                ADD_VALUERANGEITEM(0, "小数", "X", "X")
                ADD_VALUERANGEITEM(1, "中数", "Z", "Z")
                ADD_VALUERANGEITEM(2, "大数", "D", "D")
				END_FILLVALUERANGE()
			} break;
                
            case IPID_S_012:
			{
				BEGIN_FILLVALUERANGE(3)
                ADD_VALUERANGEITEM(0, "0路", "0", "0")
                ADD_VALUERANGEITEM(1, "1路", "1", "1")
                ADD_VALUERANGEITEM(2, "2路", "2", "2")
				END_FILLVALUERANGE()
			} break;
                
            case IPID_S_PRIMECOMPOSITE:
			{
				BEGIN_FILLVALUERANGE(2)
                ADD_VALUERANGEITEM(1, "质数", "质", "Z")
                ADD_VALUERANGEITEM(0, "合数", "合", "H")
				END_FILLVALUERANGE()
			} break;
            case IPID_S_TAIL: _FillValueRange_Tail("尾数"); break;
            case IPID_S_ABS: _FillValueRange_Abs(pICdtParent); break;
            case IPID_S_ABS_TAIL: _FillValueRange_Tail("绝对值尾数"); break;
            case IPID_S_SUM:
			{
				int nMaxCountOfValue = 0;
				pICdtParent->IsMultipleValue(&nMaxCountOfValue);
				_AutoFillValueRange_Default(pICdtParent->GetValueRange()->nMinValue * nMaxCountOfValue, pICdtParent->GetValueRange()->nMaxValue * nMaxCountOfValue); break;
			} break;
            case IPID_S_SUM_TAIL: _FillValueRange_Tail("合值"); break;
            case IPID_S_SUM_N:
			{
				int nMaxCountOfValue = 0;
				pICdtParent->IsMultipleValue(&nMaxCountOfValue);
				int nMaxValue = pICdtParent->GetValueRange()->nMaxValue * nMaxCountOfValue;
				int nMinValue = pICdtParent->GetValueRange()->nMinValue;
				if(nMaxValue > 9)
				{
					nMaxValue = 9;
					if(nMinValue > 1) nMinValue = 1;
				}
				_AutoFillValueRange_Default(nMinValue, nMaxValue);
			}break;
            case IPID_S_SPAN: _AutoFillValueRange_Default(0, pICdtParent->GetValueRange()->nMaxValue - pICdtParent->GetValueRange()->nMinValue); break;
            case IPID_S_SPAN_TAIL: _FillValueRange_Tail("跨度尾数"); break;
            case IPID_S_AVG:
            case IPID_S_MIN:
            case IPID_S_MID:
            case IPID_S_MAX: _AutoFillValueRange_Default(pICdtParent->GetValueRange()->nMinValue, pICdtParent->GetValueRange()->nMaxValue); break;
            case IPID_S_AVG_TAIL: _FillValueRange_Tail("均值尾数"); break;
            case IPID_S_MIN_TAIL: _FillValueRange_Tail("最小值尾数"); break;
            case IPID_S_MID_TAIL: _FillValueRange_Tail("中间值尾数"); break;
            case IPID_S_MAX_TAIL: _FillValueRange_Tail("最大值尾数"); break;
            case IPID_D_DISTANCE:
			{
				_GetValueRange_Distance(pICdtParent, FALSE);
			} break;
            case IPID_D_DISTANCE_TAIL:
			{
				_FillValueRange_Tail("距离尾");
			} break;
            case IPID_D_CIRCLE_DISTANCE:
			{
				_GetValueRange_Distance(pICdtParent, TRUE);
			} break;
            case IPID_D_ADJACENT_SUB:
			{
				_GetValueRange_Adjacent(pICdtParent, FALSE);
			} break;
            case IPID_D_ADJACENT_SUM:
			{
				_GetValueRange_Adjacent(pICdtParent, TRUE);
			} break;
            case IPID_D_CIRCLE_DISTANCE_TAIL:
			{
				_FillValueRange_Tail("环距尾");
			} break;
            case IPID_D_ADJACENT_SUMTAIL:
			{
				_FillValueRange_Tail("邻期合");
			} break;
            case IPID_D_SPJ:
			{
				BEGIN_FILLVALUERANGE(3)
                ADD_VALUERANGEITEM(0, "升", "S", "S")
                ADD_VALUERANGEITEM(1, "平", "P", "P")
                ADD_VALUERANGEITEM(2, "降", "J", "J")
				END_FILLVALUERANGE()
			} break;
                
            case IPID_D_AXIS_SUB:
            case IPID_D_AXIS_DISTANCE: _GetValueRange_Axis(pICdtParent, cdtIPID.dwSystemIPID); break;
            case IPID_D_AXIS_DISTANCE_TAIL: _FillValueRange_Tail("轴距尾"); break;
                
            case IPID_D_SKIP: _AutoFillValueRange_LinkNoLimit(0); break;
            case IPID_D_SKIP_TAIL: _FillValueRange_Tail("遗漏尾"); break;
            case IPID_D_TESTNUMS_COMPARE:
			{
				BEGIN_FILLVALUERANGE(2)
                ADD_VALUERANGEITEM(0, "不同", "0", "0")
                ADD_VALUERANGEITEM(1, "相同", "1", "1")
				END_FILLVALUERANGE()
			} break;
            case IPID_D_TESTNUMS_DISTANCE: _GetValueRange_Distance(pICdtParent, FALSE); break;
            case IPID_D_TESTNUMS_DISTANCE_TAIL: _FillValueRange_Tail("试机距离尾"); break;
            case IPID_D_TESTNUMS_SUB: _GetValueRange_Adjacent(pICdtParent, FALSE); break;
            case IPID_D_TESTNUMS_CIRSUB: _GetValueRange_Distance(pICdtParent, TRUE); break;
            case IPID_D_TESTNUMS_CIRSUB_TAIL: _FillValueRange_Tail("试机环距尾"); break;
                
            default:
			{
				if(cdtIPID.dwSystemIPID >= IPID_S_REMAINDER_0 && cdtIPID.dwSystemIPID <= IPID_S_REMAINDER_MAX)
				{
					// 指标参数为余数
					_GetValueRange_Remainder(pICdtParent, cdtIPID.dwSystemIPID - IPID_S_REMAINDER_0);
				} 
				else if(cdtIPID.dwSystemIPID >= IPID_S_SUBAREA_0 && cdtIPID.dwSystemIPID <= IPID_S_SUBAREA_MAX)
				{
					// 指标参数为分区
					_FillValueRange_SubArea(cdtIPID.dwSystemIPID - IPID_S_SUBAREA_0);
				}
				else
				{
					DEBUGOUT("Error: In CIndicatorParam::GetValueRange, cdtIPID.dwSystemIPID=0X%02X not found!!!", cdtIPID.dwSystemIPID);
				}
			} break;
		}
	}
	
	ASSERT(m_lpValueRangeTemp->nItemCount != -1);	// 确保正确获取了
    
	int i = 0, nValue = 0;
	
#ifdef _DEBUG
	// 检查越界
	for(i=0; i<m_lpValueRangeTemp->nItemCount; i++)
	{
		CHECK_STRING_STACK(m_lpValueRangeTemp->stValueItem[i].szBallName);
		CHECK_STRING_STACK(m_lpValueRangeTemp->stValueItem[i].szColName);
		CHECK_STRING_STACK(m_lpValueRangeTemp->stValueItem[i].szValueName);
	}
#endif
	
	// 设置最大和最小值
	m_lpValueRangeTemp->nMinValue = m_lpValueRangeTemp->nMaxValue = m_lpValueRangeTemp->stValueItem[0].nValue;
	for(i=1; i<m_lpValueRangeTemp->nItemCount; i++)
	{
		nValue = m_lpValueRangeTemp->stValueItem[i].nValue;
		if(m_lpValueRangeTemp->nMinValue > nValue)
			m_lpValueRangeTemp->nMinValue = nValue;
		if(m_lpValueRangeTemp->nMaxValue < nValue)
			m_lpValueRangeTemp->nMaxValue = nValue;
	}
	
	int nBufLen = sizeof(CDTVALUERANGE) + sizeof(CDTVALUERANGEITEM) * m_lpValueRangeTemp->nItemCount;
	(*lplpValueRange) = (LPCDTVALUERANGE)new BYTE[nBufLen];
	memcpy((*lplpValueRange), m_lpValueRangeTemp, nBufLen);
	
	// 返回
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
void CIndicatorParam::_SetValueRangeItemValue(int nValueIndex, int nValue, LPCSTR lpszColName, LPCSTR lpszBallName, LPCSTR lpszValueName)
{
	ASSERT(nValueIndex >= 0 && nValueIndex < m_lpValueRangeTemp->nItemCount);
	
	m_lpValueRangeTemp->stValueItem[nValueIndex].nValue = nValue;
	if(lpszColName == NULL)
	{
		if(nValue > -10 && nValue < 100)
			sprintf(m_lpValueRangeTemp->stValueItem[nValueIndex].szColName, "%d", nValue);
		else
			m_lpValueRangeTemp->stValueItem[nValueIndex].szColName[0] = 0;
	}
	else
	{
		strcpy(m_lpValueRangeTemp->stValueItem[nValueIndex].szColName, lpszColName);
	}
	CHECK_STRING_STACK(m_lpValueRangeTemp->stValueItem[nValueIndex].szColName);
	
	if(lpszBallName == NULL)
	{
		strcpy(m_lpValueRangeTemp->stValueItem[nValueIndex].szBallName, m_lpValueRangeTemp->stValueItem[nValueIndex].szColName);
	}
	else
	{
		strcpy(m_lpValueRangeTemp->stValueItem[nValueIndex].szBallName, lpszBallName);
		CHECK_STRING_STACK(m_lpValueRangeTemp->stValueItem[nValueIndex].szBallName);
	}
	
	if(lpszValueName == NULL)
	{
		sprintf(m_lpValueRangeTemp->stValueItem[nValueIndex].szValueName, "%d", nValue);
	}
	else
	{
		strcpy(m_lpValueRangeTemp->stValueItem[nValueIndex].szValueName, lpszValueName);
		CHECK_STRING_STACK(m_lpValueRangeTemp->stValueItem[nValueIndex].szValueName);
	}
}

void CIndicatorParam::_AutoFillValueRange_Default(int nMinValue, int nMaxValue)
{
	m_lpValueRangeTemp->nItemCount = nMaxValue - nMinValue + 1;
	ASSERT(m_lpValueRangeTemp->nItemCount > 0);
    
	for(int i=0; i<m_lpValueRangeTemp->nItemCount; i++)
	{
		m_lpValueRangeTemp->stValueItem[i].nValue = i + nMinValue;
		sprintf(m_lpValueRangeTemp->stValueItem[i].szValueName, "%d", m_lpValueRangeTemp->stValueItem[i].nValue);
		if(strlen(m_lpValueRangeTemp->stValueItem[i].szValueName) < sizeof(m_lpValueRangeTemp->stValueItem[i].szBallName))
		{
			strcpy(m_lpValueRangeTemp->stValueItem[i].szBallName, m_lpValueRangeTemp->stValueItem[i].szValueName);
			strcpy(m_lpValueRangeTemp->stValueItem[i].szColName, m_lpValueRangeTemp->stValueItem[i].szValueName);
		}
		else
		{
			m_lpValueRangeTemp->stValueItem[i].szBallName[0] = m_lpValueRangeTemp->stValueItem[i].szColName[0] = 0;
		}
	}
}

void CIndicatorParam::_AutoFillValueRange_LinkNoLimit(int nMinValue)
{	
	m_lpValueRangeTemp->nItemCount = 1;	
	
	// 无上限的值，则值域有一个元素保存最小值
	ZeroMemory(m_lpValueRangeTemp->stValueItem, m_lpValueRangeTemp->nItemCount * sizeof(CDTVALUERANGEITEM));
	
	_SetValueRangeItemValue(0, nMinValue);
}

void CIndicatorParam::_FillValueRange_Tail(LPCSTR /*lpszName*/)
{
	char szBall[8] = {0};
	
	BEGIN_FILLVALUERANGE(10)
	for(int i=0; i<10; i++)
	{
		sprintf(szBall, "%d", i);
		ADD_VALUERANGEITEM(i, szBall, szBall, szBall);
	}
	END_FILLVALUERANGE()
}

void CIndicatorParam::_FillValueRange_Abs(ICondition *pICdtParent)								
{
	ASSERT(pICdtParent != NULL);
	
	// 获取父条件的值域
	LPCDTVALUERANGE lpParentValueRange = pICdtParent->GetValueRange();
	
	// 确保父条件的值域存在并且符合条件
	if(lpParentValueRange == NULL)
	{
		return ;
	}
	
	int nMaxValue = max(abs(lpParentValueRange->nMinValue), abs(lpParentValueRange->nMaxValue));
    /*	CDWordArray cAbsValueArray;
     cAbsValueArray.SetSize(nMaxValue + 1);
     for(int i=0; i<lpParentValueRange->nItemCount; i++)
     {
     cAbsValueArray.SetAt(abs(lpParentValueRange->stValueItem[i].nValue), 1);
     }
     */
	// 重新设置值域信息
	int nIndex = 0;
	for(int i=0; i<=nMaxValue; i++)
	{
        //		if(cAbsValueArray[i] == 1)
		{
			m_lpValueRangeTemp->stValueItem[nIndex].nValue = i;
			sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szBallName, "%d", i);
			strcpy(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, m_lpValueRangeTemp->stValueItem[nIndex].szBallName);
			strcpy(m_lpValueRangeTemp->stValueItem[nIndex].szColName, m_lpValueRangeTemp->stValueItem[nIndex].szBallName);
			nIndex ++;
		}
	}
	m_lpValueRangeTemp->nItemCount = nIndex;
}

void CIndicatorParam::_FillValueRange_SubArea(int nAreaCount)
{
	ASSERT(nAreaCount > 1);
	
	char szTemp[32] = {0}, szBall[8] = {0};
	
	BEGIN_FILLVALUERANGE(nAreaCount)
	for(int i=1; i<=nAreaCount; i++)
	{
		sprintf(szTemp, "%d分区%d", nAreaCount, i);
		sprintf(szBall, "%d", i);
		ADD_VALUERANGEITEM(i, szTemp, szBall, szBall);
	}
	END_FILLVALUERANGE()
}

void CIndicatorParam::_FillValueRange_CustomIP(LPCIP_HEAD lpCustomIP, ICondition *pICdtParent)
{	
	ASSERT(lpCustomIP != NULL);
    
	char szBall[8] = {0};
    
	switch(lpCustomIP->wFlag)
	{
        case CIPHF_AREA: 
		{
			LPCIP_CUSTOMAREA lpCustomArea = (LPCIP_CUSTOMAREA)lpCustomIP->lpDataBuf;
			BEGIN_FILLVALUERANGE(lpCustomArea->wAreaCount)
			for(int i=0; i<lpCustomArea->wAreaCount; i++)
			{
				sprintf(szBall, "%d", i + lpCustomArea->btSubAreaMinValue);
				ADD_VALUERANGEITEM(i + lpCustomArea->btSubAreaMinValue, lpCustomArea->GetSubAreaName(i), szBall, szBall);
			}
			END_FILLVALUERANGE()
		} break;
        case CIPHF_REMAINDER:
		{
			LPCIP_CUSTOMREMAINDER lpCustomRem = (LPCIP_CUSTOMREMAINDER)lpCustomIP->lpDataBuf;
			_GetValueRange_Remainder(pICdtParent, lpCustomRem->nDivisor);
		} break;
        default: ASSERT(FALSE); break;
	}
}

void CIndicatorParam::_FillValueRange_DataArea(LPDATAAREA lpDataArea)
{
	ASSERT(lpDataArea != NULL);
    
	char szBall[8] = {0};
	
	BEGIN_FILLVALUERANGE(lpDataArea->btAreaCount)
	for(int i=0; i<lpDataArea->btAreaCount; i++)
	{
		sprintf(szBall, "%d", i + lpDataArea->btSubAreaMinValue);
		ADD_VALUERANGEITEM(i + lpDataArea->btSubAreaMinValue, lpDataArea->szSubAreaNames[i], szBall, szBall);
	}
	END_FILLVALUERANGE()
}

BOOL CIndicatorParam::_GetValueRange_Remainder(ICondition *pICdtParent, int nDivisor)
{
	// 获取取余数前条件的值域
	LPCDTVALUERANGE lpParentValueRange = pICdtParent->GetValueRange();
	
	// 确保取余数前的值域存在并且符合条件
	if(lpParentValueRange == NULL)
	{
		return FALSE;
	}
    
	// 计算最大最小值
	int nMax = 0, nMin = 0;
    
	if(pICdtParent->GetValueRangeType() & CDTRT_NOLIMIT)
	{
		nMax = nDivisor - 1;
        //		nMin = 1 - nDivisor;
	}
	else
	{
		nMax = lpParentValueRange->nMaxValue % nDivisor;
		nMin = lpParentValueRange->nMinValue % nDivisor;
		int nTemp = 0;
		for(int i=lpParentValueRange->nMinValue; i<=lpParentValueRange->nMaxValue; i++)
		{
			nTemp = i % nDivisor;
			if(nTemp > nMax) nMax = nTemp;
			if(nTemp < nMin) nMin = nTemp;
		}
	}
	
	m_lpValueRangeTemp->nItemCount = nMax - nMin + 1;
	ASSERT(m_lpValueRangeTemp->nItemCount > 0);
	
	for(int i=0; i<m_lpValueRangeTemp->nItemCount; i++)
	{
		m_lpValueRangeTemp->stValueItem[i].nValue = i + nMin;
        //		sprintf(m_lpValueRangeTemp->stValueItem[i].szValueName, "除%d余%d的数", nDivisor, m_lpValueRangeTemp->stValueItem[i].nValue);
		if(abs(m_lpValueRangeTemp->stValueItem[i].nValue) < 100)
		{
			sprintf(m_lpValueRangeTemp->stValueItem[i].szBallName, "%d", m_lpValueRangeTemp->stValueItem[i].nValue);
			strcpy(m_lpValueRangeTemp->stValueItem[i].szColName, m_lpValueRangeTemp->stValueItem[i].szBallName);
			strcpy(m_lpValueRangeTemp->stValueItem[i].szValueName, m_lpValueRangeTemp->stValueItem[i].szBallName);
		}
		else
		{
			m_lpValueRangeTemp->stValueItem[i].szBallName[0] = m_lpValueRangeTemp->stValueItem[i].szColName[0] = 0;
		}
	}
    
	return TRUE;
}

BOOL CIndicatorParam::_GetValueRange_Distance(ICondition *pICdtParent, BOOL /*bCircle*/)
{
	ASSERT(pICdtParent != NULL);
    
	// 获取取距离前条件的值域
	LPCDTVALUERANGE lpParentValueRange = pICdtParent->GetValueRange();
    
	// 确保取距离前的值域存在并且符合条件
	if(lpParentValueRange == NULL)
	{
		return FALSE;
	}
    
	// 计算距离的最大值
	int nMaxDis = abs(lpParentValueRange->nMaxValue - lpParentValueRange->nMinValue);
    
	// 重新设置值域信息
	_AutoFillValueRange_Default(0, nMaxDis);
    
	return TRUE;
}

BOOL CIndicatorParam::_GetValueRange_Adjacent(ICondition *pICdtParent, BOOL bSum)
{
	ASSERT(pICdtParent != NULL);
	
	// 获取取值前条件的值域
	LPCDTVALUERANGE lpParentValueRange = pICdtParent->GetValueRange();
	
	// 确保取值前的值域存在并且符合条件
	if(lpParentValueRange == NULL)
	{
		return FALSE;
	}
	
	// 计算最大值
	int nMinValue = bSum ? (lpParentValueRange->nMinValue * 2) : (lpParentValueRange->nMinValue - lpParentValueRange->nMaxValue);
	int nMaxValue = bSum ? (lpParentValueRange->nMaxValue * 2) : (lpParentValueRange->nMaxValue - lpParentValueRange->nMinValue);
	
	// 重新设置值域信息
	_AutoFillValueRange_Default(nMinValue, nMaxValue);
	
	return TRUE;
}

BOOL CIndicatorParam::_GetValueRange_Axis(ICondition *pICdtParent, DWORD dwIPID)
{
	ASSERT(pICdtParent != NULL);
	
	// 获取取轴差（距）前条件的值域
	LPCDTVALUERANGE lpParentValueRange = pICdtParent->GetValueRange();
	
	// 确保取轴差（距）前的值域存在并且符合条件
	if(lpParentValueRange == NULL)
	{
		return FALSE;
	}
	
	// 计算轴差（距）的最大值
	int nMaxValue = lpParentValueRange->nItemCount / 2, nMinValue = 0;
	if(dwIPID == IPID_D_AXIS_SUB)	// 轴差
	{
		nMinValue = 0 - nMaxValue;
	}
	
	// 重新设置值域信息
	_AutoFillValueRange_Default(nMinValue, nMaxValue);
	
	return TRUE;
}

