//
//  IndicatorParam_GetName.cpp
//  QCLottery
//
//  Created by 武 孙 on 12-5-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "IndicatorParam.h"

#define		BUILDINDICATORPARAMNAME(szName) {ASSERT(strlen(szName) < MAXLEN_INDICATORPARAM_NAME); return szName;}
#define		BUILDINDICATORPARAMEXPLAIN(szExplain) {strcpy(m_szExplain, szExplain);}

///////////////////////////////////////////////////////////////////////////////////////

LPCSTR CIndicatorParam::GetName(const CDTIPID &cdtIPID, DWORD dwCustomData)
{
	if(cdtIPID.btType & CDTIPID_TYPE_CUSTOM)		// 自定义指标参数
	{
		if(cdtIPID.btType & CDTIPID_TYPE_DATAAREA)		// 号码分区指标参数
		{
			LPDATAAREA lpDataArea = LPDATAAREA(dwCustomData);
			ASSERT(lpDataArea != NULL);
			return lpDataArea->szName;
		}
        
		LPCIP_HEAD lpCustomIP = LPCIP_HEAD(dwCustomData);
		switch(lpCustomIP->wFlag)
		{
            case CIPHF_AREA:
			{
				LPCIP_CUSTOMAREA lpCustomArea = LPCIP_CUSTOMAREA(lpCustomIP->lpDataBuf);
				ASSERT(lpCustomArea != NULL);
				return lpCustomArea->szName;
			} break;
            case CIPHF_REMAINDER:
			{
				LPCIP_CUSTOMREMAINDER lpCustomRem = (LPCIP_CUSTOMREMAINDER)lpCustomIP->lpDataBuf;
				sprintf(m_szNameTemp, "除%d余数", lpCustomRem->nDivisor);
				return m_szNameTemp;
			} break;
            default: ASSERT(FALSE); break;
		}
	}
    
	// 系统指标参数
	switch(cdtIPID.dwSystemIPID)
	{
        case IPID_NONE: m_szNameTemp[0] = 0; break;
        case IPID_S_ODDEVEN: BUILDINDICATORPARAMNAME("奇偶"); break;
        case IPID_S_BIGSMALL: BUILDINDICATORPARAMNAME("大小"); break;
        case IPID_S_DZX: BUILDINDICATORPARAMNAME("XZD"); break;
        case IPID_S_012: BUILDINDICATORPARAMNAME("012路"); break;
        case IPID_S_PRIMECOMPOSITE: BUILDINDICATORPARAMNAME("质合"); break;
        case IPID_S_TAIL: BUILDINDICATORPARAMNAME("尾数"); break;
        case IPID_S_ABS: BUILDINDICATORPARAMNAME("绝对值"); break;
        case IPID_S_ABS_TAIL: BUILDINDICATORPARAMNAME("绝对值尾数"); break;
        case IPID_S_SUM: BUILDINDICATORPARAMNAME("和值"); break;
        case IPID_S_SUM_TAIL: BUILDINDICATORPARAMNAME("合值"); break;
        case IPID_S_SUM_N: BUILDINDICATORPARAMNAME("N次和"); break;
        case IPID_S_SPAN: BUILDINDICATORPARAMNAME("跨度"); break;
        case IPID_S_SPAN_TAIL: BUILDINDICATORPARAMNAME("跨度尾数"); break;
        case IPID_S_AVG: BUILDINDICATORPARAMNAME("均值"); break;
        case IPID_S_AVG_TAIL: BUILDINDICATORPARAMNAME("均值尾数"); break;
        case IPID_S_MIN: BUILDINDICATORPARAMNAME("最小值"); break;
        case IPID_S_MIN_TAIL: BUILDINDICATORPARAMNAME("最小值尾数"); break;
        case IPID_S_MID: BUILDINDICATORPARAMNAME("中间值"); break;
        case IPID_S_MID_TAIL: BUILDINDICATORPARAMNAME("中间值尾数"); break;
        case IPID_S_MAX: BUILDINDICATORPARAMNAME("最大值"); break;
        case IPID_S_MAX_TAIL: BUILDINDICATORPARAMNAME("最大值尾数"); break;
        case IPID_D_DISTANCE: BUILDINDICATORPARAMNAME("距离"); break;
        case IPID_D_DISTANCE_TAIL: BUILDINDICATORPARAMNAME("距离尾"); break;
        case IPID_D_CIRCLE_DISTANCE: BUILDINDICATORPARAMNAME("环距"); break;
        case IPID_D_CIRCLE_DISTANCE_TAIL: BUILDINDICATORPARAMNAME("环距尾"); break;
        case IPID_D_ADJACENT_SUB: BUILDINDICATORPARAMNAME("邻期差"); break;
        case IPID_D_ADJACENT_SUM: BUILDINDICATORPARAMNAME("邻期和"); break;
        case IPID_D_ADJACENT_SUMTAIL: BUILDINDICATORPARAMNAME("邻期合"); break;
        case IPID_D_SPJ: BUILDINDICATORPARAMNAME("升平降"); break;
        case IPID_D_AXIS_SUB: BUILDINDICATORPARAMNAME("轴差"); break;
        case IPID_D_AXIS_DISTANCE: BUILDINDICATORPARAMNAME("轴距"); break;
        case IPID_D_AXIS_DISTANCE_TAIL: BUILDINDICATORPARAMNAME("轴距尾"); break;
        case IPID_D_SKIP: BUILDINDICATORPARAMNAME("遗漏"); break;
        case IPID_D_SKIP_TAIL: BUILDINDICATORPARAMNAME("遗漏尾"); break;
            
            // 以下是开奖号与试机号相关指标参数
        case IPID_D_TESTNUMS_COMPARE: BUILDINDICATORPARAMNAME("与试机号对照"); break;
        case IPID_D_TESTNUMS_DISTANCE: BUILDINDICATORPARAMNAME("与试机号距离"); break;
        case IPID_D_TESTNUMS_DISTANCE_TAIL: BUILDINDICATORPARAMNAME("与试机号距离尾"); break;
        case IPID_D_TESTNUMS_SUB: BUILDINDICATORPARAMNAME("与试机号差值"); break;
        case IPID_D_TESTNUMS_CIRSUB: BUILDINDICATORPARAMNAME("与试机号环差"); break;
        case IPID_D_TESTNUMS_CIRSUB_TAIL: BUILDINDICATORPARAMNAME("与试机号环差尾"); break;
            
        default:
		{
			if(cdtIPID.dwSystemIPID >= IPID_S_REMAINDER_0 && cdtIPID.dwSystemIPID <= IPID_S_REMAINDER_MAX)
			{
				// 指标参数为余数
				sprintf(m_szNameTemp, "除%lu余数", cdtIPID.dwSystemIPID - IPID_S_REMAINDER_0);
			}
			else if(cdtIPID.dwSystemIPID >= IPID_S_SUBAREA_0 && cdtIPID.dwSystemIPID <= IPID_S_SUBAREA_MAX)
			{
				// 指标参数为分区
				sprintf(m_szNameTemp, "系统%lu分区", cdtIPID.dwSystemIPID - IPID_S_SUBAREA_0);
			}
			else
			{
#ifdef _DEBUG
				CString strTemp;
				strTemp.Format("Error: In CIndicatorParam::GetName, cdtIPID.dwSystemIPID=0X%02X not found!!!", cdtIPID.dwSystemIPID);
				AfxMessageBox(strTemp);
#endif
			}
		} break;
	}
	
	// 返回
	CHECK_STRING_STACK(m_szNameTemp);
	return m_szNameTemp;
}

