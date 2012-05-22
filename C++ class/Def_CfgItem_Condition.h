//
//  Def_CfgItem_Condition.h
//  QCLottery
//
//  Created by 武 孙 on 12-5-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ICondition.h"

// 以下定义条件相关配置相关ID
#define		CFGID_CONDITION_BEGIN			0X00030000						// 条件相关配置ID起始值

#define		CFGID_CONDITION_MINE			CFGID_CONDITION_BEGIN + 1		// 我的条件配置，数据结构：CONDITIONINFO
#define		CFGID_CONDITION_EXTEND			CFGID_CONDITION_BEGIN + 2		// 扩展条件配置，数据结构：EXTENDCONDITIONINFO数组
#define		CFGID_CONDITION_EXTEND_SYSTEM	CFGID_CONDITION_BEGIN + 3		// 系统扩展条件配置，数据结构：CDTID数组（仅供系统使用）

#define		CFGID_CONDITION_FILTERGRID		CFGID_CONDITION_BEGIN + 10		// 缩水器的条件配置，数据结构：CONDITIONSHOWSET
#define		CFGID_CONDITION_VERTSTAT		CFGID_CONDITION_BEGIN + 11		// 直落统计的条件配置
#define		CFGID_CONDITION_CODEQUERY		CFGID_CONDITION_BEGIN + 12		// 号码查询的条件配置
#define		CFGID_CONDITION_STAMQUERY		CFGID_CONDITION_BEGIN + 13		// 特征串查询的条件配置
#define		CFGID_CONDITION_GALLKILL_COMMON	CFGID_CONDITION_BEGIN + 14		// 胆杀中常用指标的条件配置

#define		CFGID_CONGROSET_DEFCON			CFGID_CONDITION_BEGIN + 20		// 条件组设置默认条件
#define		CFGID_HISTORY_CONDITION_INFO	CFGID_CONDITION_BEGIN + 21		// 历史条件设置，数据结构：HISTORYCONDITIONINFO
#define		CFGID_CONGROSET_AUTONAME		CFGID_CONDITION_BEGIN + 22		// 条件组中自动设置组名

#define		CFGID_VALIDATEDLG_OPTION		CFGID_CONDITION_BEGIN + 30		// 验证对话框中的配置

#define		CFGID_CONDITION_GALLONEROW		CFGID_CONDITION_BEGIN + 40		// 胆码条件是否单行处理设置(0不处理，1单行，否则多行)
#define		CFGID_CONDITION_REPEATONETIMES	CFGID_CONDITION_BEGIN + 41		// 缩水条件中重复个数设置
#define		CFGID_CONDITION_NUMSGROUPONEROW	CFGID_CONDITION_BEGIN + 42		// 号码组条件是否单行处理(同上)

#define		CFGID_CONDITION_PICK			CFGID_CONDITION_BEGIN + 50		// 条件提取框设置
#define		CFGID_FORMULA_PICK				CFGID_CONDITION_BEGIN + 51		// 公式提取框设置
#define		CFGID_BIGP_PICK					CFGID_CONDITION_BEGIN + 52		// 大底提取框设置

#define		CFGID_CONDITION_BTN_COMPLEX		CFGID_CONDITION_BEGIN + 60		// 复合条件按钮设置（前面为CONDITIONBUTTON_COUNT，以后依次为所有组（CONDITIONBUTTON_GROUP）和单元（CONDITIONBUTTON））
#define		CFGID_CONDITION_BTN_MINE		CFGID_CONDITION_BEGIN + 61		// 我的条件按钮设置，数据结构：CONDITIONBUTTON_MINE
#define		CFGID_CONDITION_BTN_FIX			CFGID_CONDITION_BEGIN + 62		// 固定条件按钮设置（同复合条件）

#define		CFGID_CONDITION_CUSTOM_ID		CFGID_CONDITION_BEGIN + 150		// 自定义指标配置，数据结构：DWORD(个数)+CID_CUSTOM数组
#define		CFGID_CONDITION_CUSTOM_IP		CFGID_CONDITION_BEGIN + 160		// 自定义指标参数配置，数据结构：DWORD(个数)+CIP_CUSTOM数组

////////////////////////////////////////////////////////////////////////////////////////////////////////
// 定义EXTENDCONDITIONINFO::btType的取值
#define		EXCDT_TYPE_DEFALUT			(BYTE)0X00	// 缺省状态（表示增加对应的用户扩展条件）
#define		EXCDT_TYPE_DEL				(BYTE)0X01	// 删除状态（表示对应的系统扩展条件被删除）
#define		EXCDT_TYPE_SAME				(BYTE)0X80	// 相同状态（表示对应的系统扩展条件已经存在与系统扩展条件，系统临时使用）

