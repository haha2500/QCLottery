//
//  IndicatorParam.cpp
//  QCLottery
//
//  Created by 武 孙 on 12-5-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "IndicatorParam.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIndicatorParam::CIndicatorParam()
{
	m_bFirstParam = TRUE;
	ZeroMemory(m_szNameTemp, sizeof(m_szNameTemp));
	ZeroMemory(m_szExplain, sizeof(m_szExplain));
	
	// 初始化最大个数的值域空间
	m_lpValueRangeTemp = LPCDTVALUERANGE(new BYTE[sizeof(CDTVALUERANGE) + sizeof(CDTVALUERANGEITEM) * (MAXCOUNT_VALUEITEM)]);
	// 初始化值指针空间
	m_lpValueTemp = LPCDTVALUE(new BYTE[sizeof(CDTVALUE) + sizeof(CDTVALUEITEM) * MAXBYTE]);
}

CIndicatorParam::~CIndicatorParam()
{
	if(m_lpValueRangeTemp != NULL)
	{
		delete []((LPBYTE)m_lpValueRangeTemp);
		m_lpValueRangeTemp = NULL;
	}
	
	if(m_lpValueTemp != NULL)
	{
		delete []((LPBYTE)m_lpValueTemp);
		m_lpValueTemp = NULL;
	}
}

//////////////////////////////////////////////////////////////

BOOL CIndicatorParam::Initialize(BOOL bFirstParam)
{
	m_bFirstParam = bFirstParam;
    
	return TRUE;
}

BOOL CIndicatorParam::IsValid(const CDTIPID &cdtIPID)
{
	UNUSED_ALWAYS(cdtIPID);
    
    
#ifndef _PROFESSIONAL	// 标准版
	if(!m_bFirstParam)
	{
		ASSERT(FALSE);
		return FALSE;	// 不支持二级扩展 
	}
    
	if(cdtIPID.btType & CDTIPID_TYPE_CUSTOM)	// 自定义指标参数
	{
		return FALSE;
	}
    
	if(cdtIPID.dwSystemIPID >= IPID_S_SUBAREA_0 && cdtIPID.dwSystemIPID <= IPID_S_SUBAREA_MAX)
	{
		return FALSE;	// 不支持自定义分区
	}
    
	if(cdtIPID.dwSystemIPID >= IPID_D_CIRCLE_DISTANCE && cdtIPID.dwSystemIPID <= IPID_D_AXIS_DISTANCE_TAIL)	
	{
		return FALSE;	// 不支持环距等
	}
    
	if(cdtIPID.dwSystemIPID >= IPID_D_TESTNUMS_SUB && cdtIPID.dwSystemIPID <= IPID_D_TESTNUMS_CIRSUB_TAIL)	
	{
		return FALSE;	// 不支持试机号差等
	}
#else
#endif
    
	return TRUE;
}

