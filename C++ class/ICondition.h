//
//  ICondition.h
//  QCLottery
//
//  Created by 武 孙 on 12-5-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "MFCClass.h"
#include "QuickAddString.h"
#include "INumbers.h"
#include "IData.h"

// 定义指标ID号CDTIID中btType的取值
#define		CDTIID_TYPE_CUSTOM					BYTE(0X80)		// 自定义指标，否则为系统指标
#define		CDTIID_TYPE_DYNAMIC					BYTE(0X40)		// 动态指标，否则为静态指标
#define		CDTIID_TYPE_DATAAREA				BYTE(0X20)		// 号码分区指标
#define		CDTIID_TYPE_TESTNUMS				BYTE(0X10)		// 试机号相关指标
#define		CDTIID_TYPE_VALID					BYTE(0X01)		// 有效指标，否则为无效指标
#define		CDTIID_TYPE_INIT					BYTE(0X00)		// 初始指标

// 定义指标参数ID号CDTIPID中btType的取值
#define		CDTIPID_TYPE_CUSTOM					BYTE(0X80)		// 普通自定义指标参数，否则为系统指标参数
#define		CDTIPID_TYPE_DYNAMIC				BYTE(0X40)		// 动态指标参数，否则为静态指标参数
#define		CDTIPID_TYPE_DATAAREA				BYTE(0X20)		// 号码分区指标参数
#define		CDTIPID_TYPE_TESTNUMS				BYTE(0X10)		// 试机号相关指标参数
#define		CDTIPID_TYPE_VALID					BYTE(0X01)		// 有效指标参数，否则为无效指标参数
#define		CDTIPID_TYPE_INIT					BYTE(0X00)		// 初始指标参数

// 定义条件ID号CDTID中btType的取值
#define		CDTID_TYPE_INIT						BYTE(0)			// 初始条件（尚未设置条件）
#define		CDTID_TYPE_IID						BYTE(1)			// 只含指标的条件
#define		CDTID_TYPE_IPID						BYTE(2)			// 含有一级指标参数的条件
#define		CDTID_TYPE_IPID2					BYTE(3)			// 含有二级指标参数的条件
#define		CDTID_TYPE_COMPLEX					BYTE(0XFF)		// 复合条件
#define		CDTID_TYPE_INVALID					BYTE(0X8F)		// 无效条件

// 定义条件名称的最大长度
#define		MAXLEN_CDT_NAME					129			// 最大条件名称长度
#define		MAXLEN_CDT_SHORTNAME			100			// 最大条件简称长度

// 定义指标名称的最大长度
#define		MAXLEN_INDICATOR_NAME			65			// 最大指标名称长度
#define		MAXLEN_INDICATOR_SHORTNAME		33			// 最大指标简称长度
#define		MAXLEN_INDICATOR_DETAILS		256			// 最大指标详细说明长度

// 定义指标参数名称的最大长度
#define		MAXLEN_INDICATORPARAM_NAME		33			// 最大指标参数名称长度

#define		MAXCOUNT_VALUEITEM				10000		// 值域的最大元素个数

// 定义值域类型值
#define		CDTRT_DEFAULT				BYTE(0X00)		// 缺省值，为连续的、有上限、从小到大排序
#define		CDTRT_UNCONT				BYTE(0X01)		// 非连续的值，比如跨度为0-9连续的值
#define		CDTRT_NOLIMIT				BYTE(0X02)		// 无上限的值，则值域有一个元素保存最小值
#define		CDTRT_ORDER_DESC			BYTE(0X04)		// 从大到小排序
#define		CDTRT_ARRANGE				BYTE(0X08)		// 排列型，包括单选排列型、组选排列型、个数排列型
#define		CDTRT_NOTSUPPORT			BYTE(0X80)		// 不支持的值域（这种类型的条件只能用于缩水，暂时不用）

// 定义值类型值
#define		CDTVT_DIGITAL				BYTE(0)			// 数字型，比如：1,2,10
#define		CDTVT_DIGITAL2				BYTE(1)			// 固定2位的数字型，比如：01,10
#define		CDTVT_DIGITAL3				BYTE(2)			// 固定3位的数字型，比如：001,100
#define		CDTVT_DIGITAL4				BYTE(3)			// 固定4位的数字型，比如：0001,1000
#define		CDTVT_DIGITAL5				BYTE(4)			// 固定5位的数字型，比如：00001,10000
#define		CDTVT_DIGITAL6				BYTE(5)			// 固定6位的数字型，比如：000001,100000
#define		CDTVT_DIGITAL7				BYTE(6)			// 固定7位的数字型，比如：0000001,1000000
#define		CDTVT_DIGITAL8				BYTE(7)			// 固定8位的数字型，比如：00000001,10000000
#define		CDTVT_ARRANGE_SINGLE		BYTE(10)		// 单选排列型，比如遗漏的单选排列或者热温冷的单选排列
#define		CDTVT_ARRANGE_GROUP			BYTE(11)		// 组选排列型，比如遗漏的组选排列
#define		CDTVT_ARRANGE_COUNT_BEGIN	BYTE(20)		// 个数排列型开始（从2个排列开始），比如热温冷的个数排列，除N余数个数排列
#define		CDTVT_ARRANGE_COUNT_END		BYTE(29)		// 个数排列型结束（到10个排列结束）

// 定义输入类型值
#define		CDTIT_EDIT_SINGLELINE		BYTE(1)			// 单行编辑器，用于数字型
#define		CDTIT_EDIT_MULTILINE		BYTE(2)			// 多行编辑器，用于排列型
#define		CDTIT_CHECKBOX				BYTE(5)			// 复选框

// 定义IConditionMan::BuildConditionTree中的dwType的取值
#define		BCTT_CDT_MINE				BYTE(0X01)		// 我的条件
#define		BCTT_CDT_SYSTEM				BYTE(0X02)		// 系统条件（缺省包含扩展条件）
#define		BCTT_NAME_WHOLE				BYTE(0X10)		// 条件全称
#define		BCTT_NAME_SHORT				BYTE(0X20)		// 条件简称
#define		BCTT_INCLUDETESTNUMS		BYTE(0X80)		// 包含试机号相关条件（用于排列三等条件扩展树，只对系统条件树有效）

#define		BCTT_DEFAULT				BYTE(BCTT_CDT_SYSTEM|BCTT_NAME_WHOLE)		// 缺省值

// 定义条件数据中的标志的取值
#define		CDDF_ENABLE					BYTE(0X80)		// 启用
#define		CDDF_KEEP					BYTE(0X40)		// 保留
#define		CDDF_ALLOWERROR				BYTE(0X20)		// 容错
#define		CDDF_LOCALAE				BYTE(0X10)		// 本级容错
#define		CDDF_VALIDATEOK				BYTE(0X01)		// 验证成功
#define		CDDF_HISTORY				BYTE(0X02)		// 历史条件
#define		CDDF_DEFAULT				(CDDF_ENABLE | CDDF_KEEP | CDDF_ALLOWERROR)

