//
//  Data.h
//  Graph
//
//  Created by 武 孙 on 12-5-9.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "IData.h"
#include "DataFactory.h"

#define DATAFILE_VERSION_CUR        1   // 数据文件当前版本号码

#pragma pack(1)
typedef struct tagDATAFILEHEAD		// 数据文件头部，该部分不和数据一起加密
{
	BYTE	btVersion;					// 文件版本号，目前为 DATAFILE_VERSION_CUR
	DWORD	dwDataItemCount;			// 当前数据元素总数
} DATAFILEHEAD, *LPDATAFILEHEAD;
#pragma pack()

class CData : public IData  
{
public:
	CData();
	virtual ~CData();
    
public:     // 数据文件相关操作
    // 打开彩票文件并载入数据，此时初始号码数据有效
    virtual BOOL OpenLotteryFile();
    // 关闭文件并释放载入的数据，初始号码变为无效
    virtual void CloseLotteryFile(BOOL bSave);
    
    // 增加开奖号
	virtual BOOL AddLtyNums(DWORD dwDataTime, DWORD dwIssue, LPBYTE lpData);
	// 修改开奖号
	virtual BOOL ModifyLtyNums(DWORD dwIndex, DWORD dwDataTime, DWORD dwIssue, LPBYTE lpData);
	// 删除最后的开奖号
	virtual BOOL DeleteLastLtyNums();
	// 删除全部开奖号
	virtual BOOL DeleteAllLtyNums();
	
    // 下载开奖号码，下载失败返回FALSE，当下载成功时bModified有效，包含TRUE则表示数据更新过
	virtual BOOL DownloadLtyNums(BOOL &bModified);
    
public:
	// 获取彩票ID
	virtual DWORD GetLotteryID();
	// 获取到彩票属性
	virtual LPLOTTERYPROPERTY GetLotteryProperty();
	
	// 获取开奖期号类型，返回宏定义（如：DATAISSUE_TYPE_YYYYXXX）
	virtual BYTE GetIssueType();
	// 获取开奖期号数据长度，bOnlyLastXXX为TRUE时只获取最后需要的几位，比如ISSUE_TYPE_YYYYXXX类型的则是最后3位，长度为3
	virtual BYTE GetIssueLen(BOOL bOnlyLastXXX = FALSE);
    
	// 获取数据类型，btDataSource：数据源，见宏定义
	virtual BYTE GetDataType(BYTE btDataSource = DATA_SOURCE_CUR);
	
	// 获取数据集合中元素总数
	virtual DWORD GetItemCount(BYTE btDataSource = DATA_SOURCE_CUR);
	
	// 获取指定索引的开奖日期（如：20091105）
	virtual DWORD GetItemDate(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR);
	// 获取指定索引的开奖日期文本串（如：2009-11-05，注意：返回值临时有效，每次调用时覆盖）
	virtual LPCSTR GetItemDateString(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR);
	
	// 获取指定索引的开奖时间（如：1220，只含小时和分钟，不包含日期和秒）
	virtual DWORD GetItemTime(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR);
	// 获取指定索引的开奖时间（如：12:20，只含小时和分钟，不包含日期和秒）文本串（注意：返回值临时有效，每次调用时覆盖）
	virtual LPCSTR GetItemTimeString(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR);
	
	// 获取指定索引的开奖星期几，星期日为0，星期一为1，以此类推
	virtual int GetItemDayOfWeek(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR);
	// 获取指定索引的开奖星期几文本串，星期日为“日”，星期一为“一”，以此类推
	virtual LPCSTR GetItemDayOfWeekString(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR);
	
	// 获取指定索引的开奖日期和时间
	virtual void GetItemDateTime(DWORD dwDataIndex, CTime &tmDateTime, BYTE btDataSource = DATA_SOURCE_CUR);
	// 获取指定索引的开奖日期和时间文本串（如高频彩为：2011-04-07 11:25，其他为：2011-04-07 11:25）
	virtual LPCSTR GetItemDateTimeString(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR);
	
	// 获取指定索引的开奖期号
	virtual DWORD GetItemIssue(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR);
	// 获取指定索引的开奖期号文本串（注意：返回值临时有效，每次调用时覆盖）
	virtual LPCSTR GetItemIssueString(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR);
	
