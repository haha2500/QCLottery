//
//  Indicator.h
//  QCLottery
//
//  Created by 武 孙 on 12-5-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "Def_ConditionID.h"
#include "Def_ConditionValue.h"
#include "ICondition.h"

// 定义_AutoFillNameInValueRange中btFlag的取值
#define		AFNVF_NORMAL						0			// 常规方式，即名字和值一样

class CIndicator  
{
public:
	CIndicator();
	virtual ~CIndicator();
    
public:
    
	// 判断ID是否有效
	BOOL IsValid(const CDTIID &cdtIID, DWORD dwCustomData);
    
	// 获取指标名称，bGetShortName为TRUE时获取指标简称
	LPCSTR GetName(const CDTIID &cdtIID, BOOL bGetShortName = FALSE, DWORD dwCustomData = 0);
    
	// 获取指标说明
	LPCSTR GetExplain(const CDTIID &cdtIID, DWORD dwCustomData);
    
	// 获取指标值域
	BOOL GetValueRange(const CDTIID &cdtIID, DWORD dwCustomData, LPCDTVALUERANGE *lplpValueRange);
	
	// 获取值域范围类型，返回值见宏定义，如：CDTRT_DEFAULT
	BYTE GetValueRangeType(const CDTIID &cdtIID, DWORD dwCustomData);
	// 获取值类型，返回值见宏定义，如：CDTVT_DIGITAL
	BYTE GetValueType(const CDTIID &cdtIID, DWORD dwCustomData);
	// 获取输入类型，返回值见宏定义，如：CDTIT_CHECKBOX
	BYTE GetInputType(const CDTIID &cdtIID, DWORD dwCustomData);
    
	// 是否是多值条件，pMaxCountOfValue：返回值个数的最大值
	BOOL IsMultipleValue(const CDTIID &cdtIID, DWORD dwCustomData, int *pMaxCountOfValue, LPBOOL lpHasSameValue);
    
	// 获取指定号码数据的指标的值，lpFltNumsData：指定的号码，dwIssueIndex：指定的期号索引
	// 如果lpFltNumsData为NULL，则dwIssueIndex不能为DATA_INDEX_INVALID，此时为dwIssueIndex指定的历史号码
	// 如果lpFltNumsData不为NULL，则当dwIssueIndex==DATA_INDEX_INVALID时为当期缩水号码，否则为dwIssueIndex指定期号的缩水号码
	LPCDTVALUE GetValue(const CDTIID &cdtIID, DWORD dwCustomData, LPBYTE lpFltNumsData, DWORD dwIssueIndex = DATA_INDEX_INVALID, BYTE btType = 0);
    
	// 当数据类型改变时
	void OnDataTypeChanged();
    
	// 查询指标值名是否是数字型的，比如和值是数字型的，而类型等是文本型的
	BOOL IsDigitalValueName(const CDTIID &cdtIID, DWORD dwCustomData, LPCDTVALUERANGE lpValueRange);
    
	// 获取值说明，返回strExplainArray的个数，为0则表示无相关说明
// TEST	int GetValueExplain(CStringArray &strExplainArray, const CDTIID &cdtIID, DWORD dwCustomData, LPCDTVALUERANGE lpValueRange);
	
private:
	char	m_szDetailsTemp[MAXLEN_INDICATOR_DETAILS + 1];				// 临时使用的指标详细说明
	LPCDTVALUERANGE	m_lpValueRangeTemp;									// 临时使用的值域指针
	LPCDTVALUE m_lpValueTemp;											// 临时使用的值指针
    
	BYTE	m_btMinNumber;												// 当前最小号码
	BYTE	m_btMaxNumber;												// 当前最大号码
	BYTE	m_btNumberCount;											// 当前号码个数
	BYTE	m_btDataType;												// 当前数据类型
    
	LPBYTE	m_lpNumsData;												// 号码数据
    
private:
	
