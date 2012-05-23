//
//  Indicator_GetName.cpp
//  QCLottery
//
//  Created by 武 孙 on 12-5-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "Indicator.h"
#include "ConditionInfo.h"
#include "ICstPubFunc.h"

#define		BUILDINDICATORNAME(szName, szShortName) { \
if(bGetShortName) { \
ASSERT(strlen(szShortName) < MAXLEN_INDICATOR_SHORTNAME); \
return szShortName; \
} \
else { \
ASSERT(strlen(szName) < MAXLEN_INDICATOR_NAME); \
return szName; \
} \
}

#define		BUILDINDICATORNAME_SAME(szShortName) { \
ASSERT(strlen(szShortName) < MAXLEN_INDICATOR_SHORTNAME); \
return szShortName; \
}

#define		BUILDINDICATORNAME_POS(nPosIndex, szName, szShortName) { \
if(bGetShortName) { \
strcpy(m_szShortName, m_strPosShortNames[nPosIndex]); \
strcat(m_szShortName, szShortName); \
ASSERT(strlen(m_szShortName) < MAXLEN_INDICATOR_SHORTNAME); \
return m_szShortName; \
} \
else { \
strcpy(m_szName, m_strPosNames[nPosIndex]); \
strcat(m_szName, szName); \
ASSERT(strlen(m_szName) < MAXLEN_INDICATOR_NAME); \
return m_szName; \
} \
}