BYTE CIndicatorParam::GetValueRangeType(const CDTIPID &cdtIPID, DWORD /*dwCustomData*/)
{
	if(cdtIPID.btType & (CDTIPID_TYPE_DATAAREA | CDTIPID_TYPE_CUSTOM))	// 自定义分区指标参数
	{
		return CDTRT_DEFAULT;
	}
	else	// 常规指标参数
	{
		switch(cdtIPID.dwSystemIPID)
		{
            case IPID_S_ODDEVEN: 
            case IPID_S_BIGSMALL:
            case IPID_S_PRIMECOMPOSITE:
                return CDTRT_ORDER_DESC;
                
            case IPID_S_012:
            case IPID_S_DZX:		
            case IPID_S_TAIL:
            case IPID_S_ABS:
            case IPID_S_ABS_TAIL:
            case IPID_S_SUM:
            case IPID_S_SUM_TAIL:
            case IPID_S_SUM_N:
            case IPID_S_SPAN:
            case IPID_S_SPAN_TAIL:
            case IPID_S_AVG:
            case IPID_S_AVG_TAIL:
            case IPID_S_MIN:
            case IPID_S_MIN_TAIL:
            case IPID_S_MID:
            case IPID_S_MID_TAIL:
            case IPID_S_MAX:
            case IPID_S_MAX_TAIL:
            case IPID_D_DISTANCE:
            case IPID_D_DISTANCE_TAIL:
            case IPID_D_CIRCLE_DISTANCE:
            case IPID_D_CIRCLE_DISTANCE_TAIL:
            case IPID_D_ADJACENT_SUB:
            case IPID_D_ADJACENT_SUM:
            case IPID_D_ADJACENT_SUMTAIL:
            case IPID_D_SPJ:
            case IPID_D_AXIS_SUB:
            case IPID_D_AXIS_DISTANCE:
            case IPID_D_AXIS_DISTANCE_TAIL:
            case IPID_D_SKIP_TAIL:
                return CDTRT_DEFAULT;
                
            case IPID_D_TESTNUMS_COMPARE:
            case IPID_D_TESTNUMS_DISTANCE:
            case IPID_D_TESTNUMS_DISTANCE_TAIL:
            case IPID_D_TESTNUMS_SUB:
            case IPID_D_TESTNUMS_CIRSUB:
            case IPID_D_TESTNUMS_CIRSUB_TAIL:
                return (g_pIData->GetLotteryProperty()->btProperty & CSTLPP_TESTNUMS) ? CDTRT_DEFAULT : CDTRT_NOTSUPPORT;
                
            case IPID_D_SKIP:
                return CDTRT_NOLIMIT;
                
            default:
			{
				if(cdtIPID.dwSystemIPID >= IPID_S_REMAINDER_0 && cdtIPID.dwSystemIPID <= IPID_S_REMAINDER_MAX)
				{
					// 指标参数为余数
					return CDTRT_DEFAULT;
				} 
				else if(cdtIPID.dwSystemIPID >= IPID_S_SUBAREA_0 && cdtIPID.dwSystemIPID <= IPID_S_SUBAREA_MAX)
				{
					// 指标参数为分区
					return CDTRT_DEFAULT;
				}
				else
				{
					DEBUGOUT("Error: In CIndicatorParam::GetValueRangeType, dwSystemIPID=0X%08X not found!!!", cdtIPID.dwSystemIPID);
				}
			} break;
		}
	}
    
	ASSERT(FALSE);
	return 0;
}

BYTE CIndicatorParam::GetValueType(const CDTIPID &cdtIPID, DWORD /*dwCustomData*/)
{
	if(cdtIPID.btType & (CDTIPID_TYPE_DATAAREA | CDTIPID_TYPE_CUSTOM))	// 自定义分区指标参数
	{
		return CDTVT_DIGITAL;
	}
	else	// 常规指标参数
	{	
		switch(cdtIPID.dwSystemIPID)
		{
            case IPID_S_ODDEVEN: 
            case IPID_S_BIGSMALL:
            case IPID_S_DZX:		
            case IPID_S_012:
            case IPID_S_PRIMECOMPOSITE:
            case IPID_S_TAIL:
            case IPID_S_ABS:
            case IPID_S_ABS_TAIL:
            case IPID_S_SUM:
            case IPID_S_SUM_TAIL:
            case IPID_S_SUM_N:
            case IPID_S_SPAN:
            case IPID_S_SPAN_TAIL:
            case IPID_S_AVG:
            case IPID_S_AVG_TAIL:
            case IPID_S_MIN:
            case IPID_S_MIN_TAIL:
            case IPID_S_MID:
            case IPID_S_MID_TAIL:
            case IPID_S_MAX:
            case IPID_S_MAX_TAIL:
            case IPID_D_DISTANCE:
            case IPID_D_DISTANCE_TAIL:
            case IPID_D_CIRCLE_DISTANCE:
            case IPID_D_CIRCLE_DISTANCE_TAIL:
            case IPID_D_ADJACENT_SUB:
            case IPID_D_ADJACENT_SUM:
            case IPID_D_ADJACENT_SUMTAIL:
            case IPID_D_SPJ:
            case IPID_D_AXIS_SUB:
            case IPID_D_AXIS_DISTANCE:
            case IPID_D_AXIS_DISTANCE_TAIL:
            case IPID_D_SKIP:
            case IPID_D_SKIP_TAIL:		
            case IPID_D_TESTNUMS_COMPARE:
            case IPID_D_TESTNUMS_DISTANCE:
            case IPID_D_TESTNUMS_DISTANCE_TAIL:
            case IPID_D_TESTNUMS_SUB:
            case IPID_D_TESTNUMS_CIRSUB:
            case IPID_D_TESTNUMS_CIRSUB_TAIL:			
                return CDTVT_DIGITAL;
                
            default:
			{
				if(cdtIPID.dwSystemIPID >= IPID_S_REMAINDER_0 && cdtIPID.dwSystemIPID <= IPID_S_REMAINDER_MAX)
				{
					// 指标参数为余数
					return CDTVT_DIGITAL;
				} 
				else if(cdtIPID.dwSystemIPID >= IPID_S_SUBAREA_0 && cdtIPID.dwSystemIPID <= IPID_S_SUBAREA_MAX)
				{
					// 指标参数为分区
					return CDTVT_DIGITAL;
				}
				else
				{
					DEBUGOUT("Error: In CIndicatorParam::GetValueType, dwSystemIPID=0X%08X not found!!!", cdtIPID.dwSystemIPID);
				}
			} break;
		}
	}
	
	ASSERT(FALSE);
	return 0;
}

