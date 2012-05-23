//
//  Condition.h
//  QCLottery
//
//  Created by 武 孙 on 12-5-23.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ICondition.h"
#include "Def_ConditionID.h"
#include "Indicator.h"
#include "IndicatorParam.h"

class CCondition : public ICondition  
{
public:
	CCondition();
	virtual ~CCondition();
    
public:		// 接口函数
    
	// 获取条件ID号
	virtual CDTID& GetID(){return m_cdtID;};
	// 获取条件中的指标ID号
	virtual CDTIID& GetIndicatorID(){return m_cdtID.cdtIID;};
	// 获取条件中的指标参数ID号，bFirst为TRUE则是第一个参数否则为第二个参数
	virtual CDTIPID& GetIndicatorParamID(BOOL bFirst = TRUE){return bFirst ? m_cdtID.cdtIPID : m_cdtID.cdtIPID2;};
    
	// 获取条件的父条件，如果返回NULL则是指标条件
	virtual ICondition * GetIConditionParent(){return m_pIConditionParent;};
	
	// 获取条件名称，bGetShortName为TRUE时获取条件简称
	virtual LPCSTR GetName(BOOL bGetShortName = FALSE);
	
	// 获取条件值域
	virtual LPCDTVALUERANGE GetValueRange();
    
	// 查找指定值在值域中的索引，返回-1表示没有找到
	virtual int GetIndexOfValueRange(int nValue);
	// 查找指定值在值域中的数据单元指针，返回NULL表示没有找到
	virtual LPCDTVALUERANGEITEM GetItemOfValueRange(int nValue);
    
	// 获取值域范围类型，返回值见宏定义，如：CDTRT_DEFAULT
	virtual BYTE GetValueRangeType();
	// 获取值类型，返回值见宏定义，如：CDTVT_DIGITAL
	virtual BYTE GetValueType();
	// 获取输入类型，返回值见宏定义，如：CDTIT_CHECKBOX
	virtual BYTE GetInputType();
    
	// 是否是复合条件
	virtual BOOL IsComplex();
	
	// 是否是多值条件，pMaxCountOfValue：返回值个数的最大值，lpHasSameValue：返回条件值是否可含有相同值
	virtual BOOL IsMultipleValue(int *pMaxCountOfValue = NULL, LPBOOL lpHasSameValue = NULL);
    
	// 是否是动态条件
	virtual BOOL IsDynamic();
    
	// 是否是试机号相关条件
	virtual BOOL IsIncludeTestNums();
	
	// 获取指定号码数据的条件值，如果是多值条件，则返回计算后的值（比如多值为2、0、3，则返回203）
	// 如果dwIssueIndex == DATA_INDEX_INVALID，则lpFltNumsData不能为NULL，此时获取当期缩水号码数据的条件值
	// 否则，如果lpFltNumsData为NULL，则获取dwIssueIndex指定期号的历史数据的条件值，
	// bType类型，见宏定义
	virtual int GetValue(LPBYTE lpFltNumsData, DWORD dwIssueIndex = DATA_INDEX_INVALID, BYTE btType = CDTGVT_NORMAL);
	
	// 获取当期号码数据的条件值（扩展方法），参数说明同上
	virtual LPCDTVALUE GetValueEx(LPBYTE lpFltNumsData, DWORD dwIssueIndex = DATA_INDEX_INVALID, BYTE btType = CDTGVT_NORMAL);
	
	// 获取当期号码数据的条件值文本串，bUseValueName为TRUE则使用值名，否则使用列名
	// lpFltNumsData和dwIssueIndex的说明同上
	virtual LPCSTR GetValueString(LPBYTE lpFltNumsData, DWORD dwIssueIndex = DATA_INDEX_INVALID, BOOL bUseValueName = TRUE);
	
	// 条件值转换为文本串，bUseValueName为TRUE则使用值名，否则使用列名
	virtual LPCSTR ValueToString(int nValue, BOOL bUseValueName = TRUE);
	virtual LPCSTR ValueToString(CDWordArray &cValueArray, BOOL bUseValueName = TRUE);
	virtual LPCSTR ValueToString(LPINT lpValues, int nValueCount, BOOL bUseValueName = TRUE);
    
	// 条件值名是否是数字型的，比如和值是数字型的，而类型等是文本型的
	virtual BOOL IsDigitalValueName();
    
	// 获取值说明信息，nLeftSpaceCount：为前置空格个数，nSepSpaceCount为值说明间的空格个数
//	virtual BOOL GetValueExplain(CString &strExplain, int nLeftSpaceCount = 0, int nSepSpaceCount = 2, int nMaxCol = MAXCHAR_EXPLAIN_ROW);
    
	// 是否支持图表
	virtual BOOL IsSupportGraph();
    
public:	// 自定义条件相关接口
	
	// 是否是自定义条件
	virtual BOOL IsCustom(){return g_pIConditionMan->IsCustomCondition(m_cdtID);};
	
	// 设置和获取自定义数据
	virtual void SetCustomData(DWORD dwCustomData, BYTE btFlag = CDTCDF_CONDITION);
	virtual DWORD GetCustomData(BYTE btFlag = CDTCDF_CONDITION);
    
public:
	// 条件初始化（正常条件）
	BOOL Initialize(const CDTID& cdtID);
	// 用于临时条件的初始化，pIParentCdt：父级条件指针
	BOOL InitializeForTemp(const CDTID& cdtID, ICondition *pIParentCdt = NULL);
    
	// 清空条件的值域
	void EmptyValueRange();
    
	// 是否是位积相关条件
	BOOL IsPosMulConditions();
    
private:
	LPCSTR _GetValueString(int nValue, BYTE btValueRangeType, BYTE btValueType, BOOL bUseValueName = TRUE);
	int _QuckFind_ValueItem(int nFindValue, BOOL bValueSortAsc = TRUE);
    
private:
    
	CDTID				m_cdtID;							// 条件ID号
	LPCDTVALUERANGE		m_lpValueRange;						// 值域
    
	ICondition			*m_pIConditionParent;				// 父级指标条件指针，比如当前为含两个指标的条件，则为第一个指标条件指针，如果为NULL，则本身为指标
	DWORD				m_dwCustomData;						// 自定义数据（如果为0，则表示当前最终的扩展不是自定义数据，比如条件：百位自定义五分区奇偶）
        
	static char			ms_szValueStringTemp[128];			// 临时使用的单个值文本串
    static char         ms_szValueStringMore[1024 * 10];    // 临时使用的多个值文本串
    
public:
    static CIndicator *m_pIndicator;
    static CIndicatorParam *m_pIndicatorParamFirst;
    static CIndicatorParam *m_pIndicatorParamSecond;
};

