//
//  Def_GraphGrid_ID.h
//  Graph
//
//  Created by 武 孙 on 12-5-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

// =========================================================================================
// 图表的ID号为四个字节，最高字如果不为0，则为对应条件的ID号此时为条件图表，
// 当高字为0时，则为特定的图表，此时低字有效，定义如下：
// 前4位（0-15）为图表类型，后12位（0-4095）ID号。
// =========================================================================================
typedef struct tagGRAPHGRIDID							// 图表ID
{
	BYTE	btType;										// 图表类型，见宏定义，如：GGTYPE_CONDITION
	union
	{
		DWORD		dwGraphGridID;						// 图表ID，当不为条件图表和分区表时有效
        DWORD       dwCdtID;                            // TEST
//		CDTID		cdtID;								// 条件ID，当为条件图表时有效
//		DWORD		dwAreaDiffValues[3];				// 分区区分数据（前2个DWORD）和标志值（后1个DWORD），当btType为GGTYPE_DATATABLE_AREA（即分区表）或GGTYPE_DISTRIBUTE_AREA（即分区分布图）时有效
        // 目前标志值可为1：分区个数（用于分区表）或者基本分区分布图
	};
    
	BOOL operator==(const tagGRAPHGRIDID &ggid)
	{
		return !memcmp(&ggid, this, sizeof(tagGRAPHGRIDID));
	};
	BOOL operator!=(const tagGRAPHGRIDID &ggid)
	{
		return memcmp(&ggid, this, sizeof(tagGRAPHGRIDID));
	};
    
}GRAPHGRIDID, *LPGRAPHGRIDID;

// 定义特定的图表类型
#define		GGTYPE_INIT							BYTE(0)					// 初始值
#define		GGTYPE_CONDITION					BYTE(1)					// 条件图表，此时表格ID为条件的ID号，（特殊值）
#define		GGTYPE_TREND						BYTE(2)					// 特定的走势图，此时表格ID为走势图ID，比如演示走势图
#define		GGTYPE_DISTRIBUTE					BYTE(3)					// 特定的分布图，此时表格ID为分布图ID，比如热温冷分布图
#define		GGTYPE_DATATABLE					BYTE(4)					// 数据表，此时表格ID为数据表ID，比如遗漏值排列表等
#define		GGTYPE_DATATABLE_AREA				BYTE(5)					// 分区数据表，此时表格ID为分区的数据区分值和标志值
#define		GGTYPE_DISTRIBUTE_AREA				BYTE(6)					// 分区分布图，此时表格ID为分区的数据区分值和标志值
#define		GGTYPE_HIDE							BYTE(0X80)				// 隐藏标志，必须与上面的某个值一起使用

// 定义特定走势图ID
#define		GGID_TG_BEGIN						0X00001000				// 特定走势图开始ID
#define		GGID_TG_DEMO_1						(GGID_TG_BEGIN+1)		// 演示走势图1
#define		GGID_TG_DEMO_20						(GGID_TG_BEGIN+20)		// 演示走势图20
#define		GGID_TG_CUSTOMARRANGE_POS_1			(GGID_TG_BEGIN+100)		// 第1位自定义排列走势图
#define		GGID_TG_CUSTOMARRANGE_POS_2			(GGID_TG_BEGIN+101)		// 第2位自定义排列走势图
#define		GGID_TG_CUSTOMARRANGE_POS_3			(GGID_TG_BEGIN+102)		// 第3位自定义排列走势图
#define		GGID_TG_CUSTOMARRANGE_POS_4			(GGID_TG_BEGIN+103)		// 第4位自定义排列走势图
#define		GGID_TG_CUSTOMARRANGE_POS_5			(GGID_TG_BEGIN+104)		// 第5位自定义排列走势图
#define		GGID_TG_CUSTOMARRANGE_POS_6			(GGID_TG_BEGIN+105)		// 第6位自定义排列走势图
#define		GGID_TG_CUSTOMARRANGE_POS_7			(GGID_TG_BEGIN+106)		// 第7位自定义排列走势图
#define		GGID_TG_CUSTOMARRANGE_POS_8			(GGID_TG_BEGIN+107)		// 第8位自定义排列走势图
#define		GGID_TG_CUSTOMARRANGE_SUMTAIL		(GGID_TG_BEGIN+120)		// 合值自定义排列走势图
#define		GGID_TG_CUSTOMARRANGE_SUMN			(GGID_TG_BEGIN+130)		// N次和自定义排列走势图
#define		GGID_TG_CUSTOMARRANGE_SPAN			(GGID_TG_BEGIN+140)		// 跨度自定义排列走势图
#define		GGID_TG_CUSTOMARRANGE_AVG			(GGID_TG_BEGIN+150)		// 均值自定义排列走势图
#define		GGID_TG_CUSTOMCOMBO					(GGID_TG_BEGIN+200)		// 自定义组合走势图

