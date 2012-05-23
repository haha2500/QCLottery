//
//  ConditionMan.h
//  QCLottery
//
//  Created by 武 孙 on 12-5-23.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ICondition.h"
#include "Condition.h"
#include "ConditionInfo.h"
#include "Def_ConditionCustom.h"
#include "Indicator.h"
#include "IndicatorParam.h"

class CConditionMan : public IConditionMan  
{
public:
    
	CConditionMan();
	virtual ~CConditionMan();
    
public:	// 单个条件接口函数
	
	// 获取指定的ID号的条件接口对象
	virtual ICondition * GetICondition(const CDTID &cdtID);
    
	// 获取指定的ID号的条件接口对象（仅用于系统条件）
	virtual ICondition * GetICondition_System(DWORD dwSysDefIID, DWORD dwSysDefIPID = 0, DWORD dwSysDefIPID2 = 0);
    
	// 获取指定的ID号的条件的父级条件ID号，返回值临时有效
	virtual CDTID GetParentConditionID(const CDTID &cdtID);
    
	// 创建和释放临时条件接口对象（用于条件扩展功能）
	virtual ICondition * CreateTempICondition(const CDTID &cdtID, DWORD dwCustomData = 0, ICondition *pIParentCdt = NULL);
	virtual void DeleteTempICondition(ICondition *pITempCondition);
	
	// 获取指定条件名称，bGetShortName为TRUE时获取条件简称，如果是无效ID，则返回NULL，返回值临时有效
	virtual LPCSTR GetConditionName(const CDTID &cdtID, BOOL bGetShortName = FALSE);
	
	// 获取指定指标名称，bGetShortName为TRUE时获取条件简称，如果是无效ID，则返回NULL，返回值临时有效
	virtual LPCSTR GetIndicatorName(const CDTIID &cdtIID, BOOL bGetShortName = FALSE, DWORD dwCustomData = MAXDWORD);
	
	// 获取指定指标参数（不包括自定义指标参数）名称，如果是无效ID，则返回NULL，返回值临时有效
	virtual LPCSTR GetIndicatorParamName(const CDTIPID &cdtIPID, DWORD dwCustomData = MAXDWORD);
    
	// 查询条件是否有效
	virtual BOOL IsValidCondition(const CDTID &cdtID);
	
	// 查询条件是否是试机号相关条件
	virtual BOOL IsTestNumsCondition(const CDTID &cdtID, DWORD dwCustomData = MAXDWORD);
    
	// 查询条件是否是多值条件，pMaxCountOfValue：返回值个数的最大值，lpHasSameValue：返回条件值是否可含有相同值
	virtual BOOL IsMultipleValueCondition(const CDTID &cdtID, int *pMaxCountOfValue = NULL, LPBOOL lpHasSameValue = NULL);
    
	// 查询条件是否是动态条件
	virtual BOOL IsDynamicCondition(const CDTID &cdtID, DWORD dwCustomData = MAXDWORD);
        
	// 获取指定条件值域范围类型，返回值见宏定义，如：CDTRT_DEFAULT
	virtual BYTE GetConditionValueRangeType(const CDTID &cdtID);
	// 获取指定条件值类型，返回值见宏定义，如：CDTVT_DIGITAL
	virtual BYTE GetConditionValueType(const CDTID &cdtID);
	// 获取指定条件输入类型，返回值见宏定义，如：CDTIT_CHECKBOX
	virtual BYTE GetConditionInputType(const CDTID &cdtID);
    
	// 获取指定条件的扩展条件ID集合，如果lpCdtID为NULL，则获取所有扩展条件
	// 返回扩展条件个数，返回0表示当前没有扩展条件，返回-1表示当前条件已经不能扩展了
//	virtual int GetExtendConditions(LPCDTID lpCdtID, CArray<CDTID, CDTID &> *pExtIDArray = NULL);
    
	// 获取指标说明，如果是无效ID，则返回NULL，返回值临时有效
	virtual LPCSTR GetIndicatorExplain(const CDTIID &cdtIID);
	// 获取指标参数说明，如果是无效ID，则返回NULL，返回值临时有效
	virtual LPCSTR GetIndicatorParamExplain(const CDTIPID &cdtIPID);
	
