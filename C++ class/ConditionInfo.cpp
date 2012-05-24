//
//  ConditionInfo.cpp
//  QCLottery
//
//  Created by 武 孙 on 12-5-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "ConditionInfo.h"
#include "Def_ConditionID.h"
#include "Def_CfgItem_Data.h"
#include <math.h>
#include "ICstPubFunc.h"


#define		BEGIN_LOAD() \
                DWORD dwParentID[10] = {0}; \
                WORD wIndex = 0; \
                int nGroupLevel = 0; \
                cSysConditionInfoPointArray.SetSize(5000); \
                LPCONDITIONINFO lpCdtInfo = NULL;

#define		END_LOAD() \
                if(nGroupLevel != 0) { \
                    ASSERT(FALSE); \
                    return ; \
                } \
                ASSERT(wIndex <= cSysConditionInfoPointArray.GetSize()); \
                cSysConditionInfoPointArray.SetSize(wIndex);

#define		BEGIN_ADD_GROUP(szGroupNameIn, dwGroupIDIn) \
                lpCdtInfo = new CONDITIONINFO; \
                lpCdtInfo->dwParentID = dwParentID[nGroupLevel++]; \
                lpCdtInfo->dwGroupID = dwGroupIDIn; \
                if(lpCdtInfo->dwGroupID > 0 && !(lpCdtInfo->dwGroupID & CDTGID_TYPE_EXTEND)) { \
                    strcpy(lpCdtInfo->szName, (szGroupNameIn)); \
                    CHECK_STRING_STACK(lpCdtInfo->szName); \
                    dwParentID[nGroupLevel] = lpCdtInfo->dwGroupID; \
                } \
                else { \
                    ASSERT(FALSE); \
                } \
                cSysConditionInfoPointArray.SetAt(wIndex++, (DWORD)lpCdtInfo);

#define		END_ADD_GROUP() \
                nGroupLevel--; \
                if(nGroupLevel < 0) { \
                    ASSERT(FALSE); \
                return ; \
                }

#define		ADD_CONDITION(dwSysDefIID) \
                lpCdtInfo = new CONDITIONINFO; \
                lpCdtInfo->dwParentID = dwParentID[nGroupLevel]; \
                lpCdtInfo->dwGroupID = 0; \
                lpCdtInfo->cdtID.InitSystemID((dwSysDefIID), IPID_NONE, IPID_NONE); \
                cSysConditionInfoPointArray.SetAt(wIndex++, (DWORD)lpCdtInfo);


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CConditionInfo::CConditionInfo()
{
	m_stSysFixCdtGroupRoot.lpParent = m_stMineFixCdtGroupRoot.lpParent = m_stSysFixCdtGroupRoot_CdtExt.lpParent = NULL;
	m_stSysFixCdtGroupRoot.dwGroupID = m_stMineFixCdtGroupRoot.dwGroupID = m_stSysFixCdtGroupRoot_CdtExt.dwGroupID = CDTID_EXTENDGROUP;
    //	m_stSysFixCdtGroupRoot.szGroupName[0] = m_stMineFixCdtGroupRoot.szGroupName[0] = m_stSysFixCdtGroupRoot_CdtExt.szGroupName[0] = 0;
    
	m_nDataAreaCount = 0;
	m_lpDataAreas = NULL;
    
	// 装载系统条件信息
	_LoadSysConditions(m_cSysBaseConditionInfoPointArray, TRUE);
    _LoadSysConditions(m_cSysAdvConditionInfoPointArray, FALSE);
}

CConditionInfo::~CConditionInfo()
{
	// 销毁所有装载的条件信息
//	_ReleaseFixConditionGroup(&m_stSysFixCdtGroupRoot);
//	_ReleaseFixConditionGroup(&m_stMineFixCdtGroupRoot);
//	_ReleaseFixConditionGroup(&m_stSysFixCdtGroupRoot_CdtExt);
    m_cSysBaseConditionInfoPointArray.RemoveAll();
	m_cSysAdvConditionInfoPointArray.RemoveAll();
	m_cMineConditionInfoPointArray.RemoveAll();
	m_cSysConditionInfoPointArray_CdtExt.RemoveAll();
}

//////////////////////////////////////////////////////////////////////

void CConditionInfo::OnDataTypeChanged()
{
}

void CConditionInfo::OnConfigChanged(DWORD dwConfigID)
{
	if(dwConfigID == CFGID_DATA_AREA || dwConfigID == 0)	// 分区数据改变
	{
// TEST		m_lpDataAreas = (LPDATAAREA)g_pIConfig->GetConfigBuffer(CFGID_DATA_AREA, &m_nDataAreaCount);
		m_nDataAreaCount /= sizeof(DATAAREA);
	}
    
	if(dwConfigID == CFGID_DATA_AREA || dwConfigID == CFGID_CONDITION_EXTEND || dwConfigID == 0)	// 扩展条件改变
	{
		// 重新设置系统固定条件组
//		_ReleaseFixConditionGroup(&m_stSysFixCdtGroupRoot);
//		_ConditionInfoToFixConditionGroup(FCTYPE_SYSTEM);
	}
    
	if(dwConfigID == CFGID_DATA_AREA || dwConfigID == CFGID_CONDITION_MINE || dwConfigID == 0)	// 我的条件改变
	{
		// 销毁上一次装载的我的条件信息
		m_cMineConditionInfoPointArray.RemoveAll();
		
		// 重新装载我的条件信息
		_LoadMineConditions();
		// 重新设置固定条件组
//		_ReleaseFixConditionGroup(&m_stMineFixCdtGroupRoot);
		m_stMineFixCdtGroupRoot.dwGroupID = CDTID_MINEGROUP;
//		_ConditionInfoToFixConditionGroup(FCTYPE_MINE);
	}
}

/*
int CConditionInfo::GetExtendConditions(LPCDTID lpCdtID, CArray<CDTID, CDTID &> *pExtIDArray)
{
	if(lpCdtID != NULL && lpCdtID->btType == CDTID_TYPE_IPID2)
	{
		return -1;		// 不能扩展了
	}
    
	// 获取扩展条件信息
	CTypedPtrArray<CPtrArray, LPCDTID> cdtExtendIDPointArray;
	_GetExtendConditions(cdtExtendIDPointArray);
    
	int i = 0, nCount = cdtExtendIDPointArray.GetSize(), nIndex = 0;
	LPCDTID lpCdtExtendID = NULL;
	
	for(i=0; i<nCount; i++)
	{
		lpCdtExtendID = cdtExtendIDPointArray[i];
		if(!lpCdtExtendID->IsExtendOf(*lpCdtID))
		{
			continue;	// 不是该条件的扩展条件
		}
        
		// 是指定的扩展条件
		if(pExtIDArray != NULL)
		{
			if(pExtIDArray->GetSize() <= nIndex)
			{
				pExtIDArray->SetSize(nIndex + 100);
			}
			pExtIDArray->SetAt(nIndex, *lpCdtExtendID);
		}
		nIndex ++;
	}
    
	if(pExtIDArray != NULL)
	{
		pExtIDArray->SetSize(nIndex);
	}
	return nIndex;
}*/

LPFIXCONDITIONGROUP CConditionInfo::GetFixConditionGroup_ForCdtExtend(BOOL bDefault)
{
	ASSERT(m_cSysConditionInfoPointArray_CdtExt.GetSize() == 0);
    
	_LoadSysConditions(m_cSysConditionInfoPointArray_CdtExt);
//	_ConditionInfoToFixConditionGroup(FCTYPE_SYSTEM_CDTEXT, bDefault);
	
	return &m_stSysFixCdtGroupRoot_CdtExt;
}

void CConditionInfo::ReleaseFixConditionGroup_ForCdtExtend()
{
//	_ReleaseFixConditionGroup(&m_stSysFixCdtGroupRoot_CdtExt);
	m_cSysConditionInfoPointArray_CdtExt.RemoveAll();
}