	// 获取指定索引的开奖号码数据，返回NULL则表示没有该开奖号码数据
	virtual LPBYTE GetItemNums(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR);
	// 获取指定索引的开奖号码的文本串（注意：返回值临时有效，每次调用时覆盖）
	virtual LPCSTR GetItemNumsString(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR);
	// 获取指定索引的开奖号码对应的整型值
	virtual DWORD GetItemNumsValue(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR);
	
	// 获取指定索引的第2开奖号码（比如当前为双色球红球，则获取蓝球，如果为蓝球，则获取红球）数据，返回NULL则表示没有该数据
	virtual LPBYTE GetItemNums2(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR);
	// 获取指定索引的第2开奖号码（同上）的文本串（注意：返回值临时有效，每次调用时覆盖）
	virtual LPCSTR GetItemNums2String(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR);
	// 获取指定索引的第2开奖号码（同上）对应的整型值
	virtual DWORD GetItemNums2Value(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR);
	
	// 获取指定索引的试机号码数据，返回NULL则表示没有该开奖号码数据
	virtual LPBYTE GetItemTestNums(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR);
	// 获取指定索引的试机号码的文本串（注意：返回值临时有效，每次调用时覆盖）
	virtual LPCSTR GetItemTestNumsString(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR);
	// 获取指定索引的试机号码对应的整型值
	virtual DWORD GetItemTestNumsValue(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR);
	// 判断当前数据是否是由试机号变来的
	virtual BOOL IsTestNumsItem(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR);
    
	// 获取指定索引的机器号
	virtual BYTE GetItemMachineIndex(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR);
	// 获取指定索引的球号
	virtual BYTE GetItemBallIndex(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR);
	
	// 根据当前数据索引获取对应的初始数据的索引，没找到时返回DATAINDEX_INVALID
	virtual DWORD GetInitItemIndex(DWORD dwCurDataIndex);
	
	// 设置预测行号码数据，lpData为NULL时，清除预测行号码数据
	virtual void SetForecastNums(LPBYTE lpNumsData = NULL);
	// 获取预测行号码数据，没有则返回NULL
	virtual LPBYTE GetForecastNums();
    
	// 设置彩票号码区段索引
	virtual void SetSectionIndex(BYTE btSectionIndex = 0);
	// 设置彩票玩法信息
	virtual void SetPlayType(LPPLAYTYPEITEM lpPTItem);
	// 设置分析方式信息
	virtual void SetAnalysisMode(LPANALYSISMODEITEM lpAMItem);
	// 设置数据变换信息
	virtual void SetDataTrans(LPVOID lpDataTrans);
	// 设置排列方式
	virtual void SetDataOrderType(BYTE btDataOrderType);
    
	// 获取当前使用的彩票数据区段的索引，比如双色球的红蓝球等
	virtual BYTE GetCurrentSectionIndex();
    
	// 是否是原始数据，即没有设置玩法和分析方式等，如果bIgnorePlayType为TRUE，则忽略玩法检查
	virtual BOOL IsOrgData(BOOL bIgnorePlayType = FALSE);
	
	// 获取当前使用的玩法设置，返回NULL为原始玩法，lpDataName：分析名，
	// lpPTValueOut：返回玩法设置的值，这个值是由号码位置和是否取尾标志计算出来的，具有唯一性
	// lpszPTNameOut：返回玩法设置名称
	virtual LPPLAYTYPEITEM GetCurrentPlayType(LPDWORD lpPTValueOut = NULL, LPSTR lpszPTNameOut = NULL);
	// 查询指定玩法是否为原始玩法，如果lpPTItem == NULL，则查询当前玩法
	virtual BOOL IsOrgPlayType(LPPLAYTYPEITEM lpPTItem = NULL);
    
	// 获取当前使用的分析方式，返回NULL为原始数据，lpDataName：分析名，
	// lpAMValueOut：返回分析方式的值，这个值是由号码位置和是否取尾标志计算出来的，具有唯一性
	// lpszAMNameOut：返回分析方式名称
	virtual LPANALYSISMODEITEM GetCurrentAnalysisMode(LPDWORD lpAMValueOut = NULL, LPSTR lpszAMNameOut = NULL);
	// 查询指定分析方式是否为原始分析方式，如果lpAMItem == NULL，则查询当前分析方式
	virtual BOOL IsOrgAnalysisMode(LPANALYSISMODEITEM lpAMItem = NULL);
    
