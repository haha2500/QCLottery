//
//  Condition.cpp
//  QCLottery
//
//  Created by 武 孙 on 12-5-23.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "Condition.h"
#include "ConditionMan.h"

char CCondition::ms_szValueStringTemp[128] = {0};
char CCondition::ms_szValueStringMore[1024 * 10] = {0};
CIndicator *CCondition::m_pIndicator = NULL;
CIndicatorParam *CCondition::m_pIndicatorParamFirst = NULL;
CIndicatorParam *CCondition::m_pIndicatorParamSecond = NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CCondition::CCondition()
{
	m_lpValueRange = NULL;
	m_pIConditionParent = NULL;
	m_dwCustomData = 0;
	m_cdtID.Init();
}

CCondition::~CCondition()
{
	if(m_lpValueRange != NULL)
	{
		delete [](LPBYTE)m_lpValueRange;
		m_lpValueRange = NULL;
	}
}

//////////////////////////////////////////////////////////////////////////
BOOL CCondition::Initialize(const CDTID& cdtID)
{
	ASSERT(m_cdtID.btType == CDTID_TYPE_INIT);	// 确保只初始化一次
	ASSERT(cdtID.btType != CDTID_TYPE_INIT && m_pIConditionParent == NULL);
	
	m_cdtID = cdtID;
    
	// 获取父级条件的接口
	if(cdtID.HasFirstIP())
	{
		m_pIConditionParent = g_pIConditionMan->GetICondition(g_pIConditionMan->GetParentConditionID(cdtID));
	}
	
	return TRUE;
}

BOOL CCondition::InitializeForTemp(const CDTID& cdtID, ICondition *pIParentCdt)
{
	ASSERT(m_cdtID.btType == CDTID_TYPE_INIT);	// 确保只初始化一次
	ASSERT(cdtID.btType != CDTID_TYPE_INIT && m_pIConditionParent == NULL);
	
	if(cdtID.btType == CDTID_TYPE_IPID || cdtID.btType == CDTID_TYPE_IPID2)
	{
		if(pIParentCdt == NULL)
		{
			ASSERT(FALSE);
			m_cdtID.btType = CDTID_TYPE_INIT;
			return FALSE;
		}
	}
	else
	{
		if(pIParentCdt != NULL)
		{
			ASSERT(FALSE);
			m_cdtID.btType = CDTID_TYPE_INIT;
			return FALSE;
		}
	}
    
	m_pIConditionParent = pIParentCdt;
	m_cdtID = cdtID;
	
	return TRUE;
}

void CCondition::EmptyValueRange()
{
	DELETE_POINTER(m_lpValueRange);
}

BOOL CCondition::IsPosMulConditions()
{
	if(!m_cdtID.cdtIID.IsSystemIID())
	{
		return FALSE;
	}
    
	if(m_cdtID.cdtIID.dwSystemIID < IID_STC_TWOPOS_BEGIN || m_cdtID.cdtIID.dwSystemIID > IID_STC_TWOPOS_END)
	{
		return FALSE;
	}
	
	if((m_cdtID.cdtIID.dwSystemIID - IID_STC_TWOPOS_BEGIN) / 100 != TPT_MUL)
	{
		return FALSE;
	}
    
	return TRUE;
}

/////////////////////////////////////////////////////////////////////
LPCSTR CCondition::GetName(BOOL bGetShortName)
{
	return g_pIConditionMan->GetConditionName(m_cdtID, bGetShortName);
}

LPCDTVALUERANGE CCondition::GetValueRange()
{
	if(m_lpValueRange == NULL)
	{
		// 如果没有计算过，则进行计算
		if(!m_pIndicatorParamSecond->GetValueRange(m_cdtID.cdtIPID2, m_pIConditionParent, m_dwCustomData, &m_lpValueRange))
		{
			if(!m_pIndicatorParamFirst->GetValueRange(m_cdtID.cdtIPID, m_pIConditionParent, m_dwCustomData, &m_lpValueRange))
			{
				if(!m_pIndicator->GetValueRange(m_cdtID.cdtIID, m_dwCustomData, &m_lpValueRange))
				{
					ASSERT(FALSE);
				}
			}
		}
	}
    
	ASSERT(m_lpValueRange != NULL);
    
	return m_lpValueRange;
}

