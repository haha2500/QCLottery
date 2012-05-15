//
//  Def_CfgItem_Data.h
//  Graph
//
//  Created by 武 孙 on 12-5-10.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

// 以下定义数据相关配置相关ID
#define		CFGID_DATA_BEGIN				0X00010000						// 数据相关配置ID起始值

#define		CFGID_DATA_LOTTERY_ID			CFGID_DATA_BEGIN + 1			// 当前使用的彩票的ID号
#define		CFGID_DATA_SECTION_INDEX		CFGID_DATA_BEGIN + 2			// 当前使用的彩票区段索引
#define		CFGID_DATA_PLAYTYPE_INDEX		CFGID_DATA_BEGIN + 3			// 当前使用的彩票玩法索引
#define		CFGID_DATA_ANALYSISMODE_INDEX	CFGID_DATA_BEGIN + 4			// 当前使用的分析方式索引
#define		CFGID_DATA_TRANSFORM_INDEX		CFGID_DATA_BEGIN + 5			// 当前使用的数据变换索引

#define		CFGID_DATA_LOTTERY_INFO			CFGID_DATA_BEGIN + 11			// 彩票信息，数据结构：DWORD数组
#define		CFGID_DATA_PLAYTYPE_INFO		CFGID_DATA_BEGIN + 12			// 玩法信息，数据结构：LOTTERYPOSITEM数组
#define		CFGID_DATA_ANALYSISMODE_INFO	CFGID_DATA_BEGIN + 13			// 分析方式信息，数据结构：LOTTERYPOSITEM数组
#define		CFGID_DATA_TRANSFORM_INFO		CFGID_DATA_BEGIN + 14			// 数据变换信息，数据结构：DATATRANSSET数组

#define		CFGID_DATA_MACHINEBALL			CFGID_DATA_BEGIN + 21			// 当前使用的机（十位）球（个位）号
#define		CFGID_DATA_TESTNUMSFLAG			CFGID_DATA_BEGIN + 23			// 试机号设置，见宏定义，如TESTNUMSFLAG_JOIN
#define		CFGID_DATA_RANGE				CFGID_DATA_BEGIN + 24			// 号码范围设置，数据结构：DATARANGESET
#define		CFGID_DATA_DIV					CFGID_DATA_BEGIN + 25			// 号码间隔设置，数据结构：DATADIVSET
#define		CFGID_DATA_ORDERTYPE			CFGID_DATA_BEGIN + 26			// 排列方式，数据结构：DATAORDERTYPE
#define		CFGID_DATA_FILTER				CFGID_DATA_BEGIN + 28			// 数据筛选

#define		CFGID_DATA_RWL					CFGID_DATA_BEGIN + 50			// 热温冷设置，数据结构：DATARWL
#define		CFGID_DATA_PRODUCT				CFGID_DATA_BEGIN + 51			// 积数设置，数据结构：DATAPRODUCT
#define		CFGID_DATA_AREA					CFGID_DATA_BEGIN + 52			// 分区设置，数据结构：DATAAREA

#define		CFGID_DATA_NUMBERS_SEP_INIT		CFGID_DATA_BEGIN + 100			// 初始数据分隔符，数据结构：NUMBERSSEPARATOR
#define		CFGID_DATA_NUMBERS_SEP_ORG		CFGID_DATA_BEGIN + 101			// 原始数据分隔符，数据结构：NUMBERSSEPARATOR
#define		CFGID_DATA_NUMBERS_SEP_CUR		CFGID_DATA_BEGIN + 102			// 当前数据分隔符，数据结构：NUMBERSSEPARATOR

#define		CFGID_DATA_AREALIST_BEGIN		CFGID_DATA_BEGIN + 200			// 分区数据列表开始（0分区，+2则是2分区），数据结构：DWORD(总数) +　字符串（每个分区间使用“\0”分隔）
//            ................
#define		CFGID_DATA_AREALIST_END			CFGID_DATA_BEGIN + 299			// 分区数据列表结束，数据结构：同上

// 定义试机号设置值
#define		TESTNUMSFLAG_NONE				0								// 未设置
#define		TESTNUMSFLAG_JOIN				1								// 将试机号视为开奖号
#define		TESTNUMSFLAG_SWAP				2								// 试机号与开奖号互换

// 定义号码分区相关
#define		MAXCOUNT_DATAAREA				100			// 最大自定义号码分区个数
#define		MAXCOUNT_ITEMOFDATAAREA			50			// 每个号码分区最大条件单元个数

// 定义DATATRANSSET::btType的取值
#define		DATATRANS_TYPE_ORG				0			// 原始数据
#define		DATATRANS_TYPE_VALID			1			// 有效数据

