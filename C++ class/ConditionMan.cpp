//
//  ConditionMan.cpp
//  QCLottery
//
//  Created by 武 孙 on 12-5-23.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "ConditionMan.h"

#include "INumbers.h"
#include "QuickAddString.h"
#include "ICstPubFunc.h"

#define	BUILDFILTERCDTNAME(szName, szShortName) \
        { \
            if(bGetShortName) { \
                ASSERT(strlen(szShortName) < MAXLEN_CDT_SHORTNAME); \
                return szShortName; \
            }else{ \
                ASSERT(strlen(szName) < MAXLEN_CDT_NAME); \
                return szName; \
            } \
        }

const char INVALID_CONDITION_NAME[] = {"无效条件"};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConditionMan::CConditionMan()
{
	ZeroMemory(m_szConditionName, sizeof(m_szConditionName));
	ZeroMemory(m_szConditionShortName, sizeof(m_szConditionShortName));
	ZeroMemory(m_szCdtNameTemp, sizeof(m_szCdtNameTemp));
	ZeroMemory(m_szAreaRemarks, sizeof(m_szAreaRemarks));
	ZeroMemory(m_szNumsAreaInfoText, sizeof(m_szNumsAreaInfoText));
	ZeroMemory(m_szIndicatorExpain, sizeof(m_szIndicatorExpain));
	ZeroMemory(m_szIndicatorClassExpain, sizeof(m_szIndicatorClassExpain));
	ZeroMemory(m_szIndicatorParamExpain, sizeof(m_szIndicatorParamExpain));
	ZeroMemory(&m_stDataDiffValue, sizeof(m_stDataDiffValue));
    
	m_dwCurConditionCount = 0;
    
	g_pIConditionMan = (IConditionMan *)this;
    
	m_pIConditionInvalid = (ICondition *)new CCondition;
    
	m_btNumberCount = 0;
	m_stFCGroupMine.dwGroupID = CDTGID_TYPE_MIX | CDTGID_TYPE_MINE;
	m_stFCGroupSys.dwGroupID = CDTGID_TYPE_MIX;
    
    CCondition::m_pIndicator = &m_cIndicator;
    m_cIndicatorParamFirst.Initialize(TRUE);
    CCondition::m_pIndicatorParamFirst = &m_cIndicatorParamFirst;
    m_cIndicatorParamSecond.Initialize(FALSE);
    CCondition::m_pIndicatorParamSecond = &m_cIndicatorParamSecond;
}

CConditionMan::~CConditionMan()
{
	if(m_pIConditionInvalid != NULL)
	{
		delete (CCondition *)m_pIConditionInvalid;
		m_pIConditionInvalid = NULL;
	}
    
	// 删除所有号码分区信息指针集合
	for(int i=m_cDataAreaPointArray.GetSize()-1; i>=0; i--)
	{
		delete (LPDATAAREA)m_cDataAreaPointArray.GetAt(i);
	}
	m_cDataAreaPointArray.RemoveAll();
    
	// 删除所有自定义指标参数信息
	for(int i=m_cCustomIPPointArray.GetSize()-1; i>=0; i--)
	{
		delete (LPCIP_HEAD)m_cCustomIPPointArray.GetAt(i);
	}
	m_cCustomIPPointArray.RemoveAll();
    
	// 删除所有条件对象指针
	_RemoveAllConditionInfo();
}

///////////////////////////////////////////////////////////////////////
ICondition * CConditionMan::GetICondition(const CDTID &cdtID)
{
	// 查询条件是否存在
	int nInsertIndex = 0;
	ICondition *pICondition = _HalfFindICondition(cdtID, nInsertIndex);
	if(pICondition != NULL)
	{
		return pICondition;
	}
    
	if(!cdtID.IsValid())		// 无效ID号
	{
		return m_pIConditionInvalid;	// 无效条件
	}
    
	// 查询条件是否有效
	if(!IsValidCondition(cdtID))
	{
		return m_pIConditionInvalid;	// 无效条件
	}
	
	// 没有找到，则创建新的，并添加到集合
	CCondition *pCondition = new CCondition;
    
	// 预留空间，以便优化
	if(m_cIConditionPointArray.GetSize() <= (int)m_dwCurConditionCount)
	{
		m_cIConditionPointArray.SetSize(m_dwCurConditionCount + 1000);
	}
	
	// 插入到指定位置
	for(int i=m_dwCurConditionCount; i>nInsertIndex; i--)
	{
		m_cIConditionPointArray.SetAt(i, m_cIConditionPointArray.GetAt(i-1));
	}
	m_cIConditionPointArray.SetAt(nInsertIndex, (DWORD)(ICondition *)pCondition);
	m_dwCurConditionCount ++;
	
	// 初始化条件对象信息
	pCondition->Initialize(cdtID);
    
	// 设置自定义数据
	_InitCustomData(pCondition);
    
	return (ICondition *)pCondition;
}

ICondition * CConditionMan::GetICondition_System(DWORD dwSysDefIID, DWORD dwSysDefIPID, DWORD dwSysDefIPID2)
{
	CDTID cdtID;
	cdtID.InitSystemID(dwSysDefIID, dwSysDefIPID, dwSysDefIPID2);
    
	return GetICondition(cdtID);
}

CDTID CConditionMan::GetParentConditionID(const CDTID &cdtID)
{
	static CDTID cdtParentID;
    
	switch(cdtID.btType)
	{
        case CDTID_TYPE_IPID:
            cdtParentID = cdtID;
            cdtParentID.cdtIPID.Init();		// 去掉第一个指标参数
            cdtParentID.btType = CDTID_TYPE_IID;
            break;
            
        case CDTID_TYPE_IPID2:
            cdtParentID = cdtID;
            cdtParentID.cdtIPID2.Init();	// 去掉第二个指标参数
            cdtParentID.btType = CDTID_TYPE_IPID;
            break;
            
        default: cdtParentID.Init(); break;
	}
    
	return cdtParentID;
}

