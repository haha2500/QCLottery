//
//  Def_ConditionID.h
//  QCLottery
//
//  Created by 武 孙 on 12-5-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

// =========================================================================================
// 条件的ID号的数据类型为CDTID（见定义）
// =========================================================================================

// =========================================================================================
// 以下定义相关操作宏
// 根据指标ID和指标参数ID号生成条件ID号
#define		BUILD_CONDITIONID_IID(cdtIIDin) CDTID((cdtIIDin), 0, 0)
#define		BUILD_CONDITIONID_IPID(cdtIIDin, cdtIPIDin) CDTID((cdtIIDin), (cdtIPIDin), 0)
#define		BUILD_CONDITIONID_IPID2(cdtIIDin, cdtIPIDin, cdtIPIDin2) CDTID((cdtIIDin), (cdtIPIDin), (cdtIPIDin2))

// 根据系统指标ID和指标参数ID号生成条件ID号
#define		BUILD_SYSCONDITIONID_IID(dwSysIID) CDTID((dwSysIID), 0, 0)
#define		BUILD_SYSCONDITIONID_IPID(dwSysIID, cdtIPIDin) CDTID((dwSysIID), (cdtIPIDin), 0)
#define		BUILD_SYSCONDITIONID_IPID2(dwSysIID, cdtIPIDin, cdtIPIDin2) CDTID((dwSysIID), (cdtIPIDin), (cdtIPIDin2))

// =========================================================================================
// 系统指标ID号说明，如下
// 1、分类（最高字节）：定义如下：（目前不再使用）
// #define		IID_TYPE_DYNAMIC							DWORD(0X80000000)					// 动态指标，否则为静态指标（缺省）

// 2、序号：0X00000001-0X00FFFFFF（即1-16777215，不同分类的序号分别计数）
//------------------------------------------------------------------------------------------
// 以下定义静态指标的ID号，统一格式为：IID_STC_XXXXXXXX
#define		IID_STATIC_BEGIN							DWORD(0X00000000)					// 静态指标的起始ID号

// 以下定义按位
#define		IID_STC_POS_1								DWORD(IID_STATIC_BEGIN+1)				// 第一位号码
#define		IID_STC_POS_2								DWORD(IID_STATIC_BEGIN+2)				// 第二位号码
#define		IID_STC_POS_3								DWORD(IID_STATIC_BEGIN+3)				// 第三位号码
#define		IID_STC_POS_4								DWORD(IID_STATIC_BEGIN+4)				// 第四位号码
#define		IID_STC_POS_5								DWORD(IID_STATIC_BEGIN+5)				// 第五位号码
#define		IID_STC_POS_6								DWORD(IID_STATIC_BEGIN+6)				// 第六位号码
#define		IID_STC_POS_7								DWORD(IID_STATIC_BEGIN+7)				// 第七位号码
#define		IID_STC_POS_8								DWORD(IID_STATIC_BEGIN+8)				// 第八位号码
#define		IID_STC_POS_ANY								DWORD(IID_STATIC_BEGIN+9)				// 任意位号码

// 以下定义和值跨度等
#define		IID_STC_SUM									DWORD(IID_STATIC_BEGIN+10)				// 和值
#define		IID_STC_SUMTAIL								DWORD(IID_STATIC_BEGIN+11)				// 合值
#define		IID_STC_SPAN								DWORD(IID_STATIC_BEGIN+12)				// 跨度
#define		IID_STC_SPANTAIL							DWORD(IID_STATIC_BEGIN+13)				// 跨度尾数
#define		IID_STC_AVG									DWORD(IID_STATIC_BEGIN+14)				// 均值
#define		IID_STC_AVGTAIL								DWORD(IID_STATIC_BEGIN+15)				// 均值尾数
#define		IID_STC_POSMULSUM							DWORD(IID_STATIC_BEGIN+16)				// 位置积和
#define		IID_STC_POSMULSUMTAIL						DWORD(IID_STATIC_BEGIN+17)				// 位置积合
#define		IID_STC_RESPOSMULSUM						DWORD(IID_STATIC_BEGIN+18)				// 反位置积和
#define		IID_STC_RESPOSMULSUMTAIL					DWORD(IID_STATIC_BEGIN+19)				// 反位置积合
#define		IID_STC_AC									DWORD(IID_STATIC_BEGIN+30)				// AC值
#define		IID_STC_DETRACT								DWORD(IID_STATIC_BEGIN+31)				// 散度
#define		IID_STC_SUM_N								DWORD(IID_STATIC_BEGIN+33)				// N次和值
#define		IID_STC_MUL									DWORD(IID_STATIC_BEGIN+40)				// 积数
#define		IID_STC_MUL_ODDEVEN							DWORD(IID_STATIC_BEGIN+41)				// 积数奇偶零
#define		IID_STC_MUL_BIGSMALL						DWORD(IID_STATIC_BEGIN+42)				// 积数大小零
#define		IID_STC_MUL_012								DWORD(IID_STATIC_BEGIN+43)				// 积数012路零
#define		IID_STC_MULTAIL								DWORD(IID_STATIC_BEGIN+49)				// 积数尾数

// 以下定义类型
#define		IID_STC_GENRE_NORMAL						DWORD(IID_STATIC_BEGIN+60)				// 类型
#define		IID_STC_GENRE_TRANS							DWORD(IID_STATIC_BEGIN+61)				// 变换类型
#define		IID_STC_GENRE_TWOLINK						DWORD(IID_STATIC_BEGIN+62)				// 二连码

// 以下定义胆合跨
#define		IID_STC_DHK_HD								DWORD(IID_STATIC_BEGIN+80)				// 合胆
#define		IID_STC_DHK_KD								DWORD(IID_STATIC_BEGIN+81)				// 跨胆
#define		IID_STC_DHK_HK								DWORD(IID_STATIC_BEGIN+82)				// 合跨
#define		IID_STC_DHK_HDKD							DWORD(IID_STATIC_BEGIN+83)				// 合胆跨胆
#define		IID_STC_DHK_HDKDHK							DWORD(IID_STATIC_BEGIN+84)				// 合胆跨胆合跨

// 以下定义号码
#define		IID_STC_NUMS_SIN							DWORD(IID_STATIC_BEGIN+90)				// 单选号码
#define		IID_STC_NUMS_GRO							DWORD(IID_STATIC_BEGIN+91)				// 组选号码

// 以下定义奇偶
#define		IID_STC_ODDENEN_COUNT_O						DWORD(IID_STATIC_BEGIN+100)				// 奇数个数
#define		IID_STC_ODDENEN_COUNT_E						DWORD(IID_STATIC_BEGIN+101)				// 偶数个数
#define		IID_STC_ODDENEN_ARR							DWORD(IID_STATIC_BEGIN+102)				// 奇偶排列
#define		IID_STC_ODDENEN_ARR_VALUE					DWORD(IID_STATIC_BEGIN+103)				// 奇偶排列值
#define		IID_STC_ODDENEN_LINK						DWORD(IID_STATIC_BEGIN+105)				// 奇偶连

// 以下定义大小
#define		IID_STC_BIGSMALL_COUNT_B					DWORD(IID_STATIC_BEGIN+110)				// 大数个数
#define		IID_STC_BIGSMALL_COUNT_S					DWORD(IID_STATIC_BEGIN+111)				// 小数个数
#define		IID_STC_BIGSMALL_ARR						DWORD(IID_STATIC_BEGIN+112)				// 大小排列
#define		IID_STC_BIGSMALL_ARR_VALUE					DWORD(IID_STATIC_BEGIN+113)				// 大小排列值

// 以下定义质合
#define		IID_STC_PRIMECOMPOSITE_COUNT_P				DWORD(IID_STATIC_BEGIN+120)				// 质数个数
#define		IID_STC_PRIMECOMPOSITE_COUNT_C				DWORD(IID_STATIC_BEGIN+121)				// 合数个数
#define		IID_STC_PRIMECOMPOSITE_ARR					DWORD(IID_STATIC_BEGIN+122)				// 质合排列
#define		IID_STC_PRIMECOMPOSITE_ARR_VALUE			DWORD(IID_STATIC_BEGIN+123)				// 质合排列值
#define		IID_STC_PRIMECOMPOSITE_LINK					DWORD(IID_STATIC_BEGIN+125)				// 质合连

// 以下定义012路
#define		IID_STC_012_COUNT_0							DWORD(IID_STATIC_BEGIN+130)				// 0路个数
#define		IID_STC_012_COUNT_1							DWORD(IID_STATIC_BEGIN+131)				// 1路个数
#define		IID_STC_012_COUNT_2							DWORD(IID_STATIC_BEGIN+132)				// 2路个数
#define		IID_STC_012_SHAPE							DWORD(IID_STATIC_BEGIN+133)				// 012路形态
#define		IID_STC_012_ARR_SIN							DWORD(IID_STATIC_BEGIN+134)				// 012路单选排列
#define		IID_STC_012_ARR_SIN_VALUE					DWORD(IID_STATIC_BEGIN+135)				// 012路单选排列值
#define		IID_STC_012_ARR_COUNT						DWORD(IID_STATIC_BEGIN+136)				// 012路个数排列
#define		IID_STC_012_ARR_COUNT_VALUE					DWORD(IID_STATIC_BEGIN+137)				// 012路个数排列值
#define		IID_STC_012_ARR_GRO							DWORD(IID_STATIC_BEGIN+138)				// 012路组选排列
#define		IID_STC_012_ARR_GRO_VALUE					DWORD(IID_STATIC_BEGIN+139)				// 012路组选排列值