	// 是否是有效的号码分区指标
	BOOL _IsValid_DataArea(const CDTIID &cdtIID, LPDATAAREA lpDataArea);
    
private:
	// 自动填充缺省类型的值域			指标ID号		最小值          最大值         固定值长度（为0则不固定，否则不够长度前补0）
	void _AutoFillValueRange_DefaultType(DWORD dwIID, int nMinValue, int nMaxValue, BYTE btFixValueLen = 0);
	// 自动填充连续且无上限类型的值域	指标ID号		最小值
	void _AutoFillValueRange_LinkNoLimit(DWORD dwIID, int nMinValue = 0);
    
	// 设置（修改）单个值域单元信息，之前必须已经设置了值域信息
	void _SetValueRangeItemValue(int nValueIndex, int nValue, LPCSTR lpszColName = NULL, LPCSTR lpszBallName = NULL, LPCSTR lpszValueName = NULL);
    
	// 获取指定位置（btPosIndex）的最大号码，bSingle是否是单选
	BYTE _GetPosMaxValue(BYTE btPosIndex = 0, BOOL bSingle = TRUE);
	// 获取指定位置（btPosIndex）的最小号码，bSingle是否是单选
	BYTE _GetPosMinValue(BYTE btPosIndex = 0, BOOL bSingle = TRUE);
    
	// 获取AC值范围，返回最大AC值
	int _GetACValueRange(int &nMin);
    
private:	// 填充静态指标值域
	void _FillValueRange_SameCombo();										// 填充相同数字组合指标值域
	void _FillValueRange_Genre();											// 填充类型指标值域
	void _FillValueRange_GenreTrans();										// 填充变换类型指标值域
	void _FillValueRange_Shape();											// 填充形态指标值域
	void _FillValueRange_ShapeEx();											// 填充细分形态指标值域
	void _FillValueRange_Shape_XXX(DWORD dwIID);							// 填充组三等形态指标值域
	void _FillValueRange_XXXCombo(LPCSTR lpszName1, LPCSTR lpszName2);		// 填充组合指标（比如斜连值）值域
	void _FillValueRange_DHK(DWORD dwIID);									// 填充胆合跨指标值域
	void _FillValueRange_Nums(DWORD dwIID);									// 填充号码指标值域
	void _FillValueRange_PosX(DWORD dwIID, BYTE btPosIndex);				// 填充按位型指标值域
	void _FillValueRange_Sum();												// 填充和值指标值域
	void _FillValueRange_Avg();												// 填充均值指标值域
	void _FillValueRange_EspecialSum(DWORD dwIID);							// 填充特码和指标值域
	void _FillValueRange_Order(DWORD dwIID);								// 填充顺序排列指标值域
	void _FillValueRange_TwoNums(DWORD dwIID);								// 填充二码指标值域
	void _FillValueRange_Space(DWORD dwIID);								// 填充间距指标值域
	void _FillValueRange_Mul(DWORD dwIID);									// 填充积数指标值域
	void _FillValueRange_PosMulSum(DWORD dwIID);							// 填充位置积和指标值域
	void _FillValueRange_LinkCombo(DWORD dwIID);							// 填充连号组合指标值域
	void _FillValueRange_LinkPos(DWORD dwIID);								// 填充连号位置指标值域
    
	void _FillValueRange_SinArrangeValue(DWORD dwIID, char szName[10][8], int nStateCount = 3);		// 填充单选排列值指标值域
	void _FillValueRange_SinArrangeValue_Numbers(DWORD dwIID, int nMinNumber, int nMaxNumber);		// 填充数字型单选排列值指标值域
	void _FillValueRange_GroArrangeValue(DWORD dwIID, char szName[10][8], int nStateCount = 3);		// 填充组选排列值指标值域
	void _FillValueRange_GroArrangeValue_Numbers(DWORD dwIID, int nMinNumber, int nMaxNumber);		// 填充数字型组选排列值指标值域
	void _FillValueRange_CountArrangeValue(DWORD dwIID, char szName[3][8], BOOL bAscOrder = TRUE);	// 填充个数排列值（比如3态指标的组选排列等）指标值域
	void _FillValueRange_CountArrangeValue_Numbers(DWORD dwIID, BYTE btItemCount);					// 填充数字型个数排列值（比如3态指标的组选排列等）指标值域
    
	void _FillValueRange_Arrange(DWORD dwIID, char szNames[32][8], int nValues[32], BYTE btItemCount);	// 填充排列（含单选和组选）指标值域
	void _FillValueRange_Arrange_Numbers(DWORD dwIID, int nMinValue, int nMaxValue);					// 填充数值型排列（含单选和组选）指标值域
    