//////////////////////////////////////////////////////////////////////
/*void CConditionInfo::_ConditionInfoToFixConditionGroup(BYTE btFCType, BOOL bDefault)
{
	LPFIXCONDITIONGROUP	lpFixCdtGroupRoot = NULL;
	CArray<CONDITIONINFO, CONDITIONINFO&> *pConditionInfoArray = NULL;
	switch(btFCType)
	{
        case FCTYPE_SYSTEM:
            lpFixCdtGroupRoot = &m_stSysFixCdtGroupRoot;
            pConditionInfoArray = &m_cSysConditionInfoPointArray;
            break;
        case FCTYPE_MINE:
            lpFixCdtGroupRoot = &m_stMineFixCdtGroupRoot;
            pConditionInfoArray = &m_cMineConditionInfoPointArray;
            break;
        case FCTYPE_SYSTEM_CDTEXT:
            lpFixCdtGroupRoot = &m_stSysFixCdtGroupRoot_CdtExt;
            pConditionInfoArray = &m_cSysConditionInfoPointArray_CdtExt;
            break;
        default: ASSERT(FALSE); break;
	}
	
	LPFIXCONDITIONGROUP lpParent = lpFixCdtGroupRoot, lpItem = NULL, lpFCGroupTestNums = NULL;
    
	int nCount = pConditionInfoArray->GetSize();
	LPCONDITIONINFO lpCdtInfo = NULL;
	LPFIXCONDITIONGROUP lpFCMineUngrouped = NULL;		// 我的条件中未分组信息
    
	for(int i=0; i<nCount; i++)
	{
		lpCdtInfo = &pConditionInfoArray->ElementAt(i);
		if(lpCdtInfo->dwGroupID == 0)	// 检查条件信息是否有效
		{
			if(!g_pIConditionMan->IsValidCondition(lpCdtInfo->cdtID) || !_IsFCGroupValid(lpCdtInfo->dwParentID))
			{
				continue;	// 如果不是有效条件或者父组无效，则忽略
			}
		}
		else	// 检查组信息是否有效
		{
			if(!_IsFCGroupValid(lpCdtInfo->dwGroupID))	// 查询组ID是否有效
			{
				continue;
			}			
		}
        
		// 往上查询对应的父节点
		while(lpParent != lpFixCdtGroupRoot)
		{
			if(lpParent->dwGroupID == lpCdtInfo->dwParentID)
			{
				break;	// 找到，则退出
			}
			
			// 没有找到，则继续往上找
			lpParent = lpParent->lpParent;
		}
		
		if(lpCdtInfo->dwGroupID == 0)	// 条件信息
		{
			if(lpParent == lpFixCdtGroupRoot)	// 如果是根下的条件，且不是我的条件，则需要为其建立一个同名扩展条件组
			{
				if(btFCType == FCTYPE_MINE)	// 我的条件，则需要增加或修改未分组信息
				{
					if(lpFCMineUngrouped == NULL)	// 没有，则增加
					{
						lpFCMineUngrouped = new FIXCONDITIONGROUP;
						lpFCMineUngrouped->lpParent = lpParent;
						lpFCMineUngrouped->dwGroupID = CDTID_MINEGROUP_UNGROUPED;
						lpFCMineUngrouped->SetGroupName("未分组条件");
					}
					lpParent = lpFCMineUngrouped;
				}
				else
				{
					lpItem = new FIXCONDITIONGROUP;
					lpItem->lpParent = lpParent;
					lpItem->dwGroupID = CDTID_EXTENDGROUP;
					lpItem->SetCdtID(lpCdtInfo->cdtID);
					lpParent->stSubItemPointArray.Add(lpItem);
					lpParent = lpItem;
				}
			}
            
			lpItem = new FIXCONDITIONGROUP;
			lpItem->lpParent = lpParent;
			lpItem->dwGroupID = lpCdtInfo->dwGroupID;
			lpItem->SetCdtID(lpCdtInfo->cdtID);
			lpParent->stSubItemPointArray.Add(lpItem);
		}
		else	// 组信息
		{
			// 插入组信息
			lpItem = new FIXCONDITIONGROUP;
			lpItem->lpParent = lpParent;
			lpItem->dwGroupID = lpCdtInfo->dwGroupID;
			lpItem->SetGroupName(lpCdtInfo->szName);
			lpParent->stSubItemPointArray.Add(lpItem);
			lpParent = lpItem;
            
			if(lpItem->dwGroupID == CDTID_FIXGROUP_TESTNUMS && btFCType != FCTYPE_SYSTEM_CDTEXT)
			{
				lpFCGroupTestNums = lpItem;		// 试机号条件组
			}
			else if(lpItem->dwGroupID >= CDTID_FIXGROUP_AREA_BEGIN && lpItem->dwGroupID <= CDTID_FIXGROUP_AREA_END)
			{
				// 分区
				_LoadAreaConditions(lpItem, BYTE(lpItem->dwGroupID - CDTID_FIXGROUP_AREA_BEGIN));
			}
		}
	}
    
	if(lpFCMineUngrouped != NULL)	// 增加未分组条件组
	{
		lpFixCdtGroupRoot->stSubItemPointArray.Add(lpFCMineUngrouped);
	}
	
	// 增加扩展条件
	if(btFCType != FCTYPE_MINE)
	{
		// 获取扩展条件信息
		CTypedPtrArray<CPtrArray, LPCDTID> cdtExtendIDPointArray;
		_GetExtendConditions(cdtExtendIDPointArray, bDefault);
        
		int nCount = cdtExtendIDPointArray.GetSize();
        
		// 将扩展条件信息放置到相应位置
		LPCDTID lpCdtID = NULL;
        
		for(i=0; i<nCount; i++)
		{
			lpCdtID = cdtExtendIDPointArray.GetAt(i);
			if(!g_pIConditionMan->IsValidCondition(*lpCdtID))
			{
				continue;	// 无效条件，则忽略
			}
            
			if(!_AddExtendConditionToConditionGroup(lpFixCdtGroupRoot, *lpCdtID, lpFCGroupTestNums))
			{
				if(lpCdtID->cdtIID.btType & CDTIID_TYPE_DATAAREA || lpCdtID->cdtIPID.btType & CDTIPID_TYPE_DATAAREA  || lpCdtID->cdtIPID2.btType & CDTIPID_TYPE_DATAAREA)
				{
					// 号码分区，则忽略
				}
				else
				{
					ASSERT(FALSE);
				}
			}
		}
        
		// 整理系统固定条件组，以便更好的显示
		_TrimSysConditionGroup(lpFixCdtGroupRoot, btFCType);
	}
}

void CConditionInfo::_ReleaseFixConditionGroup(LPFIXCONDITIONGROUP lpFixCdtGroup)
{
	if(lpFixCdtGroup == NULL)
	{
		return ;
	}
	if((DWORD)lpFixCdtGroup == 0xfeeefeee)
	{
		ASSERT(FALSE);
	}
    
	for(int i=lpFixCdtGroup->stSubItemPointArray.GetSize()-1; i>=0; i--)
	{
		_ReleaseFixConditionGroup(lpFixCdtGroup->stSubItemPointArray.GetAt(i));
		delete lpFixCdtGroup->stSubItemPointArray.GetAt(i);
	}
	lpFixCdtGroup->stSubItemPointArray.RemoveAll();
}

BOOL CConditionInfo::_AddExtendConditionToConditionGroup(LPFIXCONDITIONGROUP lpFixCdtGroup, CDTID &cdtExtendCdtID, LPFIXCONDITIONGROUP lpFCGroupTestNums)
{	
	int nCount = lpFixCdtGroup->stSubItemPointArray.GetSize();
	if(lpFixCdtGroup->dwGroupID != 0) // 条件组，则遍历下面的子元素
	{	
		for(int i=0; i<nCount; i++)
		{
			if(_AddExtendConditionToConditionGroup(lpFixCdtGroup->stSubItemPointArray[i], cdtExtendCdtID, lpFCGroupTestNums))
			{
				return TRUE;
			}
		}
		
		return FALSE;	
	}
	
	// 为条件，则需要进行相关处理
	LPFIXCONDITIONGROUP lpItem = NULL;
	if(lpFCGroupTestNums != NULL && g_pIConditionMan->IsTestNumsCondition(cdtExtendCdtID))
	{
		// 试机号相关条件，则统一放到试机号条件组中
		if(!_AddExtendConditionToTestNumsConditionGroup(cdtExtendCdtID, lpFCGroupTestNums))
		{
			// 如果没有找到，则直接在该组中新建一个
			DWORD dwTestNumsDefIPID = 0;
			if(cdtExtendCdtID.cdtIPID.btType & CDTIPID_TYPE_TESTNUMS)
			{
				dwTestNumsDefIPID = cdtExtendCdtID.cdtIPID.dwSystemIPID;
			}
			else if(cdtExtendCdtID.cdtIPID2.btType & CDTIPID_TYPE_TESTNUMS)
			{
				dwTestNumsDefIPID = cdtExtendCdtID.cdtIPID2.dwSystemIPID;
			}
            
			int nIndex = dwTestNumsDefIPID - IPID_D_TESTNUMS_BEGIN + 1;
			ASSERT(nIndex >= 1 && nIndex < 10);
            
			lpItem = new FIXCONDITIONGROUP;
			lpItem->lpParent = lpFCGroupTestNums->stSubItemPointArray[nIndex];
			lpItem->dwGroupID = 0;
			lpItem->SetCdtID(cdtExtendCdtID);
			lpFCGroupTestNums->stSubItemPointArray[nIndex]->stSubItemPointArray.Add(lpItem);
		}
		return TRUE;
	}
    
	// 判断是否是该条件的扩展条件
	if(!cdtExtendCdtID.IsExtendOf(lpFixCdtGroup->GetCdtID()))
	{
		return FALSE;	// 非扩展条件
	}
    
	// 是该条件的扩展条件，则进行添加
	if(lpFixCdtGroup->lpParent->dwGroupID & CDTGID_TYPE_EXTEND && lpFixCdtGroup->lpParent->GetCdtID() == lpFixCdtGroup->GetCdtID())
	{
		// 该条件的父组是扩展组条件，并且和父组同属一个条件，则直接添加到父组
		lpItem = new FIXCONDITIONGROUP;
		lpItem->lpParent = lpFixCdtGroup->lpParent;
		ASSERT(lpItem->lpParent != NULL);
		lpItem->dwGroupID = 0;
		lpItem->SetCdtID(cdtExtendCdtID);
		lpFixCdtGroup->lpParent->stSubItemPointArray.Add(lpItem);
	}
	else	// 将条件变为条件组，并添加自身到组的第一个成员
	{
		lpFixCdtGroup->dwGroupID = CDTID_EXTENDGROUP;
		lpFixCdtGroup->stSubItemPointArray.SetSize(2);
		
		lpItem = new FIXCONDITIONGROUP;
		lpItem->lpParent = lpFixCdtGroup;
		lpItem->dwGroupID = 0;
		lpItem->SetCdtID(lpFixCdtGroup->GetCdtID());
		lpFixCdtGroup->stSubItemPointArray.SetAt(0, lpItem);
		
		// 然后添加该扩展条件
		lpItem = new FIXCONDITIONGROUP;
		lpItem->lpParent = lpFixCdtGroup;
		lpItem->dwGroupID = 0;
		lpItem->SetCdtID(cdtExtendCdtID);
		lpFixCdtGroup->stSubItemPointArray.SetAt(1, lpItem);
	}
    
	return TRUE;
}

BOOL CConditionInfo::_AddExtendConditionToTestNumsConditionGroup(CDTID &cdtExtendCdtID, LPFIXCONDITIONGROUP lpFCGroupTestNums)
{
	if(lpFCGroupTestNums == NULL)
	{
		return FALSE;
	}
    
	int nCount = lpFCGroupTestNums->stSubItemPointArray.GetSize();
	if(lpFCGroupTestNums->dwGroupID != 0) // 条件组，则遍历下面的子元素
	{	
		for(int i=0; i<nCount; i++)
		{
			if(_AddExtendConditionToTestNumsConditionGroup(cdtExtendCdtID, lpFCGroupTestNums->stSubItemPointArray[i]))
			{
				return TRUE;
			}
		}
		
		return FALSE;	
	}
    
	// 判断是否是该条件的扩展条件
	if(!cdtExtendCdtID.IsExtendOf(lpFCGroupTestNums->GetCdtID()))
	{
		return FALSE;	// 非扩展条件
	}
	
	// 是该条件的扩展条件，则进行添加
	LPFIXCONDITIONGROUP lpItem = NULL;
	if(lpFCGroupTestNums->lpParent->dwGroupID & CDTGID_TYPE_EXTEND && lpFCGroupTestNums->lpParent->GetCdtID() == lpFCGroupTestNums->GetCdtID())
	{
		// 该条件的父组是扩展组条件，并且和父组同属一个条件，则直接添加到父组
		lpItem = new FIXCONDITIONGROUP;
		lpItem->lpParent = lpFCGroupTestNums->lpParent;
		ASSERT(lpItem->lpParent != NULL);
		lpItem->dwGroupID = 0;
		lpItem->SetCdtID(cdtExtendCdtID);
		lpFCGroupTestNums->lpParent->stSubItemPointArray.Add(lpItem);
	}
	else	// 将条件变为条件组，并添加自身到组的第一个成员
	{
		lpFCGroupTestNums->dwGroupID = CDTID_EXTENDGROUP;
		lpFCGroupTestNums->stSubItemPointArray.SetSize(2);
		
		lpItem = new FIXCONDITIONGROUP;
		lpItem->lpParent = lpFCGroupTestNums;
		ASSERT(lpItem->lpParent != NULL);
		lpItem->dwGroupID = 0;
		lpItem->SetCdtID(lpFCGroupTestNums->GetCdtID());
		lpFCGroupTestNums->stSubItemPointArray.SetAt(0, lpItem);
		
		// 然后添加该扩展条件
		lpItem = new FIXCONDITIONGROUP;
		lpItem->lpParent = lpFCGroupTestNums;
		ASSERT(lpItem->lpParent != NULL);
		lpItem->dwGroupID = 0;
		lpItem->SetCdtID(cdtExtendCdtID);
		lpFCGroupTestNums->stSubItemPointArray.SetAt(1, lpItem);
	}
    
	return TRUE;
}

void CConditionInfo::_TrimSysConditionGroup(LPFIXCONDITIONGROUP lpFixCdtGroup, BYTE btFCType)
{
	if(lpFixCdtGroup == NULL || lpFixCdtGroup->dwGroupID == 0)
	{
		return ;
	}
    
	int nSubCount = lpFixCdtGroup->stSubItemPointArray.GetSize();
	for(int i=nSubCount-1; i>=0; i--)
	{
		_TrimSysConditionGroup(lpFixCdtGroup->stSubItemPointArray[i], btFCType);
	}
    
	nSubCount = lpFixCdtGroup->stSubItemPointArray.GetSize();
	if(nSubCount == 0)	// 没有子单元，则需要删除本身
	{
		if(lpFixCdtGroup->lpParent != NULL)	// 如果存在父组，则从父组中删除
		{
			for(int i=lpFixCdtGroup->lpParent->stSubItemPointArray.GetSize()-1; i>=0; i--)
			{
				if(lpFixCdtGroup->lpParent->stSubItemPointArray.GetAt(i) == lpFixCdtGroup)
				{
					lpFixCdtGroup->lpParent->stSubItemPointArray.RemoveAt(i);
					delete lpFixCdtGroup;
					break;
				}
				ASSERT(i >= 0);
			}
		}
		else	// 没有父组，则直接删除
		{
			delete lpFixCdtGroup;
		}
	}
	else if(nSubCount == 1)	// 如果只有一个子单元
	{
		LPFIXCONDITIONGROUP lpSubItem = lpFixCdtGroup->stSubItemPointArray[0];
		if(!lpSubItem->IsCondition())	// 如果该子单元为条件组，则将自己的子单元交给父组并删除自己
		{
			lpSubItem->lpParent = lpFixCdtGroup->lpParent;
			if(btFCType == FCTYPE_SYSTEM)	// 系统条件（不用于条件扩展）则需要将父组信息给与子组
			{
				lpSubItem->dwGroupID = lpFixCdtGroup->dwGroupID;
				if(lpFixCdtGroup->IsCdtIDValid())
				{
					lpSubItem->SetCdtID(lpFixCdtGroup->GetCdtID());
				}
				else
				{
					lpSubItem->SetGroupName(lpFixCdtGroup->GetName());
				}
			}
			if(lpFixCdtGroup->lpParent != NULL)
			{
				for(int i=lpFixCdtGroup->lpParent->stSubItemPointArray.GetSize()-1; i>=0; i--)
				{
					if(lpFixCdtGroup->lpParent->stSubItemPointArray.GetAt(i) == lpFixCdtGroup)
					{
						lpFixCdtGroup->lpParent->stSubItemPointArray.SetAt(i, lpSubItem);
						break;
					}
				}
				ASSERT(i >= 0);
			}
			delete lpFixCdtGroup;
		}
	}
}
*/
void CConditionInfo::_LoadMineConditions()
{
	// 装载用户配置
/*	int nBufLen = 0;
	LPCONDITIONINFO lpCdtInfo = (LPCONDITIONINFO)g_pIConfig->GetConfigBuffer(CFGID_CONDITION_MINE, &nBufLen);
	int nCount = nBufLen / sizeof(CONDITIONINFO);
    
	if(nCount > 0)
	{
		m_cMineConditionInfoPointArray.SetSize(nCount);
		memcpy(m_cMineConditionInfoPointArray.GetData(), lpCdtInfo, nCount * sizeof(CONDITIONINFO));
	}*/
}