// 以下定义 DZX
#define		IID_STC_DZX_COUNT_D							DWORD(IID_STATIC_BEGIN+140)				// D数个数
#define		IID_STC_DZX_COUNT_Z							DWORD(IID_STATIC_BEGIN+141)				// Z数个数
#define		IID_STC_DZX_COUNT_X							DWORD(IID_STATIC_BEGIN+142)				// X数个数
#define		IID_STC_DZX_SHAPE							DWORD(IID_STATIC_BEGIN+143)				// DZX形态
#define		IID_STC_DZX_ARR_SIN							DWORD(IID_STATIC_BEGIN+144)				// DZX单选排列
#define		IID_STC_DZX_ARR_SIN_VALUE					DWORD(IID_STATIC_BEGIN+145)				// DZX单选排列值
#define		IID_STC_DZX_ARR_COUNT						DWORD(IID_STATIC_BEGIN+146)				// DZX个数排列
#define		IID_STC_DZX_ARR_COUNT_VALUE					DWORD(IID_STATIC_BEGIN+147)				// DZX个数排列值
#define		IID_STC_DZX_ARR_GRO							DWORD(IID_STATIC_BEGIN+148)				// DZX组选排列
#define		IID_STC_DZX_ARR_GRO_VALUE					DWORD(IID_STATIC_BEGIN+149)				// DZX组选排列值

// 以下定义特码和
#define		IID_STC_ESPECIALSUM_ODD						DWORD(IID_STATIC_BEGIN+200)				// 奇码和
#define		IID_STC_ESPECIALSUM_EVEN					DWORD(IID_STATIC_BEGIN+201)				// 偶码和
#define		IID_STC_ESPECIALSUM_BIG						DWORD(IID_STATIC_BEGIN+202)				// 大码和
#define		IID_STC_ESPECIALSUM_SMALL					DWORD(IID_STATIC_BEGIN+203)				// 小码和
#define		IID_STC_ESPECIALSUM_PRIME					DWORD(IID_STATIC_BEGIN+204)				// 质码和
#define		IID_STC_ESPECIALSUM_COMPOSITE				DWORD(IID_STATIC_BEGIN+205)				// 合码和
#define		IID_STC_ESPECIALSUM_0						DWORD(IID_STATIC_BEGIN+206)				// 0码和
#define		IID_STC_ESPECIALSUM_1						DWORD(IID_STATIC_BEGIN+207)				// 1码和
#define		IID_STC_ESPECIALSUM_2						DWORD(IID_STATIC_BEGIN+208)				// 2码和
#define		IID_STC_ESPECIALSUM_D						DWORD(IID_STATIC_BEGIN+209)				// D码和
#define		IID_STC_ESPECIALSUM_Z						DWORD(IID_STATIC_BEGIN+210)				// Z码和
#define		IID_STC_ESPECIALSUM_X						DWORD(IID_STATIC_BEGIN+211)				// X码和

// 以下定义二码
#define		IID_STC_TWONUM_ANY							DWORD(IID_STATIC_BEGIN+230)				// 任意二码
#define		IID_STC_TWONUM_Z6							DWORD(IID_STATIC_BEGIN+240)				// 组六分类二码
#define		IID_STC_TWONUM_Z6_2							DWORD(IID_STATIC_BEGIN+241)				// 组六分类二码2
#define		IID_STC_TWONUM_Z3							DWORD(IID_STATIC_BEGIN+250)				// 组三分类二码
#define		IID_STC_TWONUM_Z3_2							DWORD(IID_STATIC_BEGIN+251)				// 组三分类二码2

// 以下定义顺序排列（仅用于3位数字）
#define		IID_STC_ORDER_MIN							DWORD(IID_STATIC_BEGIN+300)				// 最小数
#define		IID_STC_ORDER_MID							DWORD(IID_STATIC_BEGIN+301)				// 中间数
#define		IID_STC_ORDER_MAX							DWORD(IID_STATIC_BEGIN+302)				// 最大数
#define		IID_STC_ORDER_MINADDMID						DWORD(IID_STATIC_BEGIN+310)				// 最小数＋中间数
#define		IID_STC_ORDER_MINADDMAX						DWORD(IID_STATIC_BEGIN+311)				// 最小数＋最大数
#define		IID_STC_ORDER_MIDADDMAX						DWORD(IID_STATIC_BEGIN+312)				// 中间数＋最大数
#define		IID_STC_ORDER_POSSUM_ARR_SIN				DWORD(IID_STATIC_BEGIN+318)				// 顺序位和单选排列
#define		IID_STC_ORDER_POSSUM_ARR_GRO				DWORD(IID_STATIC_BEGIN+319)				// 顺序位和组选排列
#define		IID_STC_ORDER_MINADDMID_TAIL				DWORD(IID_STATIC_BEGIN+320)				// 最小数＋中间数尾数
#define		IID_STC_ORDER_MINADDMAX_TAIL				DWORD(IID_STATIC_BEGIN+321)				// 最小数＋最大数尾数
#define		IID_STC_ORDER_MIDADDMAX_TAIL				DWORD(IID_STATIC_BEGIN+322)				// 中间数＋最大数尾数
#define		IID_STC_ORDER_POSSUMTAIL_ARR_SIN			DWORD(IID_STATIC_BEGIN+328)				// 顺序位合单选排列
#define		IID_STC_ORDER_POSSUMTAIL_ARR_GRO			DWORD(IID_STATIC_BEGIN+329)				// 顺序位合组选排列
#define		IID_STC_ORDER_MIDSUBMIN						DWORD(IID_STATIC_BEGIN+330)				// 中间数－最小数
#define		IID_STC_ORDER_MAXSUBMIN						DWORD(IID_STATIC_BEGIN+331)				// 最大数－最小数
#define		IID_STC_ORDER_MAXSUBMID						DWORD(IID_STATIC_BEGIN+332)				// 最大数－中间数
#define		IID_STC_ORDER_POSSUB_ARR_SIN				DWORD(IID_STATIC_BEGIN+338)				// 顺序位差单选排列
#define		IID_STC_ORDER_POSSUB_ARR_GRO				DWORD(IID_STATIC_BEGIN+339)				// 顺序位差组选排列
#define		IID_STC_ORDER_MIDSUBMIN_TAIL				DWORD(IID_STATIC_BEGIN+340)				// 中间数－最小数尾数
#define		IID_STC_ORDER_MAXSUBMIN_TAIL				DWORD(IID_STATIC_BEGIN+341)				// 最大数－最小数尾数
#define		IID_STC_ORDER_MAXSUBMID_TAIL				DWORD(IID_STATIC_BEGIN+342)				// 中间数＋最大数尾数
#define		IID_STC_ORDER_POSSUBTAIL_ARR_SIN			DWORD(IID_STATIC_BEGIN+348)				// 顺序位差尾数单选排列
#define		IID_STC_ORDER_POSSUBTAIL_ARR_GRO			DWORD(IID_STATIC_BEGIN+349)				// 顺序位差尾数组选排列
#define		IID_STC_ORDER_RELATIVEDZX_POS_1				DWORD(IID_STATIC_BEGIN+390)				// 百位相对大中小
#define		IID_STC_ORDER_RELATIVEDZX_POS_2				DWORD(IID_STATIC_BEGIN+391)				// 十位相对大中小
#define		IID_STC_ORDER_RELATIVEDZX_POS_3				DWORD(IID_STATIC_BEGIN+392)				// 个位相对大中小
#define		IID_STC_ORDER_RELATIVEDZX_ARR				DWORD(IID_STATIC_BEGIN+395)				// 相对大中小排列
#define		IID_STC_ORDER_RELATIVEDZX_ARR_VALUE			DWORD(IID_STATIC_BEGIN+396)				// 相对大中小排列值