	// 返回当前使用的数据变换，需要强转为LPDATATRANSSET
	virtual LPVOID GetCurrentDataTrans();
	// 查询指定数据变换是否为原始数据变换，如果lpDTItem == NULL，则查询当前数据变换
	virtual BOOL IsOrgDataTrans(LPVOID lpDTItem = NULL);
    
	// 获取当前使用的排列方式
	virtual BYTE GetCurrentDataOrderType();
    
	// 获取当前使用的数据名称，根据btFlag的值（见宏定义）返回，（注意：返回值临时有效，每次调用时覆盖）
	virtual LPCSTR GetCurrentDataName(BYTE btFlag = GCDNF_NAME_LOTTERY);
	
	// 获取当前数据区分值
	virtual void GetCurrentDataDiffValue(LPDATADIFFVALUE lpValueOut, DWORD dwDataDiffMask = DDLM_GLOBAL);
    
	// 获取当前数据区分名字（一般用于自动保存文件名的组成）
	virtual LPCSTR GetCurrentDataDiffName(DWORD dwDataDiffMask = DDLM_GLOBAL);
    
	// 获取开奖号码个数，btDataSource：数据源，见宏定义
	virtual BYTE GetNumberCount(BYTE btDataSource = DATA_SOURCE_CUR);	
	// 获取开奖号码中指定位置号码的最小值，btDataSource：数据源，见宏定义
	virtual	BYTE GetMinNumber(BYTE btDataSource = DATA_SOURCE_CUR);
	// 获取开奖号码中指定位置号码的最大值，btDataSource：数据源，见宏定义
	virtual	BYTE GetMaxNumber(BYTE btDataSource = DATA_SOURCE_CUR);
	
	// 获取所有号码个数，btDataSource：数据源，见宏定义
	virtual BYTE GetAllNumberCount(BYTE btDataSource = DATA_SOURCE_CUR);
	
	// 获取号码文本，比如双色球号码02，文本为“02”，扑克号码1的文本为“A”
	// nIndex为指定索引的号码（最小号码索引为0），如果为-1，则btNumber有效，btDataSource：数据源，见宏定义
	virtual LPCSTR GetNumberText(int nIndex, BYTE btNumber = MAXBYTE, BYTE btDataSource = DATA_SOURCE_CUR);
    
	// 获取号码文本的号码值，返回MAXBYTE，则表示错误，btDataSource：数据源，见宏定义
	virtual BYTE GetNumberValue(LPCSTR lpszNumText, BYTE btDataSource = DATA_SOURCE_CUR);
    
	// 获取下一期期号，btDataSource：数据源，见宏定义
	virtual DWORD GetNextIssue(BYTE btDataSource = DATA_SOURCE_CUR);
	
	// 获取第一个指定的期号，dwIssue为指定的期号，如果为0使用tmDateTime指定的期号。
	// tmDateTime保存返回的开奖时间，返回0则表示输入的tmDateTime或dwIssue错误，否则为返回的期号
	virtual DWORD GetFirstSpecifiedIssue(CTime &tmDateTime, DWORD dwIssue = 0);
	
	// 获取下一个指定的期号，tmDateTime保存对应的开奖时间，之前必须调用GetFirstSpecifiedIssue。
	virtual DWORD GetNextSpecifiedIssue(CTime &tmDateTime);
	
	// 获取指定期号的文本串（注意：返回值临时有效，每次调用时覆盖），期号必须为实际期号，否则返回空串
	virtual LPCSTR GetIssueString(DWORD dwIssue);
	
	// 获取指定日期时间的文本串（注意：返回值临时有效，每次调用时覆盖），如果获取中包含时间并且不是高频彩，则自动不包含时间
	virtual LPCSTR GetDateTimeString(CTime &tmDateTime, BYTE btFlag = DTSF_DATETIME);
    