void CConditionInfo::_LoadSysConditions(CDWordArray &cSysConditionInfoPointArray, BOOL bBase)
{
	if(cSysConditionInfoPointArray.GetSize() > 0)
	{
		return ;		// 已经装载过，则忽略
	}
    
	int i = 0, j = 0, k = 0, nTemp = 0, nNumberCount = g_pIData->GetNumberCount();
	char szTemp[128] = {0};
    
 	BEGIN_LOAD()
    if (bBase)  // 基本条件
    {
        for(i=0; i<nNumberCount; i++)
        {
            BEGIN_ADD_GROUP(g_pICstPubFunc->GetPosName(i), CDTID_FIXGROUP_POS)   // 第n位号码
                ADD_CONDITION(IID_STC_POS_1 + i)						
            END_ADD_GROUP()
        }
        
    }
    else        // 高级条件
    {
        
    }
    END_LOAD()
 /*
        BEGIN_ADD_GROUP("数字", CDTID_FIXGROUP_NUMBERS)
            BEGIN_ADD_GROUP("任意码", CDTID_FIXGROUP_NUMBERS_ANY)
                ADD_CONDITION(IID_STC_POS_ANY)						// 任意单码
                ADD_CONDITION(IID_STC_TWONUM_ANY)					// 任意二码
            END_ADD_GROUP()
            BEGIN_ADD_GROUP("单选二码", CDTID_FIXGROUP_NUMBERS_TWONUMSIN)
                ADD_CONDITION(IID_STC_TWONUMSIN_ANY)				// 任意单选二码
                for(i=0; i<7; i++)
                {
                    for(j=i+1; j<7; j++)
                    {
                        ADD_CONDITION(IID_STC_TWONUMSIN_MN + i * 10 + j)	// 指定位置的单选二码
                    }
                }
            END_ADD_GROUP()
            BEGIN_ADD_GROUP("数字个数", CDTID_FIXGROUP_NUMBERS_COUNT)
            for(int i=0; i<10; i++)
            {
                ADD_CONDITION(IID_STC_NUMBER_0NUM + i)				// 数字N个数
            }
            END_ADD_GROUP() 
        END_ADD_GROUP()
  
   BEGIN_ADD_GROUP("相同数字", CDTID_FIXGROUP_NUMBERS_SAME)
    ADD_CONDITION(IID_STC_NUMBER_SAMECOMBO)				// 相同数字组合
    ADD_CONDITION(IID_STC_NUMBER_MAXCOUNT)				// 任意数字最大出现个数
    ADD_CONDITION(IID_STC_NUMBER_SAMEGROUP_2)			// 2个相同数字组数
    ADD_CONDITION(IID_STC_NUMBER_SAMEGROUP_3)			// 3个相同数字组数
    ADD_CONDITION(IID_STC_NUMBER_SAMEGROUP_4)			// 4个相同数字组数
    ADD_CONDITION(IID_STC_NUMBER_SAMEGROUP_5)			// 5个相同数字组数
    ADD_CONDITION(IID_STC_NUMBER_SAMEGROUP_6)			// 6个相同数字组数
    ADD_CONDITION(IID_STC_NUMBER_SAMEGROUP_7)			// 7个相同数字组数
    ADD_CONDITION(IID_STC_NUMBER_SAMEGROUP_8)			// 8个相同数字组数
    END_ADD_GROUP()
    END_ADD_GROUP()
    
    BEGIN_ADD_GROUP("类型", CDTID_FIXGROUP_GENRE)
    ADD_CONDITION(IID_STC_GENRE_NORMAL)						// 类型
    ADD_CONDITION(IID_STC_GENRE_TRANS)						// 变换类型
    ADD_CONDITION(IID_STC_GENRE_TWOLINK)					// 二连码
    END_ADD_GROUP()
    
    BEGIN_ADD_GROUP("形态", CDTID_FIXGROUP_SHAPE)
    ADD_CONDITION(IID_STC_SHAPE_NORMAL)						// 形态
    ADD_CONDITION(IID_STC_SHAPE_EX)							// 细分形态
    ADD_CONDITION(IID_STC_SHAPE_EQUALDIFF)					// 等差形
    ADD_CONDITION(IID_STC_SHAPE_AAB)						// 组三形态
    ADD_CONDITION(IID_STC_SHAPE_ABC)						// 顺子形态
    ADD_CONDITION(IID_STC_SHAPE_ABD)						// 半顺形态
    ADD_CONDITION(IID_STC_SHAPE_COUNT_SAMENUM)				// 相同号码个数
    ADD_CONDITION(IID_STC_SHAPE_COUNT_SAMEGRO)				// 相同号码组数
    ADD_CONDITION(IID_STC_SHAPE_COUNT_DIFFNUM)				// 不同号码个数
    END_ADD_GROUP()
    
    BEGIN_ADD_GROUP("斜连值", CDTID_FIXGROUP_XL)
    ADD_CONDITION(IID_DNC_XL_VALUE)							// 斜连值
    ADD_CONDITION(IID_DNC_XL_LEFT)							// 左斜连
    ADD_CONDITION(IID_DNC_XL_RIGHT)							// 右斜连
    ADD_CONDITION(IID_DNC_XL_COMBO)							// 左右斜连组合
    END_ADD_GROUP()
    
    BEGIN_ADD_GROUP("连号", CDTID_FIXGROUP_LINK)
    ADD_CONDITION(IID_STC_LINK_COMBO)						// 连号组合
    ADD_CONDITION(IID_STC_LINK_NUMCOUNT)					// 连号个数
    ADD_CONDITION(IID_STC_LINK_GROCOUNT)					// 连号组数
    ADD_CONDITION(IID_STC_LINK_NUMCOUNT_AS)					// 升序连号个数
    ADD_CONDITION(IID_STC_LINK_GROCOUNT_AS)					// 升序连号组数
    ADD_CONDITION(IID_STC_LINK_NUMCOUNT_DS)					// 降序连号个数
    ADD_CONDITION(IID_STC_LINK_GROCOUNT_DS)					// 降序连号组数
    ADD_CONDITION(IID_STC_LINK_COUNT_ODD)					// 奇连个数
    ADD_CONDITION(IID_STC_LINK_COUNT_EVEN)					// 偶连个数
    ADD_CONDITION(IID_STC_LINK_COUNT_PRIME)					// 质连个数
    ADD_CONDITION(IID_STC_LINK_COUNT_COMPOSITE)				// 合连个数
    ADD_CONDITION(IID_STC_LINK_POS_2)						// 2连号位置
    ADD_CONDITION(IID_STC_LINK_POS_3)						// 3连号位置
    ADD_CONDITION(IID_STC_LINK_POS_4)						// 4连号位置
    ADD_CONDITION(IID_STC_LINK_POS_5)						// 5连号位置
    ADD_CONDITION(IID_STC_LINK_POS_6)						// 6连号位置
    ADD_CONDITION(IID_STC_LINK_POS_7)						// 7连号位置
    END_ADD_GROUP()
    
    BEGIN_ADD_GROUP("按位", CDTID_FIXGROUP_POS)
    for(i=0; i<8; i++)
    {
        ADD_CONDITION(IID_STC_POS_1 + i)						// 第n位号码
    }
    END_ADD_GROUP()
    
    BEGIN_ADD_GROUP("奇偶", CDTID_FIXGROUP_ODDENEN)
    ADD_CONDITION(IID_STC_ODDENEN_COUNT_O)					// 奇数个数
    ADD_CONDITION(IID_STC_ODDENEN_COUNT_E)					// 偶数个数
    ADD_CONDITION(IID_STC_ODDENEN_ARR)						// 奇偶排列
    ADD_CONDITION(IID_STC_ODDENEN_ARR_VALUE)				// 奇偶排列值
    ADD_CONDITION(IID_STC_ODDENEN_LINK)						// 奇偶连
    END_ADD_GROUP()
    
    BEGIN_ADD_GROUP("大小", CDTID_FIXGROUP_BIGSMALL)
    ADD_CONDITION(IID_STC_BIGSMALL_COUNT_B)					// 大数个数
    ADD_CONDITION(IID_STC_BIGSMALL_COUNT_S)					// 小数个数
    ADD_CONDITION(IID_STC_BIGSMALL_ARR)						// 大小排列
    ADD_CONDITION(IID_STC_BIGSMALL_ARR_VALUE)				// 大小排列值
    END_ADD_GROUP()
    
    BEGIN_ADD_GROUP("质合", CDTID_FIXGROUP_PRIMECOMPOSITE)
    ADD_CONDITION(IID_STC_PRIMECOMPOSITE_COUNT_P)			// 质数个数
    ADD_CONDITION(IID_STC_PRIMECOMPOSITE_COUNT_C)			// 合数个数
    ADD_CONDITION(IID_STC_PRIMECOMPOSITE_ARR)				// 质合排列
    ADD_CONDITION(IID_STC_PRIMECOMPOSITE_ARR_VALUE)			// 质合排列值
    ADD_CONDITION(IID_STC_PRIMECOMPOSITE_LINK)				// 质合连
    END_ADD_GROUP()
    
    BEGIN_ADD_GROUP("012路", CDTID_FIXGROUP_012)
    ADD_CONDITION(IID_STC_012_COUNT_0)						// 0路个数
    ADD_CONDITION(IID_STC_012_COUNT_1)						// 1路个数
    ADD_CONDITION(IID_STC_012_COUNT_2)						// 2路个数
    ADD_CONDITION(IID_STC_012_SHAPE)						// 012路形态
    ADD_CONDITION(IID_STC_012_ARR_SIN)						// 012路单选排列
    ADD_CONDITION(IID_STC_012_ARR_SIN_VALUE)				// 012路单选排列值
    ADD_CONDITION(IID_STC_012_ARR_GRO)						// 012路组选排列
    ADD_CONDITION(IID_STC_012_ARR_GRO_VALUE)				// 012路组选排列值
    ADD_CONDITION(IID_STC_012_ARR_COUNT)					// 012路个数排列
    ADD_CONDITION(IID_STC_012_ARR_COUNT_VALUE)				// 012路个数排列值
    END_ADD_GROUP()
    
    BEGIN_ADD_GROUP("小中大", CDTID_FIXGROUP_DZX)
    ADD_CONDITION(IID_STC_DZX_COUNT_X)						// X数个数
    ADD_CONDITION(IID_STC_DZX_COUNT_Z)						// Z数个数
    ADD_CONDITION(IID_STC_DZX_COUNT_D)						// D数个数
    ADD_CONDITION(IID_STC_DZX_SHAPE)						// 大中小形态
    ADD_CONDITION(IID_STC_DZX_ARR_SIN)						// 大中小单选排列
    ADD_CONDITION(IID_STC_DZX_ARR_SIN_VALUE)				// 大中小单选排列值
    ADD_CONDITION(IID_STC_DZX_ARR_GRO)						// 大中小组选排列
    ADD_CONDITION(IID_STC_DZX_ARR_GRO_VALUE)				// 大中小组选排列值
    ADD_CONDITION(IID_STC_DZX_ARR_COUNT)					// 大中小个数排列
    ADD_CONDITION(IID_STC_DZX_ARR_COUNT_VALUE)				// 大中小个数排列值
    END_ADD_GROUP()
    
    // 加入动态3态指标
    for(i=0; i<D3ST_COUNT; i++)
    {
        nTemp = i * 100;
        BEGIN_ADD_GROUP(NAMES_3STATE_DYNAMIC[i], GROUPID_3STATE_DYNAMIC[i])
        for(j=0; j<8; j++)
        {
            ADD_CONDITION(IID_DNC_XXX_POS_1 + nTemp + j)	// 第N位3态指标
        }
        ADD_CONDITION(IID_DNC_XXX_COUNT_0 + nTemp)			// 3态指标0个数
        ADD_CONDITION(IID_DNC_XXX_COUNT_1 + nTemp)			// 3态指标1个数
        ADD_CONDITION(IID_DNC_XXX_COUNT_2 + nTemp)			// 3态指标2个数
        ADD_CONDITION(IID_DNC_XXX_SHAPE + nTemp)			// 3态指标形态
        ADD_CONDITION(IID_DNC_XXX_ARR_SIN + nTemp)			// 3态指标单选排列
        ADD_CONDITION(IID_DNC_XXX_ARR_SIN_VALUE + nTemp)	// 3态指标单选排列值
        ADD_CONDITION(IID_DNC_XXX_ARR_GRO + nTemp)			// 3态指标组选排列
        ADD_CONDITION(IID_DNC_XXX_ARR_GRO_VALUE + nTemp)	// 3态指标组选排列值
        ADD_CONDITION(IID_DNC_XXX_ARR_COUNT + nTemp)		// 3态指标个数排列
        ADD_CONDITION(IID_DNC_XXX_ARR_COUNT_VALUE + nTemp)	// 3态指标个数排列值
        END_ADD_GROUP()					
    }
    
    BEGIN_ADD_GROUP("胆合跨", CDTID_FIXGROUP_DHK)
    ADD_CONDITION(IID_STC_DHK_HD)							// 合胆
    ADD_CONDITION(IID_STC_DHK_KD)							// 跨胆
    ADD_CONDITION(IID_STC_DHK_HK)							// 合跨
    ADD_CONDITION(IID_STC_DHK_HDKD)							// 合胆跨胆
    ADD_CONDITION(IID_STC_DHK_HDKDHK)						// 合胆跨胆合跨
    END_ADD_GROUP()
    
    BEGIN_ADD_GROUP("重复号个数", CDTID_FIXGROUP_REPEAT)
    BEGIN_ADD_GROUP("前N期重复号个数", CDTID_FIXGROUP_REPEAT_1TON)
    for(i=0; i<10; i++)
    {
        ADD_CONDITION(IID_DNC_REPEAT_1TON + i)				// 前N期重复号
    }
    END_ADD_GROUP()
    BEGIN_ADD_GROUP("前第N期重复号个数", CDTID_FIXGROUP_REPEAT_N)
    for(i=0; i<10; i++)
    {
        ADD_CONDITION(IID_DNC_REPEAT_N + i)					// 前第N期重复号
    }
    END_ADD_GROUP()
    END_ADD_GROUP()
    
    BEGIN_ADD_GROUP("和值", CDTID_FIXGROUP_SUM)
    ADD_CONDITION(IID_STC_SUM)								// 和值
    ADD_CONDITION(IID_STC_SUMTAIL)							// 合值
    ADD_CONDITION(IID_STC_SUM_N)							// N次和
    END_ADD_GROUP()
    
    BEGIN_ADD_GROUP("跨度", CDTID_FIXGROUP_SPAN)
    ADD_CONDITION(IID_STC_SPAN)								// 跨度
    ADD_CONDITION(IID_STC_SPANTAIL)							// 跨度尾数
    END_ADD_GROUP()
    
    BEGIN_ADD_GROUP("均值", CDTID_FIXGROUP_AVG)
    ADD_CONDITION(IID_STC_AVG)								// 均值
    ADD_CONDITION(IID_STC_AVGTAIL)							// 均值尾数
    END_ADD_GROUP()
    
    BEGIN_ADD_GROUP("AC值", CDTID_FIXGROUP_AC)
    ADD_CONDITION(IID_STC_AC)								// AC值
    END_ADD_GROUP()
    
    BEGIN_ADD_GROUP("散度", CDTID_FIXGROUP_DETRACT)
    ADD_CONDITION(IID_STC_DETRACT)							// 散度
    END_ADD_GROUP()
	
    BEGIN_ADD_GROUP("偏度", CDTID_FIXGROUP_DEPARTURE)
    ADD_CONDITION(IID_DNC_DEPARTURE)						// 偏度
    END_ADD_GROUP()
    
    BEGIN_ADD_GROUP("特码和", CDTID_FIXGROUP_ESPECIALSUM)
    ADD_CONDITION(IID_STC_ESPECIALSUM_ODD)					// 奇码和
    ADD_CONDITION(IID_STC_ESPECIALSUM_EVEN)					// 偶码和
    ADD_CONDITION(IID_STC_ESPECIALSUM_BIG)					// 大码和
    ADD_CONDITION(IID_STC_ESPECIALSUM_SMALL)				// 小码和
    ADD_CONDITION(IID_STC_ESPECIALSUM_PRIME)				// 质码和
    ADD_CONDITION(IID_STC_ESPECIALSUM_COMPOSITE)			// 合码和
    ADD_CONDITION(IID_STC_ESPECIALSUM_0)					// 0码和
    ADD_CONDITION(IID_STC_ESPECIALSUM_1)					// 1码和
    ADD_CONDITION(IID_STC_ESPECIALSUM_2)					// 2码和
    ADD_CONDITION(IID_STC_ESPECIALSUM_D)					// D码和
    ADD_CONDITION(IID_STC_ESPECIALSUM_Z)					// Z码和
    ADD_CONDITION(IID_STC_ESPECIALSUM_X)					// X码和
    END_ADD_GROUP()
    	
    BEGIN_ADD_GROUP("间距", CDTID_FIXGROUP_SPACE)
    BEGIN_ADD_GROUP("单选间距", CDTID_FIXGROUP_SPACE_SIN)
    ADD_CONDITION(IID_STC_SPACE_SIN)						// 单选间距
    ADD_CONDITION(IID_STC_SPACETAIL_SIN)					// 单选间距尾数
    ADD_CONDITION(IID_STC_SPACE_SIN_HEAD)					// 单选首间距
    ADD_CONDITION(IID_STC_SPACE_SIN_TAIL)					// 单选尾间距
    ADD_CONDITION(IID_STC_SPACE_SIN_1)						// 单选第1间距
    ADD_CONDITION(IID_STC_SPACE_SIN_2)						// 单选第2间距
    ADD_CONDITION(IID_STC_SPACE_SIN_3)						// 单选第3间距
    ADD_CONDITION(IID_STC_SPACE_SIN_4)						// 单选第4间距
    ADD_CONDITION(IID_STC_SPACE_SIN_5)						// 单选第5间距
    ADD_CONDITION(IID_STC_SPACE_SIN_6)						// 单选第6间距
    ADD_CONDITION(IID_STC_SPACE_SIN_7)						// 单选第7间距
    ADD_CONDITION(IID_STC_SPACE_SIN_8)						// 单选第8间距
    END_ADD_GROUP()
    BEGIN_ADD_GROUP("组选间距", CDTID_FIXGROUP_SPACE_GRO)
    ADD_CONDITION(IID_STC_SPACE_GRO)						// 组选间距
    ADD_CONDITION(IID_STC_SPACE_GRO_HEAD)					// 组选首间距
    ADD_CONDITION(IID_STC_SPACE_GRO_TAIL)					// 组选尾间距
    ADD_CONDITION(IID_STC_SPACE_GRO_1)						// 组选第1间距
    ADD_CONDITION(IID_STC_SPACE_GRO_2)						// 组选第2间距
    ADD_CONDITION(IID_STC_SPACE_GRO_3)						// 组选第3间距
    ADD_CONDITION(IID_STC_SPACE_GRO_4)						// 组选第4间距
    ADD_CONDITION(IID_STC_SPACE_GRO_5)						// 组选第5间距
    ADD_CONDITION(IID_STC_SPACE_GRO_6)						// 组选第6间距
    ADD_CONDITION(IID_STC_SPACE_GRO_7)						// 组选第7间距
    ADD_CONDITION(IID_STC_SPACE_GRO_8)						// 组选第8间距
    END_ADD_GROUP()
    END_ADD_GROUP()
    
    BEGIN_ADD_GROUP("1D", CDTID_FIXGROUP_1D)
    ADD_CONDITION(IID_STC_1D_POSITIVE_SIN)					// 单选正1D
    ADD_CONDITION(IID_STC_1D_POSITIVE_GRO)					// 组选正1D
    ADD_CONDITION(IID_STC_1D_NEGATIVE_SIN)					// 单选负1D
    ADD_CONDITION(IID_STC_1D_NEGATIVE_GRO)					// 组选负1D
    ADD_CONDITION(IID_STC_1D_RPOSITIVE_SIN)					// 单选反正1D
    ADD_CONDITION(IID_STC_1D_RPOSITIVE_GRO)					// 组选反正1D
    ADD_CONDITION(IID_STC_1D_RNEGATIVE_SIN)					// 单选反负1D
    ADD_CONDITION(IID_STC_1D_RNEGATIVE_GRO)					// 组选反负1D
    END_ADD_GROUP()

    // 加入邻期关系指标
    for(i=0; i<PDT_COUNT; i++)
    {
        nTemp = i * 100;
        BEGIN_ADD_GROUP(NAMES_PREVDATA[i], CDTID_FIXGROUP_PREVDATA_ITEM + i)
        for(j=0; j<8; j++)
        {
            ADD_CONDITION(IID_DNC_PREVDATA_POS_1 + nTemp + j)		// 指定位邻期关系
        }
        ADD_CONDITION(IID_DNC_PREVDATA_ANY + nTemp)					// 任意邻期关系
        ADD_CONDITION(IID_DNC_PREVDATA_MIN + nTemp)					// 最小邻期关系
        ADD_CONDITION(IID_DNC_PREVDATA_MID + nTemp)					// 中间邻期关系
        ADD_CONDITION(IID_DNC_PREVDATA_MAX + nTemp)					// 最大邻期关系
        ADD_CONDITION(IID_DNC_PREVDATA_SUM + nTemp)					// 邻期关系和
        ADD_CONDITION(IID_DNC_PREVDATA_SUMTAIL + nTemp)				// 邻期关系合
        ADD_CONDITION(IID_DNC_PREVDATA_ARR_SIN + nTemp)				// 邻期关系单选排列
        ADD_CONDITION(IID_DNC_PREVDATA_ARR_GRO + nTemp)				// 邻期关系组选排列
        END_ADD_GROUP()
    }
    
    // 加入首尾和和首尾边距指标
    BEGIN_ADD_GROUP("首尾关系", CDTID_FIXGROUP_HEADTAIL)
    ADD_CONDITION(IID_STC_TWOPOS_HEADTAIL + TPT_SUM * 100)			// 首尾和
    ADD_CONDITION(IID_STC_TWOPOS_HEADTAIL + TPT_SUM_TAIL * 100)		// 首尾合
    ADD_CONDITION(IID_STC_TWOPOS_HEADTAIL + TPT_SUB * 100)			// 首尾边距
    ADD_CONDITION(IID_STC_TWOPOS_HEADTAIL + TPT_SUB_TAIL * 100)		// 首尾边距尾数
	
    ADD_CONDITION(IID_STC_TWOPOS_HEADTAIL + TPT_CIRSUB * 100)		// 首尾环差
    ADD_CONDITION(IID_STC_TWOPOS_HEADTAIL + TPT_CIRSUB_TAIL * 100)	// 首尾环差尾数

    END_ADD_GROUP()
    
    BEGIN_ADD_GROUP("邻位间距", CDTID_FIXGROUP_NEARSPACE)
    ADD_CONDITION(IID_STC_NEARSPACE_12)						// 12邻位间距
    ADD_CONDITION(IID_STC_NEARSPACE_23)						// 23邻位间距
    ADD_CONDITION(IID_STC_NEARSPACE_34)						// 34邻位间距
    ADD_CONDITION(IID_STC_NEARSPACE_45)						// 45邻位间距
    ADD_CONDITION(IID_STC_NEARSPACE_56)						// 56邻位间距
    ADD_CONDITION(IID_STC_NEARSPACE_67)						// 67邻位间距
    ADD_CONDITION(IID_STC_NEARSPACE_78)						// 78邻位间距
    ADD_CONDITION(IID_STC_NEARSPACE_HT)						// 首尾邻位间距
    ADD_CONDITION(IID_STC_NEARSPACE_MAX)					// 最大邻位间距
    ADD_CONDITION(IID_STC_NEARSPACE_MAXEX)					// 最大邻位间距(含首尾)
    ADD_CONDITION(IID_STC_NEARSPACE_MIN)					// 最小邻位间距
    ADD_CONDITION(IID_STC_NEARSPACE_MINEX)					// 最小邻位间距(含首尾)
    ADD_CONDITION(IID_STC_NEARSPACE_AVG)					// 平均邻位间距
    ADD_CONDITION(IID_STC_NEARSPACE_ANY)					// 任意邻位间距
    END_ADD_GROUP()
    
    // 加入两位关系指标
    for(i=0; i<TPT_COUNT; i++)
    {
        nTemp = i * 100;
        BEGIN_ADD_GROUP(NAMES_TWOPOS[i], CDTID_FIXGROUP_TWOPOS_ITEM + i)
        for(j=0; j<7; j++)
        {
            for(k=j+1; k<8; k++)
            {
                ADD_CONDITION(IID_STC_TWOPOS_MN + nTemp + j * 10 + k)		// 指定两位关系
            }
        }
        ADD_CONDITION(IID_STC_TWOPOS_MIN + nTemp)						// 最小两位关系
        ADD_CONDITION(IID_STC_TWOPOS_MID + nTemp)						// 中间两位关系（仅用于3个号码）
        ADD_CONDITION(IID_STC_TWOPOS_MAX + nTemp)						// 最大两位关系
        ADD_CONDITION(IID_STC_TWOPOS_ANY + nTemp)						// 任意两位关系
        ADD_CONDITION(IID_STC_TWOPOS_ARR_SIN + nTemp)					// 两位关系单排
        ADD_CONDITION(IID_STC_TWOPOS_ARR_GRO + nTemp)					// 两位关系组排
        END_ADD_GROUP()
    }
    
    BEGIN_ADD_GROUP("单选遗漏", CDTID_FIXGROUP_SKIPSIN)
    for(i=0; i<8; i++)
    {
        ADD_CONDITION(IID_DNC_SKIPSIN_1 + i)					// 第n位单选遗漏
    }
    ADD_CONDITION(IID_DNC_SKIPSIN_MIN)						// 最小单选遗漏
    ADD_CONDITION(IID_DNC_SKIPSIN_MAX)						// 最大单选遗漏
    
    for(i=0; i<8; i++)
    {
        ADD_CONDITION(IID_DNC_SKIPSIN_TAIL_1 + i)				// 第n位单选遗漏尾
    }
    ADD_CONDITION(IID_DNC_SKIPSIN_TAIL_ANY)					// 任意单选遗漏尾
    ADD_CONDITION(IID_DNC_SKIPSIN_TAIL_MIN)					// 最小单选遗漏尾
    ADD_CONDITION(IID_DNC_SKIPSIN_TAIL_MAX)					// 最大单选遗漏尾
    
    BEGIN_ADD_GROUP("单选遗漏尾排列", CDTID_FIXGROUP_SKIPSIN_TAILARR)
    ADD_CONDITION(IID_DNC_SKIPSIN_TAIL_ARR_SIN)			// 单选遗漏尾单选排列
    ADD_CONDITION(IID_DNC_SKIPSIN_TAIL_ARR_GRO)			// 单选遗漏尾组选排列
    END_ADD_GROUP()
    
    ADD_CONDITION(IID_DNC_SKIPSIN_SUM)						// 单选遗漏和值
    ADD_CONDITION(IID_DNC_SKIPSIN_SUM_AREA)					// 单选遗漏和值分区
    ADD_CONDITION(IID_DNC_SKIPSIN_SUM_AREA10)				// 单选遗漏和值10分区
    ADD_CONDITION(IID_DNC_SKIPSIN_SUMTAIL)					// 单选遗漏合值
    END_ADD_GROUP()
    
    BEGIN_ADD_GROUP("组选遗漏", CDTID_FIXGROUP_SKIPGRO)
    for(i=0; i<8; i++)
    {
        ADD_CONDITION(IID_DNC_SKIPGRO_1 + i)					// 第n位组选遗漏
    }	
    ADD_CONDITION(IID_DNC_SKIPGRO_MIN)						// 最小组选遗漏
    ADD_CONDITION(IID_DNC_SKIPGRO_MAX)						// 最大组选遗漏
    
    for(i=0; i<8; i++)
    {
        ADD_CONDITION(IID_DNC_SKIPGRO_TAIL_1 + i)				// 第n位组选遗漏尾
    }
    ADD_CONDITION(IID_DNC_SKIPGRO_TAIL_ANY)					// 任意组选遗漏尾
    ADD_CONDITION(IID_DNC_SKIPGRO_TAIL_MIN)					// 最小组选遗漏尾
    ADD_CONDITION(IID_DNC_SKIPGRO_TAIL_MAX)					// 最大组选遗漏尾
    
    BEGIN_ADD_GROUP("组选遗漏尾排列", CDTID_FIXGROUP_SKIPGROTAILARR)
    ADD_CONDITION(IID_DNC_SKIPGRO_TAIL_ARR_SIN)			// 组选遗漏尾单选排列
    ADD_CONDITION(IID_DNC_SKIPGRO_TAIL_ARR_GRO)			// 组选遗漏尾组选排列
    END_ADD_GROUP()
    
    ADD_CONDITION(IID_DNC_SKIPGRO_SUM)						// 组选遗漏和值
    ADD_CONDITION(IID_DNC_SKIPGRO_SUM_AREA)					// 组选遗漏和值分区
    ADD_CONDITION(IID_DNC_SKIPGRO_SUM_AREA10)				// 组选遗漏和值10分区
    ADD_CONDITION(IID_DNC_SKIPGRO_SUMTAIL)					// 组选遗漏合值
    END_ADD_GROUP()
    
    BEGIN_ADD_GROUP("余数", CDTID_FIXGROUP_REMM)
    for(i=2; i<=16; i++)
    {
        nTemp = (i - 2) * 100;
        
        sprintf(szTemp, "除%d余数", i);
        BEGIN_ADD_GROUP(szTemp, CDTID_FIXGROUP_REMM+i-1)
        for(j=0; j<8; j++)
        {
            ADD_CONDITION(IID_STC_REMM_POS_1 + nTemp + j)		// 按位除M余数
        }
        ADD_CONDITION(IID_STC_REMM_POS_ANY + nTemp)				// 除M任意余数
        ADD_CONDITION(IID_STC_REMM_SUM + nTemp)					// 除M余数和
        ADD_CONDITION(IID_STC_REMM_SUMTAIL + nTemp)				// 除M余数合
        ADD_CONDITION(IID_STC_REMM_ARR_SIN + nTemp)				// 除M余数单选排列
        ADD_CONDITION(IID_STC_REMM_ARR_SIN_VALUE + nTemp)		// 除M余数单选排列值
        ADD_CONDITION(IID_STC_REMM_ARR_GRO + nTemp)				// 除M余数组选排列
        ADD_CONDITION(IID_STC_REMM_ARR_GRO_VALUE + nTemp)		// 除M余数组选排列值
        ADD_CONDITION(IID_STC_REMM_ARR_COUNT + nTemp)			// 除M余数个数排列
        ADD_CONDITION(IID_STC_REMM_ARR_COUNT_VALUE + nTemp)		// 除M余数个数排列值
        for(j=0; j<i; j++)
        {
            ADD_CONDITION(IID_STC_REMM_COUNT_N + nTemp + j)		// 除M余N个数（N：0-19）
        }
        if(i == 3)
        {
            ADD_CONDITION(IID_STC_REMM_SHAPE + nTemp)			// 除M余数形态（只用于除3余数）
        }
        ADD_CONDITION(IID_STC_REMM_MAXLINK + nTemp)				// 除M余数最大邻出
        ADD_CONDITION(IID_STC_REMM_MAXNONE + nTemp)				// 除M余数最大邻断
        ADD_CONDITION(IID_STC_REMM_COUNT_NONE + nTemp)			// 除M余数未出个数
        ADD_CONDITION(IID_STC_REMM_COUNT_MATCH + nTemp)			// 除M余数出现个数
        END_ADD_GROUP()
    }
    END_ADD_GROUP()
    
    for(i=2; i<=16; i++)
    {
        sprintf(szTemp, "%d分区", i);
        BEGIN_ADD_GROUP(szTemp, CDTID_FIXGROUP_AREA_BEGIN + i)
        END_ADD_GROUP()
    }
    
    BEGIN_ADD_GROUP("积数", CDTID_FIXGROUP_MUL)
    ADD_CONDITION(IID_STC_MUL)								// 积数
    ADD_CONDITION(IID_STC_MUL_ODDEVEN)						// 积数奇偶零
    ADD_CONDITION(IID_STC_MUL_BIGSMALL)						// 积数大小零
    ADD_CONDITION(IID_STC_MUL_012)							// 积数012路零
    ADD_CONDITION(IID_STC_MULTAIL)							// 积数尾数
    END_ADD_GROUP()
    
    BEGIN_ADD_GROUP("位置积和", CDTID_FIXGROUP_POSMULSUM)
    ADD_CONDITION(IID_STC_POSMULSUM)						// 位置积和
    ADD_CONDITION(IID_STC_POSMULSUMTAIL)					// 位置积合
    END_ADD_GROUP()
    
    BEGIN_ADD_GROUP("反位置积和", CDTID_FIXGROUP_RESPOSMULSUM)
    ADD_CONDITION(IID_STC_RESPOSMULSUM)						// 反位置积和
    ADD_CONDITION(IID_STC_RESPOSMULSUMTAIL)					// 反位置积合
    END_ADD_GROUP()
    
    BEGIN_ADD_GROUP("顺序排列", CDTID_FIXGROUP_ORDER)
    BEGIN_ADD_GROUP("顺序排列按位", CDTID_FIXGROUP_ORDER_POS)
    ADD_CONDITION(IID_STC_ORDER_MIN)						// 最小数
    ADD_CONDITION(IID_STC_ORDER_MID)						// 中间数
    ADD_CONDITION(IID_STC_ORDER_MAX)						// 最大数
    END_ADD_GROUP()
    BEGIN_ADD_GROUP("顺序排列位和", CDTID_FIXGROUP_ORDER_SUM)
    ADD_CONDITION(IID_STC_ORDER_MINADDMID)				// 最小数＋中间数
    ADD_CONDITION(IID_STC_ORDER_MINADDMAX)				// 最小数＋最大数
    ADD_CONDITION(IID_STC_ORDER_MIDADDMAX)				// 中间数＋最大数
    ADD_CONDITION(IID_STC_ORDER_POSSUM_ARR_SIN)			// 顺序位和单选排列
    ADD_CONDITION(IID_STC_ORDER_POSSUM_ARR_GRO)			// 顺序位和组选排列
    END_ADD_GROUP()
    BEGIN_ADD_GROUP("顺序排列位合", CDTID_FIXGROUP_ORDER_SUMTAIL)
    ADD_CONDITION(IID_STC_ORDER_MINADDMID_TAIL)			// 最小数＋中间数尾数
    ADD_CONDITION(IID_STC_ORDER_MINADDMAX_TAIL)			// 最小数＋最大数尾数
    ADD_CONDITION(IID_STC_ORDER_MIDADDMAX_TAIL)			// 中间数＋最大数尾数
    ADD_CONDITION(IID_STC_ORDER_POSSUMTAIL_ARR_SIN)		// 顺序位合单选排列
    ADD_CONDITION(IID_STC_ORDER_POSSUMTAIL_ARR_GRO)		// 顺序位合组选排列
    END_ADD_GROUP()
    BEGIN_ADD_GROUP("顺序排列位差", CDTID_FIXGROUP_ORDER_SUB)
    ADD_CONDITION(IID_STC_ORDER_MIDSUBMIN)				// 中间数－最小数
    ADD_CONDITION(IID_STC_ORDER_MAXSUBMIN)				// 最大数－最小数
    ADD_CONDITION(IID_STC_ORDER_MAXSUBMID)				// 最大数－中间数
    ADD_CONDITION(IID_STC_ORDER_POSSUB_ARR_SIN)			// 顺序位差单选排列
    ADD_CONDITION(IID_STC_ORDER_POSSUB_ARR_GRO)			// 顺序位差组选排列
    END_ADD_GROUP()
    BEGIN_ADD_GROUP("顺序排列位差尾", CDTID_FIXGROUP_ORDER_SUBTAIL)
    ADD_CONDITION(IID_STC_ORDER_MIDSUBMIN_TAIL)			// 中间数－最小数尾数
    ADD_CONDITION(IID_STC_ORDER_MAXSUBMIN_TAIL)			// 最大数－最小数尾数
    ADD_CONDITION(IID_STC_ORDER_MAXSUBMID_TAIL)			// 最大数－中间数尾数
    ADD_CONDITION(IID_STC_ORDER_POSSUBTAIL_ARR_SIN)		// 顺序位差尾数单选排列
    ADD_CONDITION(IID_STC_ORDER_POSSUBTAIL_ARR_GRO)		// 顺序位差尾数组选排列
    END_ADD_GROUP()
    
    BEGIN_ADD_GROUP("相对大中小", CDTID_FIXGROUP_ORDER_RELATIVEDZX)
    ADD_CONDITION(IID_STC_ORDER_RELATIVEDZX_POS_1)		// 百位相对大中小
    ADD_CONDITION(IID_STC_ORDER_RELATIVEDZX_POS_2)		// 十位相对大中小
    ADD_CONDITION(IID_STC_ORDER_RELATIVEDZX_POS_3)		// 个位相对大中小
    ADD_CONDITION(IID_STC_ORDER_RELATIVEDZX_ARR)		// 相对大中小排列
    ADD_CONDITION(IID_STC_ORDER_RELATIVEDZX_ARR_VALUE)	// 相对大中小排列值
    END_ADD_GROUP()

    BEGIN_ADD_GROUP("顺序排列", CDTID_FIXGROUP_ORDER_THAN3)	// 数字个数大于3时使用
    ADD_CONDITION(IID_STC_ORDER_MIN)						// 最小数
    ADD_CONDITION(IID_STC_ORDER_MID)						// 中间数
    ADD_CONDITION(IID_STC_ORDER_MAX)						// 最大数
    ADD_CONDITION(IID_STC_ORDER_MINADDMAX)					// 最小数＋最大数
    ADD_CONDITION(IID_STC_ORDER_MINADDMAX_TAIL)				// 最小数＋最大数尾数
    ADD_CONDITION(IID_STC_ORDER_MAXSUBMIN)					// 最大数－最小数
    ADD_CONDITION(IID_STC_ORDER_MAXSUBMIN_TAIL)				// 最大数－最小数尾数
    END_ADD_GROUP()
    END_ADD_GROUP()
    
    BEGIN_ADD_GROUP("号码", CDTID_FIXGROUP_NUMS)
    ADD_CONDITION(IID_STC_NUMS_SIN)							// 单选号码
    ADD_CONDITION(IID_STC_NUMS_GRO)							// 组选号码
    ADD_CONDITION(IID_DNC_NUMS_MATCHCOUNT_SIN)				// 单选号码出现次数
    ADD_CONDITION(IID_DNC_NUMS_MATCHCOUNT_GRO)				// 组选号码出现次数
    END_ADD_GROUP()
    
    BEGIN_ADD_GROUP("分类二码", CDTID_FIXGROUP_TWONUM_TYPE)
    ADD_CONDITION(IID_STC_TWONUM_Z6)						// 组六分类二码
    ADD_CONDITION(IID_STC_TWONUM_Z6_2)						// 组六分类二码2
    ADD_CONDITION(IID_STC_TWONUM_Z3)						// 组三分类二码
    ADD_CONDITION(IID_STC_TWONUM_Z3_2)						// 组三分类二码2
    END_ADD_GROUP()
    
    BEGIN_ADD_GROUP("试机号", CDTID_FIXGROUP_TESTNUMS)
    BEGIN_ADD_GROUP("试机号和开奖号总体", CDTID_FIXGROUP_TESTNUMS_ITEM_BEGIN)
    ADD_CONDITION(IID_DNC_TESTNUMS_REPEAT_CUR_CUR)			// 当期试机号与当期开奖号重复个数（组选）
    ADD_CONDITION(IID_DNC_TESTNUMS_REPEAT_PRE_CUR)			// 上期试机号与当期开奖号重复个数（组选）
    ADD_CONDITION(IID_DNC_TESTNUMS_REPEAT_COMPLEX)			// 与当期试机号和开奖号综合重复信息
    ADD_CONDITION(IID_DNC_TESTNUMS_REPEAT_COMBO)			// 与当期试机号和开奖号重复组合
    END_ADD_GROUP()
    BEGIN_ADD_GROUP("开奖号与试机号对照", CDTID_FIXGROUP_TESTNUMS_ITEM_BEGIN + 2)
    END_ADD_GROUP()
    BEGIN_ADD_GROUP("开奖号与试机号距离", CDTID_FIXGROUP_TESTNUMS_ITEM_BEGIN + 3)
    END_ADD_GROUP()
//    BEGIN_ADD_GROUP("开奖号与试机号距离尾数", CDTID_FIXGROUP_TESTNUMS_ITEM_BEGIN + 4)
 //   END_ADD_GROUP()
    BEGIN_ADD_GROUP("开奖号与试机号之差", CDTID_FIXGROUP_TESTNUMS_ITEM_BEGIN + 5)
    END_ADD_GROUP()
    BEGIN_ADD_GROUP("开奖号与试机号环差", CDTID_FIXGROUP_TESTNUMS_ITEM_BEGIN + 6)
    END_ADD_GROUP()
 //   BEGIN_ADD_GROUP("开奖号与试机号环差尾数", CDTID_FIXGROUP_TESTNUMS_ITEM_BEGIN + 7)
    END_ADD_GROUP()
    */
	
}

