//
//  Indicator_GetValue.cpp
//  QCLottery
//
//  Created by 武 孙 on 12-5-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "Indicator.h"
#include "ConstData.h"
#include "ICstPubFunc.h"
#include <math.h>

#define _GETSPECIALSUMVALUE(fnGetXXXValue, nCompValue) { for(int i=0; i<m_btNumberCount; i++){ if(fnGetXXXValue(m_lpNumsData[i]) == nCompValue){ nValue += m_lpNumsData[i]; } } }

///////////////////////////////////////////////////////////////////////////////////////
LPCDTVALUE CIndicator::GetValue(const CDTIID &cdtIID, DWORD dwCustomData, LPBYTE lpFltNumsData, DWORD dwIssueIndex, BYTE btType)
{
	if(lpFltNumsData == NULL)	// 历史数据
	{
		ASSERT(dwIssueIndex != DATA_INDEX_INVALID);
		m_lpNumsData = g_pIData->GetItemNums(dwIssueIndex);
	}
	else
	{
		m_lpNumsData = lpFltNumsData;
	}
	if(m_lpNumsData == NULL)
	{
		return _FillSingleValues(CDTVALUE_INVALID);
	}
	if(dwIssueIndex == DATA_INDEX_LAST)
	{
		dwIssueIndex = g_pIData->GetItemCount() - 1;
	}
    
	if(cdtIID.btType & CDTIID_TYPE_DATAAREA)		// 分区指标
	{
		return _GetAreaValue(cdtIID.stCustomIID.wUseType, dwCustomData);
	}
    
	if(cdtIID.btType & CDTIID_TYPE_CUSTOM)	// 自定义指标
	{
		return NULL;		// TEST
	}
	
	int nValue = 0;
	// 系统指标
	switch(cdtIID.dwSystemIID)
	{
        case 0: nValue = 0; break;
            
        case IID_STC_NUMBER_SAMECOMBO:
		{
			int nValues[8] = {0};
			nValue = _GetSameNumGroupCount(cdtIID.dwSystemIID - IID_STC_NUMBER_SAMECOMBO, nValues);
			return _FillMulValues(nValues, (BYTE)nValue);
		} break;			
        case IID_STC_NUMBER_MAXCOUNT:
        case IID_STC_NUMBER_SAMEGROUP_2:
        case IID_STC_NUMBER_SAMEGROUP_3:
        case IID_STC_NUMBER_SAMEGROUP_4:
        case IID_STC_NUMBER_SAMEGROUP_5:
        case IID_STC_NUMBER_SAMEGROUP_6:
        case IID_STC_NUMBER_SAMEGROUP_7:
        case IID_STC_NUMBER_SAMEGROUP_8: nValue = _GetSameNumGroupCount(cdtIID.dwSystemIID - IID_STC_NUMBER_SAMECOMBO); break;
            
        case IID_STC_GENRE_NORMAL: nValue = _GetTypeValue(); break;
        case IID_STC_GENRE_TRANS: nValue = TYPE_VALUE[_GetNumsIndex()] & 0x0F; break;
        case IID_STC_GENRE_TWOLINK: nValue = _GetTwoLinkValue(); break;
            
        case IID_STC_SHAPE_NORMAL: nValue = _GetShapeValue(); break;
        case IID_STC_SHAPE_EX: nValue = STATE_VALUE[_GetNumsIndex()] & 0x0F; break;
        case IID_STC_SHAPE_EQUALDIFF: nValue = _GetEqualDiffValue(); break;
        case IID_STC_SHAPE_AAB: 
            nValue = ((m_lpNumsData[0] == m_lpNumsData[1]) && (m_lpNumsData[2] == m_lpNumsData[1])) ? DTAAB_NONE : (((AAB_ABD_ABC_VALUE[_GetNumsIndex()] >> 6) & 0x03) + 1);
            if(nValue > DTAAB_BAA) nValue = DTAAB_NONE;	// 兼容以前
            break;
        case IID_STC_SHAPE_ABC: 
            nValue = (AAB_ABD_ABC_VALUE[_GetNumsIndex()] & 0x0F) + 1;
            if(nValue > DTABC_CBA) nValue = DTABC_NONE;	// 兼容以前
            break;
        case IID_STC_SHAPE_ABD: 
            nValue = ((AAB_ABD_ABC_VALUE[_GetNumsIndex()] >> 4) & 0x03) + 1;
            if(nValue > DTABD_DAB) nValue = DTABD_NONE;	// 兼容以前
            break;
        case IID_STC_SHAPE_COUNT_SAMENUM: 
        case IID_STC_SHAPE_COUNT_DIFFNUM:
        case IID_STC_SHAPE_COUNT_SAMEGRO: nValue = _GetSameOrDiffNumCountValue(cdtIID.dwSystemIID); break;
            
        case IID_STC_LINK_COMBO:
		{
			int nValues[8] = {0};
			nValue = _GetLinkCombo(nValues);
			return _FillMulValues(nValues, (BYTE)nValue);
		} break;
        case IID_STC_LINK_NUMCOUNT: nValue = _GetLinkCountValue(FALSE); break;
        case IID_STC_LINK_GROCOUNT: nValue = _GetLinkCountValue(TRUE); break;
        case IID_STC_LINK_NUMCOUNT_AS: nValue = _GetLinkASOrDsCountValue(FALSE, TRUE); break;
        case IID_STC_LINK_GROCOUNT_AS: nValue = _GetLinkASOrDsCountValue(TRUE, TRUE); break;
        case IID_STC_LINK_NUMCOUNT_DS: nValue = _GetLinkASOrDsCountValue(FALSE, FALSE); break;
        case IID_STC_LINK_GROCOUNT_DS: nValue = _GetLinkASOrDsCountValue(TRUE, FALSE); break;
        case IID_STC_LINK_COUNT_ODD: nValue = _GetOELinkCountValue(TRUE); break;
        case IID_STC_LINK_COUNT_EVEN: nValue = _GetOELinkCountValue(FALSE); break;
        case IID_STC_LINK_COUNT_PRIME:
        case IID_STC_LINK_COUNT_COMPOSITE: break; // TEST 暂时不支持
        case IID_STC_LINK_POS_2:
        case IID_STC_LINK_POS_3:
        case IID_STC_LINK_POS_4:
        case IID_STC_LINK_POS_5:
        case IID_STC_LINK_POS_6:
        case IID_STC_LINK_POS_7:
		{
			int nValues[8] = {0};
			nValue = _GetLinkPos(cdtIID.dwSystemIID - IID_STC_LINK_POS_2 + 2, nValues);
			return _FillMulValues(nValues, (BYTE)nValue);
		} break;
            
        case IID_STC_DHK_HD:
        case IID_STC_DHK_KD:
        case IID_STC_DHK_HK: nValue = _GetDHKValue(cdtIID.dwSystemIID) == -1 ? 0 : 1; break;
        case IID_STC_DHK_HDKD:
        case IID_STC_DHK_HDKDHK:  nValue = _GetDHKValue(cdtIID.dwSystemIID); break;
            
        case IID_STC_NUMS_SIN: 
        case IID_STC_NUMS_GRO: nValue = _GetNumsValue(cdtIID.dwSystemIID); break;
            
        case IID_STC_POS_1: nValue = m_lpNumsData[0]; break;
        case IID_STC_POS_2: nValue = m_lpNumsData[1]; break;
        case IID_STC_POS_3: nValue = m_lpNumsData[2]; break;
        case IID_STC_POS_4: nValue = m_lpNumsData[3]; break;
        case IID_STC_POS_5: nValue = m_lpNumsData[4]; break;
        case IID_STC_POS_6: nValue = m_lpNumsData[5]; break;
        case IID_STC_POS_7: nValue = m_lpNumsData[6]; break;
        case IID_STC_POS_8: nValue = m_lpNumsData[7]; break;
        case IID_STC_POS_ANY: 
		{
			int nValues[8] = {0};
			for(int i=0; i<m_btNumberCount; i++)
			{
				nValues[i] = m_lpNumsData[i];
			}
			return _FillMulValues(nValues, m_btNumberCount);
		} break;
            
        case IID_STC_SUM: nValue = _GetSumValue(); break;
        case IID_STC_SUMTAIL: nValue = _GetSumValue() % 10; break;
        case IID_STC_SUM_N: nValue = _GetSumNValue(); break;
        case IID_STC_SPAN: nValue = _GetSpanValue(); break;
        case IID_STC_SPANTAIL: nValue = _GetSpanValue() % 10; break;
        case IID_STC_AVG: nValue = _GetAvgValue(); break;
        case IID_STC_AVGTAIL: nValue = _GetAvgValue() % 10; break;
        case IID_STC_AC: nValue = _GetACValue(); break;
        case IID_STC_DETRACT: nValue = _GetDetractValue(); break;
            
        case IID_STC_ODDENEN_COUNT_O: nValue = _GetOddEvenCount(1); break;
        case IID_STC_ODDENEN_COUNT_E: nValue = _GetOddEvenCount(0); break;
        case IID_STC_ODDENEN_ARR: nValue = _GetOddEvenArrange(FALSE); break;
        case IID_STC_ODDENEN_ARR_VALUE: nValue = _GetOddEvenArrange(TRUE); break;
        case IID_STC_ODDENEN_LINK: nValue = _GetOddEvenLinkValue(); break;
            
        case IID_STC_BIGSMALL_COUNT_B: nValue = _GetBigSmallCount(1); break;
        case IID_STC_BIGSMALL_COUNT_S: nValue = _GetBigSmallCount(0); break;
        case IID_STC_BIGSMALL_ARR: nValue = _GetBigSmallArrange(FALSE); break;
        case IID_STC_BIGSMALL_ARR_VALUE: nValue = _GetBigSmallArrange(TRUE); break;
            
        case IID_STC_PRIMECOMPOSITE_COUNT_P: nValue = _GetPrimeCompositeCount(1); break;
        case IID_STC_PRIMECOMPOSITE_COUNT_C: nValue = _GetPrimeCompositeCount(0); break;
        case IID_STC_PRIMECOMPOSITE_ARR: nValue = _GetPrimeCompositeArrange(FALSE); break;
        case IID_STC_PRIMECOMPOSITE_ARR_VALUE: nValue = _GetPrimeCompositeArrange(TRUE); break;
        case IID_STC_PRIMECOMPOSITE_LINK: nValue = 0; break;
            
        case IID_STC_012_COUNT_0: nValue = _Get012Count(0); break;
        case IID_STC_012_COUNT_1: nValue = _Get012Count(1); break;
        case IID_STC_012_COUNT_2: nValue = _Get012Count(2); break;
        case IID_STC_012_SHAPE: nValue = _Get012Shape(); break;
        case IID_STC_012_ARR_SIN: nValue = _Get012Arrange(1, FALSE); break;
        case IID_STC_012_ARR_SIN_VALUE: nValue = _Get012Arrange(1, TRUE); break;
        case IID_STC_012_ARR_GRO: nValue = _Get012Arrange(2, FALSE); break;
        case IID_STC_012_ARR_GRO_VALUE: nValue = _Get012Arrange(2, TRUE); break;
        case IID_STC_012_ARR_COUNT: nValue = _Get012Arrange(3, FALSE); break;
        case IID_STC_012_ARR_COUNT_VALUE: nValue = _Get012Arrange(3, TRUE); break;
            
        case IID_STC_DZX_COUNT_D: nValue = _GetDZXCount(2); break;
        case IID_STC_DZX_COUNT_Z: nValue = _GetDZXCount(1); break;
        case IID_STC_DZX_COUNT_X: nValue = _GetDZXCount(0); break;
        case IID_STC_DZX_SHAPE: nValue = _GetDZXShape(); break;
        case IID_STC_DZX_ARR_SIN: nValue = _GetDZXArrange(1, FALSE); break;
        case IID_STC_DZX_ARR_SIN_VALUE: nValue = _GetDZXArrange(1, TRUE); break;
        case IID_STC_DZX_ARR_GRO: nValue = _GetDZXArrange(2, FALSE); break;
        case IID_STC_DZX_ARR_GRO_VALUE: nValue = _GetDZXArrange(2, TRUE); break;
        case IID_STC_DZX_ARR_COUNT: nValue = _GetDZXArrange(3, FALSE); break;
        case IID_STC_DZX_ARR_COUNT_VALUE: nValue = _GetDZXArrange(3, TRUE); break;
            
        case IID_STC_ESPECIALSUM_ODD: _GETSPECIALSUMVALUE(_GetOddEven, 1); break;
        case IID_STC_ESPECIALSUM_EVEN: _GETSPECIALSUMVALUE(_GetOddEven, 0); break;
        case IID_STC_ESPECIALSUM_BIG: _GETSPECIALSUMVALUE(_GetBigSmall, 1); break;
        case IID_STC_ESPECIALSUM_SMALL: _GETSPECIALSUMVALUE(_GetBigSmall, 0); break;
        case IID_STC_ESPECIALSUM_PRIME: _GETSPECIALSUMVALUE(_GetPrimeComposite, 1); break;
        case IID_STC_ESPECIALSUM_COMPOSITE: _GETSPECIALSUMVALUE(_GetPrimeComposite, 0); break;
        case IID_STC_ESPECIALSUM_0: _GETSPECIALSUMVALUE(_Get012, 0); break;
        case IID_STC_ESPECIALSUM_1: _GETSPECIALSUMVALUE(_Get012, 1); break;
        case IID_STC_ESPECIALSUM_2: _GETSPECIALSUMVALUE(_Get012, 2); break;
        case IID_STC_ESPECIALSUM_D: _GETSPECIALSUMVALUE(_GetDZX, 2); break;
        case IID_STC_ESPECIALSUM_Z: _GETSPECIALSUMVALUE(_GetDZX, 1); break;
        case IID_STC_ESPECIALSUM_X: _GETSPECIALSUMVALUE(_GetDZX, 0); break;
            
        case IID_STC_TWONUMSIN_ANY:
        case IID_STC_TWONUM_ANY: return _GetTwoNumsValue(cdtIID.dwSystemIID); break;
        case IID_STC_TWONUM_Z6:
        case IID_STC_TWONUM_Z6_2: nValue = _GetTwoNums_Z6(cdtIID.dwSystemIID); break;
        case IID_STC_TWONUM_Z3:
        case IID_STC_TWONUM_Z3_2: nValue = _GetTwoNums_Z3(cdtIID.dwSystemIID); break;
            
        case IID_STC_ORDER_MIN:
        case IID_STC_ORDER_MID: 
        case IID_STC_ORDER_MAX:
        case IID_STC_ORDER_MINADDMID:
        case IID_STC_ORDER_MINADDMAX:
        case IID_STC_ORDER_MIDADDMAX:
        case IID_STC_ORDER_POSSUM_ARR_SIN:
        case IID_STC_ORDER_POSSUM_ARR_GRO:
        case IID_STC_ORDER_MINADDMID_TAIL:
        case IID_STC_ORDER_MINADDMAX_TAIL:
        case IID_STC_ORDER_MIDADDMAX_TAIL:
        case IID_STC_ORDER_POSSUMTAIL_ARR_SIN:
        case IID_STC_ORDER_POSSUMTAIL_ARR_GRO:
        case IID_STC_ORDER_MIDSUBMIN:
        case IID_STC_ORDER_MAXSUBMIN:
        case IID_STC_ORDER_MAXSUBMID:
        case IID_STC_ORDER_POSSUB_ARR_SIN:
        case IID_STC_ORDER_POSSUB_ARR_GRO:
        case IID_STC_ORDER_MIDSUBMIN_TAIL:
        case IID_STC_ORDER_MAXSUBMIN_TAIL:
        case IID_STC_ORDER_MAXSUBMID_TAIL:
        case IID_STC_ORDER_POSSUBTAIL_ARR_SIN:
        case IID_STC_ORDER_POSSUBTAIL_ARR_GRO:
        case IID_STC_ORDER_RELATIVEDZX_POS_1:
        case IID_STC_ORDER_RELATIVEDZX_POS_2:
        case IID_STC_ORDER_RELATIVEDZX_POS_3:
        case IID_STC_ORDER_RELATIVEDZX_ARR:
        case IID_STC_ORDER_RELATIVEDZX_ARR_VALUE:
            return _GetOrderValue(cdtIID.dwSystemIID); break;
            
        case IID_STC_SPACE_SIN: 
        case IID_STC_SPACETAIL_SIN:
        case IID_STC_SPACE_SIN_HEAD:
        case IID_STC_SPACE_SIN_TAIL:
        case IID_STC_SPACE_SIN_1:
        case IID_STC_SPACE_SIN_2:
        case IID_STC_SPACE_SIN_3:
        case IID_STC_SPACE_SIN_4:
        case IID_STC_SPACE_SIN_5:
        case IID_STC_SPACE_SIN_6:
        case IID_STC_SPACE_SIN_7:
        case IID_STC_SPACE_SIN_8:
        case IID_STC_SPACE_GRO:
        case IID_STC_SPACE_GRO_HEAD:
        case IID_STC_SPACE_GRO_TAIL:
        case IID_STC_SPACE_GRO_1:
        case IID_STC_SPACE_GRO_2:
        case IID_STC_SPACE_GRO_3:
        case IID_STC_SPACE_GRO_4:
        case IID_STC_SPACE_GRO_5:
        case IID_STC_SPACE_GRO_6:
        case IID_STC_SPACE_GRO_7:
        case IID_STC_SPACE_GRO_8: nValue = _GetSpaceValue(cdtIID.dwSystemIID); break;
            
        case IID_STC_NEARSPACE_12: nValue = m_lpNumsData[1] - m_lpNumsData[0]; break;
        case IID_STC_NEARSPACE_23: nValue = m_lpNumsData[2] - m_lpNumsData[1]; break;
        case IID_STC_NEARSPACE_34: nValue = m_lpNumsData[3] - m_lpNumsData[2]; break;
        case IID_STC_NEARSPACE_45: nValue = m_lpNumsData[4] - m_lpNumsData[3]; break;
        case IID_STC_NEARSPACE_56: nValue = m_lpNumsData[5] - m_lpNumsData[4]; break;
        case IID_STC_NEARSPACE_67: nValue = m_lpNumsData[6] - m_lpNumsData[5]; break;
        case IID_STC_NEARSPACE_78: nValue = m_lpNumsData[7] - m_lpNumsData[6]; break;
        case IID_STC_NEARSPACE_HT: nValue = (m_btMaxNumber - m_btMinNumber + 1) - m_lpNumsData[m_btNumberCount-1] + m_lpNumsData[0]; break;
        case IID_STC_NEARSPACE_MAX:
        case IID_STC_NEARSPACE_MAXEX:
		{
			nValue = m_lpNumsData[1] - m_lpNumsData[0];
			int nOtherValue = 0;
			for(int i=2; i<m_btNumberCount; i++)
			{
				nOtherValue = m_lpNumsData[i] - m_lpNumsData[i-1];
				if(nValue < nOtherValue)
					nValue = nOtherValue;
			}
			if(cdtIID.dwSystemIID == IID_STC_NEARSPACE_MAXEX)
			{
				nOtherValue = (m_btMaxNumber - m_btMinNumber + 1) - m_lpNumsData[m_btNumberCount-1] + m_lpNumsData[0];
				if(nValue < nOtherValue)
					nValue = nOtherValue;
			}
		} break;
        case IID_STC_NEARSPACE_MIN:
        case IID_STC_NEARSPACE_MINEX:
		{
			nValue = m_lpNumsData[1] - m_lpNumsData[0];
			int nOtherValue = 0;
			for(int i=2; i<m_btNumberCount; i++)
			{
				nOtherValue = m_lpNumsData[i] - m_lpNumsData[i-1];
				if(nValue > nOtherValue)
					nValue = nOtherValue;
			}
			if(cdtIID.dwSystemIID == IID_STC_NEARSPACE_MINEX)
			{
				nOtherValue = (m_btMaxNumber - m_btMinNumber + 1) - m_lpNumsData[m_btNumberCount-1] + m_lpNumsData[0];
				if(nValue > nOtherValue)
					nValue = nOtherValue;
			}
		} break;
        case IID_STC_NEARSPACE_AVG:
		{
			for(int i=1; i<m_btNumberCount; i++)
			{
				nValue += m_lpNumsData[i] - m_lpNumsData[i-1];
			}
			double db = (double)nValue / (double)(m_btNumberCount - 1);
			nValue = int(db);
			if(db - (double)nValue >= 0.5)
				nValue ++;
		} break;
        case IID_STC_NEARSPACE_ANY:
		{
			int i = 0, nValues[8] = {0};
			for(i=1; i<m_btNumberCount; i++)
			{
				nValues[i-1] = m_lpNumsData[i] - m_lpNumsData[i-1];
			}
			nValues[i-1] = (m_btMaxNumber - m_btMinNumber + 1) - m_lpNumsData[m_btNumberCount-1] + m_lpNumsData[0];
			return _FillMulValues(nValues, (BYTE)i);
		} break;
            
        case IID_STC_1D_POSITIVE_SIN: 
        case IID_STC_1D_POSITIVE_GRO:
        case IID_STC_1D_NEGATIVE_SIN:
        case IID_STC_1D_NEGATIVE_GRO:
        case IID_STC_1D_RPOSITIVE_SIN:
        case IID_STC_1D_RPOSITIVE_GRO:
        case IID_STC_1D_RNEGATIVE_SIN:
        case IID_STC_1D_RNEGATIVE_GRO: nValue = _Get1DValue(cdtIID.dwSystemIID); break;
            
        case IID_STC_MUL:
        case IID_STC_MUL_ODDEVEN:
        case IID_STC_MUL_BIGSMALL:
        case IID_STC_MUL_012:
        case IID_STC_MULTAIL:
        case IID_STC_POSMULSUM:
        case IID_STC_POSMULSUMTAIL:
        case IID_STC_RESPOSMULSUM:
        case IID_STC_RESPOSMULSUMTAIL: nValue = _GetProductValue(cdtIID.dwSystemIID); break;
            
            ////////////////////////////////////////////////////////////////////////////////
        case IID_DNC_DIS_POS_1: return _GetPrevDataValue(0, dwIssueIndex, PDT_DIS); break;
        case IID_DNC_DIS_POS_2: return _GetPrevDataValue(1, dwIssueIndex, PDT_DIS); break;
        case IID_DNC_DIS_POS_3: return _GetPrevDataValue(2, dwIssueIndex, PDT_DIS); break;
        case IID_DNC_DIS_POS_4: return _GetPrevDataValue(3, dwIssueIndex, PDT_DIS); break;
        case IID_DNC_DIS_POS_5: return _GetPrevDataValue(4, dwIssueIndex, PDT_DIS); break;
        case IID_DNC_DIS_POS_6: return _GetPrevDataValue(5, dwIssueIndex, PDT_DIS); break;
        case IID_DNC_DIS_POS_7: return _GetPrevDataValue(6, dwIssueIndex, PDT_DIS); break;
        case IID_DNC_DIS_POS_8: return _GetPrevDataValue(7, dwIssueIndex, PDT_DIS); break;
        case IID_DNC_DIS_SUM: return _GetPrevDataValue(-1, dwIssueIndex, PDT_DIS); break;
        case IID_DNC_DIS_SUMTAIL: return _GetPrevDataValue(-2, dwIssueIndex, PDT_DIS); break;
        case IID_DNC_DIS_POS_ANY: return _GetPrevDataValue(-3, dwIssueIndex, PDT_DIS); break;
        case IID_DNC_DIS_ARR_SIN: return _GetPrevDataValue(-4, dwIssueIndex, PDT_DIS); break;
        case IID_DNC_DIS_ARR_GRO: return _GetPrevDataValue(-5, dwIssueIndex, PDT_DIS); break;
            
        case IID_DNC_XL_LEFT: nValue = _GetXLValue(0, dwIssueIndex); break;
        case IID_DNC_XL_RIGHT: nValue = _GetXLValue(1, dwIssueIndex); break;
        case IID_DNC_XL_VALUE: nValue = _GetXLValue(2, dwIssueIndex); break;
        case IID_DNC_XL_COMBO: nValue = _GetXLValue(3, dwIssueIndex); break;
            
        case IID_DNC_DEPARTURE: nValue = _GetDeparture(dwIssueIndex); break;
            
        case IID_DNC_NUMS_MATCHCOUNT_SIN:
        case IID_DNC_NUMS_MATCHCOUNT_GRO: nValue = _GetNumsMatchCount(cdtIID.dwSystemIID, dwIssueIndex, btType); break;
            
        case IID_DNC_TESTNUMS_REPEAT_CUR_CUR:
        case IID_DNC_TESTNUMS_REPEAT_PRE_CUR: nValue = _GetSameCount_TestNums(cdtIID.dwSystemIID, dwIssueIndex); break;
        case IID_DNC_TESTNUMS_REPEAT_COMPLEX:
        case IID_DNC_TESTNUMS_REPEAT_COMBO: nValue = _GetSameCount_TestNums_PrevNums(cdtIID.dwSystemIID, dwIssueIndex); break;
            
        case IID_DNC_REPEAT_1TON:
        case IID_DNC_REPEAT_1TON+1:
        case IID_DNC_REPEAT_1TON+2:
        case IID_DNC_REPEAT_1TON+3:
        case IID_DNC_REPEAT_1TON+4:
        case IID_DNC_REPEAT_1TON+5:
        case IID_DNC_REPEAT_1TON+6:
        case IID_DNC_REPEAT_1TON+7:
        case IID_DNC_REPEAT_1TON+8:
        case IID_DNC_REPEAT_1TON+9: 
        case IID_DNC_REPEAT_N:
        case IID_DNC_REPEAT_N+1:
        case IID_DNC_REPEAT_N+2:
        case IID_DNC_REPEAT_N+3:
        case IID_DNC_REPEAT_N+4:
        case IID_DNC_REPEAT_N+5:
        case IID_DNC_REPEAT_N+6:
        case IID_DNC_REPEAT_N+7:
        case IID_DNC_REPEAT_N+8:
        case IID_DNC_REPEAT_N+9: nValue = _GetRepeatCountValue(cdtIID.dwSystemIID, dwIssueIndex); break;
            
        case IID_DNC_SKIPSIN_1: nValue = g_pIData->GetSkipValue(GSKF_POS_1, dwIssueIndex, m_lpNumsData, TRUE); break;
        case IID_DNC_SKIPSIN_2: nValue = g_pIData->GetSkipValue(GSKF_POS_2, dwIssueIndex, m_lpNumsData, TRUE); break;
        case IID_DNC_SKIPSIN_3: nValue = g_pIData->GetSkipValue(GSKF_POS_3, dwIssueIndex, m_lpNumsData, TRUE); break;
        case IID_DNC_SKIPSIN_4: nValue = g_pIData->GetSkipValue(GSKF_POS_4, dwIssueIndex, m_lpNumsData, TRUE); break;
        case IID_DNC_SKIPSIN_5: nValue = g_pIData->GetSkipValue(GSKF_POS_5, dwIssueIndex, m_lpNumsData, TRUE); break;
        case IID_DNC_SKIPSIN_6: nValue = g_pIData->GetSkipValue(GSKF_POS_6, dwIssueIndex, m_lpNumsData, TRUE); break;
        case IID_DNC_SKIPSIN_7: nValue = g_pIData->GetSkipValue(GSKF_POS_7, dwIssueIndex, m_lpNumsData, TRUE); break;
        case IID_DNC_SKIPSIN_8: nValue = g_pIData->GetSkipValue(GSKF_POS_8, dwIssueIndex, m_lpNumsData, TRUE); break;
        case IID_DNC_SKIPSIN_MIN: nValue = g_pIData->GetSkipValue(GSKF_MIN, dwIssueIndex, m_lpNumsData, TRUE); break;
        case IID_DNC_SKIPSIN_MAX: nValue = g_pIData->GetSkipValue(GSKF_MAX, dwIssueIndex, m_lpNumsData, TRUE); break;
            
        case IID_DNC_SKIPSIN_TAIL_1: nValue = g_pIData->GetSkipValue(GSKF_POS_1, dwIssueIndex, m_lpNumsData, TRUE) % 10; break;
        case IID_DNC_SKIPSIN_TAIL_2: nValue = g_pIData->GetSkipValue(GSKF_POS_2, dwIssueIndex, m_lpNumsData, TRUE) % 10; break;
        case IID_DNC_SKIPSIN_TAIL_3: nValue = g_pIData->GetSkipValue(GSKF_POS_3, dwIssueIndex, m_lpNumsData, TRUE) % 10; break;
        case IID_DNC_SKIPSIN_TAIL_4: nValue = g_pIData->GetSkipValue(GSKF_POS_4, dwIssueIndex, m_lpNumsData, TRUE) % 10; break;
        case IID_DNC_SKIPSIN_TAIL_5: nValue = g_pIData->GetSkipValue(GSKF_POS_5, dwIssueIndex, m_lpNumsData, TRUE) % 10; break;
        case IID_DNC_SKIPSIN_TAIL_6: nValue = g_pIData->GetSkipValue(GSKF_POS_6, dwIssueIndex, m_lpNumsData, TRUE) % 10; break;
        case IID_DNC_SKIPSIN_TAIL_7: nValue = g_pIData->GetSkipValue(GSKF_POS_7, dwIssueIndex, m_lpNumsData, TRUE) % 10; break;
        case IID_DNC_SKIPSIN_TAIL_8: nValue = g_pIData->GetSkipValue(GSKF_POS_8, dwIssueIndex, m_lpNumsData, TRUE) % 10; break;
        case IID_DNC_SKIPSIN_TAIL_MIN: nValue = g_pIData->GetSkipValue(GSKF_TAIL_MIN, dwIssueIndex, m_lpNumsData, TRUE); break;
        case IID_DNC_SKIPSIN_TAIL_MAX: nValue = g_pIData->GetSkipValue(GSKF_TAIL_MAX, dwIssueIndex, m_lpNumsData, TRUE); break;
        case IID_DNC_SKIPSIN_TAIL_ANY:
		{
			int nSkipValues[8] = {0};
			for(int i=0; i<m_btNumberCount; i++)
			{
				nSkipValues[i] = g_pIData->GetSkipValue(GSKF_POS_1 + i, dwIssueIndex, m_lpNumsData, TRUE) % 10;
			}
			return _FillMulValues(nSkipValues, m_btNumberCount);
		} break;
        case IID_DNC_SKIPSIN_TAIL_ARR_SIN: nValue = g_pIData->GetSkipValue(GSKF_TAILARR_SINGLE, dwIssueIndex, m_lpNumsData, TRUE); break;
        case IID_DNC_SKIPSIN_TAIL_ARR_GRO: nValue = g_pIData->GetSkipValue(GSKF_TAILARR_GROUP, dwIssueIndex, m_lpNumsData, TRUE); break;
            
        case IID_DNC_SKIPSIN_SUM: nValue = g_pIData->GetSkipValue(GSKF_SUM, dwIssueIndex, m_lpNumsData, TRUE); break;
        case IID_DNC_SKIPSIN_SUM_AREA:
		{
			int nMax = (m_btNumberCount == 1) ? 50 : m_btNumberCount * 20;
			if(nMax > 90) nMax = 90;
			nValue = g_pIData->GetSkipValue(GSKF_SUM, dwIssueIndex, m_lpNumsData, TRUE);
			if(nValue > nMax)
				nValue = nMax;
		}
            break;
        case IID_DNC_SKIPSIN_SUM_AREA10:
            nValue = g_pIData->GetSkipValue(GSKF_SUM, dwIssueIndex, m_lpNumsData, TRUE);
            if(m_btNumberCount >= 4)
            {
                nValue /= 10;
                if(nValue == 0) nValue = 1;
                if(nValue > 10) nValue = 10;
            }
            else
            {
                nValue /= 6;
                if(nValue > 9) nValue = 9;
            }
            break;
        case IID_DNC_SKIPSIN_SUMTAIL: nValue = g_pIData->GetSkipValue(GSKF_SUM_TAIL, dwIssueIndex, m_lpNumsData, TRUE); break;
            
        case IID_DNC_SKIPGRO_1: nValue = g_pIData->GetSkipValue(GSKF_POS_1, dwIssueIndex, m_lpNumsData, FALSE); break;
        case IID_DNC_SKIPGRO_2: nValue = g_pIData->GetSkipValue(GSKF_POS_2, dwIssueIndex, m_lpNumsData, FALSE); break;
        case IID_DNC_SKIPGRO_3: nValue = g_pIData->GetSkipValue(GSKF_POS_3, dwIssueIndex, m_lpNumsData, FALSE); break;
        case IID_DNC_SKIPGRO_4: nValue = g_pIData->GetSkipValue(GSKF_POS_4, dwIssueIndex, m_lpNumsData, FALSE); break;
        case IID_DNC_SKIPGRO_5: nValue = g_pIData->GetSkipValue(GSKF_POS_5, dwIssueIndex, m_lpNumsData, FALSE); break;
        case IID_DNC_SKIPGRO_6: nValue = g_pIData->GetSkipValue(GSKF_POS_6, dwIssueIndex, m_lpNumsData, FALSE); break;
        case IID_DNC_SKIPGRO_7: nValue = g_pIData->GetSkipValue(GSKF_POS_7, dwIssueIndex, m_lpNumsData, FALSE); break;
        case IID_DNC_SKIPGRO_8: nValue = g_pIData->GetSkipValue(GSKF_POS_8, dwIssueIndex, m_lpNumsData, FALSE); break;
        case IID_DNC_SKIPGRO_MIN: nValue = g_pIData->GetSkipValue(GSKF_MIN, dwIssueIndex, m_lpNumsData, FALSE); break;
        case IID_DNC_SKIPGRO_MAX: nValue = g_pIData->GetSkipValue(GSKF_MAX, dwIssueIndex, m_lpNumsData, FALSE); break;
            
        case IID_DNC_SKIPGRO_TAIL_1: nValue = g_pIData->GetSkipValue(GSKF_POS_1, dwIssueIndex, m_lpNumsData, FALSE) % 10; break;
        case IID_DNC_SKIPGRO_TAIL_2: nValue = g_pIData->GetSkipValue(GSKF_POS_2, dwIssueIndex, m_lpNumsData, FALSE) % 10; break;
        case IID_DNC_SKIPGRO_TAIL_3: nValue = g_pIData->GetSkipValue(GSKF_POS_3, dwIssueIndex, m_lpNumsData, FALSE) % 10; break;
        case IID_DNC_SKIPGRO_TAIL_4: nValue = g_pIData->GetSkipValue(GSKF_POS_4, dwIssueIndex, m_lpNumsData, FALSE) % 10; break;
        case IID_DNC_SKIPGRO_TAIL_5: nValue = g_pIData->GetSkipValue(GSKF_POS_5, dwIssueIndex, m_lpNumsData, FALSE) % 10; break;
        case IID_DNC_SKIPGRO_TAIL_6: nValue = g_pIData->GetSkipValue(GSKF_POS_6, dwIssueIndex, m_lpNumsData, FALSE) % 10; break;
        case IID_DNC_SKIPGRO_TAIL_7: nValue = g_pIData->GetSkipValue(GSKF_POS_7, dwIssueIndex, m_lpNumsData, FALSE) % 10; break;
        case IID_DNC_SKIPGRO_TAIL_8: nValue = g_pIData->GetSkipValue(GSKF_POS_8, dwIssueIndex, m_lpNumsData, FALSE) % 10; break;
        case IID_DNC_SKIPGRO_TAIL_MIN: nValue = g_pIData->GetSkipValue(GSKF_TAIL_MIN, dwIssueIndex, m_lpNumsData, FALSE); break;
        case IID_DNC_SKIPGRO_TAIL_MAX: nValue = g_pIData->GetSkipValue(GSKF_TAIL_MAX, dwIssueIndex, m_lpNumsData, FALSE); break;
        case IID_DNC_SKIPGRO_TAIL_ANY:
		{
			int nSkipValues[8] = {0};
			for(int i=0; i<m_btNumberCount; i++)
			{
				nSkipValues[i] = g_pIData->GetSkipValue(GSKF_POS_1 + i, dwIssueIndex, m_lpNumsData, FALSE) % 10;
			}
			return _FillMulValues(nSkipValues, m_btNumberCount);
		} break;
        case IID_DNC_SKIPGRO_TAIL_ARR_SIN: nValue = g_pIData->GetSkipValue(GSKF_TAILARR_SINGLE, dwIssueIndex, m_lpNumsData, FALSE); break;
        case IID_DNC_SKIPGRO_TAIL_ARR_GRO: nValue = g_pIData->GetSkipValue(GSKF_TAILARR_GROUP, dwIssueIndex, m_lpNumsData, FALSE); break;
            
        case IID_DNC_SKIPGRO_SUM: nValue = g_pIData->GetSkipValue(GSKF_SUM, dwIssueIndex, m_lpNumsData, FALSE); break;
        case IID_DNC_SKIPGRO_SUM_AREA:
            nValue = g_pIData->GetSkipValue(GSKF_SUM, dwIssueIndex, m_lpNumsData, FALSE);
            if(nValue > 30)
                nValue = 30;
            break;
        case IID_DNC_SKIPGRO_SUM_AREA10:
            nValue = g_pIData->GetSkipValue(GSKF_SUM, dwIssueIndex, m_lpNumsData, FALSE);
            if(m_btNumberCount >= 4)
            {
                nValue /= 2;
            }
            else
            {
                nValue /= 3;
            }
            if(nValue > 9) nValue = 9;
            break;
        case IID_DNC_SKIPGRO_SUMTAIL: nValue = g_pIData->GetSkipValue(GSKF_SUM_TAIL, dwIssueIndex, m_lpNumsData, FALSE); break;
            
        case IID_HIS_NUMS_ARR_SIN:
        case IID_HIS_NUMS_1_ARR_SIN:
        case IID_HIS_NUMS_2_ARR_SIN:
        case IID_HIS_NUMS_ARR_GRO:
        case IID_HIS_NUMS_1_ARR_GRO:
        case IID_HIS_NUMS_2_ARR_GRO: nValue = 0; break;		// 不需要计算结果，在历史中处理
            
        default: 
		{
			if(cdtIID.dwSystemIID >= IID_STC_NUMBER_0NUM && cdtIID.dwSystemIID < IID_STC_NUMBER_SAMECOMBO)	// 数字N个数
			{
				BYTE btNumX = (BYTE)(cdtIID.dwSystemIID - IID_STC_NUMBER_0NUM);
				for(int i=0; i<m_btNumberCount; i++)
				{
					if(m_lpNumsData[i] == btNumX)
						nValue ++;
				}
			}
			else if(cdtIID.dwSystemIID >= IID_DNC_XXX_BEGIN && cdtIID.dwSystemIID <= IID_DNC_XXX_END)	// 动态3态指标
			{
				nValue = _Get3StateValue(cdtIID.dwSystemIID, dwIssueIndex);
			}
			else if(cdtIID.dwSystemIID >= IID_STC_TWOPOS_BEGIN && cdtIID.dwSystemIID <= IID_STC_TWOPOS_END)	// 两位关系
			{
				return _GetTwoPosValue(cdtIID.dwSystemIID);
			}
			else if(cdtIID.dwSystemIID >= IID_STC_TWONUMSIN_BEGIN && cdtIID.dwSystemIID <= IID_STC_TWONUMSIN_END)	// 单选二码
			{
				int nType = cdtIID.dwSystemIID - IID_STC_TWONUMSIN_BEGIN;
				nValue = m_lpNumsData[nType / 10] * 10 + m_lpNumsData[nType % 10];
			}
			else if(cdtIID.dwSystemIID >= IID_STC_REMM_BEGIN && cdtIID.dwSystemIID <= IID_STC_REMM_END)		// 余数
			{
				return _GetRemainderValue(cdtIID.dwSystemIID);
			}
			else if(cdtIID.dwSystemIID >= IID_DNC_PREVDATA_BEGIN && cdtIID.dwSystemIID <= IID_DNC_PREVDATA_END)	// 邻期关系
			{
				int nTypeIndex = (cdtIID.dwSystemIID - IID_DNC_PREVDATA_BEGIN) / 100;
				switch(cdtIID.dwSystemIID - nTypeIndex * 100)
				{
                    case IID_DNC_PREVDATA_POS_1: return _GetPrevDataValue(0, dwIssueIndex, nTypeIndex); break;
                    case IID_DNC_PREVDATA_POS_2: return _GetPrevDataValue(1, dwIssueIndex, nTypeIndex); break;
                    case IID_DNC_PREVDATA_POS_3: return _GetPrevDataValue(2, dwIssueIndex, nTypeIndex); break;
                    case IID_DNC_PREVDATA_POS_4: return _GetPrevDataValue(3, dwIssueIndex, nTypeIndex); break;
                    case IID_DNC_PREVDATA_POS_5: return _GetPrevDataValue(4, dwIssueIndex, nTypeIndex); break;
                    case IID_DNC_PREVDATA_POS_6: return _GetPrevDataValue(5, dwIssueIndex, nTypeIndex); break;
                    case IID_DNC_PREVDATA_POS_7: return _GetPrevDataValue(6, dwIssueIndex, nTypeIndex); break;
                    case IID_DNC_PREVDATA_POS_8: return _GetPrevDataValue(7, dwIssueIndex, nTypeIndex); break;
                    case IID_DNC_PREVDATA_ANY: return _GetPrevDataValue(-3, dwIssueIndex, nTypeIndex); break;
                    case IID_DNC_PREVDATA_MIN: return _GetPrevDataValue(-6, dwIssueIndex, nTypeIndex); break;
                    case IID_DNC_PREVDATA_MID: return _GetPrevDataValue(-7, dwIssueIndex, nTypeIndex); break;
                    case IID_DNC_PREVDATA_MAX: return _GetPrevDataValue(-8, dwIssueIndex, nTypeIndex); break;
                    case IID_DNC_PREVDATA_SUM: return _GetPrevDataValue(-1, dwIssueIndex, nTypeIndex); break;
                    case IID_DNC_PREVDATA_SUMTAIL: return _GetPrevDataValue(-2, dwIssueIndex, nTypeIndex); break;
                    case IID_DNC_PREVDATA_ARR_SIN: return _GetPrevDataValue(-4, dwIssueIndex, nTypeIndex); break;
                    case IID_DNC_PREVDATA_ARR_GRO: return _GetPrevDataValue(-5, dwIssueIndex, nTypeIndex); break;
                    default: ASSERT(FALSE); break;
				}
			}
			else
			{
				ASSERT(FALSE);
			}
		} break;
	}
    
	return _FillSingleValues(nValue);
}

