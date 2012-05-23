//
//  IndicatorParam.h
//  QCLottery
//
//  Created by 武 孙 on 12-5-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "Def_ConditionID.h"
#include "Def_ConditionCustom.h"
#include "ICondition.h"

#define		BEGIN_FILLVALUERANGE(btItemCountIn) { \
                int nIndex = 0; \
                m_lpValueRangeTemp->nItemCount = btItemCountIn;

#define		ADD_VALUERANGEITEM(nValueIn, szValueNameIn, szColNameIn, szBallNameIn) \
                m_lpValueRangeTemp->stValueItem[nIndex].nValue = nValueIn; \
                strcpy(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, szValueNameIn); \
                strcpy(m_lpValueRangeTemp->stValueItem[nIndex].szColName, szColNameIn); \
                strcpy(m_lpValueRangeTemp->stValueItem[nIndex].szBallName, szBallNameIn); \
                nIndex ++;

#define		END_FILLVALUERANGE() ASSERT(nIndex == m_lpValueRangeTemp->nItemCount); }

class CIndicatorParam  
{
public:
	CIndicatorParam();
	virtual ~CIndicatorParam();
    
public:
	
	// 初始化，bFirstParam为TRUE则表示第一个指标参数
	BOOL Initialize(BOOL bFirstParam = TRUE);
    
	// 判断ID是否有效
	BOOL IsValid(const CDTIPID &cdtIPID);
	
	// 获取指标参数名称
	LPCSTR GetName(const CDTIPID &cdtIPID, DWORD dwCustomData);
	
	// 获取指标参数说明
	LPCSTR GetExplain(const CDTIPID &cdtIPID);
    
	// 获取指标参数值域
	BOOL GetValueRange(const CDTIPID &cdtIPID, ICondition *pICdtParent, DWORD dwCustomData, LPCDTVALUERANGE *lplpValueRange);
    
	// 获取值域范围类型，返回值见宏定义，如：CDTRT_DEFAULT
	BYTE GetValueRangeType(const CDTIPID &cdtIPID, DWORD dwCustomData);
	// 获取值类型，返回值见宏定义，如：CDTVT_DIGITAL
	BYTE GetValueType(const CDTIPID &cdtIPID, DWORD dwCustomData);
	// 获取输入类型，返回值见宏定义，如：CDTIT_CHECKBOX
	BYTE GetInputType(const CDTIPID &cdtIPID, DWORD dwCustomData);
	
	// 是否是多值条件，pMaxCountOfValue：返回值个数的最大值
	BOOL IsMultipleValue(const CDTIPID &cdtIPID, DWORD dwCustomData, int *pMaxCountOfValue = NULL);
	
	// 获取指标值的指标参数的值，如果dwIssueIndex不为DATA_INDEX_INVALID，则是对应期号索引的指标值
	LPCDTVALUE GetValue(const CDTIPID &cdtIPID, ICondition *pICdtParent, DWORD dwCustomData, LPBYTE lpFltNumsData, DWORD dwIssueIndex = DATA_INDEX_INVALID, BYTE btType = 0);
    
	// 查询指标参数值名是否是数字型的，比如距离是数字型的，而大小等是文本型的
	BOOL IsDigitalValueName(const CDTIPID &cdtIPID, DWORD dwCustomData);
    
	// 获取值说明，返回strExplainArray的个数，为0则表示无相关说明
//	int GetValueExplain(CStringArray &strExplainArray, const CDTIPID &cdtIPID, DWORD dwCustomData, ICondition *pIConditionParent, LPCDTVALUERANGE lpValueRangeSelf);
    
private:
	BOOL			m_bFirstParam;										// 是否是第一个指标参数
	char			m_szNameTemp[MAXLEN_INDICATORPARAM_NAME + 1];		// 临时使用的指标参数名称
	char			m_szExplain[256];									// 指标参数说明
	
	LPCDTVALUERANGE	m_lpValueRangeTemp;									// 临时使用的值域指针
	LPCDTVALUE		m_lpValueTemp;										// 临时使用的值指针
    
private:
    