	// 获取指标分类ID，如果该指标没有分类，则返回0
	virtual WORD GetIndicatorClassID(const CDTIID &cdtIID);
	// 获取指标分类说明，如果是无效ID，则返回NULL，返回值临时有效
	virtual LPCSTR GetIndicatorClassExplain(WORD wICID);
    
public:	// 固定条件组（含我的条件）接口函数
	
	// 获取指定的固定条件组信息
	virtual LPFIXCONDITIONGROUP GetFixConditionGroup(DWORD dwGroupID);
	// 根据dwGroupID替换FIXCONDITIONGROUP中的名称，仅用于FIXCONDITIONGROUP::GetName中调用。返回TRUE表示被替换
	virtual BOOL ReplaceFixConditionGroupName(DWORD dwGroupID, LPSTR lpszGroupName);
    
public:	// 通知接口函数
    
	// 当数据类型改变前
	virtual void OnPreDataTypeChange();
	
	// 当数据类型改变后
	virtual void OnDataTypeChanged();
    
	// 当配置改变时
	virtual void OnConfigChanged(DWORD dwConfigID);
	    
public:	// 分区相关接口函数
    
	// 获取系统分区中指定子分区数据，返回的低字中保存该分区的最小值，高字中保存该分区的最大值，返回MAXDWORD的则是错误
	// dwAreaIndex：为分区索引，0为第一分区，nMinValue为本分区的最小值，nMaxValue为最大值，以下同
	virtual DWORD GetSystemSubAreaData(DWORD dwAreaCount, DWORD dwAreaIndex, int nMinValue, int nMaxValue);
	
	// 获取值所在系统分区的子分区索引，0为第一分区，依次类推，小于则是错误
	virtual int GetSystemSubAreaIndex(DWORD dwAreaCount, int nValue, int nMinValue, int nMaxValue);
    
	// 获取指定号码分区信息串（临时有效），bNoSubName为TRUE时不包含子分区名称，lpszSep为分隔符
	virtual LPCSTR GetDataAreaRemarks(LPDATAAREA lpDataArea, BOOL bNoSubName = FALSE, LPCSTR lpszSep = "/");
    
	// 获取指定条件ID号的号码分区数据指针
	virtual LPDATAAREA GetDataArea(const CDTID &cdtID);
    
	// 根据号码分区区分值获取号码分区数据指针
	virtual LPDATAAREA GetDataArea(DWORD dwAreaDiffValues[2]);
	
	// 查询指定的号码分区信息是否存在
	virtual BOOL IsDataAreaExist(LPDATAAREA lpDataArea);
	
	// 添加号码分区数据，bModifyIfExist为TRUE时则表示如果存在则用lpDataArea的数据覆盖当前存在的数据，否则不进行处理
	// bToSystem为TURE时则添加到系统中，否则临时保存
	// 返回TRUE则表示添加或者修改（当bModifyIfExist为TRUE时有效）成功
	virtual BOOL AddDataArea(LPDATAAREA lpDataArea, BOOL bModifyIfExist = FALSE, BOOL bToSystem = TRUE);
    
public:		// 自定义条件相关接口
	
	// 查询指定条件是否是自定义条件
	virtual BOOL IsCustomCondition(const CDTID &cdtID);
	
	// 查询指定自定义指标是否存在
	virtual BOOL IsCustomIIDExist(const CDTIID &cdtIID);
	
	// 添加自定义指标，bModifyIfExist为TRUE时则表示如果存在则用dwCustomData的数据覆盖当前存在的自定义数据，否则不进行处理
	// bToSystem为TURE时则添加到系统中，否则临时保存
	// 返回TRUE则表示添加或者修改（当bModifyIfExist为TRUE时有效）成功
	virtual BOOL AddCustomID(DWORD dwCustomData, BOOL bModifyIfExist = FALSE, BOOL bToSystem = TRUE);
	
	// 查询指定自定义指标参数是否存在
	virtual BOOL IsCustomIPIDExist(const CDTIPID &cdtIPID);
	
	// 添加自定义指标参数，bModifyIfExist为TRUE时则表示如果存在则用dwCustomData的数据覆盖当前存在的自定义数据，否则不进行处理
	// bToSystem为TURE时则添加到系统中，否则临时保存
	// 返回TRUE则表示添加或者修改（当bModifyIfExist为TRUE时有效）成功
	virtual BOOL AddCustomIP( DWORD dwCustomData, BOOL bModifyIfExist = FALSE, BOOL bToSystem = TRUE);
    
public:	// 数据变换相关接口函数
    