ICondition * CConditionMan::CreateTempICondition(const CDTID &cdtID, DWORD dwCustomData, ICondition *pIParentCdt)
{
	// 创建新的
	CCondition *pCondition = new CCondition;
	
	// 初始化条件对象信息
	if(!pCondition->InitializeForTemp(cdtID, pIParentCdt))
	{
		ASSERT(FALSE);
		delete pCondition;
		return NULL;
	}
	
	BOOL bIsCustom = FALSE;
	switch(cdtID.btType)
	{
        case CDTID_TYPE_IID:
            bIsCustom = (cdtID.cdtIID.btType & CDTIID_TYPE_CUSTOM) ? TRUE : FALSE;
            if(dwCustomData == 0)
            {
                dwCustomData = _GetCustomData(cdtID.cdtIID);
            }
            break;
        case CDTID_TYPE_IPID: bIsCustom = (cdtID.cdtIPID.btType & CDTIPID_TYPE_CUSTOM) ? TRUE : FALSE; break;
        case CDTID_TYPE_IPID2: bIsCustom = (cdtID.cdtIPID2.btType & CDTIPID_TYPE_CUSTOM) ? TRUE : FALSE; break;
	}
    
	if(bIsCustom)
	{
		// 设置自定义数据
		ASSERT(dwCustomData != 0);
		pCondition->SetCustomData(dwCustomData);
	}
	
	return (ICondition *)pCondition;
}

void CConditionMan::DeleteTempICondition(ICondition *pITempCondition)
{
	if(pITempCondition != NULL)
	{
		delete (CCondition *)pITempCondition;
	}
}

LPCSTR CConditionMan::GetConditionName(const CDTID &cdtID, BOOL bGetShortName)
{
	if(!IsValidCondition(cdtID))
	{
		return INVALID_CONDITION_NAME;
	}
    
	if(cdtID.btType == CDTID_TYPE_COMPLEX)	// 复合条件
	{
		switch(cdtID.cdtIID.dwSystemIID)
		{
            case CDTID_FLT_GALL: BUILDFILTERCDTNAME("胆码", "胆码"); break;
            case CDTID_FLT_NUMS: BUILDFILTERCDTNAME("号码群", "号码群"); break;
            case CDTID_FLT_NUMSGROUP: BUILDFILTERCDTNAME("号码组", "号码组"); break;
            case CDTID_FLT_POSDUPLEX: BUILDFILTERCDTNAME("定位复式", "定位复式"); break;
            case CDTID_FLT_AREACOMBO: BUILDFILTERCDTNAME("分区组号", "分区组号"); break;
            case CDTID_FLT_AREACOMBINE: BUILDFILTERCDTNAME("分区组合", "分区组合"); break;
            case CDTID_FLT_SUPERDISTRIBUTE: BUILDFILTERCDTNAME("大底分布", "大底分布"); break;
            case CDTID_FLT_SUPERBIGP: BUILDFILTERCDTNAME("超级分布", "超级分布"); break;
            case CDTID_FLT_CUSTOMCOMBO: BUILDFILTERCDTNAME("自定义组合", "自定义组合"); break;
            case CDTID_FLT_ASSEMPLY: BUILDFILTERCDTNAME("集合", "集合"); break;
            case CDTID_FLT_MOREDUPLEX: BUILDFILTERCDTNAME("多期复式", "多期复式"); break;
            case CDTID_FLT_GROUPDUPLEX: BUILDFILTERCDTNAME("分组复式", "分组复式"); break;
            case CDTID_FLT_ANY: BUILDFILTERCDTNAME("任意码", "任意码"); break;
            case CDTID_FLT_HISTORY: BUILDFILTERCDTNAME("历史", "历史"); break;
            case CDTID_FLT_NONE: BUILDFILTERCDTNAME("无条件", "无条件"); break;
		}
	}
	else if(cdtID.btType == CDTID_TYPE_INVALID)
	{
		BUILDFILTERCDTNAME("无效条件", "无效条件");
	}
    
	LPSTR lpszName = bGetShortName ? m_szConditionShortName : m_szConditionName;
    
	// 首先获取条件包含的指标名称
	strcpy(lpszName, m_cIndicator.GetName(cdtID.cdtIID, bGetShortName, _GetCustomData(cdtID.cdtIID)));
	
	// 其次获取条件包含的指标参数名称
	if(cdtID.cdtIPID.btType & CDTIPID_TYPE_VALID)
	{
		strcat(lpszName, m_cIndicatorParamFirst.GetName(cdtID.cdtIPID, _GetCustomData(cdtID.cdtIPID)));
        
		// 第二指标参数
		if(cdtID.cdtIPID2.btType & CDTIPID_TYPE_VALID)
		{
			strcat(lpszName, m_cIndicatorParamSecond.GetName(cdtID.cdtIPID2, _GetCustomData(cdtID.cdtIPID2)));
		}
	}
	
	CHECK_STRING_STACK(m_szConditionShortName);
	CHECK_STRING_STACK(m_szConditionName);
    
	return lpszName;
}

LPCSTR CConditionMan::GetIndicatorName(const CDTIID &cdtIID, BOOL bGetShortName, DWORD dwCustomData)
{
	if(cdtIID.btType & CDTIID_TYPE_CUSTOM)
	{
		if(dwCustomData == MAXDWORD)
		{
			dwCustomData = _GetCustomData(cdtIID);
			ASSERT(dwCustomData != 0);
		}
	}
    
	return m_cIndicator.GetName(cdtIID, bGetShortName, dwCustomData);
}

LPCSTR CConditionMan::GetIndicatorParamName(const CDTIPID &cdtIPID, DWORD dwCustomData)
{
	if(cdtIPID.btType & CDTIPID_TYPE_CUSTOM)
	{
		if(dwCustomData == MAXDWORD)
		{
			dwCustomData = _GetCustomData(cdtIPID);
			ASSERT(dwCustomData != 0);
		}
	}
    
	return m_cIndicatorParamFirst.GetName(cdtIPID, dwCustomData);
}