///////////////////////////////////////////////////////////////////////////////////////
LPCSTR CIndicator::GetName(const CDTIID &cdtIID, BOOL bGetShortName, DWORD dwCustomData)
{
	if(cdtIID.btType & CDTIID_TYPE_DATAAREA)		// 分区指标
	{
		return _GetDataAreaIndicatorName(cdtIID, bGetShortName, dwCustomData);
	}
	
	// 系统指标
	switch(cdtIID.dwSystemIID)
	{
        case 0:	BUILDINDICATORNAME_SAME("无效条件"); break;
            
        case IID_STC_NUMBER_SAMECOMBO: BUILDINDICATORNAME_SAME("相同数字组合"); break;
        case IID_STC_NUMBER_MAXCOUNT: BUILDINDICATORNAME_SAME("任意数字最大出现个数"); break;
        case IID_STC_NUMBER_SAMEGROUP_2: BUILDINDICATORNAME("2个相同数字组数", "2同组数"); break;
        case IID_STC_NUMBER_SAMEGROUP_3: BUILDINDICATORNAME("3个相同数字组数", "3同组数"); break;
        case IID_STC_NUMBER_SAMEGROUP_4: BUILDINDICATORNAME("4个相同数字组数", "4同组数"); break;
        case IID_STC_NUMBER_SAMEGROUP_5: BUILDINDICATORNAME("5个相同数字组数", "5同组数"); break;
        case IID_STC_NUMBER_SAMEGROUP_6: BUILDINDICATORNAME("6个相同数字组数", "6同组数"); break;
        case IID_STC_NUMBER_SAMEGROUP_7: BUILDINDICATORNAME("7个相同数字组数", "7同组数"); break;
        case IID_STC_NUMBER_SAMEGROUP_8: BUILDINDICATORNAME("8个相同数字组数", "8同组数"); break;
            
        case IID_STC_GENRE_NORMAL: BUILDINDICATORNAME("号码类型", "类型"); break;
        case IID_STC_GENRE_TRANS: BUILDINDICATORNAME("号码变换类型", "变换类型"); break;
        case IID_STC_GENRE_TWOLINK: BUILDINDICATORNAME_SAME("二连码"); break;
            
        case IID_STC_SHAPE_NORMAL: BUILDINDICATORNAME_SAME("形态"); break;
        case IID_STC_SHAPE_EX: BUILDINDICATORNAME_SAME("细分形态"); break;
        case IID_STC_SHAPE_EQUALDIFF: BUILDINDICATORNAME_SAME("等差形"); break;
        case IID_STC_SHAPE_AAB: BUILDINDICATORNAME_SAME("组三形态"); break;
        case IID_STC_SHAPE_ABC: BUILDINDICATORNAME_SAME("顺子形态"); break;
        case IID_STC_SHAPE_ABD: BUILDINDICATORNAME_SAME("半顺形态"); break;
        case IID_STC_SHAPE_COUNT_SAMENUM: BUILDINDICATORNAME("相同号码个数", "同号个数"); break;
        case IID_STC_SHAPE_COUNT_SAMEGRO: BUILDINDICATORNAME("相同号码组数", "同号组数"); break;
        case IID_STC_SHAPE_COUNT_DIFFNUM: BUILDINDICATORNAME("不同号码个数", "不同号个数"); break;
            
        case IID_STC_LINK_NUMCOUNT: BUILDINDICATORNAME_SAME("连号个数"); break;
        case IID_STC_LINK_GROCOUNT: BUILDINDICATORNAME_SAME("连号组数"); break;
        case IID_STC_LINK_COMBO: BUILDINDICATORNAME_SAME("连号组合"); break;
        case IID_STC_LINK_NUMCOUNT_AS: BUILDINDICATORNAME("升序连号个数", "升连个数"); break;
        case IID_STC_LINK_GROCOUNT_AS: BUILDINDICATORNAME("升序连号组数", "升连组数"); break;
        case IID_STC_LINK_NUMCOUNT_DS: BUILDINDICATORNAME("降序连号个数", "降连个数"); break;
        case IID_STC_LINK_GROCOUNT_DS: BUILDINDICATORNAME("降序连号组数", "降连组数"); break;
        case IID_STC_LINK_COUNT_ODD: BUILDINDICATORNAME("奇连个数", "奇连数"); break;
        case IID_STC_LINK_COUNT_EVEN: BUILDINDICATORNAME("偶连个数", "偶连数"); break;
        case IID_STC_LINK_COUNT_PRIME: BUILDINDICATORNAME("质连个数", "质连数"); break;
        case IID_STC_LINK_COUNT_COMPOSITE: BUILDINDICATORNAME("合连个数", "合连数"); break;
        case IID_STC_LINK_POS_2: BUILDINDICATORNAME("2连号位置", "2连位置"); break;
        case IID_STC_LINK_POS_3: BUILDINDICATORNAME("3连号位置", "3连位置"); break;
        case IID_STC_LINK_POS_4: BUILDINDICATORNAME("4连号位置", "4连位置"); break;
        case IID_STC_LINK_POS_5: BUILDINDICATORNAME("5连号位置", "5连位置"); break;
        case IID_STC_LINK_POS_6: BUILDINDICATORNAME("6连号位置", "6连位置"); break;
        case IID_STC_LINK_POS_7: BUILDINDICATORNAME("7连号位置", "7连位置"); break;
            
        case IID_STC_DHK_HD: BUILDINDICATORNAME_SAME("合胆"); break;
        case IID_STC_DHK_KD: BUILDINDICATORNAME_SAME("跨胆"); break;
        case IID_STC_DHK_HK: BUILDINDICATORNAME_SAME("合跨"); break;
        case IID_STC_DHK_HDKD: BUILDINDICATORNAME_SAME("合胆跨胆"); break;
        case IID_STC_DHK_HDKDHK: BUILDINDICATORNAME_SAME("合胆跨胆合跨"); break;
            
        case IID_STC_NUMS_SIN: BUILDINDICATORNAME_SAME("单选号码"); break;
        case IID_STC_NUMS_GRO: BUILDINDICATORNAME_SAME("组选号码"); break;
            
        case IID_STC_POS_1: BUILDINDICATORNAME_POS(0, m_btNumberCount == 1 ? "" : (m_btDataType & DATA_TYPE_NUM_SZ ? "数字" : "号码"), ""); break;
        case IID_STC_POS_2: BUILDINDICATORNAME_POS(1, m_btDataType & DATA_TYPE_NUM_SZ ? "数字" : "号码", ""); break;
        case IID_STC_POS_3: BUILDINDICATORNAME_POS(2, m_btDataType & DATA_TYPE_NUM_SZ ? "数字" : "号码", ""); break;
        case IID_STC_POS_4: BUILDINDICATORNAME_POS(3, m_btDataType & DATA_TYPE_NUM_SZ ? "数字" : "号码", ""); break;
        case IID_STC_POS_5: BUILDINDICATORNAME_POS(4, m_btDataType & DATA_TYPE_NUM_SZ ? "数字" : "号码", ""); break;
        case IID_STC_POS_6: BUILDINDICATORNAME_POS(5, m_btDataType & DATA_TYPE_NUM_SZ ? "数字" : "号码", ""); break;
        case IID_STC_POS_7: BUILDINDICATORNAME_POS(6, m_btDataType & DATA_TYPE_NUM_SZ ? "数字" : "号码", ""); break;
        case IID_STC_POS_8: BUILDINDICATORNAME_POS(7, m_btDataType & DATA_TYPE_NUM_SZ ? "数字" : "号码", ""); break;
        case IID_STC_POS_ANY:
            if(g_pIData->GetDataType() & DATA_TYPE_NUM_SZ)
			{BUILDINDICATORNAME_SAME("任意单码");}
            else
			{BUILDINDICATORNAME_SAME("任意号码");}
            break;
        case IID_STC_SUM: BUILDINDICATORNAME("和值", "和"); break;
        case IID_STC_SUMTAIL: BUILDINDICATORNAME("合值", "合"); break;
        case IID_STC_SUM_N: BUILDINDICATORNAME("N次和", "N次和"); break;
        case IID_STC_SPAN: BUILDINDICATORNAME("跨度", "跨"); break;
        case IID_STC_SPANTAIL: BUILDINDICATORNAME("跨度尾数", "跨尾"); break;
        case IID_STC_AVG: BUILDINDICATORNAME("均值", "均"); break;
        case IID_STC_AVGTAIL: BUILDINDICATORNAME("均值尾数", "均尾"); break;
        case IID_STC_AC: BUILDINDICATORNAME("AC值", "AC"); break;
        case IID_STC_DETRACT: BUILDINDICATORNAME_SAME("散度"); break;
            
        case IID_STC_ODDENEN_COUNT_O: BUILDINDICATORNAME("奇数个数", "奇个"); break;
        case IID_STC_ODDENEN_COUNT_E: BUILDINDICATORNAME("偶数个数", "偶个"); break;
        case IID_STC_ODDENEN_ARR: BUILDINDICATORNAME_SAME("奇偶排列"); break;
        case IID_STC_ODDENEN_ARR_VALUE: BUILDINDICATORNAME_SAME("奇偶排列值"); break;
        case IID_STC_ODDENEN_LINK: BUILDINDICATORNAME_SAME("奇偶连"); break;
            
        case IID_STC_BIGSMALL_COUNT_B: BUILDINDICATORNAME("大数个数", "大个"); break;
        case IID_STC_BIGSMALL_COUNT_S: BUILDINDICATORNAME("小数个数", "小个"); break;
        case IID_STC_BIGSMALL_ARR: BUILDINDICATORNAME_SAME("大小排列"); break;
        case IID_STC_BIGSMALL_ARR_VALUE: BUILDINDICATORNAME_SAME("大小排列值"); break;
            
        case IID_STC_PRIMECOMPOSITE_COUNT_P: BUILDINDICATORNAME("质数个数", "质个"); break;
        case IID_STC_PRIMECOMPOSITE_COUNT_C: BUILDINDICATORNAME("合数个数", "合个"); break;
        case IID_STC_PRIMECOMPOSITE_ARR: BUILDINDICATORNAME_SAME("质合排列"); break;
        case IID_STC_PRIMECOMPOSITE_ARR_VALUE: BUILDINDICATORNAME_SAME("质合排列值"); break;
        case IID_STC_PRIMECOMPOSITE_LINK: BUILDINDICATORNAME_SAME("质合连"); break;
            
        case IID_STC_012_COUNT_0: BUILDINDICATORNAME("0路个数", "0路个"); break;
        case IID_STC_012_COUNT_1: BUILDINDICATORNAME("1路个数", "1路个"); break;
        case IID_STC_012_COUNT_2: BUILDINDICATORNAME("2路个数", "2路个"); break;
        case IID_STC_012_SHAPE: BUILDINDICATORNAME("012路形态", "012形态"); break;
        case IID_STC_012_ARR_SIN: BUILDINDICATORNAME("012路单选排列", "012单排"); break;
        case IID_STC_012_ARR_SIN_VALUE: BUILDINDICATORNAME("012路单选排列值", "012单排值"); break;
        case IID_STC_012_ARR_GRO: BUILDINDICATORNAME("012路组选排列", "012组排"); break;
        case IID_STC_012_ARR_GRO_VALUE: BUILDINDICATORNAME("012路组选排列值", "012组排值"); break;
        case IID_STC_012_ARR_COUNT: BUILDINDICATORNAME("012路个数排列", "012个排"); break;
        case IID_STC_012_ARR_COUNT_VALUE: BUILDINDICATORNAME("012路个数排列值", "012个排值"); break;
            
        case IID_STC_DZX_COUNT_D: BUILDINDICATORNAME("D数个数", "D数个"); break;
        case IID_STC_DZX_COUNT_Z: BUILDINDICATORNAME("Z数个数", "Z数个"); break;
        case IID_STC_DZX_COUNT_X: BUILDINDICATORNAME("X数个数", "X数个"); break;
        case IID_STC_DZX_SHAPE: BUILDINDICATORNAME_SAME("XZD形态"); break;
        case IID_STC_DZX_ARR_SIN: BUILDINDICATORNAME("XZD单选排列", "XZD单排"); break;
        case IID_STC_DZX_ARR_SIN_VALUE: BUILDINDICATORNAME("XZD单选排列值", "XZD单排值"); break;
        case IID_STC_DZX_ARR_GRO: BUILDINDICATORNAME("XZD组选排列", "XZD组排"); break;
        case IID_STC_DZX_ARR_GRO_VALUE: BUILDINDICATORNAME("XZD组选排列值", "XZD组排值"); break;
        case IID_STC_DZX_ARR_COUNT: BUILDINDICATORNAME("XZD个数排列", "XZD个排"); break;
        case IID_STC_DZX_ARR_COUNT_VALUE: BUILDINDICATORNAME("XZD个数排列值", "XZD个排值"); break;
            
        case IID_STC_ESPECIALSUM_ODD: BUILDINDICATORNAME_SAME("奇数和"); break;
        case IID_STC_ESPECIALSUM_EVEN: BUILDINDICATORNAME_SAME("偶数和"); break;
        case IID_STC_ESPECIALSUM_BIG: BUILDINDICATORNAME_SAME("大数和"); break;
        case IID_STC_ESPECIALSUM_SMALL: BUILDINDICATORNAME_SAME("小数和"); break;
        case IID_STC_ESPECIALSUM_PRIME: BUILDINDICATORNAME_SAME("质数和"); break;
        case IID_STC_ESPECIALSUM_COMPOSITE: BUILDINDICATORNAME_SAME("合数和"); break;
        case IID_STC_ESPECIALSUM_0: BUILDINDICATORNAME_SAME("0路和"); break;
        case IID_STC_ESPECIALSUM_1: BUILDINDICATORNAME_SAME("1路和"); break;
        case IID_STC_ESPECIALSUM_2: BUILDINDICATORNAME_SAME("2路和"); break;
        case IID_STC_ESPECIALSUM_D: BUILDINDICATORNAME_SAME("D数和"); break;
        case IID_STC_ESPECIALSUM_Z: BUILDINDICATORNAME_SAME("Z数和"); break;
        case IID_STC_ESPECIALSUM_X: BUILDINDICATORNAME_SAME("X数和"); break;
            
        case IID_STC_TWONUM_ANY: BUILDINDICATORNAME_SAME("任意二码"); break;
        case IID_STC_TWONUM_Z6: BUILDINDICATORNAME_SAME("组六分类二码"); break;
        case IID_STC_TWONUM_Z6_2: BUILDINDICATORNAME_SAME("组六分类二码2"); break;
        case IID_STC_TWONUM_Z3: BUILDINDICATORNAME_SAME("组三分类二码"); break;
        case IID_STC_TWONUM_Z3_2: BUILDINDICATORNAME_SAME("组三分类二码2"); break;
            
        case IID_STC_TWONUMSIN_ANY: BUILDINDICATORNAME_SAME("任意单选二码"); break;
            
        case IID_STC_ORDER_MIN: BUILDINDICATORNAME("最小数", "最小"); break;
        case IID_STC_ORDER_MID: BUILDINDICATORNAME("中间数", "中间"); break;
        case IID_STC_ORDER_MAX: BUILDINDICATORNAME("最大数", "最大"); break;
        case IID_STC_ORDER_MINADDMID: BUILDINDICATORNAME("最小数＋中间数", "最小＋中间"); break;
        case IID_STC_ORDER_MINADDMAX: BUILDINDICATORNAME("最小数＋最大数", "最小＋最大"); break;
        case IID_STC_ORDER_MIDADDMAX: BUILDINDICATORNAME("中间数＋最大数", "中间＋最大"); break;
        case IID_STC_ORDER_POSSUM_ARR_SIN: BUILDINDICATORNAME("顺序位和单选排列", "顺序位和单排"); break;
        case IID_STC_ORDER_POSSUM_ARR_GRO: BUILDINDICATORNAME("顺序位和组选排列", "顺序位和组排"); break;
        case IID_STC_ORDER_MINADDMID_TAIL: BUILDINDICATORNAME("最小数＋中间数尾数", "最小＋中间尾"); break;
        case IID_STC_ORDER_MINADDMAX_TAIL: BUILDINDICATORNAME("最小数＋最大数尾数", "最小＋最大尾"); break;
        case IID_STC_ORDER_MIDADDMAX_TAIL: BUILDINDICATORNAME("中间数＋最大数尾数", "中间＋最大尾"); break;
        case IID_STC_ORDER_POSSUMTAIL_ARR_SIN: BUILDINDICATORNAME("顺序位合单选排列", "顺序位合单排"); break;
        case IID_STC_ORDER_POSSUMTAIL_ARR_GRO: BUILDINDICATORNAME("顺序位合组选排列", "顺序位合组排"); break;
        case IID_STC_ORDER_MIDSUBMIN: BUILDINDICATORNAME("中间数－最小数", "中间－最小"); break;
        case IID_STC_ORDER_MAXSUBMIN: BUILDINDICATORNAME("最大数－最小数", "最大－最小"); break;
        case IID_STC_ORDER_MAXSUBMID: BUILDINDICATORNAME("最大数－中间数", "最大－中间"); break;
        case IID_STC_ORDER_POSSUB_ARR_SIN: BUILDINDICATORNAME("顺序位差单选排列", "顺序位差单排"); break;
        case IID_STC_ORDER_POSSUB_ARR_GRO: BUILDINDICATORNAME("顺序位差组选排列", "顺序位差组排"); break;
        case IID_STC_ORDER_MIDSUBMIN_TAIL: BUILDINDICATORNAME("中间数－最小数尾数", "中间－最小尾"); break;
        case IID_STC_ORDER_MAXSUBMIN_TAIL: BUILDINDICATORNAME("最大数－最小数尾数", "最大－最小尾"); break;
        case IID_STC_ORDER_MAXSUBMID_TAIL: BUILDINDICATORNAME("最大数－中间数尾数", "最大－中间尾"); break;
        case IID_STC_ORDER_POSSUBTAIL_ARR_SIN: BUILDINDICATORNAME("顺序位差尾数单选排列", "顺序位差尾单排"); break;
        case IID_STC_ORDER_POSSUBTAIL_ARR_GRO: BUILDINDICATORNAME("顺序位差尾数组选排列", "顺序位差尾组排"); break;
        case IID_STC_ORDER_RELATIVEDZX_POS_1: BUILDINDICATORNAME_SAME("百位相对大中小"); break;
        case IID_STC_ORDER_RELATIVEDZX_POS_2: BUILDINDICATORNAME_SAME("十位相对大中小"); break;
        case IID_STC_ORDER_RELATIVEDZX_POS_3: BUILDINDICATORNAME_SAME("个位相对大中小"); break;
        case IID_STC_ORDER_RELATIVEDZX_ARR: BUILDINDICATORNAME_SAME("相对大中小排列"); break;
        case IID_STC_ORDER_RELATIVEDZX_ARR_VALUE: BUILDINDICATORNAME_SAME("相对大中小排列值"); break;
            
        case IID_STC_SPACE_SIN: BUILDINDICATORNAME("单选间距", "单间"); break;
        case IID_STC_SPACETAIL_SIN: BUILDINDICATORNAME("单选间距尾数", "单间尾"); break;
        case IID_STC_SPACE_SIN_HEAD: BUILDINDICATORNAME("单选首间距", "单首间"); break;
        case IID_STC_SPACE_SIN_TAIL: BUILDINDICATORNAME("单选尾间距", "单尾间"); break;
        case IID_STC_SPACE_SIN_1: BUILDINDICATORNAME("单选第1间距", "单1间"); break;
        case IID_STC_SPACE_SIN_2: BUILDINDICATORNAME("单选第2间距", "单2间"); break;
        case IID_STC_SPACE_SIN_3: BUILDINDICATORNAME("单选第3间距", "单3间"); break;
        case IID_STC_SPACE_SIN_4: BUILDINDICATORNAME("单选第4间距", "单4间"); break;
        case IID_STC_SPACE_SIN_5: BUILDINDICATORNAME("单选第5间距", "单5间"); break;
        case IID_STC_SPACE_SIN_6: BUILDINDICATORNAME("单选第6间距", "单6间"); break;
        case IID_STC_SPACE_SIN_7: BUILDINDICATORNAME("单选第7间距", "单7间"); break;
        case IID_STC_SPACE_SIN_8: BUILDINDICATORNAME("单选第8间距", "单8间"); break;
        case IID_STC_SPACE_GRO: BUILDINDICATORNAME("组选间距", "组间"); break;
        case IID_STC_SPACE_GRO_HEAD: BUILDINDICATORNAME("组选首间距", "组首间"); break;
        case IID_STC_SPACE_GRO_TAIL: BUILDINDICATORNAME("组选尾间距", "组尾间"); break;
        case IID_STC_SPACE_GRO_1: BUILDINDICATORNAME("组选第1间距", "组1间"); break;
        case IID_STC_SPACE_GRO_2: BUILDINDICATORNAME("组选第2间距", "组2间"); break;
        case IID_STC_SPACE_GRO_3: BUILDINDICATORNAME("组选第3间距", "组3间"); break;
        case IID_STC_SPACE_GRO_4: BUILDINDICATORNAME("组选第4间距", "组4间"); break;
        case IID_STC_SPACE_GRO_5: BUILDINDICATORNAME("组选第5间距", "组5间"); break;
        case IID_STC_SPACE_GRO_6: BUILDINDICATORNAME("组选第6间距", "组6间"); break;
        case IID_STC_SPACE_GRO_7: BUILDINDICATORNAME("组选第7间距", "组7间"); break;
        case IID_STC_SPACE_GRO_8: BUILDINDICATORNAME("组选第8间距", "组8间"); break;
            
        case IID_STC_NEARSPACE_12: BUILDINDICATORNAME("1-2邻位间距", "12邻间"); break;
        case IID_STC_NEARSPACE_23: BUILDINDICATORNAME("2-3邻位间距", "23邻间"); break;
        case IID_STC_NEARSPACE_34: BUILDINDICATORNAME("3-4邻位间距", "34邻间"); break;
        case IID_STC_NEARSPACE_45: BUILDINDICATORNAME("4-5邻位间距", "45邻间"); break;
        case IID_STC_NEARSPACE_56: BUILDINDICATORNAME("5-6邻位间距", "56邻间"); break;
        case IID_STC_NEARSPACE_67: BUILDINDICATORNAME("6-7邻位间距", "67邻间"); break;
        case IID_STC_NEARSPACE_78: BUILDINDICATORNAME("7-8邻位间距", "78邻间"); break;
        case IID_STC_NEARSPACE_HT: 
		{
			if(bGetShortName)
			{
				sprintf(m_szShortName, "%d1邻间", m_btNumberCount);
				return m_szShortName;
			}
			else
			{
				sprintf(m_szName, "%d-1邻位间距", m_btNumberCount);
				return m_szName;
			} 
		}break;
        case IID_STC_NEARSPACE_MAX: BUILDINDICATORNAME("最大邻位间距", "最大邻间"); break;
        case IID_STC_NEARSPACE_MAXEX: 
            if(bGetShortName)
            {
                sprintf(m_szShortName, "最大邻间(含%d-1)", m_btNumberCount);
                return m_szShortName;
            }
            else
            {
                sprintf(m_szName, "最大邻位间距(含%d-1)", m_btNumberCount);
                return m_szName;
            } break;
        case IID_STC_NEARSPACE_MIN: BUILDINDICATORNAME("最小邻位间距", "最小邻间"); break;
        case IID_STC_NEARSPACE_MINEX:
            if(bGetShortName)
            {
                sprintf(m_szShortName, "最小邻间(含%d-1)", m_btNumberCount);
                return m_szShortName;
            }
            else
            {
                sprintf(m_szName, "最小邻位间距(含%d-1)", m_btNumberCount);
                return m_szName;
            } break;
        case IID_STC_NEARSPACE_AVG: BUILDINDICATORNAME("平均邻位间距", "平均邻间"); break;
        case IID_STC_NEARSPACE_ANY: BUILDINDICATORNAME("任意邻位间距", "任意邻间"); break;
            
        case IID_STC_1D_POSITIVE_SIN: BUILDINDICATORNAME("单选正1D", "单正1D"); break;
        case IID_STC_1D_POSITIVE_GRO: BUILDINDICATORNAME("组选正1D", "组正1D"); break;
        case IID_STC_1D_NEGATIVE_SIN: BUILDINDICATORNAME("单选负1D", "单负1D"); break;
        case IID_STC_1D_NEGATIVE_GRO: BUILDINDICATORNAME("组选负1D", "组负1D"); break;
        case IID_STC_1D_RPOSITIVE_SIN: BUILDINDICATORNAME("单选反正1D", "单反正1D"); break;
        case IID_STC_1D_RPOSITIVE_GRO: BUILDINDICATORNAME("组选反正1D", "组反正1D"); break;
        case IID_STC_1D_RNEGATIVE_SIN: BUILDINDICATORNAME("单选反负1D", "单反负1D"); break;
        case IID_STC_1D_RNEGATIVE_GRO: BUILDINDICATORNAME("组选反负1D", "组反负1D"); break;
            
        case IID_STC_MUL: BUILDINDICATORNAME("积数", "积"); break;
        case IID_STC_MUL_ODDEVEN: BUILDINDICATORNAME("积数奇偶零", "积奇偶零"); break;
        case IID_STC_MUL_BIGSMALL: BUILDINDICATORNAME("积数大小零", "积大小零"); break;
        case IID_STC_MUL_012: BUILDINDICATORNAME("积数012路零", "积012零"); break;
        case IID_STC_MULTAIL: BUILDINDICATORNAME("积数尾数", "积尾"); break;
        case IID_STC_POSMULSUM: BUILDINDICATORNAME_SAME("位置积和"); break;
        case IID_STC_POSMULSUMTAIL: BUILDINDICATORNAME_SAME("位置积合"); break;
        case IID_STC_RESPOSMULSUM: BUILDINDICATORNAME_SAME("反位置积和"); break;
        case IID_STC_RESPOSMULSUMTAIL: BUILDINDICATORNAME_SAME("反位置积合"); break;
            
            /////////////////////////////////////////////////////////////////////////////////////////////////
        case IID_DNC_DIS_POS_1: BUILDINDICATORNAME_POS(0, "距离", "距"); break;
        case IID_DNC_DIS_POS_2: BUILDINDICATORNAME_POS(1, "距离", "距"); break;
        case IID_DNC_DIS_POS_3: BUILDINDICATORNAME_POS(2, "距离", "距"); break;
        case IID_DNC_DIS_POS_4: BUILDINDICATORNAME_POS(3, "距离", "距"); break;
        case IID_DNC_DIS_POS_5: BUILDINDICATORNAME_POS(4, "距离", "距"); break;
        case IID_DNC_DIS_POS_6: BUILDINDICATORNAME_POS(5, "距离", "距"); break;
        case IID_DNC_DIS_POS_7: BUILDINDICATORNAME_POS(6, "距离", "距"); break;
        case IID_DNC_DIS_POS_8: BUILDINDICATORNAME_POS(7, "距离", "距"); break;
        case IID_DNC_DIS_POS_ANY: BUILDINDICATORNAME("任意距离", "任距"); break;
            
        case IID_DNC_DIS_SUM: BUILDINDICATORNAME_SAME("距离和"); break;
        case IID_DNC_DIS_SUMTAIL: BUILDINDICATORNAME_SAME("距离合"); break;
            
        case IID_DNC_DIS_ARR_SIN: BUILDINDICATORNAME("距离单选排列", "距单排"); break;
        case IID_DNC_DIS_ARR_GRO: BUILDINDICATORNAME("距离组选排列", "距组排"); break;
            
        case IID_DNC_XL_VALUE: BUILDINDICATORNAME_SAME("斜连值"); break;
        case IID_DNC_XL_LEFT: BUILDINDICATORNAME_SAME("左斜连"); break;
        case IID_DNC_XL_RIGHT: BUILDINDICATORNAME_SAME("右斜连"); break;
        case IID_DNC_XL_COMBO: BUILDINDICATORNAME("左右斜连组合", "斜连组合"); break;
            
        case IID_DNC_DEPARTURE: BUILDINDICATORNAME_SAME("偏度"); break;
            
        case IID_DNC_NUMS_MATCHCOUNT_SIN: BUILDINDICATORNAME("单选号码出现次数", "单选号码次数"); break;
        case IID_DNC_NUMS_MATCHCOUNT_GRO: BUILDINDICATORNAME("组选号码出现次数", "组选号码次数"); break;
            
        case IID_DNC_TESTNUMS_REPEAT_CUR_CUR: BUILDINDICATORNAME_SAME("当期试机重复"); break;
        case IID_DNC_TESTNUMS_REPEAT_PRE_CUR: BUILDINDICATORNAME_SAME("上期试机重复"); break;
        case IID_DNC_TESTNUMS_REPEAT_COMPLEX: BUILDINDICATORNAME_SAME("试机综合重复"); break;		
        case IID_DNC_TESTNUMS_REPEAT_COMBO: BUILDINDICATORNAME_SAME("试机重复组合"); break;		
            
        case (IID_DNC_REPEAT_1TON): BUILDINDICATORNAME("前1期重复号个数", "前1期重号数"); break;
        case (IID_DNC_REPEAT_1TON+1): BUILDINDICATORNAME("前2期重复号个数", "前2期重号数"); break;
        case (IID_DNC_REPEAT_1TON+2): BUILDINDICATORNAME("前3期重复号个数", "前3期重号数"); break;
        case (IID_DNC_REPEAT_1TON+3): BUILDINDICATORNAME("前4期重复号个数", "前4期重号数"); break;
        case (IID_DNC_REPEAT_1TON+4): BUILDINDICATORNAME("前5期重复号个数", "前5期重号数"); break;
        case (IID_DNC_REPEAT_1TON+5): BUILDINDICATORNAME("前6期重复号个数", "前6期重号数"); break;
        case (IID_DNC_REPEAT_1TON+6): BUILDINDICATORNAME("前7期重复号个数", "前7期重号数"); break;
        case (IID_DNC_REPEAT_1TON+7): BUILDINDICATORNAME("前8期重复号个数", "前8期重号数"); break;
        case (IID_DNC_REPEAT_1TON+8): BUILDINDICATORNAME("前9期重复号个数", "前9期重号数"); break;
        case (IID_DNC_REPEAT_1TON+9): BUILDINDICATORNAME("前10期重复号个数", "前10期重号数"); break;
            
        case (IID_DNC_REPEAT_N): BUILDINDICATORNAME("前第1期重复号个数", "前第1期重号数"); break;
        case (IID_DNC_REPEAT_N+1): BUILDINDICATORNAME("前第2期重复号个数", "前第2期重号数"); break;
        case (IID_DNC_REPEAT_N+2): BUILDINDICATORNAME("前第3期重复号个数", "前第3期重号数"); break;
        case (IID_DNC_REPEAT_N+3): BUILDINDICATORNAME("前第4期重复号个数", "前第4期重号数"); break;
        case (IID_DNC_REPEAT_N+4): BUILDINDICATORNAME("前第5期重复号个数", "前第5期重号数"); break;
        case (IID_DNC_REPEAT_N+5): BUILDINDICATORNAME("前第6期重复号个数", "前第6期重号数"); break;
        case (IID_DNC_REPEAT_N+6): BUILDINDICATORNAME("前第7期重复号个数", "前第7期重号数"); break;
        case (IID_DNC_REPEAT_N+7): BUILDINDICATORNAME("前第8期重复号个数", "前第8期重号数"); break;
        case (IID_DNC_REPEAT_N+8): BUILDINDICATORNAME("前第9期重复号个数", "前第9期重号数"); break;
        case (IID_DNC_REPEAT_N+9): BUILDINDICATORNAME("前第10期重复号个数", "前第10期重号数"); break;
            
        case IID_DNC_SKIPSIN_1:
            if(m_btNumberCount == 1) 
            {
                BUILDINDICATORNAME("遗漏值", "遗漏")
            }
            else
            {
                BUILDINDICATORNAME_POS(0, "单选遗漏", "单遗");
            }
            break;
        case IID_DNC_SKIPSIN_2: BUILDINDICATORNAME_POS(1, "单选遗漏", "单遗"); break;
        case IID_DNC_SKIPSIN_3: BUILDINDICATORNAME_POS(2, "单选遗漏", "单遗"); break;
        case IID_DNC_SKIPSIN_4: BUILDINDICATORNAME_POS(3, "单选遗漏", "单遗"); break;
        case IID_DNC_SKIPSIN_5: BUILDINDICATORNAME_POS(4, "单选遗漏", "单遗"); break;
        case IID_DNC_SKIPSIN_6: BUILDINDICATORNAME_POS(5, "单选遗漏", "单遗"); break;
        case IID_DNC_SKIPSIN_7: BUILDINDICATORNAME_POS(6, "单选遗漏", "单遗"); break;
        case IID_DNC_SKIPSIN_8: BUILDINDICATORNAME_POS(7, "单选遗漏", "单遗"); break;
        case IID_DNC_SKIPSIN_MIN: BUILDINDICATORNAME("最小单选遗漏", "最小单遗"); break;
        case IID_DNC_SKIPSIN_MAX: BUILDINDICATORNAME("最大单选遗漏", "最大单遗"); break;
            
        case IID_DNC_SKIPSIN_TAIL_1: 
            if(m_btNumberCount == 1) 
            {
                BUILDINDICATORNAME("遗漏值尾", "遗漏尾")
            }
            else
            {
                BUILDINDICATORNAME_POS(0, "单选遗漏尾", "单遗尾")
            };
            break;
        case IID_DNC_SKIPSIN_TAIL_2: BUILDINDICATORNAME_POS(1, "单选遗漏尾", "单遗尾"); break;
        case IID_DNC_SKIPSIN_TAIL_3: BUILDINDICATORNAME_POS(2, "单选遗漏尾", "单遗尾"); break;
        case IID_DNC_SKIPSIN_TAIL_4: BUILDINDICATORNAME_POS(3, "单选遗漏尾", "单遗尾"); break;
        case IID_DNC_SKIPSIN_TAIL_5: BUILDINDICATORNAME_POS(4, "单选遗漏尾", "单遗尾"); break;
        case IID_DNC_SKIPSIN_TAIL_6: BUILDINDICATORNAME_POS(5, "单选遗漏尾", "单遗尾"); break;
        case IID_DNC_SKIPSIN_TAIL_7: BUILDINDICATORNAME_POS(6, "单选遗漏尾", "单遗尾"); break;
        case IID_DNC_SKIPSIN_TAIL_8: BUILDINDICATORNAME_POS(7, "单选遗漏尾", "单遗尾"); break;
        case IID_DNC_SKIPSIN_TAIL_MIN: BUILDINDICATORNAME("最小单选遗漏尾", "最小单遗尾"); break;
        case IID_DNC_SKIPSIN_TAIL_MAX: BUILDINDICATORNAME("最大单选遗漏尾", "最大单遗尾"); break;
        case IID_DNC_SKIPSIN_TAIL_ANY: BUILDINDICATORNAME("任意单选遗漏尾", "任意单遗尾"); break;
        case IID_DNC_SKIPSIN_TAIL_ARR_SIN: BUILDINDICATORNAME("单选遗漏尾单选排列", "单遗尾单排"); break;
        case IID_DNC_SKIPSIN_TAIL_ARR_GRO: BUILDINDICATORNAME("单选遗漏尾组选排列", "单遗尾组排"); break;
            
        case IID_DNC_SKIPSIN_SUM: BUILDINDICATORNAME("单选遗漏和值", "单遗和"); break;
        case IID_DNC_SKIPSIN_SUM_AREA:
            if(m_btNumberCount == 1)
                BUILDINDICATORNAME("遗漏值分区", "遗漏分区")
                else
                    BUILDINDICATORNAME("单选遗漏和值分区", "单遗和分区")
                    break;
        case IID_DNC_SKIPSIN_SUM_AREA10:
            if(m_btNumberCount == 1)
                BUILDINDICATORNAME("遗漏值十分区", "遗漏十分区")
                else
                    BUILDINDICATORNAME("单选遗漏和值十分区", "单遗和十分区")
                    break;
        case IID_DNC_SKIPSIN_SUMTAIL: BUILDINDICATORNAME("单选遗漏合值", "单遗合"); break;
            
        case IID_DNC_SKIPGRO_1: 
        case IID_DNC_SKIPGRO_2:
        case IID_DNC_SKIPGRO_3:
        case IID_DNC_SKIPGRO_4:
        case IID_DNC_SKIPGRO_5:
        case IID_DNC_SKIPGRO_6:
        case IID_DNC_SKIPGRO_7:
        case IID_DNC_SKIPGRO_8:	BUILDINDICATORNAME_POS(cdtIID.dwSystemIID - IID_DNC_SKIPGRO_1, "组选遗漏", "组遗"); break;
        case IID_DNC_SKIPGRO_MIN: BUILDINDICATORNAME("最小组选遗漏", "最小组遗"); break;
        case IID_DNC_SKIPGRO_MAX: BUILDINDICATORNAME("最大组选遗漏", "最大组遗"); break;
            
        case IID_DNC_SKIPGRO_TAIL_1:
        case IID_DNC_SKIPGRO_TAIL_2:
        case IID_DNC_SKIPGRO_TAIL_3:
        case IID_DNC_SKIPGRO_TAIL_4:
        case IID_DNC_SKIPGRO_TAIL_5:
        case IID_DNC_SKIPGRO_TAIL_6:
        case IID_DNC_SKIPGRO_TAIL_7:
        case IID_DNC_SKIPGRO_TAIL_8: BUILDINDICATORNAME_POS(cdtIID.dwSystemIID - IID_DNC_SKIPGRO_TAIL_1, "组选遗漏尾", "组遗尾"); break;
        case IID_DNC_SKIPGRO_TAIL_MIN: BUILDINDICATORNAME("最小组选遗漏尾", "最小组遗尾"); break;
        case IID_DNC_SKIPGRO_TAIL_MAX: BUILDINDICATORNAME("最大组选遗漏尾", "最大组遗尾"); break;
        case IID_DNC_SKIPGRO_TAIL_ANY: BUILDINDICATORNAME("任意组选遗漏尾", "任意组遗尾"); break;
        case IID_DNC_SKIPGRO_TAIL_ARR_SIN: BUILDINDICATORNAME("组选遗漏尾单选排列", "组遗尾单排"); break;
        case IID_DNC_SKIPGRO_TAIL_ARR_GRO: BUILDINDICATORNAME("组选遗漏尾组选排列", "组遗尾组排"); break;
            
        case IID_DNC_SKIPGRO_SUM: BUILDINDICATORNAME("组选遗漏和值", "组遗和"); break;
        case IID_DNC_SKIPGRO_SUM_AREA: BUILDINDICATORNAME("组选遗漏和值分区", "组遗和分区"); break;
        case IID_DNC_SKIPGRO_SUM_AREA10: BUILDINDICATORNAME("组选遗漏和值十分区", "组遗和十分区"); break;
        case IID_DNC_SKIPGRO_SUMTAIL: BUILDINDICATORNAME("组选遗漏合值", "组遗合"); break;
            
        case IID_HIS_NUMS_ARR_SIN: 
		{
			if(g_pIData->GetDataType() & DATA_TYPE_ORDERED)
				BUILDINDICATORNAME_SAME("历史号码")
                else
                    BUILDINDICATORNAME_SAME("号码单选排列")
                    }break;
        case IID_HIS_NUMS_ARR_GRO: BUILDINDICATORNAME_SAME("号码组选排列"); break;
        case IID_HIS_NUMS_1_ARR_SIN:
		{
			if(g_pIData->GetDataType() & DATA_TYPE_ORDERED)
				BUILDINDICATORNAME_SAME("单码")
                else
                    BUILDINDICATORNAME_SAME("单选单码")
                    }break;
        case IID_HIS_NUMS_1_ARR_GRO: BUILDINDICATORNAME_SAME("组选单码"); break;
        case IID_HIS_NUMS_2_ARR_SIN:
		{
			if(g_pIData->GetDataType() & DATA_TYPE_ORDERED)
				BUILDINDICATORNAME_SAME("两码")
                else
                    BUILDINDICATORNAME_SAME("单选两码")
                    }break;
        case IID_HIS_NUMS_2_ARR_GRO: BUILDINDICATORNAME_SAME("组选两码"); break;
            
            /////////////////////////////////////////////////////////////////////////////////////////////////
            
        default:
		{
			if(cdtIID.dwSystemIID >= IID_STC_NUMBER_0NUM && cdtIID.dwSystemIID <= IID_STC_NUMBER_0NUM + 100)	// 数字
			{
				if(bGetShortName)
					sprintf(m_szShortName, "%s个数", g_pIData->GetNumberText(-1, BYTE(cdtIID.dwSystemIID - IID_STC_NUMBER_0NUM)));
				else
					sprintf(m_szName, "数字%s个数", g_pIData->GetNumberText(-1, BYTE(cdtIID.dwSystemIID - IID_STC_NUMBER_0NUM)));
			}
			else if(cdtIID.dwSystemIID >= IID_STC_TWOPOS_BEGIN && cdtIID.dwSystemIID <= IID_STC_TWOPOS_END)	// 2位关系
			{
				int nTypeIndex = (cdtIID.dwSystemIID - IID_STC_TWOPOS_BEGIN) / 100;
				LPSTR lpName = bGetShortName ? m_szShortName : m_szName;
				switch(cdtIID.dwSystemIID - nTypeIndex * 100)
				{
                    case IID_STC_TWOPOS_ANY: strcpy(lpName, "任意"); strcat(lpName, NAMES_TWOPOS[nTypeIndex]); break;
                    case IID_STC_TWOPOS_MIN: strcpy(lpName, "最小"); strcat(lpName, NAMES_TWOPOS[nTypeIndex]); break;
                    case IID_STC_TWOPOS_MID: strcpy(lpName, "中间"); strcat(lpName, NAMES_TWOPOS[nTypeIndex]); break;
                    case IID_STC_TWOPOS_MAX: strcpy(lpName, "最大"); strcat(lpName, NAMES_TWOPOS[nTypeIndex]); break;
                    case IID_STC_TWOPOS_ARR_SIN: strcpy(lpName, NAMES_TWOPOS[nTypeIndex]); strcat(lpName, bGetShortName ? "单排" : "单选排列"); break;
                    case IID_STC_TWOPOS_ARR_GRO: strcpy(lpName, NAMES_TWOPOS[nTypeIndex]); strcat(lpName, bGetShortName ? "组排" : "组选排列"); break;
                    case IID_STC_TWOPOS_HEADTAIL: 
                        strcpy(lpName, "首尾");
                        switch(nTypeIndex)
					{
                        case TPT_SUM: strcat(lpName, "和"); break;
                        case TPT_SUM_TAIL: strcat(lpName, "合"); break;
                        case TPT_SUB: bGetShortName ? strcat(lpName, "距") : strcat(lpName, "边距"); break;
                        case TPT_SUB_TAIL: bGetShortName ? strcat(lpName, "距尾") : strcat(lpName, "边距尾数"); break;
                        case TPT_CIRSUB: strcat(lpName, "环差"); break;
                        case TPT_CIRSUB_TAIL: bGetShortName ? strcat(lpName, "环差尾") : strcat(lpName, "环差尾数"); break;
                        default: strcat(lpName, NAMES_TWOPOS[nTypeIndex]); break;
					}
                        break;
                    default: // 指定两位关系
					{
						int nPos1 = ((cdtIID.dwSystemIID - IID_STC_TWOPOS_BEGIN) / 10) % 10, nPos2 = (cdtIID.dwSystemIID - IID_STC_TWOPOS_BEGIN) % 10;
                        LPSTR lpPos1Name = bGetShortName ? m_strPosShortNames[nPos1] : m_strPosNames[nPos1];
                        LPSTR lpPos2Name = bGetShortName ? m_strPosShortNames[nPos2] : m_strPosNames[nPos2];
                        switch(nTypeIndex)
						{
                            case TPT_SUM: sprintf(lpName, "%s＋%s", lpPos1Name, lpPos2Name); break;
                            case TPT_SUM_TAIL: sprintf(lpName, "%s＋%s尾", lpPos1Name, lpPos2Name); break;
                            case TPT_SUB_ASC: sprintf(lpName, "%s－%s", lpPos1Name, lpPos2Name); break;
                            case TPT_SUB_DESC: 
                            case TPT_SUB: sprintf(lpName, "%s－%s", lpPos2Name, lpPos1Name); break;
                            case TPT_SUB_TAIL: sprintf(lpName, "%s－%s尾", lpPos2Name, lpPos1Name); break;
                            case TPT_SUBABS: sprintf(lpName, "|%s－%s|", lpPos1Name, lpPos2Name); break;
                            case TPT_SUBABS_TAIL: sprintf(lpName, "|%s－%s|尾", lpPos1Name, lpPos2Name); break;
                            case TPT_CIRSUB: 
                            case TPT_CIRSUB_ASC: sprintf(lpName, "%s%s环差", lpPos1Name, lpPos2Name); break;
                            case TPT_CIRSUB_DESC: sprintf(lpName, "%s%s环差", lpPos2Name, lpPos1Name); break;
                            case TPT_CIRSUB_TAIL:
                            case TPT_CIRSUB_ASC_TAIL: sprintf(lpName, "%s%s环差尾", lpPos1Name, lpPos2Name); break;
                            case TPT_CIRSUB_DESC_TAIL: sprintf(lpName, "%s%s环差尾", lpPos2Name, lpPos1Name); break;
                            case TPT_MUL: sprintf(lpName, "%sＸ%s", lpPos1Name, lpPos2Name); break;
                            case TPT_MUL_TAIL: sprintf(lpName, "%sＸ%s尾", lpPos1Name, lpPos2Name); break;
                            case TPT_SUMSUB: sprintf(lpName, "%s%s合差", lpPos1Name, lpPos2Name); break;
                            case TPT_SUMCIRSUB: sprintf(lpName, "%s%s合环差", lpPos1Name, lpPos2Name); break;
                            default: return "未命名2位关系";
						}
					}
				}
			}
			else if(cdtIID.dwSystemIID >= IID_STC_TWONUMSIN_BEGIN && cdtIID.dwSystemIID <= IID_STC_TWONUMSIN_END)	// 单选二码
			{
				int nType = cdtIID.dwSystemIID - IID_STC_TWONUMSIN_BEGIN;
				int nPos1 = (nType / 10) % 10, nPos2 = nType % 10;
				sprintf(bGetShortName ? m_szShortName : m_szName, "%s%s位", m_strPosShortNames[nPos1], m_strPosShortNames[nPos2]);
			}
			else if(cdtIID.dwSystemIID >= IID_STC_REMM_BEGIN && cdtIID.dwSystemIID <= IID_STC_REMM_END)	// 余数指标
			{
				int nDivisor = (cdtIID.dwSystemIID - IID_STC_REMM_BEGIN) / 100 + 2;
				WORD wTempID = (WORD)(cdtIID.dwSystemIID - (nDivisor - 2) * 100);
				switch(wTempID)
				{
                    case IID_STC_REMM_POS_1: bGetShortName ? sprintf(m_szShortName, "%s除%d余", m_strPosShortNames[0], nDivisor) : sprintf(m_szName, "%s除%d余数", m_strPosNames[0], nDivisor); break;
                    case IID_STC_REMM_POS_2: bGetShortName ? sprintf(m_szShortName, "%s除%d余", m_strPosShortNames[1], nDivisor) : sprintf(m_szName, "%s除%d余数", m_strPosNames[1], nDivisor); break;
                    case IID_STC_REMM_POS_3: bGetShortName ? sprintf(m_szShortName, "%s除%d余", m_strPosShortNames[2], nDivisor) : sprintf(m_szName, "%s除%d余数", m_strPosNames[2], nDivisor); break;
                    case IID_STC_REMM_POS_4: bGetShortName ? sprintf(m_szShortName, "%s除%d余", m_strPosShortNames[3], nDivisor) : sprintf(m_szName, "%s除%d余数", m_strPosNames[3], nDivisor); break;
                    case IID_STC_REMM_POS_5: bGetShortName ? sprintf(m_szShortName, "%s除%d余", m_strPosShortNames[4], nDivisor) : sprintf(m_szName, "%s除%d余数", m_strPosNames[4], nDivisor); break;
                    case IID_STC_REMM_POS_6: bGetShortName ? sprintf(m_szShortName, "%s除%d余", m_strPosShortNames[5], nDivisor) : sprintf(m_szName, "%s除%d余数", m_strPosNames[5], nDivisor); break;
                    case IID_STC_REMM_POS_7: bGetShortName ? sprintf(m_szShortName, "%s除%d余", m_strPosShortNames[6], nDivisor) : sprintf(m_szName, "%s除%d余数", m_strPosNames[6], nDivisor); break;
                    case IID_STC_REMM_POS_8: bGetShortName ? sprintf(m_szShortName, "%s除%d余", m_strPosShortNames[7], nDivisor) : sprintf(m_szName, "%s除%d余数", m_strPosNames[7], nDivisor); break;
                    case IID_STC_REMM_POS_ANY: bGetShortName ? sprintf(m_szShortName, "任意除%d余", nDivisor) : sprintf(m_szName, "任意号码除%d余数", nDivisor); break;
                    case IID_STC_REMM_SUM: bGetShortName ? sprintf(m_szShortName, "除%d余和", nDivisor) : sprintf(m_szName, "除%d余数和", nDivisor); break;
                    case IID_STC_REMM_SUMTAIL: bGetShortName ? sprintf(m_szShortName, "除%d余合", nDivisor) : sprintf(m_szName, "除%d余数合", nDivisor); break;
                    case IID_STC_REMM_ARR_SIN: bGetShortName ? sprintf(m_szShortName, "除%d余单排", nDivisor) : sprintf(m_szName, "除%d余数单选排列", nDivisor); break;
                    case IID_STC_REMM_ARR_SIN_VALUE: bGetShortName ? sprintf(m_szShortName, "除%d余单排值", nDivisor) : sprintf(m_szName, "除%d余数单选排列值", nDivisor); break;
                    case IID_STC_REMM_ARR_GRO: bGetShortName ? sprintf(m_szShortName, "除%d余组排", nDivisor) : sprintf(m_szName, "除%d余数组选排列", nDivisor); break;
                    case IID_STC_REMM_ARR_GRO_VALUE: bGetShortName ? sprintf(m_szShortName, "除%d余组排值", nDivisor) : sprintf(m_szName, "除%d余数组选排列值", nDivisor); break;
                    case IID_STC_REMM_ARR_COUNT: bGetShortName ? sprintf(m_szShortName, "除%d余个排", nDivisor) : sprintf(m_szName, "除%d余数个数排列", nDivisor); break;
                    case IID_STC_REMM_ARR_COUNT_VALUE: bGetShortName ? sprintf(m_szShortName, "除%d余个排值", nDivisor) : sprintf(m_szName, "除%d余数个数排列值", nDivisor); break;
                    case IID_STC_REMM_MAXLINK: bGetShortName ? sprintf(m_szShortName, "除%d余最大邻出", nDivisor) : sprintf(m_szName, "除%d余数最大邻出", nDivisor); break;
                    case IID_STC_REMM_MAXNONE: bGetShortName ? sprintf(m_szShortName, "除%d余最大邻断", nDivisor) : sprintf(m_szName, "除%d余数最大邻断", nDivisor); break;
                    case IID_STC_REMM_COUNT_NONE: bGetShortName ? sprintf(m_szShortName, "除%d余未出个数", nDivisor) : sprintf(m_szName, "除%d余数未出个数", nDivisor); break;
                    case IID_STC_REMM_COUNT_MATCH: bGetShortName ? sprintf(m_szShortName, "除%d余已出个数", nDivisor) : sprintf(m_szName, "除%d余数已出个数", nDivisor); break;
                    case IID_STC_REMM_SHAPE: bGetShortName ? sprintf(m_szShortName, "除%d余形态", nDivisor) : sprintf(m_szName, "除%d余数形态", nDivisor); break;
                    default:
					{
						if(wTempID >= IID_STC_REMM_COUNT_N && wTempID < IID_STC_REMM_COUNT_N + nDivisor)
						{
							LPSTR lpName = bGetShortName ? m_szShortName : m_szName;
							sprintf(lpName, "除%d余%lu个数", nDivisor, wTempID - IID_STC_REMM_COUNT_N);
						}
						else
						{
							return "未命名余数指标";
						}
					} break;
				}
			}
			else if(cdtIID.dwSystemIID >= IID_DNC_XXX_BEGIN && cdtIID.dwSystemIID <= IID_DNC_XXX_END)	// 动态3态指标
			{
				int nTypeIndex = ((cdtIID.dwSystemIID - IID_DNC_XXX_BEGIN) / 100) % 10;
				LPSTR lpName = bGetShortName ? m_szShortName : m_szName;
				switch(cdtIID.dwSystemIID - nTypeIndex * 100)
				{
                    case IID_DNC_XXX_POS_1: BUILDINDICATORNAME_POS(0, NAMES_3STATE_DYNAMIC[nTypeIndex], NAMES_3STATE_DYNAMIC[nTypeIndex]); break;
                    case IID_DNC_XXX_POS_2: BUILDINDICATORNAME_POS(1, NAMES_3STATE_DYNAMIC[nTypeIndex], NAMES_3STATE_DYNAMIC[nTypeIndex]); break;
                    case IID_DNC_XXX_POS_3: BUILDINDICATORNAME_POS(2, NAMES_3STATE_DYNAMIC[nTypeIndex], NAMES_3STATE_DYNAMIC[nTypeIndex]); break;
                    case IID_DNC_XXX_POS_4: BUILDINDICATORNAME_POS(3, NAMES_3STATE_DYNAMIC[nTypeIndex], NAMES_3STATE_DYNAMIC[nTypeIndex]); break;
                    case IID_DNC_XXX_POS_5: BUILDINDICATORNAME_POS(4, NAMES_3STATE_DYNAMIC[nTypeIndex], NAMES_3STATE_DYNAMIC[nTypeIndex]); break;
                    case IID_DNC_XXX_POS_6: BUILDINDICATORNAME_POS(5, NAMES_3STATE_DYNAMIC[nTypeIndex], NAMES_3STATE_DYNAMIC[nTypeIndex]); break;
                    case IID_DNC_XXX_POS_7: BUILDINDICATORNAME_POS(6, NAMES_3STATE_DYNAMIC[nTypeIndex], NAMES_3STATE_DYNAMIC[nTypeIndex]); break;
                    case IID_DNC_XXX_POS_8: BUILDINDICATORNAME_POS(7, NAMES_3STATE_DYNAMIC[nTypeIndex], NAMES_3STATE_DYNAMIC[nTypeIndex]); break;
                    case IID_DNC_XXX_COUNT_0: memcpy(lpName, NAMES_3STATE_DYNAMIC[nTypeIndex], 2); memcpy(&lpName[2], "码个数", 6); lpName[8] = 0; break;
                    case IID_DNC_XXX_COUNT_1: memcpy(lpName, &NAMES_3STATE_DYNAMIC[nTypeIndex][2], 2); memcpy(&lpName[2], "码个数", 6); lpName[8] = 0; break;
                    case IID_DNC_XXX_COUNT_2: memcpy(lpName, &NAMES_3STATE_DYNAMIC[nTypeIndex][4], 2); memcpy(&lpName[2], "码个数", 6); lpName[8] = 0; break;
                    case IID_DNC_XXX_SHAPE: strcpy(lpName, NAMES_3STATE_DYNAMIC[nTypeIndex]); strcat(lpName, "形态"); break;
                    case IID_DNC_XXX_ARR_SIN: strcpy(lpName, NAMES_3STATE_DYNAMIC[nTypeIndex]); strcat(lpName, bGetShortName ? "单排" : "单选排列"); break;
                    case IID_DNC_XXX_ARR_SIN_VALUE: strcpy(lpName, NAMES_3STATE_DYNAMIC[nTypeIndex]); strcat(lpName, bGetShortName ? "单排值" : "单选排列值"); break;
                    case IID_DNC_XXX_ARR_GRO: strcpy(lpName, NAMES_3STATE_DYNAMIC[nTypeIndex]); strcat(lpName, bGetShortName ? "组排" : "组选排列"); break;
                    case IID_DNC_XXX_ARR_GRO_VALUE: strcpy(lpName, NAMES_3STATE_DYNAMIC[nTypeIndex]); strcat(lpName, bGetShortName ? "组排值" : "组选排列值"); break;
                    case IID_DNC_XXX_ARR_COUNT: strcpy(lpName, NAMES_3STATE_DYNAMIC[nTypeIndex]); strcat(lpName, bGetShortName ? "个排" : "个数排列"); break;
                    case IID_DNC_XXX_ARR_COUNT_VALUE: strcpy(lpName, NAMES_3STATE_DYNAMIC[nTypeIndex]); strcat(lpName, bGetShortName ? "个排值" : "个数排列值"); break;
                    default: return "未命名动态3态指标";
				}
			}
			else if(cdtIID.dwSystemIID >= IID_DNC_PREVDATA_BEGIN && cdtIID.dwSystemIID <= IID_DNC_PREVDATA_END)	// 邻期关系
			{
				int nTypeIndex = (cdtIID.dwSystemIID - IID_DNC_PREVDATA_BEGIN) / 100;
				LPSTR lpName = bGetShortName ? m_szShortName : m_szName;
				switch(cdtIID.dwSystemIID - nTypeIndex * 100)
				{
                    case IID_DNC_PREVDATA_MIN: strcpy(lpName, "最小"); strcat(lpName, NAMES_PREVDATA[nTypeIndex]); break;
                    case IID_DNC_PREVDATA_MID: strcpy(lpName, "中间"); strcat(lpName, NAMES_PREVDATA[nTypeIndex]); break;
                    case IID_DNC_PREVDATA_MAX: strcpy(lpName, "最大"); strcat(lpName, NAMES_PREVDATA[nTypeIndex]); break;
                    case IID_DNC_PREVDATA_ANY: strcpy(lpName, "任意"); strcat(lpName, NAMES_PREVDATA[nTypeIndex]); break;
                    case IID_DNC_PREVDATA_SUM: strcpy(lpName, NAMES_PREVDATA[nTypeIndex]); strcat(lpName, "和"); break;
                    case IID_DNC_PREVDATA_SUMTAIL: strcpy(lpName, NAMES_PREVDATA[nTypeIndex]); strcat(lpName, "合"); break;
                    case IID_DNC_PREVDATA_ARR_SIN: strcpy(lpName, NAMES_PREVDATA[nTypeIndex]); strcat(lpName, bGetShortName ? "单排" : "单选排列"); break;
                    case IID_DNC_PREVDATA_ARR_GRO: strcpy(lpName, NAMES_PREVDATA[nTypeIndex]); strcat(lpName, bGetShortName ? "组排" : "组选排列"); break;
                    default: // 指定位邻期关系
					{
						int nPos = cdtIID.dwSystemIID - nTypeIndex * 100 - IID_DNC_PREVDATA_POS_1;
						ASSERT(nPos >= 0 && nPos < 8);
						sprintf(lpName, "%s%s", (bGetShortName ? m_strPosShortNames[nPos] : m_strPosNames[nPos]), NAMES_PREVDATA[nTypeIndex]);
					}
				}
			}
			else
			{
				return "未命名条件";
			}
#ifdef _DEBUG
            //			CString strTemp;
            //			strTemp.Format("Error: In CIndicator::GetName, cdtIID.dwSystemIID=0X%04X not found!!!", cdtIID.dwSystemIID);
            //			AfxMessageBox(strTemp);
#endif
			return bGetShortName ? m_szShortName : m_szName;
		} break;
	}
}