// 定义号码分区指标ID包含的使用类型（UseType）的取值（0-50）
#define		CIDAUT_POS_1						(BYTE)0			// 第一位M分区
#define		CIDAUT_POS_2						(BYTE)1			// 第二位M分区
#define		CIDAUT_POS_3						(BYTE)2			// 第三位M分区
#define		CIDAUT_POS_4						(BYTE)3			// 第四位M分区
#define		CIDAUT_POS_5						(BYTE)4			// 第五位M分区
#define		CIDAUT_POS_6						(BYTE)5			// 第六位M分区
#define		CIDAUT_POS_7						(BYTE)6			// 第七位M分区
#define		CIDAUT_POS_8						(BYTE)7			// 第八位M分区
#define		CIDAUT_POS_ANY						(BYTE)8			// 任意位M分区
#define		CIDAUT_ARR_SIN						(BYTE)10		// M分区单选排列
#define		CIDAUT_ARR_SIN_VALUE				(BYTE)11		// M分区单选排列值
#define		CIDAUT_ARR_GRO						(BYTE)12		// M分区组选排列
#define		CIDAUT_ARR_GRO_VALUE				(BYTE)13		// M分区组选排列值
#define		CIDAUT_ARR_COUNT					(BYTE)14		// M分区个数排列
#define		CIDAUT_ARR_COUNT_VALUE				(BYTE)15		// M分区个数排列值
#define		CIDAUT_MAXLINK						(BYTE)20		// M分区最大邻出
#define		CIDAUT_MAXNONE						(BYTE)21		// M分区最大邻断
#define		CIDAUT_COUNT_NONE					(BYTE)22		// M分区未出个数
#define		CIDAUT_COUNT_MATCH					(BYTE)23		// M分区出现个数
#define		CIDAUT_SHAPE						(BYTE)29		// M分区形态（只用于3分区）
#define		CIDAUT_COUNT_N						(BYTE)34		// M分区第N分区个数（N：1-16），即34-49

// 定义自定义指标参数数据CUSTOMCDTDATA::btDataType的取值
#define		CCD_TYPE_DATAAREA					(BYTE)1		// 号码分区
#define		CCD_TYPE_REMAINDER					(BYTE)2		// 自定义余数
#define		CCD_TYPE_CIPDT_AREA					(BYTE)3		// 自定义分区

// 定义IConditionData中类型值
#define		CDTYPE_NONE					(BYTE)0X00		// 初始值
#define		CDTYPE_CONDITION			(BYTE)0X01		// 单个条件
#define		CDTYPE_GROUP_COMPLEX		(BYTE)0X82		// 复合条件组
#define		CDTYPE_GROUP_FIX			(BYTE)0X84		// 固定条件组
#define		CDTYPE_GROUP_MIX			(BYTE)0X88		// 混合条件组

#define		CDTYPE_MASK_GROUP			(BYTE)0X80		// 组掩码

// 定义ICondition::SetCustomData中btFlag的取值
#define		CDTCDF_CONDITION			(BYTE)0			// 条件的自定义数据，也即最终扩展条件的自定义数据
#define		CDTCDF_INDICATOR			(BYTE)1			// 指标的自定义数据
#define		CDTCDF_INDICATORPARAM		(BYTE)2			// 第一指标参数的自定义数据
#define		CDTCDF_INDICATORPARAM2		(BYTE)3			// 第二指标参数的自定义数据

// 定义ICondition::GetValue中的btType的取值
#define		CDTGVT_NORMAL				(BYTE)0			// 通常使用的值，缺省值
#define		CDTGVT_FILTER				(BYTE)1			// 用于缩水
#define		CDTGVT_VALIDATE				(BYTE)2			// 用于验证
#define		CDTGVT_HISFILTER			(BYTE)11		// 用于历史条件中的缩水
#define		CDTGVT_HISVALIDATE			(BYTE)12		// 用于历史条件中的验证

// 定义AddtoBigPPickDialog中的btFlag的取值
#define		PICKDATA_INUMBER		(BYTE)1		// INumber 接口
#define		PICKDATA_PBIGPITEM		(BYTE)2		// 大底数据结构

#pragma pack(1)
#pragma warning(push, 3)
typedef struct tagCDTIID						// 指标ID号
{
	BYTE	btType;								// 指标ID类型，见宏定义，如：CDTIID_TYPE_CUSTOM为自定义指标
	union
	{
		DWORD	dwSystemIID;					// 系统指标的ID号（系统统一定义）
		struct
		{
			DWORD	dwDiffValues[2];			// 数据区分值（使用当前系统时间和GetTickCount的值）
			WORD	wUseType;					// 使用类型，比如分区指标中的CIDAUT_POS_1
		} stCustomIID;							// 自定义指标的ID号
	};
    
	void Init()
	{
		ZeroMemory(this, sizeof(tagCDTIID));
	};
    
	// 初始化系统指标ID
	void InitSysDefIID(DWORD dwSysDefIIDIn, BOOL bIsComplex = FALSE)
	{
		Init();
		if(dwSysDefIIDIn == 0)
		{
			return ;
		}
        
		btType = CDTIID_TYPE_VALID;
		if(!bIsComplex && (dwSysDefIIDIn < 0XFFFF0000) && (dwSysDefIIDIn & 0X80000000))
		{
			btType |= CDTIID_TYPE_DYNAMIC;
			if(dwSysDefIIDIn >= (0X80000000 + 80) && dwSysDefIIDIn <= (0X80000000 + 89))	// 试机号相关指标
			{
				btType |= CDTIID_TYPE_TESTNUMS;
			}
		}
        
		dwSystemIID = dwSysDefIIDIn;
	};
    
	// 初始化号码分区指标ID
	void InitDataAreaIID(DWORD dwDiffValues[2], WORD wUseType)
	{
		Init();
		
		btType = CDTIID_TYPE_VALID | CDTIID_TYPE_CUSTOM | CDTIID_TYPE_DATAAREA;
		stCustomIID.dwDiffValues[0] = dwDiffValues[0];
		stCustomIID.dwDiffValues[1] = dwDiffValues[1];
		stCustomIID.wUseType = wUseType;
	};
    
	BOOL IsValid() const		// 是否有效
	{
		return (btType == CDTIID_TYPE_INIT) ? FALSE : TRUE;
	}
    
	// 是否是系统指标ID，如果dwIID不为0，则还需判断是否是该dwIID
	BOOL IsSystemIID(DWORD dwIID = 0)
	{
		if(btType & CDTIID_TYPE_CUSTOM)
		{
			return FALSE;	// 自定义指标
		}
        
		if(dwIID != 0 && dwIID != dwSystemIID)
		{
			return FALSE;
		}
        
		return TRUE;
	}
    
	BOOL operator==(const tagCDTIID &cdtIID) const
	{
		return !memcmp(&cdtIID, this, sizeof(tagCDTIID));
	};
	BOOL operator!=(const tagCDTIID &cdtIID) const
	{
		return memcmp(&cdtIID, this, sizeof(tagCDTIID));
	};
    
}CDTIID, *LPCDTIID;