int CCondition::GetIndexOfValueRange(int nValue)
{
	// 首选获取值域
	GetValueRange();
	
	if(m_lpValueRange == NULL || nValue == CDTVALUE_INVALID)
	{
		return -1;	// 如果不存在值域，则返回NULL
	}
	
	// 根据值域类型查找	
	BYTE btVRType = GetValueRangeType();
	int nIndex = -1;
	if(btVRType == CDTRT_DEFAULT)		// 缺省值，为连续的、有上限、从小到大排序
	{
		nIndex = nValue - m_lpValueRange->stValueItem[0].nValue;
		if(nIndex < 0 || nIndex >= m_lpValueRange->nItemCount)
		{
			nIndex = -1;
		}
	}
	else	// 否则使用折半查找
	{
		BOOL bValueSortAsc = (btVRType & CDTRT_ORDER_DESC) ? FALSE : TRUE;
		nIndex = _QuckFind_ValueItem(nValue, bValueSortAsc);
	}
    
	ASSERT(nIndex != -1);	// 不在值域中
	
	return nIndex;
}

LPCDTVALUERANGEITEM CCondition::GetItemOfValueRange(int nValue)
{
	int nIndex = GetIndexOfValueRange(nValue);
	if(nIndex == -1)
	{
		return NULL;
	}
    
	return &m_lpValueRange->stValueItem[nIndex];
}

BYTE CCondition::GetValueRangeType()
{
	switch(m_cdtID.btType)
	{
        case CDTID_TYPE_INIT:		// 初始条件
        case CDTID_TYPE_INVALID:	// 无效条件
            return CDTRT_NOTSUPPORT;
            break;
            
        case CDTID_TYPE_IID:	// 使用指标相关的值
            return m_pIndicator->GetValueRangeType(m_cdtID.cdtIID, m_dwCustomData);
            break;
            
        case CDTID_TYPE_IPID:	// 使用第一指标参数相关的值
            return m_pIndicatorParamFirst->GetValueRangeType(m_cdtID.cdtIPID, m_dwCustomData);
            break;
            
        case CDTID_TYPE_IPID2:	// 使用第二指标参数相关的值
            return m_pIndicatorParamSecond->GetValueRangeType(m_cdtID.cdtIPID2, m_dwCustomData);
            break;
            
        default: ASSERT(FALSE);
	}
    
	return 0;
}

BYTE CCondition::GetValueType()
{
	switch(m_cdtID.btType)
	{
        case CDTID_TYPE_INIT:		// 初始条件
        case CDTID_TYPE_INVALID:	// 无效条件
            return CDTVT_DIGITAL;
            break;
            
        case CDTID_TYPE_IID:	// 使用指标相关的值
            return m_pIndicator->GetValueType(m_cdtID.cdtIID, m_dwCustomData);
            break;
            
        case CDTID_TYPE_IPID:	// 使用第一指标参数相关的值
            return m_pIndicatorParamFirst->GetValueType(m_cdtID.cdtIPID, m_dwCustomData);
            break;
            
        case CDTID_TYPE_IPID2:	// 使用第二指标参数相关的值
            return m_pIndicatorParamSecond->GetValueType(m_cdtID.cdtIPID2, m_dwCustomData);
            break;
            
        default: ASSERT(FALSE);
	}
	
	return 0;
}

BYTE CCondition::GetInputType()
{
	switch(m_cdtID.btType)
	{
        case CDTID_TYPE_INIT:		// 初始条件
        case CDTID_TYPE_INVALID:	// 无效条件
            return CDTIT_EDIT_SINGLELINE;
            break;
            
        case CDTID_TYPE_IID:	// 使用指标相关的值
            return m_pIndicator->GetInputType(m_cdtID.cdtIID, m_dwCustomData);
            break;
            
        case CDTID_TYPE_IPID:	// 使用第一指标参数相关的值
            return m_pIndicatorParamFirst->GetInputType(m_cdtID.cdtIPID, m_dwCustomData);
            break;
            
        case CDTID_TYPE_IPID2:	// 使用第二指标参数相关的值
            return m_pIndicatorParamSecond->GetInputType(m_cdtID.cdtIPID2, m_dwCustomData);
            break;
            
        default: ASSERT(FALSE);
	}
	
	return 0;
}