// 定义特定分布图ID
#define		GGID_DG_BEGIN						0X00002000				// 特定分布图开始ID
#define		GGID_DG_DEMO_1						(GGID_DG_BEGIN+1)		// 演示分布图1
#define		GGID_DG_DEMO_20						(GGID_DG_BEGIN+20)		// 演示分布图20
#define		GGID_DG_NUMBER_HORIZONTAL			(GGID_DG_BEGIN+101)		// 号码横向显示分布图
#define		GGID_DG_NUMBER_VERTICAL				(GGID_DG_BEGIN+102)		// 号码纵向显示分布图
#define		GGID_DG_NUMBER_CROSS				(GGID_DG_BEGIN+103)		// 号码斜向显示分布图
#define		GGID_DG_NUMBER_LT					(GGID_DG_BEGIN+105)		// 乐透号码分布图
#define		GGID_DG_NUMBER_LGC					(GGID_DG_BEGIN+200)		// 号码邻孤传分布图
#define		GGID_DG_NUMBER_FGZ					(GGID_DG_BEGIN+201)		// 号码复隔中分布图
#define		GGID_DG_NUMBER_CXT					(GGID_DG_BEGIN+202)		// 号码重斜跳分布图
#define		GGID_DG_NUMBER_RWL					(GGID_DG_BEGIN+203)		// 号码热温冷分布图
#define		GGID_DG_NUMBER_ODDEVEN_EVEN			(GGID_DG_BEGIN+300)		// 号码先偶后奇分布图
#define		GGID_DG_NUMBER_ODDEVEN_ODD			(GGID_DG_BEGIN+301)		// 号码先奇后偶分布图
#define		GGID_DG_NUMBER_BIGSMALL_SMALL		(GGID_DG_BEGIN+302)		// 号码先小后大分布图
#define		GGID_DG_NUMBER_BIGSMALL_BIG			(GGID_DG_BEGIN+303)		// 号码先大后小分布图
#define		GGID_DG_TWONUM_BASE					(GGID_DG_BEGIN+400)		// 二码分布图
#define		GGID_DG_TWONUM_SUMTAIL				(GGID_DG_BEGIN+401)		// 二码合分布图
#define		GGID_DG_TWONUM_SUBABS				(GGID_DG_BEGIN+402)		// 二码差分布图
#define		GGID_DG_Z3_BASE						(GGID_DG_BEGIN+500)		// 组三分布图
#define		GGID_DG_TLJ_BASE					(GGID_DG_BEGIN+501)		// 顺子分布图
#define		GGID_DG_BS_BASE						(GGID_DG_BEGIN+502)		// 半顺分布图
#define		GGID_DG_LM_BASE						(GGID_DG_BEGIN+503)		// 二连码分布图
#define		GGID_DG_JOL_BASE					(GGID_DG_BEGIN+504)		// 奇偶连分布图

#define		GGID_DG_DHK_HD						(GGID_DG_BEGIN+610)		// 合胆同分布图
#define		GGID_DG_DHK_KD						(GGID_DG_BEGIN+611)		// 跨胆同分布图
#define		GGID_DG_DHK_HK						(GGID_DG_BEGIN+612)		// 合跨同分布图