BYTE CIndicatorParam::GetInputType(const CDTIPID &cdtIPID, DWORD /*dwCustomData*/)
{
	if(cdtIPID.btType & (CDTIPID_TYPE_DATAAREA | CDTIPID_TYPE_CUSTOM))	// 自定义分区指标参数
	{
		return CDTIT_EDIT_SINGLELINE;
	}
	else	// 常规指标参数
	{
		switch(cdtIPID.dwSystemIPID)
		{
            case IPID_S_ODDEVEN: 
            case IPID_S_BIGSMALL:
            case IPID_S_DZX:		
            case IPID_S_012:
            case IPID_S_PRIMECOMPOSITE:
            case IPID_D_TESTNUMS_COMPARE:
            case IPID_D_SPJ:
                return CDTIT_CHECKBOX;
                
            case IPID_S_TAIL:
            case IPID_S_ABS:
            case IPID_S_ABS_TAIL:
            case IPID_S_SUM:
            case IPID_S_SUM_TAIL:
            case IPID_S_SUM_N:
            case IPID_S_SPAN:
            case IPID_S_SPAN_TAIL:
            case IPID_S_AVG:
            case IPID_S_AVG_TAIL:
            case IPID_S_MIN:
            case IPID_S_MIN_TAIL:
            case IPID_S_MID:
            case IPID_S_MID_TAIL:
            case IPID_S_MAX:
            case IPID_S_MAX_TAIL:
            case IPID_D_DISTANCE:
            case IPID_D_DISTANCE_TAIL:
            case IPID_D_CIRCLE_DISTANCE:
            case IPID_D_CIRCLE_DISTANCE_TAIL:
            case IPID_D_ADJACENT_SUB:
            case IPID_D_ADJACENT_SUM:
            case IPID_D_ADJACENT_SUMTAIL:
            case IPID_D_AXIS_SUB:
            case IPID_D_AXIS_DISTANCE:
            case IPID_D_AXIS_DISTANCE_TAIL:
            case IPID_D_SKIP:
            case IPID_D_SKIP_TAIL:		
            case IPID_D_TESTNUMS_DISTANCE:
            case IPID_D_TESTNUMS_DISTANCE_TAIL:
            case IPID_D_TESTNUMS_SUB:
            case IPID_D_TESTNUMS_CIRSUB:
            case IPID_D_TESTNUMS_CIRSUB_TAIL:			
                return CDTIT_EDIT_SINGLELINE;
                
            default:
			{
				if(cdtIPID.dwSystemIPID >= IPID_S_REMAINDER_0 && cdtIPID.dwSystemIPID <= IPID_S_REMAINDER_MAX)
				{
					// 指标参数为余数
					return CDTIT_EDIT_SINGLELINE;
				} 
				else if(cdtIPID.dwSystemIPID >= IPID_S_SUBAREA_0 && cdtIPID.dwSystemIPID <= IPID_S_SUBAREA_MAX)
				{
					// 指标参数为分区
					return CDTIT_EDIT_SINGLELINE;
				}
				else
				{
					DEBUGOUT("Error: In CIndicatorParam::GetInputType, dwSystemIPID=0X%08X not found!!!", cdtIPID.dwSystemIPID);
				}
			} break;
		}
	}
	
	ASSERT(FALSE);
	return 0;
}