// 以下定义间距（仅用于3个号码）
#define		IID_STC_SPACE_SIN							DWORD(IID_STATIC_BEGIN+400)				// 单选间距
#define		IID_STC_SPACETAIL_SIN						DWORD(IID_STATIC_BEGIN+401)				// 单选间距尾数
#define		IID_STC_SPACE_SIN_HEAD						DWORD(IID_STATIC_BEGIN+410)				// 单选首间距
#define		IID_STC_SPACE_SIN_TAIL						DWORD(IID_STATIC_BEGIN+411)				// 单选尾间距
#define		IID_STC_SPACE_SIN_1							DWORD(IID_STATIC_BEGIN+412)				// 单选第1间距
#define		IID_STC_SPACE_SIN_2							DWORD(IID_STATIC_BEGIN+413)				// 单选第2间距
#define		IID_STC_SPACE_SIN_3							DWORD(IID_STATIC_BEGIN+414)				// 单选第3间距
#define		IID_STC_SPACE_SIN_4							DWORD(IID_STATIC_BEGIN+415)				// 单选第4间距
#define		IID_STC_SPACE_SIN_5							DWORD(IID_STATIC_BEGIN+416)				// 单选第5间距
#define		IID_STC_SPACE_SIN_6							DWORD(IID_STATIC_BEGIN+417)				// 单选第6间距
#define		IID_STC_SPACE_SIN_7							DWORD(IID_STATIC_BEGIN+418)				// 单选第7间距
#define		IID_STC_SPACE_SIN_8							DWORD(IID_STATIC_BEGIN+419)				// 单选第8间距
#define		IID_STC_SPACE_GRO							DWORD(IID_STATIC_BEGIN+420)				// 组选间距
#define		IID_STC_SPACE_GRO_HEAD						DWORD(IID_STATIC_BEGIN+430)				// 组选首间距
#define		IID_STC_SPACE_GRO_TAIL						DWORD(IID_STATIC_BEGIN+431)				// 组选尾间距
#define		IID_STC_SPACE_GRO_1							DWORD(IID_STATIC_BEGIN+432)				// 组选第1间距
#define		IID_STC_SPACE_GRO_2							DWORD(IID_STATIC_BEGIN+433)				// 组选第2间距
#define		IID_STC_SPACE_GRO_3							DWORD(IID_STATIC_BEGIN+434)				// 组选第3间距
#define		IID_STC_SPACE_GRO_4							DWORD(IID_STATIC_BEGIN+435)				// 组选第4间距
#define		IID_STC_SPACE_GRO_5							DWORD(IID_STATIC_BEGIN+436)				// 组选第5间距
#define		IID_STC_SPACE_GRO_6							DWORD(IID_STATIC_BEGIN+437)				// 组选第6间距
#define		IID_STC_SPACE_GRO_7							DWORD(IID_STATIC_BEGIN+438)				// 组选第7间距
#define		IID_STC_SPACE_GRO_8							DWORD(IID_STATIC_BEGIN+439)				// 组选第8间距

// 以下定义邻位间距
#define		IID_STC_NEARSPACE_12						DWORD(IID_STATIC_BEGIN+450)				// 12邻位间距
#define		IID_STC_NEARSPACE_23						DWORD(IID_STATIC_BEGIN+451)				// 23邻位间距
#define		IID_STC_NEARSPACE_34						DWORD(IID_STATIC_BEGIN+452)				// 34邻位间距
#define		IID_STC_NEARSPACE_45						DWORD(IID_STATIC_BEGIN+453)				// 45邻位间距
#define		IID_STC_NEARSPACE_56						DWORD(IID_STATIC_BEGIN+454)				// 56邻位间距
#define		IID_STC_NEARSPACE_67						DWORD(IID_STATIC_BEGIN+455)				// 67邻位间距
#define		IID_STC_NEARSPACE_78						DWORD(IID_STATIC_BEGIN+456)				// 78邻位间距
#define		IID_STC_NEARSPACE_HT						DWORD(IID_STATIC_BEGIN+457)				// 首尾邻位间距
#define		IID_STC_NEARSPACE_MAX						DWORD(IID_STATIC_BEGIN+460)				// 最大邻位间距
#define		IID_STC_NEARSPACE_MAXEX						DWORD(IID_STATIC_BEGIN+461)				// 最大邻位间距(含首尾)
#define		IID_STC_NEARSPACE_MIN						DWORD(IID_STATIC_BEGIN+462)				// 最小邻位间距
#define		IID_STC_NEARSPACE_MINEX						DWORD(IID_STATIC_BEGIN+463)				// 最小邻位间距(含首尾)
#define		IID_STC_NEARSPACE_AVG						DWORD(IID_STATIC_BEGIN+464)				// 平均邻位间距
#define		IID_STC_NEARSPACE_ANY						DWORD(IID_STATIC_BEGIN+465)				// 任意邻位间距

// 以下定义1D（仅用于3个号码）
#define		IID_STC_1D_POSITIVE_SIN						DWORD(IID_STATIC_BEGIN+480)				// 单选正1D
#define		IID_STC_1D_NEGATIVE_SIN						DWORD(IID_STATIC_BEGIN+481)				// 单选负1D
#define		IID_STC_1D_RPOSITIVE_SIN					DWORD(IID_STATIC_BEGIN+482)				// 单选反正1D
#define		IID_STC_1D_RNEGATIVE_SIN					DWORD(IID_STATIC_BEGIN+483)				// 单选反负1D
#define		IID_STC_1D_POSITIVE_GRO						DWORD(IID_STATIC_BEGIN+490)				// 组选正1D
#define		IID_STC_1D_NEGATIVE_GRO						DWORD(IID_STATIC_BEGIN+491)				// 组选负1D
#define		IID_STC_1D_RPOSITIVE_GRO					DWORD(IID_STATIC_BEGIN+492)				// 组选反正1D
#define		IID_STC_1D_RNEGATIVE_GRO					DWORD(IID_STATIC_BEGIN+493)				// 组选反负1D

// 以下定义形态
#define		IID_STC_SHAPE_NORMAL						DWORD(IID_STATIC_BEGIN+500)				// 形态
#define		IID_STC_SHAPE_EX							DWORD(IID_STATIC_BEGIN+501)				// 细分形态
#define		IID_STC_SHAPE_EQUALDIFF						DWORD(IID_STATIC_BEGIN+502)				// 等差形
#define		IID_STC_SHAPE_AAB							DWORD(IID_STATIC_BEGIN+503)				// 组三形态
#define		IID_STC_SHAPE_ABC							DWORD(IID_STATIC_BEGIN+504)				// 顺子形态
#define		IID_STC_SHAPE_ABD							DWORD(IID_STATIC_BEGIN+505)				// 半顺形态
#define		IID_STC_SHAPE_COUNT_SAMENUM					DWORD(IID_STATIC_BEGIN+510)				// 相同号码个数
#define		IID_STC_SHAPE_COUNT_SAMEGRO					DWORD(IID_STATIC_BEGIN+511)				// 相同号码组数
#define		IID_STC_SHAPE_COUNT_DIFFNUM					DWORD(IID_STATIC_BEGIN+520)				// 不同号码个数

// 以下定义连号
#define		IID_STC_LINK_NUMCOUNT						DWORD(IID_STATIC_BEGIN+530)				// 连号个数
#define		IID_STC_LINK_GROCOUNT						DWORD(IID_STATIC_BEGIN+531)				// 连号组数
#define		IID_STC_LINK_COMBO							DWORD(IID_STATIC_BEGIN+532)				// 连号组合
#define		IID_STC_LINK_NUMCOUNT_AS					DWORD(IID_STATIC_BEGIN+540)				// 升序连号个数
#define		IID_STC_LINK_GROCOUNT_AS					DWORD(IID_STATIC_BEGIN+541)				// 升序连号组数
#define		IID_STC_LINK_NUMCOUNT_DS					DWORD(IID_STATIC_BEGIN+545)				// 降序连号个数
#define		IID_STC_LINK_GROCOUNT_DS					DWORD(IID_STATIC_BEGIN+546)				// 降序连号组数
#define		IID_STC_LINK_COUNT_ODD						DWORD(IID_STATIC_BEGIN+550)				// 奇连个数
#define		IID_STC_LINK_COUNT_EVEN						DWORD(IID_STATIC_BEGIN+551)				// 偶连个数
#define		IID_STC_LINK_COUNT_PRIME					DWORD(IID_STATIC_BEGIN+552)				// 质连个数
#define		IID_STC_LINK_COUNT_COMPOSITE				DWORD(IID_STATIC_BEGIN+553)				// 合连个数
#define		IID_STC_LINK_POS_2							DWORD(IID_STATIC_BEGIN+560)				// 2连号位置
#define		IID_STC_LINK_POS_3							DWORD(IID_STATIC_BEGIN+561)				// 3连号位置
#define		IID_STC_LINK_POS_4							DWORD(IID_STATIC_BEGIN+562)				// 4连号位置
#define		IID_STC_LINK_POS_5							DWORD(IID_STATIC_BEGIN+563)				// 5连号位置
#define		IID_STC_LINK_POS_6							DWORD(IID_STATIC_BEGIN+564)				// 6连号位置
#define		IID_STC_LINK_POS_7							DWORD(IID_STATIC_BEGIN+565)				// 7连号位置

// 以下定义数字个数等（90个）
#define		IID_STC_NUMBER_0NUM							DWORD(IID_STATIC_BEGIN+600)				// 数字0个数
// .........
#define		IID_STC_NUMBER_SAMECOMBO					DWORD(IID_STATIC_BEGIN+690)				// 相同数字组合
#define		IID_STC_NUMBER_MAXCOUNT						DWORD(IID_STATIC_BEGIN+691)				// 任意数字最大出现个数
#define		IID_STC_NUMBER_SAMEGROUP_2					DWORD(IID_STATIC_BEGIN+692)				// 2个相同数字组数
#define		IID_STC_NUMBER_SAMEGROUP_3					DWORD(IID_STATIC_BEGIN+693)				// 3个相同数字组数
#define		IID_STC_NUMBER_SAMEGROUP_4					DWORD(IID_STATIC_BEGIN+694)				// 4个相同数字组数
#define		IID_STC_NUMBER_SAMEGROUP_5					DWORD(IID_STATIC_BEGIN+695)				// 5个相同数字组数
#define		IID_STC_NUMBER_SAMEGROUP_6					DWORD(IID_STATIC_BEGIN+696)				// 6个相同数字组数
#define		IID_STC_NUMBER_SAMEGROUP_7					DWORD(IID_STATIC_BEGIN+697)				// 7个相同数字组数
#define		IID_STC_NUMBER_SAMEGROUP_8					DWORD(IID_STATIC_BEGIN+698)				// 8个相同数字组数

