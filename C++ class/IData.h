//
//  IData.h
//  Graph
//
//  Created by 武 孙 on 12-5-10.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "WinDef.h"

// 定义彩票ID
#define     LOTTERYID_FC3D                      1           // 福彩3D

// 定义彩票属性
#define		CSTLPP_MACHINE						0X01		// 含有机号
#define		CSTLPP_BALL							0X02		// 含有球号
#define		CSTLPP_TESTNUMS						0X04		// 含有试机号

// 定义彩票类型标志
#define		CSTLTF_NEED							0X01		// 是开奖号码的必要部分
#define		CSTLTF_JOIN							0X02		// 加入第一分区共同作为开奖号码，比如广东福彩36选1的特码

// 定义开奖日标志
#define		CSTLPOF_ALL							0XFF		// 每天开奖
#define		CSTLPOF_DAY0						0X80		// 每周日开奖
#define		CSTLPOF_DAY1						0X40		// 每周一天开奖
#define		CSTLPOF_DAY2						0X20		// 每周二天开奖
#define		CSTLPOF_DAY3						0X10		// 每周三天开奖
#define		CSTLPOF_DAY4						0X08		// 每周四天开奖
#define		CSTLPOF_DAY5						0X04		// 每周五天开奖
#define		CSTLPOF_DAY6						0X02		// 每周六天开奖

// 对奖信息
#define		MAX_FIXED_PRIZE_MONEY		MAXDWORD - 100		// 最大固定奖
#define		MAX_FLOAT_PRIZE_MONEY		MAXDWORD			// 最大浮动奖

// 以下定义LOTTERYPRIZEITEM中wType的取值
#define     LPIT_SINGLE			0x11		// 单选，比如排列五等
#define     LPIT_SINGLE_NOTLINK	0x12		// 单选不连续，比如扑克选四中三等
#define     LPIT_GROUP			0x21		// 组选，比如双色球等
#define     LPIT_GROUP_MORETYPE	0x22		// 组选多种类型，比如数字三组选分为组选3和组选6等，快乐扑克等
#define     LPIT_OTHER_SSC_1	0x41		// 其他，时时彩第一种类型（新疆时时彩，如五星一等奖，五星二等奖等）
#define     LPIT_OTHER_SSC_2	0x42		// 其他，时时彩第二种类型（江西时时彩，如五星通选等）

//////////////////////////////////////////////////////////////////////////////////////////////////

// 定义数据索引值，用于GetItemXXX(DWORD dwDataIndex, DWORD dwFlag)等函数中dwDataIndex的特殊取值
#define		DATA_INDEX_INVALID					MAXDWORD			// 无效索引
#define		DATA_INDEX_FORECAST					MAXDWORD-1			// 预测期索引
#define		DATA_INDEX_LAST						MAXDWORD-2			// 最后一期索引
#define		DATA_INDEX_ONLYTESTNUMS				MAXDWORD-3			// 只含试机号的一期索引，如果没有，则使用最后一期

// 定义数据类型值，用于GetDataType的返回值
#define		DATA_TYPE_NUM_SZ					0X01				// 数字型，小于10的一位数字
#define		DATA_TYPE_NUM_LT					0X02				// 乐透型，大于10的两位数字
#define		DATA_TYPE_NUM_PK					0X04				// 扑克型，特殊的
#define		DATA_TYPE_ORDERED					0X10				// 已排序的
#define		DATA_TYPE_REPEATABLE				0X20				// 可重复的

#define		DATA_TYPE_NUM_MASK					0X0F				// 单个号码类型的掩码，用于只获取单个号码类型值（如DATA_TYPE_NUM_SZ）

// 定义数据源值，用于GetNumberCount,GetMinNumber,GetMaxNumber等函数中的btDataSource取值
#define		DATA_SOURCE_INIT					BYTE(2)			// 初始数据，未设置玩法以前，一般在需要对最初的数据进行操作时使用，比如录入数据等
#define		DATA_SOURCE_ORG						BYTE(1)			// 原始数据，设置玩法以后
#define		DATA_SOURCE_CUR						BYTE(0)			// 当前使用的数据，设置玩法和分析方式等以后

// 定义彩票最大号码个数
#define		MAX_NUMBER_COUNT					8

// 定义排列方式值
#define		DATA_ORDERTYPE_NONE					0				// 无排序
#define		DATA_ORDERTYPE_ASC					1				// 按从小到大排列
#define		DATA_ORDERTYPE_ASC_AAB				2				// 按从小到大排列且组三按AAB排列
#define		DATA_ORDERTYPE_ASC_ABA				3				// 按从小到大排列且组三按ABA排列
#define		DATA_ORDERTYPE_ASC_BAA				4				// 按从小到大排列且组三按BAA排列

////////////////////////////////////////////////////////////////////////////////////////////////////////