	// 条件值处理
	void _RemoveAllValue();
	void _AddValue(int nValue, BYTE btValueCount = 1);
	void _GetValueRangeForPosArea(int &nMinValue, int &nMaxValue, ICondition *pIConditionParent);
    
private:
	void _SetValueRangeItemValue(int nValueIndex, int nValue, LPCSTR lpszColName = NULL, LPCSTR lpszBallName = NULL, LPCSTR lpszValueName = NULL);
	void _AutoFillValueRange_Default(int nMinValue, int nMaxValue);
	void _AutoFillValueRange_LinkNoLimit(int nMinValue);
	void _FillValueRange_Tail(LPCSTR lpszName = "尾数");									
	void _FillValueRange_Abs(ICondition *pICdtParent);									
	void _FillValueRange_SubArea(int nAreaCount);	
	void _FillValueRange_CustomIP(LPCIP_HEAD lpCustomIP, ICondition *pICdtParent);	
	void _FillValueRange_DataArea(LPDATAAREA lpDataArea);	
    
	BOOL _GetValueRange_Remainder(ICondition *pICdtParent, int nDivisor);
	BOOL _GetValueRange_Distance(ICondition *pICdtParent, BOOL bCircle = FALSE);		
	BOOL _GetValueRange_Adjacent(ICondition *pICdtParent, BOOL bSum = TRUE);	
	BOOL _GetValueRange_Axis(ICondition *pICdtParent, DWORD dwIPID);
    
private:
	int _GetSumValue(LPCDTVALUE lpParentValue, BOOL bGetTail = FALSE);
	int _GetSumNValue(LPCDTVALUE lpParentValue);
	int _GetSpanValue(LPCDTVALUE lpParentValue, BOOL bGetTail = FALSE);
	int _GetAvgValue(LPCDTVALUE lpParentValue, BOOL bGetTail = FALSE);
	int _GetMinValue(LPCDTVALUE lpParentValue, BOOL bGetTail = FALSE);
	int _GetMidValue(LPCDTVALUE lpParentValue, BOOL bGetTail = FALSE);
	int _GetMaxValue(LPCDTVALUE lpParentValue, BOOL bGetTail = FALSE);
	
	int _GetDistanceValue(ICondition *pICdtParent, int nValueIn, DWORD dwIssueIndex, DWORD dwIPID);
	int _GetCircleDistanceValue(ICondition *pICdtParent, int nValueIn, DWORD dwIssueIndex, DWORD dwIPID);
	int _GetAdjacentValue(ICondition *pICdtParent, int nValueIn, DWORD dwIssueIndex, DWORD dwIPID);
    
	int _GetSPJValue(ICondition *pICdtParent, int nValueIn, DWORD dwIssueIndex);
    
	int _GetAxisValue(ICondition *pICdtParent, int nValueIn, DWORD dwIssueIndex, DWORD dwIPID);
    
	int _GetSkipValue(ICondition *pICdtParent, int nValueIn, DWORD dwIssueIndex, DWORD dwIPID);
    
	int _GetTestNumsXXXValue(DWORD dwIPID, ICondition *pICdtParent, int nValueIn, DWORD dwIssueIndex);
	
	LPCDTVALUE _GetCustomValue(const CDTIPID &cdtIPID, ICondition *pICdtParent, DWORD dwCustomData, LPCDTVALUE lpParentValue);
	
private:
	void _GetExplain_Custom(const CDTIPID &cdtIPID);
    
	int _GetCustomIPValue(const CDTIPID &cdtIPID, DWORD dwCustomData, int nParentValue);
/*	CString _GetSubDataAreaText(int nAreaIndex, LPDATAAREA lpDataArea);
	CString _GetSubCustomAreaText(int nAreaIndex, LPCIP_CUSTOMAREA lpCustomArea, ICondition * pIConditionParent);
	int _GetValueExplain_Custom(CStringArray &strExplainArray, const CDTIPID &cdtIPID, DWORD dwCustomData, ICondition *pIConditionParent, LPCDTVALUERANGE lpValueRangeSelf);
 */
};