#define		GGID_DG_LGC_L						(GGID_DG_BEGIN+700)		// 邻码分布图
#define		GGID_DG_LGC_G						(GGID_DG_BEGIN+701)		// 孤码分布图
#define		GGID_DG_LGC_C						(GGID_DG_BEGIN+702)		// 传码分布图
#define		GGID_DG_FGZ_F						(GGID_DG_BEGIN+710)		// 复码分布图
#define		GGID_DG_FGZ_G						(GGID_DG_BEGIN+711)		// 隔码分布图
#define		GGID_DG_FGZ_Z						(GGID_DG_BEGIN+712)		// 中码分布图
#define		GGID_DG_CXT_C						(GGID_DG_BEGIN+720)		// 重码分布图
#define		GGID_DG_CXT_X						(GGID_DG_BEGIN+721)		// 斜码分布图
#define		GGID_DG_CXT_T						(GGID_DG_BEGIN+722)		// 跳码分布图
#define		GGID_DG_RWL_R						(GGID_DG_BEGIN+730)		// 热码分布图
#define		GGID_DG_RWL_W						(GGID_DG_BEGIN+731)		// 温码分布图
#define		GGID_DG_RWL_L						(GGID_DG_BEGIN+732)		// 冷码分布图

#define		GGID_DG_CUSTOMARRANGE_POSANY		(GGID_DG_BEGIN+800)		// 任意号码自定义排列分布图
#define		GGID_DG_REMAINDER_BEGIN				(GGID_DG_BEGIN+1000)	// 除N余数分布图开始（N=0）
#define		GGID_DG_REMAINDER_END				(GGID_DG_BEGIN+1099)	// 除N余数分布图结束（N=99）

// 定义数据表ID
#define		GGID_DT_BEGIN						0X00003000				// 数据表开始ID
#define		GGID_DT_NUMSTABLE					(GGID_DT_BEGIN)			// 号码表
#define		GGID_DT_ORDER_LGC					(GGID_DT_BEGIN+1)		// 邻孤传排列表
#define		GGID_DT_ORDER_FGZ					(GGID_DT_BEGIN+2)		// 复隔中排列表
#define		GGID_DT_ORDER_CXT					(GGID_DT_BEGIN+3)		// 重斜跳排列表
#define		GGID_DT_ORDER_RWL					(GGID_DT_BEGIN+4)		// 热温冷排列表
#define		GGID_DT_SKIP_GRO					(GGID_DT_BEGIN+100)		// 组选遗漏值表
#define		GGID_DT_SKIP_SIN					(GGID_DT_BEGIN+101)		// 单选遗漏值表
#define		GGID_DT_MUL							(GGID_DT_BEGIN+200)		// 积数数值表
#define		GGID_DT_XLNUM						(GGID_DT_BEGIN+500)		// 斜连号码显示表

#define		GGID_DT_REMAINDER_COUNT_BEGIN		(GGID_DT_BEGIN+1000)	// 除N余数个数表开始（N=0）
#define		GGID_DT_REMAINDER_COUNT_END			(GGID_DT_BEGIN+1099)	// 除N余数个数表结束（N=99）
// =========================================================================================

// -----------------------------------------------------------------------------------------
// 定义获取表格ID类型的宏，返回表格类型值，如：GGTYPE_DISTRIBUTE，旧版本使用，2010-12-21
#define		GET_GRAPHGRID_TYPE_OLD(dwGraphGridIDIn) \
BYTE((HIWORD(dwGraphGridIDIn) == 0) ? ((((dwGraphGridIDIn) >> 12) & 0X0000000F) + 1) : GGTYPE_CONDITION)

// =========================================================================================
// 图表组的ID号为四个字节，最高字节为类型，第二字节保留，低字为ID号
// =========================================================================================

// 以下定义类型
#define		GGGID_TYPE_MINE							0X80000000		// 属于我的图表组
#define		GGGID_TYPE_GRIDS						0X40000000		// 含有图表的组（即为叶子节点）

// 以下定义我的图表组
#define		GGGID_MINE_GROUP_BEGIN					0X80000000		// 我的图表组（不含图表）开始
#define		GGGID_MINE_ITEM_BEGIN					0XC0000000		// 我的图表组（含图表）开始

// 以下定义系统分组ID
#define		GGGID_BASIC_GROUP						0X00000100		// 基本图表
#define		GGGID_BASIC_NORMAL						0X40000101		// 常规走势图
#define		GGGID_BASIC_ODDEVEN						0X40000102		// 奇偶走势图
#define		GGGID_BASIC_BIGSMALL					0X40000103		// 大小走势图
#define		GGGID_BASIC_BMS							0X40000104		// 大中小走势图
#define		GGGID_BASIC_012							0X40000105		// 012走势图
#define		GGGID_BASIC_PRIMENUMBER					0X40000106		// 质合走势图