// 定义函数ResetCurUsedData(DWORD dwFlag)中的dwFlag取值
#define		RESETDATABY_NONE					0X00000000			// 无条件改变
#define		RESETDATABY_ISSUERANGE				0X00000001			// 因号码范围改变
#define		RESETDATABY_DIV						0X00000002			// 因数据间隔改变
// #define		RESETDATABY_ORDER					0X00000004			// 因排列顺序改变（功能改变，不在使用）
#define		RESETDATABY_MACHINE					0X00000010			// 因机号改变
#define		RESETDATABY_BALL					0X00000020			// 因球号改变
#define		RESETDATABY_TESTNUMS				0X00000040			// 因试机号设置改变
#define		RESETDATABY_DATAFILTER				0X00000100			// 因数据筛选改变
#define		RESETDATABY_DATARWL					0X00000200			// 因热温冷设置改变
#define		RESETDATABY_RELOADDATA				0XFFFFFFFF			// 因重新装载号码，其他设置需要相应的改变

// 定义GetThreeStateData(BYTE btTypeIndex, DWORD dwFlag, DWORD dwIssueIndex, LPBYTE lpFilterNums)中btTypeIndex的取值
#define		GTSDT_SPJ							0					// 升平降
#define		GTSDT_LGC							1					// 邻孤传
#define		GTSDT_FGZ							2					// 复隔中
#define		GTSDT_CXT							3					// 重斜跳
#define		GTSDT_RWL							4					// 热温冷
// 定义GetThreeStateData(BYTE btTypeIndex, DWORD dwFlag, DWORD dwIssueIndex, LPBYTE lpFilterNums)中dwFlag的取值
// 为0-9时，返回数字0-9的信息，比如为4时，如果4是邻码返回0，是孤码返回1。等，依次类推
#define		GTSDF_ONE							0XFFFF0001			// 第一位信息（返回0，1，2）
#define		GTSDF_TWO							0XFFFF0002			// 第二位信息（返回0，1，2）
#define		GTSDF_THREE							0XFFFF0003			// 第三位信息（返回0，1，2）
#define		GTSDF_FOUR							0XFFFF0004			// 第四位信息（返回0，1，2）
#define		GTSDF_FIVE							0XFFFF0005			// 第五位信息（返回0，1，2）
#define		GTSDF_SIX							0XFFFF0006			// 第六位信息（返回0，1，2）
#define		GTSDF_SEVEN							0XFFFF0007			// 第七位信息（返回0，1，2）
#define		GTSDF_EIGHT							0XFFFF0008			// 第八位信息（返回0，1，2）
#define		GTSDF_COUNT_0						0XFFFF0010			// 第一态（比如邻码、热码等）个数信息
#define		GTSDF_COUNT_1						0XFFFF0011			// 第二态（比如孤码、温码等）个数信息
#define		GTSDF_COUNT_2						0XFFFF0012			// 第三态（比如传码、冷码等）个数信息
#define		GTSDF_ARR_COUNT						0XFFFF0100			// 三态个数排列（比如210）
#define		GTSDF_ARR_SIN						0XFFFF0200			// 单选三态排列（比如221）
#define		GTSDF_ARR_SIN_VALUE					0XFFFF0300			// 单选三态排列值（比如1代表001排列）
#define		GTSDF_ARR_GRO						0XFFFF0400			// 组选三态排列（比如122）
#define		GTSDF_SHAPE							0XFFFF1000			// 三态形态信息（返回0-2，具体参考走势图）

// 定义GetHistorySkipValue和GetFilterSkipValue函数中dwFlag的取值
#define		GSKF_POS_1							0XFFFF0001			// 第一位相关遗漏值
#define		GSKF_POS_2							0XFFFF0002			// 第二位相关遗漏值
#define		GSKF_POS_3							0XFFFF0003			// 第三位相关遗漏值
#define		GSKF_POS_4							0XFFFF0004			// 第四位相关遗漏值
#define		GSKF_POS_5							0XFFFF0005			// 第五位相关遗漏值
#define		GSKF_POS_6							0XFFFF0006			// 第六位相关遗漏值
#define		GSKF_POS_7							0XFFFF0007			// 第七位相关遗漏值
#define		GSKF_POS_8							0XFFFF0008			// 第八位相关遗漏值
#define		GSKF_MIN							0XFFFF0010			// 最小遗漏值
#define		GSKF_TAIL_MIN						0XFFFF0011			// 最小遗漏值尾
#define		GSKF_MAX							0XFFFF0015			// 最大遗漏值
#define		GSKF_TAIL_MAX						0XFFFF0016			// 最大遗漏值尾
#define		GSKF_SUM							0XFFFF0020			// 相关遗漏值和值
#define		GSKF_SUM_TAIL						0XFFFF0021			// 相关遗漏值合值
#define		GSKF_TAILARR_SINGLE					0XFFFF0030			// 遗漏值尾单选排列
#define		GSKF_TAILARR_GROUP					0XFFFF0035			// 遗漏值尾组选排列