	// 获取dwDataIndex指定索引的前一个数据的索引，如果返回DATA_INDEX_INVALID则无效，btDataSource：数据源，见宏定义
	virtual DWORD GetPrevDataIndex(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR);
	// 获取dwDataIndex指定索引的后一个数据的索引，如果返回DATA_INDEX_INVALID则无效，btDataSource：数据源，见宏定义
	virtual DWORD GetNextDataIndex(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR);
	
	// 根据dwFlag（见宏定义）重新装载当前使用的数据集合
	virtual void ResetCurUsedData(DWORD dwFlag = RESETDATABY_RELOADDATA); 
	
	// 获取号码的遗漏值，dwFlag的取值见宏定义，bSingle是否单选遗漏
	// dwIssueIndex如果为DATA_INDEX_INVALID，则获取缩水号码lpFilterNums的遗漏值，否则获取指定索引的历史数据的遗漏值
	// lpFilterNums当dwIssueIndex为DATA_INDEX_INVALID时有效且不能为NULL，否则无效
	virtual int GetSkipValue(DWORD dwFlag, DWORD dwIssueIndex, LPBYTE lpFilterNums = NULL, BOOL bSingle = TRUE);	
	
	// 获取三态型（邻孤传等）数据，btTypeIndex为三态类型索引，见宏定义，dwIssueIndex为指定的历史数据索引值
	// lpFilterNums为传入的号码，如果lpFilterNums==NULL，则使用dwIssueIndex指定的历史号码
	virtual int GetThreeStateData(BYTE btTypeIndex, DWORD dwFlag, DWORD dwIssueIndex = 0, LPBYTE lpFilterNums = NULL);	
	
	// 获取号码值
	virtual DWORD GetNumsValue(LPBYTE lpNums, BYTE btDataSource = DATA_SOURCE_CUR);
    
public:		// 号码转换表相关函数
    
	// 根据数据值获取变换后的数据
	virtual LPBYTE DataTransTable_GetTransData(DWORD dwDataValue);
	
	// 是否是原始数据变换，即没有进行数据变换
	virtual BOOL DataTransTable_IsOrgTrans();
	
	// 数据排序，根据当前排序设置
	virtual BOOL DataTransTable_OrderData(LPBYTE lpNumsData, BYTE btNumberCount);
    
public:
	// 防破解
	virtual BOOL IsCracked();
    
public:
	void Initialize(CDataFactory * pInitDataFactory);
    
	// 当初始数据改变以后
	void OnLtyNumsChanged(LPLOTTERYPROPERTY lpLotteryProperty);
	
private:
    void _InitLotteryProperty();
    BYTE _GetValueFromSpecialChar(char caSpecialChar);
	DWORD _GetValueFromString(LPCSTR lpszString);
	BOOL _GetDataFromString(LPCSTR lpszData, BYTE btMaxNumber, LPBYTE lpData);
	DWORD _GetValueFromIssueString(LPCSTR lpszString);
    
	void _RemoveAll();	// 清除所有数据
    
	LPBYTE _CalcCXTData();
	LPBYTE _CalcLGCData();
	LPBYTE _CalcFGZData();
	LPBYTE _CalcRWLData();
	int _GetSPJData(DWORD dwFlag, DWORD dwIssueIndex, LPBYTE lpFilterNums);
	BYTE _GetRWLType(int nCalcIssueCount, LPBYTE lpDataBuf, int nNumber, int nBallCount = 10);
    
	LPDWORD _CalcSkipValueData_Single();
	LPDWORD _CalcSkipValueData_Group();
    
	void _PreResetCurUsedData(DWORD dwFlag);
	void _ResetCurUsedData(DWORD dwFlag, BOOL bUseWholeData = FALSE);
	DWORD _GetValueFromPosItem(LPLOTTERYPOSITEM lpPosItem);
	
	void _TreatDataRange();
	void _TreatMachineBall();
	void _TreatDataDiv();
	void _TreatTestNums();
	void _TreatDataOrder();
	void _TreatDataTrans();
	void _TreatDataFilter();
	
	void _InitDataToCurData();
	void _ClearCurUsedData(BOOL bInludeNums = TRUE);
    
