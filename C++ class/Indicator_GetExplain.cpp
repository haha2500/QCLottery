//
//  Indicator_GetExplain.cpp
//  QCLottery
//
//  Created by 武 孙 on 12-5-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "Indicator.h"

#define BUILDINDICATOREXPLAIN(szExplainIn) \
            strcpy(m_szExplain, szExplainIn);

LPCSTR CIndicator::GetExplain(const CDTIID &cdtIID, DWORD dwCustomData)
{
	m_szExplain[0] = 0;
    
	if(cdtIID.btType & CDTIID_TYPE_CUSTOM)	// 自定义指标
	{
		if(cdtIID.btType & CDTIID_TYPE_DATAAREA)	// 号码分区
		{
			return _GetValueExplain_DataArea(cdtIID, dwCustomData);
		}
        //	return m_szExplain;
		return NULL;
	}
    
	// 系统指标
	switch(cdtIID.dwSystemIID)
	{
        case IID_STC_NUMBER_SAMECOMBO: BUILDINDICATOREXPLAIN("号码中相同数字个数的组合，比如号码中含有2个和3个相同数字，则组合为23"); break;
        case IID_STC_NUMBER_MAXCOUNT: BUILDINDICATOREXPLAIN("号码中出现个数最多的数字"); break;
        case IID_STC_GENRE_TWOLINK: BUILDINDICATOREXPLAIN("2个相连的号码"); break;
            
        case IID_STC_SHAPE_EQUALDIFF: BUILDINDICATOREXPLAIN("是指号码按组选方式的等间隔连号"); break;
            
        case IID_STC_DHK_HD: BUILDINDICATOREXPLAIN("将合值和胆码进行对照，看是否相同"); break;
        case IID_STC_DHK_KD: BUILDINDICATOREXPLAIN("将跨度和胆码进行对照，看是否相同"); break;
        case IID_STC_DHK_HK: BUILDINDICATOREXPLAIN("将合值和跨度进行对照，看是否相同"); break;
        case IID_STC_DHK_HDKD: BUILDINDICATOREXPLAIN("将合值和胆码与跨度和胆码进行综合对照"); break;
        case IID_STC_DHK_HDKDHK: BUILDINDICATOREXPLAIN("将合值和胆码、跨度和胆码以及合值和跨度进行综合对照"); break;
            
        case IID_STC_POS_1: if(m_btNumberCount > 1) BUILDINDICATOREXPLAIN("号码中的第一个数字"); break;
        case IID_STC_POS_2: BUILDINDICATOREXPLAIN("号码中的第二个数字"); break;
        case IID_STC_POS_3: BUILDINDICATOREXPLAIN("号码中的第三个数字"); break;
        case IID_STC_POS_4: BUILDINDICATOREXPLAIN("号码中的第四个数字"); break;
        case IID_STC_POS_5: BUILDINDICATOREXPLAIN("号码中的第五个数字"); break;
        case IID_STC_POS_6: BUILDINDICATOREXPLAIN("号码中的第六个数字"); break;
        case IID_STC_POS_7: BUILDINDICATOREXPLAIN("号码中的第七个数字"); break;
        case IID_STC_POS_8: BUILDINDICATOREXPLAIN("号码中的第八个数字"); break;
        case IID_STC_POS_ANY: BUILDINDICATOREXPLAIN("号码中任意一个数字"); break;
            
        case IID_STC_SUM: BUILDINDICATOREXPLAIN("号码的各个数字相加的总和"); break;
        case IID_STC_SUMTAIL: BUILDINDICATOREXPLAIN("和值（号码的各个数字相加的总和）的尾数"); break;
        case IID_STC_SUM_N: BUILDINDICATOREXPLAIN("将号码的各个数字相加的值，如果大于9，则继续将该值的各个数字相加，直到小于等于9"); break;
        case IID_STC_SPAN: BUILDINDICATOREXPLAIN("号码中的最大数字减去最小数字的值"); break;
        case IID_STC_SPANTAIL: BUILDINDICATOREXPLAIN("跨度（号码中的最大数字减去最小数字的值）的尾数"); break;
        case IID_STC_AVG: BUILDINDICATOREXPLAIN("号码中的所有数字的平均值"); break;
        case IID_STC_AVGTAIL: BUILDINDICATOREXPLAIN("均值（号码中的所有数字的平均值）的尾数"); break;
        case IID_STC_AC: BUILDINDICATOREXPLAIN("号码中所有两个数字的正数差值的总数减去r-1（r 为投注号码数）的值"); break;
        case IID_STC_DETRACT: sprintf(m_szExplain, "号码中各个数字分别与%d-%d中所有数字相减的绝对值的最小值中的最大的一个；散度值越大说明号码的集中程度越高，反之说明号码越分散", g_pIData->GetMinNumber(), g_pIData->GetMaxNumber()); break;
            
        case IID_STC_TWONUM_Z6: BUILDINDICATOREXPLAIN("对组六号码中的二码进行详细分类"); break;
        case IID_STC_TWONUM_Z6_2: BUILDINDICATOREXPLAIN("对组六号码中的二码进行分类"); break;
        case IID_STC_TWONUM_Z3: BUILDINDICATOREXPLAIN("对组三号码中的二码进行详细分类"); break;
        case IID_STC_TWONUM_Z3_2: BUILDINDICATOREXPLAIN("对组三号码中的二码进行分类"); break;
            
        case IID_STC_SPACE_SIN: BUILDINDICATOREXPLAIN("单选第一间距、单选第二间距等中的最大值"); break;
        case IID_STC_SPACETAIL_SIN: BUILDINDICATOREXPLAIN("单选间距（单选第一间距、单选第二间距等中的最大值）的尾数"); break;
        case IID_STC_SPACE_SIN_HEAD: BUILDINDICATOREXPLAIN("指号码中第一位数字和数字0的差值"); break;
        case IID_STC_SPACE_SIN_TAIL: BUILDINDICATOREXPLAIN("指数字9和号码中第三位数字的差值"); break;
        case IID_STC_SPACE_SIN_1: BUILDINDICATOREXPLAIN("单选首间距和单选尾间距的总和"); break;
        case IID_STC_SPACE_SIN_2: BUILDINDICATOREXPLAIN("第一位数字和第二位数字之差的绝对值"); break;
        case IID_STC_SPACE_SIN_3: BUILDINDICATOREXPLAIN("第二位数字和第三位数字之差的绝对值"); break;
        case IID_STC_SPACE_SIN_4: BUILDINDICATOREXPLAIN("第三位数字和第四位数字之差的绝对值"); break;
        case IID_STC_SPACE_SIN_5: BUILDINDICATOREXPLAIN("第四位数字和第五位数字之差的绝对值"); break;
        case IID_STC_SPACE_SIN_6: BUILDINDICATOREXPLAIN("第五位数字和第六位数字之差的绝对值"); break;
        case IID_STC_SPACE_SIN_7: BUILDINDICATOREXPLAIN("第六位数字和第七位数字之差的绝对值"); break;
        case IID_STC_SPACE_SIN_8: BUILDINDICATOREXPLAIN("第七位数字和第八位数字之差的绝对值"); break;
        case IID_STC_SPACE_GRO: BUILDINDICATOREXPLAIN("组选第一间距、组选第二间距等中的最大值"); break;
        case IID_STC_SPACE_GRO_HEAD: BUILDINDICATOREXPLAIN("组选号码中第一位数字和数字0的差值"); break;
        case IID_STC_SPACE_GRO_TAIL: BUILDINDICATOREXPLAIN("数字9和组选号码中第三位数字的差值"); break;
        case IID_STC_SPACE_GRO_1: BUILDINDICATOREXPLAIN("组选首间距和组选尾间距的总和"); break;
        case IID_STC_SPACE_GRO_2: BUILDINDICATOREXPLAIN("组选号码中第一位数字和第二位数字之差的绝对值"); break;
        case IID_STC_SPACE_GRO_3: BUILDINDICATOREXPLAIN("组选号码中第二位数字和第三位数字之差的绝对值"); break;
        case IID_STC_SPACE_GRO_4: BUILDINDICATOREXPLAIN("组选号码中第三位数字和第四位数字之差的绝对值"); break;
        case IID_STC_SPACE_GRO_5: BUILDINDICATOREXPLAIN("组选号码中第四位数字和第五位数字之差的绝对值"); break;
        case IID_STC_SPACE_GRO_6: BUILDINDICATOREXPLAIN("组选号码中第五位数字和第六位数字之差的绝对值"); break;
        case IID_STC_SPACE_GRO_7: BUILDINDICATOREXPLAIN("组选号码中第六位数字和第七位数字之差的绝对值"); break;
        case IID_STC_SPACE_GRO_8: BUILDINDICATOREXPLAIN("组选号码中第七位数字和第八位数字之差的绝对值"); break;
            
        case IID_STC_NEARSPACE_12: BUILDINDICATOREXPLAIN("号码中第二个数字减第一个数字的值"); break;
        case IID_STC_NEARSPACE_23: BUILDINDICATOREXPLAIN("号码中第三个数字减第二个数字的值"); break;
        case IID_STC_NEARSPACE_34: BUILDINDICATOREXPLAIN("号码中第四个数字减第三个数字的值"); break;
        case IID_STC_NEARSPACE_45: BUILDINDICATOREXPLAIN("号码中第五个数字减第四个数字的值"); break;
        case IID_STC_NEARSPACE_56: BUILDINDICATOREXPLAIN("号码中第六个数字减第五个数字的值"); break;
        case IID_STC_NEARSPACE_67: BUILDINDICATOREXPLAIN("号码中第七个数字减第六个数字的值"); break;
        case IID_STC_NEARSPACE_78: BUILDINDICATOREXPLAIN("号码中第八个数字减第七个数字的值"); break;
        case IID_STC_NEARSPACE_HT: sprintf(m_szExplain, "%d－最后一个数字值＋第一个数字的值", g_pIData->GetAllNumberCount()); break;
            
        case IID_STC_1D_POSITIVE_SIN: BUILDINDICATOREXPLAIN("（百+十的和值尾数）+ （十+个的和值尾数）的和值尾数"); break;
        case IID_STC_1D_POSITIVE_GRO: BUILDINDICATOREXPLAIN("组选号码的（百+十的和值尾数）+ （十+个的和值尾数）的和值尾数"); break;
        case IID_STC_1D_NEGATIVE_SIN: BUILDINDICATOREXPLAIN("（百-十的绝对值）- （十-个的绝对值）的绝对值"); break;
        case IID_STC_1D_NEGATIVE_GRO: BUILDINDICATOREXPLAIN("组选号码的（百-十的绝对值）- （十-个的绝对值）的绝对值"); break;
        case IID_STC_1D_RPOSITIVE_SIN: BUILDINDICATOREXPLAIN("（百+十的和值尾数）- （十+个的和值尾数）的绝对值"); break;
        case IID_STC_1D_RPOSITIVE_GRO: BUILDINDICATOREXPLAIN("组选号码的（百+十的和值尾数）- （十+个的和值尾数）的绝对值"); break;
        case IID_STC_1D_RNEGATIVE_SIN: BUILDINDICATOREXPLAIN("（百-十的绝对值）+ （十-个的绝对值）的和值尾数"); break;
        case IID_STC_1D_RNEGATIVE_GRO: BUILDINDICATOREXPLAIN("组选号码的（百-十的绝对值）+ （十-个的绝对值）的和值尾数"); break;
            
        case IID_STC_MUL: BUILDINDICATOREXPLAIN("号码的各个数字相乘的值"); break;
        case IID_STC_MULTAIL: BUILDINDICATOREXPLAIN("积数（号码的各个数字相乘的值）的尾数"); break;
        case IID_STC_POSMULSUM: BUILDINDICATOREXPLAIN("号码中每个数字乘以它所在的位置数，再相加得到的数值"); break;
        case IID_STC_POSMULSUMTAIL: BUILDINDICATOREXPLAIN("位置积和（号码中每个数字乘以它所在的位置数，再相加得到的数值）的尾数"); break;
        case IID_STC_RESPOSMULSUM: BUILDINDICATOREXPLAIN("号码中每个数字乘以(号码的个数+1)与它所在的位置数的差值，再相加得到的数值"); break;
        case IID_STC_RESPOSMULSUMTAIL: BUILDINDICATOREXPLAIN("反位置积和（号码中每个数字乘以(号码的个数+1)与它所在的位置数的差值，再相加得到的数值）的尾数"); break;
            
        case IID_STC_ESPECIALSUM_ODD: BUILDINDICATOREXPLAIN("号码中所有奇数数字的总和"); break;
        case IID_STC_ESPECIALSUM_EVEN: BUILDINDICATOREXPLAIN("号码中所有偶数数字的总和"); break;
        case IID_STC_ESPECIALSUM_BIG: BUILDINDICATOREXPLAIN("号码中所有大数数字的总和"); break;
        case IID_STC_ESPECIALSUM_SMALL: BUILDINDICATOREXPLAIN("号码中所有小数数字的总和"); break;
        case IID_STC_ESPECIALSUM_PRIME: BUILDINDICATOREXPLAIN("号码中所有质数数字的总和"); break;
        case IID_STC_ESPECIALSUM_COMPOSITE: BUILDINDICATOREXPLAIN("号码中所有合数数字的总和"); break;
        case IID_STC_ESPECIALSUM_0: BUILDINDICATOREXPLAIN("号码中所有0路数字的总和"); break;
        case IID_STC_ESPECIALSUM_1: BUILDINDICATOREXPLAIN("号码中所有1路数字的总和"); break;
        case IID_STC_ESPECIALSUM_2: BUILDINDICATOREXPLAIN("号码中所有2路数字的总和"); break;
        case IID_STC_ESPECIALSUM_D: BUILDINDICATOREXPLAIN("号码中所有D数数字的总和"); break;
        case IID_STC_ESPECIALSUM_Z: BUILDINDICATOREXPLAIN("号码中所有Z数数字的总和"); break;
        case IID_STC_ESPECIALSUM_X: BUILDINDICATOREXPLAIN("号码中所有X数数字的总和"); break;
            
        case IID_STC_LINK_NUMCOUNT_AS: BUILDINDICATOREXPLAIN("号码中所有升序连号（号码中某一个数字比其后一个数字小1）的个数"); break;
        case IID_STC_LINK_GROCOUNT_AS: BUILDINDICATOREXPLAIN("号码中所有升序连号（号码中某一个数字比其后一个数字小1）的组数"); break;
        case IID_STC_LINK_NUMCOUNT_DS: BUILDINDICATOREXPLAIN("号码中所有降序连号（号码中某一个数字比其后一个数字大1）的个数"); break;
        case IID_STC_LINK_GROCOUNT_DS: BUILDINDICATOREXPLAIN("号码中所有降序连号（号码中某一个数字比其后一个数字大1）的组数"); break;
            /////////////////////////////////////////////////////////////////////////////////////////////////
            
        case IID_DNC_XL_VALUE: BUILDINDICATOREXPLAIN("上下两期号码在组选分布图中所能连成斜线的个数总和，斜连包括左斜连和右斜连"); break;
        case IID_DNC_XL_LEFT: BUILDINDICATOREXPLAIN("上下两期号码在组选分布图中所能连成左斜线的个数总和"); break;
        case IID_DNC_XL_RIGHT: BUILDINDICATOREXPLAIN("上下两期号码在组选分布图中所能连成右斜线的个数总和"); break;
        case IID_DNC_XL_COMBO: BUILDINDICATOREXPLAIN("左斜连个数（十位）和右斜连个数（个位）的组合"); break;
            
        case IID_DNC_DEPARTURE: BUILDINDICATOREXPLAIN("当期号码与上期号码中所有数字之差的绝对值的最小值中的最大的一个；偏度值越大则该期号码对上期号码的整体偏离程度越大，反之则越小"); break;
            
        case IID_DNC_TESTNUMS_REPEAT_CUR_CUR: BUILDINDICATOREXPLAIN("当期试机号与当期开奖号重复数字的个数"); break;
        case IID_DNC_TESTNUMS_REPEAT_PRE_CUR: BUILDINDICATOREXPLAIN("上期试机号与当期开奖号重复数字的个数"); break;
        case IID_DNC_TESTNUMS_REPEAT_COMPLEX: BUILDINDICATOREXPLAIN("当期开奖号与当期试机号以及上期开奖号重复数字个数的综合信息"); break;		
        case IID_DNC_TESTNUMS_REPEAT_COMBO: BUILDINDICATOREXPLAIN("当期开奖号与当期试机号重复数字个数（十位）和当期开奖号与上期开奖号重复数字个数（个位）的组合"); break;		
            
        case IID_DNC_SKIPSIN_1:
        case IID_DNC_SKIPSIN_2:
        case IID_DNC_SKIPSIN_3:
        case IID_DNC_SKIPSIN_4:
        case IID_DNC_SKIPSIN_5:
        case IID_DNC_SKIPSIN_6:
        case IID_DNC_SKIPSIN_7:
        case IID_DNC_SKIPSIN_8:
            if(m_btNumberCount == 1) 
            {
                BUILDINDICATOREXPLAIN("号码没出现的期数值")
            }
            else
            {
                sprintf(m_szExplain, "在区分开奖号位置（即单选）的情况下的%s数字没出现的期数值", m_strPosNames[cdtIID.dwSystemIID - IID_DNC_SKIPSIN_1]);
            }
            break;
            
        case IID_DNC_SKIPSIN_TAIL_1: 
        case IID_DNC_SKIPSIN_TAIL_2:
        case IID_DNC_SKIPSIN_TAIL_3:
        case IID_DNC_SKIPSIN_TAIL_4:
        case IID_DNC_SKIPSIN_TAIL_5:
        case IID_DNC_SKIPSIN_TAIL_6:
        case IID_DNC_SKIPSIN_TAIL_7:
        case IID_DNC_SKIPSIN_TAIL_8:
            if(m_btNumberCount == 1) 
            {
                BUILDINDICATOREXPLAIN("遗漏值（号码没出现的期数值）的尾数")
            }
            else
            {
                sprintf(m_szExplain, "%s单选遗漏（在区分开奖号位置（即单选）的情况下的%s数字没出现的期数值）的尾数", m_strPosNames[cdtIID.dwSystemIID - IID_DNC_SKIPSIN_TAIL_1], m_strPosNames[cdtIID.dwSystemIID - IID_DNC_SKIPSIN_TAIL_1]);
            }
            break;
            
        case IID_DNC_SKIPSIN_SUM: BUILDINDICATOREXPLAIN("号码的各个位置的单选遗漏值的总和"); break;
        case IID_DNC_SKIPSIN_SUM_AREA:
            if(m_btNumberCount == 1)
                BUILDINDICATOREXPLAIN("遗漏值（号码没出现的期数值）的系统分区")
                else
                    BUILDINDICATOREXPLAIN("单选遗漏和值（号码的各个位置的单选遗漏值的总和）的系统分区")
                    break;
        case IID_DNC_SKIPSIN_SUM_AREA10:
            if(m_btNumberCount == 1)
                BUILDINDICATOREXPLAIN("遗漏值（号码没出现的期数值）的系统十分区")
                else
                    BUILDINDICATOREXPLAIN("单选遗漏和值（号码的各个位置的单选遗漏值的总和）的系统十分区")
                    break;
        case IID_DNC_SKIPSIN_SUMTAIL: BUILDINDICATOREXPLAIN("单选遗漏和值（号码的各个位置的单选遗漏值的总和）的尾数"); break;
            
        case IID_DNC_SKIPGRO_1:
        case IID_DNC_SKIPGRO_2:
        case IID_DNC_SKIPGRO_3:
        case IID_DNC_SKIPGRO_4:
        case IID_DNC_SKIPGRO_5:
        case IID_DNC_SKIPGRO_6:
        case IID_DNC_SKIPGRO_7:
        case IID_DNC_SKIPGRO_8:
            sprintf(m_szExplain, "在不区分开奖号位置（即组选）的情况下的%s数字没出现的期数值", m_strPosNames[cdtIID.dwSystemIID - IID_DNC_SKIPGRO_1]); break;
            
        case IID_DNC_SKIPGRO_TAIL_1:
        case IID_DNC_SKIPGRO_TAIL_2:
        case IID_DNC_SKIPGRO_TAIL_3:
        case IID_DNC_SKIPGRO_TAIL_4:
        case IID_DNC_SKIPGRO_TAIL_5:
        case IID_DNC_SKIPGRO_TAIL_6:
        case IID_DNC_SKIPGRO_TAIL_7:
        case IID_DNC_SKIPGRO_TAIL_8:
            sprintf(m_szExplain, "%s组选遗漏（在不区分开奖号位置（即组选）的情况下的%s数字没出现的期数值）的尾数", m_strPosNames[cdtIID.dwSystemIID - IID_DNC_SKIPGRO_TAIL_1], m_strPosNames[cdtIID.dwSystemIID - IID_DNC_SKIPGRO_TAIL_1]); break;
            
        case IID_DNC_SKIPGRO_SUM: BUILDINDICATOREXPLAIN("号码的各个位置的组选遗漏值的总和"); break;
        case IID_DNC_SKIPGRO_SUM_AREA: BUILDINDICATOREXPLAIN("组选遗漏和值（号码的各个位置的组选遗漏值的总和）的系统分区"); break;
        case IID_DNC_SKIPGRO_SUM_AREA10: BUILDINDICATOREXPLAIN("组选遗漏和值（号码的各个位置的组选遗漏值的总和）的系统十分区"); break;
        case IID_DNC_SKIPGRO_SUMTAIL: BUILDINDICATOREXPLAIN("组选遗漏和值（号码的各个位置的组选遗漏值的总和）的尾数"); break;
            
            /////////////////////////////////////////////////////////////////////////////////////////////////
            
        default:
		{
			if(cdtIID.dwSystemIID >= IID_DNC_REPEAT_1TON && cdtIID.dwSystemIID <= IID_DNC_REPEAT_1TON + 9)	// 前N期重复号
			{
				if(cdtIID.dwSystemIID == IID_DNC_REPEAT_1TON)
					sprintf(m_szExplain, "当前号码与往前1期中所有号码相同数字的个数（重复数字算1个）");
				else
					sprintf(m_szExplain, "当前号码与往前1-%lu期中所有号码相同数字的个数（重复数字算1个）", cdtIID.dwSystemIID - IID_DNC_REPEAT_1TON + 1);
			}
			if(cdtIID.dwSystemIID >= IID_DNC_REPEAT_N && cdtIID.dwSystemIID <= IID_DNC_REPEAT_N + 9)		// 前第N期重复号
			{
				sprintf(m_szExplain, "当前号码与往前的第%lu期号码相同数字的个数（重复数字算多个）", cdtIID.dwSystemIID - IID_DNC_REPEAT_N + 1);
			}
            
            /*			if(cdtIID.dwSystemIID >= IID_STC_NUMBER_0NUM && cdtIID.dwSystemIID <= IID_STC_NUMBER_0NUM + 100)	// 数字
             {
             if(bGetShortName)
             sprintf(m_szShortName, "%d个数", cdtIID.dwSystemIID - IID_STC_NUMBER_0NUM);
             else
             sprintf(m_szName, "数字%d个数", cdtIID.dwSystemIID - IID_STC_NUMBER_0NUM);
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
             default: strcat(lpName, NAMES_TWOPOS[nTypeIndex]); break;
             }
             break;
             default: // 指定两位关系
             {
             int nPos1 = ((cdtIID.dwSystemIID - IID_STC_TWOPOS_BEGIN) / 10) % 10, nPos2 = (cdtIID.dwSystemIID - IID_STC_TWOPOS_BEGIN) % 10;
             CString *pPosNames = bGetShortName ? m_strPosShortNames : m_strPosNames;
             switch(nTypeIndex)
             {
             case TPT_SUM: sprintf(lpName, "%s＋%s", pPosNames[nPos1], pPosNames[nPos2]); break;
             case TPT_SUM_TAIL: sprintf(lpName, "%s＋%s尾", pPosNames[nPos1], pPosNames[nPos2]); break;
             case TPT_SUB_ASC: sprintf(lpName, "%s－%s", pPosNames[nPos1], pPosNames[nPos2]); break;
             case TPT_SUB_DESC: 
             case TPT_SUB: sprintf(lpName, "%s－%s", pPosNames[nPos2], pPosNames[nPos1]); break;
             case TPT_SUB_TAIL: sprintf(lpName, "%s－%s尾", pPosNames[nPos2], pPosNames[nPos1]);
             case TPT_SUBABS: sprintf(lpName, "|%s－%s|", pPosNames[nPos1], pPosNames[nPos2]); break;
             case TPT_SUBABS_TAIL: sprintf(lpName, "|%s－%s|尾", pPosNames[nPos1], pPosNames[nPos2]); break;
             case TPT_CIRSUB_ASC: sprintf(lpName, "%s%s环差", pPosNames[nPos1], pPosNames[nPos2]); break;
             case TPT_CIRSUB_DESC: 
             case TPT_CIRSUB: sprintf(lpName, "%s%s环差", pPosNames[nPos1], pPosNames[nPos2]); break;
             case TPT_CIRSUB_ASC_TAIL: sprintf(lpName, "%s%s环差尾", pPosNames[nPos1], pPosNames[nPos2]); break;
             case TPT_CIRSUB_DESC_TAIL: 
             case TPT_CIRSUB_TAIL: sprintf(lpName, "%s%s环差尾", pPosNames[nPos2], pPosNames[nPos1]); break;
             case TPT_MUL: sprintf(lpName, "%sＸ%s", pPosNames[nPos1], pPosNames[nPos2]); break;
             case TPT_MUL_TAIL: sprintf(lpName, "%sＸ%s尾", pPosNames[nPos1], pPosNames[nPos2]); break;
             case TPT_SUMSUB: sprintf(lpName, "%s%s合差", pPosNames[nPos1], pPosNames[nPos2]); break;
             case TPT_SUMCIRSUB: sprintf(lpName, "%s%s合环差", pPosNames[nPos1], pPosNames[nPos2]); break;
             default: return "未命名2位关系";
             }
             }
             }
             }
             else */if(cdtIID.dwSystemIID >= IID_STC_REMM_BEGIN && cdtIID.dwSystemIID <= IID_STC_REMM_END)	// 余数指标
             {
                 int nDivisor = (cdtIID.dwSystemIID - IID_STC_REMM_BEGIN) / 100 + 2;
                 WORD wTempID = (WORD)(cdtIID.dwSystemIID - (nDivisor - 2) * 100);
                 switch(wTempID)
                 {
                         /*				case IID_STC_REMM_POS_1: bGetShortName ? sprintf(m_szShortName, "%s除%d余", m_strPosShortNames[0], nDivisor) : sprintf(m_szName, "%s除%d余数", m_strPosNames[0], nDivisor); break;
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
                          case IID_STC_REMM_ARR_COUNT: bGetShortName ? sprintf(m_szShortName, "除%d余组排", nDivisor) : sprintf(m_szName, "除%d余数组选排列", nDivisor); break;
                          case IID_STC_REMM_ARR_COUNT_VALUE: bGetShortName ? sprintf(m_szShortName, "除%d余组排值", nDivisor) : sprintf(m_szName, "除%d余数组选排列值", nDivisor); break;*/
                     case IID_STC_REMM_MAXLINK: sprintf(m_szExplain, "在任意除%d余数中相邻余数出现个数的最大值", nDivisor); break;
                     case IID_STC_REMM_MAXNONE: sprintf(m_szExplain, "在任意除%d余数中相邻未出余数个数的最大值", nDivisor); break;
                         //				case IID_STC_REMM_COUNT_NONE: bGetShortName ? sprintf(m_szShortName, "除%d余未出个数", nDivisor) : sprintf(m_szName, "除%d余数未出个数", nDivisor); break;
                         //				case IID_STC_REMM_SHAPE: bGetShortName ? sprintf(m_szShortName, "除%d余形态", nDivisor) : sprintf(m_szName, "除%d余数形态", nDivisor); break;
                     default:
                     {
                         /*						if(wTempID >= IID_STC_REMM_COUNT_N && wTempID < IID_STC_REMM_COUNT_N + nDivisor)
                          {
                          LPSTR lpName = bGetShortName ? m_szShortName : m_szName;
                          sprintf(lpName, "除%d余%d个数", nDivisor, wTempID - IID_STC_REMM_COUNT_N);
                          }
                          else
                          {
                          return "未命名余数指标";
                          }*/
                         return NULL;
                     } break;
                 }
             }
			/*else if(cdtIID.dwSystemIID >= IID_DNC_XXX_BEGIN && cdtIID.dwSystemIID <= IID_DNC_XXX_END)	// 动态3态指标
             {
             int nTypeIndex = ((cdtIID.dwSystemIID - IID_DNC_XXX_BEGIN) / 100) % 10;
             LPSTR lpName = bGetShortName ? m_szShortName : m_szName;
             switch(cdtIID.dwSystemIID - nTypeIndex * 100)
             {
             case IID_DNC_XXX_POS_1: BUILDINDICATOREXPLAIN_POS(0, NAMES_3STATE_DYNAMIC[nTypeIndex], NAMES_3STATE_DYNAMIC[nTypeIndex]); break;
             case IID_DNC_XXX_POS_2: BUILDINDICATOREXPLAIN_POS(1, NAMES_3STATE_DYNAMIC[nTypeIndex], NAMES_3STATE_DYNAMIC[nTypeIndex]); break;
             case IID_DNC_XXX_POS_3: BUILDINDICATOREXPLAIN_POS(2, NAMES_3STATE_DYNAMIC[nTypeIndex], NAMES_3STATE_DYNAMIC[nTypeIndex]); break;
             case IID_DNC_XXX_POS_4: BUILDINDICATOREXPLAIN_POS(3, NAMES_3STATE_DYNAMIC[nTypeIndex], NAMES_3STATE_DYNAMIC[nTypeIndex]); break;
             case IID_DNC_XXX_POS_5: BUILDINDICATOREXPLAIN_POS(4, NAMES_3STATE_DYNAMIC[nTypeIndex], NAMES_3STATE_DYNAMIC[nTypeIndex]); break;
             case IID_DNC_XXX_POS_6: BUILDINDICATOREXPLAIN_POS(5, NAMES_3STATE_DYNAMIC[nTypeIndex], NAMES_3STATE_DYNAMIC[nTypeIndex]); break;
             case IID_DNC_XXX_POS_7: BUILDINDICATOREXPLAIN_POS(6, NAMES_3STATE_DYNAMIC[nTypeIndex], NAMES_3STATE_DYNAMIC[nTypeIndex]); break;
             case IID_DNC_XXX_POS_8: BUILDINDICATOREXPLAIN_POS(7, NAMES_3STATE_DYNAMIC[nTypeIndex], NAMES_3STATE_DYNAMIC[nTypeIndex]); break;
             case IID_DNC_XXX_COUNT_0: memcpy(lpName, NAMES_3STATE_DYNAMIC[nTypeIndex], 2); memcpy(&lpName[2], "码个数", 6); lpName[8] = 0; break;
             case IID_DNC_XXX_COUNT_1: memcpy(lpName, &NAMES_3STATE_DYNAMIC[nTypeIndex][2], 2); memcpy(&lpName[2], "码个数", 6); lpName[8] = 0; break;
             case IID_DNC_XXX_COUNT_2: memcpy(lpName, &NAMES_3STATE_DYNAMIC[nTypeIndex][4], 2); memcpy(&lpName[2], "码个数", 6); lpName[8] = 0; break;
             case IID_DNC_XXX_SHAPE: strcpy(lpName, NAMES_3STATE_DYNAMIC[nTypeIndex]); strcat(lpName, "形态"); break;
             case IID_DNC_XXX_ARR_SIN: strcpy(lpName, NAMES_3STATE_DYNAMIC[nTypeIndex]); strcat(lpName, bGetShortName ? "单排" : "单选排列"); break;
             case IID_DNC_XXX_ARR_SIN_VALUE: strcpy(lpName, NAMES_3STATE_DYNAMIC[nTypeIndex]); strcat(lpName, bGetShortName ? "单排值" : "单选排列值"); break;
             case IID_DNC_XXX_ARR_COUNT: strcpy(lpName, NAMES_3STATE_DYNAMIC[nTypeIndex]); strcat(lpName, bGetShortName ? "组排" : "组选排列"); break;
             case IID_DNC_XXX_ARR_COUNT_VALUE: strcpy(lpName, NAMES_3STATE_DYNAMIC[nTypeIndex]); strcat(lpName, bGetShortName ? "组排值" : "组选排列值"); break;
             default: return "未命名动态3态指标";
             }
             }*/
            /*	if(cdtIID.dwSystemIID >= IID_DNC_PREVDATA_BEGIN && cdtIID.dwSystemIID <= IID_DNC_PREVDATA_END)	// 邻期关系
             {
             int nTypeIndex = (cdtIID.dwSystemIID - IID_DNC_PREVDATA_BEGIN) / 100;
             switch(cdtIID.dwSystemIID - nTypeIndex * 100)
             {
             case IID_DNC_PREVDATA_MIN: 
             case IID_DNC_PREVDATA_MID:
             case IID_DNC_PREVDATA_MAX: 
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
             }*/
		} break; 
	}
    
	if(m_szExplain[0] == 0)
	{
		return NULL;
	}
    
	CHECK_STRING_STACK(m_szExplain);
	return m_szExplain;
}

LPCSTR CIndicator::_GetValueExplain_DataArea(const CDTIID &cdtIID, DWORD dwCustomData)
{
	ASSERT(cdtIID.btType & CDTIID_TYPE_DATAAREA);	// 分区指标
	LPDATAAREA lpCurDataArea = LPDATAAREA(dwCustomData);
    
	switch(cdtIID.stCustomIID.wUseType)
	{
        case CIDAUT_MAXLINK: sprintf(m_szExplain, "在任意%s中相邻分区出现个数的最大值", lpCurDataArea->szName); break;
        case CIDAUT_MAXNONE: sprintf(m_szExplain, "在任意%s中分区连续断区个数的最大值", lpCurDataArea->szName); break;
        default: return NULL;
	}
    
	return m_szExplain;
}