// 定义GetCurrentDataName(BYTE btFlag = 0)中的btFlag的值
#define		GCDNF_NAME_LOTTERY						0					// 彩票名称，如“福彩双色球”
#define		GCDNF_NAME_SECTION						1					// 彩票区段名称，如“红球”
#define		GCDNF_NAME_PLAYTYPE						2					// 玩法名称，如“奖号123位尾数”
#define		GCDNF_NAME_ANALYSISMODE					3					// 分析方式名称，如“玩法123位尾数”
#define		GCDNF_NAME_DATATRANS					4					// 数据变换名称，如“距离排列”
#define		GCDNF_NAME_LOTTERYANDSECTION			10					// 彩票名称和区段名称，如“福彩双色球红球”
#define		GCDNF_NAME_WHOLE						0XFF				// 数据全称，如“福彩双色球红球（奖号123位尾数）”或者“福彩双色球蓝球”

// 定义GetIssueType的返回值
#define		ISSUE_TYPE_YYYYXXX						0					// 普通类型，如2002001
#define		ISSUE_TYPE_YYYYMMDDXX					1					// 时时乐等类型，如2002010112，目前用于福彩的高频彩
#define		ISSUE_TYPE_YYXXXXXX						2					// 扑克等类型，如08000001，目前用于体彩的高频彩
#define		ISSUE_TYPE_YYYYMMDDXXX					3					// 时时彩等类型，如20080101001，由于超过10位，所以我们年只取2位，同以下值
#define		ISSUE_TYPE_YYMMDDXXX					4					// 时时彩等类型替代，如080101001
#define		ISSUE_TYPE_YYMMDDXX						5					// 山东扑克等类型，08010101

// 定义GetDateTimeString中btFlag的取值
#define		DTSF_DATE					BYTE(0X01)						// 获取日期的文本串，如：2011-04-29
#define		DTSF_TIME					BYTE(0X02)						// 获取时间的文本串，如：18:20，如果没有时间则返回空串
#define		DTSF_DATETIME				BYTE(DTSF_DATE | DTSF_TIME)		// 获取日期和时间组合的文本串，如：2011-04-29 18:20

#pragma pack(1)

typedef struct tagLOTTERYPRIZEITEM		// 中奖信息单元
{
	BYTE	btLevel;			// 奖级索引，从1开始，0表示未中奖
	DWORD	dwTypeData;			// 开奖类型相关数据，主要用于数字型和扑克型彩票的组选。计算方法：将开奖数字个数按从小到大排列，比如组选1234，则为1111，组选1224则为112，组选1122则为22等
	char	szName[16];			// 奖级名字，比如一等奖、组选24等
	DWORD	dwMoney;			// 奖级金额，见宏定义
	DWORD	dwCompareData;		// 中奖比较数据，比如取值6100则为6+1。
}LOTTERYPRIZEITEM, *LPLOTTERYPRIZEITEM;

typedef struct tagLOTTERYPRIZEGROUP			// 中奖信息组，一般一个类型的为一组
{
	BYTE	btSectionIndex;			// 彩票数据区段索引，比如0为大乐透前区，1为大乐透后区
	BYTE	btNumberCount;			// 号码个数，比如时时彩、扑克等，有从开奖号码中选择其中几个号码的玩法
	BYTE	btType;					// 开奖类型，比如组选和单选，见宏定义
	char	szName[16];				// 名字，比如组选等，可以为空字符串
	BYTE	btPrizeItemCount;		// 中奖信息单元总数
	BYTE	btPrizeItemBeginIndex;	// 起始中奖信息单元索引
	char	szDetails[512];			// 详细信息
}LOTTERYPRIZEGROUP, *LPLOTTERYPRIZEGROUP;

typedef struct tagLOTTERYPRIZEINFO			// 中奖信息
{
	BYTE					btPrizeGroupCount;		// 中奖信息组总数，范围：1-8
	LOTTERYPRIZEGROUP		stPrizeGroupArray[8];	// 中奖信息组集合，目前最大8组
	LOTTERYPRIZEITEM		stPrizeItemArray[32];	// 中奖信息单元集合，目前最大32个
}LOTTERYPRIZEINFO, *LPLOTTERYPRIZEINFO;