// 是否是多值条件
BOOL CIndicatorParam::IsMultipleValue(const CDTIPID &cdtIPID, DWORD dwCustomData, int *pMaxCountOfValue)
{
	ASSERT(pMaxCountOfValue != NULL);
    
	if(cdtIPID.btType & CDTIPID_TYPE_CUSTOM)		// 自定义指标参数
	{
		if(dwCustomData == 0)
		{
			return FALSE;
		}
        
		if(cdtIPID.btType & CDTIPID_TYPE_DATAAREA)		// 号码分区指标参数
		{
			return (*pMaxCountOfValue) > 1 ? TRUE : FALSE;
		}
		else
		{
			LPCIP_HEAD lpCustomIP = (LPCIP_HEAD)dwCustomData;
			switch(lpCustomIP->wFlag)
			{
                case CIPHF_AREA:
                case CIPHF_REMAINDER:
                    return (*pMaxCountOfValue) > 1 ? TRUE : FALSE;
			}
		}
        
		return FALSE;
	}
    
	// 系统指标参数
	switch(cdtIPID.dwSystemIPID)
	{
		case IPID_S_SUM:
		case IPID_S_SUM_TAIL:
		case IPID_S_SUM_N:
		case IPID_S_SPAN:
		case IPID_S_SPAN_TAIL:
		case IPID_S_AVG:
		case IPID_S_AVG_TAIL:
		case IPID_S_MIN:
		case IPID_S_MIN_TAIL:
		case IPID_S_MID:
		case IPID_S_MID_TAIL:
		case IPID_S_MAX:
		case IPID_S_MAX_TAIL:
			(*pMaxCountOfValue) = 1;	// 单值
			return FALSE;			// 这些条件一定不是多值条件
	}
    
	return (*pMaxCountOfValue) > 1 ? TRUE : FALSE;
}

BOOL CIndicatorParam::IsDigitalValueName(const CDTIPID &cdtIPID, DWORD /*dwCustomData*/)
{
	if(cdtIPID.btType & (CDTIPID_TYPE_DATAAREA | CDTIPID_TYPE_CUSTOM))	// 自定义分区指标参数
	{
		return TRUE;		// 数字型
	}
	else	// 常规指标参数
	{
		switch(cdtIPID.dwSystemIPID)
		{
            case IPID_S_ODDEVEN: 
            case IPID_S_BIGSMALL:
            case IPID_S_DZX:		
            case IPID_S_012:
            case IPID_S_PRIMECOMPOSITE:
            case IPID_D_TESTNUMS_COMPARE:
            case IPID_D_SPJ:
                return FALSE;		// 非数字型
                
            default:
			{
				if(cdtIPID.dwSystemIPID >= IPID_S_SUBAREA_0 && cdtIPID.dwSystemIPID <= IPID_S_SUBAREA_MAX)	// 指标参数为分区
				{
					return FALSE;		// 非数字型
				}
			} break;
		}
	}
	
	return TRUE;	// 数字型
}

#define		GETVALUEEXPLAINITEM(nCalcValueIndex) \
ASSERT(lpValueRangeSelf->nItemCount <= 128); \
if(bIsParentDigitalValueName) { \
for(i=0; i<lpValueRangeSelf->nItemCount; i++) { \
cValueDataArrays[i].SetSize(lpValueRangeParent->nItemCount); \
} \
cVDAIndexArray.SetSize(lpValueRangeSelf->nItemCount); \
} else { \
strValueArray.SetSize(lpValueRangeSelf->nItemCount); \
for(i=0; i<lpValueRangeSelf->nItemCount; i++) { \
strValueArray[i].Empty(); \
} \
} \
for(i=0; i<lpValueRangeParent->nItemCount; i++)	{ \
nIndex = (nCalcValueIndex) - lpValueRangeSelf->nMinValue; \
if(bIsParentDigitalValueName) { \
cValueDataArrays[nIndex].SetAt(cVDAIndexArray[nIndex]++, lpValueRangeParent->stValueItem[i].nValue); \
} else { \
strValueArray[nIndex] += lpValueRangeParent->stValueItem[i].szValueName; \
strValueArray[nIndex] += ", "; \
} \
} \
/* 去除最后面的逗号和空格 */ \
strExplainArray.SetSize(lpValueRangeSelf->nItemCount); \
for(i=0; i<lpValueRangeSelf->nItemCount; i++) { \
if(bIsParentDigitalValueName) { \
cValueDataArrays[i].SetSize(cVDAIndexArray[i]); \
strExplainArray[i].Format("%s：%s（%s）", lpValueRangeSelf->stValueItem[i].szBallName, lpValueRangeSelf->stValueItem[i].szValueName, g_pICstPubFunc->IntArrayToText(cValueDataArrays[i])); \
} else { \
strValueArray[i].TrimRight(", "); \
strExplainArray[i].Format("%s：%s（%s）", lpValueRangeSelf->stValueItem[i].szBallName, lpValueRangeSelf->stValueItem[i].szValueName, strValueArray[i]); \
} \
}