typedef struct tagCDTIPID						// 指标参数ID号
{
	BYTE	btType;								// 指标ID类型，见宏定义，如：CDTIPID_TYPE_CUSTOM为自定义指标参数
	union
	{
		DWORD	dwSystemIPID;					// 系统指标参数的ID号（系统统一定义）
		struct
		{
			DWORD	dwDiffValues[2];			// 数据区分值（使用当前系统时间和GetTickCount的值）
			BYTE	btCustomFlag;				// 自定义数据标志
		} stCustomIPID;
	};
	
	void Init()
	{
		ZeroMemory(this, sizeof(tagCDTIPID));
	};
	
	// 初始化系统指标参数
	void InitSysDefIPID(DWORD dwSysDefIPIDIn)
	{
		Init();
		if(dwSysDefIPIDIn == 0)
		{
			return ;
		}
		btType = CDTIPID_TYPE_VALID;
		dwSystemIPID = dwSysDefIPIDIn;
		if(dwSysDefIPIDIn & 0X80000000)
		{
			btType |= CDTIPID_TYPE_DYNAMIC;
			if(dwSystemIPID >= (0X80000000+30) && dwSystemIPID <= (0X80000000+39))
			{
				btType |= CDTIPID_TYPE_TESTNUMS;
			}
		}
	};
    
	// 初始化自定义指标参数ID
	void InitCustomID(DWORD dwDiffValues[2], BYTE btCustomFlag = 0)
	{
		Init();
		
		btType = CDTIPID_TYPE_VALID | CDTIPID_TYPE_CUSTOM;
		stCustomIPID.dwDiffValues[0] = dwDiffValues[0];
		stCustomIPID.dwDiffValues[1] = dwDiffValues[1];
		stCustomIPID.btCustomFlag = btCustomFlag;
	};
    
	// 初始化号码分区指标参数ID
	void InitDataAreaID(DWORD dwDiffValues[2], BYTE btCustomFlag = 0)
	{
		Init();
		
		btType = CDTIPID_TYPE_VALID | CDTIPID_TYPE_CUSTOM | CDTIPID_TYPE_DATAAREA;
		stCustomIPID.dwDiffValues[0] = dwDiffValues[0];
		stCustomIPID.dwDiffValues[1] = dwDiffValues[1];
		stCustomIPID.btCustomFlag = btCustomFlag;
	};
    
	BOOL IsValid() const		// 是否有效
	{
		return (btType == CDTIPID_TYPE_INIT) ? FALSE : TRUE;
	}
    
	// 是否是系统指标参数ID，如果dwIPID不为MAXDWORD，则还需判断是否是该dwIPID
	BOOL IsSystemIPID(DWORD dwIPID = MAXDWORD)
	{
		if(btType & CDTIPID_TYPE_CUSTOM)
		{
			return FALSE;	// 自定义指标参数
		}
		
		if(dwIPID != MAXDWORD && dwIPID != dwSystemIPID)
		{
			return FALSE;
		}
		
		return TRUE;
	}
    
	BOOL operator==(const tagCDTIPID &cdtIPID) const
	{
		return !memcmp(&cdtIPID, this, sizeof(tagCDTIPID));
	};
	BOOL operator!=(const tagCDTIPID &cdtIPID) const
	{
		return memcmp(&cdtIPID, this, sizeof(tagCDTIPID));
	};
}CDTIPID, *LPCDTIPID;

#pragma warning(pop)

typedef struct tagCDTID							// 条件ID
{
	BYTE		btType;							// 类型，见宏定义，如：CDTID_TYPE_COMPLEX
	CDTIID		cdtIID;							// 指标ID
	CDTIPID		cdtIPID;						// 指标参数ID
	CDTIPID		cdtIPID2;						// 指标参数2ID
    
	void Init()
	{
		ZeroMemory(this, sizeof(tagCDTID));
	};
    
	// 初始化复合条件ID
	void InitComplexID(DWORD dwCompexID)
	{
		ASSERT(dwCompexID != 0);
		Init();
        
		cdtIID.InitSysDefIID(dwCompexID, TRUE);
		btType = CDTID_TYPE_COMPLEX;
	}
    
	// 初始化系统条件ID
	void InitSystemID(DWORD dwSysIID, DWORD dwSysIPID = 0, DWORD dwSysIPID2 = 0)
	{
		ASSERT(dwSysIID != 0);
		Init();
        
		cdtIID.InitSysDefIID(dwSysIID);
		btType = CDTID_TYPE_IID;
        
		if(dwSysIPID == 0)
		{
			return ;
		}
		cdtIPID.InitSysDefIPID(dwSysIPID);
		btType = CDTID_TYPE_IPID;
        
		if(dwSysIPID2 == 0)
		{
			return ;
		}
		cdtIPID2.InitSysDefIPID(dwSysIPID2);
		btType = CDTID_TYPE_IPID2;
	};
    
	void InitID(const CDTIID &cdtIIDIn, const CDTIPID &cdtIPIDIn, const CDTIPID &cdtIPID2In)
	{
		ASSERT(cdtIIDIn.btType & CDTIID_TYPE_VALID);
		Init();
        
		cdtIID = cdtIIDIn;
		btType = CDTID_TYPE_IID;
		if(!(cdtIPIDIn.btType & CDTIPID_TYPE_VALID))
		{
			return ;
		}
		cdtIPID = cdtIPIDIn;
		btType = CDTID_TYPE_IPID;
        
		if(!(cdtIPID2In.btType & CDTIPID_TYPE_VALID))
		{
			return ;
		}
		cdtIPID2 = cdtIPID2In;
		btType = CDTID_TYPE_IPID2;
	};
    
	void InitID(const CDTIID &cdtIIDIn, const CDTIPID &cdtIPIDIn)
	{
		ASSERT(cdtIIDIn.btType & CDTIID_TYPE_VALID);
		Init();
		
		cdtIID = cdtIIDIn;
		btType = CDTID_TYPE_IID;
		if(!(cdtIPIDIn.btType & CDTIPID_TYPE_VALID))
		{
			return ;
		}
		cdtIPID = cdtIPIDIn;
		btType = CDTID_TYPE_IPID;
	};
    
	void InitID(const CDTIID &cdtIIDIn)
	{
		ASSERT(cdtIIDIn.btType & CDTIID_TYPE_VALID);
		Init();
		
		cdtIID = cdtIIDIn;
		btType = CDTID_TYPE_IID;
	};
    
	void InitOldVerID(DWORD dwID)		// 从旧版本ID号进行初始化
	{
		WORD wOldIID = HIWORD(dwID);
		BYTE btOldIPID = HIBYTE(LOWORD(dwID)), btOldIPID2 = LOBYTE(LOWORD(dwID));
		DWORD dwIID = wOldIID & WORD(~0X4000);
        
		if(dwIID == 0)	// 复合条件
		{
			InitComplexID(dwID);
			return ;
		}
        
		CDTIID cdtIID;
		if(dwIID >= 12000 && dwIID <= 13999)	// 分区指标
		{
			DWORD dwDiffValue[2] = {MAXDWORD, (dwIID - 12000) / 100 + 2};
			WORD wUseType = WORD(((dwIID - 12000) % 100) - 1);
			if(wUseType >= 69)
				wUseType = WORD(wUseType - 69 + 34);
			else if(wUseType > 8)
			{
				switch(wUseType + 1)
				{
                    case 20: wUseType = 10; break;
                    case 21: wUseType = 11; break;
                    case 22: wUseType = 12; break;
                    case 23: wUseType = 13; break;
                    case 25: wUseType = 14; break;
                    case 26: wUseType = 15; break;
                    case 30: wUseType = 20; break;
                    case 31: wUseType = 21; break;
                    case 40: wUseType = 22; break;
                    case 69: wUseType = 29; break;
				}
			}
			cdtIID.InitDataAreaIID(dwDiffValue, wUseType);
		}
		else
		{
			if(wOldIID & 0X4000)
				dwIID |= 0X80000000;		// 动态指标转换
			cdtIID.InitSysDefIID(dwIID);
		}
        
		CDTIPID cdtIPID, cdtIPID2;
		DWORD dwIPID = btOldIPID & BYTE(~0X40);
		if(btOldIPID & 0X40)
			dwIPID |= 0X80000000;		// 动态指标参数转换
		cdtIPID.InitSysDefIPID(dwIPID);
        
		DWORD dwIPID2 = btOldIPID2 & BYTE(~0X40);
		if(btOldIPID2 & 0X40)
			dwIPID2 |= 0X80000000;		// 动态指标参数转换
		cdtIPID2.InitSysDefIPID(dwIPID2);
        
		InitID(cdtIID, cdtIPID, cdtIPID2);
	}
    
	BOOL IsValid() const		// 是否有效
	{
		return (btType == CDTID_TYPE_INIT) ? FALSE : TRUE;
	}
	BOOL IsComplex(DWORD dwComplexID = 0) const		// 是否是复合条件，如果dwComplexID不为零，还需要与之匹配
	{
		if(btType != CDTID_TYPE_COMPLEX)
		{
			return FALSE;
		}
        
		if(dwComplexID != 0 && dwComplexID != cdtIID.dwSystemIID)
		{
			return FALSE;
		}
        
		return TRUE;
	}
	DWORD GetComplexID() const
	{
		ASSERT(IsComplex());
		return cdtIID.dwSystemIID;
	}
	BOOL HasFirstIP() const		// 是否有第一指标参数
	{
		return (btType == CDTID_TYPE_IPID || btType == CDTID_TYPE_IPID2) ? TRUE : FALSE;
	}
	BOOL HasSecondIP() const	// 是否有第二指标参数
	{
		return (btType == CDTID_TYPE_IPID2) ? TRUE : FALSE;
	}
    
	// 查询当前ID是否是cdtSrcID的扩展条件，bIncludeSameID为TRUE时则表示如果两个ID相同，也算是扩展条件
	BOOL IsExtendOf(const tagCDTID &cdtSrcID, BOOL bIncludeSameID = FALSE) const
	{
		if(cdtSrcID.cdtIID != cdtIID)
		{
			return FALSE;		// 指标不同，则肯定不是
		}
		if(cdtSrcID.cdtIPID.btType == CDTIPID_TYPE_INIT)	// 待比较的ID只有指标
		{
			if(cdtIPID2.btType != CDTIPID_TYPE_INIT)
			{
				return FALSE;	// 如果当前条件含有第2指标参数，则返回否
			}
			if(cdtIPID.btType == CDTIPID_TYPE_INIT && !bIncludeSameID)
			{
				return FALSE;	// 条件相同并且不允许相同，则返回否
			}
		}
		else if(cdtSrcID.cdtIPID != cdtIPID)
		{
			return FALSE;		// 第一指标参数不同，则肯定不是
		}
		else	// 第一指标参数相同
		{
			if(cdtSrcID.cdtIPID2.btType == CDTIPID_TYPE_INIT)	// 待比较的ID只有第一指标参数
			{
				if(cdtIPID2.btType == CDTIPID_TYPE_INIT && !bIncludeSameID)
				{
					return FALSE;	// 条件相同并且不允许相同，则返回否
				}
			}
			else	// 待比较的ID有第二指标参数
			{
				if(!bIncludeSameID || cdtSrcID.cdtIPID2 != cdtIPID2)
				{
					return FALSE;
				}
			}
		}
        
		return TRUE;
	}
    
	// 是否是系统ID，各参数不为缺省值时则还需要判断对应的ID值是否相等
	BOOL IsSystemID(DWORD dwIID = 0, DWORD dwIPID = MAXDWORD, DWORD dwIPID2 = MAXDWORD)
	{
		if(!cdtIID.IsSystemIID(dwIID))
		{
			return FALSE;
		}
        
		if(!cdtIPID.IsSystemIPID(dwIPID))
		{
			return FALSE;
		}
        
		if(!cdtIPID2.IsSystemIPID(dwIPID2))
		{
			return FALSE;
		}
        
		return TRUE;
	}
    
	BOOL operator==(const tagCDTID &cdtSrcID) const
	{
		return !memcmp(&cdtSrcID, this, sizeof(tagCDTID));
	};
	BOOL operator!=(const tagCDTID &cdtSrcID) const
	{
		return memcmp(&cdtSrcID, this, sizeof(tagCDTID));
	};
	BOOL operator<(const tagCDTID &cdtSrcID) const
	{
		return memcmp(this, &cdtSrcID, sizeof(tagCDTID)) < 0 ? TRUE : FALSE;
	};
	BOOL operator<=(const tagCDTID &cdtSrcID) const
	{
		return memcmp(this, &cdtSrcID, sizeof(tagCDTID)) <= 0 ? TRUE : FALSE;
	};
	BOOL operator>(const tagCDTID &cdtSrcID) const
	{
		return memcmp(this, &cdtSrcID, sizeof(tagCDTID)) > 0 ? TRUE : FALSE;
	};
	BOOL operator>=(const tagCDTID &cdtSrcID) const
	{
		return memcmp(this, &cdtSrcID, sizeof(tagCDTID)) >= 0 ? TRUE : FALSE;
	};
}CDTID, *LPCDTID;