// 定义CONDITIONSHOWSET::btState的取值
#define		CSS_HIDE					(BYTE)0X00		// 隐藏状态
#define		CSS_SHOW					(BYTE)0X01		// 显示状态
#define		CSS_FIX						(BYTE)0X80		// 固定行设置

// 定义CONDITIONSHOWSET::btFixCounts下标的取值
#define		CSS_FIX_TOP					(BYTE)0			// 上侧固定行
#define		CSS_FIX_BOTTOM				(BYTE)1			// 下侧固定行
#define		CSS_FIX_LEFT				(BYTE)2			// 左侧固定列
#define		CSS_FIX_RIGHT				(BYTE)3			// 右侧固定列

// 定义CONDITIONPICKSET::btGroupType的取值
#define		CPS_GT_NONE					(BYTE)0			// 不分组
#define		CPS_GT_NEW					(BYTE)1			// 新建组
#define		CPS_GT_SELECT				(BYTE)2			// 选择组

// 定义CONDITIONPICKSET::btAddType的取值
#define		CPS_AT_OVER					(BYTE)0			// 覆盖
#define		CPS_AT_APPEND				(BYTE)1			// 添加

// 定义VALIDATEDLGOPTION::btEnableFlag的取值
#define		VDOPTEF_ENABLEAE			(BYTE)0X01		// 启用容错验证
#define		VDOPTEF_ONLYSHOWERROR		(BYTE)0X02		// 只显示错误的信息

// 定义CONDITIONBUTTON::btType的取值
#define		CDTBTN_TYPE_ITEM			(BYTE)0			// 单个条件
#define		CDTBTN_TYPE_GROUP			(BYTE)1			// 组
#define		CDTBTN_TYPE_ROWSET			(BYTE)10		// 行设置


// 以下定义条件相关配置相关数据结构
#pragma pack(1)

typedef struct tagCONDITIONINFO_OLD				// 条件信息（旧版本）
{
    WORD	wParentID;							// 父组ID号，0则表示顶级组
    WORD	wGroupID;							// 组ID号，当为条件信息时设置为0，为固定分组时则是固定分组ID的低字，为条件扩展组则是MAXWORD
    union
    {
        char	szName[33];						// 名称（当wGroupID>0 && wGroupID<MAXWORD时）
        DWORD	dwConditionID;					// 条件ID号（当wGroupID==0或者MAXWORD时）
    };
}CONDITIONINFO_OLD, *LPCONDITIONINFO_OLD;

typedef struct tagCONDITIONINFO					// 条件信息（2010-12-20）
{
    DWORD	dwParentID;							// 父组ID号，0则表示顶级组
    DWORD	dwGroupID;							// 组ID号，当为条件信息时设置为0，为条件组时则是组ID
    union
    {
        char	szName[33];						// 名称（当不为条件和扩展条件组时）
        CDTID	cdtID;							// 条件ID号（当为条件和扩展条件组时）
    };
}CONDITIONINFO, *LPCONDITIONINFO;

typedef struct tagEXTENDCONDITIONINFO_OLD		// 扩展条件信息（旧版本）
{
    BYTE	btType;								// 类型，见宏定义，如：EXCDT_TYPE_DEFALUT
    DWORD	dwID;								// 条件ID
}EXTENDCONDITIONINFO_OLD, *LPEXTENDCONDITIONINFO_OLD;

typedef struct tagEXTENDCONDITIONINFO			// 扩展条件信息（2010-12-22）
{
    BYTE	btType;								// 类型，见宏定义，如：EXCDT_TYPE_DEFALUT
    CDTID	cdtID;								// 条件ID
}EXTENDCONDITIONINFO, *LPEXTENDCONDITIONINFO;

typedef struct tagCONDITIONSHOWSET_OLD			// 条件显示设置（旧版本）
{
    BYTE	btState;							// 状态，见宏定义，如：CSS_HIDE
    union
    {
        DWORD	dwConfigID;						// 条件ID号，当btState不为CSS_FIX时使用，否则使用下面的
        BYTE	btFixCounts[4];					// 固定行列数，依次为上下左右，下标值见宏定义，如：CSS_FIX_LEFT，如果值为MAXBYTE，则表示无对应设置
    };
}CONDITIONSHOWSET_OLD, *LPCONDITIONSHOWSET_OLD;