#define		GGGID_COMPLEX_GROUP						0X00000200		// 综合图表
#define		GGGID_COMPLEX_SPJ_GROUP					0X40000201		// 组选升平降走势图
#define		GGGID_COMPLEX_LGC_GROUP					0X40000202		// 组选邻孤传走势图
#define		GGGID_COMPLEX_FGZ_GROUP					0X40000203		// 组选复隔中走势图
#define		GGGID_COMPLEX_CXT_GROUP					0X40000204		// 组选重斜跳走势图
#define		GGGID_COMPLEX_RWL_GROUP					0X40000205		// 组选热温冷走势图
#define		GGGID_COMPLEX_SPJ_SINGLE				0X40000211		// 单选升平降走势图
#define		GGGID_COMPLEX_LGC_SINGLE				0X40000212		// 单选邻孤传走势图
#define		GGGID_COMPLEX_FGZ_SINGLE				0X40000213		// 单选复隔中走势图
#define		GGGID_COMPLEX_CXT_SINGLE				0X40000214		// 单选重斜跳走势图
#define		GGGID_COMPLEX_RWL_SINGLE				0X40000215		// 单选热温冷走势图
#define		GGGID_COMPLEX_FLEM						0X40000220		// 分类二码走势图
#define		GGGID_COMPLEX_DHK						0X40000230		// 胆合跨走势图
#define		GGGID_COMPLEX_NUMBER					0X40000240		// 号码数字走势图
#define		GGGID_COMPLEX_REPEAT_N					0X40000250		// 前第N期走势图
#define		GGGID_COMPLEX_REPEAT_1TON				0X40000253		// 前N期走势图

#define		GGGID_INDICATOR_GROUP					0X00000300		// 指标图表
#define		GGGID_INDICATOR_POS_1					0X40000301		// 第一位走势图
#define		GGGID_INDICATOR_POS_2					0X40000302		// 第二位走势图
#define		GGGID_INDICATOR_POS_3					0X40000303		// 第三位走势图
#define		GGGID_INDICATOR_POS_4					0X40000304		// 第四位走势图
#define		GGGID_INDICATOR_POS_5					0X40000305		// 第五位走势图
#define		GGGID_INDICATOR_POS_6					0X40000306		// 第六位走势图
#define		GGGID_INDICATOR_POS_7					0X40000307		// 第七位走势图
#define		GGGID_INDICATOR_POS_8					0X40000308		// 第八位走势图
#define		GGGID_INDICATOR_SPAN					0X40000310		// 跨度走势图
#define		GGGID_INDICATOR_SPAN2					0X40000311		// 跨度走势图2
#define		GGGID_INDICATOR_SPANTAIL				0X40000315		// 跨度尾数走势图
#define		GGGID_INDICATOR_SPANTAIL2				0X40000316		// 跨度尾数走势图2
#define		GGGID_INDICATOR_SUM						0X40000320		// 和值走势图
#define		GGGID_INDICATOR_SUM2					0X40000321		// 和值走势图2
#define		GGGID_INDICATOR_SUM_AREA				0X40000325		// 和值分区走势图
#define		GGGID_INDICATOR_SUMTAIL					0X40000330		// 合值走势图
#define		GGGID_INDICATOR_SUMTAIL2				0X40000331		// 合值走势图2
#define		GGGID_INDICATOR_SUM_N					0X40000340		// N次合值走势图
#define		GGGID_INDICATOR_SUM_N2					0X40000341		// N次合值走势图2
#define		GGGID_INDICATOR_AVG						0X40000345		// 均值走势图
#define		GGGID_INDICATOR_AVG2					0X40000346		// 均值走势图2
#define		GGGID_INDICATOR_TMH						0X40000350		// 特码和走势图
#define		GGGID_INDICATOR_TMH2					0X40000351		// 特码和2走势图
#define		GGGID_INDICATOR_DETRACT					0X40000360		// 散度走势图
#define		GGGID_INDICATOR_DEPARTURE				0X40000370		// 偏度走势图
#define		GGGID_INDICATOR_SPACE_SINGLE			0X40000380		// 单选间距走势图
#define		GGGID_INDICATOR_SPACE_GROUP				0X40000383		// 组选间距走势图
#define		GGGID_INDICATOR_XL						0X40000390		// 斜连值走势图
#define		GGGID_INDICATOR_STATE					0X400003A0		// 形态走势图
#define		GGGID_INDICATOR_LINK					0X400003A5		// 连号走势图
#define		GGGID_INDICATOR_AC						0X400003B0		// AC值走势图