BOOL CCondition::IsComplex()
{
	return m_cdtID.IsComplex();
}

BOOL CCondition::IsIncludeTestNums()
{
	return g_pIConditionMan->IsTestNumsCondition(m_cdtID);
}

BOOL CCondition::IsMultipleValue(int *pMaxCountOfValue, LPBOOL lpHasSameValue)
{
	if(m_cdtID.btType == CDTID_TYPE_INVALID)	// 无效条件
	{
		return FALSE;
	}
	
	if(lpHasSameValue != NULL)
	{
		*lpHasSameValue = TRUE;		// 缺省设为可含有相同条件值
	}
	
	int nMaxCountOfValue = 1;
	
	// 获取指标的多值条件属性
	BOOL bIsMultipleValue = m_pIndicator->IsMultipleValue(m_cdtID.cdtIID, GetCustomData(CDTCDF_INDICATOR), &nMaxCountOfValue, lpHasSameValue);
	
	if(m_cdtID.cdtIPID.IsValid())
	{
		bIsMultipleValue = m_pIndicatorParamFirst->IsMultipleValue(m_cdtID.cdtIPID, GetCustomData(CDTCDF_INDICATORPARAM), &nMaxCountOfValue);
	}
	
	if(m_cdtID.cdtIPID2.IsValid())
	{
		bIsMultipleValue = m_pIndicatorParamSecond->IsMultipleValue(m_cdtID.cdtIPID2, GetCustomData(CDTCDF_INDICATORPARAM2), &nMaxCountOfValue);
	}
	
	if(pMaxCountOfValue != NULL)
	{
		*pMaxCountOfValue = nMaxCountOfValue;
	}
	
	if(!bIsMultipleValue && lpHasSameValue != NULL)
	{
		*lpHasSameValue = FALSE;		// 单值条件不含有相同条件值
	}
	
	return bIsMultipleValue;
    //	return g_pIConditionMan->IsMultipleValueCondition(m_cdtID, pMaxCountOfValue, lpHasSameValue);
}

BOOL CCondition::IsDynamic()
{
	return g_pIConditionMan->IsDynamicCondition(m_cdtID);
}

int CCondition::GetValue(LPBYTE lpFltNumsData, DWORD dwIssueIndex, BYTE btType)
{
	LPCDTVALUE lpValue = GetValueEx(lpFltNumsData, dwIssueIndex, btType);
    
	if(lpValue->btItemCount == 1 && lpValue->stValueItems[0].btCount == 1)
	{
		return lpValue->stValueItems[0].nValue;			// 单值条件
	}
	else	// 多值条件
	{
		int nValue = 0, nRatio = 1;
		for(int i=lpValue->btItemCount-1; i>=0; i--)
		{
			nValue += lpValue->stValueItems[i].nValue * nRatio;
			nRatio *= 10;
		}
        
		return nValue;
	}
}

LPCDTVALUE CCondition::GetValueEx(LPBYTE lpFltNumsData, DWORD dwIssueIndex, BYTE btType)
{
	LPCDTVALUE lpValue = NULL;
	static CDTVALUE stInvalidCdtValue = {1, {CDTVALUE_INVALID, 1}};
    
	switch(m_cdtID.btType)
	{
        case CDTID_TYPE_INIT:		// 初始条件
        case CDTID_TYPE_INVALID:	// 无效条件
            lpValue = &stInvalidCdtValue;
            break;
            
        case CDTID_TYPE_IID:	// 使用指标相关的值
            lpValue = m_pIndicator->GetValue(m_cdtID.cdtIID, m_dwCustomData, lpFltNumsData, dwIssueIndex, btType);
            break;
            
        case CDTID_TYPE_IPID:	// 使用第一指标参数相关的值
            lpValue = m_pIndicatorParamFirst->GetValue(m_cdtID.cdtIPID, m_pIConditionParent, m_dwCustomData, lpFltNumsData, dwIssueIndex, btType);
            break;
            
        case CDTID_TYPE_IPID2:	// 使用第二指标参数相关的值
            lpValue = m_pIndicatorParamSecond->GetValue(m_cdtID.cdtIPID2, m_pIConditionParent, m_dwCustomData, lpFltNumsData, dwIssueIndex, btType);
            break;
            
        default: ASSERT(FALSE);
	}
    
	
	ASSERT(lpValue != NULL);
	return lpValue;
}

