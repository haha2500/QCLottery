//
//  DataFactory.h
//  Graph
//
//  Created by 武 孙 on 12-5-10.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "IData.h"
#include "Numbers.h"

// 定义DATAITEM中dwFlag的取值
#define	DATAITEM_FLAG_NONE					0				// 无标志
#define	DATAITEM_FLAG_TESTNUM				1				// 由试机号变成的数据

#pragma pack(1)
typedef struct tagDATAFILEITEM				// 文件中的数据单元
{
	DWORD	dwDateTime;                     // 开奖日期和时间（格式：YYMMDDhhmm，如：0911061030）
	DWORD	dwIssue;                        // 开奖期号（如：2009001等）
	BYTE	btData[MAX_NUMBER_COUNT];       // 开奖号码，数据左靠，后补0
} DATAFILEITEM, *LPDATAFILEITEM;
#pragma pack()

class CDataFactory  
{
public:
	CDataFactory();
	virtual ~CDataFactory();
	
public:
	// 指定个数创建，bCurData为TRUE时创建当前数据，否则创建初始数据
	BOOL Create(DWORD dwCount, BOOL bCurData = FALSE);
	// 销毁
	void Destroy();
	
	// 当数据个数改变后（用于数据变换）(只用于当前数据)
	void OnNumberCountChanged();
	
	// 获取总数
	DWORD GetCount(BOOL bIgnoreOnlyTest);
	
	// 设置总数
	void SetCount(DWORD dwCount);
	
	// 复制
	void Copy(CDataFactory *pSrcDataFactory);
    
    // 设置和获取数据文件中的信息，用于开奖数据文件的装载和保存
    BOOL SetDataFileItem(DWORD dwDataIndex, LPDATAFILEITEM lpDataFileItem);
    BOOL GetDataFileItem(DWORD dwDataIndex, LPDATAFILEITEM lpDataFileItem);
    
	// 设置数据
	BOOL SetItemInfo(DWORD dwDataIndex, DWORD dwDataTime, DWORD dwIssue, LPBYTE lpData);
	// 添加数据信息，返回索引号
	BOOL AddItemInfo(DWORD dwDataTime, DWORD dwIssue, LPBYTE lpData);
	// 设置数据
	void SetItemData(DWORD dwDataIndex, LPBYTE lpData);
	
	// 复制数据，只能从初始数据工厂复制到当前数据工厂
	BOOL CopyItem(DWORD dwDataIndex, DWORD dwInitDataIndex, CDataFactory *pInitDataFactory);
	
	// 根据试机号标志修改数据（必须是当前数据）
	void ModifyItemData(DWORD dwDataIndex, int nTestNumsFlag = TESTNUMSFLAG_JOIN);
    
	// 排序号码数据（必须是当前数据）
	void OrderItemData(DWORD dwDataIndex, BYTE btOrderType, BOOL bHasTestNums);
    
	// 删除最后一个数据
	BOOL DeleteLast();
	// 删除所有数据
	void DeleteAll();
    
	// 批量删除操作
	void BeginBatchDelete();
	void BatchDelete(DWORD dwDataIndex);
	void EndBatchDelete();
    
public:
	// 开奖期号和时间
	DWORD GetItemDataTimeAndIssue(DWORD dwDataIndex, DWORD &dwIssue);
    
	// 开奖期号相关
	DWORD GetItemIssue(DWORD dwDataIndex);
	LPCSTR GetItemIssueString(DWORD dwDataIndex);
    
	// 开奖日期相关，用于初始数据
	DWORD GetItemDate(DWORD dwDataIndex);
	LPCSTR GetItemDateString(DWORD dwDataIndex);
	DWORD GetItemTime(DWORD dwDataIndex);
	LPCSTR GetItemTimeString(DWORD dwDataIndex);
	int GetItemDayOfWeek(DWORD dwDataIndex);
	LPCSTR GetItemDayOfWeekString(DWORD dwDataIndex);
	void GetItemDateTime(DWORD dwDataIndex, CTime &tmDateTime);
	LPCSTR GetItemDateTimeString(DWORD dwDataIndex);
    