#define		GGGID_HEADTAIL_GROUP					0X00000400		// 首尾关系图表
#define		GGGID_HEADTAIL_SUM						0X40000410		// 首尾和走势图
#define		GGGID_HEADTAIL_SUMTAIL					0X40000420		// 首尾合走势图
#define		GGGID_HEADTAIL_SUB						0X40000430		// 首尾差走势图
#define		GGGID_HEADTAIL_SUBTAIL					0X40000440		// 首尾差尾数走势图
#define		GGGID_HEADTAIL_CIRSUB					0X40000450		// 首尾环差走势图
#define		GGGID_HEADTAIL_CIRSUBTAIL				0X40000460		// 首尾环差尾数走势图

#define		GGGID_NEARSPACE_GROUP					0X00000500		// 邻位间距图表
#define		GGGID_NEARSPACE_TOTAL					0X40000510		// 邻位间距总体走势图
#define		GGGID_NEARSPACE_TOTAL2					0X40000515		// 邻位间距总体走势图2
#define		GGGID_NEARSPACE_12						0X40000520		// 1-2邻位间距走势图
#define		GGGID_NEARSPACE_23						0X40000521		// 2-3邻位间距走势图
#define		GGGID_NEARSPACE_34						0X40000522		// 3-4邻位间距走势图
#define		GGGID_NEARSPACE_45						0X40000523		// 4-5邻位间距走势图
#define		GGGID_NEARSPACE_56						0X40000524		// 5-6邻位间距走势图
#define		GGGID_NEARSPACE_67						0X40000525		// 6-7邻位间距走势图
#define		GGGID_NEARSPACE_78						0X40000526		// 7-8邻位间距走势图
#define		GGGID_NEARSPACE_TH						0X4000052F		// 尾-首邻位间距走势图

#define		GGGID_1D_GROUP							0X00000600		// 1D图表
#define		GGGID_1D_POSITIVE_SIN					0X40000610		// 单选正1D走势图
#define		GGGID_1D_POSITIVE_GRO					0X40000620		// 组选正1D走势图
#define		GGGID_1D_NEGATIVE_SIN					0X40000630		// 单选负1D走势图
#define		GGGID_1D_NEGATIVE_GRO					0X40000640		// 组选负1D走势图
#define		GGGID_1D_RPOSITIVE_SIN					0X40000650		// 单选反正1D走势图
#define		GGGID_1D_RPOSITIVE_GRO					0X40000660		// 组选反正1D走势图
#define		GGGID_1D_RNEGATIVE_SIN					0X40000670		// 单选反负1D走势图
#define		GGGID_1D_RNEGATIVE_GRO					0X40000680		// 组选反负1D走势图

#define		GGGID_SKIP_GROUP						0X00000700		// 遗漏值图表
#define		GGGID_SKIP_GRO_GROUP					0X00000701		// 组选遗漏值图表
#define		GGGID_SKIP_GRO_POS_1					0X40000701		// 第一位组选遗漏值走势图
#define		GGGID_SKIP_GRO_POS_2					0X40000702		// 第二位组选遗漏值走势图
#define		GGGID_SKIP_GRO_POS_3					0X40000703		// 第三位组选遗漏值走势图
#define		GGGID_SKIP_GRO_POS_4					0X40000704		// 第四位组选遗漏值走势图
#define		GGGID_SKIP_GRO_POS_5					0X40000705		// 第五位组选遗漏值走势图
#define		GGGID_SKIP_GRO_POS_6					0X40000706		// 第六位组选遗漏值走势图
#define		GGGID_SKIP_GRO_POS_7					0X40000707		// 第七位组选遗漏值走势图
#define		GGGID_SKIP_GRO_POS_8					0X40000708		// 第八位组选遗漏值走势图
#define		GGGID_SKIP_GRO_SUM						0X40000710		// 组选遗漏值和值走势图
#define		GGGID_SKIP_GRO_SUMTAIL					0X40000711		// 组选遗漏值合值走势图