// 以下定义两位关系（ID号的十位与个位确定两位位置（0：第一位，1：第二位，以此类推），千百位：类型，见TWOPOS_TYPE定义）
#define		IID_STC_TWOPOS_BEGIN						DWORD(IID_STATIC_BEGIN+1000)				// 两位关系开始
#define		IID_STC_TWOPOS_MN							DWORD(IID_STATIC_BEGIN+1000)				// 指定两位关系
#define		IID_STC_TWOPOS_ANY							DWORD(IID_STATIC_BEGIN+1091)				// 任意两位关系
#define		IID_STC_TWOPOS_MIN							DWORD(IID_STATIC_BEGIN+1092)				// 最小两位关系
#define		IID_STC_TWOPOS_MID							DWORD(IID_STATIC_BEGIN+1093)				// 中间两位关系（仅用于3个号码）
#define		IID_STC_TWOPOS_MAX							DWORD(IID_STATIC_BEGIN+1094)				// 最大两位关系
#define		IID_STC_TWOPOS_ARR_SIN						DWORD(IID_STATIC_BEGIN+1095)				// 两位关系单排
#define		IID_STC_TWOPOS_ARR_GRO						DWORD(IID_STATIC_BEGIN+1096)				// 两位关系组排
#define		IID_STC_TWOPOS_HEADTAIL						DWORD(IID_STATIC_BEGIN+1097)				// 首尾两位关系
#define		IID_STC_TWOPOS_END							DWORD(IID_STATIC_BEGIN+2999)				// 两位关系结束

// 以下定义单选二码（ID号的十位与个位确定两位位置（0：第一位，1：第二位，以此类推）)
#define		IID_STC_TWONUMSIN_BEGIN						DWORD(IID_STATIC_BEGIN+3000)				// 单选二码开始
#define		IID_STC_TWONUMSIN_MN						DWORD(IID_STC_TWONUMSIN_BEGIN)				// 单选二码（指定位置）
#define		IID_STC_TWONUMSIN_ANY						DWORD(IID_STC_TWONUMSIN_BEGIN+90)			// 任意单选二码
#define		IID_STC_TWONUMSIN_END						DWORD(IID_STC_TWONUMSIN_BEGIN+99)			// 单选二码结束

// 以下定义组合
#define		IID_STC_SINCOMBIN_ODDEVENBIGSMALL			DWORD(IID_STATIC_BEGIN+9000)				// 单选奇偶大小
#define		IID_STC_SINCOMBIN_ODDEVENPRIMEHE			DWORD(IID_STATIC_BEGIN+9001)				// 单选奇偶质合
#define		IID_STC_SINCOMBIN_BIGSMALLPRIMEHE			DWORD(IID_STATIC_BEGIN+9002)				// 单选大小质合
#define		IID_STC_SINCOMBIN_012DZX					DWORD(IID_STATIC_BEGIN+9003)				// 单选012大中小
#define		IID_STC_GROCOMBIN_ODDEVENBIGSMALL			DWORD(IID_STATIC_BEGIN+9010)				// 组选奇偶大小
#define		IID_STC_GROCOMBIN_ODDEVENPRIMEHE			DWORD(IID_STATIC_BEGIN+9011)				// 组选奇偶质合
#define		IID_STC_GROCOMBIN_BIGSMALLPRIMEHE			DWORD(IID_STATIC_BEGIN+9012)				// 组选大小质合
#define		IID_STC_GROCOMBIN_012DZX					DWORD(IID_STATIC_BEGIN+9013)				// 组选012大中小

// 以下定义除M余数（M：2-20，由千位和百位确定，0为除2，1为除3，以此类推）
#define		IID_STC_REMM_BEGIN							DWORD(IID_STATIC_BEGIN+10000)			// 余数开始
#define		IID_STC_REMM_POS_1							DWORD(IID_STATIC_BEGIN+10001)			// 第一位除M余数
#define		IID_STC_REMM_POS_2							DWORD(IID_STATIC_BEGIN+10002)			// 第二位除M余数
#define		IID_STC_REMM_POS_3							DWORD(IID_STATIC_BEGIN+10003)			// 第三位除M余数
#define		IID_STC_REMM_POS_4							DWORD(IID_STATIC_BEGIN+10004)			// 第四位除M余数
#define		IID_STC_REMM_POS_5							DWORD(IID_STATIC_BEGIN+10005)			// 第五位除M余数
#define		IID_STC_REMM_POS_6							DWORD(IID_STATIC_BEGIN+10006)			// 第六位除M余数
#define		IID_STC_REMM_POS_7							DWORD(IID_STATIC_BEGIN+10007)			// 第七位除M余数
#define		IID_STC_REMM_POS_8							DWORD(IID_STATIC_BEGIN+10008)			// 第八位除M余数
#define		IID_STC_REMM_POS_ANY						DWORD(IID_STATIC_BEGIN+10009)			// 任意位除M余数
#define		IID_STC_REMM_SUM							DWORD(IID_STATIC_BEGIN+10010)			// 除M余数和
#define		IID_STC_REMM_SUMTAIL						DWORD(IID_STATIC_BEGIN+10011)			// 除M余数合
#define		IID_STC_REMM_ARR_SIN						DWORD(IID_STATIC_BEGIN+10020)			// 除M余数单选排列
#define		IID_STC_REMM_ARR_SIN_VALUE					DWORD(IID_STATIC_BEGIN+10021)			// 除M余数单选排列值
#define		IID_STC_REMM_ARR_GRO						DWORD(IID_STATIC_BEGIN+10022)			// 除M余数组选排列
#define		IID_STC_REMM_ARR_GRO_VALUE					DWORD(IID_STATIC_BEGIN+10023)			// 除M余数组选排列值
#define		IID_STC_REMM_ARR_COUNT						DWORD(IID_STATIC_BEGIN+10025)			// 除M余数个数排列
#define		IID_STC_REMM_ARR_COUNT_VALUE				DWORD(IID_STATIC_BEGIN+10026)			// 除M余数个数排列值
#define		IID_STC_REMM_MAXLINK						DWORD(IID_STATIC_BEGIN+10030)			// 除M余数最大邻出
#define		IID_STC_REMM_MAXNONE						DWORD(IID_STATIC_BEGIN+10031)			// 除M余数最大邻断
#define		IID_STC_REMM_COUNT_NONE						DWORD(IID_STATIC_BEGIN+10040)			// 除M余数未出个数
#define		IID_STC_REMM_COUNT_MATCH					DWORD(IID_STATIC_BEGIN+10041)			// 除M余数出现个数
#define		IID_STC_REMM_SHAPE							DWORD(IID_STATIC_BEGIN+10069)			// 除M余数形态（只用于除3余数）
#define		IID_STC_REMM_COUNT_N						DWORD(IID_STATIC_BEGIN+10070)			// 除M余N个数（N：0-19）
#define		IID_STC_REMM_END							DWORD(IID_STATIC_BEGIN+11999)			// 余数结束

//------------------------------------------------------------------------------------------
// 以下定义动态指标的ID号，统一格式为：IID_DNC_XXXXXXXX
#define		IID_DYNAMIC_BEGIN							DWORD(0X80000000)						// 动态指标的起始ID号

// 以下定义距离（兼容旧版本用，新版本使用邻位关系，2010-09-29）
#define		IID_DNC_DIS_POS_1							DWORD(IID_DYNAMIC_BEGIN+1)				// 第一位距离
#define		IID_DNC_DIS_POS_2							DWORD(IID_DYNAMIC_BEGIN+2)				// 第二位距离
#define		IID_DNC_DIS_POS_3							DWORD(IID_DYNAMIC_BEGIN+3)				// 第三位距离
#define		IID_DNC_DIS_POS_4							DWORD(IID_DYNAMIC_BEGIN+4)				// 第四位距离
#define		IID_DNC_DIS_POS_5							DWORD(IID_DYNAMIC_BEGIN+5)				// 第五位距离
#define		IID_DNC_DIS_POS_6							DWORD(IID_DYNAMIC_BEGIN+6)				// 第六位距离
#define		IID_DNC_DIS_POS_7							DWORD(IID_DYNAMIC_BEGIN+7)				// 第七位距离
#define		IID_DNC_DIS_POS_8							DWORD(IID_DYNAMIC_BEGIN+8)				// 第八位距离
#define		IID_DNC_DIS_POS_ANY							DWORD(IID_DYNAMIC_BEGIN+9)				// 任意位号码距离
#define		IID_DNC_DIS_SUM								DWORD(IID_DYNAMIC_BEGIN+10)				// 号码距离和
#define		IID_DNC_DIS_SUMTAIL							DWORD(IID_DYNAMIC_BEGIN+11)				// 号码距离合
#define		IID_DNC_DIS_ARR_SIN							DWORD(IID_DYNAMIC_BEGIN+12)				// 号码距离单选排列
#define		IID_DNC_DIS_ARR_GRO							DWORD(IID_DYNAMIC_BEGIN+13)				// 号码距离组选排列