/////////////////////////////////////////////////////////////////////////////////////////
LPCDTVALUE CIndicator::_FillMulValues(int *pValues, BYTE btValueCount)
{
	ASSERT(pValues != NULL && btValueCount > 0);
    
	// 先进行排序
	g_pICstPubFunc->QuickSort(pValues, btValueCount, QSVT_INT);
    
	LPCDTVALUEITEM lpItem = NULL;
	int i = 0, j = 0, nIndex = 0;
    
	for(i=0; i<btValueCount; i++)
	{
		for(j=0; j<nIndex; j++)
		{
			lpItem = &m_lpValueTemp->stValueItems[j];
			if(lpItem->nValue == pValues[i])
			{
				// 值已经存在，则只需要对应的个数加1
				lpItem->btCount ++;
				break;
			}
		}
        
		if(j == nIndex)
		{
			// 值不存在，则新添加一个值
			lpItem = &m_lpValueTemp->stValueItems[nIndex++];
			lpItem->btCount = 1;
			lpItem->nValue = pValues[i];
		}
	}
    
	m_lpValueTemp->btItemCount = (BYTE)nIndex;
    
	return m_lpValueTemp;
}

LPCDTVALUE CIndicator::_FillSingleValues(int nValue)
{
	m_lpValueTemp->btItemCount = (BYTE)1;
	m_lpValueTemp->stValueItems[0].nValue = nValue;
	m_lpValueTemp->stValueItems[0].btCount = 1;
    
	return m_lpValueTemp;
}