typedef struct tagCONDITIONSHOWSET				// 条件显示设置（2010-12-21）
{
    BYTE	btState;							// 状态，见宏定义，如：CSS_HIDE
    union
    {
        CDTID	cdtID;							// 条件ID号，当btState不为CSS_FIX时使用，否则使用下面的
        BYTE	btFixCounts[4];					// 固定行列数，依次为上下左右，下标值见宏定义，如：CSS_FIX_LEFT，如果值为MAXBYTE，则表示无对应设置
    };
}CONDITIONSHOWSET, *LPCONDITIONSHOWSET;

typedef struct tagHISTORYCONDITIONINFO			// 历史条件信息
{
    CDTID	cdtID;								// 最后选择的条件ID号
    int		nFromIndex;							// 从第N期
    int		nIssueCount;						// 期数
}HISTORYCONDITIONINFO, *LPHISTORYCONDITIONINFO;
/*
typedef struct tagVALIDATEDLGOPTION				// 验证对话框界面配置
{
    BYTE	btType;								// 类型：0、最近一期，1：自定义最近期数，2：全部期数
    int		nCustomCount;						// 自定义最近期数数值
    BYTE	btEnableFlag;						// 启用标志，见宏定义，如：VDOPTEF_ENABLEAE
}VALIDATEDLGOPTION, *LPVALIDATEDLGOPTION;

typedef struct tagCONDITIONPICKSET				// 条件提取设置
{
    BYTE	btGroupType;						// 分组类型：0、不分组，1、新建组，2、选择组，见宏定义
    BYTE	btAddType;							// 条件类型：0、覆盖，1、追加，见宏定义
    RECT	rectDlg;							// 对话框矩形信息
    BYTE	btGroupNameType;					// 组名类型：0：自定义输入，1：使用指定条件名
    DWORD	dwGroupNameCdtIndex;				// 组名使用的条件索引，0：最后一个条件，1：第一个条件，以此类推
    BYTE	btDelOutCdt;						// 删除转出条件
    
    // 条件提取自动分组设置
    DWORD dwGroupMode;	// 分组模式 0按组数、1按每组个数
    DWORD dwGroupCount;	// 组数
    DWORD dwCountPreG;	// 每组个数
    DWORD dwRangeMode;	// 排列模式 0顺序、1随机
    BOOL bSameCount;	// 保证每组个数相同，删除多余数据
    BOOL bError;		// 本级容错
    char szError[64];	// 本级容错
    
    BOOL bErrorGroup;			// 组间容错
    char szErrorGroup[64];		// 组间容错
    
    BOOL bRandBatchGroup;		// 重复分组
    DWORD dwBatchGroupCount;	// 重复分组数
}CONDITIONPICKSET, *LPCONDITIONPICKSET;

typedef struct tagCONDITIONBUTTON_MINE			// 我的条件按钮设置
{
    BYTE	btRowCount;							// 行数
    BYTE	btRes;								// 保留
    WORD	wRes;								// 保留
}CONDITIONBUTTON_MINE, *LPCONDITIONBUTTON_MINE;

typedef struct tagCONDITIONBUTTON_COUNT			// 条件按钮个数信息
{
    BYTE	btRowCount;							// 显示行数
    WORD	wGroupCount;						// 组个数
    WORD	wItemCount;							// 单元个数
}CONDITIONBUTTON_COUNT, *LPCONDITIONBUTTON_COUNT;

typedef struct tagCONDITIONBUTTON_GROUP			// 条件按钮组
{
    BYTE	btHide;								// 是否隐藏，为0则显示
    WORD	wID;								// ID号
    DWORD	dwDefaultFltCdtID;					// 缺省缩水条件ID号
    char	szGroupName[17];					// 组名（当btType==CDTBTN_TYPE_GROUP时有效）
    WORD	wItemBeginIndex;					// 所含单元起始位置
    WORD	wItemCount;							// 所含单元个数
    COLORREF crText;							// 文字颜色
}CONDITIONBUTTON_GROUP, *LPCONDITIONBUTTON_GROUP;

typedef struct tagCONDITIONBUTTON				// 条件按钮
{
    BYTE	btHide;								// 是否隐藏，为0则显示
    BYTE	btRes;								// 预留
    WORD	wGroupID;							// 所在组ID
    DWORD	dwFilterConditionID;				// 缩水条件ID（当btType==CDTBTN_TYPE_ITEM时有效）
}CONDITIONBUTTON, *LPCONDITIONBUTTON;*/
#pragma pack()