// 以下定义斜连值
#define		IID_DNC_XL_LEFT								DWORD(IID_DYNAMIC_BEGIN+30)				// 左斜连
#define		IID_DNC_XL_RIGHT							DWORD(IID_DYNAMIC_BEGIN+31)				// 右斜连
#define		IID_DNC_XL_VALUE							DWORD(IID_DYNAMIC_BEGIN+32)				// 斜连值
#define		IID_DNC_XL_COMBO							DWORD(IID_DYNAMIC_BEGIN+33)				// 左右斜连组合

#define		IID_DNC_DEPARTURE							DWORD(IID_DYNAMIC_BEGIN+40)				// 偏度

// 以下定义前N期重复号（N为1-10）
#define		IID_DNC_REPEAT_1TON							DWORD(IID_DYNAMIC_BEGIN+50)				// 前N期重复号

// 以下定义前第N期重复号（N为1-10）
#define		IID_DNC_REPEAT_N							DWORD(IID_DYNAMIC_BEGIN+60)				// 前第N期重复号

// 以下定义号码出现次数
#define		IID_DNC_NUMS_MATCHCOUNT_SIN					DWORD(IID_DYNAMIC_BEGIN+70)				// 单选号码出现次数
#define		IID_DNC_NUMS_MATCHCOUNT_GRO					DWORD(IID_DYNAMIC_BEGIN+71)				// 组选号码出现次数

// 以下定义试机号相关
#define		IID_DNC_TESTNUMS_REPEAT_CUR_CUR				DWORD(IID_DYNAMIC_BEGIN+80)				// 当期试机号与当期开奖号重复个数（组选）
#define		IID_DNC_TESTNUMS_REPEAT_PRE_CUR				DWORD(IID_DYNAMIC_BEGIN+81)				// 上期试机号与当期开奖号重复个数（组选）
#define		IID_DNC_TESTNUMS_REPEAT_COMPLEX				DWORD(IID_DYNAMIC_BEGIN+82)				// 与当期试机号和开奖号综合重复信息
#define		IID_DNC_TESTNUMS_REPEAT_COMBO				DWORD(IID_DYNAMIC_BEGIN+83)				// 与当期试机号和开奖号重复组合

// 以下定义单选遗漏
#define		IID_DNC_SKIPSIN_1							DWORD(IID_DYNAMIC_BEGIN+200)				// 第一位单选遗漏
#define		IID_DNC_SKIPSIN_2							DWORD(IID_DYNAMIC_BEGIN+201)				// 第二位单选遗漏
#define		IID_DNC_SKIPSIN_3							DWORD(IID_DYNAMIC_BEGIN+202)				// 第三位单选遗漏
#define		IID_DNC_SKIPSIN_4							DWORD(IID_DYNAMIC_BEGIN+203)				// 第四位单选遗漏
#define		IID_DNC_SKIPSIN_5							DWORD(IID_DYNAMIC_BEGIN+204)				// 第五位单选遗漏
#define		IID_DNC_SKIPSIN_6							DWORD(IID_DYNAMIC_BEGIN+205)				// 第六位单选遗漏
#define		IID_DNC_SKIPSIN_7							DWORD(IID_DYNAMIC_BEGIN+206)				// 第七位单选遗漏
#define		IID_DNC_SKIPSIN_8							DWORD(IID_DYNAMIC_BEGIN+207)				// 第八位单选遗漏
#define		IID_DNC_SKIPSIN_MIN							DWORD(IID_DYNAMIC_BEGIN+208)				// 最小单选遗漏
#define		IID_DNC_SKIPSIN_MAX							DWORD(IID_DYNAMIC_BEGIN+209)				// 最大单选遗漏
#define		IID_DNC_SKIPSIN_TAIL_1						DWORD(IID_DYNAMIC_BEGIN+210)				// 第一位单选遗漏尾
#define		IID_DNC_SKIPSIN_TAIL_2						DWORD(IID_DYNAMIC_BEGIN+211)				// 第二位单选遗漏尾
#define		IID_DNC_SKIPSIN_TAIL_3						DWORD(IID_DYNAMIC_BEGIN+212)				// 第三位单选遗漏尾
#define		IID_DNC_SKIPSIN_TAIL_4						DWORD(IID_DYNAMIC_BEGIN+213)				// 第四位单选遗漏尾
#define		IID_DNC_SKIPSIN_TAIL_5						DWORD(IID_DYNAMIC_BEGIN+214)				// 第五位单选遗漏尾
#define		IID_DNC_SKIPSIN_TAIL_6						DWORD(IID_DYNAMIC_BEGIN+215)				// 第六位单选遗漏尾
#define		IID_DNC_SKIPSIN_TAIL_7						DWORD(IID_DYNAMIC_BEGIN+216)				// 第七位单选遗漏尾
#define		IID_DNC_SKIPSIN_TAIL_8						DWORD(IID_DYNAMIC_BEGIN+217)				// 第八位单选遗漏尾
#define		IID_DNC_SKIPSIN_TAIL_MIN					DWORD(IID_DYNAMIC_BEGIN+218)				// 最小单选遗漏尾
#define		IID_DNC_SKIPSIN_TAIL_MAX					DWORD(IID_DYNAMIC_BEGIN+219)				// 最大单选遗漏尾
#define		IID_DNC_SKIPSIN_TAIL_ANY					DWORD(IID_DYNAMIC_BEGIN+220)				// 任意单选遗漏尾
#define		IID_DNC_SKIPSIN_TAIL_ARR_SIN				DWORD(IID_DYNAMIC_BEGIN+225)				// 单选遗漏尾单选排列
#define		IID_DNC_SKIPSIN_TAIL_ARR_GRO				DWORD(IID_DYNAMIC_BEGIN+226)				// 单选遗漏尾组选排列
#define		IID_DNC_SKIPSIN_SUM							DWORD(IID_DYNAMIC_BEGIN+230)				// 单选遗漏和值
#define		IID_DNC_SKIPSIN_SUMTAIL						DWORD(IID_DYNAMIC_BEGIN+231)				// 单选遗漏合值
#define		IID_DNC_SKIPSIN_SUM_AREA					DWORD(IID_DYNAMIC_BEGIN+238)				// 单选遗漏和值分区（用于标准版）
#define		IID_DNC_SKIPSIN_SUM_AREA10					DWORD(IID_DYNAMIC_BEGIN+239)				// 单选遗漏和值10分区（用于标准版）

// 以下定义组选遗漏
#define		IID_DNC_SKIPGRO_1							DWORD(IID_DYNAMIC_BEGIN+250)				// 第一位组选遗漏
#define		IID_DNC_SKIPGRO_2							DWORD(IID_DYNAMIC_BEGIN+251)				// 第二位组选遗漏
#define		IID_DNC_SKIPGRO_3							DWORD(IID_DYNAMIC_BEGIN+252)				// 第三位组选遗漏
#define		IID_DNC_SKIPGRO_4							DWORD(IID_DYNAMIC_BEGIN+253)				// 第四位组选遗漏
#define		IID_DNC_SKIPGRO_5							DWORD(IID_DYNAMIC_BEGIN+254)				// 第五位组选遗漏
#define		IID_DNC_SKIPGRO_6							DWORD(IID_DYNAMIC_BEGIN+255)				// 第六位组选遗漏
#define		IID_DNC_SKIPGRO_7							DWORD(IID_DYNAMIC_BEGIN+256)				// 第七位组选遗漏
#define		IID_DNC_SKIPGRO_8							DWORD(IID_DYNAMIC_BEGIN+257)				// 第八位组选遗漏
#define		IID_DNC_SKIPGRO_MIN							DWORD(IID_DYNAMIC_BEGIN+258)				// 最小组选遗漏
#define		IID_DNC_SKIPGRO_MAX							DWORD(IID_DYNAMIC_BEGIN+259)				// 最大组选遗漏
#define		IID_DNC_SKIPGRO_TAIL_1						DWORD(IID_DYNAMIC_BEGIN+260)				// 第一位组选遗漏尾
#define		IID_DNC_SKIPGRO_TAIL_2						DWORD(IID_DYNAMIC_BEGIN+261)				// 第二位组选遗漏尾
#define		IID_DNC_SKIPGRO_TAIL_3						DWORD(IID_DYNAMIC_BEGIN+262)				// 第三位组选遗漏尾
#define		IID_DNC_SKIPGRO_TAIL_4						DWORD(IID_DYNAMIC_BEGIN+263)				// 第四位组选遗漏尾
#define		IID_DNC_SKIPGRO_TAIL_5						DWORD(IID_DYNAMIC_BEGIN+264)				// 第五位组选遗漏尾
#define		IID_DNC_SKIPGRO_TAIL_6						DWORD(IID_DYNAMIC_BEGIN+265)				// 第六位组选遗漏尾
#define		IID_DNC_SKIPGRO_TAIL_7						DWORD(IID_DYNAMIC_BEGIN+266)				// 第七位组选遗漏尾
#define		IID_DNC_SKIPGRO_TAIL_8						DWORD(IID_DYNAMIC_BEGIN+267)				// 第八位组选遗漏尾
#define		IID_DNC_SKIPGRO_TAIL_MIN					DWORD(IID_DYNAMIC_BEGIN+268)				// 最小组选遗漏尾
#define		IID_DNC_SKIPGRO_TAIL_MAX					DWORD(IID_DYNAMIC_BEGIN+269)				// 最大组选遗漏尾
#define		IID_DNC_SKIPGRO_TAIL_ANY					DWORD(IID_DYNAMIC_BEGIN+270)				// 任意组选遗漏尾
#define		IID_DNC_SKIPGRO_TAIL_ARR_SIN				DWORD(IID_DYNAMIC_BEGIN+275)				// 组选遗漏尾单选排列
#define		IID_DNC_SKIPGRO_TAIL_ARR_GRO				DWORD(IID_DYNAMIC_BEGIN+276)				// 组选遗漏尾组选排列
#define		IID_DNC_SKIPGRO_SUM							DWORD(IID_DYNAMIC_BEGIN+280)				// 组选遗漏和值
#define		IID_DNC_SKIPGRO_SUMTAIL						DWORD(IID_DYNAMIC_BEGIN+281)				// 组选遗漏合值
#define		IID_DNC_SKIPGRO_SUM_AREA					DWORD(IID_DYNAMIC_BEGIN+288)				// 组选遗漏和值分区（用于标准版）
#define		IID_DNC_SKIPGRO_SUM_AREA10					DWORD(IID_DYNAMIC_BEGIN+289)				// 组选遗漏和值10分区（用于标准版）