typedef struct tagLOTTERYSECTIONITEM	// 彩票区段单元，比如双色球红球
{
	char	szName[16];					// 区段名字，比如双色球中的“红球号码”、“蓝球号码”、“星期天号码”，只有一种类型时为“开奖号码”
	BYTE	btFlag;						// 区段标志（是否是开奖号码的必要区段等），见宏定义，如：CSTLTF_NEED
	BYTE	btPosIndex;					// 在数据中的起始位置，从0开始，比如双色球红球为0，蓝球为6，星期天为7
	BYTE	btDataType;					// 数据类型，见宏定义，如：DATA_TYPE_LT
	BYTE	btNumberCount;				// 号码个数，比如红球个数为6，蓝球则为1
	BYTE	btMinNumber;				// 最小号码，比如红球和蓝球均为1
	BYTE	btMaxNumber;				// 最大号码，比如红球为33，蓝球为16
}LOTTERYSECTIONITEM, *LPLOTTERYSECTIONITEM;

typedef struct tagLOTTERYDATADOWNLOADINFO
{
	BYTE	btEnableUDP;				// 是否允许UDP数据下载方式，1：允许
	char	szURL[64];					// HTTP方式下载数据时使用的URL
	DWORD	dwLastDate;					// 最后下载日期（YYYYMMDD）
	DWORD	dwLastTime;					// 最后下载时间（HHMMSS）
}LOTTERYDATADOWNLOADINFO, *LPLOTTERYDATADOWNLOADINFO;

typedef struct tagLOTTERYPROPERTY			// 彩票属性
{
	DWORD	dwLotteryID;						// 彩票ID，见宏定义
	char	szLotteryName[32];					// 彩票名
	BYTE	btProperty;							// 属性（是否含有试机号等），见宏定义，如CSTLPP_TESTNUMS					
	BYTE	btOpenFlag;							// 开奖日标志，见宏定义，如CSTLPOF_DAY0
	BYTE	btIssueType;						// 开奖期号类型，比如DATAISSUE_TYPE_YYYYXXX
	WORD	wOpenTimeArray[256];				// 开奖时间数组（按顺序）
	BYTE	btOpenTimesPerDay;					// 每日开奖次数（用于快乐扑克等高频彩种）
	BYTE	btOpenIntervalMinutes;				// 开奖间隔时间（单位分）（用于快乐扑克等高频彩种）
	LOTTERYDATADOWNLOADINFO stDownloadDataInfo;	// 数据下载信息
	BYTE	btSectionCount;						// 区段个数（目前范围：1-4），比如3D、P3、P5、7X、22选5为1个类型，七乐彩、大乐透、29选7为2个类型，双色球为3个类型（红球、蓝球、星期天）
	LOTTERYSECTIONITEM	stSectionInfo[4];		// 区段信息
	LOTTERYPRIZEINFO	stPrizeInfo;			// 中奖信息
}LOTTERYPROPERTY, *LPLOTTERYPROPERTY;

// 定义彩票分位信息LOTTERYPOSITEM中btFlagPrev与btFlagNext的取值
#define LPIF_NONE			BYTE(0)			// 不处理
#define LPIF_ORDER			BYTE(1)			// 排序
#define LPIF_TAIL			BYTE(2)			// 取尾数
#define LPIF_ORDER_TAIL		BYTE(3)			// 排序后取尾数
#define LPIF_TAIL_ORDER		BYTE(4)			// 取尾数后排序
#define LPIF_ORG			BYTE(9)			// 原始玩法或原始分析方式

typedef struct tagLOTTERYPOSITEM		// 彩票分位信息（包括分析方式和玩法设置）
{
	BYTE	btFlagPrev;					// 取位前处理标志，见上面的宏定义
	BYTE	btFlagNext;					// 取位后处理标志，见上面的宏定义
	BYTE	btCount;					// 分位个数，取值1-8
	BYTE	btPosArray[8];				// 位置信息
	char	szName[32];					// 名字
    
	// 以下重载相关操作符
	BOOL operator==(const tagLOTTERYPOSITEM &stValue)	// 不比较名称
	{
		return !memcmp(&stValue, this, sizeof(tagLOTTERYPOSITEM) - sizeof(szName));
	};
	BOOL operator!=(const tagLOTTERYPOSITEM &stValue)	// 不比较名称
	{
		return memcmp(&stValue, this, sizeof(tagLOTTERYPOSITEM) - sizeof(szName));
	};
	const tagLOTTERYPOSITEM& operator=(const tagLOTTERYPOSITEM &stValue)
	{
		memcpy(this, &stValue, sizeof(stValue)); 
		return *this;
	};
}LOTTERYPOSITEM, *LPLOTTERYPOSITEM, ANALYSISMODEITEM, *LPANALYSISMODEITEM, PLAYTYPEITEM, *LPPLAYTYPEITEM;

////////////////////////////////////////////////////////////////////////////////////////////////////////
// 定义数据区分掩码值
#define DDLM_NONE						0X00000000		// 无区分