	void _FillValueRange_3StateShape(DWORD dwIID, char szName[3][8]);		// 填充3态指标的形态指标值域
    
	void _FillValueRange_TwoPos(DWORD dwID);								// 填充两位关系指标值域
	void _FillValueRange_TwoPos_Sum(DWORD dwID, DWORD dwIID);				// 填充两位和指标值域
	void _FillValueRange_TwoPos_Sub(DWORD dwID, DWORD dwIID);				// 填充两位差指标值域
	void _FillValueRange_TwoPos_CirSub(DWORD dwID, DWORD dwIID);			// 填充两位环差指标值域
	void _FillValueRange_TwoPos_Mul(DWORD dwID, DWORD dwIID);				// 填充两位积指标值域
	void _FillValueRange_Remainder(DWORD dwIID);							// 填充余数指标值域
	void _FillValueRange_DataArea(WORD wUseType, DWORD dwCustomData);		// 填充号码分区指标值域
    
	void _FillValueRange_Skip_Area(DWORD dwIID);							// 填充单选或组选遗漏值系统（十）分区指标值域
    
private:	// 填充动态指标值域
	void _FillValueRange_3State(DWORD dwIID);								// 填充动态3态型指标值域
	void _FillValueRange_PrevData(DWORD dwIID, int nTypeIndex = PDT_DIS);	// 填充邻期关系型距离指标值域
    
	///////////////////////////////////////////////////////////////////////////////////////////////////
private:
	// 填充多值
	LPCDTVALUE _FillMulValues(int *pValues, BYTE btValueCount);
    
	// 填充单值
	LPCDTVALUE _FillSingleValues(int nValue);
    
	// 获取排序后的号码数据（即组选号码）
	void _GetOrderedNumsData(LPBYTE lpOrderedNumsData);
    
private:	// 获取静态指标值
	int _GetNumsIndex();
	int _GetSameNumGroupCount(int nSameCount, int *pValuesOut = NULL);
	int _GetTypeValue();
	int _GetTwoLinkValue();
	int _GetShapeValue();
	int _GetEqualDiffValue();
	int _GetSameOrDiffNumCountValue(DWORD dwIID);
	int _GetLinkCountValue(BOOL bGroupCount = FALSE);
	int _GetLinkASOrDsCountValue(BOOL bGroupCount = FALSE, BOOL bAscOrder = TRUE);
	int _GetOELinkCountValue(BOOL bOddLink = TRUE);
	int _GetLinkCombo(LPINT lpLinkCountArray);
	int _GetLinkPos(int nLinkCount, int *lpPosArray);
	int _GetDHKValue(DWORD dwIID);
	int _GetNumsValue(DWORD dwIID);
	int _GetPosSum(int nPosIndex, BOOL bGetTail = FALSE);
	int _GetPosSpan(int nPosIndex, BOOL bGetAbs = TRUE);
	int _GetSumValue();
	int _GetSumNValue();
	int _GetSpanValue();
	int _GetAvgValue();
	int _GetACValue();
	int _GetDetractValue();
	int _GetDeparture(DWORD dwIssueIndex);
	int _GetOddEven(int nValue){return nValue % 2;};
	int _GetOddEvenCount(int nFlag);
	int _GetOddEvenArrange(BOOL bGetValue = FALSE);
	int _GetOddEvenLinkValue();
	int _GetBigSmall(int nValue);
	int _GetBigSmallCount(int nFlag);
	int _GetBigSmallArrange(BOOL bGetValue = FALSE);
	int _GetPrimeComposite(int nValue);
	int _GetPrimeCompositeCount(int nFlag);
	int _GetPrimeCompositeArrange(BOOL bGetValue = FALSE);
	int _Get012(int nValue){return nValue % 3;};
	int _Get012Count(int nFlag);
	int _Get012Shape();
	int _Get012Arrange(BYTE btType = 1, BOOL bGetValue = FALSE);
	int _GetDZX(int nValue);
	int _GetDZXCount(int nFlag);
	int _GetDZXShape();
	int _GetDZXArrange(BYTE btType = 1, BOOL bGetValue = FALSE);
	LPCDTVALUE _GetOrderValue(DWORD dwIID);
	int _GetSpaceValue(DWORD dwIID);
	int _Get1DValue(DWORD dwIID);
	int _GetProductValue(DWORD dwIID);
	int _GetTwoNums_Z3(DWORD dwIID);
	int _GetTwoNums_Z6(DWORD dwIID);
	int _GetNumsMatchCount(DWORD dwIID, DWORD dwIssueIndex, BOOL bType);
	int _GetSameCount_TestNums(DWORD dwIID, DWORD dwIssueIndex);
	int _GetSameCount_TestNums_PrevNums(DWORD dwIID, DWORD dwIssueIndex);
    
