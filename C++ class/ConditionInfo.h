//
//  ConditionInfo.h
//  QCLottery
//
//  Created by 武 孙 on 12-5-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once
#include "Def_CfgItem_Condition.h"

// 定义_ConditionInfoToFixConditionGroup中btFCType的取值
#define		FCTYPE_SYSTEM				1				// 系统条件
#define		FCTYPE_MINE					2				// 我的条件
#define		FCTYPE_SYSTEM_CDTEXT		3				// 系统条件（用于条件扩展）

class CConditionInfo  
{
public:
	CConditionInfo();
	virtual ~CConditionInfo();
    
public:
	// 当数据类型改变时
	void OnDataTypeChanged();
	void OnConfigChanged(DWORD dwConfigID);
	LPFIXCONDITIONGROUP GetFixConditionGroup(BOOL bSysCdt){return bSysCdt ? &m_stSysFixCdtGroupRoot : &m_stMineFixCdtGroupRoot;};
    
	// 获取指定条件的扩展条件ID集合，如果lpCdtID为NULL，则获取所有扩展条件
	// 返回扩展条件个数，返回0表示当前没有扩展条件，返回-1表示当前条件已经不能扩展了
//	int GetExtendConditions(LPCDTID lpCdtID, CArray<CDTID, CDTID &> *pExtIDArray);
    
public:	// 用于条件扩展树
	LPFIXCONDITIONGROUP GetFixConditionGroup_ForCdtExtend(BOOL bDefault = FALSE);
	void ReleaseFixConditionGroup_ForCdtExtend();
	
private:	
	void _LoadMineConditions();		// 装载我的条件信息
	void _LoadSysConditions(CDWordArray &cSysConditionInfoPointArray);              // 装载系统条件信息
	void _LoadAreaConditions(LPFIXCONDITIONGROUP lpParent, BYTE btAreaCount);		// 装载分区条件信息
    
/*	void _GetExtendConditions(CDWordArray &cdtExtendIDPointArray, BOOL bDefault = FALSE);
    
	void _ConditionInfoToFixConditionGroup(BYTE btFCType = FCTYPE_SYSTEM, BOOL bDefault = FALSE);
	void _ReleaseFixConditionGroup(LPFIXCONDITIONGROUP lpFixCdtGroup);
	BOOL _AddExtendConditionToConditionGroup(LPFIXCONDITIONGROUP lpFixCdtGroup, CDTID &cdtExtendCdtID, LPFIXCONDITIONGROUP lpFCGroupTestNums);
	BOOL _AddExtendConditionToTestNumsConditionGroup(CDTID &cdtExtendCdtID, LPFIXCONDITIONGROUP lpFCGroupTestNums);
	void _TrimSysConditionGroup(LPFIXCONDITIONGROUP lpFixCdtGroup, BYTE btFCType = FCTYPE_SYSTEM);
    
	static int _SortExtendCdt(const void *arg1, const void *arg2);
    
	BOOL _IsFCGroupValid(DWORD dwGroupID);*/
    
private:
	CDWordArray m_cSysConditionInfoPointArray;			// 系统条件信息组指针集合
	CDWordArray m_cMineConditionInfoPointArray;         // 我的条件信息组指针集合
	CDWordArray m_cSysConditionInfoPointArray_CdtExt;   // 系统条件信息组指针集合（用于条件扩展）
    
	FIXCONDITIONGROUP	m_stSysFixCdtGroupRoot;								// 系统条件固定根组
	FIXCONDITIONGROUP	m_stMineFixCdtGroupRoot;							// 我的条件固定根组
	FIXCONDITIONGROUP	m_stSysFixCdtGroupRoot_CdtExt;						// 系统条件固定根组（用于条件扩展）
	
	int					m_nDataAreaCount;									// 分区数据个数
	LPDATAAREA			m_lpDataAreas;										// 分区数据指针
};