typedef struct tagCDTVALUERANGEITEM				// 条件值域取值元素
{
	int		nValue;								// 值
	char	szValueName[33];					// 值对应的名字
	char	szColName[4];						// 走势图中列名
	char	szBallName[4];						// 走势图中球名
}CDTVALUERANGEITEM, *LPCDTVALUERANGEITEM;

typedef struct tagCDTVALUERANGE					// 条件值域
{
	int				nItemCount;					// 值总数
	int				nMinValue;					// 最小值
	int				nMaxValue;					// 最大值
	CDTVALUERANGEITEM	stValueItem[1];			// 取值信息集合
} CDTVALUERANGE, *LPCDTVALUERANGE;

typedef struct tagCDTVALUEITEM					// 条件值单元
{
	int				nValue;						// 条件值
	BYTE			btCount;					// 条件值对应的个数，即该值出现次数，为1-255
}CDTVALUEITEM, *LPCDTVALUEITEM;

typedef struct tagCDTVALUE						// 条件值信息
{
	BYTE			btItemCount;				// 条件值单元个数
	CDTVALUEITEM	stValueItems[1];			// 条件值单元数组，
}CDTVALUE, *LPCDTVALUE;

typedef struct tagCUSTOMCDTDATA					// 自定义条件数据
{
	BYTE	btDataType;							// 数据类型，见宏定义，如：CCD_TYPE_DATAAREA
	DWORD	dwDiffValues[2];					// 条件ID
	WORD	wDataLen;							// 数据长度
	BYTE	btDataBufBegin[1];					// 数据缓冲区（保存对应的数据类型的数据结构）
}CUSTOMCDTDATA, *LPCUSTOMCDTDATA;

typedef struct tagDATAAREA						// 分区设置
{
	DWORD	dwAreaDiffValues[2];				// 分区区分数据（对应于自定义指标ID和自定义指标的区分数据）
	BYTE	btAreaCount;						// 分区个数，目前最大支持50个分区
	BYTE	btHide;								// 是否隐藏，为0则是显示
	char	szName[33];							// 名称
	char	szShortName[17];					// 简称
	char	szSubAreaNames[50][17];				// 子分区名称
	BYTE	btSubAreaMinValue;					// 子分区的最小值，即决定分区的起始值，比如系统缺省为1，可以设为0以方便一些自定义使用
	BYTE	btSubAreaValues[50];				// 子分区数据集合（保存下标值指定的号码所在的子分区值，btSubAreaMinValue+n表示第n分区）
	
	BOOL IsSameID(tagDATAAREA *lpSrcDataArea) const
	{
		ASSERT(lpSrcDataArea != NULL);
		return ((dwAreaDiffValues[0] == lpSrcDataArea->dwAreaDiffValues[0]) && dwAreaDiffValues[1] == lpSrcDataArea->dwAreaDiffValues[1]) ? TRUE : FALSE;
	};
    
	void operator=(const tagDATAAREA &stSrcDataArea)
	{
		memcpy(this, &stSrcDataArea, sizeof(tagDATAAREA));
	};
    
	BOOL operator==(const tagDATAAREA &stSrcDataArea) const
	{
		return !memcmp(&stSrcDataArea, this, sizeof(tagDATAAREA));
	};
    
}DATAAREA, *LPDATAAREA;