LPCSTR CIndicator::_GetDataAreaIndicatorName(const CDTIID &cdtIID, BOOL bGetShortName, DWORD dwCustomData)
{
	ASSERT(dwCustomData != 0);
	
	LPDATAAREA lpDataArea = LPDATAAREA(dwCustomData);
	if(lpDataArea == NULL)
	{
		return "被删除的分区";
	}
	
	LPCSTR lpszAreaName = bGetShortName ? lpDataArea->szShortName : lpDataArea->szName;
	LPSTR lpszRtn = bGetShortName ? m_szShortName : m_szName;
    
	switch(cdtIID.stCustomIID.wUseType)
	{
        case CIDAUT_POS_1:
        case CIDAUT_POS_2:
        case CIDAUT_POS_3:
        case CIDAUT_POS_4:
        case CIDAUT_POS_5:
        case CIDAUT_POS_6:
        case CIDAUT_POS_7:
        case CIDAUT_POS_8: strcpy(lpszRtn, g_pICstPubFunc->GetPosName(cdtIID.stCustomIID.wUseType - CIDAUT_POS_1, bGetShortName)); strcat(lpszRtn, lpszAreaName); break;
        case CIDAUT_POS_ANY: sprintf(lpszRtn, "任意%s", lpszAreaName); break;
        case CIDAUT_ARR_SIN: bGetShortName ? sprintf(lpszRtn, "%s单排", lpszAreaName) : sprintf(lpszRtn, "%s单选排列", lpszAreaName); break;
        case CIDAUT_ARR_SIN_VALUE: bGetShortName ? sprintf(lpszRtn, "%s单排值", lpszAreaName) : sprintf(lpszRtn, "%s单选排列值", lpszAreaName); break;
        case CIDAUT_ARR_GRO: bGetShortName ? sprintf(lpszRtn, "%s组排", lpszAreaName) : sprintf(lpszRtn, "%s组选排列", lpszAreaName); break;
        case CIDAUT_ARR_GRO_VALUE: bGetShortName ? sprintf(lpszRtn, "%s组排值", lpszAreaName) : sprintf(lpszRtn, "%s组选排列值", lpszAreaName); break;
        case CIDAUT_ARR_COUNT: bGetShortName ? sprintf(lpszRtn, "%s个排", lpszAreaName) : sprintf(lpszRtn, "%s个数排列", lpszAreaName); break;
        case CIDAUT_ARR_COUNT_VALUE: bGetShortName ? sprintf(lpszRtn, "%s个排值", lpszAreaName) : sprintf(lpszRtn, "%s个数排列值", lpszAreaName); break;
        case CIDAUT_MAXLINK: bGetShortName ? sprintf(lpszRtn, "%s最大邻出", lpszAreaName) : sprintf(lpszRtn, "%s最大邻出", lpszAreaName); break;
        case CIDAUT_MAXNONE: bGetShortName ? sprintf(lpszRtn, "%s最大邻断", lpszAreaName) : sprintf(lpszRtn, "%s最大邻断", lpszAreaName); break;
        case CIDAUT_COUNT_NONE: sprintf(lpszRtn, "%s未出个数", lpszAreaName); break;
        case CIDAUT_COUNT_MATCH: sprintf(lpszRtn, "%s已出个数", lpszAreaName); break;
        case CIDAUT_SHAPE: sprintf(lpszRtn, "%s形态", lpszAreaName); break;
        default:
		{
			if(cdtIID.stCustomIID.wUseType >= CIDAUT_COUNT_N && cdtIID.stCustomIID.wUseType < CIDAUT_COUNT_N + lpDataArea->btAreaCount)
			{
				sprintf(lpszRtn, "%s个数", lpDataArea->szSubAreaNames[cdtIID.stCustomIID.wUseType - CIDAUT_COUNT_N]);
			}
			else
			{
				strcpy(lpszRtn, "未命名分区指标");
			}
		} break;
	}
    
	return lpszRtn;
}