#define		ADD_AREA_CONDITION(wUseTypeIn) { \
                cdtIID.btType = CDTIID_TYPE_DATAAREA | CDTIID_TYPE_CUSTOM | CDTIID_TYPE_VALID; \
                cdtIID.stCustomIID.dwDiffValues[0] = lpDataArea->dwAreaDiffValues[0]; \
                cdtIID.stCustomIID.dwDiffValues[1] = lpDataArea->dwAreaDiffValues[1]; \
                cdtIID.stCustomIID.wUseType = WORD(wUseTypeIn); \
                dtTempID.InitID(cdtIID); \
                if(theApp.m_cIndicator.IsValid(cdtIID, (DWORD)lpDataArea)) { \
                    lpItem = new FIXCONDITIONGROUP; \
                    lpItem->lpParent = lpGroup; \
                    lpItem->dwGroupID = 0; \
                    lpItem->SetCdtID(cdtTempID); \
                    lpGroup->stSubItemPointArray.SetAt(nIndex, lpItem); \
                    nIndex ++; \
                } \
            }

void CConditionInfo::_LoadAreaConditions(LPFIXCONDITIONGROUP lpParent, BYTE btAreaCount)
{
	ASSERT(lpParent != NULL && m_lpDataAreas != NULL);
    
/*	LPFIXCONDITIONGROUP lpItem = NULL, lpGroup = NULL;
	int i = 0, j = 0, nIndex = 0, nNumberCount = g_pIData->GetNumberCount();
	CDTIID cdtIID;
	CDTID cdtTempID;
	LPDATAAREA lpDataArea = NULL;
    
	for(i=0; i<m_nDataAreaCount; i++)
	{
		if(m_lpDataAreas[i].btHide == 0 && m_lpDataAreas[i].btAreaCount == btAreaCount)
		{
			lpDataArea = &m_lpDataAreas[i];
            
			// 插入组信息
			lpGroup = new FIXCONDITIONGROUP;
			lpGroup->lpParent = lpParent;
			lpGroup->dwGroupID = CDTID_FIXGROUP_AREA_DETAILS_BEGIN + i;
			lpGroup->SetGroupName(m_lpDataAreas[i].szName);
			lpParent->stSubItemPointArray.Add(lpGroup);
			lpGroup->stSubItemPointArray.SetSize(50);
			nIndex = 0;
            
			// 插入分区数据信息
			for(j=0; j<nNumberCount; j++)
			{
				ADD_AREA_CONDITION(CIDAUT_POS_1 + j)		// 按位M分区
			}
            ADD_AREA_CONDITION(CIDAUT_POS_ANY)			// 任意M分区
            ADD_AREA_CONDITION(CIDAUT_ARR_SIN)			// M分区单选排列
            ADD_AREA_CONDITION(CIDAUT_ARR_SIN_VALUE)	// M分区单选排列值
            ADD_AREA_CONDITION(CIDAUT_ARR_GRO)			// M分区组选排列		
            ADD_AREA_CONDITION(CIDAUT_ARR_GRO_VALUE)	// M分区组选排列值	
            ADD_AREA_CONDITION(CIDAUT_ARR_COUNT)		// M分区个数排列		
            ADD_AREA_CONDITION(CIDAUT_ARR_COUNT_VALUE)	// M分区个数排列值	
			for(j=0; j<lpDataArea->btAreaCount; j++)
			{
				ADD_AREA_CONDITION(CIDAUT_COUNT_N + j)		// M分区第N分区个数（N：0-19）
			}
            ADD_AREA_CONDITION(CIDAUT_SHAPE)			// M分区形态（只用于3分区）
            ADD_AREA_CONDITION(CIDAUT_MAXLINK)			// M分区最大邻出
            ADD_AREA_CONDITION(CIDAUT_MAXNONE)			// M分区最大邻断
            ADD_AREA_CONDITION(CIDAUT_COUNT_NONE)		// M分区未出个数
            ADD_AREA_CONDITION(CIDAUT_COUNT_MATCH)		// M分区已出个数
            
			ASSERT(nIndex < lpGroup->stSubItemPointArray.GetSize());
			lpGroup->stSubItemPointArray.SetSize(nIndex);
		}
	}*/
}