#pragma pack()

////////////////////////////////////////////////////////////////////////////////
// 以下定义条件接口
// 该接口类不能用外部直接创建和销毁，所以不声明接口ID，由IConditionMan管理

class ICondition
{
public:		// 接口函数
    
	// 获取条件ID号
	virtual	CDTID& GetID() = 0;
	// 获取条件中的指标ID号
	virtual CDTIID& GetIndicatorID() = 0;
	// 获取条件中的指标参数ID号，bFirst为TRUE则是第一个参数否则为第二个参数
	virtual CDTIPID& GetIndicatorParamID(BOOL bFirst = TRUE) = 0;
    
	// 获取条件的父条件，如果返回NULL则是指标条件
	virtual ICondition * GetIConditionParent() = 0;
    
	// 获取条件名称，bGetShortName为TRUE时获取条件简称
	virtual LPCSTR GetName(BOOL bGetShortName = FALSE) = 0;
    
	// 获取条件值域
	virtual LPCDTVALUERANGE GetValueRange() = 0;
    
	// 查找指定值在值域中的索引，返回-1表示没有找到
	virtual int GetIndexOfValueRange(int nValue) = 0;
	// 查找指定值在值域中的数据单元指针，返回NULL表示没有找到
	virtual LPCDTVALUERANGEITEM GetItemOfValueRange(int nValue) = 0;
    
	// 获取值域范围类型，返回值见宏定义，如：CDTRT_DEFAULT
	virtual BYTE GetValueRangeType() = 0;
	// 获取值类型，返回值见宏定义，如：CDTVT_DIGITAL
	virtual BYTE GetValueType() = 0;
	// 获取输入类型，返回值见宏定义，如：CDTIT_CHECKBOX
	virtual BYTE GetInputType() = 0;
    
	// 是否是复合条件
	virtual BOOL IsComplex() = 0;
    
	// 是否是多值条件，pMaxCountOfValue：返回值个数的最大值，lpHasSameValue：返回条件值是否可含有相同值
	virtual BOOL IsMultipleValue(int *pMaxCountOfValue = NULL, LPBOOL lpHasSameValue = NULL) = 0;
    
	// 是否是动态条件
	virtual BOOL IsDynamic() = 0;
    
	// 是否是试机号相关条件
	virtual BOOL IsIncludeTestNums() = 0;
    
	// 是否是免费版可使用的条件
	virtual BOOL IsFree() = 0;
    
	// 获取指定号码数据的条件值，如果是多值条件，则返回计算后的值（比如多值为2、0、3，则返回203）
	// 如果dwIssueIndex == DATA_INDEX_INVALID，则lpFltNumsData不能为NULL，此时获取当期缩水号码数据的条件值
	// 否则，如果lpFltNumsData为NULL，则获取dwIssueIndex指定期号的历史数据的条件值，
	// 否则获取dwIssueIndex指定期号的缩水数据lpFltNumsData的条件值
	// bType类型，见宏定义
	virtual int GetValue(LPBYTE lpFltNumsData, DWORD dwIssueIndex = DATA_INDEX_INVALID, BYTE btType = CDTGVT_NORMAL) = 0;
    
	// 获取当期号码数据的条件值（扩展方法），参数说明同上
	virtual LPCDTVALUE GetValueEx(LPBYTE lpFltNumsData, DWORD dwIssueIndex = DATA_INDEX_INVALID, BYTE btType = CDTGVT_NORMAL) = 0;
    
	// 获取当期号码数据的条件值文本串，bUseValueName为TRUE则使用值名，否则使用列名
	// lpFltNumsData和dwIssueIndex的说明同上
	virtual LPCSTR GetValueString(LPBYTE lpFltNumsData, DWORD dwIssueIndex = DATA_INDEX_INVALID, BOOL bUseValueName = TRUE) = 0;
    
	// 条件值转换为文本串，bUseValueName为TRUE则使用值名，否则使用列名
	virtual LPCSTR ValueToString(int nValue, BOOL bUseValueName = TRUE) = 0;
	virtual LPCSTR ValueToString(CDWordArray &cValueArray, BOOL bUseValueName = TRUE) = 0;
	virtual LPCSTR ValueToString(LPINT lpValues, int nValueCount, BOOL bUseValueName = TRUE) = 0;
    
	// 条件值名是否是数字型的，比如和值是数字型的，而类型等是文本型的
	virtual BOOL IsDigitalValueName() = 0;
	
	// 获取值说明信息，nLeftSpaceCount：为前置空格个数，nSepSpaceCount为值说明间的空格个数
//	virtual BOOL GetValueExplain(CQuickAddString &strExplain, int nLeftSpaceCount = 0, int nSepSpaceCount = 2, int nMaxCol = 120) = 0;
    
	// 是否支持图表
	virtual BOOL IsSupportGraph() = 0;
    
public:	// 自定义条件相关接口
	
	// 是否是自定义条件
	virtual BOOL IsCustom() = 0;
    
	// 设置和获取自定义数据
	virtual void SetCustomData(DWORD dwCustomData, BYTE btFlag = CDTCDF_CONDITION) = 0;
	virtual DWORD GetCustomData(BYTE btFlag = CDTCDF_CONDITION) = 0;
};

////////////////////////////////////////////////////////////////////////////////
// 以下定义条件数据接口
class IConditionData
{
public:		// 接口函数，整体
    
	// 创建顶级组
	virtual BOOL CreateRootGroup() = 0;
    
	// 销毁
	virtual void Destroy() = 0;
    
	// 是否是条件组，FALSE则为单个条件数据
	virtual BOOL IsGroup() = 0;
    
	// 是否是顶级组
	virtual BOOL IsRootGroup() = 0;
    
	// 获取类型，返回值见宏定义，比如：CDTYPE_GROUP_MIX
	virtual BYTE GetType() = 0;
    
	// 获取ICondition接口指针，如果是混合条件组和固定条件组，则返回NULL
	virtual ICondition *GetICondition() = 0;
	// 获取条件ID，如果是条件组，则返回无效的ID
	virtual const CDTID &GetConditionID() = 0;
	// 获取条件组ID，如果是条件，则返回0
	virtual DWORD GetConditionGroupID() = 0;
	
	// 获取数据文本串，返回的文本串临时有效，bForExport为TRUE时则用于导出文本（此时需要完整的文本，主要对于复合条件）
	virtual LPCSTR GetDataString(BOOL bForExport = FALSE) = 0;
	
	// 获取导出的文本串（即该条件设置的详细文本描述），bFormat：为TRUE时自动进行格式化（增加回车和空格缩进等），否则仅以一个空格分隔条件
	virtual CQuickAddString GetExportString(BOOL bFormat = TRUE) = 0;
    
	// 从文件获取数据，返回FALSE时错误文本串可由GetLastErrorString获取
	// 如果pINumbers不为NULL，则需要将缩水号码覆盖到pINumbers，bAppend是否是添加，否则为覆盖
	virtual BOOL LoadData(LPCSTR lpszFilename, BOOL bAppend = FALSE, INumbers *pINumbers = NULL) = 0;
    
	// 保存数据到文件，返回FALSE时错误文本串可由GetLastErrorString获取
	// 如果pINumbers不为NULL，则需要从pINumbers获取缩水号码并保存
	virtual BOOL SaveData(LPCSTR lpszFilename, INumbers *pINumbers = NULL) = 0;
    
	// 导出数据到文本文件，返回FALSE时错误文本串可由GetLastErrorString获取
	virtual BOOL ExportTextFile(LPCSTR lpszFilename) = 0;
	