int CIndicatorParam::GetValueExplain(CStringArray &strExplainArray, const CDTIPID &cdtIPID, DWORD dwCustomData, ICondition *pIConditionParent, LPCDTVALUERANGE lpValueRangeSelf)
{
	ASSERT(pIConditionParent != NULL && lpValueRangeSelf != NULL);
    
	if(cdtIPID.btType & CDTIPID_TYPE_CUSTOM)	// 自定义指标参数
	{
		return _GetValueExplain_Custom(strExplainArray, cdtIPID, dwCustomData, pIConditionParent, lpValueRangeSelf);
	}
    
	// 系统指标参数
	BYTE btVRType = pIConditionParent->GetValueRangeType();
	if(btVRType & (CDTRT_ARRANGE | CDTRT_NOTSUPPORT))
	{
		return 0;
	}
	
	LPCDTVALUERANGE lpValueRangeParent = pIConditionParent->GetValueRange();
	BOOL bIsParentDigitalValueName = pIConditionParent->IsDigitalValueName();
	CStringArray strValueArray;
	CDWordArray cValueDataArrays[128], cVDAIndexArray;
	int i = 0, nIndex = 0;
    
	switch(cdtIPID.dwSystemIPID)
	{
        case IPID_S_ODDEVEN:
		{
			if(btVRType & CDTRT_NOLIMIT)
			{
				strExplainArray.SetSize(2);
				strExplainArray[0] = "J：奇数（1,3,5,7,9 ...） ";
				strExplainArray[1] = "O：偶数（0,2,4,6,8 ...） ";
			}
			else
			{
				GETVALUEEXPLAINITEM(1 - (abs(lpValueRangeParent->stValueItem[i].nValue) % 2));
			}
		} break;
        case IPID_S_BIGSMALL:
		{
			int nMinValue = 0, nMaxValue = 0;
			_GetValueRangeForPosArea(nMinValue, nMaxValue, pIConditionParent);
			GETVALUEEXPLAINITEM(2 - theApp.GetSubAreaIndex(2, lpValueRangeParent->stValueItem[i].nValue, nMinValue, nMaxValue));
		} break;
        case IPID_S_DZX:
		{
			int nMinValue = 0, nMaxValue = 0;
			_GetValueRangeForPosArea(nMinValue, nMaxValue, pIConditionParent);
			GETVALUEEXPLAINITEM(theApp.GetSubAreaIndex(3, lpValueRangeParent->stValueItem[i].nValue, nMinValue, nMaxValue) - 1);
		} break;
        case IPID_S_012:
		{
			if(btVRType & CDTRT_NOLIMIT)
			{
				strExplainArray.SetSize(3);
				strExplainArray[0] = "0：0路（除3余0，如：0,3,6 ...） ";
				strExplainArray[1] = "1：1路（除3余1，如：1,4,7 ...） ";
				strExplainArray[2] = "2：2路（除3余2，如：2,5,8 ...） ";
			}
			else
			{
				GETVALUEEXPLAINITEM(abs(lpValueRangeParent->stValueItem[i].nValue) % 3);
			}
		} break;
        case IPID_S_PRIMECOMPOSITE:		
		{
			GETVALUEEXPLAINITEM(1 - theApp.m_btPrimeFlag[abs(lpValueRangeParent->stValueItem[i].nValue)]);
		} break;
        case IPID_S_TAIL:
		{
			if(btVRType & CDTRT_NOLIMIT)
			{
				strExplainArray.SetSize(10);
				for(int i=0; i<10; i++)
				{
					strExplainArray[i].Format("%d：（%d,%d,%d ...）", i, i, i + 10, i + 20);
				}
			}
			else
			{
				GETVALUEEXPLAINITEM(abs(lpValueRangeParent->stValueItem[i].nValue) % 10);
			}
		} break;
        case IPID_S_ABS:		
		{
			GETVALUEEXPLAINITEM(abs(lpValueRangeParent->stValueItem[i].nValue));
		} break;
        case IPID_S_ABS_TAIL:		
		{
			GETVALUEEXPLAINITEM(abs(lpValueRangeParent->stValueItem[i].nValue) % 10);
		} break;
        case IPID_S_SUM:
        case IPID_S_SUM_TAIL:
        case IPID_S_SUM_N:
        case IPID_S_SPAN:
        case IPID_S_SPAN_TAIL:
        case IPID_S_AVG:
        case IPID_S_AVG_TAIL:
        case IPID_S_MIN:
        case IPID_S_MIN_TAIL:
        case IPID_S_MID:
        case IPID_S_MID_TAIL:
        case IPID_S_MAX:
        case IPID_S_MAX_TAIL:
		{
            
		} break;		// TEST
        case IPID_D_TESTNUMS_COMPARE:
		{
			strExplainArray.SetSize(2);
			strExplainArray[0] = "0：不同";
			strExplainArray[1] = "1：相同";
		} break;
        default:
		{
			if(cdtIPID.dwSystemIPID >= IPID_S_REMAINDER_0 && cdtIPID.dwSystemIPID <= IPID_S_REMAINDER_MAX)	// 指标参数为余数
			{
				int nDivisor = cdtIPID.dwSystemIPID - IPID_S_REMAINDER_0;
				if(btVRType & CDTRT_NOLIMIT)
				{
					strExplainArray.SetSize(nDivisor);
					for(int i=0; i<nDivisor; i++)
					{
						strExplainArray[i].Format("%d：（%d,%d,%d ...）", i, i, i + nDivisor, i + 2 * nDivisor);
					}
				}
				else
				{
					GETVALUEEXPLAINITEM(lpValueRangeParent->stValueItem[i].nValue % nDivisor);
				}
			}
			else if(cdtIPID.dwSystemIPID >= IPID_S_SUBAREA_0 && cdtIPID.dwSystemIPID <= IPID_S_SUBAREA_MAX)	// 指标参数为分区
			{
				int nMinValue = 0, nMaxValue = 0;
				_GetValueRangeForPosArea(nMinValue, nMaxValue, pIConditionParent);
				int nAreaCount = cdtIPID.dwSystemIPID - IPID_S_SUBAREA_0;
				GETVALUEEXPLAINITEM(theApp.GetSubAreaIndex(nAreaCount, lpValueRangeParent->stValueItem[i].nValue, nMinValue, nMaxValue));
			}
			else
			{
				return 0;	// 没有相关说明信息
			}
		} break;
	}
    
	return strExplainArray.GetSize();
}