void CIndicator::_GetOrderedNumsData(LPBYTE lpOrderedNumsData)
{
	ASSERT(lpOrderedNumsData != NULL && m_lpNumsData != NULL);
    
	memcpy(lpOrderedNumsData, m_lpNumsData, m_btNumberCount);
	g_pICstPubFunc->QuickSort(lpOrderedNumsData, m_btNumberCount);
}

//////////////////////////////////////////////////////////////////////////////////////////////////
int CIndicator::_GetNumsIndex()
{
	int i = 0, j = 1, nValue = 0, nRatio = 10; // m_btMaxNumber - m_btMinNumber + 1;
	
	for(i=m_btNumberCount-1; i>=0; i--)
	{
		nValue += m_lpNumsData[i] * j;
		j *= nRatio;
	}
	
	return nValue;
}

int CIndicator::_GetSameNumGroupCount(int nSameCount, int *pValuesOut)
{
	int i = 0, nValue = 0;
	BYTE btNum[128] = {0};
	
	for(i=0; i<m_btNumberCount; i++)
		btNum[m_lpNumsData[i]]++;
	
	if(nSameCount == 0)			//	相同数字组合
	{
		ASSERT(pValuesOut != NULL);
		for(i=m_btMinNumber; i<=m_btMaxNumber; i++)
		{
			if(btNum[i] > 1)
			{
				pValuesOut[nValue ++] = btNum[i];
			}
		}
        
		if(nValue == 0)
		{
			pValuesOut[0] = 0;
			nValue = 1;
		}
		else
		{
			g_pICstPubFunc->QuickSort(pValuesOut, nValue, QSVT_INT);
		}
	}
	else if(nSameCount == 1)	// 任意数字最大出现个数
	{
		for(i=m_btMinNumber; i<=m_btMaxNumber; i++)
		{
			if(btNum[i] > nValue)
			{
				nValue = btNum[i];
			}
		}
	}
	else						// N个相同数字组数
	{
		for(i=m_btMinNumber; i<=m_btMaxNumber; i++)
		{
			if(btNum[i] == nSameCount)
				nValue++;
		}
	}
	
	return nValue;
}

int CIndicator::_GetTypeValue()
{
	if(m_btDataType & DATA_TYPE_NUM_SZ && m_btNumberCount == 3)
	{
		// 数字三
		return (TYPE_VALUE[_GetNumsIndex()] >> 4) & 0x0F;
	}
    
	int i = 0;
	BYTE btTemp[100] = {0}, btMaxSame = 1, btAllNumberCount = g_pIData->GetAllNumberCount();
    
	// 计算最大重复号码个数
	if(m_btNumberCount > 2)
	{
		for(i=0; i<m_btNumberCount; i++)
		{
			btTemp[m_lpNumsData[i]] ++;
			if(btTemp[m_lpNumsData[i]] > btMaxSame)
				btMaxSame = btTemp[m_lpNumsData[i]];
		}
	}	
	
	switch(m_btNumberCount)
	{
        case 2:
            if(m_lpNumsData[0] == m_lpNumsData[1])
            {
                return DT_AA;
            }
            i = abs(m_lpNumsData[0] - m_lpNumsData[1]);
            if(i == 1 || (i == (m_btMaxNumber - m_btMinNumber) && (m_btDataType & DATA_TYPE_NUM_SZ)))
            {
                return DT_AB;
            }
            else if(i == 2 || ((i == 8) && (m_btDataType & DATA_TYPE_NUM_SZ)))
            {
                return (m_lpNumsData[0] % 2 == 0) ? DT_EL : DT_OL;
            }
            return DT_AD;
            break;
        case 3:
		{
			if(btMaxSame > 2)
			{
				return DT_AAA;
			}
			else if(btMaxSame == 2)
			{
				return DT_AAB;
			}
			else
			{
				BYTE btLinkCount = 0, btMaxLinkCount = 1;
				for(i=0; i<btAllNumberCount+m_btNumberCount; i++)
				{
					if(btTemp[(i % btAllNumberCount) + m_btMinNumber] == 0)
					{
						if(btMaxLinkCount < btLinkCount)
							btMaxLinkCount = btLinkCount;
						btLinkCount = 0;
					}
					else
					{
						btLinkCount ++;
					}
				}
				return DT_ACE - (btMaxLinkCount - 1);
			}
		}
        case 4:
		{
			if(btMaxSame > 2)
			{
				return DT_AAAA + (4 - btMaxSame);
			}
			else if(btMaxSame == 2)
			{
				for(i=m_btMinNumber; i<=m_btMaxNumber; i++)
				{
					if(btTemp[i] > 0 && btTemp[i] != 2)
					{
						return DT_AABC;	// 1对
					}
				}
				return DT_AABB;		// 两对
			}
			else
			{
				BYTE btLinkCount = 0, btMaxLinkCount = 1;
				for(i=0; i<btAllNumberCount+m_btNumberCount; i++)
				{
					if(btTemp[(i % btAllNumberCount) + m_btMinNumber] == 0)
					{
						if(btMaxLinkCount < btLinkCount)
							btMaxLinkCount = btLinkCount;
						btLinkCount = 0;
					}
					else
					{
						btLinkCount ++;
					}
				}
				return DT_ACEG - (btMaxLinkCount - 1);
			}
		}break;
        default: ASSERT(FALSE);
	}
	
	return CDTVALUE_INVALID;
}