	// 可回退的更新
	virtual BOOL BeginMoidify() = 0;					// 开始更新，此前为更新前状态
	virtual BOOL EndModify(BOOL bCommit = TRUE) = 0;	// bCommit为TRUE时，表示确认更新，否则，回退到更新前状态
    
	// 获取最后错误的文本串，用于指定的函数返回错误时，返回值临时有效
	virtual LPCSTR GetLastErrorString() = 0;
    
public:		// 单个条件和条件组公用函数
	
	// 设置数据名，pGKID：胆杀指标ID，如果不为NULL，则表示用于公式缩水数据，则lpszName为公式表达式
	virtual void SetName(LPCSTR lpszName, void *pGKID = NULL) = 0;
	// 获取数据名，如果为公式缩水数据，则返回指标名称
	virtual LPCSTR GetName() = 0;
	// 获取公式表达式，如果不是公式缩水数据则返回数据名
	virtual LPCSTR GetFormulaExpression() = 0;
	// 获取胆杀指标ID指针，如果不是公式缩水数据则返回NULL
	virtual void *GetGallKillID() = 0;
	
	// 设置标志值
	virtual void SetFlag(BYTE btFlag) = 0;
	// 获取标志值
	virtual BYTE GetFlag() = 0;
    
	// 设置是否启用
	virtual void SetEnable(BOOL bEnable = TRUE) = 0;
	// 查询是否启用
	virtual BOOL IsEnable() = 0;
	
	// 设置是否保留
	virtual void SetKeep(BOOL bKeep = TRUE) = 0;
	// 查询是否保留
	virtual BOOL IsKeep() = 0;
	
	// 设置是否容错
	virtual void SetAllowError(BOOL bAllowError = TRUE) = 0;
	// 查询是否容错
	virtual BOOL IsAllowError() = 0;
	
	// 设置是否验证成功
	virtual void SetValidateOK(BOOL bOK = TRUE) = 0;
	// 查询是否验证成功
	virtual BOOL IsValidateOK() = 0;
    
public:		// 单个条件数据特有的相关函数
    
	// 连接数据，直接使用lpDataBuf缓冲区
	virtual BOOL AttachData(LPBYTE lpDataBuf, int nBufLen) = 0;
	// 去除现有的数据缓冲区
	virtual LPBYTE DetachData(int &nDataLenOut) = 0;
    
	// 复制对象
	virtual void Copy(IConditionData *pISrcConditionData) = 0;
	
	// 比较对象，btFlag确定其他需要比较的标志信息，如果为0则只比较数据缓冲区
	virtual BOOL Compare(IConditionData *pISrcConditionData, BYTE btFlag = CDDF_DEFAULT) = 0;
    
	// 设置数据缓冲区，系统自动复制一个相同的数据缓冲区，bAppend为TRUE时则是追加数据，否则为覆盖
	virtual void SetDataBuffer(LPBYTE lpDataBuf, int nBufLen, BOOL bAppend = FALSE) = 0;
	// 获取数据缓冲区，不能修改
	virtual LPBYTE GetDataBuffer(int *pDataLen) = 0;
    
	// 设置数据信息，如果返回FALSE，则错误文本串可由GetLastErrorString获取，bAppend为TRUE时则是追加数据，否则为覆盖
	virtual BOOL SetData(int nValue, BOOL bAppend = FALSE) = 0;
	virtual BOOL SetData(CDWordArray &cValueArray, BOOL bAppend = FALSE) = 0;
	virtual BOOL SetData(CQuickAddString &strDataText) = 0;
    
	// 设置任意码条件的数据信息，pMatchCountArray：出现个数集合，如果为NULL则表示出现1-最大号码个数，
	// bRepeatToOne：为TRUE则重复个数算一个，否则算多个
	virtual BOOL SetDataAny(LPCSTR lpszDataText, CDWordArray *pMatchCountArray = NULL, BOOL bRepeatToOne = FALSE) = 0;
	// 获取任意码条件的数据信息，返回任意码文本
	virtual LPCSTR GetDataAny(CDWordArray &dwMatchCountArray, BOOL &bRepeatToOne) = 0;
    
	// 设置历史数据信息，如果返回FALSE，则错误文本串可由GetLastErrorString获取
	virtual BOOL SetHistoryData(CDWordArray &cValueArray) = 0;
	// 获取历史数据信息
	virtual LPDWORD GetHistoryData(DWORD &dwCount) = 0;
    
	// 数据中是否包含该值，对于缓冲区的数据无效
	virtual BOOL IsValueInData(int nValue) = 0;
    
	// 设置是否是历史条件
	virtual void SetHistory(BOOL bHistory = TRUE) = 0;
	// 查询是否是历史条件
	virtual BOOL IsHistory() = 0;
    
public:		// 条件组数据特有的相关函数
    
	// 创建指定类型的IConditionData对象（用于单个条件）
	virtual IConditionData * CreateIConditionData_Condition(ICondition *pICondition) = 0;
	// 创建指定类型的IConditionData对象（用于复合条件组）
	virtual IConditionData * CreateIConditionData_Group_Complex(ICondition *pICondition) = 0;
	// 创建指定类型的IConditionData对象（用于固定条件组）
	virtual IConditionData * CreateIConditionData_Group_Fix(DWORD dwFixGroupID) = 0;
	// 创建指定类型的IConditionData对象（用于混合条件组）
	virtual IConditionData * CreateIConditionData_Group_Mix() = 0;
	
	// 删除IConditionData对象
	virtual void DeleteIConditionData(IConditionData *pIConditionData) = 0;
    
	// 设置单元数据总数，bAutoKeepOldItems：为TRUE时则自动保留原来的数据单元（如果当前设置的总数比原来的少，则自动丢弃后面的），否则全部清除原来的数据单元
	virtual BOOL SetItemCount(int nCount, BOOL bAutoKeepOldItems = FALSE) = 0;
	// 获取单元数据总数
	virtual int GetItemCount() = 0;
    
	// 设置指定索引的单元数据
	virtual BOOL SetItem(int nIndex, IConditionData *pIConditionData) = 0;
	// 获取指定索引的单元数据
	virtual IConditionData * GetItem(int nIndex) = 0;
    
	// 移除所有单元数据，如果bDeleteAllItems为TRUE，则删除所有单元指针对象，否则仅仅清空保存指针占用的空间
	virtual void RemoveAllItems(BOOL bDeleteAllItems = FALSE) = 0;
    
	// 设置本级容错，bEnable：是否启用，lpszAEString：容错字符串，如果为NULL，则忽略（即不改变当前设置的字符串）
	// 返回FALSE时错误文本串可由GetLastErrorString获取
	virtual BOOL SetLocalAE(BOOL bEnable = TRUE, LPCSTR lpszAEString = NULL) = 0;
	// 查询是否启用本级容错
	virtual BOOL IsEnableLocalAE() = 0;	
	// 获取本级容错文本串，返回值临时有效
	virtual LPCSTR GetLocalAEString() = 0;
    
public:		// 缩水和验证相关函数
    
	// 检查条件数据是否有效，bAllowError：是否启用容错，返回FALSE时错误文本串可由GetLastErrorString获取
	virtual BOOL CheckValid(BOOL bAllowError = TRUE) = 0;
    
	// 准备进行缩水，bAllowError：是否启用容错，返回FALSE时错误文本串可由GetLastErrorString获取
	virtual BOOL PrepareFilter(BOOL bAllowError = TRUE) = 0;
	
	// 进行缩水，为TRUE表示号码保留，否则被缩掉
	virtual BOOL Filter(LPBYTE lpNumsData) = 0;
	
	// 完成缩水
	virtual void FinishFilter() = 0;
	