#define DDLM_LOTTERYID					0X00000001		// 彩票ID号
#define DDLM_LOTTERYPROPERTY			0X00000002		// 彩票属性
#define DDLM_ISHIGHFREQ					0X00000004		// 是否高频彩
#define DDLM_SECTIONCOUNT				0X00000008		// 区段总数
#define DDLM_ISORGDATA					0X00000010		// 是否原始数据
#define DDLM_SECTION1_DATATYPE			0X00000020		// 彩票第一区段数据类型
#define DDLM_SECTION1_NUMBERCOUNT		0X00000040		// 彩票第一区段号码个数
#define DDLM_SECTION1_MINNUMBER			0X00000080		// 彩票第一区段最小号码
#define DDLM_SECTION1_MAXNUMBER			0X00000100		// 彩票第一区段最大号码
#define DDLM_SECTION2_DATATYPE			0X00000200		// 彩票第二区段数据类型
#define DDLM_SECTION2_NUMBERCOUNT		0X00000400		// 彩票第二区段号码个数
#define DDLM_SECTION2_MINNUMBER			0X00000800		// 彩票第二区段最小号码
#define DDLM_SECTION2_MAXNUMBER			0X00001000		// 彩票第二区段最大号码

#define DDLM_SECTIONINDEX				0X00002000		// 彩票区段索引

#define DDLM_PLAYTYPE					0X00004000		// 玩法标志，玩法的唯一区分数据
#define DDLM_ISORGPT					0X00008000		// 是否原始玩法
#define DDLM_ORG_DATATYPE				0X00010000		// 原始数据类型
#define DDLM_ORG_NUMBERCOUNT			0X00020000		// 原始号码个数
#define DDLM_ORG_MINNUMBER				0X00040000		// 原始数据最小号码
#define DDLM_ORG_MAXNUMBER				0X00080000		// 原始数据最大号码

#define DDLM_ANALYSISMODE				0X00100000		// 分析方式标志，分析方式的唯一区分数据
#define DDLM_ISORGAM					0X00200000		// 是否原始分析方式
#define DDLM_CAM_DATATYPE				0X00400000		// 当前分析方式的数据类型
#define DDLM_CAM_NUMBERCOUNT			0X00800000		// 当前分析方式的号码个数
#define DDLM_CAM_MINNUMBER				0X01000000		// 当前分析方式的数据最小号码
#define DDLM_CAM_MAXNUMBER				0X02000000		// 当前分析方式的数据最大号码

#define DDLM_DATATRANS					0X04000000		// 数据变换标志
// #define DDLM_ISORGDT					0X08000000		// 是否原始数据变换（2011-03-18去掉，改为排列方式）
#define DDLM_CUR_DATATYPE				0X10000000		// 当前数据类型
#define DDLM_CUR_NUMBERCOUNT			0X20000000		// 当前号码个数
#define DDLM_CUR_MINNUMBER				0X40000000		// 当前数据最小号码
#define DDLM_CUR_MAXNUMBER				0X80000000		// 当前数据最大号码

#define DDLM_CUR_ORDERTYPE				0X08000000		// 当前数据排列方式

// 全局区分，区分所有，比如条件文件的自动保存时使用的临时文件名
#define DDLM_GLOBAL						DDLM_LOTTERYID | DDLM_SECTIONINDEX | DDLM_PLAYTYPE | DDLM_ANALYSISMODE | DDLM_DATATRANS | DDLM_CUR_ORDERTYPE

//////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef struct tagDATADIFFVALUE						// 数据区分值（新版，2011-03-21）
{
	BYTE btData[32];								
    
	// 以下重载相关操作符
	BOOL operator==(const tagDATADIFFVALUE &stValue)
	{
		return !memcmp(&stValue, this, sizeof(tagDATADIFFVALUE));
	};
	BOOL operator!=(const tagDATADIFFVALUE &stValue)
	{
		return memcmp(&stValue, this, sizeof(tagDATADIFFVALUE));
	};
	const tagDATADIFFVALUE& operator=(const tagDATADIFFVALUE &stValue)
	{
		memcpy(this, &stValue, sizeof(stValue)); 
		return *this;
	};
}DATADIFFVALUE, *LPDATADIFFVALUE;

#pragma pack()

class IData  
{
public:     // 数据文件相关操作
    // 打开彩票文件并载入数据，此时初始号码数据有效
    virtual BOOL OpenLotteryFile() = 0;
    // 关闭文件并释放载入的数据，初始号码变为无效
    virtual void CloseLotteryFile(BOOL bSave = FALSE) = 0;
    
    // 增加开奖号
	virtual BOOL AddLtyNums(DWORD dwDataTime, DWORD dwIssue, LPBYTE lpData) = 0;
	// 修改开奖号
	virtual BOOL ModifyLtyNums(DWORD dwIndex, DWORD dwDataTime, DWORD dwIssue, LPBYTE lpData) = 0;
	// 删除最后的开奖号
	virtual BOOL DeleteLastLtyNums() = 0;
	// 删除全部开奖号
	virtual BOOL DeleteAllLtyNums() = 0;
    