// 定义DATATRANSSET::wID的系统取值
#define		DATATRANS_ID_ORG				0			// 原始数据ID
#define		DATATRANS_ID_DISARR				1			// 距离排列ID
#define		DATATRANS_ID_SUMTAILARR			2			// 位合排列ID
#define		DATATRANS_ID_SUBABSARR			3			// |位差|排列ID
#define		DATATRANS_ID_MULTAILARR			4			// 位积尾排列ID
#define		DATATRANS_ID_SINSKIPTAILARR		5			// 单遗尾排列ID
#define		DATATRANS_ID_GROSKIPTAILARR		6			// 组遗尾排列ID
#define		DATATRANS_ID_SUMSPANAVGTAIL		7			// 合值跨度均值ID
#define		DATATRANS_ID_SUMSPANTAIL		8			// 合值跨度ID
#define		DATATRANS_ID_SUMTAIL			9			// 合值跨度ID
#define		DATATRANS_ID_USER				100			// 用户自定义ID起始值
#define		DATATRANS_ID_MAX				1000		// 最大ID

// 以下定义数据相关配置相关数据结构
#pragma pack(1)

typedef struct tagDATADIVSET			// 数据分隔设置
{
	BYTE	btDivType;					// 分隔类型：0：无分隔，1：隔期，2：按期号，3：按日期
	BYTE	btIssueType;				// 按期类型：0：期尾，1：期余数
	BYTE	btDateType;					// 日期类型：0：星期，1：月
	int		nDivCount;					// 分隔期数
	BYTE	btDivFrom;					// 0：从预测行开始，1：从最后一期开始
	BYTE	btIssueTail[10];			// 期号尾数信息，对应下标的值为1，则被选择
	DWORD	dwIssueRem[2];				// 期号余数信息，第一个元素为除数，第二个元素为余数
	BYTE	btWeekDays[7];				// 星期信息，对应下标的值为1，则被选择
	BYTE	btMonthDays[31];			// 月日信息，对应下标的值为1，则被选择
	char	szMonthDays[64];			// 月日保存字符串
	BYTE	btTimesIndex;				// 每日开奖时间索引
	WORD	wDayDivMinutes;				// 每日间隔时间，分，如果为0，则表示未设置每日间隔
	WORD	wDayDivBeginTime;			// 每日间隔起始时间：hhmm
}DATADIVSET, *LPDATADIVSET;

typedef struct tagDATARANGESET			// 号码范围设置
{
	BYTE	btRangeType;				// 期号范围类型，0：全部，1：最近一年，2：最近一天，3：指定范围，4：保留最近N期，5：排除最近N期（已取消，2009-08-12）
	int		nBeginIssue;				// 起始期号，当btRangeType==3时有效，如果为负数，则为按索引（暂时不使用）
	int		nEndIssue;					// 终止期号，当btRangeType==3时有效，如果为-1，则表示为最新期号
	int		nFromEndCount;				// 从终止期开始往前多少期，小于0则表示未选择，绝对值为当前值
	int		nKeepLastIssueCount;		// 保留最后多少期
	int		nReverseLastIssueCount;		// 排除最后多少期，小于0则表示未选择，绝对值为当前值
	int		nNValue;					// 快捷键中的N的值
}DATARANGESET, *LPDATARANGESET;

typedef struct tagDATATRANSSET			// 数据变换设置
{
	BYTE	btType;						// 类型，见宏定义，如：DATATRANS_TYPE_ORG
	WORD	wID;						// ID号
	char	szName[33];					// 名称
	BYTE	btItemCount;				// 数据个数
	char	szItemNames[8][129];		// 数据名称集合
	DWORD	dwCdtIDs[8];				// 每个号码对应的条件ID集合，如果没有设置条件，则为初始条件，并且其中的CDTIID::dwSystemIPID为指定的固定值
}DATATRANSSET, *LPDATATRANSSET;

typedef struct tagDATAORDERTYPE			// 排列方式
{
	BYTE	btEnable;					// 0：不排列，1：按从小到大排列，2：按从小到大排列且启用组三排列方式
	BYTE	btZ3Type;					// 组3排列方式；0：按AAB排列，1：按ABA排列，2：按BAA排列
}DATAORDERTYPE, *LPDATAORDERTYPE;

typedef struct tagDATARWL				// 热温冷设置
{
	WORD	wIssueCount;				// 分析期数
	WORD	wWMMinCount;				// 温码的最小次数（当前使用）
	WORD	wWMMaxCount;				// 温码的最大次数（当前使用）
	WORD	wWMMinCountCustom;			// 温码的最小次数（自定义）
	WORD	wWMMaxCountCustom;			// 温码的最大次数（自定义）
	BYTE	btIsCustom;					// 1：启用自定义参数
	BYTE	btRepeatToOne;				// 为1则表示重复数字算出现1次
}DATARWL, *LPDATARWL;

typedef struct tagDATAPRODUCT			// 积数设置
{
	WORD	wMinMaxProduct;				// 最小的大积数
	BYTE	bt0Value;					// 号码0的值，最高位为1则是启用变换，低7位为变换值
	BYTE	bt1Value;					// 号码1的值，最高位为1则是启用变换，低7位为变换值
}DATAPRODUCT, *LPDATAPRODUCT;

typedef struct tagNUMBERSSEPARATOR 		// 号码分割符设置
{
	char	szBetweenNums[9];			// 两组号码之间的分隔符
	char	szInSection[2][9];			// 各区段内单个号码间的分割符集合
	char	szBetweenSection[9];		// 相邻区段之间分割符
}NUMBERSSEPARATOR, *LPNUMBERSSEPARATOR;
#pragma pack()