	// 准备进行验证，bAllowError：是否启用容错，返回当前参与验证的条件总数，为0时错误文本串可由GetLastErrorString获取
	virtual DWORD PrepareValidate(BOOL bAllowError = TRUE) = 0;
	
	// 进行验证，返回错误的当前参与验证的条件总数，为0则是验证通过
	virtual DWORD Validate(DWORD dwIssueIndex) = 0;
	
	// 完成验证
	virtual void FinishValidate() = 0;
    
public:	// 条件数据复制和粘贴，只用于顶层条件数据组
    
	// 设置需要复制的条件数据个数
	virtual void SetCopyItemCount(int nCount) = 0;
	// 设置指定索引的需要复制的条件数据指针
	virtual void SetCopyItem(int nIndex, IConditionData *pICdtDataCopy) = 0;
    
	// 获取需要复制的条件数据个数，如果为0则表示没有需要复制的条件数据
	virtual int GetCopyItemCount() = 0;
	// 获取指定索引的需要复制的条件数据指针（已经新建在内存中，直接使用）
	virtual IConditionData * GetCopyItem(int nIndex) = 0;
};

////////////////////////////////////////////////////////////////////////////////
// 以下定义条件管理接口

// 定义相关宏
typedef struct tagFIXCONDITIONGROUP *LPFIXCONDITIONGROUP;

class IConditionMan
{
public:	// 单个条件接口函数
    
	// 获取指定的ID号的条件接口对象
	virtual ICondition * GetICondition(const CDTID &cdtID) = 0;
	
	// 获取指定的ID号的条件接口对象（仅用于系统条件）
	virtual ICondition * GetICondition_System(DWORD dwSysDefIID, DWORD dwSysDefIPID = 0, DWORD dwSysDefIPID2 = 0) = 0;
    
	// 获取指定的ID号的条件的父级条件ID号，返回值临时有效
	virtual CDTID GetParentConditionID(const CDTID &cdtID) = 0;
    
	// 创建和释放临时条件接口对象（用于条件扩展功能）
	virtual ICondition * CreateTempICondition(const CDTID &cdtID, DWORD dwCustomData = 0, ICondition *pIParentCdt = NULL) = 0;
	virtual void DeleteTempICondition(ICondition *pITempCondition) = 0;
    
	// 获取指定条件名称，bGetShortName为TRUE时获取条件简称，如果是无效ID，则返回NULL，返回值临时有效
	virtual LPCSTR GetConditionName(const CDTID &cdtID, BOOL bGetShortName = FALSE) = 0;
    
	// 获取指定指标名称，bGetShortName为TRUE时获取条件简称，如果是无效ID，则返回NULL，返回值临时有效
	virtual LPCSTR GetIndicatorName(const CDTIID &cdtIID, BOOL bGetShortName = FALSE, DWORD dwCustomData = MAXDWORD) = 0;
    
	// 获取指定指标参数（不包括自定义指标参数）名称，如果是无效ID，则返回NULL，返回值临时有效
	virtual LPCSTR GetIndicatorParamName(const CDTIPID &cdtIPID, DWORD dwCustomData = MAXDWORD) = 0;
	
	// 查询条件是否有效
	virtual BOOL IsValidCondition(const CDTID &cdtID) = 0;
	
	// 查询条件是否是试机号相关条件
	virtual BOOL IsTestNumsCondition(const CDTID &cdtID, DWORD dwCustomData = MAXDWORD) = 0;
	
	// 查询条件是否是多值条件，pMaxCountOfValue：返回值个数的最大值，lpHasSameValue：返回条件值是否可含有相同值
	virtual BOOL IsMultipleValueCondition(const CDTID &cdtID, int *pMaxCountOfValue = NULL, LPBOOL lpHasSameValue = NULL) = 0;
	
	// 查询条件是否是动态条件
	virtual BOOL IsDynamicCondition(const CDTID &cdtID, DWORD dwCustomData = MAXDWORD) = 0;
    
	// 查询条件是否是免费版可用的条件
	virtual BOOL IsFreeCondition(const CDTID &cdtID) = 0;
    
	// 获取指定条件值域范围类型，返回值见宏定义，如：CDTRT_DEFAULT
	virtual BYTE GetConditionValueRangeType(const CDTID &cdtID) = 0;
	// 获取指定条件值类型，返回值见宏定义，如：CDTVT_DIGITAL
	virtual BYTE GetConditionValueType(const CDTID &cdtID) = 0;
	// 获取指定条件输入类型，返回值见宏定义，如：CDTIT_CHECKBOX
	virtual BYTE GetConditionInputType(const CDTID &cdtID) = 0;
    
	// 获取指定条件的扩展条件ID集合，如果lpCdtID为NULL，则获取所有扩展条件
	// 返回扩展条件个数，返回0表示当前没有扩展条件，返回-1表示当前条件已经不能扩展了
// TEST	virtual int GetExtendConditions(LPCDTID lpCdtID, CArray<CDTID, CDTID &> *pExtIDArray = NULL) = 0;
    
	// 获取指标说明，如果是无效ID，则返回NULL，返回值临时有效
	virtual LPCSTR GetIndicatorExplain(const CDTIID &cdtIID) = 0;
	// 获取指标参数说明，如果是无效ID，则返回NULL，返回值临时有效
	virtual LPCSTR GetIndicatorParamExplain(const CDTIPID &cdtIPID) = 0;
    
	// 获取指标分类ID，如果该指标没有分类，则返回0
	virtual WORD GetIndicatorClassID(const CDTIID &cdtIID) = 0;
	// 获取指标分类说明，如果是无效ID，则返回NULL，返回值临时有效
	virtual LPCSTR GetIndicatorClassExplain(WORD wICID) = 0;
    
public:	// 固定条件组（含我的条件）接口函数
    
	// 获取指定的固定条件组信息
	virtual LPFIXCONDITIONGROUP GetFixConditionGroup(DWORD dwGroupID) = 0;
	// 根据dwGroupID替换FIXCONDITIONGROUP中的名称，仅用于FIXCONDITIONGROUP::GetName中调用。返回TRUE表示被替换
	virtual BOOL ReplaceFixConditionGroupName(DWORD dwGroupID, LPSTR lpszGroupName) = 0;
    
public:	// 通知接口函数
    
	// 当数据类型改变前
	virtual void OnPreDataTypeChange() = 0;
    
	// 当数据类型改变后
	virtual void OnDataTypeChanged() = 0;
	
	// 当配置改变时
	virtual void OnConfigChanged(DWORD dwConfigID) = 0;
    
public:	// 分区相关接口函数
    
	// 获取系统分区中指定子分区数据，返回的低字中保存该分区的最小值，高字中保存该分区的最大值，返回MAXDWORD的则是错误
	// dwAreaIndex：为分区索引，0为第一分区，nMinValue为本分区的最小值，nMaxValue为最大值，以下同
	virtual DWORD GetSystemSubAreaData(DWORD dwAreaCount, DWORD dwAreaIndex, int nMinValue, int nMaxValue) = 0;
	
	// 获取值所在系统分区的子分区索引，0为第一分区，依次类推，小于则是错误
	virtual int GetSystemSubAreaIndex(DWORD dwAreaCount, int nValue, int nMinValue, int nMaxValue) = 0;
    
	// 获取指定号码分区信息串（临时有效），bNoSubName为TRUE时不包含子分区名称，lpszSep为分隔符
	virtual LPCSTR GetDataAreaRemarks(LPDATAAREA lpDataArea, BOOL bNoSubName = FALSE, LPCSTR lpszSep = "/") = 0;
    
	// 获取指定条件ID号的号码分区数据指针
	virtual LPDATAAREA GetDataArea(const CDTID &cdtID) = 0;
    