/////////////////////////////////////////////////////////////////////////////////////////////////

void CIndicatorParam::_RemoveAllValue()
{
	ASSERT(m_lpValueTemp != NULL);
	
	m_lpValueTemp->btItemCount = 0;
}

void CIndicatorParam::_AddValue(int nValue, BYTE btValueCount)
{
	ASSERT(m_lpValueTemp != NULL);
	ASSERT(m_lpValueTemp->btItemCount < MAXBYTE - 1);
	
	LPCDTVALUEITEM lpItem = NULL;
	for(int i=0; i<m_lpValueTemp->btItemCount; i++)
	{
		lpItem = &m_lpValueTemp->stValueItems[i];
		if(lpItem->nValue == nValue)
		{
			// 值已经存在，则只需要对应的个数加btValueCount
			ASSERT(lpItem->btCount < MAXBYTE);
			lpItem->btCount = BYTE(lpItem->btCount + btValueCount);
			return ;
		}
	}
	
	// 值不存在，则新添加一个值
	lpItem = &m_lpValueTemp->stValueItems[m_lpValueTemp->btItemCount];
	lpItem->btCount = btValueCount;
	lpItem->nValue = nValue;
	
	m_lpValueTemp->btItemCount ++;
}

void CIndicatorParam::_GetValueRangeForPosArea(int &nMinValue, int &nMaxValue, ICondition *pIConditionParent)
{
	ASSERT(pIConditionParent != NULL);
    
	if(!(g_pIData->GetDataType() & DATA_TYPE_REPEATABLE) && (g_pIData->GetDataType() & DATA_TYPE_ORDERED))
	{
		// 不可重复且排序的需要特殊处理
		LPCDTID lpCdtID = &pIConditionParent->GetID();
		if(!lpCdtID->HasFirstIP() && !(lpCdtID->cdtIID.btType & CDTIID_TYPE_CUSTOM))
		{
			if(lpCdtID->cdtIID.dwSystemIID >= IID_STC_POS_1 && lpCdtID->cdtIID.dwSystemIID <= IID_STC_POS_8)
			{
				// 按位1-8，需要统一使用整个号码的值域，否则每位的分区信息不一样
				nMinValue = g_pIData->GetMinNumber();
				nMaxValue = g_pIData->GetMaxNumber();
				return ;
			}
		}
	}
    
	// 其他则直接使用值域
	LPCDTVALUERANGE lpRange = pIConditionParent->GetValueRange();
	ASSERT(lpRange != NULL);
    
	nMinValue = lpRange->nMinValue;
	nMaxValue = lpRange->nMaxValue;
}
////////////////////////////////////////////////////////////////

