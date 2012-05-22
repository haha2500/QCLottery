//
//  Indicator.cpp
//  QCLottery
//
//  Created by 武 孙 on 12-5-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "Indicator.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIndicator::CIndicator()
{
	m_btMinNumber = m_btMaxNumber = m_btNumberCount = m_btDataType = 0;
	m_lpNumsData = NULL;
	m_szName[0] = m_szShortName[0] = 0;
	ZeroMemory(m_szExplain, sizeof(m_szExplain));
    
	// 初始化最大个数的值域空间
	m_lpValueRangeTemp = LPCDTVALUERANGE(new BYTE[sizeof(CDTVALUERANGE) + sizeof(CDTVALUERANGEITEM) * (MAXCOUNT_VALUEITEM)]);
    
	// 初始化值指针空间
	m_lpValueTemp = LPCDTVALUE(new BYTE[sizeof(CDTVALUE) + sizeof(CDTVALUEITEM) * MAXBYTE]);
}

CIndicator::~CIndicator()
{
	if(m_lpValueRangeTemp != NULL)
	{
		delete []((LPBYTE)m_lpValueRangeTemp);
		m_lpValueRangeTemp = NULL;
	}
    
	if(m_lpValueTemp != NULL)
	{
		delete []((LPBYTE)m_lpValueTemp);
		m_lpValueTemp = NULL;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CIndicator::OnDataTypeChanged()
{
	m_btMinNumber = g_pIData->GetMinNumber();
	m_btMaxNumber = g_pIData->GetMaxNumber();
	m_btNumberCount = g_pIData->GetNumberCount();
	m_btDataType = g_pIData->GetDataType();
    
	// 初始化个数排列值和组选排列值索引值信息
	for(int i=2; i<=9; i++)
	{
		_InitIndexOfCountArrange((BYTE)i);
        /*		for(int j=1; j<=9; j++)
         {
         _InitIndexOfGroupArrange((BYTE)i, BYTE(j));
         }
         */	
    }
    
	for(int i=0; i<m_btNumberCount; i++)
	{
		m_strPosNames[i] = g_pICstPubFunc->GetPosName(i, FALSE);
		m_strPosShortNames[i] = g_pICstPubFunc->GetPosName(i, TRUE);
	}
}

BOOL CIndicator::IsValid(const CDTIID &cdtIID, DWORD dwCustomData)
{
	if(!(cdtIID.btType & CDTIID_TYPE_VALID))
	{
		return FALSE;
	}
	
	if(cdtIID.btType & CDTIID_TYPE_CUSTOM)	// 自定义指标
	{
		if(cdtIID.btType & CDTIID_TYPE_DATAAREA)	// 自定义号码分区
		{
			return _IsValid_DataArea(cdtIID, LPDATAAREA(dwCustomData));
		}
		ASSERT(FALSE);
		return FALSE;
	}
	
	// 以下为系统指标
	switch(cdtIID.dwSystemIID)
	{
        case IID_STC_GENRE_NORMAL: 
		{
			if(m_btDataType & DATA_TYPE_REPEATABLE)
				return (m_btNumberCount > 1 && m_btNumberCount < 5) ? TRUE : FALSE;
			else
				return m_btNumberCount == 2 ? TRUE : FALSE;
		} break;
#ifdef _PROFESSIONAL	// 专业版
        case IID_STC_GENRE_TRANS:
#endif
        case IID_STC_GENRE_TWOLINK: 
        case IID_STC_SHAPE_NORMAL:
#ifdef _PROFESSIONAL	// 专业版
        case IID_STC_SHAPE_EX:
#endif
        case IID_STC_SHAPE_EQUALDIFF:
        case IID_STC_SHAPE_AAB:
        case IID_STC_SHAPE_ABC:
        case IID_STC_SHAPE_ABD:
            return ((m_btNumberCount == 3) && (m_btDataType & DATA_TYPE_NUM_SZ)) ? TRUE : FALSE;
            
        case IID_STC_SHAPE_COUNT_SAMENUM:
        case IID_STC_SHAPE_COUNT_SAMEGRO:
        case IID_STC_SHAPE_COUNT_DIFFNUM:
            return ((m_btNumberCount >= 4) && (m_btDataType & DATA_TYPE_REPEATABLE)) ? TRUE : FALSE;
            
        case IID_STC_LINK_NUMCOUNT:
        case IID_STC_LINK_GROCOUNT:
        case IID_STC_LINK_COMBO: return (m_btNumberCount > 3) ? TRUE : FALSE;
        case IID_STC_LINK_NUMCOUNT_AS:
        case IID_STC_LINK_GROCOUNT_AS:
        case IID_STC_LINK_NUMCOUNT_DS:
        case IID_STC_LINK_GROCOUNT_DS: return ((m_btNumberCount > 3) && !(m_btDataType & DATA_TYPE_ORDERED)) ? TRUE : FALSE;
        case IID_STC_LINK_COUNT_ODD:
        case IID_STC_LINK_COUNT_EVEN: return ((m_btNumberCount > 2) && !(m_btDataType & DATA_TYPE_REPEATABLE)) ? TRUE : FALSE;
        case IID_STC_LINK_COUNT_PRIME:
        case IID_STC_LINK_COUNT_COMPOSITE: return FALSE; // 暂不支持
        case IID_STC_LINK_POS_2:
        case IID_STC_LINK_POS_3:
        case IID_STC_LINK_POS_4:
        case IID_STC_LINK_POS_5:
        case IID_STC_LINK_POS_6:
        case IID_STC_LINK_POS_7: return ((m_btNumberCount > 3) && (m_btDataType & DATA_TYPE_ORDERED) && (int(cdtIID.dwSystemIID - IID_STC_LINK_POS_2 + 2) < int(m_btNumberCount - 1))) ? TRUE : FALSE;
            
        case IID_STC_DHK_HD:
        case IID_STC_DHK_KD:
        case IID_STC_DHK_HK:
        case IID_STC_DHK_HDKD:
        case IID_STC_DHK_HDKDHK: return ((m_btNumberCount > 1) && (m_btDataType & DATA_TYPE_NUM_SZ)) ? TRUE : FALSE;
            
        case IID_STC_NUMS_SIN:
        case IID_STC_NUMS_GRO: return ((m_btNumberCount > 1 && m_btNumberCount <= 3) && (m_btDataType & DATA_TYPE_NUM_SZ)) ? TRUE : FALSE;
            
        case IID_STC_POS_1:
        case IID_STC_POS_2:
        case IID_STC_POS_3:
        case IID_STC_POS_4:
        case IID_STC_POS_5:
        case IID_STC_POS_6:
        case IID_STC_POS_7:
        case IID_STC_POS_8: return ((cdtIID.dwSystemIID - IID_STC_POS_1) < m_btNumberCount) ? TRUE : FALSE;
        case IID_STC_POS_ANY:  return (m_btNumberCount > 1) ? TRUE : FALSE;
            
        case IID_STC_SUM:
        case IID_STC_SPAN: 
        case IID_STC_AVG: return (m_btNumberCount > 1 || m_btMaxNumber > 9) ? TRUE : FALSE;
        case IID_STC_SUM_N:
        case IID_STC_SUMTAIL: return (m_btNumberCount > 1 || m_btMaxNumber > 18) ? TRUE : FALSE;
        case IID_STC_SPANTAIL: return ((m_btNumberCount > 1) && (m_btMaxNumber - m_btMinNumber) > 9) ? TRUE : FALSE;
        case IID_STC_AVGTAIL: return ((m_btNumberCount > 1) && (m_btMaxNumber > 9)) ? TRUE : FALSE;
        case IID_STC_AC: 
        case IID_STC_DETRACT:  return (m_btNumberCount > 2) ? TRUE : FALSE;
            
        case IID_STC_BIGSMALL_ARR_VALUE: return ((m_btNumberCount > 1) && !(m_btDataType & DATA_TYPE_ORDERED)) ? TRUE : FALSE;
            
        case IID_STC_ODDENEN_COUNT_O:
        case IID_STC_ODDENEN_COUNT_E:
        case IID_STC_ODDENEN_ARR:
        case IID_STC_ODDENEN_ARR_VALUE:
        case IID_STC_BIGSMALL_COUNT_B:
        case IID_STC_BIGSMALL_COUNT_S:
        case IID_STC_BIGSMALL_ARR:
            //	case IID_STC_BIGSMALL_ARR_VALUE:
        case IID_STC_PRIMECOMPOSITE_COUNT_P:
        case IID_STC_PRIMECOMPOSITE_COUNT_C:
        case IID_STC_PRIMECOMPOSITE_ARR:
        case IID_STC_PRIMECOMPOSITE_ARR_VALUE:
            //	case IID_STC_PRIMECOMPOSITE_LINK:	// TEST 暂时不支持
        case IID_STC_012_COUNT_0:
        case IID_STC_012_COUNT_1:
        case IID_STC_012_COUNT_2:
        case IID_STC_012_ARR_SIN:
        case IID_STC_012_ARR_SIN_VALUE:
        case IID_STC_012_ARR_GRO:
        case IID_STC_012_ARR_GRO_VALUE:
        case IID_STC_012_ARR_COUNT:
        case IID_STC_012_ARR_COUNT_VALUE:
        case IID_STC_DZX_COUNT_D:
        case IID_STC_DZX_COUNT_Z:
        case IID_STC_DZX_COUNT_X:
        case IID_STC_DZX_ARR_SIN:
        case IID_STC_DZX_ARR_SIN_VALUE:
        case IID_STC_DZX_ARR_GRO:
        case IID_STC_DZX_ARR_GRO_VALUE:
        case IID_STC_DZX_ARR_COUNT:
        case IID_STC_DZX_ARR_COUNT_VALUE:
        case IID_STC_ESPECIALSUM_ODD:
        case IID_STC_ESPECIALSUM_EVEN:
        case IID_STC_ESPECIALSUM_BIG:
        case IID_STC_ESPECIALSUM_SMALL:
        case IID_STC_ESPECIALSUM_PRIME:
        case IID_STC_ESPECIALSUM_COMPOSITE:
        case IID_STC_ESPECIALSUM_0:
        case IID_STC_ESPECIALSUM_1:
        case IID_STC_ESPECIALSUM_2:
        case IID_STC_ESPECIALSUM_D:
        case IID_STC_ESPECIALSUM_Z:
        case IID_STC_ESPECIALSUM_X:
            return (m_btNumberCount > 1) ? TRUE : FALSE;
            
        case IID_STC_ODDENEN_LINK:
            return (m_btNumberCount > 1 && (m_btDataType & DATA_TYPE_NUM_SZ)) ? TRUE : FALSE;
            
        case IID_STC_TWONUM_ANY:
        case IID_STC_TWONUMSIN_ANY:
            return (m_btNumberCount > 2 && (m_btDataType & DATA_TYPE_NUM_SZ)) ? TRUE : FALSE;
            
        case IID_STC_TWONUM_Z6:
        case IID_STC_TWONUM_Z6_2:
        case IID_STC_TWONUM_Z3:
        case IID_STC_TWONUM_Z3_2:
            return ((m_btNumberCount == 3) && (m_btDataType & DATA_TYPE_REPEATABLE)) ? TRUE : FALSE;
            
        case IID_STC_012_SHAPE:
        case IID_STC_DZX_SHAPE: 
            return (m_btNumberCount == 3) ? TRUE : FALSE;
            
        case IID_STC_ORDER_MIN:
        case IID_STC_ORDER_MID: 
        case IID_STC_ORDER_MAX:
        case IID_STC_ORDER_MINADDMAX:
        case IID_STC_ORDER_MAXSUBMIN: return (!(m_btDataType & DATA_TYPE_ORDERED) && (m_btNumberCount >= 3)) ? TRUE : FALSE;
        case IID_STC_ORDER_MINADDMAX_TAIL: return (!(m_btDataType & DATA_TYPE_ORDERED) && (m_btNumberCount >= 3) && (m_btMaxNumber >= 5)) ? TRUE : FALSE;
        case IID_STC_ORDER_MAXSUBMIN_TAIL: return (!(m_btDataType & DATA_TYPE_ORDERED) && (m_btNumberCount >= 3) && (m_btMaxNumber - m_btMinNumber > 9)) ? TRUE : FALSE;
            
        case IID_STC_ORDER_MINADDMID: 
        case IID_STC_ORDER_MIDADDMAX: 
        case IID_STC_ORDER_MINADDMID_TAIL: 
        case IID_STC_ORDER_MIDADDMAX_TAIL:
        case IID_STC_ORDER_MIDSUBMIN: 
        case IID_STC_ORDER_MAXSUBMID: 
#ifdef _PROFESSIONAL	// 专业版
        case IID_STC_ORDER_RELATIVEDZX_POS_1: 
        case IID_STC_ORDER_RELATIVEDZX_POS_2: 
        case IID_STC_ORDER_RELATIVEDZX_POS_3: 
        case IID_STC_ORDER_RELATIVEDZX_ARR: 
        case IID_STC_ORDER_RELATIVEDZX_ARR_VALUE: 
#endif
            //	case IID_STC_ORDER_POSSUM_ARR_SIN:
            //	case IID_STC_ORDER_POSSUM_ARR_GRO:
        case IID_STC_ORDER_POSSUMTAIL_ARR_SIN:
        case IID_STC_ORDER_POSSUMTAIL_ARR_GRO:
        case IID_STC_ORDER_POSSUB_ARR_SIN:
        case IID_STC_ORDER_POSSUB_ARR_GRO: return (!(m_btDataType & DATA_TYPE_ORDERED) && (m_btNumberCount == 3)) ? TRUE : FALSE;
            
        case IID_STC_ORDER_POSSUBTAIL_ARR_SIN:
        case IID_STC_ORDER_POSSUBTAIL_ARR_GRO:
        case IID_STC_ORDER_MIDSUBMIN_TAIL: 
        case IID_STC_ORDER_MAXSUBMID_TAIL: return (!(m_btDataType & DATA_TYPE_ORDERED) && (m_btNumberCount == 3) && (m_btMaxNumber - m_btMinNumber > 9)) ? TRUE : FALSE;
            
#ifdef _PROFESSIONAL	// 专业版
        case IID_STC_SPACE_SIN:
        case IID_STC_SPACETAIL_SIN:
        case IID_STC_SPACE_SIN_HEAD:
        case IID_STC_SPACE_SIN_TAIL:
        case IID_STC_SPACE_SIN_1:
        case IID_STC_SPACE_SIN_2:
        case IID_STC_SPACE_GRO:
        case IID_STC_SPACE_GRO_HEAD:
        case IID_STC_SPACE_GRO_TAIL:
        case IID_STC_SPACE_GRO_1:
        case IID_STC_SPACE_GRO_2: return ((m_btNumberCount > 1) && (m_btDataType & DATA_TYPE_NUM_SZ)) ? TRUE : FALSE;
        case IID_STC_SPACE_SIN_3:
        case IID_STC_SPACE_SIN_4:
        case IID_STC_SPACE_SIN_5:
        case IID_STC_SPACE_SIN_6:
        case IID_STC_SPACE_SIN_7:
        case IID_STC_SPACE_SIN_8:
            return ((m_btNumberCount > 1) && (m_btDataType & DATA_TYPE_NUM_SZ) && (cdtIID.dwSystemIID - IID_STC_SPACE_SIN_1 < m_btNumberCount)) ? TRUE : FALSE;
        case IID_STC_SPACE_GRO_3:
        case IID_STC_SPACE_GRO_4:
        case IID_STC_SPACE_GRO_5:
        case IID_STC_SPACE_GRO_6:
        case IID_STC_SPACE_GRO_7:
        case IID_STC_SPACE_GRO_8:
            return ((m_btNumberCount > 1) && (m_btDataType & DATA_TYPE_NUM_SZ) && (cdtIID.dwSystemIID - IID_STC_SPACE_GRO_1 < m_btNumberCount)) ? TRUE : FALSE;
#endif
            
        case IID_STC_NEARSPACE_12:
        case IID_STC_NEARSPACE_23:
        case IID_STC_NEARSPACE_34:
        case IID_STC_NEARSPACE_45:
        case IID_STC_NEARSPACE_56:
        case IID_STC_NEARSPACE_67:
        case IID_STC_NEARSPACE_78:
            return ((m_btNumberCount > 2) && (cdtIID.dwSystemIID - IID_STC_NEARSPACE_12 + 1 < m_btNumberCount) && (m_btDataType & DATA_TYPE_ORDERED)) ? TRUE : FALSE;
            
        case IID_STC_NEARSPACE_HT:
        case IID_STC_NEARSPACE_MAX:
        case IID_STC_NEARSPACE_MAXEX:
        case IID_STC_NEARSPACE_MIN:
        case IID_STC_NEARSPACE_MINEX:
        case IID_STC_NEARSPACE_AVG:
        case IID_STC_NEARSPACE_ANY:
            return ((m_btNumberCount > 2) && (m_btDataType & DATA_TYPE_ORDERED)) ? TRUE : FALSE;
            
#ifdef _PROFESSIONAL	// 专业版
        case IID_STC_1D_POSITIVE_SIN:
        case IID_STC_1D_POSITIVE_GRO:
        case IID_STC_1D_NEGATIVE_SIN:
        case IID_STC_1D_NEGATIVE_GRO:
        case IID_STC_1D_RPOSITIVE_SIN:
        case IID_STC_1D_RPOSITIVE_GRO:
        case IID_STC_1D_RNEGATIVE_SIN:
        case IID_STC_1D_RNEGATIVE_GRO:
            return ((m_btNumberCount == 3) && (m_btDataType & DATA_TYPE_NUM_SZ)) ? TRUE : FALSE;
            
        case IID_STC_MUL:
            return ((m_btNumberCount > 1 && m_btNumberCount < 6) && (m_btDataType & DATA_TYPE_NUM_SZ)) ? TRUE : FALSE;
        case IID_STC_MUL_ODDEVEN:
        case IID_STC_MUL_BIGSMALL:
        case IID_STC_MUL_012:
        case IID_STC_MULTAIL:
        case IID_STC_POSMULSUM:
        case IID_STC_POSMULSUMTAIL:
        case IID_STC_RESPOSMULSUM:
        case IID_STC_RESPOSMULSUMTAIL:
            return (m_btNumberCount > 1 && m_btNumberCount < 8 && (m_btDataType & DATA_TYPE_NUM_SZ)) ? TRUE : FALSE;
#endif
            
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////
        case IID_DNC_DIS_POS_1:
        case IID_DNC_DIS_POS_2:
        case IID_DNC_DIS_POS_3:
        case IID_DNC_DIS_POS_4:
        case IID_DNC_DIS_POS_5:
        case IID_DNC_DIS_POS_6:
        case IID_DNC_DIS_POS_7:
        case IID_DNC_DIS_POS_8:	return ((cdtIID.dwSystemIID - IID_DNC_DIS_POS_1) < m_btNumberCount) ? TRUE : FALSE;
            
        case IID_DNC_DIS_SUM:
        case IID_DNC_DIS_SUMTAIL: 
        case IID_DNC_DIS_POS_ANY: 
        case IID_DNC_DIS_ARR_SIN:
        case IID_DNC_DIS_ARR_GRO:
            return (m_btNumberCount > 1) ? TRUE : FALSE;
            
        case IID_DNC_XL_VALUE:
        case IID_DNC_XL_LEFT:
        case IID_DNC_XL_RIGHT:
        case IID_DNC_XL_COMBO:
            return (m_btNumberCount > 1) ? TRUE : FALSE;
            
        case IID_DNC_DEPARTURE: return (m_btNumberCount > 2) ? TRUE : FALSE;
            
        case IID_DNC_NUMS_MATCHCOUNT_SIN:
        case IID_DNC_NUMS_MATCHCOUNT_GRO: return ((m_btNumberCount > 1 && m_btNumberCount <= 3) && (m_btDataType & DATA_TYPE_NUM_SZ)) ? TRUE : FALSE;
            
        case IID_DNC_TESTNUMS_REPEAT_CUR_CUR:
        case IID_DNC_TESTNUMS_REPEAT_PRE_CUR:
        case IID_DNC_TESTNUMS_REPEAT_COMPLEX:
            //	return ((m_btNumberCount <= 3) && (g_pIData->GetLotteryProperty()->btProperty & CSTLPP_TESTNUMS)) ? TRUE : FALSE;
            return ((m_btNumberCount <= 3) && (m_btDataType & DATA_TYPE_NUM_SZ)) ? TRUE : FALSE;
        case IID_DNC_TESTNUMS_REPEAT_COMBO:
            //	return ((m_btNumberCount > 1 && m_btNumberCount <= 3) && (g_pIData->GetLotteryProperty()->btProperty & CSTLPP_TESTNUMS)) ? TRUE : FALSE;
            return ((m_btNumberCount > 1 && m_btNumberCount <= 3) && (m_btDataType & DATA_TYPE_NUM_SZ)) ? TRUE : FALSE;
            
        case IID_DNC_SKIPSIN_1:
        case IID_DNC_SKIPSIN_2:
        case IID_DNC_SKIPSIN_3:
        case IID_DNC_SKIPSIN_4:
        case IID_DNC_SKIPSIN_5:
        case IID_DNC_SKIPSIN_6:
        case IID_DNC_SKIPSIN_7:
        case IID_DNC_SKIPSIN_8:	return ((cdtIID.dwSystemIID - IID_DNC_SKIPSIN_1) < m_btNumberCount) ? TRUE : FALSE;
        case IID_DNC_SKIPSIN_MIN:
        case IID_DNC_SKIPSIN_MAX: return (m_btNumberCount > 1) ? TRUE : FALSE;
            
        case IID_DNC_SKIPSIN_TAIL_1:
        case IID_DNC_SKIPSIN_TAIL_2:
        case IID_DNC_SKIPSIN_TAIL_3:
        case IID_DNC_SKIPSIN_TAIL_4:
        case IID_DNC_SKIPSIN_TAIL_5:
        case IID_DNC_SKIPSIN_TAIL_6:
        case IID_DNC_SKIPSIN_TAIL_7:
        case IID_DNC_SKIPSIN_TAIL_8:	return ((cdtIID.dwSystemIID - IID_DNC_SKIPSIN_TAIL_1) < m_btNumberCount) ? TRUE : FALSE;
        case IID_DNC_SKIPSIN_TAIL_MIN:
        case IID_DNC_SKIPSIN_TAIL_MAX:
        case IID_DNC_SKIPSIN_TAIL_ANY:
        case IID_DNC_SKIPSIN_TAIL_ARR_SIN:
        case IID_DNC_SKIPSIN_TAIL_ARR_GRO: 
        case IID_DNC_SKIPSIN_SUM:
        case IID_DNC_SKIPSIN_SUMTAIL:
            return (m_btNumberCount > 1) ? TRUE : FALSE;
        case IID_DNC_SKIPSIN_SUM_AREA:
        case IID_DNC_SKIPSIN_SUM_AREA10:
            return TRUE;
            
        case IID_DNC_SKIPGRO_1:
        case IID_DNC_SKIPGRO_2:
        case IID_DNC_SKIPGRO_3:
        case IID_DNC_SKIPGRO_4:
        case IID_DNC_SKIPGRO_5:
        case IID_DNC_SKIPGRO_6:
        case IID_DNC_SKIPGRO_7:
        case IID_DNC_SKIPGRO_8:	return (m_btNumberCount > 1 && (cdtIID.dwSystemIID - IID_DNC_SKIPGRO_1) < m_btNumberCount) ? TRUE : FALSE;
        case IID_DNC_SKIPGRO_MIN:
        case IID_DNC_SKIPGRO_MAX: return (m_btNumberCount > 1) ? TRUE : FALSE;
            
        case IID_DNC_SKIPGRO_TAIL_1:
        case IID_DNC_SKIPGRO_TAIL_2:
        case IID_DNC_SKIPGRO_TAIL_3:
        case IID_DNC_SKIPGRO_TAIL_4:
        case IID_DNC_SKIPGRO_TAIL_5:
        case IID_DNC_SKIPGRO_TAIL_6:
        case IID_DNC_SKIPGRO_TAIL_7:
        case IID_DNC_SKIPGRO_TAIL_8:	return (m_btNumberCount > 1 && (cdtIID.dwSystemIID - IID_DNC_SKIPGRO_TAIL_1) < m_btNumberCount) ? TRUE : FALSE;
        case IID_DNC_SKIPGRO_TAIL_MIN:
        case IID_DNC_SKIPGRO_TAIL_MAX:
        case IID_DNC_SKIPGRO_TAIL_ANY:
        case IID_DNC_SKIPGRO_TAIL_ARR_SIN:
        case IID_DNC_SKIPGRO_TAIL_ARR_GRO: 
        case IID_DNC_SKIPGRO_SUM:
        case IID_DNC_SKIPGRO_SUMTAIL:
        case IID_DNC_SKIPGRO_SUM_AREA:
        case IID_DNC_SKIPGRO_SUM_AREA10:
            return (m_btNumberCount > 1) ? TRUE : FALSE;
            
        case IID_HIS_NUMS_ARR_SIN: return TRUE;
        case IID_HIS_NUMS_ARR_GRO: return (m_btDataType & DATA_TYPE_ORDERED) ? FALSE : TRUE;
        case IID_HIS_NUMS_1_ARR_SIN: return (m_btDataType & DATA_TYPE_NUM_SZ && m_btNumberCount > 1) ? TRUE : FALSE;
        case IID_HIS_NUMS_1_ARR_GRO: return ((m_btDataType & DATA_TYPE_ORDERED) || (m_btNumberCount <= 1) || !(m_btDataType & DATA_TYPE_NUM_SZ)) ? FALSE : TRUE;
        case IID_HIS_NUMS_2_ARR_SIN: return (m_btDataType & DATA_TYPE_NUM_SZ && m_btNumberCount > 2) ? TRUE : FALSE;
        case IID_HIS_NUMS_2_ARR_GRO: return ((m_btDataType & DATA_TYPE_ORDERED) || (m_btNumberCount <= 2) || !(m_btDataType & DATA_TYPE_NUM_SZ)) ? FALSE : TRUE;
            
            // TEST
            /*	case IID_HSS_NUMS_SIN: return TRUE;
             case IID_HSS_NUMS_GRO: return (m_btDataType & DATA_TYPE_ORDERED) ? FALSE : TRUE;
             case IID_HSS_NUM2_SIN: return (m_btNumberCount > 1) ? TRUE : FALSE;
             case IID_HSS_NUM2_GRO: return (m_btDataType & DATA_TYPE_ORDERED) ? FALSE : ((m_btNumberCount > 1) ? TRUE : FALSE);
             */
        default:
		{
			if(cdtIID.dwSystemIID >= IID_STC_NUMBER_0NUM && cdtIID.dwSystemIID <= IID_STC_NUMBER_SAMEGROUP_8)		// 数字
			{
				if((m_btDataType & DATA_TYPE_REPEATABLE) && m_btNumberCount > 1)
				{
					if(cdtIID.dwSystemIID >= IID_STC_NUMBER_SAMECOMBO)
					{
						if((m_btNumberCount < 4) || (cdtIID.dwSystemIID - IID_STC_NUMBER_SAMEGROUP_2 + 2 > (DWORD)_GetMaxSameNumberCount()))
						{
							return FALSE;
						}
						return TRUE;
					}
                    
					int nTemp = cdtIID.dwSystemIID - IID_STC_NUMBER_0NUM;
					if(nTemp >= m_btMinNumber && nTemp <= m_btMaxNumber)
						return TRUE;
				}
			}
			else if(cdtIID.dwSystemIID >= IID_DNC_REPEAT_1TON && cdtIID.dwSystemIID <= IID_DNC_REPEAT_N + 9)	// 重复号
			{
				return (m_btNumberCount > 1) ? TRUE : FALSE;
			}
			else if(cdtIID.dwSystemIID >= IID_STC_TWOPOS_BEGIN && cdtIID.dwSystemIID <= IID_STC_TWOPOS_END)	// 2位关系
			{
				if(m_btNumberCount < 3)
				{
					return FALSE;		// 必须大于等于3个数字
				}
				int nType = (cdtIID.dwSystemIID - IID_STC_TWOPOS_BEGIN) / 100;
#ifndef _PROFESSIONAL	// 非专业版
				if(nType >= TPT_CIRSUB)
					return FALSE;
#endif
				DWORD dwTempID = WORD(cdtIID.dwSystemIID - nType * 100);
				BOOL b0To9 = FALSE;
				switch(nType)
				{
                    case TPT_SUM:			// 位和，都可以
                    case TPT_SUMSUB:		// 二码合差，都可以
                    case TPT_SUMCIRSUB:		// 二码合环差，都可以
                        break;
                    case TPT_SUB:			// 位差，只用于排序型数据
                        if(!(m_btDataType & DATA_TYPE_ORDERED)) return FALSE;
                        break;
                    case TPT_SUB_ASC:		// 顺位差，只用于不排序型数据
                    case TPT_SUB_DESC:		// 逆位差，只用于不排序型数据
                        if(m_btDataType & DATA_TYPE_ORDERED) return FALSE;
                        break;
                    case TPT_MUL:			// 位积，只用于数字型彩票
                        if(!(m_btDataType & DATA_TYPE_NUM_SZ)) return FALSE;
                        break;
                    case TPT_SUM_TAIL:		// 位合，都可以
                        b0To9 = TRUE; break;
                    case TPT_MUL_TAIL:		// 位积尾数，只用于数字型数据
                        if(!(m_btDataType & DATA_TYPE_NUM_SZ)) return FALSE;
                        b0To9 = TRUE; break;
                    case TPT_SUBABS:		// 位差绝对值，只用于不排序型数据
                        if(m_btDataType & DATA_TYPE_ORDERED) return FALSE;
                        if(m_btMaxNumber - m_btMinNumber < 10) b0To9 = TRUE; 
                        break;
                    case TPT_CIRSUB:		// 位环差，只用于排序型数据
                        if(!(m_btDataType & DATA_TYPE_ORDERED)) return FALSE;
                        if(m_btMaxNumber - m_btMinNumber < 10) b0To9 = TRUE; 
                        break;
                    case TPT_CIRSUB_ASC:	// 顺位环差，只用于不排序型数据
                    case TPT_CIRSUB_DESC:	// 逆位环差，只用于不排序型数据
                        if(m_btDataType & DATA_TYPE_ORDERED) return FALSE;
                        if(m_btMaxNumber - m_btMinNumber < 10) b0To9 = TRUE; 
                        break;
                    case TPT_SUB_TAIL:		// 位差尾数，只用于排序型数据和含有大于10的值
                        if(!(m_btDataType & DATA_TYPE_ORDERED)) return FALSE;
                        if(m_btMaxNumber - m_btMinNumber < 10) return FALSE;
                        b0To9 = TRUE;
                        break;
                    case TPT_SUBABS_TAIL:	// 位差绝对值尾数，只用于不排序型数据和含有大于10的值
                        if(m_btDataType & DATA_TYPE_ORDERED) return FALSE;
                        if(m_btMaxNumber - m_btMinNumber < 10) return FALSE;
                        b0To9 = TRUE;
                        break;
                    case TPT_CIRSUB_TAIL:		// 位环差尾数，只用于排序型数据和含有大于10的值
                        if(!(m_btDataType & DATA_TYPE_ORDERED)) return FALSE;
                        if(m_btMaxNumber - m_btMinNumber < 10) return FALSE;
                        b0To9 = TRUE;
                        break;
                    case TPT_CIRSUB_ASC_TAIL:	// 顺位环差尾数，只用于不排序型数据和含有大于10的值
                    case TPT_CIRSUB_DESC_TAIL:	// 逆位环差尾数，只用于不排序型数据和含有大于10的值
                        if(m_btDataType & DATA_TYPE_ORDERED) return FALSE;
                        if(m_btMaxNumber - m_btMinNumber < 10) return FALSE;
                        b0To9 = TRUE;
                        break;
				}
				switch(dwTempID)
				{
                    case IID_STC_TWOPOS_ANY: return TRUE;
                    case IID_STC_TWOPOS_MIN:
                    case IID_STC_TWOPOS_MAX: return (nType < TPT_SUMSUB) ? TRUE : FALSE;
                    case IID_STC_TWOPOS_MID: return ((nType < TPT_SUMSUB) && (m_btNumberCount == 3)) ? TRUE : FALSE;
                    case IID_STC_TWOPOS_ARR_SIN:
                    case IID_STC_TWOPOS_ARR_GRO: return b0To9 && (m_btNumberCount == 3) && (nType < TPT_SUMSUB);
                    case IID_STC_TWOPOS_HEADTAIL: // 首尾
                        if(!(m_btDataType & DATA_TYPE_ORDERED)) return FALSE;
                        if(m_btDataType & DATA_TYPE_NUM_LT) return TRUE;
                    default:	// 任意两位
					{
						int nPos1 = (dwTempID / 10) % 10, nPos2 = dwTempID % 10;
						if(nPos2 > nPos1 && nPos2 < m_btNumberCount)
						{
							return TRUE;
						}
					} break;
				}
			}
			else if(cdtIID.dwSystemIID >= IID_STC_TWONUMSIN_BEGIN && cdtIID.dwSystemIID <= IID_STC_TWONUMSIN_END)		// 单选两码
			{
				if((m_btNumberCount <= 2) || !(m_btDataType & DATA_TYPE_NUM_SZ))
				{
					return FALSE;	// 必须是数字3及其以上
				}
				int nPos1 = ((cdtIID.dwSystemIID - IID_STC_TWONUMSIN_BEGIN) / 10) % 10, nPos2 = (cdtIID.dwSystemIID - IID_STC_TWONUMSIN_BEGIN) % 10;
				if(nPos1 >= nPos2 || nPos2 >= m_btNumberCount)
				{
					return FALSE;	// 超出范围
				}
				return TRUE;
			}
			else if(cdtIID.dwSystemIID >= IID_STC_REMM_BEGIN && cdtIID.dwSystemIID <= IID_STC_REMM_END)		// 余数指标
			{
				int nDivisor = (cdtIID.dwSystemIID - IID_STC_REMM_BEGIN) / 100 + 2;
				if(nDivisor > m_btMaxNumber)
				{
					return FALSE;		// 除数必须小于等于最大号码
				}
				DWORD dwTempID = DWORD(cdtIID.dwSystemIID - (nDivisor - 2) * 100);
				if(dwTempID <= IID_STC_REMM_POS_8)			// 按位余数
				{
					return (dwTempID - IID_STC_REMM_POS_1) < m_btNumberCount ? TRUE : FALSE;
				}
				else if(dwTempID >= IID_STC_REMM_COUNT_N)	// 除M余N个数
				{
					return (m_btNumberCount > 1 && (dwTempID - IID_STC_REMM_COUNT_N) < (DWORD)nDivisor) ? TRUE : FALSE;					
				}
				switch(dwTempID)
				{
                    case IID_STC_REMM_POS_ANY:
                    case IID_STC_REMM_SUM: return (m_btNumberCount > 1) ? TRUE : FALSE;
                    case IID_STC_REMM_SUMTAIL: return (m_btNumberCount > 1 && (nDivisor - 1) * m_btNumberCount > 9) ? TRUE : FALSE;
                    case IID_STC_REMM_ARR_SIN: 
                    case IID_STC_REMM_ARR_GRO:
                    case IID_STC_REMM_ARR_COUNT: return ((m_btNumberCount > 1) && (nDivisor <= 9)) ? TRUE : FALSE;
                    case IID_STC_REMM_ARR_SIN_VALUE: 
                    case IID_STC_REMM_ARR_GRO_VALUE: 
                    case IID_STC_REMM_ARR_COUNT_VALUE: return ((m_btNumberCount > 1) && (nDivisor <= 9) && ((int)pow(nDivisor, m_btNumberCount) <= 1000)) ? TRUE : FALSE;
#ifdef _PROFESSIONAL	// 专业版
                    case IID_STC_REMM_MAXLINK: 
                    case IID_STC_REMM_MAXNONE: 
                    case IID_STC_REMM_COUNT_NONE:
                    case IID_STC_REMM_COUNT_MATCH: return (m_btNumberCount > 2 && nDivisor > 2) ? TRUE : FALSE;
#else
                    case IID_STC_REMM_COUNT_NONE: return (m_btNumberCount > 2 && nDivisor > 2) ? TRUE : FALSE;
#endif
                    case IID_STC_REMM_SHAPE: return (nDivisor == 3 && m_btNumberCount == 3) ? TRUE : FALSE;
                    default: return FALSE;
				}
				return TRUE;
			}
			else if(cdtIID.dwSystemIID >= IID_DNC_XXX_BEGIN && cdtIID.dwSystemIID <= IID_DNC_XXX_END)			// 动态3态指标
			{
				if(m_btNumberCount < 2)
				{
					return FALSE;		// 必须为2个数字以上
				}
				int nType = (cdtIID.dwSystemIID - IID_DNC_XXX_BEGIN) / 100;
				DWORD dwTempID = (DWORD)(cdtIID.dwSystemIID - nType * 100);
				if(dwTempID == IID_DNC_XXX_SHAPE)
				{
					return m_btNumberCount == 3 ? TRUE : FALSE;
				}
				else if(dwTempID <= IID_DNC_XXX_POS_8)
				{
					if(dwTempID - IID_DNC_XXX_POS_1 < m_btNumberCount) return TRUE;
				}
				else if(dwTempID == IID_DNC_XXX_ARR_SIN_VALUE || dwTempID == IID_DNC_XXX_ARR_COUNT_VALUE)
				{
					return m_btNumberCount > 1 ? TRUE : FALSE;
				}
				else
					return TRUE;
			}
			else if(cdtIID.dwSystemIID >= IID_DNC_PREVDATA_BEGIN && cdtIID.dwSystemIID <= IID_DNC_PREVDATA_END)			// 邻期关系指标
			{
				int nType = (cdtIID.dwSystemIID - IID_DNC_PREVDATA_BEGIN) / 100;
#ifndef _PROFESSIONAL	// 非专业版
				if(nType >= PDT_SUB)
					return FALSE;
#endif
				DWORD dwTempID = DWORD(cdtIID.dwSystemIID - nType * 100);
				BOOL b0To9 = FALSE;
				switch(nType)
				{
                    case PDT_DISTAIL:		// 距离尾
                    case PDT_CIRDISTAIL:	// 环距尾
                        b0To9 = TRUE;
                        if(m_btMaxNumber - m_btMinNumber < 10) return FALSE;
                        break;
                    case PDT_AXISDISTAIL:	// 轴距尾
                        b0To9 = TRUE;
                        if(m_btMaxNumber - m_btMinNumber < 20) return FALSE;
                        break;
                    case PDT_SUMTAIL:		// 邻期合
                        b0To9 = TRUE;
                        if(m_btMaxNumber * 2 < 10) return FALSE;
                        break;
                    case PDT_DIS:
                    case PDT_CIRDIS:
                        if(m_btMaxNumber - m_btMinNumber < 10) b0To9 = TRUE; 
                        break;
                    case PDT_AXISDIS:
                        if(m_btMaxNumber - m_btMinNumber < 20) b0To9 = TRUE; 
                        break;
				}
				switch(dwTempID)
				{
                    case IID_DNC_PREVDATA_MIN:
                    case IID_DNC_PREVDATA_MAX: 
                    case IID_DNC_PREVDATA_ANY: return (m_btNumberCount > 1) ? TRUE : FALSE;
                    case IID_DNC_PREVDATA_MID: return (m_btNumberCount == 3) ? TRUE : FALSE;
                    case IID_DNC_PREVDATA_SUM:
                    case IID_DNC_PREVDATA_SUMTAIL:
					{
						switch(nType)
						{
                            case PDT_DIS:
                            case PDT_DISTAIL:
                            case PDT_SUMTAIL:
                            case PDT_CIRDIS:
                            case PDT_CIRDISTAIL:
                            case PDT_AXISDIS:
                            case PDT_AXISDISTAIL: return (m_btNumberCount > 1) ? TRUE : FALSE; break;
                            default: return FALSE; break;
						}
					} break;
                    case IID_DNC_PREVDATA_ARR_SIN:
                    case IID_DNC_PREVDATA_ARR_GRO: return b0To9 && (m_btNumberCount > 1);
                    default:	// 指定位
                        return ((dwTempID - IID_DNC_PREVDATA_POS_1) < m_btNumberCount) ? TRUE : FALSE;
				}
			}
		} break;
	}
    
	return FALSE;
}

BYTE CIndicator::GetValueRangeType(const CDTIID &cdtIID, DWORD /*dwCustomData*/)
{
	ASSERT(cdtIID.btType & CDTIID_TYPE_VALID);
    
	if(cdtIID.btType & CDTIID_TYPE_DATAAREA)		// 分区指标
	{
		switch(cdtIID.stCustomIID.wUseType)
		{
            case CIDAUT_ARR_SIN:
            case CIDAUT_ARR_GRO:
            case CIDAUT_ARR_COUNT:
                return CDTRT_ARRANGE;
            default:
                return CDTRT_DEFAULT;
		}
	}
    
	if(cdtIID.btType & CDTIID_TYPE_CUSTOM)		// 自定义指标
	{
		return 0;
	}
    
	// 系统指标
	switch(cdtIID.dwSystemIID)
	{
        case IID_DNC_SKIPSIN_1:
        case IID_DNC_SKIPSIN_2:
        case IID_DNC_SKIPSIN_3:
        case IID_DNC_SKIPSIN_4:
        case IID_DNC_SKIPSIN_5:
        case IID_DNC_SKIPSIN_6:
        case IID_DNC_SKIPSIN_7:
        case IID_DNC_SKIPSIN_8:
        case IID_DNC_SKIPSIN_MIN:
        case IID_DNC_SKIPSIN_MAX:
        case IID_DNC_SKIPSIN_SUM:
        case IID_DNC_SKIPGRO_1:
        case IID_DNC_SKIPGRO_2:
        case IID_DNC_SKIPGRO_3:
        case IID_DNC_SKIPGRO_4:
        case IID_DNC_SKIPGRO_5:
        case IID_DNC_SKIPGRO_6:
        case IID_DNC_SKIPGRO_7:
        case IID_DNC_SKIPGRO_8:
        case IID_DNC_SKIPGRO_MIN:
        case IID_DNC_SKIPGRO_MAX:
        case IID_DNC_SKIPGRO_SUM:
        case IID_DNC_NUMS_MATCHCOUNT_SIN:
        case IID_DNC_NUMS_MATCHCOUNT_GRO:
            return CDTRT_NOLIMIT;
            
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////
        case IID_STC_NUMBER_SAMECOMBO:
        case IID_STC_SHAPE_COUNT_SAMENUM:
        case IID_STC_LINK_NUMCOUNT:
        case IID_STC_LINK_COMBO:
        case IID_STC_LINK_NUMCOUNT_AS:
        case IID_STC_LINK_NUMCOUNT_DS:
        case IID_STC_LINK_COUNT_ODD:
        case IID_STC_LINK_COUNT_EVEN:
        case IID_STC_LINK_COUNT_PRIME:
        case IID_STC_LINK_COUNT_COMPOSITE:
        case IID_STC_ESPECIALSUM_ODD:
        case IID_STC_ESPECIALSUM_EVEN:
        case IID_STC_ESPECIALSUM_BIG:
        case IID_STC_ESPECIALSUM_PRIME:
        case IID_STC_ESPECIALSUM_COMPOSITE:
        case IID_STC_ESPECIALSUM_0:
        case IID_STC_ESPECIALSUM_1:
        case IID_STC_ESPECIALSUM_2:
        case IID_STC_ESPECIALSUM_D:
        case IID_STC_ESPECIALSUM_Z:
        case IID_STC_MUL:
        case IID_STC_MUL_ODDEVEN:
        case IID_STC_MUL_BIGSMALL:
        case IID_STC_MUL_012:
        case IID_STC_TWONUM_ANY:
        case IID_STC_NUMS_GRO:
        case IID_DNC_XL_COMBO:
            return CDTRT_UNCONT;
            
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////
        case IID_STC_ODDENEN_ARR:
        case IID_STC_BIGSMALL_ARR:
        case IID_STC_PRIMECOMPOSITE_ARR:
        case IID_STC_012_ARR_SIN: 
        case IID_STC_012_ARR_GRO: 
        case IID_STC_012_ARR_COUNT:
        case IID_STC_DZX_ARR_SIN:
        case IID_STC_DZX_ARR_GRO:
        case IID_STC_DZX_ARR_COUNT:
        case IID_DNC_DIS_ARR_SIN:
        case IID_DNC_DIS_ARR_GRO:
        case IID_DNC_SKIPSIN_TAIL_ARR_SIN:
        case IID_DNC_SKIPSIN_TAIL_ARR_GRO: 
        case IID_DNC_SKIPGRO_TAIL_ARR_SIN:
        case IID_DNC_SKIPGRO_TAIL_ARR_GRO: 
        case IID_STC_ORDER_POSSUM_ARR_SIN:
        case IID_STC_ORDER_POSSUM_ARR_GRO:
        case IID_STC_ORDER_POSSUMTAIL_ARR_SIN:
        case IID_STC_ORDER_POSSUMTAIL_ARR_GRO:
        case IID_STC_ORDER_POSSUB_ARR_SIN:
        case IID_STC_ORDER_POSSUB_ARR_GRO:
        case IID_STC_ORDER_POSSUBTAIL_ARR_SIN:
        case IID_STC_ORDER_POSSUBTAIL_ARR_GRO:
        case IID_STC_ORDER_RELATIVEDZX_ARR:
            return CDTRT_ARRANGE;
            
        case IID_STC_ORDER_RELATIVEDZX_POS_1:
        case IID_STC_ORDER_RELATIVEDZX_POS_2:
        case IID_STC_ORDER_RELATIVEDZX_POS_3:
            return CDTRT_ORDER_DESC;
            
        case IID_STC_NUMS_SIN:
            return (m_btMinNumber == 0 && m_btMaxNumber == 9) ? CDTRT_DEFAULT : CDTRT_UNCONT;
            
        case IID_DNC_TESTNUMS_REPEAT_CUR_CUR:
        case IID_DNC_TESTNUMS_REPEAT_PRE_CUR:
        case IID_DNC_TESTNUMS_REPEAT_COMPLEX:
            return (g_pIData->GetLotteryProperty()->btProperty & CSTLPP_TESTNUMS) ? CDTRT_DEFAULT : CDTRT_NOTSUPPORT;
        case IID_DNC_TESTNUMS_REPEAT_COMBO:
            return (g_pIData->GetLotteryProperty()->btProperty & CSTLPP_TESTNUMS) ? CDTRT_UNCONT : CDTRT_NOTSUPPORT;
            
        case IID_HIS_NUMS_ARR_SIN:
        case IID_HIS_NUMS_ARR_GRO:
        case IID_HIS_NUMS_1_ARR_SIN:
        case IID_HIS_NUMS_1_ARR_GRO:
        case IID_HIS_NUMS_2_ARR_SIN:
        case IID_HIS_NUMS_2_ARR_GRO:
            if(m_btNumberCount > 3 || m_btMaxNumber > 9)
                return CDTRT_NOLIMIT;
            break;
        default:
		{
			if(cdtIID.dwSystemIID >= IID_DNC_XXX_BEGIN && cdtIID.dwSystemIID <= IID_DNC_XXX_END)	// 动态3态指标
			{
				int nTypeIndex = (cdtIID.dwSystemIID - IID_DNC_XXX_BEGIN) / 100;
				switch(cdtIID.dwSystemIID - nTypeIndex * 100)
				{
                    case IID_DNC_XXX_ARR_SIN:
                    case IID_DNC_XXX_ARR_GRO:
                    case IID_DNC_XXX_ARR_COUNT:
                        return CDTRT_ARRANGE;
				} 
			}
			else if(cdtIID.dwSystemIID >= IID_STC_TWOPOS_BEGIN && cdtIID.dwSystemIID <= IID_STC_TWOPOS_END)	// 两位关系
			{
				int nTypeIndex = (cdtIID.dwSystemIID - IID_STC_TWOPOS_BEGIN) / 100;
				if(nTypeIndex == TPT_MUL)
				{
					return CDTRT_UNCONT;
				}
                
				if(nTypeIndex == TPT_SUB_ASC || nTypeIndex == TPT_SUB_DESC)
				{
					if(!(m_btDataType & DATA_TYPE_REPEATABLE))
					{
						return CDTRT_UNCONT;
					}
				}
				switch(cdtIID.dwSystemIID - nTypeIndex * 100)
				{
                    case IID_STC_TWOPOS_ARR_SIN:
                    case IID_STC_TWOPOS_ARR_GRO:
                        return CDTRT_ARRANGE;
				}
			}
			else if(cdtIID.dwSystemIID >= IID_STC_REMM_BEGIN && cdtIID.dwSystemIID <= IID_STC_REMM_END)		// 余数
			{
				int nDivisorIndex = (cdtIID.dwSystemIID - IID_STC_REMM_BEGIN) / 100;
				switch(cdtIID.dwSystemIID - nDivisorIndex * 100)
				{
                    case IID_STC_REMM_ARR_SIN:
                    case IID_STC_REMM_ARR_GRO:
                    case IID_STC_REMM_ARR_COUNT:
                        return CDTRT_ARRANGE;
				}
			}
			else if(cdtIID.dwSystemIID >= IID_DNC_PREVDATA_BEGIN && cdtIID.dwSystemIID <= IID_DNC_PREVDATA_END)	// 邻期关系
			{
				int nTypeIndex = (cdtIID.dwSystemIID - IID_DNC_PREVDATA_BEGIN) / 100;
				switch(cdtIID.dwSystemIID - nTypeIndex * 100)
				{
                    case IID_DNC_PREVDATA_ARR_SIN:
                    case IID_DNC_PREVDATA_ARR_GRO:
                        return CDTRT_ARRANGE;
				}
			}
		} break;
	}
	
	return CDTRT_DEFAULT;
}

BYTE CIndicator::GetValueType(const CDTIID &cdtIID, DWORD dwCustomData)
{
	ASSERT(cdtIID.btType & CDTIID_TYPE_VALID);
    
	if(cdtIID.btType & CDTIID_TYPE_DATAAREA)		// 分区指标
	{
		switch(cdtIID.stCustomIID.wUseType)
		{
            case CIDAUT_ARR_SIN: return CDTVT_ARRANGE_SINGLE;
            case CIDAUT_ARR_GRO: return CDTVT_ARRANGE_GROUP;
            case CIDAUT_ARR_COUNT:
			{
				LPDATAAREA lpDataArea = LPDATAAREA(dwCustomData);
				return BYTE(CDTVT_ARRANGE_COUNT_BEGIN + lpDataArea->btAreaCount - 2);
			} break;
            default:
                return CDTVT_DIGITAL;
		}
	}
    
	if(cdtIID.btType & CDTIID_TYPE_CUSTOM)		// 自定义指标
	{
		return 0;
	}
    
	// 系统指标
	switch(cdtIID.dwSystemIID)
	{
        case IID_STC_POS_1:
        case IID_STC_POS_2:
        case IID_STC_POS_3:
        case IID_STC_POS_4:
        case IID_STC_POS_5:
        case IID_STC_POS_6:
        case IID_STC_POS_7:
        case IID_STC_POS_8:
        case IID_STC_POS_ANY:
            if(m_btDataType & DATA_TYPE_NUM_LT)
                return CDTVT_DIGITAL2;
            else 
                return CDTVT_DIGITAL;
            
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////
        case IID_DNC_XL_COMBO:
        case IID_STC_TWONUM_ANY:
        case IID_DNC_TESTNUMS_REPEAT_COMBO:
            return CDTVT_DIGITAL2;
            
        case IID_STC_NUMS_GRO:
        case IID_STC_NUMS_SIN:
            return BYTE(CDTVT_DIGITAL + m_btNumberCount - 1);
            
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////
        case IID_STC_ODDENEN_ARR:
        case IID_STC_BIGSMALL_ARR:
        case IID_STC_PRIMECOMPOSITE_ARR:
        case IID_STC_012_ARR_SIN:
        case IID_STC_DZX_ARR_SIN:
        case IID_DNC_DIS_ARR_SIN:
        case IID_DNC_SKIPSIN_TAIL_ARR_SIN:
        case IID_DNC_SKIPGRO_TAIL_ARR_SIN:
        case IID_STC_ORDER_POSSUM_ARR_SIN:
        case IID_STC_ORDER_POSSUMTAIL_ARR_SIN:
        case IID_STC_ORDER_POSSUB_ARR_SIN:
        case IID_STC_ORDER_POSSUBTAIL_ARR_SIN:
        case IID_STC_ORDER_RELATIVEDZX_ARR:
            return CDTVT_ARRANGE_SINGLE;
            
        case IID_STC_012_ARR_COUNT:
        case IID_STC_DZX_ARR_COUNT:
            return BYTE(CDTVT_ARRANGE_COUNT_BEGIN + 1);
            
        case IID_STC_012_ARR_GRO:
        case IID_STC_DZX_ARR_GRO:
        case IID_DNC_DIS_ARR_GRO:
        case IID_DNC_SKIPSIN_TAIL_ARR_GRO: 
        case IID_DNC_SKIPGRO_TAIL_ARR_GRO: 
        case IID_STC_ORDER_POSSUM_ARR_GRO:
        case IID_STC_ORDER_POSSUMTAIL_ARR_GRO:
        case IID_STC_ORDER_POSSUB_ARR_GRO:
        case IID_STC_ORDER_POSSUBTAIL_ARR_GRO:
            return CDTVT_ARRANGE_GROUP;
            
        default:
		{
			if(cdtIID.dwSystemIID >= IID_DNC_XXX_BEGIN && cdtIID.dwSystemIID <= IID_DNC_XXX_END)	// 动态3态指标
			{
				int nTypeIndex = (cdtIID.dwSystemIID - IID_DNC_XXX_BEGIN) / 100;
				switch(cdtIID.dwSystemIID - nTypeIndex * 100)
				{
                    case IID_DNC_XXX_ARR_SIN: return CDTVT_ARRANGE_SINGLE;
                    case IID_DNC_XXX_ARR_GRO: return CDTVT_ARRANGE_GROUP;
                    case IID_DNC_XXX_ARR_COUNT: return BYTE(CDTVT_ARRANGE_COUNT_BEGIN + 1);
				} 
			}
			else if(cdtIID.dwSystemIID >= IID_STC_TWOPOS_BEGIN && cdtIID.dwSystemIID <= IID_STC_TWOPOS_END)	// 两位关系
			{
				int nTypeIndex = (cdtIID.dwSystemIID - IID_STC_TWOPOS_BEGIN) / 100;
				switch(cdtIID.dwSystemIID - nTypeIndex * 100)
				{
                    case IID_STC_TWOPOS_ARR_SIN: return CDTVT_ARRANGE_SINGLE;
                    case IID_STC_TWOPOS_ARR_GRO: return CDTVT_ARRANGE_GROUP;
				}
			}
			else if(cdtIID.dwSystemIID >= IID_STC_TWONUMSIN_BEGIN && cdtIID.dwSystemIID <= IID_STC_TWONUMSIN_END)	// 单选二码
			{
				return CDTVT_DIGITAL2;
			}
			else if(cdtIID.dwSystemIID >= IID_STC_REMM_BEGIN && cdtIID.dwSystemIID <= IID_STC_REMM_END)		// 余数
			{
				int nDivisorIndex = (cdtIID.dwSystemIID - IID_STC_REMM_BEGIN) / 100;
				switch(cdtIID.dwSystemIID - nDivisorIndex * 100)
				{
                    case IID_STC_REMM_ARR_SIN: return CDTVT_ARRANGE_SINGLE;
                    case IID_STC_REMM_ARR_GRO: return CDTVT_ARRANGE_GROUP;
                    case IID_STC_REMM_ARR_COUNT: return BYTE(CDTVT_ARRANGE_COUNT_BEGIN + nDivisorIndex);
				}
			}
			else if(cdtIID.dwSystemIID >= IID_DNC_PREVDATA_BEGIN && cdtIID.dwSystemIID <= IID_DNC_PREVDATA_END)	// 邻期关系
			{
				int nTypeIndex = (cdtIID.dwSystemIID - IID_DNC_PREVDATA_BEGIN) / 100;
				switch(cdtIID.dwSystemIID - nTypeIndex * 100)
				{
                    case IID_DNC_PREVDATA_ARR_SIN: return CDTVT_ARRANGE_SINGLE;
                    case IID_DNC_PREVDATA_ARR_GRO: return CDTVT_ARRANGE_GROUP;
				}
			}
		} break;
	}
    
	return CDTVT_DIGITAL;
}

BYTE CIndicator::GetInputType(const CDTIID &cdtIID, DWORD /*dwCustomData*/)
{
	ASSERT(cdtIID.btType & CDTIID_TYPE_VALID);
    
	if(cdtIID.btType & CDTIID_TYPE_DATAAREA)		// 分区指标
	{
		switch(cdtIID.stCustomIID.wUseType)
		{
            case CIDAUT_ARR_SIN: 
            case CIDAUT_ARR_GRO: 
            case CIDAUT_ARR_COUNT: return CDTIT_EDIT_MULTILINE;
            case CIDAUT_SHAPE: return CDTIT_CHECKBOX;
            default:
                return CDTIT_EDIT_SINGLELINE;
		}
	}
	if(cdtIID.btType & CDTIID_TYPE_CUSTOM)		// 自定义指标
	{
		return 0;
	}
    
	// 系统指标
	switch(cdtIID.dwSystemIID)
	{
        case IID_STC_012_SHAPE: 
        case IID_STC_DZX_SHAPE:
        case IID_STC_GENRE_NORMAL:
        case IID_STC_GENRE_TRANS:
        case IID_STC_GENRE_TWOLINK:
        case IID_STC_ODDENEN_LINK:
        case IID_STC_SHAPE_NORMAL:
        case IID_STC_SHAPE_EX:
        case IID_STC_SHAPE_EQUALDIFF:
        case IID_STC_SHAPE_AAB:
        case IID_STC_SHAPE_ABC:
        case IID_STC_SHAPE_ABD:
        case IID_STC_DHK_HD:
        case IID_STC_DHK_KD:
        case IID_STC_DHK_HK:  
        case IID_STC_DHK_HDKD:
        case IID_STC_DHK_HDKDHK:	
        case IID_STC_MUL_ODDEVEN:
        case IID_STC_MUL_BIGSMALL:
        case IID_STC_MUL_012:
        case IID_STC_TWONUM_Z6:
        case IID_STC_TWONUM_Z6_2:
        case IID_STC_TWONUM_Z3:
        case IID_STC_TWONUM_Z3_2:
        case IID_DNC_TESTNUMS_REPEAT_COMPLEX:
            return CDTIT_CHECKBOX;
			
            ///////////////////////////////////////////////////////////////////////////////////////////////////////////
            
        case IID_STC_ODDENEN_ARR:
        case IID_STC_BIGSMALL_ARR:
        case IID_STC_PRIMECOMPOSITE_ARR:
        case IID_STC_012_ARR_SIN:
        case IID_STC_012_ARR_GRO:
        case IID_STC_012_ARR_COUNT:
        case IID_STC_DZX_ARR_SIN:
        case IID_STC_DZX_ARR_GRO:
        case IID_STC_DZX_ARR_COUNT:
        case IID_DNC_DIS_ARR_SIN:
        case IID_DNC_DIS_ARR_GRO:
        case IID_DNC_SKIPSIN_TAIL_ARR_SIN:
        case IID_DNC_SKIPSIN_TAIL_ARR_GRO: 
        case IID_DNC_SKIPGRO_TAIL_ARR_SIN:
        case IID_DNC_SKIPGRO_TAIL_ARR_GRO: 
        case IID_STC_ORDER_POSSUM_ARR_SIN:
        case IID_STC_ORDER_POSSUM_ARR_GRO:
        case IID_STC_ORDER_POSSUMTAIL_ARR_SIN:
        case IID_STC_ORDER_POSSUMTAIL_ARR_GRO:
        case IID_STC_ORDER_POSSUB_ARR_SIN:
        case IID_STC_ORDER_POSSUB_ARR_GRO:
        case IID_STC_ORDER_POSSUBTAIL_ARR_SIN:
        case IID_STC_ORDER_POSSUBTAIL_ARR_GRO:
        case IID_STC_ORDER_RELATIVEDZX_ARR:
        case IID_STC_NUMS_SIN:
        case IID_STC_NUMS_GRO:
            return CDTIT_EDIT_MULTILINE;
            
        default:
		{
			if(cdtIID.dwSystemIID >= IID_DNC_XXX_BEGIN && cdtIID.dwSystemIID <= IID_DNC_XXX_END)	// 动态3态指标
			{
				int nTypeIndex = ((cdtIID.dwSystemIID - IID_DNC_XXX_BEGIN) / 100) % 10;
				switch(cdtIID.dwSystemIID - nTypeIndex * 100)
				{
                    case IID_DNC_XXX_POS_1:
                    case IID_DNC_XXX_POS_2:
                    case IID_DNC_XXX_POS_3:
                    case IID_DNC_XXX_POS_4:
                    case IID_DNC_XXX_POS_5:
                    case IID_DNC_XXX_POS_6:
                    case IID_DNC_XXX_POS_7:
                    case IID_DNC_XXX_POS_8: 
                    case IID_DNC_XXX_SHAPE:	return CDTIT_CHECKBOX;
                    case IID_DNC_XXX_ARR_SIN:
                    case IID_DNC_XXX_ARR_GRO:
                    case IID_DNC_XXX_ARR_COUNT: return CDTIT_EDIT_MULTILINE;
				} 
			}
			else if(cdtIID.dwSystemIID >= IID_STC_TWOPOS_BEGIN && cdtIID.dwSystemIID <= IID_STC_TWOPOS_END)
			{
				int nTypeIndex = (cdtIID.dwSystemIID - IID_STC_TWOPOS_BEGIN) / 100;
				switch(cdtIID.dwSystemIID - nTypeIndex * 100)
				{
                    case IID_STC_TWOPOS_ARR_SIN:
                    case IID_STC_TWOPOS_ARR_GRO: return CDTIT_EDIT_MULTILINE;
				}
			}
			else if(cdtIID.dwSystemIID >= IID_STC_REMM_BEGIN && cdtIID.dwSystemIID <= IID_STC_REMM_END)		// 余数
			{
				int nDivisorIndex = (cdtIID.dwSystemIID - IID_STC_REMM_BEGIN) / 100;
				DWORD dwTempID = (DWORD)(cdtIID.dwSystemIID - nDivisorIndex * 100);
				switch(dwTempID)
				{
                    case IID_STC_REMM_ARR_SIN: 
                    case IID_STC_REMM_ARR_GRO: 
                    case IID_STC_REMM_ARR_COUNT: return CDTIT_EDIT_MULTILINE;
                    case IID_STC_REMM_SHAPE: return CDTIT_CHECKBOX;
				}
			}
			else if(cdtIID.dwSystemIID >= IID_DNC_PREVDATA_BEGIN && cdtIID.dwSystemIID <= IID_DNC_PREVDATA_END)	// 邻期关系
			{
				int nTypeIndex = (cdtIID.dwSystemIID - IID_DNC_PREVDATA_BEGIN) / 100;
				switch(cdtIID.dwSystemIID - nTypeIndex * 100)
				{
                    case IID_DNC_PREVDATA_ARR_SIN:
                    case IID_DNC_PREVDATA_ARR_GRO: return CDTIT_EDIT_MULTILINE;
				}
			}
			else if(cdtIID.dwSystemIID >= IID_STC_LINK_POS_2 && cdtIID.dwSystemIID <= IID_STC_LINK_POS_2 + 6)	// 连号位置
			{
				return CDTIT_CHECKBOX;
			}
		} break;
	}
    
	return CDTIT_EDIT_SINGLELINE;
}

#define		BUILD_MULTIPLEVALUE(nMaxCountOfValueIn) \
{if(pMaxCountOfValue != NULL) \
*pMaxCountOfValue = nMaxCountOfValueIn; \
return TRUE;}

BOOL CIndicator::IsMultipleValue(const CDTIID &cdtIID, DWORD /*dwCustomData*/, int *pMaxCountOfValue, LPBOOL lpHasSameValue)
{
	if(cdtIID.btType & CDTIID_TYPE_DATAAREA)		// 分区指标
	{
		switch(cdtIID.stCustomIID.wUseType)
		{
            case CIDAUT_POS_ANY:
                BUILD_MULTIPLEVALUE(m_btNumberCount);
                break;
            default:
			{
				// 单值条件
				if(pMaxCountOfValue != NULL)
				{
					*pMaxCountOfValue = 1;
				}
			} break;
		}
		return FALSE;
	}
    
	if(cdtIID.btType & CDTIID_TYPE_CUSTOM)		// 自定义指标
	{
		// 单值条件
		if(pMaxCountOfValue != NULL)
		{
			*pMaxCountOfValue = 1;
		}
		return FALSE;
	}
    
	// 系统指标
	switch(cdtIID.dwSystemIID)
	{
        case IID_STC_POS_ANY:
            if(lpHasSameValue != NULL)
            {
                *lpHasSameValue = (m_btDataType & DATA_TYPE_REPEATABLE) ? TRUE : FALSE;
            }
            BUILD_MULTIPLEVALUE(m_btNumberCount);
            break;
        case IID_DNC_DIS_POS_ANY:
        case IID_DNC_SKIPSIN_TAIL_ANY:
        case IID_DNC_SKIPGRO_TAIL_ANY: 
        case IID_STC_NEARSPACE_ANY:
            BUILD_MULTIPLEVALUE(m_btNumberCount);
            break;
            
        case IID_STC_NUMBER_SAMECOMBO: 
        case IID_STC_LINK_COMBO:
            BUILD_MULTIPLEVALUE(m_btNumberCount / 2);
            break;
            
        case IID_STC_LINK_POS_2:
        case IID_STC_LINK_POS_3:
        case IID_STC_LINK_POS_4:
        case IID_STC_LINK_POS_5:
        case IID_STC_LINK_POS_6:
        case IID_STC_LINK_POS_7:
            if(lpHasSameValue != NULL)
            {
                *lpHasSameValue = FALSE;
            }
            BUILD_MULTIPLEVALUE(m_btNumberCount / (cdtIID.dwSystemIID - IID_STC_LINK_POS_2 + 2));
            break;
            
        case IID_STC_TWONUM_ANY:
        case IID_STC_TWONUMSIN_ANY:
            BUILD_MULTIPLEVALUE(g_pICstPubFunc->GetArrangeComboValue(m_btNumberCount, 2));
            break;
            
        case IID_STC_ORDER_MID:
            if(m_btNumberCount > 3)
            {
                if(lpHasSameValue != NULL && !(m_btDataType & DATA_TYPE_REPEATABLE))
                {
                    *lpHasSameValue = FALSE;
                }
                BUILD_MULTIPLEVALUE(m_btNumberCount - 2);
            }
            break;
        default:
		{
			if(cdtIID.dwSystemIID >= IID_STC_TWOPOS_BEGIN && cdtIID.dwSystemIID <= IID_STC_TWOPOS_END)		// 两位关系
			{
				int nTypeIndex = (cdtIID.dwSystemIID - IID_STC_TWOPOS_BEGIN) / 100;
				if(nTypeIndex == TPT_SUMSUB)	// 两码合差
				{
					int nIDTemp = cdtIID.dwSystemIID - nTypeIndex * 100;
					if(nIDTemp == IID_STC_TWOPOS_ANY)	// 任意两码合差
					{
						BUILD_MULTIPLEVALUE(g_pICstPubFunc->GetArrangeComboValue(m_btNumberCount, 2) * 2);
					}
					else if(nIDTemp >= IID_STC_TWOPOS_MN && nIDTemp <= IID_STC_TWOPOS_MN + 90)	// 指定位置的两码合差
					{
						BUILD_MULTIPLEVALUE(2);
					}
					else
					{
						ASSERT(FALSE);
					}
				}
				else if(nTypeIndex == TPT_SUMCIRSUB)	// 两码合环差
				{
					int nIDTemp = cdtIID.dwSystemIID - nTypeIndex * 100;
					if(nIDTemp == IID_STC_TWOPOS_ANY)	// 任意两码合差
					{
						BUILD_MULTIPLEVALUE(g_pICstPubFunc->GetArrangeComboValue(m_btNumberCount, 2) * 3);
					}
					else if(nIDTemp >= IID_STC_TWOPOS_MN && nIDTemp <= IID_STC_TWOPOS_MN + 90)	// 指定位置的两码合差
					{
						BUILD_MULTIPLEVALUE(3);
					}
					else
					{
						ASSERT(FALSE);
					}
				}
				else if(cdtIID.dwSystemIID - nTypeIndex * 100 == IID_STC_TWOPOS_ANY)	// 任意两位关系
				{
					BUILD_MULTIPLEVALUE(g_pICstPubFunc->GetArrangeComboValue(m_btNumberCount, 2));
				}
			}
			else if(cdtIID.dwSystemIID >= IID_STC_REMM_BEGIN && cdtIID.dwSystemIID <= IID_STC_REMM_END)		// 余数
			{
				int nDivisorIndex = (cdtIID.dwSystemIID - IID_STC_REMM_BEGIN) / 100;
				if(cdtIID.dwSystemIID - nDivisorIndex * 100 == IID_STC_REMM_POS_ANY)
				{
					BUILD_MULTIPLEVALUE(m_btNumberCount);
				}
			}
			else if(cdtIID.dwSystemIID >= IID_DNC_PREVDATA_BEGIN && cdtIID.dwSystemIID <= IID_DNC_PREVDATA_END)	// 邻期关系
			{
				int nTypeIndex = (cdtIID.dwSystemIID - IID_DNC_PREVDATA_BEGIN) / 100;
				if(cdtIID.dwSystemIID - nTypeIndex * 100 == IID_DNC_PREVDATA_ANY)
				{
					BUILD_MULTIPLEVALUE(m_btNumberCount);
				}
			}
		}
	}
	
	// 单值条件
	if(pMaxCountOfValue != NULL)
	{
		*pMaxCountOfValue = 1;
	}
    
	return FALSE;
}

BOOL CIndicator::IsDigitalValueName(const CDTIID &cdtIID, DWORD dwCustomData, LPCDTVALUERANGE lpValueRange)
{
	if(lpValueRange == NULL)
	{
		return FALSE;
	}
	
	char szTemp[128] = {0};
	BYTE btValueType = GetValueType(cdtIID, dwCustomData);
	int nFixLen = -1;
	if(btValueType == CDTVT_DIGITAL)
	{
		nFixLen = 0;
	}
	else if(btValueType >= CDTVT_DIGITAL2 && btValueType <= CDTVT_DIGITAL8)
	{
		nFixLen = btValueType - CDTVT_DIGITAL2 + 2;
	}
    
	if(nFixLen >= 0)
	{
		for(int i=0; i<lpValueRange->nItemCount; i++)
		{
			sprintf(szTemp, "%0*d", nFixLen, lpValueRange->stValueItem[i].nValue);
			if(strcmp(lpValueRange->stValueItem[i].szValueName, szTemp))
			{
				return FALSE;	// 非数字型
			}
		}
		return TRUE;	// 数字型
	}
	else
	{
		return FALSE;	// 非数字型
	}
}

int CIndicator::GetValueExplain(CStringArray &strExplainArray, const CDTIID &cdtIID, DWORD dwCustomData, LPCDTVALUERANGE lpValueRange)
{
	if(cdtIID.btType & CDTIID_TYPE_DATAAREA)	// 分区指标
	{
		if(cdtIID.stCustomIID.wUseType <= CIDAUT_POS_ANY)
		{			
			LPDATAAREA lpCurDataArea = LPDATAAREA(dwCustomData);
			if(lpCurDataArea == NULL || lpCurDataArea->btAreaCount != lpValueRange->nItemCount)
			{
				ASSERT(FALSE);
				return 0;
			}
            
			strExplainArray.SetSize(lpValueRange->nItemCount);
			CDWordArray dwValueArray;
			BYTE i = 0, j = 0, k = 0, btIndex = 0, btMinNumber = g_pIData->GetMinNumber(), btMaxNumber = g_pIData->GetMaxNumber();
			
			for(j=lpCurDataArea->btSubAreaMinValue; j<lpCurDataArea->btAreaCount+lpCurDataArea->btSubAreaMinValue; j++)
			{
				dwValueArray.SetSize(btMaxNumber);
				btIndex = 0;
				for(k=btMinNumber; k<=btMaxNumber; k++)
				{
					if(lpCurDataArea->btSubAreaValues[k] == j)
					{
						dwValueArray.SetAt(btIndex, k);
						btIndex ++;
					}
				}
				dwValueArray.SetSize(btIndex);
				LPCSTR lpszTemp = g_pICstPubFunc->IntArrayToText(dwValueArray, btMaxNumber <= 9 ? TRUE : FALSE, btMaxNumber <= 9 ? 0 : 2);
				i = BYTE(j - lpCurDataArea->btSubAreaMinValue);
				strExplainArray[i].Format("%s：%s（%s）", lpValueRange->stValueItem[i].szBallName, lpValueRange->stValueItem[i].szValueName, lpszTemp);
			}
			return strExplainArray.GetSize();
		}
	}
    
	if(cdtIID.btType & CDTIID_TYPE_CUSTOM)	// 自定义指标
	{
	}
	else if(cdtIID.dwSystemIID >= IID_STC_REMM_BEGIN && cdtIID.dwSystemIID <= IID_STC_REMM_END)	// 余数
	{
		int nDivisor = (cdtIID.dwSystemIID - IID_STC_REMM_BEGIN) / 100 + 2;
		if(cdtIID.dwSystemIID - (nDivisor - 2) * 100 <= IID_STC_REMM_POS_ANY)
		{			
			strExplainArray.SetSize(lpValueRange->nItemCount);
			int nAllNumsCount = g_pIData->GetAllNumberCount(), nMinNumber = g_pIData->GetMinNumber();			
			CString strTemp;
			for(int i=0; i<nDivisor; i++)
			{
				for(int j=0; j<nAllNumsCount; j++)
				{
					if((j + nMinNumber) % nDivisor == i)
					{
						strTemp += g_pIData->GetNumberText(j);
						strTemp += ",";
					}
				}
				strTemp.TrimRight(",");
				strExplainArray[i].Format("%s：%s（%s）", lpValueRange->stValueItem[i].szBallName, lpValueRange->stValueItem[i].szValueName, strTemp);
				strTemp.Empty();
			}
			return strExplainArray.GetSize();
		}
	}
    
	// 普通条件
	if(IsDigitalValueName(cdtIID, dwCustomData, lpValueRange))
	{
		return 0;	// 数值型，不需要说明
	}
    
	strExplainArray.SetSize(lpValueRange->nItemCount);
	for(int i=0; i<lpValueRange->nItemCount; i++)
	{
		strExplainArray[i] = lpValueRange->stValueItem[i].szValueName;
	}
    
	return strExplainArray.GetSize();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CIndicator::_IsValid_DataArea(const CDTIID &cdtIID, LPDATAAREA lpDataArea)
{
	if(lpDataArea == NULL)
	{
		return FALSE;
	}
	
	switch(cdtIID.stCustomIID.wUseType)
	{
        case CIDAUT_POS_1:
        case CIDAUT_POS_2:
        case CIDAUT_POS_3:
        case CIDAUT_POS_4:
        case CIDAUT_POS_5:
        case CIDAUT_POS_6:
        case CIDAUT_POS_7:
        case CIDAUT_POS_8: return (cdtIID.stCustomIID.wUseType - CIDAUT_POS_1 < m_btNumberCount) ? TRUE : FALSE;
        case CIDAUT_POS_ANY: return (m_btNumberCount > 1) ? TRUE : FALSE; break;
        case CIDAUT_ARR_SIN:
        case CIDAUT_ARR_GRO:
        case CIDAUT_ARR_COUNT: return (m_btNumberCount > 1 && lpDataArea->btAreaCount < 10) ? TRUE : FALSE; break;
        case CIDAUT_ARR_SIN_VALUE:
        case CIDAUT_ARR_GRO_VALUE:
        case CIDAUT_ARR_COUNT_VALUE: return (m_btNumberCount > 1 && lpDataArea->btAreaCount < 10 && (((int)pow(lpDataArea->btAreaCount, m_btNumberCount) <= 1000))) ? TRUE : FALSE; break;
        case CIDAUT_SHAPE: return (lpDataArea->btAreaCount == 3 && m_btNumberCount == 3) ? TRUE : FALSE; break;
#ifdef _PROFESSIONAL	// 专业版
        case CIDAUT_MAXLINK:
        case CIDAUT_MAXNONE:
        case CIDAUT_COUNT_NONE:
        case CIDAUT_COUNT_MATCH: return (m_btNumberCount > 1 && lpDataArea->btAreaCount > 2) ? TRUE : FALSE; break;
#endif
        default:	// CIDAUT_COUNT_N
		{
			return (m_btNumberCount > 1 && (cdtIID.stCustomIID.wUseType - CIDAUT_COUNT_N < lpDataArea->btAreaCount)) ? TRUE : FALSE;
		} break;
	}
}