// 以下定义动态3态指标（百位为3态名字，具体见DYNAMIC3STATE_TYPE定义）
#define		IID_DNC_XXX_BEGIN							DWORD(IID_DYNAMIC_BEGIN+10000)			// 3态指标开始
#define		IID_DNC_XXX_POS_1							DWORD(IID_DNC_XXX_BEGIN+1)				// 第一位3态指标
#define		IID_DNC_XXX_POS_2							DWORD(IID_DNC_XXX_BEGIN+2)				// 第二位3态指标
#define		IID_DNC_XXX_POS_3							DWORD(IID_DNC_XXX_BEGIN+3)				// 第三位3态指标
#define		IID_DNC_XXX_POS_4							DWORD(IID_DNC_XXX_BEGIN+4)				// 第四位3态指标
#define		IID_DNC_XXX_POS_5							DWORD(IID_DNC_XXX_BEGIN+5)				// 第五位3态指标
#define		IID_DNC_XXX_POS_6							DWORD(IID_DNC_XXX_BEGIN+6)				// 第六位3态指标
#define		IID_DNC_XXX_POS_7							DWORD(IID_DNC_XXX_BEGIN+7)				// 第七位3态指标
#define		IID_DNC_XXX_POS_8							DWORD(IID_DNC_XXX_BEGIN+8)				// 第八位3态指标
#define		IID_DNC_XXX_COUNT_0							DWORD(IID_DNC_XXX_BEGIN+10)				// 3态指标0个数
#define		IID_DNC_XXX_COUNT_1							DWORD(IID_DNC_XXX_BEGIN+11)				// 3态指标1个数
#define		IID_DNC_XXX_COUNT_2							DWORD(IID_DNC_XXX_BEGIN+12)				// 3态指标2个数
#define		IID_DNC_XXX_SHAPE							DWORD(IID_DNC_XXX_BEGIN+13)				// 3态指标形态
#define		IID_DNC_XXX_ARR_SIN							DWORD(IID_DNC_XXX_BEGIN+14)				// 3态指标单选排列
#define		IID_DNC_XXX_ARR_SIN_VALUE					DWORD(IID_DNC_XXX_BEGIN+15)				// 3态指标单选排列值
#define		IID_DNC_XXX_ARR_COUNT						DWORD(IID_DNC_XXX_BEGIN+16)				// 3态指标个数排列
#define		IID_DNC_XXX_ARR_COUNT_VALUE					DWORD(IID_DNC_XXX_BEGIN+17)				// 3态指标个数排列值
#define		IID_DNC_XXX_ARR_GRO							DWORD(IID_DNC_XXX_BEGIN+18)				// 3态指标组选排列
#define		IID_DNC_XXX_ARR_GRO_VALUE					DWORD(IID_DNC_XXX_BEGIN+19)				// 3态指标组选排列值
#define		IID_DNC_XXX_END								DWORD(IID_DYNAMIC_BEGIN+10999)			// 3态指标结束

// 以下定义邻期关系指标，如："邻期和", "邻期合", "邻期差","距离", "距离尾", "环距", "环距尾", "轴差", "轴距", "轴距尾"指标
// （千百位：类型，见PREVDATA_TYPE定义，ID号的十位与个位确定具体指标，定义如下）
#define		IID_DNC_PREVDATA_BEGIN						DWORD(IID_DYNAMIC_BEGIN+11000)			// 邻期关系指标开始
#define		IID_DNC_PREVDATA_POS_1						DWORD(IID_DNC_PREVDATA_BEGIN+1)			// 第一位邻期关系
#define		IID_DNC_PREVDATA_POS_2						DWORD(IID_DNC_PREVDATA_BEGIN+2)			// 第二位邻期关系
#define		IID_DNC_PREVDATA_POS_3						DWORD(IID_DNC_PREVDATA_BEGIN+3)			// 第三位邻期关系
#define		IID_DNC_PREVDATA_POS_4						DWORD(IID_DNC_PREVDATA_BEGIN+4)			// 第四位邻期关系
#define		IID_DNC_PREVDATA_POS_5						DWORD(IID_DNC_PREVDATA_BEGIN+5)			// 第五位邻期关系
#define		IID_DNC_PREVDATA_POS_6						DWORD(IID_DNC_PREVDATA_BEGIN+6)			// 第六位邻期关系
#define		IID_DNC_PREVDATA_POS_7						DWORD(IID_DNC_PREVDATA_BEGIN+7)			// 第七位邻期关系
#define		IID_DNC_PREVDATA_POS_8						DWORD(IID_DNC_PREVDATA_BEGIN+8)			// 第八位邻期关系
#define		IID_DNC_PREVDATA_ANY						DWORD(IID_DNC_PREVDATA_BEGIN+9)			// 任意邻期关系
#define		IID_DNC_PREVDATA_MIN						DWORD(IID_DNC_PREVDATA_BEGIN+10)		// 最小邻期关系
#define		IID_DNC_PREVDATA_MID						DWORD(IID_DNC_PREVDATA_BEGIN+11)		// 中间邻期关系
#define		IID_DNC_PREVDATA_MAX						DWORD(IID_DNC_PREVDATA_BEGIN+12)		// 最大邻期关系
#define		IID_DNC_PREVDATA_SUM						DWORD(IID_DNC_PREVDATA_BEGIN+20)		// 邻期关系和
#define		IID_DNC_PREVDATA_SUMTAIL					DWORD(IID_DNC_PREVDATA_BEGIN+21)		// 邻期关系合
#define		IID_DNC_PREVDATA_ARR_SIN					DWORD(IID_DNC_PREVDATA_BEGIN+22)		// 邻期关系单选排列
#define		IID_DNC_PREVDATA_ARR_GRO					DWORD(IID_DNC_PREVDATA_BEGIN+23)		// 邻期关系组选排列
#define		IID_DNC_PREVDATA_END						DWORD(IID_DYNAMIC_BEGIN+11999)			// 邻期关系指标结束

//------------------------------------------------------------------------------------------
// 以下定义历史特定指标的ID号，统一格式为：IID_HIS_XXXXXXXX
#define		IID_HISTORY_BEGIN							DWORD(0XFFFF0000)						// 历史特定指标的起始ID号
#define		IID_HIS_NUMS_ARR_SIN						DWORD(IID_HISTORY_BEGIN+1)				// 号码单选排列
#define		IID_HIS_NUMS_ARR_GRO						DWORD(IID_HISTORY_BEGIN+2)				// 号码组选排列
#define		IID_HIS_NUMS_1_ARR_SIN						DWORD(IID_HISTORY_BEGIN+10)				// 单选单码
#define		IID_HIS_NUMS_1_ARR_GRO						DWORD(IID_HISTORY_BEGIN+11)				// 组选单码
#define		IID_HIS_NUMS_2_ARR_SIN						DWORD(IID_HISTORY_BEGIN+20)				// 单选两码
#define		IID_HIS_NUMS_2_ARR_GRO						DWORD(IID_HISTORY_BEGIN+21)				// 组选两码

// =========================================================================================

// =========================================================================================
// 以下定义系统指标参数的ID号，如下：
// 1、分类（最高字节）：定义如下：（目前不再使用）
// #define		IPID_TYPE_DYNAMIC							DWORD(0X80000000)						// 动态指标参数，否则为静态指标参数

// 2、序号：0X00000001-0X00FFFFFF（即1-16777215，不同分类的序号分别计数）
//------------------------------------------------------------------------------------------

#define		IPID_NONE									DWORD(0)								// 无指标参数