	// 根据号码分区区分值获取号码分区数据指针
	virtual LPDATAAREA GetDataArea(DWORD dwAreaDiffValues[2]) = 0;
    
	// 查询指定的号码分区信息是否存在
	virtual BOOL IsDataAreaExist(LPDATAAREA lpDataArea) = 0;
    
	// 添加号码分区数据，bModifyIfExist为TRUE时则表示如果存在则用lpDataArea的数据覆盖当前存在的数据，否则不进行处理
	// bToSystem为TURE时则添加到系统中，否则临时保存
	// 返回TRUE则表示添加或者修改（当bModifyIfExist为TRUE时有效）成功
	virtual BOOL AddDataArea(LPDATAAREA lpDataArea, BOOL bModifyIfExist = FALSE, BOOL bToSystem = TRUE) = 0;
    
public:		// 自定义条件相关接口
	
	// 查询指定条件是否是自定义条件
	virtual BOOL IsCustomCondition(const CDTID &cdtID) = 0;
    
	// 查询指定自定义指标是否存在
	virtual BOOL IsCustomIIDExist(const CDTIID &cdtIID) = 0;
	
	// 添加自定义指标，bModifyIfExist为TRUE时则表示如果存在则用dwCustomData的数据覆盖当前存在的自定义数据，否则不进行处理
	// bToSystem为TURE时则添加到系统中，否则临时保存
	// 返回TRUE则表示添加或者修改（当bModifyIfExist为TRUE时有效）成功
	virtual BOOL AddCustomID(DWORD dwCustomData, BOOL bModifyIfExist = FALSE, BOOL bToSystem = TRUE) = 0;
    
	// 查询指定自定义指标参数是否存在
	virtual BOOL IsCustomIPIDExist(const CDTIPID &cdtIPID) = 0;
    
	// 添加自定义指标参数，bModifyIfExist为TRUE时则表示如果存在则用dwCustomData的数据覆盖当前存在的自定义数据，否则不进行处理
	// bToSystem为TURE时则添加到系统中，否则临时保存
	// 返回TRUE则表示添加或者修改（当bModifyIfExist为TRUE时有效）成功
	virtual BOOL AddCustomIP(DWORD dwCustomData, BOOL bModifyIfExist = FALSE, BOOL bToSystem = TRUE) = 0;
    
public:	// 数据变换相关接口函数
	
	// 准备获取原始条件时（用于非原始的数据变换），调用这个函数以后，获取的条件都是原始数据变换的条件
	virtual void PrepareGetOrgDataTransConditions() = 0;
	// 完成获取原始条件时（用于非原始的数据变换），调用这个函数以后，获取的条件都是当前数据变换的条件
	virtual void FinishGetOrgataTransConditions() = 0;
};

extern IConditionMan *g_pIConditionMan;

#define		CDTGID_TYPE_MIX								0X80000000								// 混合条件组
#define		CDTGID_TYPE_EXTEND							0X40000000								// 扩展条件组
#define		CDTGID_TYPE_MINE							0X20000000								// 我的条件组

typedef struct tagFIXCONDITIONGROUP				// 固定条件组信息
{
	DWORD					dwGroupID;			// 组ID，为0则是条件，其他则是条件组，见宏定义，如：CDTID_FIXGROUP_NUMBERS
    
private:	// 私有，必须通过函数GetCdtID和GetName获取，通过SetCdtID和SetGroupName设置
	union
	{
		CDTID				cdtID;				// 条件ID号，当为条件或扩展条件组时有效，否则返回无效条件ID
		char				szGroupName[33];	// 条件组名称，当为条件组且不是扩展条件组时有效
	};
    
public:
	// 以下变量仅供系统使用
	tagFIXCONDITIONGROUP	*lpParent;			// 父信息指针
	CDWordArray stSubItemPointArray;            // 子信息指针(tagFIXCONDITIONGROUP)集合
	
	// 以下为操作函数
	tagFIXCONDITIONGROUP()
	{
		dwGroupID = 0;
		cdtID.Init();
		lpParent = NULL;
		stSubItemPointArray.RemoveAll();
	};
	void Copy(tagFIXCONDITIONGROUP *lpSrcFCGroup, BOOL bNoSubItem = TRUE)
	{
		ASSERT(lpSrcFCGroup != NULL);
		dwGroupID = lpSrcFCGroup->dwGroupID;
		if(lpSrcFCGroup->IsCdtIDValid())
		{
			cdtID = lpSrcFCGroup->cdtID;
		}
		else
		{
			strcpy(szGroupName, lpSrcFCGroup->szGroupName);
		}
		if(bNoSubItem)
		{
			lpParent = NULL;
			stSubItemPointArray.RemoveAll();
		}
		else
		{
			lpParent = lpSrcFCGroup->lpParent;
			stSubItemPointArray.Copy(lpSrcFCGroup->stSubItemPointArray);
		}
	};
    
	BOOL IsCondition() const		// 是否是单个条件，否则就是条件组
	{
		return dwGroupID == 0 ? TRUE : FALSE; 
	};
	BOOL IsExtendGroup() const		// 是否是扩展条件组
	{
		return (dwGroupID & CDTGID_TYPE_EXTEND) ? TRUE : FALSE;
	};
	BOOL IsMixGroup() const			// 是否是混合条件组
	{
		return (dwGroupID & CDTGID_TYPE_MIX) ? TRUE : FALSE;
	};
	BOOL IsMineGroup() const		// 是否是我的条件组
	{
		return (dwGroupID & CDTGID_TYPE_MINE) ? TRUE : FALSE;
	};
	BOOL IsCdtIDValid() const		// 条件ID是否有效
	{
		return ((dwGroupID == 0) || (dwGroupID & CDTGID_TYPE_EXTEND)) ? TRUE : FALSE;
	}
	
	const CDTID & GetCdtID()
	{
		ASSERT(IsCdtIDValid());
        
		return cdtID;
	}
	
	LPCDTID GetCdtIDPoint()
	{
		ASSERT(IsCdtIDValid());
		
		return &cdtID;
	}
    
	BOOL SetCdtID(const CDTID &cdtIDIn)
	{
		if(IsCdtIDValid())
		{
			cdtID = cdtIDIn;
			return TRUE;
		}
        
		return FALSE;
	}
    
	LPCSTR GetName() const			// 获取名称
	{
		if(IsCdtIDValid())
		{
			return g_pIConditionMan->GetConditionName(cdtID);
		}
		else
		{
			g_pIConditionMan->ReplaceFixConditionGroupName(dwGroupID, (LPSTR)szGroupName);
			return szGroupName;
		}
	};
	
	BOOL SetGroupName(LPCSTR lpszGroupName)
	{
		if(IsCdtIDValid())
		{
			return FALSE;
		}
        
		strcpy(szGroupName, lpszGroupName);
		return TRUE;
	}
    
	BOOL operator==(const tagFIXCONDITIONGROUP &stFCGroupSrc) const
	{
		if(dwGroupID != stFCGroupSrc.dwGroupID)
		{
			return FALSE;
		}
		if(stFCGroupSrc.IsCdtIDValid())
		{
			return cdtID == stFCGroupSrc.cdtID ? TRUE : FALSE;
		}
		else
		{
			return !strcmp(szGroupName, stFCGroupSrc.szGroupName) ? TRUE : FALSE;
		}
	};
	BOOL operator!=(const tagFIXCONDITIONGROUP &stFCGroupSrc) const
	{
		return (*this) == stFCGroupSrc ? FALSE : TRUE;
	};
}FIXCONDITIONGROUP, *LPFIXCONDITIONGROUP;