	// 准备获取原始条件时（用于非原始的数据变换），调用这个函数以后，获取的条件都是原始数据变换的条件
	virtual void PrepareGetOrgDataTransConditions();
	// 完成获取原始条件时（用于非原始的数据变换），调用这个函数以后，获取的条件都是当前数据变换的条件
	virtual void FinishGetOrgataTransConditions();
    
private:
	// 删除所有条件信息
	void _RemoveAllConditionInfo();
    
	ICondition * _HalfFindICondition(const CDTID &cdtID, int &nInsertIndex);
	    
	// 根据类型返回条件名称，返回值临时有效
	LPCSTR _GetConditionNameByType(const CDTID &cdtID, BYTE btType = BCTT_NAME_WHOLE);
	
	// 获取条件组名
	LPCSTR _GetConditionGroupName(LPFIXCONDITIONGROUP lpFixCdtGroup, BYTE btType = BCTT_NAME_WHOLE);
    
private:	// 自定义相关
	void _InitCustomData(CCondition *pCondition);
	DWORD _GetCustomData(const CDTIID &cdtIID);
	DWORD _GetCustomData(const CDTIPID &cdtIPID);
	
	LPDATAAREA _GetDataArea(const CDTIID &cdtIID);
	LPDATAAREA _GetDataArea(const CDTIPID &cdtIPID);
//	CString _GetSubDataAreaText(int nAreaIndex, LPDATAAREA lpDataArea);
    
	LPCIP_HEAD _GetCustomIP(const CDTIPID &cdtIPID);
	
	// 清空所有自定义条件的值域
	void _ResetAllValueRange_Custom();
    
private:
	CDWordArray     m_cIConditionPointArray;			// 条件对象指针集合
	DWORD			m_dwCurConditionCount;				// 当前条件总数，由于有优化，cIConditionPointArray的元素个数大于实际个数
	CDWordArray     m_cDataAreaPointArray;				// 号码分区数据指针集合
	CDWordArray     m_cCustomIPPointArray;				// 自定义指标参数指针集合
	DATADIFFVALUE	m_stDataDiffValue;					// 数据区分标志
    
	DATATRANSSET	m_stDataTransTemp;					// 临时保存的数据变换设置，用于数据变换
	CDWordArray     m_cIConditionPointArrayTemp;		// 临时保存的条件对象指针集合，用于数据变换
	CDWordArray     m_cDataAreaPointArrayTemp;			// 临时保存的号码分区数据指针集合，用于数据变换
	CDWordArray     m_cCustomIPPointArrayTemp;			// 临时保存的自定义指标参数指针集合，用于数据变换
	BYTE			m_btDataOrderTypeTemp;				// 临时保存的数据排列方式
	
private:
    CIndicator m_cIndicator;
    CIndicatorParam m_cIndicatorParamFirst;
    CIndicatorParam m_cIndicatorParamSecond;
    
private:	
	char	m_szConditionName[MAXLEN_CDT_NAME];			// 条件名称
	char	m_szConditionShortName[MAXLEN_CDT_NAME];	// 条件简称
	char	m_szCdtNameTemp[256];						// 临时条件名，用于_GetConditionNameByType的返回值
	char	m_szAreaRemarks[256];						// 分区信息串
	char	m_szNumsAreaInfoText[256];					// 号码分区信息串
	char	m_szIndicatorExpain[256];					// 指标说明
	char	m_szIndicatorClassExpain[256];				// 指标分类说明
	char	m_szIndicatorParamExpain[256];				// 指标参数说明
	
	CConditionInfo	m_cCdtInfo;							// 条件信息对象
	
	BYTE	m_btNumberCount;							// 当前数据号码个数
	
	ICondition	*m_pIConditionInvalid;					// 无效条件
    
	FIXCONDITIONGROUP	m_stFCGroupMine;				// 我的条件组（跟节点数据使用）
	FIXCONDITIONGROUP	m_stFCGroupSys;					// 系统条件组（跟节点数据使用）
};