//------------------------------------------------------------------------------------------
// 以下定义静态指标参数的ID号，统一格式为：IPID_S_XXXXXXXX
#define		IPID_STATIC_BEGIN							DWORD(0X00000000)						// 静态指标参数的起始ID号
#define		IPID_S_ODDEVEN								DWORD(IPID_STATIC_BEGIN+1)				// 奇偶
#define		IPID_S_BIGSMALL								DWORD(IPID_STATIC_BEGIN+2)				// 大小
#define		IPID_S_DZX									DWORD(IPID_STATIC_BEGIN+3)				// 大中小
#define		IPID_S_012									DWORD(IPID_STATIC_BEGIN+4)				// 012
#define		IPID_S_PRIMECOMPOSITE						DWORD(IPID_STATIC_BEGIN+5)				// 质合
#define		IPID_S_TAIL									DWORD(IPID_STATIC_BEGIN+6)				// 尾数
#define		IPID_S_ABS									DWORD(IPID_STATIC_BEGIN+7)				// 绝对值
#define		IPID_S_ABS_TAIL								DWORD(IPID_STATIC_BEGIN+8)				// 绝对值尾数

#define		IPID_S_REMAINDER_0							DWORD(IPID_STATIC_BEGIN+32)				// 除0余数，也即除N余数的开始，需要在后面加N表示除N余数
// ......                                        												// 从除2到16余数
#define		IPID_S_REMAINDER_MAX						DWORD(IPID_STATIC_BEGIN+48)				// 最大余数，目前最大只支持除16余数

// 这中间的值保留

#define		IPID_S_SUBAREA_0							DWORD(IPID_STATIC_BEGIN+47)				// 0分区，也即分区的开始，需要在后面加N表示分区
// ......                                           											// 从除2分区到16分区
#define		IPID_S_SUBAREA_MAX							DWORD(IPID_STATIC_BEGIN+63)				// 最大分区，目前只支持15个分区(2-16)

// 以下用于多值条件的扩展
#define		IPID_S_SUM									DWORD(IPID_STATIC_BEGIN+100)			// 和值
#define		IPID_S_SUM_TAIL								DWORD(IPID_STATIC_BEGIN+101)			// 合值
#define		IPID_S_SUM_N								DWORD(IPID_STATIC_BEGIN+102)			// N次和
#define		IPID_S_SPAN									DWORD(IPID_STATIC_BEGIN+110)			// 跨度
#define		IPID_S_SPAN_TAIL							DWORD(IPID_STATIC_BEGIN+111)			// 跨度尾数
#define		IPID_S_AVG									DWORD(IPID_STATIC_BEGIN+120)			// 均值
#define		IPID_S_AVG_TAIL								DWORD(IPID_STATIC_BEGIN+121)			// 均值尾数
#define		IPID_S_MIN									DWORD(IPID_STATIC_BEGIN+130)			// 最小值
#define		IPID_S_MIN_TAIL								DWORD(IPID_STATIC_BEGIN+131)			// 最小值尾数
#define		IPID_S_MID									DWORD(IPID_STATIC_BEGIN+134)			// 中间值
#define		IPID_S_MID_TAIL								DWORD(IPID_STATIC_BEGIN+135)			// 中间值尾数
#define		IPID_S_MAX									DWORD(IPID_STATIC_BEGIN+137)			// 最大值
#define		IPID_S_MAX_TAIL								DWORD(IPID_STATIC_BEGIN+138)			// 最大值尾数

//------------------------------------------------------------------------------------------
// 以下定义动态指标参数的ID号，统一格式为：IPID_D_XXXXXXXX
#define		IPID_DYNAMIC_BEGIN							DWORD(0X80000000)						// 动态指标参数的起始ID号
#define		IPID_D_DISTANCE								DWORD(IPID_DYNAMIC_BEGIN+1)				// 距离（邻期差绝对值）
#define		IPID_D_DISTANCE_TAIL						DWORD(IPID_DYNAMIC_BEGIN+2)				// 距离（邻期差绝对值）尾数
#define		IPID_D_CIRCLE_DISTANCE						DWORD(IPID_DYNAMIC_BEGIN+3)				// 环距（邻期环差）
#define		IPID_D_CIRCLE_DISTANCE_TAIL					DWORD(IPID_DYNAMIC_BEGIN+4)				// 环距（邻期环差）尾数
#define		IPID_D_ADJACENT_SUB							DWORD(IPID_DYNAMIC_BEGIN+5)				// 邻期差
#define		IPID_D_ADJACENT_SUM							DWORD(IPID_DYNAMIC_BEGIN+6)				// 邻期和
#define		IPID_D_ADJACENT_SUMTAIL						DWORD(IPID_DYNAMIC_BEGIN+7)				// 邻期合
#define		IPID_D_SPJ									DWORD(IPID_DYNAMIC_BEGIN+8)				// 升平降
#define		IPID_D_AXIS_SUB								DWORD(IPID_DYNAMIC_BEGIN+9)				// 轴差
#define		IPID_D_AXIS_DISTANCE						DWORD(IPID_DYNAMIC_BEGIN+10)				// 轴距
#define		IPID_D_AXIS_DISTANCE_TAIL					DWORD(IPID_DYNAMIC_BEGIN+11)				// 轴距尾

#define		IPID_D_SKIP									DWORD(IPID_DYNAMIC_BEGIN+20)				// 遗漏
#define		IPID_D_SKIP_TAIL							DWORD(IPID_DYNAMIC_BEGIN+21)				// 遗漏尾

#define		IPID_D_TESTNUMS_BEGIN						DWORD(IPID_DYNAMIC_BEGIN+30)				// 开奖号与试机号参数开始（不直接使用）
#define		IPID_D_TESTNUMS_COMPARE						DWORD(IPID_DYNAMIC_BEGIN+30)				// 开奖号与试机号对照
#define		IPID_D_TESTNUMS_DISTANCE					DWORD(IPID_DYNAMIC_BEGIN+31)				// 开奖号与试机号距离
#define		IPID_D_TESTNUMS_DISTANCE_TAIL				DWORD(IPID_DYNAMIC_BEGIN+32)				// 开奖号与试机号距离尾数
#define		IPID_D_TESTNUMS_SUB							DWORD(IPID_DYNAMIC_BEGIN+33)				// 开奖号与试机号之差
#define		IPID_D_TESTNUMS_CIRSUB						DWORD(IPID_DYNAMIC_BEGIN+34)				// 开奖号与试机号环距
#define		IPID_D_TESTNUMS_CIRSUB_TAIL					DWORD(IPID_DYNAMIC_BEGIN+35)				// 开奖号与试机号环距尾数
#define		IPID_D_TESTNUMS_END							DWORD(IPID_DYNAMIC_BEGIN+39)				// 开奖号与试机号参数结束（不直接使用）

// =========================================================================================

//------------------------------------------------------------------------------------------
// 以下定义复合条件相关ID号
// 低字为0X0001-0X0FFF，为特定的缩水条件，0X1001-0X1FFF为固定分组条件，0X2001-0X2FFF为我的条件分组
//------------------------------------------------------------------------------------------
// 以下定义特定的缩水条件
#define		CDTID_FLT_GALL								0X00000001								// 胆码
#define		CDTID_FLT_NUMS								0X00000002								// 号码
#define		CDTID_FLT_NUMSGROUP							0X00000003								// 号码组
#define		CDTID_FLT_POSDUPLEX							0X00000004								// 定位复式
#define		CDTID_FLT_AREACOMBO							0X00000005								// 分区组号
#define		CDTID_FLT_AREACOMBINE						0X00000006								// 分区组合
#define		CDTID_FLT_SUPERDISTRIBUTE					0X00000007								// 超级分布
#define		CDTID_FLT_CUSTOMCOMBO						0X00000008								// 自定义组合
#define		CDTID_FLT_ASSEMPLY							0X00000009								// 集合
#define		CDTID_FLT_ANY								0X0000000A								// 任意码
#define		CDTID_FLT_SUPERBIGP							0X0000000B								// 大底分布(新超级分布)
#define		CDTID_FLT_MOREDUPLEX						0X0000000C								// 多期复式(暂时用与睿智版)
#define		CDTID_FLT_GROUPDUPLEX						0X0000000D
#define		CDTID_FLT_NONE								0X00000FF0								// 无条件（或者任意条件），目前用于特征串查询
#define		CDTID_FLT_HISTORY							0X00000FFF								// 历史条件

// =========================================================================================
// 以下定义条件组相关ID号
// 说明：条件组分为固定条件组、我的条件组、混合条件组、扩展条件组和非条件组（即单个条件）
//------------------------------------------------------------------------------------------
// 高字为类型，定义如下：
#define		CDTGID_TYPE_MIX								0X80000000								// 混合条件组
#define		CDTGID_TYPE_EXTEND							0X40000000								// 扩展条件组
#define		CDTGID_TYPE_MINE							0X20000000								// 我的条件组