LPCSTR CCondition::GetValueString(LPBYTE lpFltNumsData, DWORD dwIssueIndex, BOOL bUseValueName)
{
	// 获取号码数据的条件值
	BYTE btType = (dwIssueIndex > DATA_INDEX_LAST) ? CDTGVT_FILTER : CDTGVT_NORMAL;
	LPCDTVALUE lpValue = GetValueEx(lpFltNumsData, dwIssueIndex, btType);
	BYTE btValueRangeType = GetValueRangeType(), btValueType = GetValueType();
    
	// 值转换为文本
	if(lpValue->btItemCount > 1)	// 多值条件
	{
		ms_szValueStringMore[0] = 0;
        
		for(int i=0; i<lpValue->btItemCount; i++)
		{
			strcat(ms_szValueStringMore, _GetValueString(lpValue->stValueItems[i].nValue, btValueRangeType, btValueType, bUseValueName));
			if(i < lpValue->btItemCount - 1)
			{
				strcat(ms_szValueStringMore, ",");
			}
		}
		
		return ms_szValueStringMore;
	}
	else					// 单值条件
	{
		// 在值域中查找
		return _GetValueString(lpValue->stValueItems[0].nValue, btValueRangeType, btValueType, bUseValueName);
	}
}

LPCSTR CCondition::ValueToString(int nValue, BOOL bUseValueName)
{
	BYTE btValueRangeType = GetValueRangeType(), btValueType = GetValueType();
    
	return _GetValueString(nValue, btValueRangeType, btValueType, bUseValueName);
}

LPCSTR CCondition::ValueToString(CDWordArray &cValueArray, BOOL bUseValueName)
{	
	return ValueToString((LPINT)cValueArray.GetData(), cValueArray.GetSize(), bUseValueName);
}

LPCSTR CCondition::ValueToString(LPINT lpValues, int nValueCount, BOOL bUseValueName)
{
	ms_szValueStringMore[0] = 0;
    
	if(lpValues == NULL || nValueCount == 0)
	{
		return ms_szValueStringMore;
	}
    
	BYTE btValueRangeType = GetValueRangeType(), btValueType = GetValueType();
	
	for(int i=0; i<nValueCount; i++)
	{
		strcat(ms_szValueStringMore, _GetValueString(lpValues[i], btValueRangeType, btValueType, bUseValueName));
		if(i < nValueCount - 1)
		{
			strcat(ms_szValueStringMore, ",");
		}
	}
	
	return ms_szValueStringMore;
}

BOOL CCondition::IsDigitalValueName()
{
	if(GetValueType() != CDTVT_DIGITAL)
	{
		return FALSE;
	}
    
	switch(m_cdtID.btType)
	{
        case CDTID_TYPE_IID:	// 使用指标相关的值
            return m_pIndicator->IsDigitalValueName(m_cdtID.cdtIID, m_dwCustomData, GetValueRange());
            break;
            
        case CDTID_TYPE_IPID:	// 使用第一指标参数相关的值
            return m_pIndicatorParamFirst->IsDigitalValueName(m_cdtID.cdtIPID, m_dwCustomData);
            break;
            
        case CDTID_TYPE_IPID2:	// 使用第二指标参数相关的值
            return m_pIndicatorParamSecond->IsDigitalValueName(m_cdtID.cdtIPID2, m_dwCustomData);
            break;
            
        default: ASSERT(FALSE);
	}
    
	return FALSE;
}