int CIndicator::_GetTwoLinkValue()
{
	ASSERT(m_btNumberCount == 3);	// 目前只用于三个数字的号码
	
	BYTE btOrderNumsData[MAX_NUMBER_COUNT] = {0};
	memcpy(btOrderNumsData, m_lpNumsData, m_btNumberCount);
	g_pICstPubFunc->QuickSort(btOrderNumsData, m_btNumberCount);
    
	if((btOrderNumsData[0] == m_btMinNumber) && (btOrderNumsData[2] == m_btMaxNumber))
	{
		return 1;
	}
	else if((btOrderNumsData[2] - btOrderNumsData[1] == 1) || (btOrderNumsData[1] - btOrderNumsData[0] == 1))
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

int CIndicator::_GetShapeValue()
{
	if((m_btDataType & DATA_TYPE_NUM_SZ) && m_btNumberCount == 3)
	{
		// 数字三
		return (STATE_VALUE[_GetNumsIndex()] >> 4) & 0x0F;
	}
	
	switch(m_btNumberCount)
	{
        case 2:
		{
			if(m_lpNumsData[0] == m_lpNumsData[1])
			{
				return DS_PARALLEL;
			}
			if(m_lpNumsData[0] < m_lpNumsData[1])
			{
				return DS_ASCEND;
			}
			return DS_DESCEND;
		} break;
        case 3:
		{
			if(m_lpNumsData[0] == m_lpNumsData[1] && m_lpNumsData[1] == m_lpNumsData[2])
			{
				return DS_PARALLEL;
			}
			if(m_lpNumsData[0] <= m_lpNumsData[1] && m_lpNumsData[1] <= m_lpNumsData[2])
			{
				return DS_ASCEND;
			}
			if(m_lpNumsData[0] >= m_lpNumsData[1] && m_lpNumsData[1] >= m_lpNumsData[2])
			{
				return DS_DESCEND;
			}
			if(m_lpNumsData[0] <= m_lpNumsData[1] && m_lpNumsData[1] >= m_lpNumsData[2])
			{
				return DS_HEAVE;
			}
			return DS_CONCAVE;
		} break;
        default: ASSERT(FALSE);
	}
	
	return CDTVALUE_INVALID;
}

int CIndicator::_GetEqualDiffValue()
{
	BYTE btOrderNumsData[MAX_NUMBER_COUNT] = {0};
	memcpy(btOrderNumsData, m_lpNumsData, m_btNumberCount);
	g_pICstPubFunc->QuickSort(btOrderNumsData, m_btNumberCount);
    
	int nValue = btOrderNumsData[1] - btOrderNumsData[0];
	for(int i=m_btNumberCount-1; i>=2; i--)
	{
		if(nValue != btOrderNumsData[i] - btOrderNumsData[i-1])
		{
			nValue = -1;	// 不等差
			break;
		}
	}
	
	return nValue;
}

int CIndicator::_GetSameOrDiffNumCountValue(DWORD dwIID)
{
	int nValue = 0, i = 0;
	
	// 置标志
	BYTE btTemp[100] = {0};
	for(i=0; i<m_btNumberCount; i++)
	{
		btTemp[m_lpNumsData[i]] ++;
	}
	
	switch(dwIID)
	{
        case IID_STC_SHAPE_COUNT_DIFFNUM:		// 不同号码个数
		{			
			for(i=m_btMinNumber; i<=m_btMaxNumber; i++)
			{
				if(btTemp[i] > 0)
				{
					nValue ++;
				}
			}
		}
            break;
            
        case IID_STC_SHAPE_COUNT_SAMENUM:
		{
			for(i=m_btMinNumber; i<=m_btMaxNumber; i++)
			{
				if(btTemp[i] > 1)
				{
					nValue += btTemp[i];
				}
			}
		} break;
        case IID_STC_SHAPE_COUNT_SAMEGRO:
		{
			for(i=m_btMinNumber; i<=m_btMaxNumber; i++)
			{
				if(btTemp[i] > 1)
				{
					nValue ++;
				}
			}
		} break;
	}
	
	return nValue;
}

int CIndicator::_GetLinkCountValue(BOOL bGroupCount)
{
	BOOL bLink = FALSE;
	int nBeginPos = 1, nEndPos = m_btNumberCount - 1, nGroupCount = 0, nNumCount = 0, nDiff = 0;
	BYTE btOrderedNumsData[MAX_NUMBER_COUNT] = {0};
	LPBYTE lpNumsData = m_lpNumsData;
    
	if(!(m_btDataType & DATA_TYPE_ORDERED))		// 需要排序
	{
		_GetOrderedNumsData(btOrderedNumsData);
		lpNumsData = btOrderedNumsData;
	}
	
	// 先处理首尾连，暂时不需要首尾连
    /*	if(lpNumsData[0] == m_btMinNumber && lpNumsData[m_btNumberCount-1] == m_btMaxNumber)
     {
     nNumCount = nGroupCount = 1;			// 组数和连号数设为1
     bLink = TRUE;
     
     for(nBeginPos=1; nBeginPos<m_btNumberCount-1; nBeginPos++)	// 查询后连号
     {
     nDiff = lpNumsData[nBeginPos] - lpNumsData[nBeginPos-1];
     if(nDiff == 1)	// 继续连号
     {
     nNumCount ++;
     }
     else if(nDiff != 0)
     {
     break;
     }
     }
     for(nEndPos=m_btNumberCount-2; nEndPos>0; nEndPos--)	// 查询前连号
     {
     nDiff = lpNumsData[nEndPos+1] - lpNumsData[nEndPos];
     if(nDiff == 1)	// 继续连号
     {
     nNumCount ++;
     }
     else if(nDiff != 0)
     {
     break;
     }
     }
     }
     */
	// 查询剩下的连号
	for(int i=nBeginPos; i<=nEndPos; i++)
	{
		nDiff = lpNumsData[i] - lpNumsData[i-1];
		if(nDiff == 0)
		{
			continue;	// 忽略
		}
		else if(nDiff == 1)	// 连号
		{
			if(!bLink)
			{
				nGroupCount ++;			// 组数加1
				bLink = TRUE;
			}
			nNumCount ++;			// 个数加1
		}
		else
		{
			bLink = FALSE;
		}
	}
	
	if(bGroupCount)	// 获取组数
	{
		return nGroupCount;
	}
	else			// 获取个数
	{
		return nNumCount + nGroupCount;
	}
}

int CIndicator::_GetLinkASOrDsCountValue(BOOL bGroupCount, BOOL bAscOrder)
{
	BOOL bLink = FALSE;
	int nGroupCount = 0, nNumCount = 0, nDiff = 0;
	int nCompValue = bAscOrder ? 1 : -1; //nCompValue2 = bAscOrder ? (m_btMinNumber - m_btMaxNumber) : (m_btMaxNumber - m_btMinNumber);
	
	for(int i=1; i<m_btNumberCount; i++)
	{
		nDiff = m_lpNumsData[i] - m_lpNumsData[i-1];
		//if(nDiff == nCompValue || nDiff == nCompValue2)	// 升序或降序连号
		if(nDiff == nCompValue)
		{
			if(!bLink)
			{
				nGroupCount ++;			// 组数加1
				bLink = TRUE;
			}
			nNumCount ++;			// 个数加1
		}
		else
		{
			bLink = FALSE;
		}
	}
	
	if(bGroupCount)	// 获取组数
	{
		return nGroupCount;
	}
	else			// 获取个数
	{
		return nNumCount + nGroupCount;
	}
}

int CIndicator::_GetOELinkCountValue(BOOL bOddLink)
{
	BOOL bLink = FALSE;
	int nGroupCount = 0, nNumCount = 0, nDiffValue = bOddLink ? 1 : 0;
	
	for(int i=1; i<m_btNumberCount; i++)
	{
		if((m_lpNumsData[i] % 2 == nDiffValue) && (m_lpNumsData[i] - m_lpNumsData[i-1] == 2))
		{
			if(!bLink)
			{
				nGroupCount ++;			// 组数加1
				bLink = TRUE;
			}
			nNumCount ++;			// 个数加1
		}
		else
		{
			bLink = FALSE;
		}
	}
	
	return nNumCount + nGroupCount;
}

int CIndicator::_GetLinkCombo(LPINT lpLinkCountArray)
{
	ASSERT(lpLinkCountArray != NULL);
	
	BOOL bLink = FALSE;
	int i, nLinkCount = 0, nLinkGroup = 0;
	BYTE btOrderedNumsData[MAX_NUMBER_COUNT] = {0};
	LPBYTE lpNumsData = m_lpNumsData;
	
	if(!(m_btDataType & DATA_TYPE_ORDERED))
	{
		_GetOrderedNumsData(btOrderedNumsData);
		lpNumsData = btOrderedNumsData;
	}
    
	for(i=1; i<m_btNumberCount; i++)
	{
		switch(lpNumsData[i] - lpNumsData[i-1])
		{
            case 0: continue;
            case 1:
                if(!bLink)
                {
                    bLink = TRUE;
                    nLinkCount = 2;
                }
                else
                {
                    nLinkCount ++;			// 个数加1
                }
                break;
            default: 
                if(nLinkCount > 0)
                {
                    lpLinkCountArray[nLinkGroup++] = nLinkCount;
                    nLinkCount = 0;
                }
                bLink = FALSE;
                break;
		}
	}
	if(nLinkCount > 0)
	{
		lpLinkCountArray[nLinkGroup++] = nLinkCount;
	}
	if(nLinkGroup == 0)
	{
		lpLinkCountArray[0] = 0;
		nLinkGroup = 1;
	}
	
	return nLinkGroup;
}

int CIndicator::_GetLinkPos(int nLinkCount, int *lpPosArray)
{
	ASSERT(nLinkCount >= 2 && nLinkCount <= m_btNumberCount);
	ASSERT(lpPosArray != NULL);
	ASSERT(m_btDataType & DATA_TYPE_ORDERED);		// 必须是已排序的数据类型
	
	BOOL bLink = FALSE;
	int i, nPos = 0, nCount = 0, j = 0;
    /*	BYTE btOrderedNumsData[MAX_NUMBER_COUNT] = {0};
     LPBYTE lpNumsData = m_lpNumsData;
     
     
     if(!(m_btDataType & DATA_TYPE_ORDERED))
     {
     _GetOrderedNumsData(btOrderedNumsData);
     lpNumsData = btOrderedNumsData;
     }
     */
	for(i=1; i<m_btNumberCount; i++)
	{
		switch(m_lpNumsData[i] - m_lpNumsData[i-1])
		{
            case 0: continue;
            case 1:
                if(!bLink)
                {
                    bLink = TRUE;
                    nCount = 2;
                    nPos = i;
                }
                else
                {
                    nCount ++;
                }
                break;
                
            default:
                if(nLinkCount == nCount)
                {
                    lpPosArray[j++] = nPos;
                    nCount = 0;
                }
                bLink = FALSE;
                break;
		}
	}
	if(nLinkCount == nCount)
	{
		lpPosArray[j++] = nPos;
	}
	
	if(j == 0)
	{
		lpPosArray[0] = 0;
		j ++;
	}
    
	return j;
}

int CIndicator::_GetDHKValue(DWORD dwIID)
{
	switch(dwIID) 
	{
        case IID_STC_DHK_KD:
		{
			BYTE btSpan = (BYTE)_GetSpanValue();
			for(int i=0; i<m_btNumberCount; i++)
			{
				if(m_lpNumsData[i] == btSpan)
				{
					return btSpan;
				}
			}
		} break;
            
        case IID_STC_DHK_HD:
		{
			BYTE btSumTail = (BYTE)(_GetSumValue() % 10);
			for(int i=0; i<m_btNumberCount; i++)
			{
				if(m_lpNumsData[i] == btSumTail)
				{
					return btSumTail;
				}
			}
		} break;
            
        case IID_STC_DHK_HK:
		{
			BYTE btSpan = (BYTE)_GetSpanValue();
			if(btSpan == (BYTE)(_GetSumValue() % 10))
				return btSpan;
		} break;
            
        case IID_STC_DHK_HDKD:
		{
			BOOL bDH = _GetDHKValue(IID_STC_DHK_HD) == -1 ? FALSE : TRUE, bDK = _GetDHKValue(IID_STC_DHK_KD) == -1 ? FALSE : TRUE;
			if(bDH)
			{
				if(bDK)
					return 0;		// 合胆同跨胆同
				else
					return 1;		// 合胆同跨胆不同
			}
			else
			{
				if(bDK)
					return 2;		// 合胆不同跨胆同
				else
					return 3;		// 合胆不同跨胆不同
			}
		} break;
            
        case IID_STC_DHK_HDKDHK:
		{
			BOOL bDH = _GetDHKValue(IID_STC_DHK_HD) == -1 ? FALSE : TRUE, bDK = _GetDHKValue(IID_STC_DHK_KD) == -1 ? FALSE : TRUE, bHK = _GetDHKValue(IID_STC_DHK_HK) == -1 ? FALSE : TRUE;
			if(bDH)
			{
				if(bDK)
				{
					if(bHK)
						return 0;		// 合胆同跨胆同 合跨同
					else
						return 1;		// 合胆同跨胆同 合跨不同
				}
				else
					return 2;		// 合胆同跨胆不同 合跨不同
			}
			if(bDK)
				return 3;			// 合胆不同跨胆同 合跨不同
			
			if(bHK)
				return 4;			// 合胆不同跨胆不同 合跨同
			else
				return 5;			// 合胆不同跨胆不同 合跨不同
		} break;
            
        default:
            break;
	}
	
	return -1;
}

int CIndicator::_GetNumsValue(DWORD dwIID)
{
	int i = 0, j = 1, nValue = 0;
    
	if(dwIID == IID_STC_NUMS_SIN)	// 单选
	{	
		for(i=m_btNumberCount-1; i>=0; i--)
		{
			nValue += m_lpNumsData[i] * j;
			j *= 10;
		}	
	}
	else	// 组选
	{
		BYTE btOrderNumsData[MAX_NUMBER_COUNT] = {0};
		_GetOrderedNumsData(btOrderNumsData);
		for(i=m_btNumberCount-1; i>=0; i--)
		{
			nValue += btOrderNumsData[i] * j;
			j *= 10;
		}	
	}
    
	return nValue;
}

int CIndicator::_GetPosSum(int nPosIndex, BOOL bGetTail)
{
	int nValue = m_lpNumsData[nPosIndex] / 10 + m_lpNumsData[nPosIndex] % 10;
	
	if(bGetTail)
	{
		nValue %= 10;
	}
	
	return nValue;
}

int CIndicator::_GetPosSpan(int nPosIndex, BOOL bGetAbs)
{
	int nValue = m_lpNumsData[nPosIndex] / 10 - m_lpNumsData[nPosIndex] % 10;
	
	if(bGetAbs && (nValue < 0))
	{
		nValue = -nValue;
	}
	
	return nValue;
}

int CIndicator::_GetSumValue()
{
	if(m_btNumberCount == 1)
	{
		return _GetPosSum(0);	// 如果只有1个号码，则使用该位数字和
	}
	
	int nValue = 0;
	
	for(int i=0; i<m_btNumberCount; i++) 
		nValue += m_lpNumsData[i];
	
	return nValue;
}

int CIndicator::_GetSumNValue()
{
	int nValue = 0, nTemp = 0;
	
	for(int i=0; i<m_btNumberCount; i++) 
		nValue += m_lpNumsData[i];
	
	while(nValue > 9)
	{
		nTemp = 0;
		while(nValue != 0)
		{
			nTemp += nValue % 10;
			nValue /= 10;
		}
		nValue = nTemp;
	}
    
	return nValue;
}

int CIndicator::_GetSpanValue()
{
	if(m_btNumberCount == 1)
	{
		return _GetPosSpan(0);	// 如果只有1个号码，则使用该位数字跨度
	}
	
	int nMin = m_lpNumsData[0], nMax = m_lpNumsData[0];
	for(int i=1; i<m_btNumberCount; i++)
	{
		if(m_lpNumsData[i] > nMax)	nMax = m_lpNumsData[i];
		if(m_lpNumsData[i] < nMin)	nMin = m_lpNumsData[i];
	}
	return nMax - nMin;
}

int CIndicator::_GetAvgValue()
{
	if(m_btNumberCount == 1)	// 如果只有1个号码，则使用该位数字和的均值
	{
		return (_GetPosSum(0) + 1) / 2;	
	}
	
	int nValue = 0;
	
	for(int i=0; i<m_btNumberCount; i++) 
		nValue += m_lpNumsData[i];
	
	return int(((double)nValue / (double)m_btNumberCount) + 0.5);
}

int CIndicator::_GetACValue()
{
	BYTE i, j, btTemp[100] = {0};
	int nValue = 0, nTemp = 0;
	
	for(i=0; i<m_btNumberCount-1; i++)
	{
		for(j=BYTE(i+1); j<m_btNumberCount; j++)
		{
			nTemp = abs(m_lpNumsData[i] - m_lpNumsData[j]);
			if(btTemp[nTemp] == 0)
			{
				btTemp[nTemp] = 1;
				nValue ++;
			}
		}
	}
    
	nValue -= (m_btNumberCount - 1);
	
	return nValue;
}

int CIndicator::_GetDetractValue()
{
	// 以双色球为例，所谓散度，指的是01~33这33个号码与6个开奖号码之差的绝对值的最小值中的最大的一个
	int i, j, nMin = 0, nTemp = 0, nValue = 0;
	
	for(i=m_btMinNumber; i<=m_btMaxNumber; i++)
	{
		nMin = m_btMaxNumber;
		for(j=0; j<m_btNumberCount; j++)
		{
			nTemp = abs(i - m_lpNumsData[j]);
			if(nTemp < nMin)
			{
				nMin = nTemp;
			}
		}
		if(nValue < nMin)
		{
			nValue = nMin;
		}
	}
	
	return nValue;
}

int CIndicator::_GetDeparture(DWORD dwIssueIndex)
{
	// 获取前一期数据索引
	DWORD dwPrevIssueIndex = g_pIData->GetPrevDataIndex(dwIssueIndex);
	if(dwPrevIssueIndex == DATA_INDEX_INVALID)
	{
		return CDTVALUE_INVALID;	// 无效，则返回错误
	}
    
	// 以双色球为例，所谓偏度，指的是本期6个开奖号码与上期6个开奖号码之差的绝对值的最小值中的最大的一个
	int i, j, nMin = 0, nMax = 0, nTemp = 0;
	LPBYTE lpLastNums = NULL;
	
	lpLastNums = g_pIData->GetItemNums(dwPrevIssueIndex);
    
	for(i=0; i<m_btNumberCount; i++)
	{
		nMin = m_btMaxNumber;
		for(j=0; j<m_btNumberCount; j++)
		{
			nTemp = abs(m_lpNumsData[i] - lpLastNums[j]);
			if(nTemp < nMin)
			{
				nMin = nTemp;
			}
		}
		if(nMax < nMin)
		{
			nMax = nMin;
		}
	}
	
	return nMax;
}

int CIndicator::_GetOddEvenCount(int nFlag)
{
	ASSERT(nFlag >= 0 && nFlag <= 2);
	int tempValue = 0;
	
	for(int i=0; i<m_btNumberCount; i++)
	{
		if(nFlag == (m_lpNumsData[i] % 2))
			tempValue++;
	}
	
	return tempValue;
}

int CIndicator::_GetOddEvenArrange(BOOL bGetValue)
{
	int j = 1, nRtn = 0;
	
	for(int i=m_btNumberCount-1; i>=0; i--)
	{
		nRtn += (m_lpNumsData[i] % 2) * j;
		j *= (bGetValue ? 2 : 10);
	}
    
	if(bGetValue && m_btNumberCount == 3)	// 特殊处理
	{
		if(nRtn == 3) 
			nRtn = 4;
		else if(nRtn == 4) 
			nRtn = 3;
	}
	
	return nRtn;
}

int CIndicator::_GetOddEvenLinkValue()
{
	int i, j, nValue = -1, nTemp = -1, nTemp2 = -1;
	int nDiffValue = 0;
	
	BYTE btOrderNumsData[MAX_NUMBER_COUNT] = {0};
	memcpy(btOrderNumsData, m_lpNumsData, m_btNumberCount);
	g_pICstPubFunc->QuickSort(btOrderNumsData, m_btNumberCount);
    
	for(i=0; i<m_btNumberCount-1; i++)
	{
		for(j=i+1; j<m_btNumberCount; j++)
		{
			nDiffValue = btOrderNumsData[j] - btOrderNumsData[i];
			if((nDiffValue == 2) || ((g_pIData->GetDataType() & DATA_TYPE_NUM_SZ) && (nDiffValue == 8)))
			{
				nTemp = btOrderNumsData[i] % 2;
				if(nTemp2 == -1)
				{
					nTemp2 = nTemp;
					nValue = (nTemp == 0) ? 1 : 0;
				}
				else if(nTemp != nTemp2)
				{
					nValue = 2;
					break;
				}
			}
		}
	}
	
	return nValue + 1;
}

int CIndicator::_GetBigSmall(int nValue)
{
// TEST	return theApp.GetSubAreaIndex(2, nValue, m_btMinNumber, m_btMaxNumber) - 1;
    return 0;
}

int CIndicator::_GetBigSmallCount(int nFlag)
{
	ASSERT(nFlag >= 0 && nFlag <= 2);
	int tempValue = 0;
	
	for(int i=0; i<m_btNumberCount; i++)
	{
		if(nFlag == _GetBigSmall(m_lpNumsData[i]))
			tempValue++;
	}
	
	return tempValue;
}

int CIndicator::_GetBigSmallArrange(BOOL bGetValue)
{
	int j = 1, nRtn = 0;
	
	for(int i=m_btNumberCount-1; i>=0; i--)
	{
		nRtn += _GetBigSmall(m_lpNumsData[i]) * j;
		j *= (bGetValue ? 2 : 10);
	}
	
	if(bGetValue && m_btNumberCount == 3)	// 特殊处理
	{
		if(nRtn == 3) 
			nRtn = 4;
		else if(nRtn == 4) 
			nRtn = 3;
	}
	
	return nRtn;
}

int CIndicator::_GetPrimeComposite(int nValue)
{
// TEST	return theApp.m_btPrimeFlag[nValue];
    return 0;
}

int CIndicator::_GetPrimeCompositeCount(int nFlag)
{
	ASSERT(nFlag >= 0 && nFlag <= 2);
	int tempValue = 0;
	
	for(int i=0; i<m_btNumberCount; i++)
	{
		if(nFlag == _GetPrimeComposite(m_lpNumsData[i]))
			tempValue++;
	}
	
	return tempValue;
}

int CIndicator::_GetPrimeCompositeArrange(BOOL bGetValue)
{
	int j = 1, nRtn = 0;
	
	for(int i=m_btNumberCount-1; i>=0; i--)
	{
// TEST		nRtn += theApp.m_btPrimeFlag[m_lpNumsData[i]] * j;
		j *= (bGetValue ? 2 : 10);
	}
	
	if(bGetValue && m_btNumberCount == 3)	// 特殊处理
	{
		if(nRtn == 3) 
			nRtn = 4;
		else if(nRtn == 4) 
			nRtn = 3;
	}
    
	return nRtn;
}

int CIndicator::_Get012Count(int nFlag)
{
	ASSERT(nFlag >= 0 && nFlag <= 2);
	int tempValue = 0;
	
	for(int i=0; i<m_btNumberCount; i++)
	{
		if(nFlag == (m_lpNumsData[i] % 3))
			tempValue++;
	}
	
	return tempValue;
}

int CIndicator::_Get012Shape()
{
	ASSERT(m_btNumberCount == 3);
    
	int nNum[3] = {0};
	for(int i=0; i<3; i++)
	{
		nNum[m_lpNumsData[i] % 3] ++;
	}
	
	if(nNum[0] == 1 && nNum[1] == 1)
		return 2;
	
	if(nNum[0] == 3 || nNum[1] == 3 || nNum[2] == 3)
		return 0;
	
	return 1;
}

int CIndicator::_Get012Arrange(BYTE btType, BOOL bGetValue)
{
	int i = 0, nRatio = 1, nRtn = 0;
	
	switch(btType)
	{
        case 1:		// 单选排列
		{
			for(i=m_btNumberCount-1; i>=0; i--)
			{
				nRtn += (m_lpNumsData[i] % 3) * nRatio;
				nRatio *= (bGetValue ? 3 : 10);
			}
		} break;
        case 2:		// 组选排列
		{
			if(bGetValue)
			{
				return _Get012Arrange(3, TRUE);		// 组选排列值和个数排列值相同
			}
            
			BYTE btTemp[MAX_NUMBER_COUNT] = {0};
			for(i=m_btNumberCount-1; i>=0; i--)
			{
				btTemp[i] = (BYTE)(m_lpNumsData[i] % 3);
			}
			g_pICstPubFunc->QuickSort(btTemp, m_btNumberCount);
			for(i=m_btNumberCount-1; i>=0; i--)
			{
				nRtn += btTemp[i] * nRatio;
				nRatio *= 10;
			}
		} break;
        case 3:		// 个数排列
		{
			BYTE btCount[8] = {0};
			for(i=m_btNumberCount-1; i>=0; i--)
			{
				btCount[m_lpNumsData[i] % 3] ++;
			}
			
			nRtn = btCount[0] * 100 + btCount[1] * 10 + btCount[2];
			if(bGetValue)
				nRtn = _GetIndexOfCountArrange(nRtn, 3, TRUE);
			
		} break;
	}
	
	return nRtn;
}

int CIndicator::_GetDZX(int nValue)
{
	return 0; // TEST theApp.GetSubAreaIndex(3, nValue, m_btMinNumber, m_btMaxNumber) - 1;
}

int CIndicator::_GetDZXCount(int nFlag)
{
	ASSERT(nFlag >= 0 && nFlag <= 2);
	int tempValue = 0;
	
	for(int i=0; i<m_btNumberCount; i++)
	{
		if(nFlag == _GetDZX(m_lpNumsData[i]))
			tempValue++;
	}
	
	return tempValue;
}

int CIndicator::_GetDZXShape()
{
	ASSERT(m_btNumberCount == 3);
	
	int nNum[3] = {0};
	for(int i=0; i<3; i++)
	{
		nNum[_GetDZX(m_lpNumsData[i])] ++;
	}
	
	if(nNum[0] == 1 && nNum[1] == 1)
		return 2;
	
	if(nNum[0] == 3 || nNum[1] == 3 || nNum[2] == 3)
		return 0;
	
	return 1;
}

int CIndicator::_GetDZXArrange(BYTE btType, BOOL bGetValue)
{
	int i = 0, nRatio = 1, nRtn = 0;
	
	switch(btType)
	{
        case 1:		// 单选排列
		{
			for(i=m_btNumberCount-1; i>=0; i--)
			{
				nRtn += _GetDZX(m_lpNumsData[i]) * nRatio;
				nRatio *= (bGetValue ? 3 : 10);
			}
		} break;
        case 2:		// 组选排列
		{
			if(bGetValue)
			{
				BYTE btCount[8] = {0};
				for(i=m_btNumberCount-1; i>=0; i--)
				{
					btCount[_GetDZX(m_lpNumsData[i])] ++;
				}
				
				nRtn = btCount[0] * 100 + btCount[1] * 10 + btCount[2];
				if(bGetValue)
					nRtn = _GetIndexOfCountArrange(nRtn, 3, TRUE);
			}
			else
			{
				BYTE btTemp[MAX_NUMBER_COUNT] = {0};
				for(i=m_btNumberCount-1; i>=0; i--)
				{
					btTemp[i] = (BYTE)_GetDZX(m_lpNumsData[i]);
				}
				g_pICstPubFunc->QuickSort(btTemp, m_btNumberCount);
				for(i=m_btNumberCount-1; i>=0; i--)
				{
					nRtn += btTemp[i] * nRatio;
					nRatio *= 10;
				}
			}
		} break;
        case 3:		// 个数排列
		{
			BYTE btCount[8] = {0};
			for(i=m_btNumberCount-1; i>=0; i--)
			{
				btCount[_GetDZX(m_lpNumsData[i])] ++;
			}
			
			nRtn = btCount[0] * 100 + btCount[1] * 10 + btCount[2];
			if(bGetValue)
				nRtn = _GetIndexOfCountArrange(nRtn, 3, TRUE);		
		} break;
	}
	
	return nRtn;
}

LPCDTVALUE CIndicator::_GetOrderValue(DWORD dwIID)
{
	int nValue = 0;
	BYTE btOrderNumsData[MAX_NUMBER_COUNT] = {0};
	memcpy(btOrderNumsData, m_lpNumsData, m_btNumberCount);
	g_pICstPubFunc->QuickSort(btOrderNumsData, m_btNumberCount);
    
	switch(dwIID)
	{
        case IID_STC_ORDER_MIN: nValue = btOrderNumsData[0]; break;
        case IID_STC_ORDER_MID:
            if(m_btNumberCount == 3)
            {
                nValue = btOrderNumsData[1];
            }
            else
            {
                int nCdtValues[8] = {0};
                for(int i=1; i<m_btNumberCount-1; i++)
                {
                    nCdtValues[i - 1] = btOrderNumsData[i];
                }
                return _FillMulValues(nCdtValues, BYTE(m_btNumberCount - 2));
            }
            break;
        case IID_STC_ORDER_MAX: nValue = btOrderNumsData[m_btNumberCount-1]; break;
        case IID_STC_ORDER_MINADDMID: nValue = btOrderNumsData[0] + btOrderNumsData[1]; break;
        case IID_STC_ORDER_MINADDMAX: nValue = btOrderNumsData[0] + btOrderNumsData[m_btNumberCount-1]; break;
        case IID_STC_ORDER_MIDADDMAX: nValue = btOrderNumsData[1] + btOrderNumsData[m_btNumberCount-1]; break;
        case IID_STC_ORDER_MINADDMID_TAIL: nValue = (btOrderNumsData[0] + btOrderNumsData[1]) % 10; break;
        case IID_STC_ORDER_MINADDMAX_TAIL: nValue = (btOrderNumsData[0] + btOrderNumsData[m_btNumberCount-1]) % 10; break;
        case IID_STC_ORDER_MIDADDMAX_TAIL: nValue = (btOrderNumsData[1] + btOrderNumsData[m_btNumberCount-1]) % 10; break;
        case IID_STC_ORDER_POSSUMTAIL_ARR_SIN:
        case IID_STC_ORDER_POSSUM_ARR_SIN:
		{
			ASSERT(m_btNumberCount == 3);
			nValue = ((btOrderNumsData[0] + btOrderNumsData[1]) % 10) * 100 + \
            ((btOrderNumsData[0] + btOrderNumsData[m_btNumberCount-1]) % 10) * 10 + \
            (btOrderNumsData[1] + btOrderNumsData[m_btNumberCount-1]) % 10;
		} break;
        case IID_STC_ORDER_POSSUMTAIL_ARR_GRO:
        case IID_STC_ORDER_POSSUM_ARR_GRO:
		{
			int nValues[3] = {(btOrderNumsData[0] + btOrderNumsData[1]) % 10, (btOrderNumsData[0] + btOrderNumsData[m_btNumberCount-1]) % 10, (btOrderNumsData[1] + btOrderNumsData[m_btNumberCount-1]) % 10};
			g_pICstPubFunc->QuickSort(nValues, 3, QSVT_INT);
			nValue = nValues[0] * 100 + nValues[1] * 10 + nValues[2];
		} break;
        case IID_STC_ORDER_MIDSUBMIN: nValue = btOrderNumsData[1] - btOrderNumsData[0]; break;
        case IID_STC_ORDER_MAXSUBMIN: nValue = btOrderNumsData[m_btNumberCount-1] - btOrderNumsData[0]; break;
        case IID_STC_ORDER_MAXSUBMID: nValue = btOrderNumsData[m_btNumberCount-1] - btOrderNumsData[1]; break;
        case IID_STC_ORDER_MIDSUBMIN_TAIL: nValue = (btOrderNumsData[1] - btOrderNumsData[0]) % 10; break;
        case IID_STC_ORDER_MAXSUBMIN_TAIL: nValue = (btOrderNumsData[m_btNumberCount-1] - btOrderNumsData[0]) % 10; break;
        case IID_STC_ORDER_MAXSUBMID_TAIL: nValue = (btOrderNumsData[m_btNumberCount-1] - btOrderNumsData[1]) % 10; break;
        case IID_STC_ORDER_POSSUB_ARR_SIN:
        case IID_STC_ORDER_POSSUBTAIL_ARR_SIN:
		{
			ASSERT(m_btNumberCount == 3);
			nValue = ((btOrderNumsData[1] - btOrderNumsData[0]) % 10) * 100 + \
            ((btOrderNumsData[m_btNumberCount-1] - btOrderNumsData[0]) % 10) * 10 + \
            (btOrderNumsData[m_btNumberCount-1] - btOrderNumsData[1]) % 10;
		} break;
        case IID_STC_ORDER_POSSUB_ARR_GRO:
        case IID_STC_ORDER_POSSUBTAIL_ARR_GRO:
		{
			int nValues[3] = {(btOrderNumsData[1] - btOrderNumsData[0]) % 10, (btOrderNumsData[m_btNumberCount-1] - btOrderNumsData[0]) % 10, (btOrderNumsData[m_btNumberCount-1] - btOrderNumsData[1]) % 10};
			g_pICstPubFunc->QuickSort(nValues, 3, QSVT_INT);
			nValue = nValues[0] * 100 + nValues[1] * 10 + nValues[2];
		} break;
        case IID_STC_ORDER_RELATIVEDZX_POS_1: nValue = _GetRelativedDZX(m_lpNumsData[0], btOrderNumsData); break;
        case IID_STC_ORDER_RELATIVEDZX_POS_2: nValue = _GetRelativedDZX(m_lpNumsData[1], btOrderNumsData); break;
        case IID_STC_ORDER_RELATIVEDZX_POS_3: nValue = _GetRelativedDZX(m_lpNumsData[2], btOrderNumsData); break;
        case IID_STC_ORDER_RELATIVEDZX_ARR: 
            nValue = _GetRelativedDZX(m_lpNumsData[0], btOrderNumsData) * 100 + _GetRelativedDZX(m_lpNumsData[1], btOrderNumsData) * 10 + _GetRelativedDZX(m_lpNumsData[2], btOrderNumsData);
            break;
        case IID_STC_ORDER_RELATIVEDZX_ARR_VALUE:
		{
			nValue = _GetRelativedDZX(m_lpNumsData[0], btOrderNumsData) * 100 + _GetRelativedDZX(m_lpNumsData[1], btOrderNumsData) * 10 + _GetRelativedDZX(m_lpNumsData[2], btOrderNumsData);
			if(m_btDataType & DATA_TYPE_REPEATABLE)
			{
				switch(nValue)
				{
                    case 2: nValue = 0; break;
                    case 12: nValue = 1; break;
                    case 20: nValue = 2; break;
                    case 21: nValue = 3; break;
                    case 22: nValue = 4; break;
                    case 102: nValue = 5; break;
                    case 111: nValue = 6; break;
                    case 120: nValue = 7; break;
                    case 200: nValue = 8; break;
                    case 201: nValue = 9; break;
                    case 202: nValue = 10; break;
                    case 210: nValue = 11; break;
                    case 220: nValue = 12; break;
                    default: ASSERT(FALSE); break;
				}
			}
			else
			{
				switch(nValue)
				{
                    case 12: nValue = 0; break;
                    case 21: nValue = 1; break;
                    case 102: nValue = 2; break;
                    case 120: nValue = 3; break;
                    case 201: nValue = 4; break;
                    case 210: nValue = 5; break;
                    default: ASSERT(FALSE); break;
				}
			}
		}
            break;
	}
    
	return _FillSingleValues(nValue);
}

int CIndicator::_GetSpaceValue(DWORD dwIID)
{
	LPBYTE lpNumsData = m_lpNumsData;
	BYTE btOrderedNumsData[MAX_NUMBER_COUNT] = {0};
	if(dwIID >= IID_STC_SPACE_GRO && dwIID <= IID_STC_SPACE_GRO_8)
	{
		_GetOrderedNumsData(btOrderedNumsData);
		lpNumsData = btOrderedNumsData;
	}
    
	int nValue = 0;
    
	switch(dwIID)
	{
        case IID_STC_SPACE_GRO:
        case IID_STC_SPACE_SIN:
        case IID_STC_SPACETAIL_SIN:
		{
			nValue = (lpNumsData[0] - m_btMinNumber) + (m_btMaxNumber - lpNumsData[m_btNumberCount - 1]);
			int nOtherValue = 0;
			for(int i=1; i<m_btNumberCount; i++)
			{
				nOtherValue = abs(lpNumsData[i] - lpNumsData[i-1]);
				if(nValue < nOtherValue)
					nValue = nOtherValue;
			}
			if(dwIID == IID_STC_SPACETAIL_SIN)
				nValue %= 10;
		} break;
        case IID_STC_SPACE_GRO_HEAD:
        case IID_STC_SPACE_SIN_HEAD: nValue = lpNumsData[0] - m_btMinNumber; break;
        case IID_STC_SPACE_GRO_TAIL:
        case IID_STC_SPACE_SIN_TAIL: nValue = m_btMaxNumber - lpNumsData[m_btNumberCount - 1]; break;
        case IID_STC_SPACE_GRO_1:
        case IID_STC_SPACE_SIN_1: nValue = (lpNumsData[0] - m_btMinNumber) + (m_btMaxNumber - lpNumsData[m_btNumberCount - 1]); break;
        case IID_STC_SPACE_GRO_2:
        case IID_STC_SPACE_SIN_2: nValue = abs(lpNumsData[1] - lpNumsData[0]); break;
        case IID_STC_SPACE_GRO_3:
        case IID_STC_SPACE_SIN_3: nValue = abs(lpNumsData[2] - lpNumsData[1]); break;
        case IID_STC_SPACE_GRO_4:
        case IID_STC_SPACE_SIN_4: nValue = abs(lpNumsData[3] - lpNumsData[2]); break;
        case IID_STC_SPACE_GRO_5:
        case IID_STC_SPACE_SIN_5: nValue = abs(lpNumsData[4] - lpNumsData[3]); break;
        case IID_STC_SPACE_GRO_6:
        case IID_STC_SPACE_SIN_6: nValue = abs(lpNumsData[5] - lpNumsData[4]); break;
        case IID_STC_SPACE_GRO_7:
        case IID_STC_SPACE_SIN_7: nValue = abs(lpNumsData[6] - lpNumsData[5]); break;
        case IID_STC_SPACE_GRO_8:
        case IID_STC_SPACE_SIN_8: nValue = abs(lpNumsData[7] - lpNumsData[6]); break;
	}
    
	return nValue;
}

int CIndicator::_Get1DValue(DWORD dwIID)
{
	ASSERT(m_btNumberCount == 3);	// 目前只支持3位
    
	LPBYTE lpNumsData = m_lpNumsData;
	BYTE btOrderedNumsData[MAX_NUMBER_COUNT] = {0};
	if(dwIID >= IID_STC_1D_POSITIVE_GRO && dwIID <= IID_STC_1D_RNEGATIVE_GRO)
	{
		_GetOrderedNumsData(btOrderedNumsData);
		lpNumsData = btOrderedNumsData;
	}
	
	int nValue = 0;
	switch(dwIID)
	{
        case IID_STC_1D_POSITIVE_SIN: 
        case IID_STC_1D_POSITIVE_GRO:
            nValue = ((lpNumsData[0] + lpNumsData[1]) % 10 + (lpNumsData[2] + lpNumsData[1]) % 10) % 10;
            break;
        case IID_STC_1D_NEGATIVE_GRO:
        case IID_STC_1D_NEGATIVE_SIN: 
            nValue = abs(abs(lpNumsData[0] - lpNumsData[1]) - abs(lpNumsData[2] - lpNumsData[1]));
            break;
        case IID_STC_1D_RPOSITIVE_GRO:
        case IID_STC_1D_RPOSITIVE_SIN:
            nValue = abs((lpNumsData[0] + lpNumsData[1]) % 10 - (lpNumsData[2] + lpNumsData[1]) % 10);
            break;
        case IID_STC_1D_RNEGATIVE_GRO:
        case IID_STC_1D_RNEGATIVE_SIN:
            nValue = (abs(lpNumsData[0] - lpNumsData[1]) + abs(lpNumsData[2] - lpNumsData[1])) % 10;
            break;
	}
    
	return nValue;
}

int CIndicator::_GetProductValue(DWORD dwIID)
{
	int nValue = 1;
  
    // TEST
    /*
	switch(dwIID)
	{
        case IID_STC_MUL: 
        case IID_STC_MUL_ODDEVEN:
        case IID_STC_MUL_BIGSMALL:
        case IID_STC_MUL_012:
        case IID_STC_MULTAIL:
            for(i=0; i<m_btNumberCount; i++)
            {
                nValue *= theApp.m_btProductNums[m_lpNumsData[i]];
            }
            switch(dwIID)
		{
            case IID_STC_MUL_ODDEVEN: if(nValue == 0) return -1; nValue %= 2; break; 
            case IID_STC_MUL_BIGSMALL: if(nValue == 0) return -1; nValue = (nValue >= theApp.m_wMinMaxProduct ? 1 : 0); break; 
            case IID_STC_MUL_012: if(nValue == 0) return -1; nValue %= 3; break; 
            case IID_STC_MULTAIL: nValue %= 10; break;
		}
            break;
            
        case IID_STC_POSMULSUM:
        case IID_STC_POSMULSUMTAIL:
            nValue = 0;
            for(i=0; i<m_btNumberCount; i++)
            {
                nValue += theApp.m_btProductNums[m_lpNumsData[i]] * (i + 1);
            }
            if(dwIID == IID_STC_POSMULSUMTAIL)
                nValue %= 10;
            break;
            
        case IID_STC_RESPOSMULSUM:
        case IID_STC_RESPOSMULSUMTAIL:
            nValue = 0;
            for(i=0; i<m_btNumberCount; i++)
            {
                nValue += theApp.m_btProductNums[m_lpNumsData[i]] * (m_btNumberCount - i);
            }
            if(dwIID == IID_STC_RESPOSMULSUMTAIL)
                nValue %= 10;
            break;
            
        default: ASSERT(FALSE); break;
	}
    */
	return nValue;
}

int CIndicator::_GetTwoNums_Z3(DWORD dwIID)
{
	ASSERT(m_btNumberCount == 3);	
	
	if(dwIID == IID_STC_TWONUM_Z3)
	{
		if(m_lpNumsData[0] == m_lpNumsData[1] && m_lpNumsData[0] == m_lpNumsData[2])
		{
			return DTNZ3_BZ;
		}
		BYTE btZ3Num = 0, btOtherNum = 0;
		if(m_lpNumsData[0] == m_lpNumsData[1])
		{
			btZ3Num = m_lpNumsData[0];
			btOtherNum = m_lpNumsData[2];
		}
		else if(m_lpNumsData[1] == m_lpNumsData[2])
		{
			btZ3Num = m_lpNumsData[1];
			btOtherNum = m_lpNumsData[0];
		}
		else if(m_lpNumsData[0] == m_lpNumsData[2])
		{
			btZ3Num = m_lpNumsData[2];
			btOtherNum = m_lpNumsData[1];
		}
		else
		{
			return DTNZ3_Z6;
		}
		return DTNZ3_00 + (btZ3Num % 3) * 3 + btOtherNum % 3;
	}
	else if(dwIID == IID_STC_TWONUM_Z3_2)
	{
		if(m_lpNumsData[0] == m_lpNumsData[1] && m_lpNumsData[0] == m_lpNumsData[2])
		{
			return DTNZ32_BZ;
		}
		BYTE btZ3Num = 0;
		if(m_lpNumsData[0] == m_lpNumsData[1])
		{
			btZ3Num = m_lpNumsData[0];
		}
		else if(m_lpNumsData[1] == m_lpNumsData[2])
		{
			btZ3Num = m_lpNumsData[1];
		}
		else if(m_lpNumsData[0] == m_lpNumsData[2])
		{
			btZ3Num = m_lpNumsData[2];
		}
		else
		{
			return DTNZ32_Z6;
		}
		return DTNZ32_0 + btZ3Num % 3;
	}
	else
	{
		ASSERT(FALSE);
	}
    
	return 0;
}

int CIndicator::_GetTwoNums_Z6(DWORD dwIID)
{
	ASSERT(m_btNumberCount == 3);	
    int i = 0;
    
	if(dwIID == IID_STC_TWONUM_Z6)
	{
		if(m_lpNumsData[0] == m_lpNumsData[1])
		{
			return (m_lpNumsData[1] == m_lpNumsData[2]) ? DTNZ6_BZ : DTNZ6_Z3;
		}
		if(m_lpNumsData[1] == m_lpNumsData[2] || m_lpNumsData[0] == m_lpNumsData[2])
		{
			return DTNZ6_Z3;
		}
		int n012Shape = _Get012Shape();
		if(n012Shape == 0)	// 003, 030, 300路
		{
			return DTNZ6_A;
		}
		if(n012Shape == 2)	// 111路
		{
			return DTNZ6_C;
		}
		BYTE btCompares[][2] = {{0, 3}, {0, 6}, {0, 9}, {1, 4}, {1, 7}, {2, 5}, {2, 8}, {MAXBYTE, MAXBYTE}, {3, 6}, {3, 9}, {4, 7}, {5, 8}, {6, 9}};
		BYTE btDataFlag[64] = {0};
		for(i=0; i<m_btNumberCount; i++)
		{
			btDataFlag[m_lpNumsData[i]] = 1;
		}
		for(i=0; i<=12; i++)
		{
			if(btCompares[i][0] == MAXBYTE)		// 无需比较
				continue;
            
			if(btDataFlag[btCompares[i][0]] != 0 && btDataFlag[btCompares[i][1]] != 0)
			{
				return DTNZ6_03 + i;
			}
		}
	}
	else if(dwIID == IID_STC_TWONUM_Z6_2)
	{
		if(m_lpNumsData[0] == m_lpNumsData[1])
		{
			return (m_lpNumsData[1] == m_lpNumsData[2]) ? DTNZ62_BZ : DTNZ62_Z3;
		}
		if(m_lpNumsData[1] == m_lpNumsData[2] || m_lpNumsData[0] == m_lpNumsData[2])
		{
			return DTNZ62_Z3;
		}
		BYTE btCount[8] = {0};
		for(i=m_btNumberCount-1; i>=0; i--)
		{
			btCount[m_lpNumsData[i] % 3] ++;
		}
		if(btCount[0] == 1 && btCount[1] == 1)	// 111路
		{
			return DTNZ62_C;
		}
		if(btCount[0] == 2)
		{
			return DTNZ62_00;
		}
		if(btCount[1] == 2)
		{
			return DTNZ62_11;
		}
		if(btCount[2] == 2)
		{
			return DTNZ62_22;
		}
		return DTNZ62_A;
	}
	else
	{
		ASSERT(FALSE);
	}
    
	return 0;
}

int CIndicator::_GetNumsMatchCount(DWORD dwIID, DWORD dwIssueIndex, BOOL /*bType*/)
{
	int nMatchCount = 0;
    
	if(dwIssueIndex == DATA_INDEX_INVALID || dwIssueIndex == DATA_INDEX_FORECAST)		// 用于缩水或者预测行
	{
		dwIssueIndex = g_pIData->GetItemCount() - 1;
		nMatchCount = 1;
	}
    
	if(dwIID == IID_DNC_NUMS_MATCHCOUNT_GRO)	// 组选次数
	{
		int i = 0, j = 0;
		BYTE btFlag[100] = {0}, btFlagOrg[100] = {0};
		LPBYTE lpHistoryNumsData = NULL;
        
		for(j=0; j<m_btNumberCount; j++)
			btFlagOrg[m_lpNumsData[j]] ++;
		
		for(i=dwIssueIndex; i>=0; i--)
		{
			memcpy(btFlag, btFlagOrg, m_btMaxNumber + 1);
            
			lpHistoryNumsData = g_pIData->GetItemNums(i);
			for(j=0; j<m_btNumberCount; j++)
			{
				if((btFlag[lpHistoryNumsData[j]]--) == 0)
				{
					break;	// not match
				}
			}
			if(j == m_btNumberCount)
			{
				nMatchCount ++;
			}
		}
	}
	else	// 单选次数
	{
		for(int i=dwIssueIndex; i>=0; i--)
		{
			if(!memcmp(g_pIData->GetItemNums(i), m_lpNumsData, m_btNumberCount))
			{
				nMatchCount ++;
			}
		}
	}
    
	// 特殊处理
    /*	switch(bType)
     {
     case CDTGVT_FILTER:			// 缩水
     nMatchCount ++;
     break;
     case CDTGVT_VALIDATE:		// 验证
     //		nMatchCount --;
     break;
     case CDTGVT_HISFILTER:		// 历史条件中的缩水
     nMatchCount ++;
     break;
     case CDTGVT_HISVALIDATE:	// 历史条件中的验证
     break;
     }*/
    
	return nMatchCount;
}

int CIndicator::_GetSameCount_TestNums(DWORD dwIID, DWORD dwIssueIndex)
{
	LPBYTE lpTestNumsData = NULL;
    
	if(dwIID == IID_DNC_TESTNUMS_REPEAT_CUR_CUR)	// 与当期试机号重复个数
	{
		lpTestNumsData = g_pIData->GetItemTestNums(dwIssueIndex);	
	}
	else	// 与上期试机号重复个数
	{
		DWORD dwPrevIndex = g_pIData->GetPrevDataIndex(dwIssueIndex);
		if(dwPrevIndex == DATA_INDEX_INVALID)
		{
			return CDTVALUE_INVALID;
		}
		
		lpTestNumsData = g_pIData->GetItemTestNums(dwPrevIndex);
	}
    
	if(lpTestNumsData == NULL)	// 没有试机号
	{
		return CDTVALUE_INVALID;
	}
    
	int i, j, nSameCount = 0;
	for(i=0; i<m_btNumberCount; i++)
	{
		for(j=0; j<m_btNumberCount; j++)
		{
			if(m_lpNumsData[i] == lpTestNumsData[j])
			{
				nSameCount ++;
				break;
			}
		}
	}
    
	return nSameCount;
}

int CIndicator::_GetSameCount_TestNums_PrevNums(DWORD dwIID, DWORD dwIssueIndex)
{
	// 获取当期试机号信息
	LPBYTE lpTestNumsData = g_pIData->GetItemTestNums(dwIssueIndex);
	if(lpTestNumsData == NULL)
	{
		return CDTVALUE_INVALID;	// 没有试机号
	}
    
	// 获取上期开奖号信息
	DWORD dwPrevIndex = g_pIData->GetPrevDataIndex(dwIssueIndex);
	if(dwPrevIndex == DATA_INDEX_INVALID)
	{
		return CDTVALUE_INVALID;
	}
    
	LPBYTE lpPrevNumsData = g_pIData->GetItemNums(dwPrevIndex);
	if(lpTestNumsData == NULL)	// 没有上期开奖号
	{
		return CDTVALUE_INVALID;
	}
    
	// 获取对应的相同号码个数
	int i, j, nTestSameCount = 0, nPrevNumsCount = 0;
	for(i=0; i<m_btNumberCount; i++)
	{
		for(j=0; j<m_btNumberCount; j++)
		{
			if(m_lpNumsData[i] == lpTestNumsData[j])
			{
				nTestSameCount ++;
				break;
			}
		}
		for(j=0; j<m_btNumberCount; j++)
		{
			if(m_lpNumsData[i] == lpPrevNumsData[j])
			{
				nPrevNumsCount ++;
				break;
			}
		}
	}
    
	int nRtn = 0;
	if(dwIID == IID_DNC_TESTNUMS_REPEAT_COMPLEX)	// 综合
	{
		nRtn = DTTESTNUMS_OTHER;
		if(nTestSameCount == 0)
		{
			if(nPrevNumsCount == 0)
			{
				nRtn = DTTESTNUMS_NONE;
			}
			else if(nPrevNumsCount == 1)
			{
				nRtn = DTTESTNUMS_NUMS1;
			}
		}
		else if(nTestSameCount == 1)
		{
			if(nPrevNumsCount == 0)
			{
				nRtn = DTTESTNUMS_TEST1;
			}
			else if(nPrevNumsCount == 1)
			{
				nRtn = DTTESTNUMS_ALL1;
			}
		}
	}
	else	// 组合
	{
		nRtn = nTestSameCount * 10 + nPrevNumsCount;
	}
    
	return nRtn;
}

LPCDTVALUE CIndicator::_GetTwoNumsValue(DWORD dwIID)
{
	int nValues[256] = {0}, nIndex = 0;
	for(int i=0; i<m_btNumberCount-1; i++)
	{
		for(int j=i+1; j<m_btNumberCount; j++)
		{
			if(dwIID == IID_STC_TWONUM_ANY)	// 组选二码
			{
				if(m_lpNumsData[i] > m_lpNumsData[j])
					nValues[nIndex++] = m_lpNumsData[j] * 10 + m_lpNumsData[i];
				else
					nValues[nIndex++] = m_lpNumsData[i] * 10 + m_lpNumsData[j];
			}
			else	// 单选二码
			{
				nValues[nIndex++] = m_lpNumsData[i] * 10 + m_lpNumsData[j];
			}
		}
	}
	
	return _FillMulValues(nValues, (BYTE)nIndex);
}

LPCDTVALUE CIndicator::_GetTwoPosValue(DWORD dwIID)
{
	int nTypeIndex = (dwIID - IID_STC_TWOPOS_BEGIN) / 100;
	int nValue = 0;
    
	if(nTypeIndex == TPT_SUMSUB)	// 二码合差
	{
		int nValues[256] = {0};
		switch(dwIID - nTypeIndex * 100)
		{
            case IID_STC_TWOPOS_ANY:
			{
				for(int i=0; i<m_btNumberCount-1; i++)
				{
					for(int j=i+1; j<m_btNumberCount; j++)
					{
						nValues[nValue++] = (m_lpNumsData[i] + m_lpNumsData[j]) % 10;
						nValues[nValue++] = abs(m_lpNumsData[i] - m_lpNumsData[j]) % 10;
					}
				}
			} break;
            default: // IID_STC_TWOPOS_MN
			{
				int nPos2 = dwIID % 10, nPos1 = (dwIID / 10) % 10;
				nValues[nValue++] = (m_lpNumsData[nPos1] + m_lpNumsData[nPos2]) % 10;
				nValues[nValue++] = abs(m_lpNumsData[nPos1] - m_lpNumsData[nPos2]) % 10;
			} break;
		}
        
		return _FillMulValues(nValues, (BYTE)nValue);
	}
	else if(nTypeIndex == TPT_SUMCIRSUB)	// 二码合环差
	{
		int nValues[256] = {0};
		switch(dwIID - nTypeIndex * 100)
		{
            case IID_STC_TWOPOS_ANY:
			{
				for(int i=0; i<m_btNumberCount-1; i++)
				{
					for(int j=i+1; j<m_btNumberCount; j++)
					{
						nValues[nValue++] = (m_lpNumsData[i] + m_lpNumsData[j]) % 10;
						nValues[nValue++] = abs(m_lpNumsData[i] - m_lpNumsData[j]) % 10;
						nValues[nValue++] = ((m_btMaxNumber - m_btMinNumber + 1) - nValues[nValue-1]) % 10;
					}
				}
			} break;
            default: // IID_STC_TWOPOS_MN
			{
				int nPos2 = dwIID % 10, nPos1 = (dwIID / 10) % 10;
				nValues[nValue++] = (m_lpNumsData[nPos1] + m_lpNumsData[nPos2]) % 10;
				nValues[nValue++] = abs(m_lpNumsData[nPos1] - m_lpNumsData[nPos2]) % 10;
				nValues[nValue++] = ((m_btMaxNumber - m_btMinNumber + 1) - nValues[nValue-1]) % 10;
			} break;
		}
		
		return _FillMulValues(nValues, (BYTE)nValue);
	}
    
	switch(dwIID - nTypeIndex * 100)
	{
        case IID_STC_TWOPOS_ANY:
		{
			int nValues[256] = {0};
			nValue = _CalcAllTwoPos(nTypeIndex, FALSE, nValues);
			return _FillMulValues(nValues, (BYTE)nValue);
		} break;
        case IID_STC_TWOPOS_MIN:
		{
			int nValues[256] = {0};
			_CalcAllTwoPos(nTypeIndex, TRUE, nValues);
			nValue = nValues[0];
		} break;
        case IID_STC_TWOPOS_MID:
		{
			int nValues[256] = {0};
			_CalcAllTwoPos(nTypeIndex, TRUE, nValues);
			nValue = nValues[1];
		} break;
        case IID_STC_TWOPOS_MAX:
		{
			int nValues[256] = {0};
			nValue = _CalcAllTwoPos(nTypeIndex, TRUE, nValues);
			nValue = nValues[nValue-1];
		} break;
        case IID_STC_TWOPOS_HEADTAIL: nValue = _CalcTwoPos(0, m_btNumberCount-1, nTypeIndex); break;
        case IID_STC_TWOPOS_ARR_SIN:
		{
			int nValues[256] = {0};
			_CalcAllTwoPos(nTypeIndex, FALSE, nValues);
			nValue = nValues[0] * 100 + nValues[1] * 10 + nValues[2];
		} break;
        case IID_STC_TWOPOS_ARR_GRO:
		{
			int nValues[256] = {0};
			_CalcAllTwoPos(nTypeIndex, TRUE, nValues);
			nValue = nValues[0] * 100 + nValues[1] * 10 + nValues[2];
		} break;
        default: // IID_STC_TWOPOS_MN
		{
			int nPos2 = dwIID % 10, nPos1 = (dwIID / 10) % 10;
			nValue = _CalcTwoPos(nPos1, nPos2, nTypeIndex);
		} break;
	}
    
	return _FillSingleValues(nValue);
}

LPCDTVALUE CIndicator::_GetRemainderValue(DWORD dwIID)
{
	int nDivisorIndex = (dwIID - IID_STC_REMM_BEGIN) / 100;
	int nValue = 0, nDivisor = nDivisorIndex + 2;
	BOOL bGetArrangeValue = TRUE;
    
	switch(dwIID - nDivisorIndex * 100)
	{
        case IID_STC_REMM_POS_1: nValue = m_lpNumsData[0] % nDivisor; break;
        case IID_STC_REMM_POS_2: nValue = m_lpNumsData[1] % nDivisor; break;
        case IID_STC_REMM_POS_3: nValue = m_lpNumsData[2] % nDivisor; break;
        case IID_STC_REMM_POS_4: nValue = m_lpNumsData[3] % nDivisor; break;
        case IID_STC_REMM_POS_5: nValue = m_lpNumsData[4] % nDivisor; break;
        case IID_STC_REMM_POS_6: nValue = m_lpNumsData[5] % nDivisor; break;
        case IID_STC_REMM_POS_7: nValue = m_lpNumsData[6] % nDivisor; break;
        case IID_STC_REMM_POS_8: nValue = m_lpNumsData[7] % nDivisor; break;
        case IID_STC_REMM_POS_ANY:
		{
			int nValues[8] = {0};
			for(int i=0; i<m_btNumberCount; i++)
			{
				nValues[i] = m_lpNumsData[i] % nDivisor;
			}
			return _FillMulValues(nValues, m_btNumberCount);
		} break;
        case IID_STC_REMM_SUM:
		{
			for(int i=0; i<m_btNumberCount; i++)
				nValue += m_lpNumsData[i] % nDivisor;
		} break;
        case IID_STC_REMM_SUMTAIL:
		{
			for(int i=0; i<m_btNumberCount; i++)
				nValue += m_lpNumsData[i] % nDivisor;
			nValue %= 10;
		} break;
        case IID_STC_REMM_ARR_SIN:
		{
			int nRatio = 1;
			for(int i=m_btNumberCount-1; i>=0; i--)
			{
				nValue += (m_lpNumsData[i] % nDivisor) * nRatio;
				nRatio *= 10;
			}
		} break;
        case IID_STC_REMM_ARR_SIN_VALUE:
		{
			int nRatio = 1;
			for(int i=m_btNumberCount-1; i>=0; i--)
			{
				nValue += (m_lpNumsData[i] % nDivisor) * nRatio;
				nRatio *= nDivisor;
			}
		} break;
        case IID_STC_REMM_ARR_GRO:
		{
			int nRatio = 1;
			BYTE btTemp[MAX_NUMBER_COUNT] = {0};
			for(int i=m_btNumberCount-1; i>=0; i--)
			{
				btTemp[i] = (BYTE)(m_lpNumsData[i] % nDivisor);
			}
			g_pICstPubFunc->QuickSort(btTemp, m_btNumberCount);
			for(int i=m_btNumberCount-1; i>=0; i--)
			{
				nValue += btTemp[i] * nRatio;
				nRatio *= 10;
			}
		} break;
        case IID_STC_REMM_ARR_COUNT:
            bGetArrangeValue = FALSE;
        case IID_STC_REMM_ARR_COUNT_VALUE:
        case IID_STC_REMM_ARR_GRO_VALUE:
		{
			int nValues[32] = {0}, nRatio = 1;
			for(int i=0; i<m_btNumberCount; i++)
			{
				nValues[m_lpNumsData[i] % nDivisor] ++;
			}
			for(int i=nDivisor-1; i>=0; i--)
			{
				nValue += nValues[i] * nRatio;
				nRatio *= 10;
			}
			if(bGetArrangeValue)
			{
				nValue = _GetIndexOfCountArrange(nValue, (BYTE)nDivisor, TRUE);
			}
		} break;
        case IID_STC_REMM_MAXLINK:
		{
			BYTE btTemp[32] = {0};
			for(int i=0; i<m_btNumberCount; i++)
			{
				btTemp[m_lpNumsData[i] % nDivisor] = 1;
			}
			nValue = _CalcMaxLinkSameValueCount(btTemp, nDivisor, 1);
		}break;	
        case IID_STC_REMM_MAXNONE:
		{
			BYTE btTemp[32] = {0};
			for(int i=0; i<m_btNumberCount; i++)
			{
				btTemp[m_lpNumsData[i] % nDivisor] = 1;
			}
			nValue = _CalcMaxLinkSameValueCount(btTemp, nDivisor, 0);
		}break;	
        case IID_STC_REMM_COUNT_NONE:
		{
			BYTE btTemp[32] = {0};
			for(int i=0; i<m_btNumberCount; i++)
			{
				btTemp[m_lpNumsData[i] % nDivisor] = 1;
			}
			for(int i=0; i<nDivisor; i++)
			{
				if(btTemp[i] == 0)
					nValue ++;
			}
		} break;
        case IID_STC_REMM_COUNT_MATCH:
		{
			BYTE btTemp[32] = {0};
			for(int i=0; i<m_btNumberCount; i++)
			{
				btTemp[m_lpNumsData[i] % nDivisor] = 1;
			}
			for(int i=0; i<nDivisor; i++)
			{
				if(btTemp[i] == 1)
					nValue ++;
			}
		} break;
        case IID_STC_REMM_SHAPE: nValue = _Get012Shape(); break;
        default: // IID_STC_REMM_COUNT_N
		{
			int nRem = dwIID - nDivisorIndex * 100 - IID_STC_REMM_COUNT_N;
			for(int i=0; i<m_btNumberCount; i++)
			{
				if(m_lpNumsData[i] % nDivisor == nRem)
					nValue ++;
			}
		} break;
	}
    
	return _FillSingleValues(nValue);
}

LPCDTVALUE CIndicator::_GetAreaValue(WORD wUseType, DWORD dwCustomData)
{
	LPDATAAREA lpDataArea = (LPDATAAREA)dwCustomData;
	if(lpDataArea == NULL)
	{
		ASSERT(FALSE);
		return _FillSingleValues(CDTVALUE_INVALID);
	}
    
	int nValue = 0;
    
	BOOL bGetArrangeValue = TRUE;
    
	switch(wUseType)
	{
        case CIDAUT_POS_1: nValue = lpDataArea->btSubAreaValues[m_lpNumsData[0]]; break;
        case CIDAUT_POS_2: nValue = lpDataArea->btSubAreaValues[m_lpNumsData[1]]; break;
        case CIDAUT_POS_3: nValue = lpDataArea->btSubAreaValues[m_lpNumsData[2]]; break;
        case CIDAUT_POS_4: nValue = lpDataArea->btSubAreaValues[m_lpNumsData[3]]; break;
        case CIDAUT_POS_5: nValue = lpDataArea->btSubAreaValues[m_lpNumsData[4]]; break;
        case CIDAUT_POS_6: nValue = lpDataArea->btSubAreaValues[m_lpNumsData[5]]; break;
        case CIDAUT_POS_7: nValue = lpDataArea->btSubAreaValues[m_lpNumsData[6]]; break;
        case CIDAUT_POS_8: nValue = lpDataArea->btSubAreaValues[m_lpNumsData[7]]; break;
        case CIDAUT_POS_ANY:
		{
			int nValues[8] = {0};
			for(int i=0; i<m_btNumberCount; i++)
			{
				nValues[i] = lpDataArea->btSubAreaValues[m_lpNumsData[i]];
			}
			return _FillMulValues(nValues, m_btNumberCount);
		} break;
        case CIDAUT_ARR_SIN:
		{
			int nRatio = 1;
			for(int i=m_btNumberCount-1; i>=0; i--)
			{
				nValue += lpDataArea->btSubAreaValues[m_lpNumsData[i]] * nRatio;
				nRatio *= 10;
			}
		} break;
        case CIDAUT_ARR_SIN_VALUE:
		{
			int nRatio = 1;
			for(int i=m_btNumberCount-1; i>=0; i--)
			{
				nValue += (lpDataArea->btSubAreaValues[m_lpNumsData[i]] - lpDataArea->btSubAreaMinValue) * nRatio;
				nRatio *= lpDataArea->btAreaCount;
			}
			if(m_btNumberCount == 3 && lpDataArea->btAreaCount == 2)	// 修改了自定义二分区排列值中的3和4的顺序，以便和奇偶等条件统一。
			{
				if(nValue == 3)
					nValue = 4;
				else if(nValue == 4)
					nValue = 3;
			}
		} break;
        case CIDAUT_ARR_GRO:
		{
			int nRatio = 1;
			BYTE btTemp[MAX_NUMBER_COUNT] = {0};
			for(int i=m_btNumberCount-1; i>=0; i--)
			{
				btTemp[i] = lpDataArea->btSubAreaValues[m_lpNumsData[i]];
				if(bGetArrangeValue)
					btTemp[i] = (BYTE)(btTemp[i]);
			}
			g_pICstPubFunc->QuickSort(btTemp, m_btNumberCount);
			for(int i=m_btNumberCount-1; i>=0; i--)
			{
				nValue += btTemp[i] * nRatio;
				nRatio *= 10;
			}
		} break;
        case CIDAUT_ARR_COUNT:
            bGetArrangeValue = FALSE;
        case CIDAUT_ARR_COUNT_VALUE:
        case CIDAUT_ARR_GRO_VALUE:
		{
			int nValues[32] = {0}, nRatio = 1;
			for(int i=0; i<m_btNumberCount; i++)
			{
				nValues[lpDataArea->btSubAreaValues[m_lpNumsData[i]] - lpDataArea->btSubAreaMinValue] ++;
			}
			for(int i=lpDataArea->btAreaCount-1; i>=0; i--)
			{
				nValue += nValues[i] * nRatio;
				nRatio *= 10;
			}
			if(bGetArrangeValue)
				nValue = _GetIndexOfCountArrange(nValue, (BYTE)lpDataArea->btAreaCount, TRUE);
		} break;
        case CIDAUT_MAXLINK:
		{
			BYTE btTemp[32] = {0};
			for(int i=0; i<m_btNumberCount; i++)
			{
				btTemp[lpDataArea->btSubAreaValues[m_lpNumsData[i]] - lpDataArea->btSubAreaMinValue] = 1;
			}
			nValue = _CalcMaxLinkSameValueCount(btTemp, lpDataArea->btAreaCount, 1);
		}break;	
        case CIDAUT_MAXNONE:
		{
			BYTE btTemp[32] = {0};
			for(int i=0; i<m_btNumberCount; i++)
			{
				btTemp[lpDataArea->btSubAreaValues[m_lpNumsData[i]] - lpDataArea->btSubAreaMinValue] = 1;
			}
			nValue = _CalcMaxLinkSameValueCount(btTemp, lpDataArea->btAreaCount, 0);
		}break;	
        case CIDAUT_COUNT_NONE:
		{
			BYTE btTemp[32] = {0};
			for(int i=0; i<m_btNumberCount; i++)
			{
				btTemp[lpDataArea->btSubAreaValues[m_lpNumsData[i]] - lpDataArea->btSubAreaMinValue] = 1;
			}
			for(int i=0; i<lpDataArea->btAreaCount; i++)
			{
				if(btTemp[i] == 0)
					nValue ++;
			}
		} break;
        case CIDAUT_COUNT_MATCH:
		{
			BYTE btTemp[32] = {0};
			for(int i=0; i<m_btNumberCount; i++)
			{
				btTemp[lpDataArea->btSubAreaValues[m_lpNumsData[i]] - lpDataArea->btSubAreaMinValue] = 1;
			}
			for(int i=0; i<lpDataArea->btAreaCount; i++)
			{
				if(btTemp[i] == 1)
					nValue ++;
			}
		} break;
        case CIDAUT_SHAPE:
		{
			ASSERT(m_btNumberCount == 3);
			
			int nNum[3] = {0};
			for(int i=0; i<3; i++)
			{
				nNum[lpDataArea->btSubAreaValues[m_lpNumsData[i]] - lpDataArea->btSubAreaMinValue] ++;
			}
			
			if(nNum[0] == 1 && nNum[1] == 1)
			{
				nValue = 2;
			}
			else if(nNum[0] == 3 || nNum[1] == 3 || nNum[2] == 3)
			{
				nValue = 0;
			}
			else
			{
				nValue = 1;
			}
		} break;
        default: // CIDAUT_COUNT_N
		{
			BYTE btSubAreaValue = (BYTE)(wUseType - CIDAUT_COUNT_N + lpDataArea->btSubAreaMinValue);
			for(int i=0; i<m_btNumberCount; i++)
			{
				if(lpDataArea->btSubAreaValues[m_lpNumsData[i]] == btSubAreaValue)
					nValue ++;
			}
		} break;
	}
	
	return _FillSingleValues(nValue);
}

int CIndicator::_CalcTwoPos(int nPos1, int nPos2, int nType)
{
	ASSERT(nPos1 >= 0 && nPos1 < m_btNumberCount);
	ASSERT(nPos2 >= 0 && nPos1 < m_btNumberCount);
	ASSERT(nPos1 != nPos2);
    
	switch(nType)
	{
        case TPT_SUM: return m_lpNumsData[nPos1] + m_lpNumsData[nPos2]; break;
        case TPT_SUM_TAIL: return (m_lpNumsData[nPos1] + m_lpNumsData[nPos2]) % 10; break;
        case TPT_SUB_ASC: return m_lpNumsData[nPos1] - m_lpNumsData[nPos2]; break;
        case TPT_SUB_DESC:
        case TPT_SUB: return m_lpNumsData[nPos2] - m_lpNumsData[nPos1]; break;
        case TPT_SUB_TAIL: return abs(m_lpNumsData[nPos1] - m_lpNumsData[nPos2]) % 10; break;
        case TPT_SUBABS: return abs(m_lpNumsData[nPos1] - m_lpNumsData[nPos2]); break;
        case TPT_SUBABS_TAIL: return abs(m_lpNumsData[nPos1] - m_lpNumsData[nPos2]) % 10; break;
        case TPT_CIRSUB_ASC:
        case TPT_CIRSUB:
		{
			int nValue = m_lpNumsData[nPos1] - m_lpNumsData[nPos2];
			if(nValue < 0)
				nValue += (m_btMaxNumber - m_btMinNumber + 1);
			return nValue;
		}
        case TPT_CIRSUB_DESC:
		{
			int nValue = m_lpNumsData[nPos2] - m_lpNumsData[nPos1];
			if(nValue < 0)
				nValue += (m_btMaxNumber - m_btMinNumber + 1);
			return nValue;
		} break;
        case TPT_CIRSUB_TAIL:
        case TPT_CIRSUB_ASC_TAIL:
		{
			int nValue = m_lpNumsData[nPos1] - m_lpNumsData[nPos2];
			if(nValue < 0)
				nValue += (m_btMaxNumber - m_btMinNumber + 1);
			nValue %= 10;
			return nValue;
		} break;
        case TPT_CIRSUB_DESC_TAIL:
		{
			int nValue = m_lpNumsData[nPos2] - m_lpNumsData[nPos1];
			if(nValue < 0)
				nValue += (m_btMaxNumber - m_btMinNumber + 1);
			nValue %= 10;
			return nValue;
		} break;
// TEST        case TPT_MUL: return theApp.m_btProductNums[m_lpNumsData[nPos1]] * theApp.m_btProductNums[m_lpNumsData[nPos2]]; break;
// TEST        case TPT_MUL_TAIL: return (theApp.m_btProductNums[m_lpNumsData[nPos1]] * theApp.m_btProductNums[m_lpNumsData[nPos2]]) % 10; break;
        default: ASSERT(FALSE); break;
	}
    
	ASSERT(FALSE);
	return CDTVALUE_INVALID;
}

int CIndicator::_CalcAllTwoPos(int nType, BOOL bAutoSort, LPINT lpValuesOut)
{
	int i, j, nValueCount = 0;
	for(i=0; i<m_btNumberCount-1; i++)
	{
		for(j=i+1; j<m_btNumberCount; j++)
		{
			lpValuesOut[nValueCount++] = _CalcTwoPos(i, j, nType);
		}
	}
    
	ASSERT(nValueCount < MAXBYTE);
    
	if(bAutoSort)
	{
		g_pICstPubFunc->QuickSort(lpValuesOut, nValueCount, QSVT_INT);
	}
    
	return nValueCount;
}

int CIndicator::_CalcMaxLinkSameValueCount(LPBYTE lpValues, int nValueCount, BYTE btSameValue)
{
	ASSERT(lpValues != NULL);
    
	int nTemp = 0, nRtnValue = 0;
    
	for(int i=0; i<nValueCount; i++)
	{
		if(lpValues[i] == btSameValue)
		{
			nTemp ++;
		}
		else
		{
			if(nTemp > nRtnValue)
			{
				nRtnValue = nTemp;
			}
			nTemp = 0;
		}
	}
    
	if(nTemp > nRtnValue)	// 处理最后一个值
		nRtnValue = nTemp;
	
	return nRtnValue;
}

int CIndicator::_GetRelativedDZX(BYTE btPosValue, LPBYTE lpOrderNumsData)
{
	ASSERT(lpOrderNumsData != NULL);
    
	if(lpOrderNumsData[0] == lpOrderNumsData[2])
	{
		return 1;	// 豹子则为：中中中
	}
	else if(lpOrderNumsData[0] == lpOrderNumsData[1]) // AAB
	{
		return (btPosValue == lpOrderNumsData[0]) ? 0 : 2;
	}
	else if(lpOrderNumsData[2] == lpOrderNumsData[1]) // ABB
	{
		return (btPosValue == lpOrderNumsData[0]) ? 0 : 2;
	}
	else
	{
		for(int i=0; i<3; i++)
		{
			if(btPosValue == lpOrderNumsData[i])
			{
				return i;
			}
		}
	}
    
	ASSERT(FALSE);
	return CDTVALUE_INVALID;
}

int CIndicator::_GetMaxSameNumberCount()
{
	if(m_btDataType & DATA_TYPE_REPEATABLE)
	{
		int nOrgMaxNumber = g_pIData->GetMaxNumber(DATA_SOURCE_ORG);
		if((m_btDataType & DATA_TYPE_NUM_SZ) && (nOrgMaxNumber > 9))
		{
			int nOrgMinNumber = g_pIData->GetMinNumber(DATA_SOURCE_ORG);
			return (nOrgMaxNumber - nOrgMinNumber) / 10 + 1;
		}
		return m_btNumberCount;
	}
	else		// 不可重复
	{
		return 1;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////
#define INIT_INDEXOF_COUNTARRANGE(btNumCountIn) \
            if(btItemCount == btNumCountIn) { \
                n[btNumCountIn-1] = m_btNumberCount; \
                for(i=0; i<btNumCountIn-1; i++) n[btNumCountIn-1] -= n[i]; \
                    nIndex ++; \
                if(nIndex >= 255) { \
                    pIndexValues[0] = MAXDWORD; \
                    return; \
                } \
                pIndexValues[nIndex] = 0; \
                nRatio = 1; \
                for(i=btNumCountIn-1; i>=0; i--) { \
                    pIndexValues[nIndex] += n[i] * nRatio; \
                    nRatio *= 10; \
                } \
                continue; \
            }

void CIndicator::_InitIndexOfCountArrange(BYTE btItemCount)
{
	if(m_btNumberCount < 2)
	{
		return ;
	}
    
	ASSERT(btItemCount > 1 && btItemCount <= 9);
	int i, nIndex = 0, nRatio = 1, n[9] = {0};
	LPDWORD pIndexValues = &m_dwCountArrangeValueIndexs[btItemCount - 2][0];
    
	for(n[0]=m_btNumberCount; n[0]>=0; n[0]--)
	{
		INIT_INDEXOF_COUNTARRANGE(2);
		for(n[1]=m_btNumberCount-n[0]; n[1]>=0; n[1]--)
		{
			INIT_INDEXOF_COUNTARRANGE(3);
			for(n[2]=m_btNumberCount-n[0]-n[1]; n[2]>=0; n[2]--)
			{
				INIT_INDEXOF_COUNTARRANGE(4);
				for(n[3]=m_btNumberCount-n[0]-n[1]-n[2]; n[3]>=0; n[3]--)
				{
					INIT_INDEXOF_COUNTARRANGE(5);
					for(n[4]=m_btNumberCount-n[0]-n[1]-n[2]-n[3]; n[4]>=0; n[4]--)
					{
						INIT_INDEXOF_COUNTARRANGE(6);
						for(n[5]=m_btNumberCount-n[0]-n[1]-n[2]-n[3]-n[4]; n[5]>=0; n[5]--)
						{
							INIT_INDEXOF_COUNTARRANGE(7);
							for(n[6]=m_btNumberCount-n[0]-n[1]-n[2]-n[3]-n[4]-n[5]; n[6]>=0; n[6]--)
							{
								INIT_INDEXOF_COUNTARRANGE(8);
								for(n[7]=m_btNumberCount-n[0]-n[1]-n[2]-n[3]-n[4]-n[5]-n[6]; n[7]>=0; n[7]--)
								{
									INIT_INDEXOF_COUNTARRANGE(9);
								}
							}
						}
					}
				}
			}
		}
	}
    
	pIndexValues[0] = nIndex;
}

int CIndicator::_GetIndexOfCountArrange(int nValue, BYTE btItemCount, BOOL bAscOrder)
{
	ASSERT(btItemCount > 1 && btItemCount <= 9);
	LPDWORD pIndexValues = &m_dwCountArrangeValueIndexs[btItemCount - 2][0];
	ASSERT(pIndexValues[0] != MAXDWORD);
    
	int nIndex = g_pICstPubFunc->QuckFind_Half((LPINT)&pIndexValues[1], pIndexValues[0], nValue, FALSE);
    
	if(bAscOrder)
	{
		return nIndex;
	}
	else
	{
		return pIndexValues[0] - nIndex - 1;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////

int CIndicator::_GetXLValue(BYTE btType, DWORD dwIssueIndex)
{
	// 获取前一期数据索引
	DWORD dwPrevIssueIndex = g_pIData->GetPrevDataIndex(dwIssueIndex);
	if(dwPrevIssueIndex == DATA_INDEX_INVALID)
	{
		return CDTVALUE_INVALID;	// 无效，则返回错误
	}
    
	LPBYTE lpLastNums = g_pIData->GetItemNums(dwPrevIssueIndex);
    
	// 设置标志
	BYTE btTemp[128] = {0}, btCount = BYTE(m_btMaxNumber - m_btMinNumber + 1);
	for(int i=0; i<m_btNumberCount; i++)
	{
		btTemp[lpLastNums[i] - m_btMinNumber] = 1;
	}
    
	// 计算斜连值
	int nLeft = 0, nRight = 0;
	for(int i=0; i<m_btNumberCount; i++)
	{
		if(btType != 1)		// 需要计算左斜连值
		{
			if(btTemp[(m_lpNumsData[i] + 1 - m_btMinNumber) % btCount] == 1)
			{
				nLeft ++;
			}
		}
		if(btType != 0)		// 需要计算右斜连值
		{
			if(btTemp[(m_lpNumsData[i] - m_btMinNumber + btCount - 1) % btCount] == 1)
			{
				nRight ++;
			}
		}
	}
    
	switch(btType)
	{
        case 0:	return nLeft; break; // 左斜连值
        case 1:	return nRight; break; // 右斜连值
        case 2:	return nLeft + nRight; break; // 斜连值
        case 3:	return nLeft * 10 + nRight; break; // 斜连值组合
	}
    
	ASSERT(FALSE);
	return CDTVALUE_INVALID;
}

LPCDTVALUE CIndicator::_GetAnySkipTail(DWORD dwIssueIndex, BOOL bSingle)
{
	int nSkipValues[8] = {0};
    
	for(int i=0; i<m_btNumberCount; i++)
	{
		nSkipValues[i] = g_pIData->GetSkipValue(GSKF_POS_1 + i, dwIssueIndex, m_lpNumsData, bSingle) % 10;
	}
	return _FillMulValues(nSkipValues, m_btNumberCount);
}

int CIndicator::_Get3StateValue(DWORD dwIID, DWORD dwIssueIndex)
{
	BYTE btTypeIndex = (BYTE)(((dwIID - IID_DNC_XXX_BEGIN) / 100) % 10);
	switch(dwIID - btTypeIndex * 100)
	{
        case IID_DNC_XXX_POS_1: return g_pIData->GetThreeStateData(btTypeIndex, GTSDF_ONE, dwIssueIndex, m_lpNumsData);
        case IID_DNC_XXX_POS_2: return g_pIData->GetThreeStateData(btTypeIndex, GTSDF_TWO, dwIssueIndex, m_lpNumsData);
        case IID_DNC_XXX_POS_3: return g_pIData->GetThreeStateData(btTypeIndex, GTSDF_THREE, dwIssueIndex, m_lpNumsData);
        case IID_DNC_XXX_POS_4: return g_pIData->GetThreeStateData(btTypeIndex, GTSDF_FOUR, dwIssueIndex, m_lpNumsData);
        case IID_DNC_XXX_POS_5: return g_pIData->GetThreeStateData(btTypeIndex, GTSDF_FIVE, dwIssueIndex, m_lpNumsData);
        case IID_DNC_XXX_POS_6: return g_pIData->GetThreeStateData(btTypeIndex, GTSDF_SIX, dwIssueIndex, m_lpNumsData);
        case IID_DNC_XXX_POS_7: return g_pIData->GetThreeStateData(btTypeIndex, GTSDF_SEVEN, dwIssueIndex, m_lpNumsData);
        case IID_DNC_XXX_POS_8: return g_pIData->GetThreeStateData(btTypeIndex, GTSDF_EIGHT, dwIssueIndex, m_lpNumsData);
        case IID_DNC_XXX_COUNT_0: return g_pIData->GetThreeStateData(btTypeIndex, GTSDF_COUNT_0, dwIssueIndex, m_lpNumsData);
        case IID_DNC_XXX_COUNT_1: return g_pIData->GetThreeStateData(btTypeIndex, GTSDF_COUNT_1, dwIssueIndex, m_lpNumsData);
        case IID_DNC_XXX_COUNT_2: return g_pIData->GetThreeStateData(btTypeIndex, GTSDF_COUNT_2, dwIssueIndex, m_lpNumsData);
        case IID_DNC_XXX_SHAPE: return g_pIData->GetThreeStateData(btTypeIndex, GTSDF_SHAPE, dwIssueIndex, m_lpNumsData);
        case IID_DNC_XXX_ARR_SIN: return g_pIData->GetThreeStateData(btTypeIndex, GTSDF_ARR_SIN, dwIssueIndex, m_lpNumsData);
        case IID_DNC_XXX_ARR_SIN_VALUE: return g_pIData->GetThreeStateData(btTypeIndex, GTSDF_ARR_SIN_VALUE, dwIssueIndex, m_lpNumsData);
        case IID_DNC_XXX_ARR_GRO: return g_pIData->GetThreeStateData(btTypeIndex, GTSDF_ARR_GRO, dwIssueIndex, m_lpNumsData);
        case IID_DNC_XXX_ARR_COUNT: return g_pIData->GetThreeStateData(btTypeIndex, GTSDF_ARR_COUNT, dwIssueIndex, m_lpNumsData);
        case IID_DNC_XXX_ARR_COUNT_VALUE: 
        case IID_DNC_XXX_ARR_GRO_VALUE:
		{
			int nValue = g_pIData->GetThreeStateData(btTypeIndex, GTSDF_ARR_COUNT, dwIssueIndex, m_lpNumsData);
			if(nValue != CDTVALUE_INVALID)
				nValue = _GetIndexOfCountArrange(nValue, 3, TRUE);
			return nValue;
		} break;
	}
    
	return 0;
}

int CIndicator::_GetRepeatCountValue(DWORD dwIID, DWORD dwIssueIndex)
{
	int nRepeatCount = -1;
	BYTE btTemp[128] = {0};
    
	if(dwIID >= IID_DNC_REPEAT_N)	// 前第N期重复号
	{
		int nCompareIssueIndex = (dwIssueIndex == DATA_INDEX_INVALID || dwIssueIndex == DATA_INDEX_FORECAST) ? g_pIData->GetItemCount() : dwIssueIndex;
		nCompareIssueIndex -= (dwIID - IID_DNC_REPEAT_N + 1);
		if(nCompareIssueIndex >= 0)
		{
			nRepeatCount = 0;
			LPBYTE lpLastNNums = g_pIData->GetItemNums(nCompareIssueIndex);
			for(int i=0; i<m_btNumberCount; i++)
			{
				btTemp[lpLastNNums[i]] = 1;
			}
			
			for(int i=0; i<m_btNumberCount; i++)
			{
				if(btTemp[m_lpNumsData[i]] == 1)
				{
					nRepeatCount ++;
				}
			}
		}
	}
	else						// 前N期重复号
	{
		int nIssueCount = (dwIID - IID_DNC_REPEAT_1TON + 1);
		int nCompareIssueIndex = (dwIssueIndex == DATA_INDEX_INVALID || dwIssueIndex == DATA_INDEX_FORECAST) ? g_pIData->GetItemCount() : dwIssueIndex;
		nCompareIssueIndex -= nIssueCount;
		if(nCompareIssueIndex >= 0)
		{
			nRepeatCount = 0;
			for(int i=0; i<m_btNumberCount; i++)
			{
				btTemp[m_lpNumsData[i]] = 1;
			}
            
			for(int j=0; j<nIssueCount; j++)
			{
				LPBYTE lpLastNNums = g_pIData->GetItemNums(nCompareIssueIndex + j);				
				for(int i=0; i<m_btNumberCount; i++)
				{
					if(btTemp[lpLastNNums[i]] == 1)
					{
						btTemp[lpLastNNums[i]] = 0;
						nRepeatCount ++;
					}
				}
			}
		}
	}
    
	return (nRepeatCount == -1) ? CDTVALUE_INVALID : nRepeatCount;
}

LPCDTVALUE CIndicator::_GetPrevDataValue(int nPosIndex, DWORD dwIssueIndex, int nTypeIndex)
{
	// 获取前一期数据索引
	DWORD dwPrevIssueIndex = g_pIData->GetPrevDataIndex(dwIssueIndex);
	if(dwPrevIssueIndex == DATA_INDEX_INVALID)
	{
		return _FillSingleValues(CDTVALUE_INVALID);	// 无效，则返回错误
	}
	
	// 获取相关数据
	LPBYTE lpLastNums = g_pIData->GetItemNums(dwPrevIssueIndex);
	
	switch(nPosIndex)
	{
        case -1:	// 邻期关系和
        case -2:	// 邻期关系和
		{
			int nSumValue = 0;
			for(int i=0; i<m_btNumberCount; i++)
			{
				nSumValue += _GetPrevDataValue_Simple(lpLastNums[i], m_lpNumsData[i], nTypeIndex);
			}
			
			if(nPosIndex == -2)
			{
				if(nSumValue < 0)
					nSumValue = 0 - nSumValue;
				nSumValue %= 10;
			}
			return _FillSingleValues(nSumValue);
		} break;
            
        case -3:	// 任意邻期关系
		{
			int nValues[8] = {0};
			for(int i=0; i<m_btNumberCount; i++)
			{
				nValues[i] = _GetPrevDataValue_Simple(lpLastNums[i], m_lpNumsData[i], nTypeIndex);
			}
			return _FillMulValues(nValues, m_btNumberCount);
		} break;
            
        case -4:	// 邻期关系单排
		{
			int nValue = 0, nRatio = 1;
			for(int i=m_btNumberCount-1; i>=0; i--)
			{
				nValue += _GetPrevDataValue_Simple(lpLastNums[i], m_lpNumsData[i], nTypeIndex) * nRatio;
				nRatio *= 10;
			}
			
			return _FillSingleValues(nValue);
		} break;
            
        case -5:	// 邻期关系组排
		{
			BYTE btValues[8] = {0};
			for(int i=0; i<m_btNumberCount; i++)
			{
				btValues[i] = (BYTE)_GetPrevDataValue_Simple(lpLastNums[i], m_lpNumsData[i], nTypeIndex);
			}
			g_pICstPubFunc->QuickSort(btValues, m_btNumberCount);
			
			int nValue = 0, nRatio = 1;
			for(int i=m_btNumberCount-1; i>=0; i--)
			{
				nValue += (btValues[i]) * nRatio;
				nRatio *= 10;
			}
			
			return _FillSingleValues(nValue);
		} break;
            
        case -6:	// 最小邻期关系
		{
			int nMinValue = _GetPrevDataValue_Simple(lpLastNums[0], m_lpNumsData[0], nTypeIndex), nValue = 0;
			for(int i=1; i<m_btNumberCount; i++)
			{
				nValue = _GetPrevDataValue_Simple(lpLastNums[i], m_lpNumsData[i], nTypeIndex);
				if(nValue < nMinValue)
					nMinValue = nValue;
			}
			return _FillSingleValues(nMinValue);
		} break;
            
        case -7:	// 中间邻期关系
		{
			ASSERT(m_btNumberCount == 3);
			int nValues[8] = {0};
			for(int i=0; i<m_btNumberCount; i++)
			{
				nValues[i] = _GetPrevDataValue_Simple(lpLastNums[i], m_lpNumsData[i], nTypeIndex);
			}
			g_pICstPubFunc->QuickSort(nValues, m_btNumberCount, QSVT_INT);
			return _FillSingleValues(nValues[1]);
		} break;
            
        case -8:	// 最大邻期关系
		{
			int nMaxValue = _GetPrevDataValue_Simple(lpLastNums[0], m_lpNumsData[0], nTypeIndex), nValue = 0;
			for(int i=1; i<m_btNumberCount; i++)
			{
				nValue = _GetPrevDataValue_Simple(lpLastNums[i], m_lpNumsData[i], nTypeIndex);
				if(nValue > nMaxValue)
					nMaxValue = nValue;
			}
			return _FillSingleValues(nMaxValue);
		} break;
            
        default:	// 各位邻期关系
		{
			ASSERT(nPosIndex >= 0 && nPosIndex < m_btNumberCount);
			
			int nValue = _GetPrevDataValue_Simple(lpLastNums[nPosIndex], m_lpNumsData[nPosIndex], nTypeIndex);
			
			return _FillSingleValues(nValue);
		} break;
	}
}

int CIndicator::_GetPrevDataValue_Simple(BYTE btPrevData, BYTE btCurData, int nTypeIndex)
{
	int nValue = CDTVALUE_INVALID;
    
	switch(nTypeIndex)
	{
        case PDT_DIS: nValue = abs(btCurData - btPrevData); break;
        case PDT_DISTAIL: nValue = abs(btCurData - btPrevData) % 10; break;
        case PDT_SUB: nValue = btCurData - btPrevData; break;
        case PDT_SUM: nValue = btCurData + btPrevData; break;
        case PDT_SUMTAIL: nValue = (btCurData + btPrevData) % 10; break;
        case PDT_CIRDIS:
        case PDT_CIRDISTAIL:
		{
			nValue = btCurData - btPrevData;
			if(nValue < 0)
			{
				nValue += (m_btMaxNumber - m_btMinNumber + 1);
			}
			if(nTypeIndex == PDT_CIRDISTAIL)
			{
				nValue %= 10;
			}
		} break;
        case PDT_AXISSUB:
        case PDT_AXISDIS:
        case PDT_AXISDISTAIL:
		{
			nValue = btCurData - btPrevData;
			if(nValue == 0)
			{
				return 0;
			}
			int nValue2 = 0, nValueAbs = 0, nValue2Abs = 0;
			if(nValue > 0)
			{
				nValueAbs = nValue;
				nValue2 = nValue - (m_btMaxNumber - m_btMinNumber + 1);
				nValue2Abs = 0 - nValue2;
			}
			else
			{
				nValueAbs = 0 - nValue;
				nValue2 = nValue + (m_btMaxNumber - m_btMinNumber + 1);
				nValue2Abs = nValue2;
			}
            
			switch(nTypeIndex)
			{
                case PDT_AXISSUB: return (nValueAbs <= nValue2Abs) ? nValue : nValue2; break;
                case PDT_AXISDIS: return (nValueAbs <= nValue2Abs) ? nValueAbs : nValue2Abs; break;
                case PDT_AXISDISTAIL: return ((nValueAbs <= nValue2Abs) ? nValueAbs : nValue2Abs) % 10; break;
			}
		}
        default: ASSERT(FALSE); break;
	}
    
	return nValue;
}