#define		GGGID_SKIP_SIN_GROUP					0X00000740		// 单选遗漏值图表
#define		GGGID_SKIP_SIN_POS_1					0X40000741		// 第一位单选遗漏值走势图
#define		GGGID_SKIP_SIN_POS_2					0X40000742		// 第二位单选遗漏值走势图
#define		GGGID_SKIP_SIN_POS_3					0X40000743		// 第三位单选遗漏值走势图
#define		GGGID_SKIP_SIN_POS_4					0X40000744		// 第四位单选遗漏值走势图
#define		GGGID_SKIP_SIN_POS_5					0X40000745		// 第五位单选遗漏值走势图
#define		GGGID_SKIP_SIN_POS_6					0X40000746		// 第六位单选遗漏值走势图
#define		GGGID_SKIP_SIN_POS_7					0X40000747		// 第七位单选遗漏值走势图
#define		GGGID_SKIP_SIN_POS_8					0X40000748		// 第八位单选遗漏值走势图
#define		GGGID_SKIP_SIN_SUM						0X40000750		// 单选遗漏值和值走势图
#define		GGGID_SKIP_SIN_SUMTAIL					0X40000751		// 单选遗漏值合值走势图

#define		GGGID_SKIPTAIL_GROUP					0X00000780		// 指标遗漏值尾图表
#define		GGGID_SKIPTAIL_SPAN						0X40000781		// 跨度遗漏值尾走势图
#define		GGGID_SKIPTAIL_SPANTAIL					0X40000782		// 跨度尾遗漏值尾走势图
#define		GGGID_SKIPTAIL_SUM						0X40000791		// 和值遗漏值尾走势图
#define		GGGID_SKIPTAIL_SUMTAIL					0X40000792		// 合值遗漏值尾走势图
#define		GGGID_SKIPTAIL_SUM_N					0X40000793		// N次和遗漏值尾走势图
#define		GGGID_SKIPTAIL_AVG						0X40000794		// 均值遗漏值尾走势图

#define		GGGID_CUSTOM_GROUP						0X00000800		// 自定义图表
#define		GGGID_CUSTOM_ARRANGE					0X40000801		// 自定义排列走势图
#define		GGGID_CUSTOM_COMBO						0X40000802		// 自定义组合走势图

#define		GGGID_PRODUCT_GROUP						0X00000900		// 积数相关图表
#define		GGGID_PRODUCT_PRODUCT					0X40000901		// 积数走势图
#define		GGGID_PRODUCT_PRODUCTTAIL				0X40000902		// 积数尾走势图
#define		GGGID_PRODUCT_PRODUCT_SUM				0X40000910		// 位置积和走势图
#define		GGGID_PRODUCT_PRODUCT_SUMTAIL			0X40000920		// 位置积合走势图
#define		GGGID_PRODUCT_RPRODUCT_SUM				0X40000930		// 反位置积和走势图
#define		GGGID_PRODUCT_RPRODUCT_SUMTAIL			0X40000940		// 反位置积合走势图

#define		GGGID_ORDER_GROUP						0X00000A00		// 顺序排列图表
#define		GGGID_ORDER_BASE						0X40000A01		// 顺序排列走势图
#define		GGGID_ORDER_SUM							0X40000A10		// 顺序排列位和走势图
#define		GGGID_ORDER_SUMTAIL						0X40000A11		// 顺序排列位合走势图
#define		GGGID_ORDER_SUB							0X40000A12		// 顺序排列位差走势图
#define		GGGID_ORDER_RELATIVEDZX					0X40000A20		// 相对大中小走势图

#define		GGGID_SPECIAL_GROUP						0X01000B00		// 特定图表
#define		GGGID_SPECIAL_NUMS						0X41000B01		// 号码分布图
#define		GGGID_SPECIAL_TWONUM					0X41000B20		// 二码分布图
#define		GGGID_SPECIAL_TWONUM_SUM				0X41000B21		// 二码合分布图
#define		GGGID_SPECIAL_TWONUM_SUB				0X41000B22		// 二码差分布图
#define		GGGID_SPECIAL_TWONUM_SUMSUB				0X41000B23		// 二码合差分布图
#define		GGGID_SPECIAL_TWONUM_SUMCIRSUB			0X41000B24		// 二码合环差分布图
#define		GGGID_SPECIAL_TYPE						0X41000B30		// 类型形态分布图
#define		GGGID_SPECIAL_CONNECT_NUM				0X41000B31		// 连码分布图
#define		GGGID_SPECIAL_ODDEVEN					0X41000B40		// 奇偶分布图（或先奇后偶分布图）
#define		GGGID_SPECIAL_BIGSMALL					0X41000B41		// 大小分布图（或先大后小分布图）
#define		GGGID_SPECIAL_EVENODD					0X41000B42		// 先偶后奇分布图
#define		GGGID_SPECIAL_SMALLBIG					0X41000B43		// 先小后大分布图