	// 获取对应的初始数据单元的索引，用于当前数据
	DWORD GetItemInitIndex(DWORD dwDataIndex);
    
	// 开奖号码相关
	LPBYTE GetItemNums(DWORD dwDataIndex);
	LPCSTR GetItemNumsString(DWORD dwDataIndex);
	DWORD GetItemNumsValue(DWORD dwDataIndex);
    
	// 开奖号码2相关
	LPBYTE GetItem2Nums(DWORD dwDataIndex);
	LPCSTR GetItemNums2String(DWORD dwDataIndex);
	DWORD GetItemNums2Value(DWORD dwDataIndex);
    
	// 试机号码相关
	LPBYTE GetItemTestNums(DWORD dwDataIndex);
	LPCSTR GetItemTestNumsString(DWORD dwDataIndex);
	DWORD GetItemTestNumsValue(DWORD dwDataIndex);
	BOOL IsTestNumsItem(DWORD dwDataIndex);
    
	// 机球相关
	BYTE GetItemMachineIndex(DWORD dwDataIndex);
	BYTE GetItemBallIndex(DWORD dwDataIndex);
    
	// 设置和获取预测行号码数据
	void SetForecastNums(LPBYTE lpData);
	LPBYTE GetForecastNums();
	
	// 获取指定号码的数据值
	DWORD GetNumsValue(LPBYTE lpNums);
    
public:
	// 根据区段转换数据
	void TransDataBySection(DWORD dwDataIndex, BYTE btBeginIndex, BYTE btCount);
	// 根据位置信息转换数据
	void TransDataByPosItem(DWORD dwDataIndex, LPLOTTERYPOSITEM lpPosItem, BOOL bForAM = FALSE);
    
public:
	// 防破解使用
	void RandItemData(DWORD dwDataIndex);
    
private:
#pragma pack(1)
	typedef struct tagDATAITEM	// 彩票开奖的数据单元
	{
		union
		{
			DWORD	dwDataTime;				// 开奖日期和时间，YYMMDDhhmm，用于初始数据
			DWORD	dwInitIndex;			// 对应的初始数据单元的索引，用于当前数据
		};
		union
		{
			DWORD	dwIssue;				// 开奖期号，用于初始数据
			DWORD	dwFlag;					// 数据标志，用于当前数据，见宏定义，如：DATAITEM_FLAG_TESTNUM
		};	
		BYTE	btData[MAX_NUMBER_COUNT];	// 开奖数据或者当前数据
	} DATAITEM, *LPDATAITEM;
#pragma pack()
    
private:
	BOOL		m_bIsCurData;										// 是否是当前数据
	LPDATAITEM	m_lpDataItems;										// 数据集合（通过Create和SetCount时使用的）
	DWORD		m_dwCountOfDataItems;								// 上面的数据集合中的数据个数
	CDWordArray m_cExtDataItemArray;                                // 额外的数据集合（调用_AddNew时使用的）
	
	DWORD		m_dwCurUsedDataItemCount;							// 当前使用的数据个数
	
	DWORD		m_dwBatchDeleteCount;								// 需要批量删除的总数
    
	CNumbers*	m_pNumbers;                                         // 号码管理对象
    
	char		m_szTempBuf[128];									// 临时使用
	char		m_szTempIssueStringBuf[32];							// 临时使用的期号文本
	char		m_szTempDataStringBuf[32];							// 临时使用的日期文本，可能含时间
	char		m_szTempTimeStringBuf[16];							// 临时使用的时间文本
    
	DATAITEM	m_stForcastItem;									// 预测号码单元
    
private:	// 获取指定索引的单元指针
	LPDATAITEM _GetAt(DWORD dwDataIndex);
	LPDATAITEM _GetDataItem(DWORD dwDataIndex);
    
	// 新增加一个单元，返回新的单元指针
	LPDATAITEM _AddNew();
    
	// 根据位置标志（玩法等前处理标志和后处理标志）处理数据
	void _FormatDataByFlag(BYTE btFlag, LPBYTE lpNums, BYTE btNumCount);
    
	// 组三方式排序
	void _OrderNumsByZ3Type(LPBYTE lpNumsData, BYTE btOrderType, int nBeginIndex = 0);
};