// 为了兼容以前的，单个条件的ID统一为0，固定条件组ID的高字统一为0，低字有效
//------------------------------------------------------------------------------------------
// 以下定义固定分组条件（高字统一为0，低字有效）
#define		CDTID_FIXGROUP_NUMBERS						0X00001010								// 数字
#define		CDTID_FIXGROUP_NUMBERS_COUNT				0X00001011								// 数字个数
#define		CDTID_FIXGROUP_NUMBERS_SAME					0X00001012								// 相同数字
#define		CDTID_FIXGROUP_NUMBERS_ANY					0X00001013								// 相同数字
#define		CDTID_FIXGROUP_NUMBERS_TWONUMSIN			0X00001014								// 相同数字
#define		CDTID_FIXGROUP_GENRE						0X00001020								// 类型
#define		CDTID_FIXGROUP_SHAPE						0X00001030								// 形态
#define		CDTID_FIXGROUP_XL							0X00001040								// 斜连值
#define		CDTID_FIXGROUP_LINK							0X00001050								// 连号
#define		CDTID_FIXGROUP_DHK							0X00001060								// 胆合跨
#define		CDTID_FIXGROUP_POS							0X00001070								// 按位
#define		CDTID_FIXGROUP_SUM							0X00001080								// 和值
#define		CDTID_FIXGROUP_SPAN							0X00001090								// 跨度
#define		CDTID_FIXGROUP_AVG							0X00001098								// 均值
#define		CDTID_FIXGROUP_AC							0X000010A0								// AC值
#define		CDTID_FIXGROUP_DETRACT						0X000010B0								// 散度
#define		CDTID_FIXGROUP_DEPARTURE					0X000010B1								// 偏度
#define		CDTID_FIXGROUP_ODDENEN						0X000010C0								// 奇偶
#define		CDTID_FIXGROUP_BIGSMALL						0X000010D0								// 大小
#define		CDTID_FIXGROUP_PRIMECOMPOSITE				0X000010E0								// 质合
#define		CDTID_FIXGROUP_012							0X000010F0								// 012路
#define		CDTID_FIXGROUP_DZX							0X00001100								// 大中小
#define		CDTID_FIXGROUP_SKIPSIN						0X00001120								// 单选遗漏
#define		CDTID_FIXGROUP_SKIPSIN_TAILARR				0X00001121								// 单选遗漏尾排列
#define		CDTID_FIXGROUP_SKIPGRO						0X00001130								// 组选遗漏
#define		CDTID_FIXGROUP_SKIPGROTAILARR				0X00001131								// 组选遗漏尾排列
#define		CDTID_FIXGROUP_SPJ							0X00001140								// 升平降
#define		CDTID_FIXGROUP_LGC							0X00001150								// 邻孤传
#define		CDTID_FIXGROUP_FGZ							0X00001160								// 复隔中
#define		CDTID_FIXGROUP_CXT							0X00001170								// 重斜跳
#define		CDTID_FIXGROUP_RWL							0X00001180								// 热温冷
#define		CDTID_FIXGROUP_HEADTAIL						0X00001190								// 首尾关系
#define		CDTID_FIXGROUP_TWOPOS_ITEM					0X000011A0								// 两位关系（细分）
#define		CDTID_FIXGROUP_ESPECIALSUM					0X00001200								// 特码和
#define		CDTID_FIXGROUP_ORDER						0X00001210								// 顺序排列
#define		CDTID_FIXGROUP_ORDER_POS					0X00001211								// 顺序排列按位
#define		CDTID_FIXGROUP_ORDER_SUM					0X00001212								// 顺序排列位和
#define		CDTID_FIXGROUP_ORDER_SUMTAIL				0X00001213								// 顺序排列位合
#define		CDTID_FIXGROUP_ORDER_SUB					0X00001214								// 顺序排列位差
#define		CDTID_FIXGROUP_ORDER_SUBTAIL				0X00001215								// 顺序排列位差尾
#define		CDTID_FIXGROUP_ORDER_RELATIVEDZX			0X00001216								// 相对大中小
#define		CDTID_FIXGROUP_ORDER_THAN3					0X00001217								// 顺序排列（号码大于3个数字时使用）
#define		CDTID_FIXGROUP_SPACE						0X00001220								// 间距
#define		CDTID_FIXGROUP_SPACE_SIN					0X00001221								// 单选间距
#define		CDTID_FIXGROUP_SPACE_GRO					0X00001222								// 组选间距
#define		CDTID_FIXGROUP_NEARSPACE					0X00001230								// 邻位间距
#define		CDTID_FIXGROUP_REPEAT						0X00001240								// 重复号个数
#define		CDTID_FIXGROUP_REPEAT_1TON					0X00001241								// 前N期重复号个数
#define		CDTID_FIXGROUP_REPEAT_N						0X00001242								// 前第N期重复号个数
#define		CDTID_FIXGROUP_REMM							0X00001250								// 余数
#define		CDTID_FIXGROUP_1D							0X000012B0								// 1D
#define		CDTID_FIXGROUP_MUL							0X000012C0								// 积数
#define		CDTID_FIXGROUP_POSMULSUM					0X000012D0								// 位置积和
#define		CDTID_FIXGROUP_RESPOSMULSUM					0X000012E0								// 反位置积和
#define		CDTID_FIXGROUP_TWONUM_TYPE					0X000012F0								// 分类二码
#define		CDTID_FIXGROUP_NUMS							0X00001310								// 号码
#define		CDTID_FIXGROUP_TESTNUMS						0X00001320								// 试机号
#define		CDTID_FIXGROUP_TESTNUMS_ITEM_BEGIN			0X00001321								// 试机号子组相关开始
#define		CDTID_FIXGROUP_TESTNUMS_ITEM_END			0X0000132F								// 试机号子组相关结束
#define		CDTID_FIXGROUP_PREVDATA_ITEM				0X00001400								// 邻期关系（细分）
#define		CDTID_FIXGROUP_AREA_BEGIN					0X0000F000								// 分区开始
#define		CDTID_FIXGROUP_AREA_END						0X0000F0FF								// 分区结束
#define		CDTID_FIXGROUP_AREA_DETAILS_BEGIN			0X0000F100								// 详细分区开始
#define		CDTID_FIXGROUP_AREA_DETAILS_END				0X0000FEFF								// 详细分区结束

//------------------------------------------------------------------------------------------
// 以下定义我的分组条件
#define		CDTID_MINEGROUP								CDTGID_TYPE_MINE						// 我的条件
#define		CDTID_MINEGROUP_POS							(CDTGID_TYPE_MINE+1)					// 分位条件
#define		CDTID_MINEGROUP_COMPLEX						(CDTGID_TYPE_MINE+2)					// 综合条件
#define		CDTID_MINEGROUP_UNGROUPED					(CDTGID_TYPE_MINE+MAXWORD)				// 我的条件

//------------------------------------------------------------------------------------------
// 以下定义其他特定条件组
#define		CDTID_MIXGROUP								CDTGID_TYPE_MIX							// 混合条件组
#define		CDTID_EXTENDGROUP							CDTGID_TYPE_EXTEND						// 扩展条件组

// =========================================================================================

//------------------------------------------------------------------------------------------
// 以下定义指标分类ID号
//------------------------------------------------------------------------------------------
#define		ICID_3STATE_BEGIN							WORD(1)									// 三态（升平降等）开始
#define		ICID_3STATE_END								WORD(10)								// 三态（升平降等）结束

#define		ICID_TWOPOS_BEGIN							WORD(100)								// 两位关系开始
#define		ICID_TWOPOS_END								WORD(199)								// 两位关系结束

#define		ICID_PREVDATA_BEGIN							WORD(200)								// 邻位关系开始
#define		ICID_PREVDATA_END							WORD(299)								// 邻位关系结束

// =========================================================================================

// 定义相关指标名字集合
// 动态3态指标
const char NAMES_3STATE_DYNAMIC[][16] = {"升平降", "邻孤传", "复隔中", "重斜跳", "热温冷"};
const char BALLNAMES_3STATE_DYNAMIC[][8] = {"SPJ", "LGC", "FGZ", "CXT", "RWL"};
enum DYNAMIC3STATE_TYPE {D3ST_SPJ=0, D3ST_LGC, D3ST_FGZ, D3ST_CXT, D3ST_RWL, D3ST_COUNT};
const DWORD GROUPID_3STATE_DYNAMIC[] = {CDTID_FIXGROUP_SPJ, CDTID_FIXGROUP_LGC, CDTID_FIXGROUP_FGZ, CDTID_FIXGROUP_CXT, CDTID_FIXGROUP_RWL};

// 两位关系指标
const char NAMES_TWOPOS[][32] = {"位和", "位合", "位差", "顺位差", "逆位差", "位差尾数", "|位差|", "|位差|尾数", \
    "位环差", "顺位环差", "逆位环差", "位环差尾数", "顺位环差尾数", "逆位环差尾数", \
    "位积", "位积尾数", "二码合差", "二码合环差"};
enum TWOPOS_TYPE{TPT_SUM=0, TPT_SUM_TAIL, TPT_SUB, TPT_SUB_ASC, TPT_SUB_DESC, TPT_SUB_TAIL, TPT_SUBABS, TPT_SUBABS_TAIL, \
    TPT_CIRSUB, TPT_CIRSUB_ASC, TPT_CIRSUB_DESC, TPT_CIRSUB_TAIL, TPT_CIRSUB_ASC_TAIL, TPT_CIRSUB_DESC_TAIL, \
    TPT_MUL, TPT_MUL_TAIL, TPT_SUMSUB, TPT_SUMCIRSUB, TPT_COUNT};

// 邻期关系指标
const char NAMES_PREVDATA[][16] = {"距离", "距离尾", "邻期差", "邻期和", "邻期合", "环距", "环距尾", "轴差", "轴距", "轴距尾"};

enum PREVDATA_TYPE{PDT_DIS=0, PDT_DISTAIL, PDT_SUB, PDT_SUM, PDT_SUMTAIL, PDT_CIRDIS, PDT_CIRDISTAIL, PDT_AXISSUB, PDT_AXISDIS, PDT_AXISDISTAIL, PDT_COUNT};

// =========================================================================================