#define		GGGID_TESTNUMS_GROUP					0X00000C00		// 试机号相关图表
#define		GGGID_TESTNUMS_WHOLE					0X40000C01		// 试机号整体走势图
#define		GGGID_TESTNUMS_DISTANCE					0X40000C10		// 开奖号与试机号距离走势图
#define		GGGID_TESTNUMS_SUB						0X40000C20		// 开奖号与试机号之差走势图
#define		GGGID_TESTNUMS_CIRSUB					0X40000C30		// 开奖号与试机号环差走势图
#define		GGGID_TESTNUMS_COMPARE_POS_1			0X40000CA0		// 第一位对照走势图
#define		GGGID_TESTNUMS_COMPARE_POS_2			0X40000CA1		// 第二位对照走势图
#define		GGGID_TESTNUMS_COMPARE_POS_3			0X40000CA2		// 第三位对照走势图
#define		GGGID_TESTNUMS_COMPARE_POS_4			0X40000CA3		// 第四位对照走势图
#define		GGGID_TESTNUMS_COMPARE_POS_5			0X40000CA4		// 第五位对照走势图
#define		GGGID_TESTNUMS_COMPARE_POS_6			0X40000CA5		// 第六位对照走势图
#define		GGGID_TESTNUMS_COMPARE_POS_7			0X40000CA6		// 第七位对照走势图
#define		GGGID_TESTNUMS_COMPARE_POS_8			0X40000CA7		// 第八位对照走势图
#define		GGGID_TESTNUMS_COMPARE_SUM				0X40000CB0		// 和值对照走势图
#define		GGGID_TESTNUMS_COMPARE_SUMTAIL			0X40000CB1		// 合值对照走势图
#define		GGGID_TESTNUMS_COMPARE_SUM_N			0X40000CB2		// N次和对照走势图
#define		GGGID_TESTNUMS_COMPARE_SPAN				0X40000CC0		// 跨度对照走势图
#define		GGGID_TESTNUMS_COMPARE_SPANTAIL			0X40000CC1		// 跨度尾数对照走势图

/////////////////////////////////////////////////////////////////////////////////////////////////
#define		GGGID_PREVDATA_GROUP					0X000080FF		// 邻期关系图表
#define		GGGID_PREVDATA_SUBGROUP_BEGIN			0X00008000		// 邻期关系子图表开始（不直接用于ID）
#define		GGGID_PREVDATA_SUBGROUP_END				0X00009FFF		// 邻期关系子图表结束（不直接用于ID）
#define		GGGID_PREVDATA_BEGIN					0X40008000		// 邻期关系开始（不直接用于ID）
#define		GGGID_PREVDATA_END						0X40009FFF		// 邻期关系结束（不直接用于ID）
// 说明：其中千位和百位为两位类型值，具体见定义（如：PREVDATA_TYPE）
// 十位和个位定义如下：
#define		PREVDATA_POS_1							1				// 第一位
#define		PREVDATA_POS_2							2				// 第二位
#define		PREVDATA_POS_3							3				// 第三位
#define		PREVDATA_POS_4							4				// 第四位
#define		PREVDATA_POS_5							5				// 第五位
#define		PREVDATA_POS_6							6				// 第六位
#define		PREVDATA_POS_7							7				// 第七位
#define		PREVDATA_POS_8							8				// 第八位
#define		PREVDATA_SUM							10				// 和
#define		PREVDATA_SUMTAIL						11				// 合
#define		PREVDATA_COMPLEX						20				// 综合走势图