	void _TreatPosItem(LPLOTTERYPOSITEM lpPosItem, BYTE &btDataType, BYTE &btMinNumber, BYTE &btMaxNumber);
	void _TreatPosItemFlag(BYTE btFlag, BYTE &btDataType, BYTE &btMinNumber, BYTE &btMaxNumber);
    
	BOOL _GetGPLotteryNextTime(CTime &tmDateTime, CTime &tmNextDateTime);	// 获取高频彩票的下一个时间，如果当天第一期，则返回TRUE
    
private:
    NSString            *m_strDataFileName;         // 数据文件名
    DATAFILEHEAD        m_stFileHead;               // 数据文件头
    LOTTERYPROPERTY     m_stLotteryProperty;        // 彩票属性
	CDataFactory		*m_pInitDataFactory;		// 初始数据信息工厂对象指针
    CDataFactory        m_cInitDataFactory;
	CDataFactory		m_cCurDataFactory;			// 当前数据信息工厂
	char				m_szDataName[64];			// 当前数据名称
	BYTE				m_btSectionIndex;			// 当前彩票区段索引
	PLAYTYPEITEM		m_stPTItem;					// 当前彩票玩法设置
	ANALYSISMODEITEM	m_stAMItem;					// 当前彩票分析方式
	DATATRANSSET		m_stDataTrans;				// 当前数据变换
    
	BYTE				m_btDataType;				// 开奖号码当前数据类型
	BYTE				m_btAMDataType;				// 开奖号码分析方式数据类型
	BYTE				m_btOrgDataType;			// 开奖号码原始数据类型
	BYTE				m_btInitDataType;			// 开奖号码初始数据类型
    
	BYTE				m_btNumberCount;			// 开奖号码当前数据个数
	BYTE				m_btAMNumberCount;			// 开奖号码分析方式数据个数
	BYTE				m_btOrgNumberCount;			// 开奖号码原始数据个数
	BYTE				m_btInitNumberCount;		// 开奖号码初始数据个数
    
	BYTE				m_btMinNumber;				// 开奖号码当前数据最小值
	BYTE				m_btAMMinNumber;			// 开奖号码分析方式数据最小值
	BYTE				m_btOrgMinNumber;			// 开奖号码原始数据最小值
	BYTE				m_btInitMinNumber;			// 开奖号码初始数据最小值
    
	BYTE				m_btMaxNumber;				// 开奖号码当前数据最大值
	BYTE				m_btAMMaxNumber;			// 开奖号码分析方式数据最大值
	BYTE				m_btOrgMaxNumber;			// 开奖号码原始数据最大值
	BYTE				m_btInitMaxNumber;			// 开奖号码初始数据最大值
	
    DATAFILEITEM        m_stLastDataFileItem;       // 最后一期开奖数据
    
private:
	DATARANGESET		m_stDataRange;				// 奖号范围
	DATADIVSET			m_stDataDiv;				// 奖号间隔
	BYTE				m_btDataOrderType;			// 排列方式
	int					m_nMachineIndex;			// 机号
	int					m_nBallIndex;				// 球号
	int					m_nTestNumsFlag;			// 试机号标志
	
	DATARWL				m_stRWLInfo;				// 热温冷信息
	LPBYTE				m_lpCTXData;				// 重斜跳数据信息
	LPBYTE				m_lpLGCData;				// 邻孤传数据信息
	LPBYTE				m_lpFGZData;				// 复隔中数据信息
	LPBYTE				m_lpRWLData;				// 热温冷数据信息
    
private:		// 遗漏相关
	LPDWORD				m_lpGroupSkipValueData;		// 组选遗漏值信息
	LPDWORD				m_lpSingleSkipValueData;	// 单选遗漏值信息
	DWORD				m_dwLastCheckTime;
	
private:
	char				m_szIssueString[16];		// 期号文本串
	char				m_szDateTimeString[20];		// 日期时间文本串
	char				m_szNumberText[8];			// 单个号码文本串
	DWORD				m_dwSpecifiedIssue;			// GetFirstSpecifiedIssue时启用的指定期号，未调用前设置为0
    
public:		// 防破解相关
	BYTE				m_btIsCracked;				// 是否被破解
};