	LPCDTVALUE _GetTwoNumsValue(DWORD dwIID);
    
	// 获取两位关系
	LPCDTVALUE _GetTwoPosValue(DWORD dwIID);
    
	// 获取余数
	LPCDTVALUE _GetRemainderValue(DWORD dwIID);
    
	// 获取分区
	LPCDTVALUE _GetAreaValue(WORD wUseType, DWORD dwCustomData);
    
private:
	int _CalcTwoPos(int nPos1, int nPos2, int nType);
	int _CalcAllTwoPos(int nType, BOOL bAutoSort, LPINT lpValuesOut);
	int _CalcMaxLinkSameValueCount(LPBYTE lpValues, int nValueCount, BYTE btSameValue = 1);
	int _GetRelativedDZX(BYTE btPosValue, LPBYTE lpOrderNumsData);
    
	int _GetMaxSameNumberCount();	// 获取最大相同号码个数
    
	// 初始化个数排列值的索引值信息
	void _InitIndexOfCountArrange(BYTE btItemCount);
	// 初始化组选排列值的索引值信息
    //	void _InitIndexOfGroupArrange(BYTE btItemCount, BYTE btMaxValue);
    
	// 获取个数排列值的索引值
	int _GetIndexOfCountArrange(int nValue, BYTE btItemCount, BOOL bAscOrder = TRUE);
	// 获取组选排列值的索引值
    //	int _GetIndexOfGroupArrange(int nValue, BYTE btItemCount, BYTE btMaxValue);
	
private:	// 获取动态指标值
	int _GetXLValue(BYTE btType, DWORD dwIssueIndex = DATA_INDEX_INVALID);
    
	// 获取任意遗漏尾
	LPCDTVALUE _GetAnySkipTail(DWORD dwIssueIndex, BOOL bSingle = TRUE);
    
	// 获取动态三态指标值
	int _Get3StateValue(DWORD dwIID, DWORD dwIssueIndex);
    
	// 获取重复号个数
	int _GetRepeatCountValue(DWORD dwIID, DWORD dwIssueIndex);
    
	// 获取邻期指标值，nPosIndex为-1则表示和，-2为合，-3为任意，-4为单排，-5为组排，0-7则是相应位置的邻期指标值
	LPCDTVALUE _GetPrevDataValue(int nPosIndex = -1, DWORD dwIssueIndex = DATA_INDEX_INVALID, int nTypeIndex = PDT_DIS);
	int _GetPrevDataValue_Simple(BYTE btPrevData, BYTE btCurData, int nTypeIndex = PDT_DIS);
    
private:
	LPCSTR _GetDataAreaIndicatorName(const CDTIID &cdtIID, BOOL bGetShortName, DWORD dwCustomData);
	LPCSTR _GetValueExplain_DataArea(const CDTIID &cdtIID, DWORD dwCustomData);
    
private:
	char	m_szName[MAXLEN_INDICATOR_NAME];					// 指标名称
	char	m_szShortName[MAXLEN_INDICATOR_SHORTNAME];			// 指标简称
	char	m_szExplain[256];									// 指标说明
	char	m_strPosNames[8][16];									// 按位名称
	char	m_strPosShortNames[8][8];								// 按位简称
    
	DWORD	m_dwCountArrangeValueIndexs[8][256];				// 个数排列值索引信息
    //	DWORD	m_dwGroupArrangeValueIndexs[8][9][1024];			// 组选排列值索引信息
};