	// 下载开奖号码，下载失败返回FALSE，当下载成功时bModified有效，包含TRUE则表示数据更新过
	virtual BOOL DownloadLtyNums(BOOL &bModified) = 0;
    
public:     // 内存中的数据相关操作
    
	// 获取彩票ID
	virtual DWORD GetLotteryID() = 0;
	// 获取到彩票属性
	virtual LPLOTTERYPROPERTY GetLotteryProperty() = 0;
    
	// 获取开奖期号类型，返回宏定义（如：DATAISSUE_TYPE_YYYYXXX）
	virtual BYTE GetIssueType() = 0;
	// 获取开奖期号数据长度，bOnlyLastXXX为TRUE时只获取最后需要的几位，比如ISSUE_TYPE_YYYYXXX类型的则是最后3位，长度为3
	virtual BYTE GetIssueLen(BOOL bOnlyLastXXX = FALSE) = 0;
	
	// 获取数据类型，btDataSource：数据源，见宏定义
	virtual BYTE GetDataType(BYTE btDataSource = DATA_SOURCE_CUR) = 0;
    
	// 获取数据集合中元素总数
	virtual DWORD GetItemCount(BYTE btDataSource = DATA_SOURCE_CUR) = 0;
    
	// 获取指定索引的开奖日期（如：20091105）
	virtual DWORD GetItemDate(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR) = 0;
	// 获取指定索引的开奖日期文本串（如：2009-11-05，注意：返回值临时有效，每次调用时覆盖）
	virtual LPCSTR GetItemDateString(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR) = 0;
    
	// 获取指定索引的开奖时间（如：1220，只含小时和分钟，不包含日期和秒）
	virtual DWORD GetItemTime(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR) = 0;
	// 获取指定索引的开奖时间（如：12:20，只含小时和分钟，不包含日期和秒）文本串（注意：返回值临时有效，每次调用时覆盖）
	virtual LPCSTR GetItemTimeString(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR) = 0;
	
	// 获取指定索引的开奖星期几，星期日为0，星期一为1，以此类推
	virtual int GetItemDayOfWeek(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR) = 0;
	// 获取指定索引的开奖星期几文本串，星期日为“日”，星期一为“一”，以此类推
	virtual LPCSTR GetItemDayOfWeekString(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR) = 0;
    
	// 获取指定索引的开奖日期和时间
//	virtual void GetItemDateTime(DWORD dwDataIndex, CTime &tmDateTime, BYTE btDataSource = DATA_SOURCE_CUR) = 0;
	// 获取指定索引的开奖日期和时间文本串（如高频彩为：2011-04-07 11:25，其他为：2011-04-07 11:25）
	virtual LPCSTR GetItemDateTimeString(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR) = 0;
    
	// 获取指定索引的开奖期号
	virtual DWORD GetItemIssue(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR) = 0;
	// 获取指定索引的开奖期号文本串（注意：返回值临时有效，每次调用时覆盖）
	virtual LPCSTR GetItemIssueString(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR) = 0;
    
	// 获取指定索引的开奖号码数据，返回NULL则表示没有该开奖号码数据
	virtual LPBYTE GetItemNums(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR) = 0;
	// 获取指定索引的开奖号码的文本串（注意：返回值临时有效，每次调用时覆盖）
	virtual LPCSTR GetItemNumsString(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR) = 0;
	// 获取指定索引的开奖号码对应的整型值
	virtual DWORD GetItemNumsValue(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR) = 0;
    
	// 获取指定索引的第2开奖号码（比如当前为双色球红球，则获取蓝球，如果为蓝球，则获取红球）数据，返回NULL则表示没有该数据
	virtual LPBYTE GetItemNums2(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR) = 0;
	// 获取指定索引的第2开奖号码（同上）的文本串（注意：返回值临时有效，每次调用时覆盖）
	virtual LPCSTR GetItemNums2String(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR) = 0;
	// 获取指定索引的第2开奖号码（同上）对应的整型值
	virtual DWORD GetItemNums2Value(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR) = 0;
    
	// 获取指定索引的试机号码数据，返回NULL则表示没有该开奖号码数据
	virtual LPBYTE GetItemTestNums(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR) = 0;
	// 获取指定索引的试机号码的文本串（注意：返回值临时有效，每次调用时覆盖）
	virtual LPCSTR GetItemTestNumsString(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR) = 0;
	// 获取指定索引的试机号码对应的整型值
	virtual DWORD GetItemTestNumsValue(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR) = 0;
	// 判断当前数据是否是由试机号变来的
	virtual BOOL IsTestNumsItem(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR) = 0;
	