/*BOOL CCondition::GetValueExplain(CString &strExplain, int nLeftSpaceCount, int nSepSpaceCount, int nMaxCol)
{
	ASSERT(nLeftSpaceCount >= 0 && nLeftSpaceCount < 256);
	ASSERT(nSepSpaceCount >= 0 && nSepSpaceCount < 32);
    
	LPCDTVALUERANGE lpValueRange = GetValueRange();
	if(lpValueRange == NULL)	// 没有值域则返回错误
	{
		return FALSE;
	}
    
	CStringArray strValueExplainArray;
	int nCount = 0;
	
	switch(m_cdtID.btType)
	{
        case CDTID_TYPE_IID:	// 使用指标相关的值
            nCount = m_pIndicator->GetValueExplain(strValueExplainArray, m_cdtID.cdtIID, m_dwCustomData, lpValueRange);
            break;
            
        case CDTID_TYPE_IPID:	// 使用第一指标参数相关的值
            nCount = m_pIndicatorParamFirst->GetValueExplain(strValueExplainArray, m_cdtID.cdtIPID, m_dwCustomData, m_pIConditionParent, lpValueRange);
            break;
            
        case CDTID_TYPE_IPID2:	// 使用第二指标参数相关的值
            nCount = m_pIndicatorParamSecond->GetValueExplain(strValueExplainArray, m_cdtID.cdtIPID2, m_dwCustomData, m_pIConditionParent, lpValueRange);
            break;
            
        default: ASSERT(FALSE); break;
	}
	
	if(0 == nCount)
	{
		return FALSE;	// 无相关信息
	}
    
	strExplain = g_pICstPubFunc->GetFormattedGraphGridDetails(strValueExplainArray, nLeftSpaceCount, nSepSpaceCount, nMaxCol);
    
	return TRUE;
}
*/
BOOL CCondition::IsSupportGraph()
{
	if(GetValueRangeType() & (CDTRT_NOLIMIT | CDTRT_NOTSUPPORT | CDTRT_ARRANGE))
	{
		return FALSE;	// 值域类型不匹配
	}
    
	GetValueRange();
	ASSERT(m_lpValueRange->nItemCount > 1);
	
	if(m_lpValueRange->nItemCount >= 128 || m_lpValueRange->nMinValue <= -100 || m_lpValueRange->nMaxValue >= 100)
	{
		return FALSE;	// 值个数不匹配
	}
    
	return TRUE;		// 支持图表
}

void CCondition::SetCustomData(DWORD dwCustomData, BYTE btFlag)
{
	if(dwCustomData == 0)
	{
		m_cdtID.btType = CDTID_TYPE_INVALID;	// 赋值为无效条件
		m_dwCustomData = 0;
		return ;
	}
    
	switch(btFlag)
	{
        case CDTCDF_CONDITION:
		{
			m_dwCustomData = dwCustomData;
		} break;
        case CDTCDF_INDICATOR:
		{
			switch(m_cdtID.btType)
			{
                case CDTID_TYPE_IID: m_dwCustomData = dwCustomData; break;
                case CDTID_TYPE_IPID: m_pIConditionParent->SetCustomData(dwCustomData); break;
                case CDTID_TYPE_IPID2: m_pIConditionParent->GetIConditionParent()->SetCustomData(dwCustomData); break;
                default: ASSERT(FALSE); break;
			}
		} break;
        case CDTCDF_INDICATORPARAM:
		{
			switch(m_cdtID.btType)
			{
                case CDTID_TYPE_IPID: m_dwCustomData = dwCustomData; break;
                case CDTID_TYPE_IPID2: m_pIConditionParent->SetCustomData(dwCustomData); break;
                default: ASSERT(FALSE); break;
			}
		} break;
        case CDTCDF_INDICATORPARAM2:
		{
			switch(m_cdtID.btType)
			{
                case CDTID_TYPE_IPID2: m_dwCustomData = dwCustomData; break;
                default: ASSERT(FALSE); break;
			}
		} break;
	}
}

