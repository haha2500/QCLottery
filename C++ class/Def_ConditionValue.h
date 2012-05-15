//
//  Def_ConditionValue.h
//  Graph
//
//  Created by 武 孙 on 12-5-10.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

//////////////////////////////////////////////////////////////////////////////////////////////////////
// 以下定义特定条件值
#define		CDTVALUE_INVALID			(int)0X7FFFFFFF			// 无效值

// 以下定义在指定条件的GetValue的相关返回值
// ----------------------------------------------------------------------------
// 定义类型条件IID_STC_GENRE_NORMAL的返回值
// 以下用于两个数字的号码
#define		DT_AA						0				// 对子
#define		DT_OL						1				// 奇连
#define		DT_EL						2				// 偶连
#define		DT_AB						3				// 连子
#define		DT_AD						4				// 杂号
// 以下用于三个数字的号码
#define		DT_AAA						0				// 豹子
#define		DT_AAB						1				// 组三
#define		DT_ABC						2				// 拖拉机
#define		DT_ABD						3				// 连子
#define		DT_ACE						4				// 杂六
// 以下用于四个数字的号码
#define		DT_AAAA						0				// 炸弹
#define		DT_AAAB						1				// 豹子
#define		DT_AABB						2				// 两对
#define		DT_AABC						3				// 一对
#define		DT_ABCD						4				// 顺子
#define		DT_ABCE						5				// 三连
#define		DT_ABDF						6				// 二连
#define		DT_ACEG						7				// 杂号

// ----------------------------------------------------------------------------
// 定义变换类型条件IID_STC_GENRE_TRANS的返回值
#define		DTE_TAAA					0				// 真豹子
#define		DTE_FAAA					1				// 伪豹子
#define		DTE_TAAB					2				// 真组三
#define		DTE_FAAB					3				// 伪组三
#define		DTE_TABC					4				// 真顺子
#define		DTE_FABC					5				// 伪顺子
#define		DTE_TABD					6				// 真半顺
#define		DTE_TACE					7				// 真杂六

// ----------------------------------------------------------------------------
// 定义形态条件IID_STC_SHAPE_NORMAL的返回值
#define     DS_PARALLEL					0				// 平行形
#define     DS_ASCEND					1			    // 上升形
#define     DS_DESCEND					2			    // 下降形
#define     DS_HEAVE					3			    // 凸起形
#define     DS_CONCAVE					4				// 凹下形

// ----------------------------------------------------------------------------
// 定义细分形态条件IID_STC_SHAPE_EX的返回值
#define     DSEX_AAA					0				// 平行形
#define     DSEX_AAB					1			    // 左升三
#define     DSEX_ABC					2			    // 上升六
#define     DSEX_ABB					3			    // 右升三
#define     DSEX_BBA					4			    // 左降三
#define     DSEX_CBA					5			    // 下降六
#define     DSEX_BAA					6			    // 右降三
#define     DSEX_BCA					7			    // 前凸六
#define     DSEX_ABA					8			    // 凸起三
#define     DSEX_ACB					9			    // 后凸六
#define     DSEX_CAB					10			    // 前凹六
#define     DSEX_BAB					11			    // 凹下三
#define     DSEX_BAC					12			    // 后凹六

// ----------------------------------------------------------------------------
// 定义组三形态条件IID_STC_SHAPE_AAB的返回值
#define		DTAAB_NONE					0				// 非组三
#define		DTAAB_AAB					1				// 组三ABB
#define		DTAAB_ABA					2				// 组三ABA
#define		DTAAB_BAA					3				// 组三BAA

// ----------------------------------------------------------------------------
// 定义顺子形态条件IID_STC_SHAPE_ABC的返回值
#define		DTABC_NONE					0				// 非顺子
#define		DTABC_ABC					1				// 顺子ABC
#define		DTABC_ACB					2				// 顺子ACB
#define		DTABC_BAC					3				// 顺子BAC
#define		DTABC_BCA					4				// 顺子BCA
#define		DTABC_CAB					5				// 顺子CAB
#define		DTABC_CBA					6				// 顺子CBA