	// 获取指定索引的机器号
	virtual BYTE GetItemMachineIndex(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR) = 0;
	// 获取指定索引的球号
	virtual BYTE GetItemBallIndex(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR) = 0;
    
	// 根据当前数据索引获取对应的初始数据的索引，没找到时返回DATAINDEX_INVALID
	virtual DWORD GetInitItemIndex(DWORD dwCurDataIndex) = 0;
    
	// 设置预测行号码数据，lpData为NULL时，清除预测行号码数据
	virtual void SetForecastNums(LPBYTE lpNumsData = NULL) = 0;
	// 获取预测行号码数据，没有则返回NULL
	virtual LPBYTE GetForecastNums() = 0;
    
	// 设置彩票号码区段索引
	virtual void SetSectionIndex(BYTE btSectionIndex = 0) = 0;
	// 设置彩票玩法信息
	virtual void SetPlayType(LPPLAYTYPEITEM lpPTItem) = 0;
	// 设置分析方式信息
	virtual void SetAnalysisMode(LPANALYSISMODEITEM lpAMItem) = 0;
	// 设置数据变换信息
	virtual void SetDataTrans(LPVOID lpDataTrans) = 0;
	// 设置排列方式
	virtual void SetDataOrderType(BYTE btDataOrderType) = 0;
    
	// 获取当前使用的彩票数据区段的索引，比如双色球的红蓝球等
	virtual BYTE GetCurrentSectionIndex() = 0;
    
	// 是否是原始数据，即没有设置玩法和分析方式等，如果bIgnorePlayType为TRUE，则忽略玩法检查
	virtual BOOL IsOrgData(BOOL bIgnorePlayType = FALSE) = 0;
    
	// 获取当前使用的玩法设置，lpDataName：分析名，
	// lpPTValueOut：返回玩法设置的值，这个值是由号码位置和是否取尾标志计算出来的，具有唯一性
	// lpszPTNameOut：返回玩法设置名称
	virtual LPPLAYTYPEITEM GetCurrentPlayType(LPDWORD lpPTValueOut = NULL, LPSTR lpszPTNameOut = NULL) = 0;
	// 查询指定玩法是否为原始玩法，如果lpPTItem == NULL，则查询当前玩法
	virtual BOOL IsOrgPlayType(LPPLAYTYPEITEM lpPTItem = NULL) = 0;
	
	// 获取当前使用的分析方式，lpDataName：分析名，
	// lpAMValueOut：返回分析方式的值，这个值是由号码位置和是否取尾标志计算出来的，具有唯一性
	// lpszAMNameOut：返回分析方式名称
	virtual LPANALYSISMODEITEM GetCurrentAnalysisMode(LPDWORD lpAMValueOut = NULL, LPSTR lpszAMNameOut = NULL) = 0;
	// 查询指定分析方式是否为原始分析方式，如果lpAMItem == NULL，则查询当前分析方式
	virtual BOOL IsOrgAnalysisMode(LPANALYSISMODEITEM lpAMItem = NULL) = 0;
    
	// 返回当前使用的数据变换，需要强转为LPDATATRANSSET
	virtual LPVOID GetCurrentDataTrans() = 0;
	// 查询指定数据变换是否为原始数据变换，如果lpDTItem == NULL，则查询当前数据变换
	virtual BOOL IsOrgDataTrans(LPVOID lpDTItem = NULL) = 0;
    
	// 获取当前使用的排列方式
	virtual BYTE GetCurrentDataOrderType() = 0;
    
	// 获取当前使用的数据名称，根据btFlag的值（见宏定义）返回，（注意：返回值临时有效，每次调用时覆盖）
	virtual LPCSTR GetCurrentDataName(BYTE btFlag = GCDNF_NAME_LOTTERY) = 0;
    
	// 获取当前数据区分值
	virtual void GetCurrentDataDiffValue(LPDATADIFFVALUE lpValueOut, DWORD dwDataDiffMask = DDLM_GLOBAL) = 0;
	// 获取当前数据区分名字（一般用于自动保存文件名的组成）
	virtual LPCSTR GetCurrentDataDiffName(DWORD dwDataDiffMask = DDLM_GLOBAL) = 0;
	
	// 获取开奖号码个数，btDataSource：数据源，见宏定义
	virtual BYTE GetNumberCount(BYTE btDataSource = DATA_SOURCE_CUR) = 0;	
	// 获取开奖号码中指定位置号码的最小值，btDataSource：数据源，见宏定义
	virtual	BYTE GetMinNumber(BYTE btDataSource = DATA_SOURCE_CUR) = 0;
	// 获取开奖号码中指定位置号码的最大值，btDataSource：数据源，见宏定义
	virtual	BYTE GetMaxNumber(BYTE btDataSource = DATA_SOURCE_CUR) = 0;
    