DWORD CCondition::GetCustomData(BYTE btFlag)
{
	ASSERT(m_cdtID.IsValid());
    
	switch(btFlag)
	{
        case CDTCDF_CONDITION:
		{
			return m_dwCustomData;
		} break;
        case CDTCDF_INDICATOR:
		{
			switch(m_cdtID.btType)
			{
                case CDTID_TYPE_IID: return m_dwCustomData; break;
                case CDTID_TYPE_IPID: return m_pIConditionParent->GetCustomData(); break;
                case CDTID_TYPE_IPID2: return m_pIConditionParent->GetIConditionParent()->GetCustomData(); break;
                default: ASSERT(FALSE); break;
			}
		} break;
        case CDTCDF_INDICATORPARAM:
		{
			switch(m_cdtID.btType)
			{
                case CDTID_TYPE_IPID: return m_dwCustomData; break;
                case CDTID_TYPE_IPID2: return m_pIConditionParent->GetCustomData(); break;
                default: ASSERT(FALSE); break;
			}
		} break;
        case CDTCDF_INDICATORPARAM2:
		{
			switch(m_cdtID.btType)
			{
                case CDTID_TYPE_IPID2: return m_dwCustomData; break;
                default: ASSERT(FALSE); break;
			}
		} break;
	}
    
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LPCSTR CCondition::_GetValueString(int nValue, BYTE btValueRangeType, BYTE btValueType, BOOL bUseValueName)
{
	if(btValueRangeType & CDTRT_ARRANGE)
	{
		// 排列型值域
		int nPosCount = g_pIData->GetNumberCount();
		if(btValueType >= CDTVT_ARRANGE_COUNT_BEGIN && btValueType <= CDTVT_ARRANGE_COUNT_END)
		{
			nPosCount = btValueType - CDTVT_ARRANGE_COUNT_BEGIN + 2;
		}
        
		sprintf(ms_szValueStringTemp, "%0*d", nPosCount, nValue);
		return ms_szValueStringTemp;
	}
	else if(btValueRangeType & CDTRT_NOLIMIT)
	{
		// 无上限型值域
		sprintf(ms_szValueStringTemp, "%d", nValue);
		return ms_szValueStringTemp;
	}
	else
	{
		LPCDTVALUERANGEITEM lpVRItem = GetItemOfValueRange(nValue);
		if(lpVRItem == NULL)
		{
			return "";
		}
        
		if(bUseValueName)
		{
			return lpVRItem->szValueName;
		}
		else
		{
			if(lpVRItem->szColName[0] == 0)
			{
				if(btValueType >= CDTVT_DIGITAL2 && btValueType <= CDTVT_DIGITAL8)
				{
					sprintf(ms_szValueStringTemp, "%0*d", btValueType - CDTVT_DIGITAL2 + 2, nValue);
				}
				else
				{
					sprintf(ms_szValueStringTemp, "%d", nValue);
				}
                
				return ms_szValueStringTemp;
			}
			else
			{
				return lpVRItem->szColName;
			}
		}
	}
}

int CCondition::_QuckFind_ValueItem(int nFindValue, BOOL bValueSortAsc)
{
	ASSERT(m_lpValueRange != NULL);
	
	if(m_lpValueRange->nItemCount < 5)		// 5个值以内则使用顺序查找
	{
		for(int i=0; i<m_lpValueRange->nItemCount; i++)
		{
			if(nFindValue == m_lpValueRange->stValueItem[i].nValue)
				return i;	// find
		}
		
		return -1;	// not find
	}
	
	// 折半查找	
	int nMid = m_lpValueRange->nItemCount / 2, nFirst = 0, nLast = m_lpValueRange->nItemCount;
	
	if(bValueSortAsc)		// 正向折半查找
	{
		while(nFirst <= nLast && nMid < m_lpValueRange->nItemCount)
		{
			if(nFindValue == m_lpValueRange->stValueItem[nMid].nValue)
			{
				return nMid;		// find
			}
			else if(nFindValue < m_lpValueRange->stValueItem[nMid].nValue)		// 如果小于当前元素值，则往前折半查找
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
	else		// 反向折半查找
	{
		while(nFirst <= nLast && nMid < m_lpValueRange->nItemCount)
		{
			if(nFindValue == m_lpValueRange->stValueItem[nMid].nValue)
			{
				return nMid;		// find
			}
			else if(nFindValue > m_lpValueRange->stValueItem[nMid].nValue)	// 如果大于当前元素值，则往前折半查找
			{
				nLast = nMid - 1;
			}
			else									// 如果小于当前元素值，则往后折半查找
			{
				nFirst = nMid + 1;
			}
			nMid = (nFirst + nLast) / 2;
		}
	}
	
	return -1;		// not find
}