// ----------------------------------------------------------------------------
// 定义半顺形态条件IID_STC_SHAPE_ABD的返回值
#define		DTABD_NONE					0				// 非半顺
#define		DTABD_ABD					1				// 半顺ABD
#define		DTABD_ADB					2				// 半顺ADB
#define		DTABD_DAB					3				// 半顺DAB

// ----------------------------------------------------------------------------
// 定义组六分类二码1条件IID_STC_TWONUM_Z6的返回值
#define		DTNZ6_03					0				// 含03组合的组六号码
#define		DTNZ6_06					1				// 含06组合的组六号码
#define		DTNZ6_09					2				// 含09组合的组六号码
#define		DTNZ6_14					3				// 含14组合的组六号码
#define		DTNZ6_17					4				// 含17组合的组六号码
#define		DTNZ6_25					5				// 含25组合的组六号码
#define		DTNZ6_28					6				// 含28组合的组六号码
#define		DTNZ6_Z3					7				// 组三号码
#define		DTNZ6_36					8				// 含36组合的组六号码
#define		DTNZ6_39					9				// 含39组合的组六号码
#define		DTNZ6_47					10				// 含47组合的组六号码
#define		DTNZ6_58					11				// 含58组合的组六号码
#define		DTNZ6_69					12				// 含69组合的组六号码
#define		DTNZ6_C						13				// 组选111路号码
#define		DTNZ6_A						14				// 组选003或030或300路号码
#define		DTNZ6_BZ					15				// 豹子号码

// 定义组六分类二码2条件IID_STC_TWONUM_Z6_2的返回值
#define		DTNZ62_00					0				// 有且仅有2个0路的组六号码
#define		DTNZ62_11					1				// 有且仅有2个1路的组六号码
#define		DTNZ62_22					2				// 有且仅有2个2路的组六号码
#define		DTNZ62_Z3					3				// 组三号码
#define		DTNZ62_C					4				// 组选111路号码
#define		DTNZ62_A					5				// 组选003或030或300路号码
#define		DTNZ62_BZ					6				// 豹子号码

// 定义组三分类二码条件IID_STC_TWONUM_Z3的返回值
#define		DTNZ3_00					0				// 对子码为0路且另一码为0路的号码
#define		DTNZ3_01					1				// 对子码为0路且另一码为1路的号码
#define		DTNZ3_02					2				// 对子码为0路且另一码为2路的号码
#define		DTNZ3_10					3				// 对子码为1路且另一码为0路的号码
#define		DTNZ3_11					4				// 对子码为1路且另一码为1路的号码
#define		DTNZ3_12					5				// 对子码为1路且另一码为2路的号码
#define		DTNZ3_20					6				// 对子码为2路且另一码为0路的号码
#define		DTNZ3_21					7				// 对子码为2路且另一码为1路的号码
#define		DTNZ3_22					8				// 对子码为2路且另一码为2路的号码
#define		DTNZ3_Z6					9				// 组六号码
#define		DTNZ3_BZ					10				// 豹子号码

// 定义组三分类二码2条件IID_STC_TWONUM_Z32的返回值
#define		DTNZ32_0					0				// 对子码为0路的号码
#define		DTNZ32_1					1				// 对子码为1路的号码
#define		DTNZ32_2					2				// 对子码为2路的号码
#define		DTNZ32_Z6					3				// 组六号码
#define		DTNZ32_BZ					4				// 豹子号码

// 定义试机号综合重复信息的返回值
#define		DTTESTNUMS_NONE					0				// 与试机号和上期开奖号都不重复
#define		DTTESTNUMS_NUMS1				1				// 与试机号不重复，与上期开奖号重复1个
#define		DTTESTNUMS_TEST1				2				// 与试机号重复1个，与上期开奖号不重复
#define		DTTESTNUMS_ALL1					3				// 与试机号和上期开奖号都重复1个
#define		DTTESTNUMS_OTHER				4				// 其他