int CIndicatorParam::_GetCustomIPValue(const CDTIPID &cdtIPID, DWORD dwCustomData, int nParentValue)
{
	ASSERT(nParentValue != CDTVALUE_INVALID);
    
	int nValue = CDTVALUE_INVALID;
	ASSERT(dwCustomData != 0);
    
	if(cdtIPID.btType & CDTIPID_TYPE_DATAAREA)		// 号码分区指标参数
	{
		LPDATAAREA lpDataArea = (LPDATAAREA)dwCustomData;
		ASSERT(nParentValue >= g_pIData->GetMinNumber() && nParentValue <= g_pIData->GetMaxNumber());
		nValue = lpDataArea->btSubAreaValues[nParentValue];
	}
	else
	{
		LPCIP_HEAD lpCustomIP = (LPCIP_HEAD)dwCustomData;
		switch(lpCustomIP->wFlag)
		{
            case CIPHF_AREA:
			{
				LPCIP_CUSTOMAREA lpCustomArea = (LPCIP_CUSTOMAREA)lpCustomIP->lpDataBuf;
				ASSERT(nParentValue >= lpCustomArea->nMinValue && nParentValue <= lpCustomArea->nMaxValue);
				nValue = lpCustomArea->GetSubAreaIndex(nParentValue);
			} break;
            case CIPHF_REMAINDER:
			{
				LPCIP_CUSTOMREMAINDER lpCustomRem = (LPCIP_CUSTOMREMAINDER)lpCustomIP->lpDataBuf;
				ASSERT(nParentValue >= lpCustomRem->nMinValue && nParentValue <= lpCustomRem->nMaxValue);
				nValue = nParentValue % lpCustomRem->nDivisor;
			} break;
            default: ASSERT(FALSE); break;
		}
	}
    
	return nValue;
}

CString CIndicatorParam::_GetSubDataAreaText(int nAreaIndex, LPDATAAREA lpDataArea)
{
	ASSERT(lpDataArea != NULL);
	
	CDWordArray dwValueArray;
	BYTE btMinNumber = g_pIData->GetMinNumber(), btMaxNumber = g_pIData->GetMaxNumber(), btIndex = 0;
	dwValueArray.SetSize(btMaxNumber);
	
	for(BYTE i=btMinNumber; i<=btMaxNumber; i++)
	{
		if(lpDataArea->btSubAreaValues[i] == (BYTE)(nAreaIndex + lpDataArea->btSubAreaMinValue))
		{
			dwValueArray.SetAt(btIndex, i);
			btIndex ++;
		}
	}
	
	dwValueArray.SetSize(btIndex);
	
	return g_pICstPubFunc->IntArrayToText(dwValueArray, btMaxNumber > 9 ? FALSE : TRUE, btMaxNumber > 9 ? 2 : 0);
}