/*void CConditionInfo::_GetExtendConditions(CTypedPtrArray<CPtrArray, LPCDTID> &cdtExtendIDPointArray, BOOL bDefault)
{
	if(bDefault)
	{
		int nSysExtendCdtIDCount = 0;
		LPCDTID lpSysCdtIDArray = (LPCDTID)g_pIConfig->GetConfigBuffer(CFGID_CONDITION_EXTEND_SYSTEM, &nSysExtendCdtIDCount);
		nSysExtendCdtIDCount /= sizeof(CDTID);
        
		cdtExtendIDPointArray.SetSize(nSysExtendCdtIDCount);
		for(int i=0; i<nSysExtendCdtIDCount; i++)
		{
			cdtExtendIDPointArray.SetAt(i, &lpSysCdtIDArray[i]);
		}
	}
	else
	{
		// 先获取系统扩展条件信息
		int nSysExtendCdtIDCount = 0;
		LPCDTID lpSysCdtIDArray = (LPCDTID)g_pIConfig->GetConfigBuffer(CFGID_CONDITION_EXTEND_SYSTEM, &nSysExtendCdtIDCount);
		nSysExtendCdtIDCount /= sizeof(CDTID);
		
		// 然后获取用户扩展条件并生成新的
		int nCount = 0;
		LPEXTENDCONDITIONINFO lpExtendCdtInfos = (LPEXTENDCONDITIONINFO)g_pIConfig->GetConfigBuffer(CFGID_CONDITION_EXTEND, &nCount);
		nCount /= sizeof(EXTENDCONDITIONINFO);
		
		// 然后进行比较后将有效的数据加入返回会的数组中
		cdtExtendIDPointArray.SetSize(nCount + nSysExtendCdtIDCount);
		int i = 0, j = 0, nIndex = 0;
		BOOL bNeedDel = FALSE;
		
		for(i=0; i<nSysExtendCdtIDCount; i++)
		{
			bNeedDel = FALSE;
			for(j=0; j<nCount; j++)
			{
				if(lpSysCdtIDArray[i] == lpExtendCdtInfos[j].cdtID)	// 相同
				{
					if(lpExtendCdtInfos[j].btType == EXCDT_TYPE_DEL)
					{
						bNeedDel = TRUE;
					}
					else
					{
						lpExtendCdtInfos[j].btType = EXCDT_TYPE_SAME;		// 修改标志，以便后面避免添加重复的
					}
				}
			}
			if(!bNeedDel)	// 无需删除，则添加
			{
				cdtExtendIDPointArray.SetAt(nIndex++, &lpSysCdtIDArray[i]);
			}
		}
		
		// 最后添加用户配置中需要添加的
		for(i=0; i<nCount; i++)
		{
			if(lpExtendCdtInfos[i].btType == EXCDT_TYPE_DEFALUT)	// 缺省标志
			{
				cdtExtendIDPointArray.SetAt(nIndex++, &lpExtendCdtInfos[i].cdtID);
			}
			else if(lpExtendCdtInfos[i].btType == EXCDT_TYPE_SAME)	// 相同的需要恢复标志，且不添加
			{
				lpExtendCdtInfos[i].btType = EXCDT_TYPE_DEFALUT;
			}
		}
		cdtExtendIDPointArray.SetSize(nIndex);
	}
	
	// 排序
	qsort(cdtExtendIDPointArray.GetData(), cdtExtendIDPointArray.GetSize(), sizeof(LPCDTID), _SortExtendCdt);
}

int CConditionInfo::_SortExtendCdt(const void *arg1, const void *arg2)
{
	LPCDTID lpExtCdtID1 = (LPCDTID)(*(LPDWORD)arg1);
	LPCDTID lpExtCdtID2 = (LPCDTID)(*(LPDWORD)arg2);
    
	int nCompValue = memcmp(lpExtCdtID1, lpExtCdtID2, sizeof(CDTID));
	if(nCompValue == 0)
	{
		return 0;
	}
    
	return nCompValue > 0 ? 1 : -1;
}

BOOL CConditionInfo::_IsFCGroupValid(DWORD dwGroupID)
{
	switch(dwGroupID)
	{
        case CDTID_FIXGROUP_ORDER_THAN3: return g_pIData->GetNumberCount() > 3 ? TRUE : FALSE; break;
            
        case CDTID_FIXGROUP_ORDER_POS:
        case CDTID_FIXGROUP_ORDER_SUM:
        case CDTID_FIXGROUP_ORDER_SUMTAIL:
        case CDTID_FIXGROUP_ORDER_SUB:
        case CDTID_FIXGROUP_ORDER_SUBTAIL:
        case CDTID_FIXGROUP_ORDER_RELATIVEDZX:
            return g_pIData->GetNumberCount() == 3 ? TRUE : FALSE;
            break;
	}
    
	return TRUE;
}*/