BOOL CConditionMan::IsValidCondition(const CDTID &cdtID)
{
	if(cdtID.btType == CDTID_TYPE_COMPLEX)	// 复合条件
	{
		switch(cdtID.cdtIID.dwSystemIID)
		{
            case 0:
                return FALSE;			// 无效条件
            case CDTID_FLT_GALL:
            case CDTID_FLT_NUMS:
            case CDTID_FLT_NUMSGROUP:
            case CDTID_FLT_POSDUPLEX:
            case CDTID_FLT_AREACOMBO:
            case CDTID_FLT_AREACOMBINE:
            case CDTID_FLT_SUPERDISTRIBUTE:
            case CDTID_FLT_SUPERBIGP:
            case CDTID_FLT_CUSTOMCOMBO:
            case CDTID_FLT_ASSEMPLY:
            case CDTID_FLT_ANY:
            case CDTID_FLT_HISTORY:
            case CDTID_FLT_NONE:
                return TRUE;
		}
	}
	else if(cdtID.btType == CDTID_TYPE_INVALID)
	{
		return TRUE;
	}
	
	// 先查询指标
	if(!m_cIndicator.IsValid(cdtID.cdtIID, _GetCustomData(cdtID.cdtIID)))
	{
		return FALSE;
	}
	
	// 再查询第一指标参数
	if(cdtID.cdtIPID.btType & CDTIPID_TYPE_VALID)
	{
		if(cdtID.cdtIPID.btType & CDTIPID_TYPE_CUSTOM)		// 自定义指标参数
		{
			if(_GetCustomData(cdtID.cdtIPID) == 0)
			{
				return FALSE;
			}
		}
		else if(!m_cIndicatorParamFirst.IsValid(cdtID.cdtIPID))
		{
			return FALSE;
		}
	}
	
	// 最后查询第二指标参数
	if(cdtID.cdtIPID2.btType & CDTIPID_TYPE_VALID)
	{
		if(cdtID.cdtIPID2.btType & CDTIPID_TYPE_CUSTOM)		// 自定义指标参数
		{
			if(_GetCustomData(cdtID.cdtIPID2) == 0)
			{
				return FALSE;
			}
		}
		else if(!m_cIndicatorParamSecond.IsValid(cdtID.cdtIPID2))
		{
			return FALSE;
		}
	}
	
	return TRUE;
}

BOOL CConditionMan::IsTestNumsCondition(const CDTID &cdtID, DWORD /*dwCustomData*/)
{
	switch(cdtID.btType)
	{
        case CDTID_TYPE_IPID2:
            if(cdtID.cdtIPID2.btType & CDTIPID_TYPE_TESTNUMS)
            {
                return TRUE;
            }
        case CDTID_TYPE_IPID:
            if(cdtID.cdtIPID.btType & CDTIPID_TYPE_TESTNUMS)
            {
                return TRUE;
            }
        case CDTID_TYPE_IID:
            if(cdtID.cdtIID.btType & CDTIID_TYPE_TESTNUMS)
            {
                return TRUE;
            }
	}
	
	return FALSE;
}