CString CIndicatorParam::_GetSubCustomAreaText(int nAreaIndex, LPCIP_CUSTOMAREA lpCustomArea, ICondition * pIConditionParent)
{
	ASSERT(lpCustomArea != NULL && pIConditionParent != NULL);
	
	CDWordArray dwValueArray;
	CString strRtn;
    
	int nIndex = 0, nMaxValue = lpCustomArea->nMaxValue;
	if(nMaxValue == CDTVALUE_INVALID)
	{
		nMaxValue = lpCustomArea->nMinValue + lpCustomArea->wAreaDataCount - 1;
		if(nAreaIndex == lpCustomArea->wAreaCount - 1)
		{
			strRtn.Format("所有大于等于%d的数", nMaxValue + 1);
			return strRtn;
		}
	}
	dwValueArray.SetSize(lpCustomArea->wAreaDataCount);
	
	for(int i=lpCustomArea->nMinValue; i<=nMaxValue; i++)
	{
		if(lpCustomArea->GetSubAreaIndex(i) == (WORD)(nAreaIndex + lpCustomArea->btSubAreaMinValue))
		{
			dwValueArray.SetAt(nIndex, i);
			nIndex ++;
		}
	}
	
	dwValueArray.SetSize(nIndex);
	
	int nFixLen = 0;
	BYTE btVT = pIConditionParent->GetValueType();
	if(btVT >= CDTVT_DIGITAL2 && btVT <= CDTVT_DIGITAL8)
	{
		nFixLen = btVT - CDTVT_DIGITAL2 + 2;
	}
	
	return g_pICstPubFunc->IntArrayToText(dwValueArray, (lpCustomArea->nMaxValue > 9 || lpCustomArea->nMinValue < 0) ? FALSE : TRUE, nFixLen);
}

int CIndicatorParam::_GetValueExplain_Custom(CStringArray &strExplainArray, const CDTIPID &cdtIPID, DWORD dwCustomData, ICondition * pIConditionParent, LPCDTVALUERANGE lpValueRangeSelf)
{
	if(cdtIPID.btType & CDTIPID_TYPE_DATAAREA)		// 号码分区指标参数
	{
		LPDATAAREA lpDataArea = (LPDATAAREA)dwCustomData;
		strExplainArray.SetSize(lpDataArea->btAreaCount);
		for(int i=0; i<lpDataArea->btAreaCount; i++)
		{
			strExplainArray[i].Format("%s：%s（%s）", lpValueRangeSelf->stValueItem[i].szBallName, lpValueRangeSelf->stValueItem[i].szValueName, _GetSubDataAreaText(i, lpDataArea));
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
				strExplainArray.SetSize(lpCustomArea->wAreaCount);
				for(int i=0; i<lpCustomArea->wAreaCount; i++)
				{
					strExplainArray[i].Format("%s：%s（%s）", lpValueRangeSelf->stValueItem[i].szBallName, lpValueRangeSelf->stValueItem[i].szValueName, 
                                              _GetSubCustomAreaText(i, lpCustomArea, pIConditionParent));
				}
			} break;
            case CIPHF_REMAINDER:
			{
                /*				LPCIP_CUSTOMREMAINDER lpCustomRem = (LPCIP_CUSTOMREMAINDER)lpCustomIP->lpDataBuf;
                 int nDivisor = lpCustomRem->nDivisor;
                 if(pIConditionParent->GetValueRangeType() & CDTRT_NOLIMIT)
                 {
                 strExplainArray.SetSize(nDivisor);
                 for(int i=0; i<nDivisor; i++)
                 {
                 strExplainArray[i].Format("%d：（%d,%d,%d ...）", i, i, i + nDivisor, i + 2 * nDivisor);
                 }
                 }
                 else
                 {
                 LPCDTVALUERANGE lpValueRangeParent = pIConditionParent->GetValueRange();
                 BOOL bIsParentDigitalValueName = pIConditionParent->IsDigitalValueName();
                 CStringArray strValueArray;
                 CDWordArray cValueDataArrays[128], cVDAIndexArray;
                 int i = 0, nIndex = 0;
                 GETVALUEEXPLAINITEM(lpValueRangeParent->stValueItem[i].nValue % nDivisor);
                 }*/
			} break;
            default: ASSERT(FALSE); break;
		}
	}
    
	return strExplainArray.GetSize();
}