LPCSTR CIndicatorParam::GetExplain(const CDTIPID &cdtIPID)
{
	m_szExplain[0] = 0;
    
	if(cdtIPID.btType & CDTIPID_TYPE_CUSTOM)		// 自定义指标参数
	{
		// _GetExplain_Custom(cdtIPID);
	}
	else	// 系统指标参数
	{
		switch(cdtIPID.dwSystemIPID)
		{
            case IPID_D_DISTANCE: BUILDINDICATORPARAMEXPLAIN("当期号码的指标值减上期号码的指标值的绝对值"); break;
            case IPID_D_DISTANCE_TAIL: BUILDINDICATORPARAMEXPLAIN("距离（当期号码的指标值减上期号码的指标值的绝对值）的尾数"); break;
            case IPID_D_CIRCLE_DISTANCE: BUILDINDICATORPARAMEXPLAIN("当期号码的指标值和上期号码的指标值在该指标的取值范围循环表中右边的数减左边的数的差值"); break;
            case IPID_D_CIRCLE_DISTANCE_TAIL: BUILDINDICATORPARAMEXPLAIN("环距（当期号码的指标值和上期号码的指标值在该指标的取值范围循环表中右边的数减左边的数的差值）的尾数"); break;
            case IPID_D_ADJACENT_SUB: BUILDINDICATORPARAMEXPLAIN("当期号码的指标值减上期号码的指标值"); break;
            case IPID_D_ADJACENT_SUM: BUILDINDICATORPARAMEXPLAIN("当期号码的指标值加上期号码的指标值"); break;
            case IPID_D_ADJACENT_SUMTAIL: BUILDINDICATORPARAMEXPLAIN("邻期和（当期号码的指标值加上期号码的指标值）的尾数"); break;
            case IPID_D_SPJ: BUILDINDICATORPARAMEXPLAIN("当期号码的指标值与上期号码的指标值相比，如果前者大则是升，相等则是平，否则为降"); break;
            case IPID_D_AXIS_SUB: BUILDINDICATORPARAMEXPLAIN("当期号码的指标值和上期号码的指标值在该指标的取值范围循环表中的最小差值，从左边到右边计算为正数，否则为负数"); break;
            case IPID_D_AXIS_DISTANCE: BUILDINDICATORPARAMEXPLAIN("当期号码的指标值和上期号码的指标值在该指标的取值范围循环表中的最小差值的绝对值"); break;
            case IPID_D_AXIS_DISTANCE_TAIL: BUILDINDICATORPARAMEXPLAIN("轴距（当期号码的指标值和上期号码的指标值在该指标的取值范围循环表中的最小差值的绝对值）的尾数"); break;
            case IPID_D_SKIP: BUILDINDICATORPARAMEXPLAIN("当期号码的指标值没出现的期数值"); break;
            case IPID_D_SKIP_TAIL: BUILDINDICATORPARAMEXPLAIN("遗漏（当期号码的指标值没出现的期数值）的尾数"); break;
                
            default:
			{
				if(cdtIPID.dwSystemIPID >= IPID_S_SUBAREA_0 && cdtIPID.dwSystemIPID <= IPID_S_SUBAREA_MAX)
				{
					// 指标参数为分区
					sprintf(m_szExplain, "系统根据条件值自动划分的%lu分区", cdtIPID.dwSystemIPID - IPID_S_SUBAREA_0);
				}
				/*else if(cdtIPID.dwSystemIPID >= IPID_S_REMAINDER_0 && cdtIPID.dwSystemIPID <= IPID_S_REMAINDER_MAX)
                 {
                 // 指标参数为余数
                 sprintf(m_szExplain, "除%d余数", cdtIPID.dwSystemIPID - IPID_S_REMAINDER_0);
                 }*/
			} break;
		}
	}
    
	CHECK_STRING_STACK(m_szExplain);
	return m_szExplain[0] == 0 ? NULL : m_szExplain;
}

void CIndicatorParam::_GetExplain_Custom(const CDTIPID &cdtIPID)
{
	if(cdtIPID.btType & CDTIPID_TYPE_DATAAREA)
	{
        
	}
	else
	{
        
	}
}