BOOL CConditionMan::IsMultipleValueCondition(const CDTID &cdtID, int *pMaxCountOfValue, LPBOOL lpHasSameValue)
{
	if(cdtID.btType == CDTID_TYPE_INVALID)	// 无效条件
	{
		return FALSE;
	}
	
	if(lpHasSameValue != NULL)
	{
		*lpHasSameValue = TRUE;		// 缺省设为可含有相同条件值
	}
    
	int nMaxCountOfValue = 1;
    
	// 获取指标的多值条件属性
	DWORD dwCustomData = _GetCustomData(cdtID.cdtIID);
	BOOL bIsMultipleValue = m_cIndicator.IsMultipleValue(cdtID.cdtIID, dwCustomData, &nMaxCountOfValue, lpHasSameValue);
    
	if(cdtID.cdtIPID.IsValid())
	{
		dwCustomData = _GetCustomData(cdtID.cdtIPID);
		bIsMultipleValue = m_cIndicatorParamFirst.IsMultipleValue(cdtID.cdtIPID, dwCustomData, &nMaxCountOfValue);
	}
	
	if(cdtID.cdtIPID2.IsValid())
	{
		dwCustomData = _GetCustomData(cdtID.cdtIPID2);
		bIsMultipleValue = m_cIndicatorParamSecond.IsMultipleValue(cdtID.cdtIPID2, dwCustomData, &nMaxCountOfValue);
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
}

// 查询条件是否是动态条件
BOOL CConditionMan::IsDynamicCondition(const CDTID &cdtID, DWORD /*dwCustomData*/)
{
	if(cdtID.cdtIPID2.btType & CDTIPID_TYPE_DYNAMIC)
	{
		return TRUE;
	}
	
	if(cdtID.cdtIPID.btType & CDTIPID_TYPE_DYNAMIC)
	{
		return TRUE;
	}
	
	return (cdtID.cdtIID.btType & CDTIID_TYPE_DYNAMIC) ? TRUE : FALSE;
}

BYTE CConditionMan::GetConditionValueRangeType(const CDTID &cdtID)
{
	switch(cdtID.btType)
	{
        case CDTID_TYPE_INVALID:	// 无效条件
            return CDTRT_NOTSUPPORT;
            break;
            
        case CDTID_TYPE_IID:	// 使用指标相关的值
		{
			return m_cIndicator.GetValueRangeType(cdtID.cdtIID, _GetCustomData(cdtID.cdtIID));
		} break;
            
        case CDTID_TYPE_IPID:	// 使用第一指标参数相关的值
		{
			return m_cIndicatorParamFirst.GetValueRangeType(cdtID.cdtIPID, _GetCustomData(cdtID.cdtIPID));		
		} break;
            
        case CDTID_TYPE_IPID2:	// 使用第二指标参数相关的值
		{
			return m_cIndicatorParamSecond.GetValueRangeType(cdtID.cdtIPID2, _GetCustomData(cdtID.cdtIPID2));		
		} break;
	}
    
	return 0;
}

BYTE CConditionMan::GetConditionValueType(const CDTID &cdtID)
{
	switch(cdtID.btType)
	{
        case CDTID_TYPE_INVALID:	// 无效条件
            return CDTVT_DIGITAL;
            break;
            
        case CDTID_TYPE_IID:	// 使用指标相关的值
		{
			return m_cIndicator.GetValueType(cdtID.cdtIID, _GetCustomData(cdtID.cdtIID));
		} break;
            
        case CDTID_TYPE_IPID:	// 使用第一指标参数相关的值
		{
			return m_cIndicatorParamFirst.GetValueType(cdtID.cdtIPID, _GetCustomData(cdtID.cdtIPID));		
		} break;
            
        case CDTID_TYPE_IPID2:	// 使用第二指标参数相关的值
		{
			return m_cIndicatorParamSecond.GetValueType(cdtID.cdtIPID2, _GetCustomData(cdtID.cdtIPID2));		
		} break;
	}
    
	return 0;
}

BYTE CConditionMan::GetConditionInputType(const CDTID &cdtID)
{
	switch(cdtID.btType)
	{
        case CDTID_TYPE_INVALID:	// 无效条件
            return CDTIT_EDIT_SINGLELINE;
            break;
            
        case CDTID_TYPE_IID:	// 使用指标相关的值
		{
			return m_cIndicator.GetInputType(cdtID.cdtIID, _GetCustomData(cdtID.cdtIID));
		} break;
            
        case CDTID_TYPE_IPID:	// 使用第一指标参数相关的值
		{
			return m_cIndicatorParamFirst.GetInputType(cdtID.cdtIPID, _GetCustomData(cdtID.cdtIPID));		
		} break;
            
        case CDTID_TYPE_IPID2:	// 使用第二指标参数相关的值
		{
			return m_cIndicatorParamSecond.GetInputType(cdtID.cdtIPID2, _GetCustomData(cdtID.cdtIPID2));		
		} break;
	}
	
	return 0;
}

/*
int CConditionMan::GetExtendConditions(LPCDTID lpCdtID, CArray<CDTID, CDTID &> *pExtIDArray)
{
	return m_cCdtInfo.GetExtendConditions(lpCdtID, pExtIDArray);
}
*/
LPCSTR CConditionMan::GetIndicatorExplain(const CDTIID &cdtIID)
{
	// 获取指标说明
	LPCSTR lpszExplain = m_cIndicator.GetExplain(cdtIID, (DWORD)_GetCustomData(cdtIID));
	if(lpszExplain == NULL)
	{
		return NULL;
	}
    
	strcpy(m_szIndicatorExpain, lpszExplain);
	
	CHECK_STRING_STACK(m_szIndicatorExpain);
	
	return m_szIndicatorExpain;	
}

LPCSTR CConditionMan::GetIndicatorParamExplain(const CDTIPID &cdtIPID)
{
	// 获取指标说明
	LPCSTR lpszExplain = m_cIndicatorParamFirst.GetExplain(cdtIPID);
	if(lpszExplain == NULL)
	{
		return NULL;
	}
	
	strcpy(m_szIndicatorParamExpain, lpszExplain);
	
	CHECK_STRING_STACK(m_szIndicatorParamExpain);
	
	return m_szIndicatorParamExpain;	
}

WORD CConditionMan::GetIndicatorClassID(const CDTIID &cdtIID)
{
	if(cdtIID.btType & CDTIID_TYPE_CUSTOM)		// 自定义指标
	{
		return 0;
	}
    
	// 系统指标
	DWORD dwSysIID = cdtIID.dwSystemIID;
	if(dwSysIID >= IID_STC_TWOPOS_BEGIN && dwSysIID <= IID_STC_TWOPOS_END)		// 两位关系
	{
		int nType = ((dwSysIID - IID_STC_TWOPOS_BEGIN) / 100) % 100;
		
		return (WORD)(ICID_TWOPOS_BEGIN + nType);
	}
    
	if(dwSysIID >= IID_DNC_XXX_BEGIN && dwSysIID <= IID_DNC_XXX_END)			// 3态指标
	{
		int nType = ((dwSysIID - IID_DNC_XXX_BEGIN) / 100) % 10;
		
		return (WORD)(ICID_3STATE_BEGIN + nType);
	}
    
	if(dwSysIID >= IID_DNC_PREVDATA_BEGIN && dwSysIID <= IID_DNC_PREVDATA_END)	// 邻期关系
	{
		int nType = ((dwSysIID - IID_DNC_PREVDATA_BEGIN) / 100) % 100;
        
		return (WORD)(ICID_PREVDATA_BEGIN + nType);
	}
    
	return 0;	// 未分类
}

LPCSTR CConditionMan::GetIndicatorClassExplain(WORD wICID)
{
	m_szIndicatorClassExpain[0] = 0;
    
    // TEST
/*	char szFilename[MAX_PATH] = {0}, szKeyName[16] = {0};
	sprintf(szFilename, "%sIndicatorClass.ini", g_pIConfig->GetPath(PATH_CONFIG));
	sprintf(szKeyName, "%d", wICID);
    
	::GetPrivateProfileString("EXPLAIN", szKeyName, "", m_szIndicatorClassExpain, sizeof(m_szIndicatorClassExpain), szFilename);
    */
	return m_szIndicatorClassExpain[0] == 0 ? NULL : m_szIndicatorClassExpain;
}

///////////////////////////////////////////////////////////////////////////////////////////////
LPFIXCONDITIONGROUP CConditionMan::GetFixConditionGroup(DWORD dwGroupID)
{
	LPFIXCONDITIONGROUP lpRoot = m_cCdtInfo.GetFixConditionGroup(dwGroupID < CDTID_MINEGROUP ? TRUE : FALSE);
    
	if(dwGroupID == 0 || dwGroupID == CDTID_MINEGROUP)
	{
		return lpRoot;
	}
    
    LPFIXCONDITIONGROUP lpFCGroup = NULL;
	for(int i=lpRoot->stSubItemPointArray.GetSize()-1; i>=0; i--)
	{
        lpFCGroup = (LPFIXCONDITIONGROUP)lpRoot->stSubItemPointArray.GetAt(i);
		if(lpFCGroup->dwGroupID == dwGroupID)
		{
			return lpFCGroup;
		}
	}
    
	return NULL;	// not find
}

BOOL CConditionMan::ReplaceFixConditionGroupName(DWORD dwGroupID, LPSTR lpszGroupName)
{
	switch(dwGroupID)
	{
        case CDTID_FIXGROUP_SKIPSIN:
		{
			if(g_pIData->GetNumberCount() == 1)
			{
				sprintf(lpszGroupName, "遗漏值");
				return TRUE;
			}
		} break;
        case CDTID_FIXGROUP_NUMBERS:
		{
			if(!(g_pIData->GetDataType() & DATA_TYPE_NUM_SZ))
			{
				sprintf(lpszGroupName, "号码");
				return TRUE;
			}
		} break;
        case CDTID_FIXGROUP_POS:
		{
			if(g_pIData->GetNumberCount() == 1)
			{
				sprintf(lpszGroupName, "号码");
				return TRUE;
			}
		} break;
	}
    
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////////////////////
void CConditionMan::OnPreDataTypeChange()
{
	
}

void CConditionMan::OnDataTypeChanged()
{
	m_btNumberCount = g_pIData->GetNumberCount();
    
	// 查询是否和当前的条件信息相同
	DATADIFFVALUE stDataDiffValue = {0};	// 数据区分标志值
	g_pIData->GetCurrentDataDiffValue(&stDataDiffValue, DDLM_CUR_DATATYPE | DDLM_CUR_NUMBERCOUNT | DDLM_CUR_MINNUMBER | DDLM_CUR_MAXNUMBER);
	if(m_stDataDiffValue != stDataDiffValue)
	{
		// 不相同，则重新装载相关信息
		// 删除所有号码分区信息指针集合
		for(int i=m_cDataAreaPointArray.GetSize()-1; i>=0; i--)
		{
			delete (LPDATAAREA)m_cDataAreaPointArray.GetAt(i);
		}
		m_cDataAreaPointArray.RemoveAll();
        
		// 删除所有自定义指标参数信息
		for(int i=m_cCustomIPPointArray.GetSize()-1; i>=0; i--)
		{
			delete (LPCIP_HEAD)m_cCustomIPPointArray.GetAt(i);
		}
		m_cCustomIPPointArray.RemoveAll();
        
		// 删除当前的条件信息
		_RemoveAllConditionInfo();
		
		m_stDataDiffValue = stDataDiffValue;

		// 通知其他对象数据类型改变
		m_cIndicator.OnDataTypeChanged();
		m_cCdtInfo.OnDataTypeChanged();
	}
	
	// 通知自定义相关配置改变
	OnConfigChanged(MAXDWORD);
}

void CConditionMan::OnConfigChanged(DWORD dwConfigID)
{
	if(dwConfigID == CFGID_DATA_PRODUCT || dwConfigID == MAXDWORD)		// 积数设置改变
	{
// TEST		theApp.ReloadProductSettings();
		
		CCondition *pConditionImpl = NULL;
		
		// 清空所有积数、位积、位置积相关条件的值域
		for(int i=m_dwCurConditionCount-1; i>=0; i--)
		{
			pConditionImpl = (CCondition *)m_cIConditionPointArray.GetAt(i);
			if(pConditionImpl->IsPosMulConditions())
			{
				pConditionImpl->EmptyValueRange();
			}
		}
	}
    
	if(dwConfigID == CFGID_DATA_AREA || dwConfigID == MAXDWORD)			// 号码分区设置改变
	{
/*		int nDataAreaCount = 0;
		LPDATAAREA lpDataAreas = (LPDATAAREA)g_pIConfig->GetConfigBuffer(CFGID_DATA_AREA, &nDataAreaCount);
		nDataAreaCount /= sizeof(DATAAREA);
        
		for(int i=0; i<nDataAreaCount; i++)
		{
			AddDataArea(&lpDataAreas[i], TRUE, FALSE);	// 添加到当前使用的号码分区数据集合中
		}
		_ResetAllValueRange_Custom();	// 清空所有自定义条件的值域
 */
	}
    
	if(dwConfigID == CFGID_CONDITION_CUSTOM_ID || dwConfigID == MAXDWORD)	// 自定义指标改变
	{
		_ResetAllValueRange_Custom();	// 清空所有自定义条件的值域
	}
    
	if(dwConfigID == CFGID_CONDITION_CUSTOM_IP || dwConfigID == MAXDWORD)	// 自定义指标参数改变
	{
/*		int nLen = 0;
		LPBYTE lpBuf = g_pIConfig->GetConfigBuffer(CFGID_CONDITION_CUSTOM_IP, &nLen);
		if(nLen > 0)
		{
			DWORD dwCount = 0, dwIndex = sizeof(dwCount);
			CIP_HEAD stCIP;
			memcpy(&dwCount, lpBuf, sizeof(dwCount));
			for(DWORD i=0; i<dwCount; i++)
			{
				stCIP.LoadData(&lpBuf[dwIndex]);
				AddCustomIP((DWORD)&stCIP, TRUE, FALSE);			// 添加到当前使用的自定义指标参数数据集合中
				dwIndex += stCIP.GetLen();
			}
			ASSERT(dwIndex == (DWORD)nLen);
			_ResetAllValueRange_Custom();	// 清空所有自定义条件的值域
		}*/
	}
    
	if(dwConfigID != MAXDWORD)
	{
		m_cCdtInfo.OnConfigChanged(dwConfigID);
	}
}

/////////////////////////////////////////////////////////////////////////
DWORD CConditionMan::GetSystemSubAreaData(DWORD dbtAreaCount, DWORD dwAreaIndex, int nMinValue, int nMaxValue)
{
	return g_pICstPubFunc->GetSubAreaData(dbtAreaCount, dwAreaIndex, nMinValue, nMaxValue);
}

int CConditionMan::GetSystemSubAreaIndex(DWORD dbtAreaCount, int nValue, int nMinValue, int nMaxValue)
{
	return g_pICstPubFunc->GetSubAreaIndex(dbtAreaCount, nValue, nMinValue, nMaxValue);
}

LPCSTR CConditionMan::GetDataAreaRemarks(LPDATAAREA lpDataArea, BOOL bNoSubName, LPCSTR lpszSep)
{
	ASSERT(lpDataArea != NULL);
    
	m_szAreaRemarks[0] = 0;
    
	char szTemp[128] = {0};
	
	for(WORD i=0; i<lpDataArea->btAreaCount; i++)
	{
		if(bNoSubName)
		{
// TEST			sprintf(szTemp, "%s", _GetSubDataAreaText(i, lpDataArea));
		}
		else
		{
// TEST			sprintf(szTemp, "%s(%s)", lpDataArea->szSubAreaNames[i], _GetSubDataAreaText(i, lpDataArea));
		}
		if((i < lpDataArea->btAreaCount - 1) && lpszSep != NULL)
		{
			// 添加分隔符
			strcat(szTemp, lpszSep);
		}
		strcat(m_szAreaRemarks, szTemp);
	}
    
	CHECK_STRING_STACK(m_szAreaRemarks);
	return m_szAreaRemarks;
}

LPDATAAREA CConditionMan::GetDataArea(const CDTID &cdtID)
{
	if(cdtID.cdtIID.btType & CDTIID_TYPE_DATAAREA)
	{
		return _GetDataArea(cdtID.cdtIID);
	}
    
	return NULL;		// 非号码分区相关条件
}

LPDATAAREA CConditionMan::GetDataArea(DWORD dwAreaDiffValues[2])
{
	LPDATAAREA lpDataArea = NULL;
	for(int i=m_cDataAreaPointArray.GetSize()-1; i>=0; i--)
	{
		lpDataArea = (LPDATAAREA)m_cDataAreaPointArray.GetAt(i);
		if(lpDataArea->dwAreaDiffValues[0] == dwAreaDiffValues[0] && lpDataArea->dwAreaDiffValues[1] == dwAreaDiffValues[1])
		{
			return lpDataArea;	// find
		}
	}
	
	return NULL;
}

BOOL CConditionMan::IsDataAreaExist(LPDATAAREA lpDataArea)
{
	for(int i=m_cDataAreaPointArray.GetSize()-1; i>=0; i--)
	{
		if(!memcmp(lpDataArea->dwAreaDiffValues, ((LPDATAAREA)m_cDataAreaPointArray.GetAt(i))->dwAreaDiffValues, sizeof(lpDataArea->dwAreaDiffValues)))
		{
			return TRUE;
		}
	}
    
	return FALSE;
}

BOOL CConditionMan::AddDataArea(LPDATAAREA lpDataArea, BOOL bModifyIfExist, BOOL bToSystem)
{
	ASSERT(lpDataArea != NULL);
	
	// 先查询是否存在
	LPDATAAREA lpExistDataArea = NULL, lpDataAreaInArray = NULL;
	for(int i=m_cDataAreaPointArray.GetSize()-1; i>=0; i--)
	{
        lpDataAreaInArray = (LPDATAAREA)m_cDataAreaPointArray.GetAt(i);
		if(lpDataAreaInArray->IsSameID(lpDataArea))
		{
			lpExistDataArea = lpDataAreaInArray;		// exist
			break;
		}
	}
    
	if(lpExistDataArea == NULL)		// 不存在，则添加
	{
		LPDATAAREA lpNewDataArea = new DATAAREA;
		(*lpNewDataArea) = (*lpDataArea);
		m_cDataAreaPointArray.Add((DWORD)lpNewDataArea);
	}
	else
	{
		if(bModifyIfExist)	// 修改当前存在的
		{
			(*lpExistDataArea) = (*lpDataArea);
		}
	}
    
/*	if(bToSystem && lpExistDataArea == NULL)	// 新的数据加入系统，则需要写入配置
	{
		int nSysDataAreaCount = 0;
		LPDATAAREA lpSysDataAreas = (LPDATAAREA)g_pIConfig->GetConfigBuffer(CFGID_DATA_AREA, &nSysDataAreaCount);
		nSysDataAreaCount /= sizeof(DATAAREA);
        
		LPDATAAREA lpNewDataAreas = new	DATAAREA[nSysDataAreaCount + 1];
		memcpy(lpNewDataAreas, lpSysDataAreas, nSysDataAreaCount * sizeof(DATAAREA));
		memcpy(&lpNewDataAreas[nSysDataAreaCount], lpDataArea, sizeof(DATAAREA));
		g_pIConfig->WriteConfig(CFGID_DATA_AREA, (nSysDataAreaCount + 1) * sizeof(DATAAREA), (LPBYTE)lpNewDataAreas);
		delete []lpNewDataAreas;
	}
    */
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
BOOL CConditionMan::IsCustomCondition(const CDTID &cdtID)
{
	if(cdtID.cdtIID.btType & CDTIID_TYPE_CUSTOM)
	{
		return TRUE;
	}
	
	if(cdtID.cdtIPID.btType & CDTIPID_TYPE_CUSTOM)
	{
		return TRUE;
	}
	
	if(cdtID.cdtIPID2.btType & CDTIPID_TYPE_CUSTOM)
	{
		return TRUE;
	}
	
	return FALSE;
}

BOOL CConditionMan::IsCustomIIDExist(const CDTIID & /*cdtIID*/)
{
	return FALSE;
}

BOOL CConditionMan::AddCustomID(DWORD /*dwCustomData*/, BOOL /*bModifyIfExist*/, BOOL /*bToSystem*/)
{
	return FALSE;
}

BOOL CConditionMan::IsCustomIPIDExist(const CDTIPID &cdtIPID)
{	
	return _GetCustomIP(cdtIPID) == NULL ? FALSE : TRUE;
}

BOOL CConditionMan::AddCustomIP(DWORD dwCustomData, BOOL bModifyIfExist, BOOL bToSystem)
{
	ASSERT(dwCustomData != 0);
	
	// 先查询是否存在
//	LPCIP_HEAD lpExistCIP = NULL, lpCIPSrc = (LPCIP_HEAD)dwCustomData;
    
/*	for(int i=m_cCustomIPPointArray.GetSize()-1; i>=0; i--)
	{
		if(m_cCustomIPPointArray[i]->IsSameID(lpCIPSrc))
		{
			lpExistCIP = m_cCustomIPPointArray[i];		// exist
			break;
		}
	}
	
	if(lpExistCIP == NULL)		// 不存在，则添加
	{
		LPCIP_HEAD lpNewCIP = new CIP_HEAD;
		(*lpNewCIP) = (*lpCIPSrc);
		m_cCustomIPPointArray.Add(lpNewCIP);
	}
	else
	{
		if(bModifyIfExist)	// 修改当前存在的
		{
			(*lpExistCIP) = (*lpCIPSrc);
		}
	}
	
	if(bToSystem && lpExistCIP == NULL)	// 新的数据加入系统，则需要写入配置
	{
		int nLen = 0;
		LPBYTE lpSysCIPs = g_pIConfig->GetConfigBuffer(CFGID_CONDITION_CUSTOM_IP, &nLen);
		
		LPBYTE lpNewCIPs = new BYTE[nLen + lpCIPSrc->GetLen()];
		memcpy(lpNewCIPs, lpSysCIPs, nLen);
		lpCIPSrc->SaveData(&lpNewCIPs[nLen]);
		g_pIConfig->WriteConfig(CFGID_CONDITION_CUSTOM_IP, nLen + lpCIPSrc->GetLen(), lpNewCIPs);
		delete []lpNewCIPs;
	}
	*/
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////
void CConditionMan::PrepareGetOrgDataTransConditions()
{
	m_stDataTransTemp.wID = MAXWORD;
	if(g_pIData->IsOrgDataTrans())
	{
		return ;		// 当前已经是原始数据变换
	}
    
	// 判断当前数据属性是否原始变换前一样，如果一样，则不需要转换
	DATADIFFVALUE stDDVCur = {0}, stDDVCam = {0};
	g_pIData->GetCurrentDataDiffValue(&stDDVCur, DDLM_CUR_DATATYPE | DDLM_CUR_NUMBERCOUNT | DDLM_CUR_MINNUMBER | DDLM_CUR_MAXNUMBER);
	g_pIData->GetCurrentDataDiffValue(&stDDVCam, DDLM_CAM_DATATYPE | DDLM_CAM_NUMBERCOUNT | DDLM_CAM_MINNUMBER | DDLM_CAM_MAXNUMBER);
	if(stDDVCur == stDDVCam)
	{
		return ;		// 当前数据变换的属性和原始数据变换属性一致
	}
    
	// 保存当前数据变换和相关条件信息
	memcpy(&m_stDataTransTemp, g_pIData->GetCurrentDataTrans(), sizeof(m_stDataTransTemp));
	m_cIConditionPointArrayTemp.SetSize(m_dwCurConditionCount);
	for(int i=0; i<(int)m_dwCurConditionCount; i++)
	{
		m_cIConditionPointArrayTemp.SetAt(i, m_cIConditionPointArray.GetAt(i));
	}
	m_cDataAreaPointArrayTemp.Copy(m_cDataAreaPointArray);
	m_cCustomIPPointArrayTemp.Copy(m_cCustomIPPointArray);
    
	// 清空当前条件信息，以免被删除
	m_cIConditionPointArray.RemoveAll();
	m_dwCurConditionCount = 0;
	m_cDataAreaPointArray.RemoveAll();
	m_cCustomIPPointArray.RemoveAll();
    
	// 获取当前排列方式以便以后恢复原始排列方式
	m_btDataOrderTypeTemp = g_pIData->GetCurrentDataOrderType();
    
	// 设置原始数据变换，以便获取原始数据相关条件值
	DATATRANSSET stDTItem = {0};
	stDTItem.btType = DATATRANS_TYPE_ORG;
	stDTItem.wID = DATATRANS_ID_ORG;
	stDTItem.btItemCount = g_pIData->GetCurrentAnalysisMode()->btCount;
	strcpy(stDTItem.szName, "原始数据");
	g_pIData->SetDataTrans(&stDTItem);
	// 将排列方式设为缺省值
	g_pIData->SetDataOrderType(DATA_ORDERTYPE_NONE);
    
	m_cIndicator.OnDataTypeChanged();
    
	// 通知自定义相关配置改变
	OnConfigChanged(MAXDWORD);
}

void CConditionMan::FinishGetOrgataTransConditions()
{
	if(m_stDataTransTemp.wID == MAXWORD)
	{
		return ;
	}
    
	// 恢复当前数据变换
	g_pIData->SetDataTrans(&m_stDataTransTemp);
    
	// 清除当前条件信息，并恢复相关条件
	for(int i=m_cDataAreaPointArray.GetSize()-1; i>=0; i--)
	{
		delete (LPDATAAREA)m_cDataAreaPointArray.GetAt(i);
	}
	m_cDataAreaPointArray.Copy(m_cDataAreaPointArrayTemp);
	m_cDataAreaPointArrayTemp.RemoveAll();
	
	// 删除所有自定义指标参数信息
	for(int i=m_cCustomIPPointArray.GetSize()-1; i>=0; i--)
	{
		delete (LPCIP_HEAD)m_cCustomIPPointArray.GetAt(i);
	}
	m_cCustomIPPointArray.Copy(m_cCustomIPPointArrayTemp);
	m_cCustomIPPointArrayTemp.RemoveAll();
    
	for(int i=0; i<(int)m_dwCurConditionCount; i++)
	{
		delete (CCondition *)m_cIConditionPointArray.GetAt(i);	
	}
	m_cIConditionPointArray.Copy(m_cIConditionPointArrayTemp);
	m_dwCurConditionCount = m_cIConditionPointArray.GetSize();
	m_cIConditionPointArrayTemp.RemoveAll();
    
	// 恢复当前排列方式
	g_pIData->SetDataOrderType(m_btDataOrderTypeTemp);
    
	m_cIndicator.OnDataTypeChanged();
	
	// 通知自定义相关配置改变
	OnConfigChanged(MAXDWORD);
}

/////////////////////////////////////////////////////////////////////////////////////////////
void CConditionMan::_InitCustomData(CCondition *pCondition)
{
	ASSERT(pCondition != NULL);
    
	CDTID &cdtID = pCondition->GetID();
    
	// 设置自定义数据
	if(!cdtID.HasSecondIP())	// 没有第二个指标参数
	{
		if(!cdtID.HasFirstIP())	// 只有指标
		{
			if(cdtID.cdtIID.btType & CDTIID_TYPE_CUSTOM)
			{
				pCondition->SetCustomData(_GetCustomData(cdtID.cdtIID));
			}
		}
		else if(cdtID.cdtIPID.btType & CDTIPID_TYPE_CUSTOM)		// 只有第一个指标参数，且为自定义
		{
			pCondition->SetCustomData(_GetCustomData(cdtID.cdtIPID));
		}
	}
	else // 含有第二指标参数
	{
		if(cdtID.cdtIPID2.btType & CDTIPID_TYPE_CUSTOM)		// 第二指标参数为自定义
		{
			pCondition->SetCustomData(_GetCustomData(cdtID.cdtIPID2));
		}
	}
}

DWORD CConditionMan::_GetCustomData(const CDTIID &cdtIID)
{
	if(cdtIID.btType & CDTIID_TYPE_DATAAREA)	// 分区指标
	{
		return (DWORD)_GetDataArea(cdtIID);
	}
    
	return 0;
}

DWORD CConditionMan::_GetCustomData(const CDTIPID &cdtIPID)
{
	if(cdtIPID.btType & CDTIPID_TYPE_CUSTOM)	// 自定义指标参数
	{
		if(cdtIPID.btType & CDTIPID_TYPE_DATAAREA)	// 分区指标参数
		{
			return (DWORD)_GetDataArea(cdtIPID);
		}
        
		return (DWORD)_GetCustomIP(cdtIPID);
	}
    
	return 0;
}

void CConditionMan::_RemoveAllConditionInfo()
{
	// 清除条件接口对象
	for(int i=0; i<(int)m_dwCurConditionCount; i++)
	{
		delete (CCondition *)m_cIConditionPointArray.GetAt(i);	
	}
	m_dwCurConditionCount = 0;
	m_cIConditionPointArray.RemoveAll();
}

ICondition * CConditionMan::_HalfFindICondition(const CDTID &cdtID, int &nInsertIndex)
{
	// 折半查找
	int nMid = m_dwCurConditionCount / 2, nFirst = 0, nLast = m_dwCurConditionCount;
	
	CDTID cdtIDExist;
    
	while(nFirst <= nLast && nMid < (int)m_dwCurConditionCount)
	{
		cdtIDExist = ((ICondition *)m_cIConditionPointArray.GetAt(nMid))->GetID();
		if(cdtIDExist == cdtID)
		{
			return (ICondition *)m_cIConditionPointArray.GetAt(nMid);		// find
		}
		else if(cdtIDExist > cdtID)		// 如果小于当前元素值，则往前折半查找
		{
			nLast = nMid - 1;
		}
		else							// 如果大于当前元素值，则往后折半查找
		{
			nFirst = nMid + 1;
		}
		nMid = (nFirst + nLast) / 2;
	}
	
	// 没有找到
	nInsertIndex = nFirst;
	return NULL;
}

LPCSTR CConditionMan::_GetConditionNameByType(const CDTID &cdtID, BYTE btType)
{
	if(btType & BCTT_NAME_WHOLE)		// 全称
	{
		if(btType & BCTT_NAME_SHORT)	// 含有简称
		{
			sprintf(m_szCdtNameTemp, "%s（%s）", GetConditionName(cdtID), GetConditionName(cdtID, TRUE));
			CHECK_STRING_STACK(m_szCdtNameTemp);
			return m_szCdtNameTemp;
		}
		else
		{
			return GetConditionName(cdtID);
		}
	}
	else	// 简称
	{
		return GetConditionName(cdtID, TRUE);
	}
}

// 获取条件组名
LPCSTR CConditionMan::_GetConditionGroupName(LPFIXCONDITIONGROUP lpFixCdtGroup, BYTE btType)
{
	ASSERT(lpFixCdtGroup != NULL);
    
	if(lpFixCdtGroup->IsCdtIDValid())
	{
		return _GetConditionNameByType(lpFixCdtGroup->GetCdtID(), btType);	// 以条件作为组
	}
    
	return lpFixCdtGroup->GetName();
}

LPDATAAREA CConditionMan::_GetDataArea(const CDTIID & cdtIID)
{
	ASSERT(cdtIID.btType & CDTIID_TYPE_DATAAREA);
    
	LPDATAAREA lpDataArea = NULL;
	for(int i=m_cDataAreaPointArray.GetSize()-1; i>=0; i--)
	{
		lpDataArea = (LPDATAAREA)m_cDataAreaPointArray.GetAt(i);
		if(lpDataArea->dwAreaDiffValues[0] == cdtIID.stCustomIID.dwDiffValues[0] && lpDataArea->dwAreaDiffValues[1] == cdtIID.stCustomIID.dwDiffValues[1])
		{
			return lpDataArea;	// find
		}
	}
    
	return NULL;
}

LPDATAAREA CConditionMan::_GetDataArea(const CDTIPID &cdtIPID)
{
	ASSERT(cdtIPID.btType & CDTIPID_TYPE_DATAAREA);
	
	LPDATAAREA lpDataArea = NULL;
	for(int i=m_cDataAreaPointArray.GetSize()-1; i>=0; i--)
	{
		lpDataArea = (LPDATAAREA)m_cDataAreaPointArray.GetAt(i);
		if(lpDataArea->dwAreaDiffValues[0] == cdtIPID.stCustomIPID.dwDiffValues[0] && lpDataArea->dwAreaDiffValues[1] == cdtIPID.stCustomIPID.dwDiffValues[1])
		{
			return lpDataArea;	// find
		}
	}
	
	return NULL;
}

/*CString CConditionMan::_GetSubDataAreaText(int nAreaIndex, LPDATAAREA lpDataArea)
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
*/
LPCIP_HEAD CConditionMan::_GetCustomIP(const CDTIPID &cdtIPID)
{
	ASSERT(cdtIPID.btType & CDTIPID_TYPE_CUSTOM);
	ASSERT(!(cdtIPID.btType & CDTIPID_TYPE_DATAAREA));
    
	LPCIP_HEAD lpCIP = NULL;
	for(int i=m_cCustomIPPointArray.GetSize()-1; i>=0; i--)
	{
		lpCIP = (LPCIP_HEAD)m_cCustomIPPointArray.GetAt(i);
		if(!memcmp(lpCIP->dwDiffValues, cdtIPID.stCustomIPID.dwDiffValues, sizeof(lpCIP->dwDiffValues)))
		{
			return lpCIP;
		}
	}
    
	return NULL;
}

void CConditionMan::_ResetAllValueRange_Custom()
{
	CCondition *pConditionImpl = NULL;
    
	for(int i=m_dwCurConditionCount-1; i>=0; i--)
	{
		pConditionImpl = (CCondition *)m_cIConditionPointArray.GetAt(i);
		if(pConditionImpl->IsCustom())
		{
			pConditionImpl->EmptyValueRange();
		}
	}
}