	// 获取所有号码个数，btDataSource：数据源，见宏定义
	virtual BYTE GetAllNumberCount(BYTE btDataSource = DATA_SOURCE_CUR) = 0;
    
	// 获取号码文本，比如双色球号码02，文本为“02”，扑克号码1的文本为“A”
	// nIndex为指定索引的号码（最小号码索引为0），如果为-1，则btNumber有效，btDataSource：数据源，见宏定义
	virtual LPCSTR GetNumberText(int nIndex, BYTE btNumber = MAXBYTE, BYTE btDataSource = DATA_SOURCE_CUR) = 0;
    
	// 获取号码文本的号码值，返回MAXBYTE，则表示错误，btDataSource：数据源，见宏定义
	virtual BYTE GetNumberValue(LPCSTR lpszNumText, BYTE btDataSource = DATA_SOURCE_CUR) = 0;
    
	// 获取下一期期号，btDataSource：数据源，见宏定义
	virtual DWORD GetNextIssue(BYTE btDataSource = DATA_SOURCE_CUR) = 0;
    
	// 获取第一个指定的期号，dwIssue为指定的期号，如果为0使用tmDateTime指定的期号。
	// tmDateTime保存返回的开奖时间，返回0则表示输入的tmDateTime或dwIssue错误，否则为返回的期号
//	virtual DWORD GetFirstSpecifiedIssue(CTime &tmDateTime, DWORD dwIssue = 0) = 0;
    
	// 获取下一个指定的期号，之前必须调用GetFirstSpecifiedIssue。
	// tmDateTime必须为GetFirstSpecifiedIssue中的tmDateTime，即作为输入（当期时间）也作为输出（下期时间）
//	virtual DWORD GetNextSpecifiedIssue(CTime &tmDateTime) = 0;
    
	// 获取指定期号的文本串（注意：返回值临时有效，每次调用时覆盖），期号必须为实际期号，否则返回空串
	virtual LPCSTR GetIssueString(DWORD dwIssue) = 0;
    
	// 获取指定日期时间的文本串（注意：返回值临时有效，每次调用时覆盖），如果获取中包含时间并且不是高频彩，则自动不包含时间
//	virtual LPCSTR GetDateTimeString(CTime &tmDateTime, BYTE btFlag = DTSF_DATETIME) = 0;
    
	// 获取dwDataIndex指定索引的前一个数据的索引，如果返回DATA_INDEX_INVALID则无效，btDataSource：数据源，见宏定义
	virtual DWORD GetPrevDataIndex(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR) = 0;
	// 获取dwDataIndex指定索引的后一个数据的索引，如果返回DATA_INDEX_INVALID则无效，btDataSource：数据源，见宏定义
	virtual DWORD GetNextDataIndex(DWORD dwDataIndex, BYTE btDataSource = DATA_SOURCE_CUR) = 0;
    
	// 根据dwFlag（见宏定义）重新装载当前使用的数据集合
	virtual void ResetCurUsedData(DWORD dwFlag = RESETDATABY_RELOADDATA) = 0; 
	
	// 获取号码的遗漏值，dwFlag的取值见宏定义，bSingle是否单选遗漏
	// dwIssueIndex如果为DATA_INDEX_INVALID，则获取缩水号码lpFilterNums的遗漏值，否则获取指定索引的历史数据的遗漏值
	// lpFilterNums当dwIssueIndex为DATA_INDEX_INVALID时有效且不能为NULL，否则无效
	virtual int GetSkipValue(DWORD dwFlag, DWORD dwIssueIndex, LPBYTE lpFilterNums = NULL, BOOL bSingle = TRUE) = 0;	
    
	// 获取三态型（邻孤传等）数据，btTypeIndex为三态类型索引，见宏定义，dwIssueIndex为指定的历史数据索引值
	// lpFilterNums为传入的号码，如果lpFilterNums==NULL，则使用dwIssueIndex指定的历史号码
	virtual int GetThreeStateData(BYTE btTypeIndex, DWORD dwFlag, DWORD dwIssueIndex = 0, LPBYTE lpFilterNums = NULL) = 0;	
	
	// 获取号码值
	virtual DWORD GetNumsValue(LPBYTE lpNums, BYTE btDataSource = DATA_SOURCE_CUR) = 0;
    
public:		// 号码转换表相关函数
    
	// 根据数据值获取变换后的数据
	virtual LPBYTE DataTransTable_GetTransData(DWORD dwDataValue) = 0;
	
	// 是否是原始数据变换，即没有进行数据变换
	virtual BOOL DataTransTable_IsOrgTrans() = 0;
	
	// 数据排序，根据当前排序设置
	virtual BOOL DataTransTable_OrderData(LPBYTE lpNumsData, BYTE btNumberCount) = 0;
	
public:
	// 防破解
	virtual BOOL IsCracked() = 0;
};

extern IData *g_pIData;