/////////////////////////////////////////////////////////////////////////////////////////////////
#define		GGGID_TWOPOS_GROUP						0X0000A0FF		// 两位关系图表
#define		GGGID_TWOPOS_SUBGROUP_BEGIN				0X0000A000		// 两位关系子图表开始（不直接用于ID）
#define		GGGID_TWOPOS_SUBGROUP_END				0X0000BFFF		// 两位关系子图表结束（不直接用于ID）
#define		GGGID_TWOPOS_SUBGROUP_HEADTAIL			0X0000BFFE		// 首尾关系图表

#define		GGGID_TWOPOS_MN_BEGIN					0X4000A000		// MN两位关系走势图开始（不直接用于ID）
#define		GGGID_TWOPOS_MN_END						0X4000BFFF		// MN两位关系走势图结束（不直接用于ID）
#define		GGGID_TWOPOS_HEADTAIL_SUM				0X4000BFF0		// 首尾和走势图
#define		GGGID_TWOPOS_HEADTAIL_SUMTAIL			0X4000BFF1		// 首尾合走势图
#define		GGGID_TWOPOS_HEADTAIL_SUB				0X4000BFF2		// 首尾差走势图
#define		GGGID_TWOPOS_HEADTAIL_SUBTAIL			0X4000BFF3		// 首尾差尾数走势图

// 说明：其中千位和百位为两位类型值，具体见定义（如：TPT_SUBABS）
// 十位和个位分别为MN，如：12为第一位和第二位，90以后的值定义如下：
#define		TWOPOS_COMPLEX							90				// 两位关系综合走势图

/////////////////////////////////////////////////////////////////////////////////////////////////
#define		GGGID_REMAINDER_GROUP					0X0000C000						// 余数图表
#define		GGGID_REMAINDER_SUBGROUP_BEGIN			0X0000C100						// 余数子图表开始（不直接用于ID）
#define		GGGID_REMAINDER_SUBGROUP_END			0X0000C1FF						// 余数子图表结束（不直接用于ID）
#define		GGGID_REMAINDER_BEGIN					0X4000C000						// 余数走势图开始（不直接用于ID）
#define		GGGID_REMAINDER_END						(GGGID_REMAINDER_BEGIN+999)		// 余数走势图结束（不直接用于ID）
#define		GGGID_REMAINDER_N_BEGIN					(GGGID_REMAINDER_BEGIN+100)		// 除N余数走势图（十个位为N值）
#define		GGGID_REMAINDER_NCOUNT_BEGIN			(GGGID_REMAINDER_BEGIN+200)		// 除N余数个数走势图（十个位为N值）
#define		GGGID_REMAINDER_NSUM_BEGIN				(GGGID_REMAINDER_BEGIN+300)		// 除N余数和走势图（十个位为N值）
#define		GGGID_REMAINDER_NSUMTAIL_BEGIN			(GGGID_REMAINDER_BEGIN+400)		// 除N余数合走势图（十个位为N值）
#define		GGGID_REMAINDER_NDG_BEGIN				(GGGID_REMAINDER_BEGIN+500)		// 除N余数分布图（十个位为N值）

/////////////////////////////////////////////////////////////////////////////////////////////////
#define		GGGID_AREA_GROUP						0X0000D000						// 分区图表（不直接用于ID）
#define		GGGID_AREA_BEGIN						0X4000D000						// 分区走势图开始（不直接用于ID）
#define		GGGID_AREA_END							(GGGID_AREA_BEGIN+9999)			// 分区走势图开始（不直接用于ID）
#define		GGGID_AREAM_GROUP_BEGIN					(GGGID_AREA_GROUP+1)			// M分区走势图开始（百十个位为分区ID号）
#define		GGGID_AREAM_GROUP_END					(GGGID_AREA_GROUP+999)			// M分区走势图开始（百十个位为分区ID号）
#define		GGGID_AREAM_POS_BEGIN					(GGGID_AREA_BEGIN+1000)			// 按位M分区走势图（百十个位为分区ID号）
#define		GGGID_AREAM_COUNT_BEGIN					(GGGID_AREA_BEGIN+2000)			// M分区个数走势图（百十个位为分区ID号）
#define		GGGID_AREAM_COMPLEX_BEGIN				(GGGID_AREA_BEGIN+3000)			// M分区综合走势图（百十个位为分区ID号）
#define		GGGID_AREAM_DG_BEGIN					(GGGID_AREA_BEGIN+4000)			// M分区分布图（百十个位为分区ID号）

/////////////////////////////////////////////////////////////////////////////////////////////
