//
//  Indicator_GetValueRange.cpp
//  QCLottery
//
//  Created by 武 孙 on 12-5-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "Indicator.h"
#include <math.h>
#include "ConditionInfo.h"
#include "ICstPubFunc.h"

///////////////////////////////////////////////////////////////////////////////////////
BOOL CIndicator::GetValueRange(const CDTIID &cdtIID, DWORD dwCustomData, LPCDTVALUERANGE *lplpValueRange)
{
	ASSERT((*lplpValueRange) == NULL);
    
	m_lpValueRangeTemp->nItemCount = -1;	// 初始化
    
	if(cdtIID.btType & CDTIID_TYPE_DATAAREA)		// 分区指标
	{
		_FillValueRange_DataArea(cdtIID.stCustomIID.wUseType, dwCustomData);
	}
	else if(cdtIID.btType & CDTIID_TYPE_CUSTOM)	// 自定义指标
	{
		return TRUE;		// TEST
	}
	else	// 系统指标
	{
		switch(cdtIID.dwSystemIID)
		{
            case 0: _AutoFillValueRange_DefaultType(0, 0, 0); break;
                
            case IID_STC_NUMBER_MAXCOUNT: _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 1, _GetMaxSameNumberCount()); break;
            case IID_STC_NUMBER_SAMECOMBO: _FillValueRange_SameCombo(); break;
            case IID_STC_NUMBER_SAMEGROUP_2: _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 0, m_btNumberCount / 2); break;
            case IID_STC_NUMBER_SAMEGROUP_3: _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 0, m_btNumberCount / 3); break;
            case IID_STC_NUMBER_SAMEGROUP_4: _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 0, m_btNumberCount / 4); break;
            case IID_STC_NUMBER_SAMEGROUP_5: _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 0, m_btNumberCount / 5); break;
            case IID_STC_NUMBER_SAMEGROUP_6: _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 0, m_btNumberCount / 6); break;
            case IID_STC_NUMBER_SAMEGROUP_7: _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 0, m_btNumberCount / 7); break;
            case IID_STC_NUMBER_SAMEGROUP_8: _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 0, m_btNumberCount / 8); break;
                
            case IID_STC_GENRE_NORMAL: _FillValueRange_Genre(); break;
            case IID_STC_GENRE_TRANS:  _FillValueRange_GenreTrans(); break;
            case IID_STC_GENRE_TWOLINK: 
                m_lpValueRangeTemp->nItemCount = 2;
                _SetValueRangeItemValue(0, 0, NULL, NULL, "(0)非二连码");
                _SetValueRangeItemValue(1, 1, NULL, NULL, "(1)二连码");
                break;
                
            case IID_STC_SHAPE_NORMAL: _FillValueRange_Shape(); break;
            case IID_STC_SHAPE_EX: _FillValueRange_ShapeEx(); break;
            case IID_STC_SHAPE_EQUALDIFF:
                m_lpValueRangeTemp->nItemCount = 6;
                _SetValueRangeItemValue(0, -1, "N", NULL, "(N)非等差");
                _SetValueRangeItemValue(1, 0, "0", NULL, "(0)等差0");
                _SetValueRangeItemValue(2, 1, "1", NULL, "(1)等差1");
                _SetValueRangeItemValue(3, 2, "2", NULL, "(2)等差2");
                _SetValueRangeItemValue(4, 3, "3", NULL, "(3)等差3");
                _SetValueRangeItemValue(5, 4, "4", NULL, "(4)等差4");
                break;
            case IID_STC_SHAPE_AAB:
            case IID_STC_SHAPE_ABC:
            case IID_STC_SHAPE_ABD: _FillValueRange_Shape_XXX(cdtIID.dwSystemIID); break;
            case IID_STC_SHAPE_COUNT_SAMENUM: _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 1, m_btNumberCount); _SetValueRangeItemValue(0, 0); break;
            case IID_STC_SHAPE_COUNT_DIFFNUM: _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 1, m_btNumberCount); break;
            case IID_STC_SHAPE_COUNT_SAMEGRO: _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 0, m_btNumberCount / 2); break;
                
            case IID_STC_LINK_COMBO: _FillValueRange_LinkCombo(cdtIID.dwSystemIID); break;
            case IID_STC_LINK_NUMCOUNT:
            case IID_STC_LINK_NUMCOUNT_AS:
            case IID_STC_LINK_NUMCOUNT_DS:
            case IID_STC_LINK_COUNT_ODD:
            case IID_STC_LINK_COUNT_EVEN:
            case IID_STC_LINK_COUNT_PRIME:
            case IID_STC_LINK_COUNT_COMPOSITE: _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 1, m_btNumberCount); _SetValueRangeItemValue(0, 0); break;
            case IID_STC_LINK_GROCOUNT:
            case IID_STC_LINK_GROCOUNT_AS:
            case IID_STC_LINK_GROCOUNT_DS: _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 0, m_btNumberCount / 2); break;
            case IID_STC_LINK_POS_2:
            case IID_STC_LINK_POS_3:
            case IID_STC_LINK_POS_4:
            case IID_STC_LINK_POS_5:
            case IID_STC_LINK_POS_6:
            case IID_STC_LINK_POS_7: _FillValueRange_LinkPos(cdtIID.dwSystemIID); break;
                
            case IID_STC_DHK_HD:
            case IID_STC_DHK_KD:
            case IID_STC_DHK_HK:  
            case IID_STC_DHK_HDKD:
            case IID_STC_DHK_HDKDHK: _FillValueRange_DHK(cdtIID.dwSystemIID); break;
                
            case IID_STC_NUMS_SIN:
            case IID_STC_NUMS_GRO: _FillValueRange_Nums(cdtIID.dwSystemIID); break;
                
            case IID_STC_POS_1:
            case IID_STC_POS_2:
            case IID_STC_POS_3:
            case IID_STC_POS_4:
            case IID_STC_POS_5:
            case IID_STC_POS_6:
            case IID_STC_POS_7:
            case IID_STC_POS_8: 
            case IID_STC_POS_ANY: _FillValueRange_PosX(cdtIID.dwSystemIID, BYTE(cdtIID.dwSystemIID - IID_STC_POS_1)); break;
                
            case IID_STC_SUM: _FillValueRange_Sum(); break;
            case IID_STC_SUM_N:	_AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, g_pIData->GetMinNumber(), 9); break;
            case IID_STC_SPAN: 
                if(m_btNumberCount == 1)
                    _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 0, 9);
                else
                    _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, (m_btDataType & DATA_TYPE_REPEATABLE) ? 0 : (m_btNumberCount - 1), m_btMaxNumber - m_btMinNumber);
                break;
            case IID_STC_SUMTAIL: 
            case IID_STC_SPANTAIL: _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 0, 9); break;
            case IID_STC_AVG: _FillValueRange_Avg(); break;
            case IID_STC_AVGTAIL: _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 0, 9); break;
            case IID_STC_AC:
			{
				int nMin = 0, nMax = _GetACValueRange(nMin);
				_AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, nMin, nMax);
			} break;
                
            case IID_STC_DETRACT: 
                if(m_btDataType & DATA_TYPE_REPEATABLE)
                    _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, (m_btMaxNumber - m_btMinNumber + 1) / (m_btNumberCount + 1), m_btMaxNumber - m_btMinNumber);
                else
                    _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, (m_btMaxNumber - m_btMinNumber + 1) / (2 * m_btNumberCount + 1), (m_btMaxNumber - m_btMinNumber + 1) - m_btNumberCount);
                break;
                
            case IID_STC_ODDENEN_COUNT_O:
            case IID_STC_ODDENEN_COUNT_E:
            case IID_STC_BIGSMALL_COUNT_B:
            case IID_STC_BIGSMALL_COUNT_S:
            case IID_STC_PRIMECOMPOSITE_COUNT_P:
            case IID_STC_PRIMECOMPOSITE_COUNT_C:
            case IID_STC_012_COUNT_0:
            case IID_STC_012_COUNT_1:
            case IID_STC_012_COUNT_2:
            case IID_STC_DZX_COUNT_D:
            case IID_STC_DZX_COUNT_Z:
            case IID_STC_DZX_COUNT_X: _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 0, m_btNumberCount); break;
                
            case IID_STC_ODDENEN_ARR:
			{
				char szNames[2][8] = {"奇", "偶",};
				int nValues[2] = {1, 0};
				_FillValueRange_Arrange(cdtIID.dwSystemIID, szNames, nValues, 2);
			} break;
            case IID_STC_BIGSMALL_ARR:
			{
				char szNames[2][8] = {"大", "小",};
				int nValues[2] = {1, 0};
				_FillValueRange_Arrange(cdtIID.dwSystemIID, szNames, nValues, 2);
			} break;
            case IID_STC_PRIMECOMPOSITE_ARR:
			{
				char szNames[2][8] = {"质", "合",};
				int nValues[2] = {1, 0};
				_FillValueRange_Arrange(cdtIID.dwSystemIID, szNames, nValues, 2);
			} break;
            case IID_STC_012_ARR_SIN:
            case IID_STC_012_ARR_GRO:
			{
				char szNames[3][8] = {"0", "1", "2"};
				int nValues[3] = {0, 1, 2};
				_FillValueRange_Arrange(cdtIID.dwSystemIID, szNames, nValues, 3);
			} break;
            case IID_STC_DZX_ARR_SIN:
            case IID_STC_DZX_ARR_GRO:
			{
				char szNames[3][8] = {"小", "中", "大"};
				int nValues[3] = {0, 1, 2};
				_FillValueRange_Arrange(cdtIID.dwSystemIID, szNames, nValues, 3);
			} break;
                
            case IID_STC_ODDENEN_LINK:
			{
				if(m_btNumberCount > 3)
				{
					m_lpValueRangeTemp->nItemCount = 4;
					_SetValueRangeItemValue(3, 3, "3", NULL, "(3)既有奇连又有偶连");
				}
				else
				{
					m_lpValueRangeTemp->nItemCount = 3;
				}
				_SetValueRangeItemValue(0, 0, "0", NULL, "(0)非奇偶连");
				_SetValueRangeItemValue(1, 1, "1", NULL, "(1)奇连");
				_SetValueRangeItemValue(2, 2, "2", NULL, "(2)偶连");
			} break;
            case IID_STC_PRIMECOMPOSITE_LINK:
			{
				if(m_btNumberCount > 3)
				{
					m_lpValueRangeTemp->nItemCount = 4;
					_SetValueRangeItemValue(3, 3, "3", NULL, "(3)既有质连又有合连");
				}
				else
				{
					m_lpValueRangeTemp->nItemCount = 3;
				}
				_SetValueRangeItemValue(0, 0, "0", NULL, "(0)非质合连");
				_SetValueRangeItemValue(1, 1, "1", NULL, "(1)质连");
				_SetValueRangeItemValue(2, 2, "2", NULL, "(2)合连");
			} break;
                
            case IID_STC_012_SHAPE: 
			{
				char szNames[3][8] = {"0路", "1路", "2路"};
				_FillValueRange_3StateShape(cdtIID.dwSystemIID, szNames);
			} break;
            case IID_STC_DZX_SHAPE: 
			{
				char szNames[3][8] = {"小数", "中数", "大数"};
				_FillValueRange_3StateShape(cdtIID.dwSystemIID, szNames);
			} break;
                
            case IID_STC_012_ARR_COUNT:
            case IID_STC_DZX_ARR_COUNT:
			{
				_FillValueRange_Arrange_Numbers(cdtIID.dwSystemIID, 0, 2);
			} break;
                
            case IID_STC_ODDENEN_ARR_VALUE: 
			{
				char szNames[2][8] = {"偶", "奇"};
				_FillValueRange_SinArrangeValue(cdtIID.dwSystemIID, szNames, 2);
			}break;
            case IID_STC_BIGSMALL_ARR_VALUE: 
			{
				char szNames[2][8] = {"小", "大"};
				_FillValueRange_SinArrangeValue(cdtIID.dwSystemIID, szNames, 2);
			}break;
            case IID_STC_PRIMECOMPOSITE_ARR_VALUE: 
			{
				char szNames[2][8] = {"合", "质"};
				_FillValueRange_SinArrangeValue(cdtIID.dwSystemIID, szNames, 2);
			}break;
            case IID_STC_012_ARR_SIN_VALUE: 
			{
				char szNames[3][8] = {"零", "一", "二"};
				_FillValueRange_SinArrangeValue(cdtIID.dwSystemIID, szNames);
			}break;
            case IID_STC_012_ARR_GRO_VALUE: 
			{
				char szNames[3][8] = {"零", "一", "二"};
				_FillValueRange_GroArrangeValue(cdtIID.dwSystemIID, szNames);
			}break;
            case IID_STC_012_ARR_COUNT_VALUE: 
			{
				char szNames[3][8] = {"零", "一", "二"};
				_FillValueRange_CountArrangeValue(cdtIID.dwSystemIID, szNames, TRUE); 
			}break;
            case IID_STC_DZX_ARR_SIN_VALUE: 
			{
				char szNames[3][8] = {"小", "中", "大"};
				_FillValueRange_SinArrangeValue(cdtIID.dwSystemIID, szNames);
			}break;
            case IID_STC_DZX_ARR_GRO_VALUE: 
			{
				char szNames[3][8] = {"小", "中", "大"};
				_FillValueRange_GroArrangeValue(cdtIID.dwSystemIID, szNames);
			}break;
            case IID_STC_DZX_ARR_COUNT_VALUE: 
			{
				char szNames[3][8] = {"小", "中", "大"};
				_FillValueRange_CountArrangeValue(cdtIID.dwSystemIID, szNames, TRUE); 
			}break;
                
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
            case IID_STC_ESPECIALSUM_X: _FillValueRange_EspecialSum(cdtIID.dwSystemIID); break;
                
            case IID_STC_TWONUM_ANY: _FillValueRange_TwoNums(cdtIID.dwSystemIID); break;
            case IID_STC_TWONUM_Z6:
			{
				m_lpValueRangeTemp->nItemCount = 16;
				_SetValueRangeItemValue(0, DTNZ6_03, "03", NULL, "(03)含03组合的组六号码");
				_SetValueRangeItemValue(1, DTNZ6_06, "06", NULL, "(06)含06组合的组六号码");
				_SetValueRangeItemValue(2, DTNZ6_09, "09", NULL, "(09)含09组合的组六号码");
				_SetValueRangeItemValue(3, DTNZ6_14, "14", NULL, "(14)含14组合的组六号码");
				_SetValueRangeItemValue(4, DTNZ6_17, "17", NULL, "(17)含17组合的组六号码");
				_SetValueRangeItemValue(5, DTNZ6_25, "25", NULL, "(25)含25组合的组六号码");
				_SetValueRangeItemValue(6, DTNZ6_28, "28", NULL, "(28)含28组合的组六号码");
				_SetValueRangeItemValue(7, DTNZ6_Z3, "Z3", NULL, "(Z3)组三号码");
				_SetValueRangeItemValue(8, DTNZ6_36, "36", NULL, "(36)含36组合的组六号码");
				_SetValueRangeItemValue(9, DTNZ6_39, "39", NULL, "(39)含39组合的组六号码");
				_SetValueRangeItemValue(10, DTNZ6_47, "47", NULL, "(47)含47组合的组六号码");
				_SetValueRangeItemValue(11, DTNZ6_58, "58", NULL, "(58)含58组合的组六号码");
				_SetValueRangeItemValue(12, DTNZ6_69, "69", NULL, "(69)含69组合的组六号码");
				_SetValueRangeItemValue(13, DTNZ6_C, "C", NULL, "(C)0路、1路、2路各出一个");
				_SetValueRangeItemValue(14, DTNZ6_A, "A", NULL, "(A)全0路、全1路、全2路");
				_SetValueRangeItemValue(15, DTNZ6_BZ, "BZ", NULL, "(BZ)豹子号码");
			} break;
            case IID_STC_TWONUM_Z6_2:
			{
				m_lpValueRangeTemp->nItemCount = 7;
				_SetValueRangeItemValue(0, DTNZ62_00, "00", NULL, "(00)有且仅有2个0路的组六号码");
				_SetValueRangeItemValue(1, DTNZ62_11, "11", NULL, "(11)有且仅有2个1路的组六号码");
				_SetValueRangeItemValue(2, DTNZ62_22, "22", NULL, "(22)有且仅有2个2路的组六号码");
				_SetValueRangeItemValue(3, DTNZ62_Z3, "Z3", NULL, "(Z3)组三号码");
				_SetValueRangeItemValue(4, DTNZ62_C, "C", NULL, "(C)0路、1路、2路各出一个");
				_SetValueRangeItemValue(5, DTNZ62_A, "A", NULL, "(A)全0路、全1路、全2路");
				_SetValueRangeItemValue(6, DTNZ62_BZ, "BZ", NULL, "(BZ)豹子号码");
			} break;
            case IID_STC_TWONUM_Z3:
			{
				m_lpValueRangeTemp->nItemCount = 11;
				_SetValueRangeItemValue(0, DTNZ3_00, "00", NULL, "(00)对子码为0路且另一码为0路");
				_SetValueRangeItemValue(1, DTNZ3_01, "01", NULL, "(01)对子码为0路且另一码为1路");
				_SetValueRangeItemValue(2, DTNZ3_02, "02", NULL, "(02)对子码为0路且另一码为2路");
				_SetValueRangeItemValue(3, DTNZ3_10, "10", NULL, "(10)对子码为1路且另一码为0路");
				_SetValueRangeItemValue(4, DTNZ3_11, "11", NULL, "(11)对子码为1路且另一码为1路");
				_SetValueRangeItemValue(5, DTNZ3_12, "12", NULL, "(12)对子码为1路且另一码为2路");
				_SetValueRangeItemValue(6, DTNZ3_20, "20", NULL, "(20)对子码为2路且另一码为0路");
				_SetValueRangeItemValue(7, DTNZ3_21, "21", NULL, "(21)对子码为2路且另一码为1路");
				_SetValueRangeItemValue(8, DTNZ3_22, "22", NULL, "(22)对子码为2路且另一码为2路");
				_SetValueRangeItemValue(9, DTNZ3_Z6, "Z6", NULL, "(Z6)组六号码");
				_SetValueRangeItemValue(10, DTNZ3_BZ, "BZ", NULL, "(BZ)豹子号码");
			} break;
            case IID_STC_TWONUM_Z3_2:
			{
				m_lpValueRangeTemp->nItemCount = 5;
				_SetValueRangeItemValue(0, DTNZ32_0, "0", NULL, "(0)对子码为0路的号码");
				_SetValueRangeItemValue(1, DTNZ32_1, "1", NULL, "(1)对子码为1路的号码");
				_SetValueRangeItemValue(2, DTNZ32_2, "2", NULL, "(2)对子码为2路的号码");
				_SetValueRangeItemValue(3, DTNZ32_Z6, "Z6", NULL, "(Z6)组六号码");
				_SetValueRangeItemValue(4, DTNZ32_BZ, "BZ", NULL, "(BZ)豹子号码");
			} break;
                
            case IID_STC_ORDER_MIN:
            case IID_STC_ORDER_MID: 
            case IID_STC_ORDER_MAX:
            case IID_STC_ORDER_MINADDMID:
            case IID_STC_ORDER_MINADDMAX:
            case IID_STC_ORDER_MIDADDMAX:
            case IID_STC_ORDER_MIDSUBMIN:
            case IID_STC_ORDER_MAXSUBMIN:
            case IID_STC_ORDER_MAXSUBMID: _FillValueRange_Order(cdtIID.dwSystemIID); break;
            case IID_STC_ORDER_MINADDMID_TAIL:
            case IID_STC_ORDER_MINADDMAX_TAIL:
            case IID_STC_ORDER_MIDADDMAX_TAIL:
            case IID_STC_ORDER_MIDSUBMIN_TAIL:
            case IID_STC_ORDER_MAXSUBMIN_TAIL:
            case IID_STC_ORDER_MAXSUBMID_TAIL: _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 0, 9); break;
            case IID_STC_ORDER_POSSUB_ARR_SIN:
            case IID_STC_ORDER_POSSUB_ARR_GRO:
            case IID_STC_ORDER_POSSUBTAIL_ARR_SIN:
            case IID_STC_ORDER_POSSUBTAIL_ARR_GRO:
            case IID_STC_ORDER_POSSUMTAIL_ARR_SIN:
            case IID_STC_ORDER_POSSUMTAIL_ARR_GRO:
            case IID_STC_ORDER_POSSUM_ARR_SIN:
            case IID_STC_ORDER_POSSUM_ARR_GRO: _FillValueRange_Arrange_Numbers(cdtIID.dwSystemIID, 0, 9); break;
            case IID_STC_ORDER_RELATIVEDZX_POS_1:
            case IID_STC_ORDER_RELATIVEDZX_POS_2:
            case IID_STC_ORDER_RELATIVEDZX_POS_3:
			{
				m_lpValueRangeTemp->nItemCount = 3;
				_SetValueRangeItemValue(0, 2, "D", NULL, NULL);
				_SetValueRangeItemValue(1, 1, "Z", NULL, NULL);
				_SetValueRangeItemValue(2, 0, "X", NULL, NULL);
			} break;
            case IID_STC_ORDER_RELATIVEDZX_ARR: 
			{
				char szNames[3][8] = {"大", "中", "小"};
				int nValues[3] = {2, 1, 0};
				_FillValueRange_Arrange(cdtIID.dwSystemIID, szNames, nValues, 3);
			}break;
            case IID_STC_ORDER_RELATIVEDZX_ARR_VALUE:
			{
				if(m_btDataType & DATA_TYPE_REPEATABLE)
				{
					char szNames[][32] = {"0：小小大（002）", "1：小中大（012）", "2：小大小（020）", "3：小大中（021）", \
						"4：小大大（022）", "5：中小大（102）", "6：中中中（111）", "7：中大小（120）", \
						"8：大小小（200）", "9：大小中（201）", "10：大小大（202）", "11：大中小（210）", "12：大大小（220）"};
					m_lpValueRangeTemp->nItemCount = sizeof(szNames)/sizeof(szNames[0]);
					for(int i=0; i<m_lpValueRangeTemp->nItemCount; i++)
						_SetValueRangeItemValue(i, i, NULL, NULL, szNames[i]);
				}
				else
				{
					char szNames[][32] = {"0：小中大（012）", "1：小大中（021）", "2：中小大（102）", "3：中大小（120）", "4：大小中（201）", "5：大中小（210）"};
					m_lpValueRangeTemp->nItemCount = sizeof(szNames)/sizeof(szNames[0]);
					for(int i=0; i<m_lpValueRangeTemp->nItemCount; i++)
						_SetValueRangeItemValue(i, i, NULL, NULL, szNames[i]);
				}
			} break;
                
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
            case IID_STC_SPACE_GRO_8: _FillValueRange_Space(cdtIID.dwSystemIID); break;
                
            case IID_STC_NEARSPACE_12:
            case IID_STC_NEARSPACE_23:
            case IID_STC_NEARSPACE_34:
            case IID_STC_NEARSPACE_45:
            case IID_STC_NEARSPACE_56:
            case IID_STC_NEARSPACE_67:
            case IID_STC_NEARSPACE_78:	
            case IID_STC_NEARSPACE_HT: 
            case IID_STC_NEARSPACE_MAX:
            case IID_STC_NEARSPACE_ANY:
                if(m_btDataType & DATA_TYPE_REPEATABLE)
                    _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 0, m_btMaxNumber - m_btMinNumber);
                else
                    _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 1, m_btMaxNumber - m_btMinNumber - m_btNumberCount + 2);
                break;
            case IID_STC_NEARSPACE_MAXEX:
                if(m_btDataType & DATA_TYPE_REPEATABLE)
                    _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, (m_btMaxNumber - m_btMinNumber + m_btNumberCount) / m_btNumberCount, m_btMaxNumber - m_btMinNumber);
                else
                    _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, (m_btMaxNumber - m_btMinNumber + m_btNumberCount) / m_btNumberCount, m_btMaxNumber - m_btMinNumber - m_btNumberCount + 2);
                break;
            case IID_STC_NEARSPACE_MIN:
                _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, (m_btDataType & DATA_TYPE_REPEATABLE) ? 0 : 1, (m_btMaxNumber - m_btMinNumber + 1) / (m_btNumberCount - 1));
                break;
            case IID_STC_NEARSPACE_MINEX:
                _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, (m_btDataType & DATA_TYPE_REPEATABLE) ? 0 : 1, (m_btMaxNumber - m_btMinNumber + 1) / m_btNumberCount);
                break;
            case IID_STC_NEARSPACE_AVG:
			{
				double dbMax = double(m_btMaxNumber - m_btMinNumber) / double(m_btNumberCount - 1);
				int nMax = int(dbMax);
				if(dbMax - double(nMax) >= 0.5)
					nMax ++;	// 四舍五入
				_AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, (m_btDataType & DATA_TYPE_REPEATABLE) ? 0 : 1, nMax);
			} break;
                
            case IID_STC_1D_POSITIVE_SIN:
            case IID_STC_1D_POSITIVE_GRO:
            case IID_STC_1D_NEGATIVE_SIN:
            case IID_STC_1D_NEGATIVE_GRO:
            case IID_STC_1D_RPOSITIVE_SIN:
            case IID_STC_1D_RPOSITIVE_GRO:
            case IID_STC_1D_RNEGATIVE_SIN:
            case IID_STC_1D_RNEGATIVE_GRO: _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 0, 9); break;
                
            case IID_STC_MUL: _FillValueRange_Mul(cdtIID.dwSystemIID); break;
            case IID_STC_MUL_ODDEVEN: 
                _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 0, 2);
                _SetValueRangeItemValue(0, 1, "奇", "J", "奇");
                _SetValueRangeItemValue(1, 0, "偶", "O", "偶");
                _SetValueRangeItemValue(2, -1, "零", "◎", "零");
                break;
            case IID_STC_MUL_BIGSMALL:
                _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 0, 2);
                _SetValueRangeItemValue(0, 1, "大", "D", "大");
                _SetValueRangeItemValue(1, 0, "小", "X", "小");
                _SetValueRangeItemValue(2, -1, "零", "◎", "零");
                break;
            case IID_STC_MUL_012:
                _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 0, 3);
                _SetValueRangeItemValue(3, -1, "零", "◎", "零");
                break;
            case IID_STC_POSMULSUM:
            case IID_STC_RESPOSMULSUM: _FillValueRange_PosMulSum(cdtIID.dwSystemIID); break;
            case IID_STC_MULTAIL:
            case IID_STC_POSMULSUMTAIL:
            case IID_STC_RESPOSMULSUMTAIL: _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 0, 9); break;
                
                ///////////////////////////////////////////////////////////////////////////////////////////////////
            case IID_DNC_DIS_POS_1:
            case IID_DNC_DIS_POS_2:
            case IID_DNC_DIS_POS_3:
            case IID_DNC_DIS_POS_4:
            case IID_DNC_DIS_POS_5:
            case IID_DNC_DIS_POS_6:
            case IID_DNC_DIS_POS_7:
            case IID_DNC_DIS_POS_8:
            case IID_DNC_DIS_POS_ANY: 
                _FillValueRange_PrevData(WORD(cdtIID.dwSystemIID - IID_DNC_DIS_POS_1 + IID_DNC_PREVDATA_POS_1), PDT_DIS); break;
                
            case IID_DNC_DIS_SUM: _FillValueRange_PrevData(IID_DNC_PREVDATA_SUM, PDT_DIS); break;
            case IID_DNC_DIS_SUMTAIL: _FillValueRange_PrevData(IID_DNC_PREVDATA_SUMTAIL, PDT_DIS); break;
                
            case IID_DNC_DIS_ARR_SIN: _FillValueRange_PrevData(IID_DNC_PREVDATA_ARR_SIN, PDT_DIS); break;
            case IID_DNC_DIS_ARR_GRO: _FillValueRange_PrevData(IID_DNC_PREVDATA_ARR_GRO, PDT_DIS); break;
                
            case IID_DNC_XL_LEFT:
            case IID_DNC_XL_RIGHT: _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 0, m_btNumberCount); break;
            case IID_DNC_XL_VALUE: _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 0, 2 * m_btNumberCount); break;
            case IID_DNC_XL_COMBO: _FillValueRange_XXXCombo("左斜连值", "右斜连值"); break;
                
            case IID_DNC_DEPARTURE:
                if(m_btDataType & DATA_TYPE_REPEATABLE)
                    _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 0, m_btMaxNumber - m_btMinNumber);
                else
                    _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 0, (m_btMaxNumber - m_btMinNumber + 1) - m_btNumberCount);
                break;
                
            case IID_DNC_NUMS_MATCHCOUNT_SIN:
            case IID_DNC_NUMS_MATCHCOUNT_GRO:
                _AutoFillValueRange_LinkNoLimit(cdtIID.dwSystemIID, 1); break;
                
            case IID_DNC_TESTNUMS_REPEAT_CUR_CUR:
            case IID_DNC_TESTNUMS_REPEAT_PRE_CUR: _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 0, m_btNumberCount); break;
            case IID_DNC_TESTNUMS_REPEAT_COMPLEX:
			{
				m_lpValueRangeTemp->nItemCount = 4;
				_SetValueRangeItemValue(0, DTTESTNUMS_NONE, "0", NULL, "(0)与试机号和上期开奖号都不重复");
				_SetValueRangeItemValue(1, DTTESTNUMS_NUMS1, "1", NULL, "(1)与试机号不重，与上期奖号重1个");
				_SetValueRangeItemValue(2, DTTESTNUMS_TEST1, "2", NULL, "(2)与试机号重1个，与上期奖号不重");
				_SetValueRangeItemValue(3, DTTESTNUMS_ALL1, "3", NULL, "(3)与试机号和上期奖号都重1个");
				if(m_btNumberCount > 1)
				{
					m_lpValueRangeTemp->nItemCount ++;
					_SetValueRangeItemValue(4, DTTESTNUMS_OTHER, "4", NULL, "(4)其他");
				}
			} break;
            case IID_DNC_TESTNUMS_REPEAT_COMBO: _FillValueRange_XXXCombo("与试机号重", "与上期奖号重"); break;
                
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
                _AutoFillValueRange_LinkNoLimit(cdtIID.dwSystemIID); break;
                
            case IID_DNC_SKIPSIN_TAIL_1:
            case IID_DNC_SKIPSIN_TAIL_2:
            case IID_DNC_SKIPSIN_TAIL_3:
            case IID_DNC_SKIPSIN_TAIL_4:
            case IID_DNC_SKIPSIN_TAIL_5:
            case IID_DNC_SKIPSIN_TAIL_6:
            case IID_DNC_SKIPSIN_TAIL_7:
            case IID_DNC_SKIPSIN_TAIL_8:
            case IID_DNC_SKIPSIN_TAIL_MIN:
            case IID_DNC_SKIPSIN_TAIL_MAX:
            case IID_DNC_SKIPSIN_TAIL_ANY: 
            case IID_DNC_SKIPSIN_SUMTAIL:
            case IID_DNC_SKIPGRO_TAIL_1:
            case IID_DNC_SKIPGRO_TAIL_2:
            case IID_DNC_SKIPGRO_TAIL_3:
            case IID_DNC_SKIPGRO_TAIL_4:
            case IID_DNC_SKIPGRO_TAIL_5:
            case IID_DNC_SKIPGRO_TAIL_6:
            case IID_DNC_SKIPGRO_TAIL_7:
            case IID_DNC_SKIPGRO_TAIL_8:
            case IID_DNC_SKIPGRO_TAIL_MIN:
            case IID_DNC_SKIPGRO_TAIL_MAX:
            case IID_DNC_SKIPGRO_TAIL_ANY: 
            case IID_DNC_SKIPGRO_SUMTAIL:
                _AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 0, 9); break;
                
            case IID_DNC_SKIPSIN_TAIL_ARR_SIN:
            case IID_DNC_SKIPSIN_TAIL_ARR_GRO: 
            case IID_DNC_SKIPGRO_TAIL_ARR_SIN:
            case IID_DNC_SKIPGRO_TAIL_ARR_GRO: 
                _FillValueRange_Arrange_Numbers(cdtIID.dwSystemIID, 0, 9); break;
                
            case IID_DNC_SKIPSIN_SUM_AREA:
            case IID_DNC_SKIPSIN_SUM_AREA10:
            case IID_DNC_SKIPGRO_SUM_AREA:
            case IID_DNC_SKIPGRO_SUM_AREA10:
                _FillValueRange_Skip_Area(cdtIID.dwSystemIID); break;
                
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
            case IID_DNC_REPEAT_N+9:
                _FillValueRange_Arrange_Numbers(cdtIID.dwSystemIID, 0, m_btNumberCount); break;
                
            case IID_HIS_NUMS_ARR_SIN: _FillValueRange_Nums(IID_STC_NUMS_SIN); break;
            case IID_HIS_NUMS_ARR_GRO: _FillValueRange_Nums(IID_STC_NUMS_GRO); break;
            case IID_HIS_NUMS_1_ARR_SIN:
            case IID_HIS_NUMS_1_ARR_GRO:_FillValueRange_PosX(cdtIID.dwSystemIID, BYTE(8)); break;
            case IID_HIS_NUMS_2_ARR_SIN: _FillValueRange_TwoNums(IID_STC_TWONUMSIN_MN + 12); break;
            case IID_HIS_NUMS_2_ARR_GRO: _FillValueRange_TwoNums(IID_STC_TWONUMSIN_ANY); break;
                
            default:
			{
				if(cdtIID.dwSystemIID >= IID_STC_NUMBER_0NUM && cdtIID.dwSystemIID < IID_STC_NUMBER_MAXCOUNT)
				{
					_AutoFillValueRange_DefaultType(cdtIID.dwSystemIID, 0, _GetMaxSameNumberCount());
				}
				else if(cdtIID.dwSystemIID >= IID_DNC_XXX_BEGIN && cdtIID.dwSystemIID <= IID_DNC_XXX_END)	// 动态3态指标
				{
					_FillValueRange_3State(cdtIID.dwSystemIID);
				}
				else if(cdtIID.dwSystemIID >= IID_STC_TWOPOS_BEGIN && cdtIID.dwSystemIID <= IID_STC_TWOPOS_END)	// 两位关系
				{
					_FillValueRange_TwoPos(cdtIID.dwSystemIID);
				}
				else if(cdtIID.dwSystemIID >= IID_STC_TWONUMSIN_BEGIN && cdtIID.dwSystemIID <= IID_STC_TWONUMSIN_END)	// 单选二码
				{
					_FillValueRange_TwoNums(cdtIID.dwSystemIID);
				}
				else if(cdtIID.dwSystemIID >= IID_STC_REMM_BEGIN && cdtIID.dwSystemIID <= IID_STC_REMM_END)		// 余数
				{
					_FillValueRange_Remainder(cdtIID.dwSystemIID);
				}
				else if(cdtIID.dwSystemIID >= IID_DNC_PREVDATA_BEGIN && cdtIID.dwSystemIID <= IID_DNC_PREVDATA_END)	// 邻期关系
				{
					int nTypeIndex = (cdtIID.dwSystemIID - IID_DNC_PREVDATA_BEGIN) / 100;
					_FillValueRange_PrevData(DWORD(cdtIID.dwSystemIID - nTypeIndex * 100), nTypeIndex);
				}
				else
				{
					ASSERT(FALSE);
				}
			} break;
		}
	}
    
	ASSERT(m_lpValueRangeTemp->nItemCount != -1);	// 确保正确获取了
    
	int i = 0, nValue = 0;
    
#ifdef _DEBUG
	// 检查越界
	for(i=0; i<m_lpValueRangeTemp->nItemCount; i++)
	{
		CHECK_STRING_STACK(m_lpValueRangeTemp->stValueItem[i].szBallName);
		CHECK_STRING_STACK(m_lpValueRangeTemp->stValueItem[i].szColName);
		CHECK_STRING_STACK(m_lpValueRangeTemp->stValueItem[i].szValueName);
	}
#endif
    
	// 设置最大和最小值
	m_lpValueRangeTemp->nMinValue = m_lpValueRangeTemp->nMaxValue = m_lpValueRangeTemp->stValueItem[0].nValue;
	if(m_lpValueRangeTemp->nItemCount <= 1)
	{
		m_lpValueRangeTemp->nMaxValue = -1;
	}
	else
	{
		for(i=1; i<m_lpValueRangeTemp->nItemCount; i++)
		{
			nValue = m_lpValueRangeTemp->stValueItem[i].nValue;
			if(m_lpValueRangeTemp->nMinValue > nValue)
				m_lpValueRangeTemp->nMinValue = nValue;
			if(m_lpValueRangeTemp->nMaxValue < nValue)
				m_lpValueRangeTemp->nMaxValue = nValue;
		}
	}
    
	int nBufLen = sizeof(CDTVALUERANGE) + sizeof(CDTVALUERANGEITEM) * m_lpValueRangeTemp->nItemCount;
	(*lplpValueRange) = (LPCDTVALUERANGE)new BYTE[nBufLen];
	memcpy((*lplpValueRange), m_lpValueRangeTemp, nBufLen);
    
	// 返回
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

void CIndicator::_AutoFillValueRange_DefaultType(DWORD /*dwID*/, int nMinValue, int nMaxValue, BYTE btFixValueLen)
{
	m_lpValueRangeTemp->nItemCount = nMaxValue - nMinValue + 1;
	ASSERT(m_lpValueRangeTemp->nItemCount > 0);
    
	for(int i=0; i<m_lpValueRangeTemp->nItemCount; i++)
	{
		m_lpValueRangeTemp->stValueItem[i].nValue = nMinValue + i;
		if(btFixValueLen == 0)
		{
			if(m_lpValueRangeTemp->stValueItem[i].nValue > -100 && m_lpValueRangeTemp->stValueItem[i].nValue < 1000)
				sprintf(m_lpValueRangeTemp->stValueItem[i].szBallName, "%d", m_lpValueRangeTemp->stValueItem[i].nValue);
			else
				m_lpValueRangeTemp->stValueItem[i].szBallName[0] = 0;
			CHECK_STRING_STACK(m_lpValueRangeTemp->stValueItem[i].szBallName);
			strcpy(m_lpValueRangeTemp->stValueItem[i].szColName, m_lpValueRangeTemp->stValueItem[i].szBallName);
			sprintf(m_lpValueRangeTemp->stValueItem[i].szValueName, "%d", m_lpValueRangeTemp->stValueItem[i].nValue);
		}
		else
		{
			sprintf(m_lpValueRangeTemp->stValueItem[i].szBallName, "%0*d", btFixValueLen, m_lpValueRangeTemp->stValueItem[i].nValue);
			strcpy(m_lpValueRangeTemp->stValueItem[i].szColName, m_lpValueRangeTemp->stValueItem[i].szBallName);
			strcpy(m_lpValueRangeTemp->stValueItem[i].szValueName, m_lpValueRangeTemp->stValueItem[i].szColName);
		}
	}
}

void CIndicator::_AutoFillValueRange_LinkNoLimit(DWORD /*dwID*/, int nMinValue)
{	
	m_lpValueRangeTemp->nItemCount = 1;	
    
	// 无上限的值，则值域有一个元素保存最小值
	ZeroMemory(m_lpValueRangeTemp->stValueItem, m_lpValueRangeTemp->nItemCount * sizeof(CDTVALUERANGEITEM));
    
	_SetValueRangeItemValue(0, nMinValue);
}

void CIndicator::_SetValueRangeItemValue(int nValueIndex, int nValue, LPCSTR lpszColName, LPCSTR lpszBallName, LPCSTR lpszValueName)
{
	ASSERT(nValueIndex >= 0 && nValueIndex < m_lpValueRangeTemp->nItemCount);
    
	m_lpValueRangeTemp->stValueItem[nValueIndex].nValue = nValue;
	if(lpszColName == NULL)
	{
		if(nValue > -100 && nValue < 100)
			sprintf(m_lpValueRangeTemp->stValueItem[nValueIndex].szColName, "%d", nValue);
		else
			m_lpValueRangeTemp->stValueItem[nValueIndex].szColName[0] = 0;
	}
	else
	{
		strcpy(m_lpValueRangeTemp->stValueItem[nValueIndex].szColName, lpszColName);
	}
	CHECK_STRING_STACK(m_lpValueRangeTemp->stValueItem[nValueIndex].szColName);
    
	if(lpszBallName == NULL)
	{
		strcpy(m_lpValueRangeTemp->stValueItem[nValueIndex].szBallName, m_lpValueRangeTemp->stValueItem[nValueIndex].szColName);
	}
	else
	{
		strcpy(m_lpValueRangeTemp->stValueItem[nValueIndex].szBallName, lpszBallName);
		CHECK_STRING_STACK(m_lpValueRangeTemp->stValueItem[nValueIndex].szBallName);
	}
    
	if(lpszValueName == NULL)
	{
		sprintf(m_lpValueRangeTemp->stValueItem[nValueIndex].szValueName, "%d", nValue);
	}
	else
	{
		strcpy(m_lpValueRangeTemp->stValueItem[nValueIndex].szValueName, lpszValueName);
		CHECK_STRING_STACK(m_lpValueRangeTemp->stValueItem[nValueIndex].szValueName);
	}
}

BYTE CIndicator::_GetPosMaxValue(BYTE btPosIndex, BOOL bSingle)
{
	if(btPosIndex >= 8)		// 任意位置
	{
		return m_btMaxNumber;
	}
    
	BOOL bOrdered = (!bSingle || (m_btDataType & DATA_TYPE_ORDERED)) ? TRUE : FALSE;
    
	if(m_btDataType & DATA_TYPE_REPEATABLE)	// 可重复的
	{
		return m_btMaxNumber;
	}
	else	// 不可重复的
	{
		if(bOrdered)	// 排序的
		{
			return BYTE(m_btMaxNumber - m_btNumberCount + btPosIndex + 1);
		}
		else	// 未排序的
		{
			return m_btMaxNumber;
		}
	}
}

BYTE CIndicator::_GetPosMinValue(BYTE btPosIndex, BOOL bSingle)
{
	if(btPosIndex >= 8)		// 任意位置
	{
		return m_btMinNumber;
	}
    
	BOOL bOrdered = (!bSingle || (m_btDataType & DATA_TYPE_ORDERED)) ? TRUE : FALSE;
	
	if(m_btDataType & DATA_TYPE_REPEATABLE)	// 可重复的
	{
		return m_btMinNumber;
	}
	else	// 不可重复的
	{
		if(bOrdered)	// 排序的
		{
			return BYTE(m_btMinNumber + btPosIndex);
		}
		else	// 未排序的
		{
			return m_btMinNumber;
		}
	}
}

int CIndicator::_GetACValueRange(int &nMin)
{
	nMin = 0;
	int nMax = g_pICstPubFunc->GetArrangeComboValue(m_btNumberCount, 2);
	int nAllNumsCount = (m_btMaxNumber - m_btMinNumber + 1);
    
	if(m_btDataType & DATA_TYPE_REPEATABLE)	// 可重复的
	{
		nMin = 2 - m_btNumberCount;
		switch(m_btNumberCount)
		{
            case 3: nMax = 3; break;
            case 4:
			{
				if(nAllNumsCount >= 7)
					nMax = 6;
				else 
					nMax = nAllNumsCount - 1;
			} break;
            case 5: 
                if(nAllNumsCount <= 7)
                    nMax = nAllNumsCount; 
                else
                    nMax = nAllNumsCount - 1; 
                break;
            case 6:
            case 7: nMax = nAllNumsCount; break;
            default: ASSERT(FALSE);
		}
		nMax -= (m_btNumberCount - 1);
	}
	else	// 不可重复的
	{
		switch(m_btNumberCount)
		{
            case 3: nMax = 1; break;
            case 4:
			{
				if(nAllNumsCount >= 7)
					nMax = 3;
				else 
					nMax = nAllNumsCount - 4;
			} break;
            case 5:
			{
				if(nAllNumsCount >= 12)
					nMax = 6;
				else if(nAllNumsCount >= 10)
					nMax = 5;
				else 
					nMax = nAllNumsCount - 5;
			} break;
            case 6:
			{
				if(nAllNumsCount >= 18)
					nMax = 10;
				else if(nAllNumsCount >= 17)
					nMax = 9;
				else if(nAllNumsCount >= 14)
					nMax = 8;
				else 
					nMax = nAllNumsCount - 6;
			} break;
            case 7:
			{
				if(nAllNumsCount >= 25)
					nMax = 15;
				else if(nAllNumsCount >= 23)
					nMax = 14;
				else if(nAllNumsCount >= 21)
					nMax = 13;
				else if(nAllNumsCount >= 20)
					nMax = 12;
				else if(nAllNumsCount >= 18)
					nMax = 11;
				else 
					nMax = nAllNumsCount - 7;
			} break;
            default: ASSERT(FALSE);
		}
	}
    
	return nMax;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////
void CIndicator::_FillValueRange_SameCombo()
{
	m_lpValueRangeTemp->nItemCount = _GetMaxSameNumberCount();
	ASSERT(m_lpValueRangeTemp->nItemCount > 0);
	
	_SetValueRangeItemValue(0, 0, "0", NULL, "(0)无同号");
	for(int i=1; i<m_lpValueRangeTemp->nItemCount; i++)
	{
		m_lpValueRangeTemp->stValueItem[i].nValue = i + 1;
		sprintf(m_lpValueRangeTemp->stValueItem[i].szBallName, "%d", m_lpValueRangeTemp->stValueItem[i].nValue);
		strcpy(m_lpValueRangeTemp->stValueItem[i].szColName, m_lpValueRangeTemp->stValueItem[i].szBallName);
		sprintf(m_lpValueRangeTemp->stValueItem[i].szValueName, "(%d)%d个同号", m_lpValueRangeTemp->stValueItem[i].nValue, m_lpValueRangeTemp->stValueItem[i].nValue);
	}
}

void CIndicator::_FillValueRange_Genre()
{
	if(m_btDataType & DATA_TYPE_REPEATABLE)
	{
		switch(m_btNumberCount)
		{
            case 2:
                m_lpValueRangeTemp->nItemCount = 5;
                _SetValueRangeItemValue(0, DT_AA, NULL, NULL, "(0)对子");
                _SetValueRangeItemValue(1, DT_OL, NULL, NULL, "(1)奇连");
                _SetValueRangeItemValue(2, DT_EL, NULL, NULL, "(2)偶连");
                _SetValueRangeItemValue(3, DT_AB, NULL, NULL, "(3)连子");
                _SetValueRangeItemValue(4, DT_AD, NULL, NULL, "(4)杂号");
                break;
            case 3:
                m_lpValueRangeTemp->nItemCount = 5;
                _SetValueRangeItemValue(0, DT_AAA, NULL, NULL, "(0)豹子");
                _SetValueRangeItemValue(1, DT_AAB, NULL, NULL, "(1)组三");
                _SetValueRangeItemValue(2, DT_ABC, NULL, NULL, "(2)顺子");
                _SetValueRangeItemValue(3, DT_ABD, NULL, NULL, "(3)半顺");
                _SetValueRangeItemValue(4, DT_ACE, NULL, NULL, "(4)杂六");
                break;
            case 4:
                m_lpValueRangeTemp->nItemCount = 8;
                _SetValueRangeItemValue(0, DT_AAAA, NULL, NULL, "(0)炸弹");
                _SetValueRangeItemValue(1, DT_AAAB, NULL, NULL, "(1)豹子");
                _SetValueRangeItemValue(2, DT_AABB, NULL, NULL, "(2)两对");
                _SetValueRangeItemValue(3, DT_AABC, NULL, NULL, "(3)一对");
                _SetValueRangeItemValue(4, DT_ABCD, NULL, NULL, "(4)顺子");
                _SetValueRangeItemValue(5, DT_ABCE, NULL, NULL, "(5)三连");
                _SetValueRangeItemValue(6, DT_ABDF, NULL, NULL, "(6)二连");
                _SetValueRangeItemValue(7, DT_ACEG, NULL, NULL, "(7)杂号");
                break;
            default: ASSERT(FALSE); break;
		}		
	}
	else
	{
		switch(m_btNumberCount)
		{
            case 2:
                m_lpValueRangeTemp->nItemCount = 4;
                _SetValueRangeItemValue(0, DT_OL, NULL, NULL, "(1)奇连");
                _SetValueRangeItemValue(1, DT_EL, NULL, NULL, "(2)偶连");
                _SetValueRangeItemValue(2, DT_AB, NULL, NULL, "(3)连子");
                _SetValueRangeItemValue(3, DT_AD, NULL, NULL, "(4)杂号");
                break;
            default: ASSERT(FALSE); break;
		}
	}
}

void CIndicator::_FillValueRange_GenreTrans()
{
	m_lpValueRangeTemp->nItemCount = 8;
	_SetValueRangeItemValue(0, DTE_TAAA, NULL, NULL, "(0)真豹子");
	_SetValueRangeItemValue(1, DTE_FAAA, NULL, NULL, "(1)伪豹子");
	_SetValueRangeItemValue(2, DTE_TAAB, NULL, NULL, "(2)真组三");
	_SetValueRangeItemValue(3, DTE_FAAB, NULL, NULL, "(3)伪组三");
	_SetValueRangeItemValue(4, DTE_TABC, NULL, NULL, "(4)真顺子");
	_SetValueRangeItemValue(5, DTE_FABC, NULL, NULL, "(5)伪顺子");
	_SetValueRangeItemValue(6, DTE_TABD, NULL, NULL, "(6)真半顺");
	_SetValueRangeItemValue(7, DTE_TACE, NULL, NULL, "(7)真杂六");
}

void CIndicator::_FillValueRange_Shape()
{
	m_lpValueRangeTemp->nItemCount = 5;
	_SetValueRangeItemValue(0, DS_PARALLEL, NULL, NULL, "(0)平行形");
	_SetValueRangeItemValue(1, DS_ASCEND, NULL, NULL, "(1)上升形");
	_SetValueRangeItemValue(2, DS_DESCEND, NULL, NULL, "(2)下降形");
	_SetValueRangeItemValue(3, DS_HEAVE, NULL, NULL, "(3)凸起形");
	_SetValueRangeItemValue(4, DS_CONCAVE, NULL, NULL, "(4)凹下形");
}

void CIndicator::_FillValueRange_ShapeEx()
{
	m_lpValueRangeTemp->nItemCount = 13;
	_SetValueRangeItemValue(0, DSEX_AAA, NULL, NULL, "(0)平行形");
	_SetValueRangeItemValue(1, DSEX_AAB, NULL, NULL, "(1)左升三");
	_SetValueRangeItemValue(2, DSEX_ABC, NULL, NULL, "(2)上升六");
	_SetValueRangeItemValue(3, DSEX_ABB, NULL, NULL, "(3)右升三");
	_SetValueRangeItemValue(4, DSEX_BBA, NULL, NULL, "(4)左降三");
	_SetValueRangeItemValue(5, DSEX_CBA, NULL, NULL, "(5)下降六");
	_SetValueRangeItemValue(6, DSEX_BAA, NULL, NULL, "(6)右降三");
	_SetValueRangeItemValue(7, DSEX_BCA, NULL, NULL, "(7)前凸六");
	_SetValueRangeItemValue(8, DSEX_ABA, NULL, NULL, "(8)凸起三");
	_SetValueRangeItemValue(9, DSEX_ACB, NULL, NULL, "(9)后凸六");
	_SetValueRangeItemValue(10, DSEX_CAB, NULL, NULL, "(10)前凹六");
	_SetValueRangeItemValue(11, DSEX_BAB, NULL, NULL, "(11)凹下三");
	_SetValueRangeItemValue(12, DSEX_BAC, NULL, NULL, "(12)后凹六");
}

void CIndicator::_FillValueRange_Shape_XXX(DWORD dwID)
{
	switch(dwID)
	{
        case IID_STC_SHAPE_AAB: 
            m_lpValueRangeTemp->nItemCount = 4;
            _SetValueRangeItemValue(0, DTAAB_NONE, NULL, NULL, "(0)非组三");
            _SetValueRangeItemValue(1, DTAAB_AAB, NULL, NULL, "(1)AAB型");
            _SetValueRangeItemValue(2, DTAAB_ABA, NULL, NULL, "(2)ABA型");
            _SetValueRangeItemValue(3, DTAAB_BAA, NULL, NULL, "(3)BAA型");
            break;
        case IID_STC_SHAPE_ABC: 
            m_lpValueRangeTemp->nItemCount = 7;
            _SetValueRangeItemValue(0, DTABC_NONE, NULL, NULL, "(0)非顺子");
            _SetValueRangeItemValue(1, DTABC_ABC, NULL, NULL, "(1)ABC型");
            _SetValueRangeItemValue(2, DTABC_ACB, NULL, NULL, "(2)ACB型");
            _SetValueRangeItemValue(3, DTABC_BAC, NULL, NULL, "(3)BAC型");
            _SetValueRangeItemValue(4, DTABC_BCA, NULL, NULL, "(4)BCA型");
            _SetValueRangeItemValue(5, DTABC_CAB, NULL, NULL, "(5)CAB型");
            _SetValueRangeItemValue(6, DTABC_CBA, NULL, NULL, "(6)CBA型");
            break;
        case IID_STC_SHAPE_ABD: 
            m_lpValueRangeTemp->nItemCount = 4;
            _SetValueRangeItemValue(0, DTABD_NONE, NULL, NULL, "(0)非半顺");
            _SetValueRangeItemValue(1, DTABD_ABD, NULL, NULL, "(1)ABD型");
            _SetValueRangeItemValue(2, DTABD_ADB, NULL, NULL, "(2)ADB型");
            _SetValueRangeItemValue(3, DTABD_DAB, NULL, NULL, "(3)DAB型");
            break;
        default: ASSERT(FALSE); break;
	}
}

void CIndicator::_FillValueRange_XXXCombo(LPCSTR lpszName1, LPCSTR lpszName2)
{
	ASSERT(lpszName1 != NULL && lpszName2 != NULL);
    
	m_lpValueRangeTemp->nItemCount = (m_btNumberCount + 1) * (m_btNumberCount + 1);
	ASSERT(m_lpValueRangeTemp->nItemCount > 0);
	
	int i = 0, j = 0, nIndex = 0;
	for(i=0; i<=m_btNumberCount; i++)
	{
		for(j=0; j<=m_btNumberCount; j++)
		{
			m_lpValueRangeTemp->stValueItem[nIndex].nValue = i * 10 + j;
			sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szBallName, "%02d", m_lpValueRangeTemp->stValueItem[nIndex].nValue);
			strcpy(m_lpValueRangeTemp->stValueItem[nIndex].szColName, m_lpValueRangeTemp->stValueItem[nIndex].szBallName);
			sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "(%02d)%s%d，%s%d", m_lpValueRangeTemp->stValueItem[nIndex].nValue, lpszName1, i, lpszName2, j);
			nIndex ++;
		}
	}
}

void CIndicator::_FillValueRange_DHK(DWORD dwID)
{
	switch(dwID)
	{
        case IID_STC_DHK_HD:
            m_lpValueRangeTemp->nItemCount = 2;
            _SetValueRangeItemValue(0, 0, "0", NULL, "(0)合胆不同"); 
            _SetValueRangeItemValue(1, 1, "1", NULL, "(1)合胆同"); 
            break;
        case IID_STC_DHK_KD:
            m_lpValueRangeTemp->nItemCount = 2;
            _SetValueRangeItemValue(0, 0, "0", NULL, "(0)跨胆不同"); 
            _SetValueRangeItemValue(1, 1, "1", NULL, "(1)跨胆同"); 
            break;
        case IID_STC_DHK_HK:  
            m_lpValueRangeTemp->nItemCount = 2;
            _SetValueRangeItemValue(0, 0, "0", NULL, "(0)合跨不同"); 
            _SetValueRangeItemValue(1, 1, "1", NULL, "(1)合跨同"); 
            break;
        case IID_STC_DHK_HDKD:
            m_lpValueRangeTemp->nItemCount = 4;
            _SetValueRangeItemValue(0, 0, "0", NULL, "(0)合胆同跨胆同"); 
            _SetValueRangeItemValue(1, 1, "1", NULL, "(1)合胆同跨胆不同"); 
            _SetValueRangeItemValue(2, 2, "2", NULL, "(2)合胆不同跨胆同"); 
            _SetValueRangeItemValue(3, 3, "3", NULL, "(3)合胆不同跨胆不同"); 
            break;
        case IID_STC_DHK_HDKDHK:	
            m_lpValueRangeTemp->nItemCount = 6;
            _SetValueRangeItemValue(0, 0, "0", NULL, "(0)合胆同跨胆同 合跨同"); 
            _SetValueRangeItemValue(1, 1, "1", NULL, "(1)合胆同跨胆同 合跨不同"); 
            _SetValueRangeItemValue(2, 2, "2", NULL, "(2)合胆同跨胆不同 合跨不同"); 
            _SetValueRangeItemValue(3, 3, "3", NULL, "(3)合胆不同跨胆同 合跨不同"); 
            _SetValueRangeItemValue(4, 4, "4", NULL, "(4)合胆不同跨胆不同 合跨同"); 
            _SetValueRangeItemValue(5, 5, "5", NULL, "(5)合胆不同跨胆不同 合跨不同"); 
            break;
	}
}

void CIndicator::_FillValueRange_Nums(DWORD dwID)
{
	ASSERT(m_btNumberCount <= 3 && m_btMaxNumber <= 9);
    
	int i, j, k, nIndex = 0, nValue = 0;
	char szValueName[8] = {0};
	m_lpValueRangeTemp->nItemCount = 1000;
    
	if(dwID == IID_STC_NUMS_SIN)		// 单选号码
	{
		for(i=m_btMinNumber; i<=m_btMaxNumber; i++)
		{
			for(j=m_btMinNumber; j<=m_btMaxNumber; j++)
			{
				if(m_btNumberCount == 2)
				{
					nValue = i * 10 + j;
					sprintf(szValueName, "%02d", nValue);
					_SetValueRangeItemValue(nIndex++, nValue, szValueName, NULL, szValueName);
					continue;
				}
				for(k=m_btMinNumber; k<=m_btMaxNumber; k++)
				{
					nValue = i * 100 + j * 10 + k;
					sprintf(szValueName, "%03d", nValue);
					_SetValueRangeItemValue(nIndex++, nValue, "", NULL, szValueName);
				}
			}
		}
		ASSERT(nIndex <= m_lpValueRangeTemp->nItemCount);
		m_lpValueRangeTemp->nItemCount = nIndex;
	}
	else	// 组选号码
	{
		for(i=m_btMinNumber; i<=m_btMaxNumber; i++)
		{
			for(j=i; j<=m_btMaxNumber; j++)
			{
				if(m_btNumberCount == 2)
				{
					nValue = i * 10 + j;
					sprintf(szValueName, "%02d", nValue);
					_SetValueRangeItemValue(nIndex++, nValue, szValueName, NULL, szValueName);
					continue;
				}
				for(k=j; k<=m_btMaxNumber; k++)
				{
					nValue = i * 100 + j * 10 + k;
					sprintf(szValueName, "%03d", nValue);
					_SetValueRangeItemValue(nIndex++, nValue, "", NULL, szValueName);
				}
			}
		}
		ASSERT(nIndex <= m_lpValueRangeTemp->nItemCount);
		m_lpValueRangeTemp->nItemCount = nIndex;
	}
}

void CIndicator::_FillValueRange_PosX(DWORD dwID, BYTE btPosIndex)
{
	int nMinValue = _GetPosMinValue(btPosIndex), nMaxValue = _GetPosMaxValue(btPosIndex);
    
	if(m_btDataType & DATA_TYPE_NUM_LT)
	{
		// 乐透型号码需要固定2位
		_AutoFillValueRange_DefaultType(dwID, nMinValue, nMaxValue, 2);
	}
	else
	{
		// 自动填充其他名称
		_AutoFillValueRange_DefaultType(dwID, nMinValue, nMaxValue);
	}
}

void CIndicator::_FillValueRange_Sum()
{
	int nMinValue = 0, nMaxValue = 0;
	if(m_btNumberCount == 1)		// 只有一个开奖号码
	{
		ASSERT(m_btMaxNumber > 9);
		nMinValue = m_btMaxNumber;
		nMaxValue = m_btMinNumber;
		
		for(int i=m_btMinNumber; i<=m_btMaxNumber; i++)
		{
			int nSum = (i / 10) + (i % 10);
			if(nSum > nMaxValue) nMaxValue = nSum;
			if(nSum < nMinValue) nMinValue = nSum;
		}
	}
	else
	{
		if(m_btDataType & DATA_TYPE_REPEATABLE)	// 号码可重复的
		{
			nMinValue = m_btMinNumber * m_btNumberCount;
			nMaxValue = m_btMaxNumber * m_btNumberCount;
		}
		else	// 号码不可重复的
		{
			for(BYTE i=0; i<m_btNumberCount; i++)
			{
				nMinValue += (i + m_btMinNumber);
			}
			for(BYTE i=0; i<m_btNumberCount; i++)
			{
				nMaxValue += (m_btMaxNumber - i);
			}
		}
	}
    
	_AutoFillValueRange_DefaultType(IID_STC_SUM, nMinValue, nMaxValue);
}

void CIndicator::_FillValueRange_Avg()
{
	int nMinValue = 0, nMaxValue = 0;
	if(m_btNumberCount == 1)		// 只有一个开奖号码
	{
		ASSERT(m_btMaxNumber > 9);
		nMinValue = m_btMaxNumber;
		nMaxValue = m_btMinNumber;
		
		for(int i=m_btMinNumber; i<=m_btMaxNumber; i++)
		{
			int nAvg = ((i / 10) + (i % 10) + 1) / 2;
			if(nAvg > nMaxValue) nMaxValue = nAvg;
			if(nAvg < nMinValue) nMinValue = nAvg;
		}
		_AutoFillValueRange_DefaultType(IID_STC_AVG, nMinValue, nMaxValue);
	}
	else
	{
		if(m_btDataType & DATA_TYPE_REPEATABLE)	// 号码可重复的
		{
			nMinValue = m_btMinNumber * m_btNumberCount;
			nMaxValue = m_btMaxNumber * m_btNumberCount;
		}
		else	// 号码不可重复的
		{
			for(BYTE i=0; i<m_btNumberCount; i++)
			{
				nMinValue += (i + m_btMinNumber);
			}
			for(BYTE i=0; i<m_btNumberCount; i++)
			{
				nMaxValue += (m_btMaxNumber - i);
			}
		}
		_AutoFillValueRange_DefaultType(IID_STC_AVG, (nMinValue + m_btNumberCount / 2) / m_btNumberCount, (nMaxValue + m_btNumberCount / 2) / m_btNumberCount);
	}	
}

#define		FILLRANGE_ESPECIALSUM(btNumCountIn) \
                nSum = btValidNumbers[n[0]]; \
                for(i=1; i<btNumCountIn; i++) \
                    nSum += btValidNumbers[n[i]]; \
                ASSERT(nSum < sizeof(btSumFlags)/sizeof(btSumFlags[0])); \
                btSumFlags[nSum] = 1; \
                if(m_btNumberCount == btNumCountIn){ \
                    continue; \
                }

#define		FILLRANGE_ESPECIALSUM_NOREPEAT(btNumCountIn) \
                if(btNumberCount == btNumCountIn){ \
                    nSum = btValidNumbers[n[0]]; \
                    for(i=1; i<btNumCountIn; i++) \
                        nSum += btValidNumbers[n[i]]; \
                    ASSERT(nSum < sizeof(btSumFlags)/sizeof(btSumFlags[0])); \
                    btSumFlags[nSum] = 1; \
                    continue; \
                }

void CIndicator::_FillValueRange_EspecialSum(DWORD dwID)
{
	// 获取符合要求的号码
	BYTE btValidNumbers[100] = {0}, btValidNumsCount = 0;
	BOOL bValid = FALSE;
    int i = 0;
    
	for(i=m_btMinNumber; i<=m_btMaxNumber; i++)
	{
		bValid = FALSE;
        
		switch(dwID)
		{
            case IID_STC_ESPECIALSUM_ODD: bValid = (i % 2 == 1) ? TRUE : FALSE; break;
            case IID_STC_ESPECIALSUM_EVEN: bValid = (i % 2 == 0) ? TRUE : FALSE; break;
            case IID_STC_ESPECIALSUM_BIG: bValid = (_GetBigSmall(i) == 1) ? TRUE : FALSE; break;
            case IID_STC_ESPECIALSUM_SMALL: bValid = (_GetBigSmall(i) == 0) ? TRUE : FALSE; break;
            case IID_STC_ESPECIALSUM_PRIME: bValid = (_GetPrimeComposite(i) == 1) ? TRUE : FALSE; break;
            case IID_STC_ESPECIALSUM_COMPOSITE: bValid = (_GetPrimeComposite(i) == 0) ? TRUE : FALSE; break;
            case IID_STC_ESPECIALSUM_0: bValid = (i % 3 == 0) ? TRUE : FALSE; break;
            case IID_STC_ESPECIALSUM_1: bValid = (i % 3 == 1) ? TRUE : FALSE; break;
            case IID_STC_ESPECIALSUM_2: bValid = (i % 3 == 2) ? TRUE : FALSE; break;
            case IID_STC_ESPECIALSUM_D: bValid = (_GetDZX(i) == 2) ? TRUE : FALSE; break;
            case IID_STC_ESPECIALSUM_Z: bValid = (_GetDZX(i) == 1) ? TRUE : FALSE; break;
            case IID_STC_ESPECIALSUM_X: bValid = (_GetDZX(i) == 0) ? TRUE : FALSE; break;
		}
        
		if(bValid)
			btValidNumbers[btValidNumsCount++] = (BYTE)i;
	}
	
	int n[8] = {0}, nSum = 0;
	BYTE btSumFlags[1000] = {0};
    
	btSumFlags[0] = 1;	// 肯定有0
    
	if(m_btDataType & DATA_TYPE_REPEATABLE)	// 可重复的
	{
		for(n[0]=0; n[0]<btValidNumsCount; n[0]++)
		{
			FILLRANGE_ESPECIALSUM(1);
			for(n[1]=0; n[1]<btValidNumsCount; n[1]++)
			{
				FILLRANGE_ESPECIALSUM(2);
				for(n[2]=0; n[2]<btValidNumsCount; n[2]++)
				{
					FILLRANGE_ESPECIALSUM(3);
					for(n[3]=0; n[3]<btValidNumsCount; n[3]++)
					{
						FILLRANGE_ESPECIALSUM(4);
						for(n[4]=0; n[4]<btValidNumsCount; n[4]++)
						{
							FILLRANGE_ESPECIALSUM(5);
							for(n[5]=0; n[5]<btValidNumsCount; n[5]++)
							{
								FILLRANGE_ESPECIALSUM(6);
								for(n[6]=0; n[6]<btValidNumsCount; n[6]++)
								{
									FILLRANGE_ESPECIALSUM(7);
									for(n[7]=0; n[7]<btValidNumsCount; n[7]++)
									{
										FILLRANGE_ESPECIALSUM(8);
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else	// 不可重复的
	{
		for(BYTE btNumberCount=1; btNumberCount<=m_btNumberCount; btNumberCount++)
		{
            for(n[0]=0; n[0]<btValidNumsCount-btNumberCount+1; n[0]++)
            {
                FILLRANGE_ESPECIALSUM_NOREPEAT(1);
                for(n[1]=n[0]+1; n[1]<btValidNumsCount-btNumberCount+2; n[1]++)
                {
                    FILLRANGE_ESPECIALSUM_NOREPEAT(2);
                    for(n[2]=n[1]+1; n[2]<btValidNumsCount-btNumberCount+3; n[2]++)
                    {
                        FILLRANGE_ESPECIALSUM_NOREPEAT(3);
                        for(n[3]=n[2]+1; n[3]<btValidNumsCount-btNumberCount+4; n[3]++)
                        {
                            FILLRANGE_ESPECIALSUM_NOREPEAT(4);
                            for(n[4]=n[3]+1; n[4]<btValidNumsCount-btNumberCount+5; n[4]++)
                            {
                                FILLRANGE_ESPECIALSUM_NOREPEAT(5);
                                for(n[5]=n[4]+1; n[5]<btValidNumsCount-btNumberCount+6; n[5]++)
                                {
                                    FILLRANGE_ESPECIALSUM_NOREPEAT(6);
                                    for(n[6]=n[5]+1; n[6]<btValidNumsCount-btNumberCount+7; n[6]++)
                                    {
                                        FILLRANGE_ESPECIALSUM_NOREPEAT(7);
                                        for(n[7]=n[6]+1; n[7]<btValidNumsCount-btNumberCount+8; n[7]++)
                                        {
                                            FILLRANGE_ESPECIALSUM_NOREPEAT(8);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
		}
	}
    
	// 设置值域
	m_lpValueRangeTemp->nItemCount = 0;
	for(i=0; i<sizeof(btSumFlags)/sizeof(btSumFlags[0]); i++)
	{
		if(btSumFlags[i] == 1)
		{
			m_lpValueRangeTemp->nItemCount ++;
			_SetValueRangeItemValue(m_lpValueRangeTemp->nItemCount-1, i, NULL, NULL, NULL);
		}
	}
}

void CIndicator::_FillValueRange_TwoNums(DWORD dwID)
{
	// 设置值域
	m_lpValueRangeTemp->nItemCount = 0;
	char szColName[8] = {0};
    
	if(dwID == IID_STC_TWONUM_ANY)	// 组选二码
	{
		for(int i=m_btMinNumber; i<=m_btMaxNumber; i++)
		{
			for(int j=i; j<=m_btMaxNumber; j++)
			{
				if(!(m_btDataType & DATA_TYPE_REPEATABLE))	// 不可重复的
				{
					if(i == j)
					{
						continue;
					}
				}
				m_lpValueRangeTemp->nItemCount ++;
				sprintf(szColName, "%02d", i * 10 + j);
				_SetValueRangeItemValue(m_lpValueRangeTemp->nItemCount-1, i * 10 + j, szColName, NULL, szColName);
			}
		}
	}
	else	// 单选二码
	{
		for(int i=m_btMinNumber; i<=m_btMaxNumber; i++)
		{
			for(int j=m_btMinNumber; j<=m_btMaxNumber; j++)
			{
				if(!(m_btDataType & DATA_TYPE_REPEATABLE))	// 不可重复的
				{
					if(i == j)
					{
						continue;
					}
				}
				m_lpValueRangeTemp->nItemCount ++;
				sprintf(szColName, "%02d", i * 10 + j);
				_SetValueRangeItemValue(m_lpValueRangeTemp->nItemCount-1, i * 10 + j, szColName, NULL, szColName);
			}
		}
	}
}

void CIndicator::_FillValueRange_Order(DWORD dwID)
{
	int nMin = m_btMinNumber, nMax = m_btMaxNumber;
	int nMinMin = m_btMinNumber, nMinMid = m_btMinNumber, nMinMax = m_btMinNumber;
	int nMaxMin = m_btMaxNumber, nMaxMid = m_btMaxNumber, nMaxMax = m_btMaxNumber;
    
	if(!(m_btDataType & DATA_TYPE_REPEATABLE))	// 不可重复的
	{
		nMinMin = m_btMinNumber;
		nMinMid = m_btMinNumber + 1;
		nMinMax = m_btMinNumber + m_btNumberCount - 1;
		nMaxMin = m_btMaxNumber - m_btNumberCount + 1;
		nMaxMid = m_btMaxNumber - 1;
		nMaxMax = m_btMaxNumber;
	}
	
	switch(dwID)
	{
        case IID_STC_ORDER_MIN: nMin = nMinMin, nMax = nMaxMin; break;
        case IID_STC_ORDER_MID: nMin = nMinMid, nMax = nMaxMid; break;
        case IID_STC_ORDER_MAX: nMin = nMinMax, nMax = nMaxMax; break;
        case IID_STC_ORDER_MINADDMID: nMin = nMinMin + nMinMid, nMax = nMaxMin + nMaxMid; break;
        case IID_STC_ORDER_MINADDMAX: nMin = nMinMin + nMinMax, nMax = nMaxMin + nMaxMax; break;
        case IID_STC_ORDER_MIDADDMAX: nMin = nMinMid + nMinMax, nMax = nMaxMid + nMaxMax; break;
        case IID_STC_ORDER_MIDSUBMIN: nMin = nMinMid - nMinMin; nMax = nMaxMid - nMinMin; break;
        case IID_STC_ORDER_MAXSUBMIN: nMin = nMinMax - nMinMin; nMax = nMaxMax - nMinMin; break;
        case IID_STC_ORDER_MAXSUBMID: nMin = nMinMax - nMinMid; nMax = nMaxMax - nMinMid; break;
        default: ASSERT(FALSE); break;
	}
	
	_AutoFillValueRange_DefaultType(dwID, nMin, nMax);
}

void CIndicator::_FillValueRange_Space(DWORD dwID)
{
	int nMin = 0, nMax = 0;
    
	switch(dwID)
	{
        case IID_STC_SPACE_SIN: 
            nMin = (m_btMaxNumber - m_btMinNumber) / m_btNumberCount; // m_btNumberCount > 3 ? 2 : (m_btNumberCount > 2 ? 3 : 2);
            if((m_btMaxNumber - m_btMinNumber) % m_btNumberCount != 0)
                nMin ++;
            nMax = (_GetPosMaxValue(0, TRUE) - m_btMinNumber) + (m_btMaxNumber - _GetPosMinValue(BYTE(m_btNumberCount-1), TRUE));
            break;
        case IID_STC_SPACETAIL_SIN: nMin = 0; nMax = 9; break;
        case IID_STC_SPACE_SIN_HEAD: 
            nMin = _GetPosMinValue(0, TRUE) - m_btMinNumber;
            nMax = _GetPosMaxValue(0, TRUE) - m_btMinNumber; 
            break;
        case IID_STC_SPACE_SIN_TAIL: 
            nMin = m_btMaxNumber - _GetPosMaxValue(BYTE(m_btNumberCount-1), TRUE);
            nMax = m_btMaxNumber - _GetPosMinValue(BYTE(m_btNumberCount-1), TRUE);
            break;
        case IID_STC_SPACE_SIN_1:
            nMin = 0;
            nMax = (_GetPosMaxValue(0, TRUE) - m_btMinNumber) + (m_btMaxNumber - _GetPosMinValue(BYTE(m_btNumberCount-1), TRUE));
            break;
        case IID_STC_SPACE_SIN_2:
        case IID_STC_SPACE_SIN_3:
        case IID_STC_SPACE_SIN_4:
        case IID_STC_SPACE_SIN_5:
        case IID_STC_SPACE_SIN_6:
        case IID_STC_SPACE_SIN_7:
        case IID_STC_SPACE_SIN_8:
            if(m_btDataType & DATA_TYPE_REPEATABLE)
            {
                nMin = 0;
                nMax = m_btMaxNumber - m_btMinNumber;
            }
            else
            {
                nMin = 1;
                nMax = m_btMaxNumber - m_btMinNumber;
            }
            break;
            
        case IID_STC_SPACE_GRO:
            //		nMin = m_btNumberCount > 3 ? 2 : (m_btNumberCount > 2 ? 3 : 2);
            nMin = (m_btMaxNumber - m_btMinNumber) / m_btNumberCount; // m_btNumberCount > 3 ? 2 : (m_btNumberCount > 2 ? 3 : 2);
            if((m_btMaxNumber - m_btMinNumber) % m_btNumberCount != 0)
                nMin ++;
            nMax = (_GetPosMaxValue(0, TRUE) - m_btMinNumber) + (m_btMaxNumber - _GetPosMinValue(BYTE(m_btNumberCount-1), TRUE));
            
            if(m_btDataType & DATA_TYPE_REPEATABLE)
                nMax = m_btMaxNumber - m_btMinNumber;
            else
                nMax = m_btMaxNumber - m_btMinNumber - m_btNumberCount + 1;
            break;
        case IID_STC_SPACE_GRO_HEAD:
            nMin = _GetPosMinValue(0, FALSE) - m_btMinNumber;
            nMax = _GetPosMaxValue(0, FALSE) - m_btMinNumber; 
            break;
        case IID_STC_SPACE_GRO_TAIL:
            nMin = m_btMaxNumber - _GetPosMaxValue(BYTE(m_btNumberCount-1), FALSE);
            nMax = m_btMaxNumber - _GetPosMinValue(BYTE(m_btNumberCount-1), FALSE);
            break;
        case IID_STC_SPACE_GRO_1:
            nMin = 0;
            if(m_btDataType & DATA_TYPE_REPEATABLE)
                nMax = m_btMaxNumber - m_btMinNumber;
            else
                nMax = m_btMaxNumber - m_btMinNumber - m_btNumberCount + 1;
            break;
        case IID_STC_SPACE_GRO_2:
        case IID_STC_SPACE_GRO_3:
        case IID_STC_SPACE_GRO_4:
        case IID_STC_SPACE_GRO_5:
        case IID_STC_SPACE_GRO_6:
        case IID_STC_SPACE_GRO_7:
        case IID_STC_SPACE_GRO_8:
            if(m_btDataType & DATA_TYPE_REPEATABLE)
            {
                nMin = 0;
                nMax = m_btMaxNumber - m_btMinNumber;
            }
            else
            {
                nMin = 1;
                nMax = m_btMaxNumber - m_btMinNumber - m_btNumberCount + 2;
            }
            break;
        default: ASSERT(FALSE); break;
	}
	
	_AutoFillValueRange_DefaultType(dwID, nMin, nMax);
}

#define		CALC_MUL_NUMBER(nNC) \
if(!(m_btDataType & DATA_TYPE_REPEATABLE)) {	/* 不可重复的 */ \
if(n[nNC-2] == n[nNC-1]) continue; \
} \
if(m_btNumberCount == nNC){ \
int nTemp = n[0]; \
for(int i=1; i<nNC; i++) \
nTemp *= n[i]; \
lpFlagBuf[nTemp] = 1; \
continue; \
}

void CIndicator::_FillValueRange_Mul(DWORD /*dwIID*/)
{
	ASSERT(m_btNumberCount > 1 && m_btNumberCount < 8);
    
	int nMaxValue = (int)pow(m_btMaxNumber, m_btNumberCount) + 1, n[8] = {0};
	LPBYTE lpFlagBuf = new BYTE[nMaxValue];
	ZeroMemory(lpFlagBuf, sizeof(nMaxValue));
    
	for(n[0]=m_btMinNumber; n[0]<=m_btMaxNumber; n[0]++)
	{
		for(n[1]=m_btMinNumber; n[1]<=m_btMaxNumber; n[1]++)
		{
			CALC_MUL_NUMBER(2);
			for(n[2]=m_btMinNumber; n[2]<=m_btMaxNumber; n[2]++)
			{
				CALC_MUL_NUMBER(3);
				for(n[3]=m_btMinNumber; n[3]<=m_btMaxNumber; n[3]++)
				{
					CALC_MUL_NUMBER(4);
					for(n[4]=m_btMinNumber; n[4]<=m_btMaxNumber; n[4]++)
					{
						CALC_MUL_NUMBER(5);
						for(n[5]=m_btMinNumber; n[5]<=m_btMaxNumber; n[5]++)
						{
							CALC_MUL_NUMBER(6);
							for(n[6]=m_btMinNumber; n[6]<=m_btMaxNumber; n[6]++)
							{
								CALC_MUL_NUMBER(7);
							}
						}
					}
				}
			}
		}
	}
    
	// 各单元赋值并计算总数
	m_lpValueRangeTemp->nItemCount = 0;
	LPCDTVALUERANGEITEM lpVRItem = NULL;
    
	for(int i=0; i<nMaxValue; i++)
	{
		if(lpFlagBuf[i] == 1)
		{
			lpVRItem = &m_lpValueRangeTemp->stValueItem[m_lpValueRangeTemp->nItemCount];
			lpVRItem->nValue = i;
			if(lpVRItem->nValue > -100 && lpVRItem->nValue < 1000)
				sprintf(lpVRItem->szBallName, "%d", lpVRItem->nValue);
			else
				lpVRItem->szBallName[0] = 0;
			CHECK_STRING_STACK(lpVRItem->szBallName);
			strcpy(lpVRItem->szColName, lpVRItem->szBallName);
			sprintf(lpVRItem->szValueName, "%d", lpVRItem->nValue);
            
			m_lpValueRangeTemp->nItemCount ++;
		}
	}
    
	delete []lpFlagBuf;
}

void CIndicator::_FillValueRange_PosMulSum(DWORD dwID)
{
	int i = 0, nMin = 0, nMax = 0;
    
	if(m_btDataType & DATA_TYPE_REPEATABLE)	// 可重复的
	{
		for(i=0; i<m_btNumberCount; i++)
		{
			nMin += (m_btMinNumber * (i + 1));
			nMax += (m_btMaxNumber * (i + 1));
		}
	}
	else	// 不可重复的
	{
		if(m_btDataType & DATA_TYPE_ORDERED)	// 排序的
		{
			if(dwID == IID_STC_POSMULSUM)	// 位置积和
			{
				for(i=0; i<m_btNumberCount; i++)
				{
					nMin += ((m_btMinNumber + i) * (i + 1));
					nMax += ((m_btMaxNumber - m_btNumberCount + i + 1) * (i + 1));
				}
			}
			else	// 反位置积和
			{
				for(i=0; i<m_btNumberCount; i++)
				{
					nMin += ((m_btMinNumber + i) * (m_btNumberCount - i));
					nMax += ((m_btMaxNumber - i) * (m_btNumberCount - i));
				}
			}
		}
		else	// 非排序的
		{
			for(i=0; i<m_btNumberCount; i++)
			{
				nMin += ((m_btMinNumber + i) * (i + 1));
				nMax += ((m_btMaxNumber - m_btNumberCount + i + 1) * (i + 1));
			}
		}
	}
    
	_AutoFillValueRange_DefaultType(dwID, nMin, nMax);
}

void CIndicator::_FillValueRange_LinkCombo(DWORD /*dwIID*/)
{
	LPCDTVALUERANGEITEM lpVRItem = NULL;
	
	m_lpValueRangeTemp->nItemCount = m_btNumberCount;
	_SetValueRangeItemValue(0, 0, "0", "0", "(0)无连号");
	
	for(int i=2; i<=m_lpValueRangeTemp->nItemCount; i++)
	{
		lpVRItem = &m_lpValueRangeTemp->stValueItem[i-1];
		lpVRItem->nValue = i;
		sprintf(lpVRItem->szBallName, "%d", i);
		strcpy(lpVRItem->szColName, lpVRItem->szBallName);
		sprintf(lpVRItem->szValueName, "(%d)含有%d连号", i, i);
	}
}

void CIndicator::_FillValueRange_LinkPos(DWORD dwIID)
{
	LPCDTVALUERANGEITEM lpVRItem = NULL;
    
	m_lpValueRangeTemp->nItemCount = m_btNumberCount-(dwIID - IID_STC_LINK_POS_2);
	char szTemp[16] = {0};
	sprintf(szTemp, "(0)无%lu连号", dwIID - IID_STC_LINK_POS_2 + 2);
	_SetValueRangeItemValue(0, 0, "0", "0", szTemp);
    
	for(int i=1; i<m_lpValueRangeTemp->nItemCount; i++)
	{
		lpVRItem = &m_lpValueRangeTemp->stValueItem[i];
		lpVRItem->nValue = i;
		sprintf(lpVRItem->szBallName, "%d", i);
		strcpy(lpVRItem->szColName, lpVRItem->szBallName);
		sprintf(lpVRItem->szValueName, "(%d)第%d个号码", i, i);
	}
}

void CIndicator::_FillValueRange_SinArrangeValue(DWORD dwID, char szName[10][8], int nStateCount)
{
	int n[8] = {0}, nIndex = 0, nCount = (int)pow(nStateCount, m_btNumberCount);
	_AutoFillValueRange_DefaultType(dwID, 0, nCount - 1);
	
	for(n[0]=0; n[0]<nStateCount; n[0]++)
	{
		for(n[1]=0; n[1]<nStateCount; n[1]++)
		{
			if(m_btNumberCount == 2)
			{
				sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d：%s%s(%d%d)", nIndex, \
                        szName[n[0]], szName[n[1]], n[0], n[1]);
				nIndex++;
				continue;
			}
			for(n[2]=0; n[2]<nStateCount; n[2]++)
			{
				if(m_btNumberCount == 3)
				{
					sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d：%s%s%s(%d%d%d)", nIndex, \
                            szName[n[0]], szName[n[1]], szName[n[2]], n[0], n[1], n[2]);
					nIndex++;
					continue;
				}
				for(n[3]=0; n[3]<nStateCount; n[3]++)
				{
					if(m_btNumberCount == 4)
					{
						sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d：%s%s%s%s(%d%d%d%d)", nIndex, \
                                szName[n[0]], szName[n[1]], szName[n[2]], szName[n[3]], n[0], n[1], n[2], n[3]);
						nIndex++;
						continue;
					}
					for(n[4]=0; n[4]<nStateCount; n[4]++)
					{
						if(m_btNumberCount == 5)
						{
							sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d：%s%s%s%s%s(%d%d%d%d%d)", nIndex, \
                                    szName[n[0]], szName[n[1]], szName[n[2]], szName[n[3]], szName[n[4]], n[0], n[1], n[2], n[3], n[4]);
							nIndex++;
							continue;
						}
						for(n[5]=0; n[5]<nStateCount; n[5]++)
						{
							if(m_btNumberCount == 6)
							{
								sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d：%s%s%s%s%s%s(%d%d%d%d%d%d)", nIndex, \
                                        szName[n[0]], szName[n[1]], szName[n[2]], szName[n[3]], szName[n[4]], szName[n[5]], n[0], n[1], n[2], n[3], n[4], n[5]);
								nIndex++;
								continue;
							}
							for(n[6]=0; n[6]<nStateCount; n[6]++)
							{
								if(m_btNumberCount == 7)
								{
									sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d：%s%s%s%s%s%s%s(%d%d%d%d%d%d%d)", nIndex, \
                                            szName[n[0]], szName[n[1]], szName[n[2]], szName[n[3]], szName[n[4]], szName[n[5]], szName[n[6]], n[0], n[1], n[2], n[3], n[4], n[5], n[6]);
									nIndex++;
									continue;
								}
								for(n[7]=0; n[7]<nStateCount; n[7]++)
								{
									if(m_btNumberCount == 8)
									{
										sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d：%s%s%s%s%s%s%s%s(%d%d%d%d%d%d%d%d)", nIndex, \
                                                szName[n[0]], szName[n[1]], szName[n[2]], szName[n[3]], szName[n[4]], szName[n[5]], szName[n[6]], szName[n[7]], n[0], n[1], n[2], n[3], n[4], n[5], n[6], n[7]);
										nIndex++;
										continue;
									}
								}
							}
						}
					}
				}
			}
		}
	}
    
	if(nStateCount == 2 && m_btNumberCount == 3)	// 需要特殊处理
	{
		sprintf(m_lpValueRangeTemp->stValueItem[3].szValueName, "3：%s%s%s(100)", szName[1], szName[0], szName[0]);
		sprintf(m_lpValueRangeTemp->stValueItem[4].szValueName, "4：%s%s%s(011)", szName[0], szName[1], szName[1]);
	}
    
	ASSERT(nIndex == nCount);
}

void CIndicator::_FillValueRange_SinArrangeValue_Numbers(DWORD dwID, int nMinNumber, int nMaxNumber)
{
	int nStateCount = nMaxNumber - nMinNumber + 1;
	int n[8] = {0}, nIndex = 0, nCount = (int)pow(nStateCount, m_btNumberCount);
	_AutoFillValueRange_DefaultType(dwID, 0, nCount - 1);
	
	for(n[0]=nMinNumber; n[0]<nStateCount+nMinNumber; n[0]++)
	{
		for(n[1]=nMinNumber; n[1]<nStateCount+nMinNumber; n[1]++)
		{
			if(m_btNumberCount == 2)
			{
				sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d(%d%d)", nIndex, n[0], n[1]);
				nIndex++;
				continue;
			}
			for(n[2]=nMinNumber; n[2]<nStateCount+nMinNumber; n[2]++)
			{
				if(m_btNumberCount == 3)
				{
					sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d(%d%d%d)", nIndex, n[0], n[1], n[2]);
					nIndex++;
					continue;
				}
				for(n[3]=nMinNumber; n[3]<nStateCount+nMinNumber; n[3]++)
				{
					if(m_btNumberCount == 4)
					{
						sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d(%d%d%d%d)", nIndex, n[0], n[1], n[2], n[3]);
						nIndex++;
						continue;
					}
					for(n[4]=nMinNumber; n[4]<nStateCount+nMinNumber; n[4]++)
					{
						if(m_btNumberCount == 5)
						{
							sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d(%d%d%d%d%d)", nIndex, n[0], n[1], n[2], n[3], n[4]);
							nIndex++;
							continue;
						}
						for(n[5]=nMinNumber; n[5]<nStateCount+nMinNumber; n[5]++)
						{
							if(m_btNumberCount == 6)
							{
								sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d(%d%d%d%d%d%d)", nIndex, n[0], n[1], n[2], n[3], n[4], n[5]);
								nIndex++;
								continue;
							}
							for(n[6]=nMinNumber; n[6]<nStateCount+nMinNumber; n[6]++)
							{
								if(m_btNumberCount == 7)
								{
									sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d(%d%d%d%d%d%d%d)", nIndex, n[0], n[1], n[2], n[3], n[4], n[5], n[6]);
									nIndex++;
									continue;
								}
								for(n[7]=nMinNumber; n[7]<nStateCount+nMinNumber; n[7]++)
								{
									if(m_btNumberCount == 8)
									{
										sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d(%d%d%d%d%d%d%d%d)", nIndex, n[0], n[1], n[2], n[3], n[4], n[5], n[6], n[7]);
										nIndex++;
										continue;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	
	if(nStateCount == 2 && m_btNumberCount == 3)	// 需要特殊处理
	{
		sprintf(m_lpValueRangeTemp->stValueItem[3].szValueName, "3(%d%d%d)", nMinNumber+1, nMinNumber, nMinNumber);
		sprintf(m_lpValueRangeTemp->stValueItem[4].szValueName, "4(%d%d%d)", nMinNumber, nMinNumber+1, nMinNumber+1);
	}
    
	ASSERT(nIndex == nCount);
}

void CIndicator::_FillValueRange_GroArrangeValue(DWORD dwID, char szName[10][8], int nStateCount)
{
	int n[8] = {0}, nIndex = 0, nCount = (int)pow(nStateCount, m_btNumberCount);
	_AutoFillValueRange_DefaultType(dwID, 0, nCount - 1);
	
	for(n[0]=0; n[0]<nStateCount; n[0]++)
	{
		for(n[1]=n[0]; n[1]<nStateCount; n[1]++)
		{
			if(m_btNumberCount == 2)
			{
				sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d：%s%s(%d%d)", nIndex, \
                        szName[n[0]], szName[n[1]], n[0], n[1]);
				nIndex++;
				continue;
			}
			for(n[2]=n[1]; n[2]<nStateCount; n[2]++)
			{
				if(m_btNumberCount == 3)
				{
					sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d：%s%s%s(%d%d%d)", nIndex, \
                            szName[n[0]], szName[n[1]], szName[n[2]], n[0], n[1], n[2]);
					nIndex++;
					continue;
				}
				for(n[3]=n[2]; n[3]<nStateCount; n[3]++)
				{
					if(m_btNumberCount == 4)
					{
						sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d：%s%s%s%s(%d%d%d%d)", nIndex, \
                                szName[n[0]], szName[n[1]], szName[n[2]], szName[n[3]], n[0], n[1], n[2], n[3]);
						nIndex++;
						continue;
					}
					for(n[4]=n[3]; n[4]<nStateCount; n[4]++)
					{
						if(m_btNumberCount == 5)
						{
							sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d：%s%s%s%s%s(%d%d%d%d%d)", nIndex, \
                                    szName[n[0]], szName[n[1]], szName[n[2]], szName[n[3]], szName[n[4]], n[0], n[1], n[2], n[3], n[4]);
							nIndex++;
							continue;
						}
						for(n[5]=n[4]; n[5]<nStateCount; n[5]++)
						{
							if(m_btNumberCount == 6)
							{
								sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d：%s%s%s%s%s%s(%d%d%d%d%d%d)", nIndex, \
                                        szName[n[0]], szName[n[1]], szName[n[2]], szName[n[3]], szName[n[4]], szName[n[5]], n[0], n[1], n[2], n[3], n[4], n[5]);
								nIndex++;
								continue;
							}
							for(n[6]=n[5]; n[6]<nStateCount; n[6]++)
							{
								if(m_btNumberCount == 7)
								{
									sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d：%s%s%s%s%s%s%s(%d%d%d%d%d%d%d)", nIndex, \
                                            szName[n[0]], szName[n[1]], szName[n[2]], szName[n[3]], szName[n[4]], szName[n[5]], szName[n[6]], n[0], n[1], n[2], n[3], n[4], n[5], n[6]);
									nIndex++;
									continue;
								}
								for(n[7]=n[6]; n[7]<nStateCount; n[7]++)
								{
									if(m_btNumberCount == 8)
									{
										sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d：%s%s%s%s%s%s%s%s(%d%d%d%d%d%d%d%d)", nIndex, \
                                                szName[n[0]], szName[n[1]], szName[n[2]], szName[n[3]], szName[n[4]], szName[n[5]], szName[n[6]], szName[n[7]], n[0], n[1], n[2], n[3], n[4], n[5], n[6], n[7]);
										nIndex++;
										continue;
									}
								}
							}
						}
					}
				}
			}
		}
	}
    
	ASSERT(nIndex <= nCount);
	m_lpValueRangeTemp->nItemCount = nIndex;
}

void CIndicator::_FillValueRange_GroArrangeValue_Numbers(DWORD dwID, int nMinNumber, int nMaxNumber)
{
	int nStateCount = nMaxNumber - nMinNumber + 1;
	int n[8] = {0}, nIndex = 0, nCount = (int)pow(nStateCount, m_btNumberCount);
	_AutoFillValueRange_DefaultType(dwID, 0, nCount - 1);
	
	for(n[0]=nMinNumber; n[0]<nStateCount+nMinNumber; n[0]++)
	{
		for(n[1]=n[0]; n[1]<nStateCount+nMinNumber; n[1]++)
		{
			if(m_btNumberCount == 2)
			{
				sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d(%d%d)", nIndex, n[0], n[1]);
				nIndex++;
				continue;
			}
			for(n[2]=n[1]; n[2]<nStateCount+nMinNumber; n[2]++)
			{
				if(m_btNumberCount == 3)
				{
					sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d(%d%d%d)", nIndex, n[0], n[1], n[2]);
					nIndex++;
					continue;
				}
				for(n[3]=n[2]; n[3]<nStateCount+nMinNumber; n[3]++)
				{
					if(m_btNumberCount == 4)
					{
						sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d(%d%d%d%d)", nIndex, n[0], n[1], n[2], n[3]);
						nIndex++;
						continue;
					}
					for(n[4]=n[3]; n[4]<nStateCount+nMinNumber; n[4]++)
					{
						if(m_btNumberCount == 5)
						{
							sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d(%d%d%d%d%d)", nIndex, n[0], n[1], n[2], n[3], n[4]);
							nIndex++;
							continue;
						}
						for(n[5]=n[4]; n[5]<nStateCount+nMinNumber; n[5]++)
						{
							if(m_btNumberCount == 6)
							{
								sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d(%d%d%d%d%d%d)", nIndex, n[0], n[1], n[2], n[3], n[4], n[5]);
								nIndex++;
								continue;
							}
							for(n[6]=n[5]; n[6]<nStateCount+nMinNumber; n[6]++)
							{
								if(m_btNumberCount == 7)
								{
									sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d(%d%d%d%d%d%d%d)", nIndex, n[0], n[1], n[2], n[3], n[4], n[5], n[6]);
									nIndex++;
									continue;
								}
								for(n[7]=n[6]; n[7]<nStateCount+nMinNumber; n[7]++)
								{
									if(m_btNumberCount == 8)
									{
										sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d(%d%d%d%d%d%d%d%d)", nIndex, n[0], n[1], n[2], n[3], n[4], n[5], n[6], n[7]);
										nIndex++;
										continue;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	
	ASSERT(nIndex <= nCount);
	m_lpValueRangeTemp->nItemCount = nIndex;
}


void CIndicator::_FillValueRange_3StateShape(DWORD dwID, char szName[3][8])
{
	_AutoFillValueRange_DefaultType(dwID, 0, 2);
    
	for(int i=0; i<3; i++)
	{
		m_lpValueRangeTemp->stValueItem[i].szColName[0] = char('A' + i);
		strcpy(m_lpValueRangeTemp->stValueItem[i].szBallName, m_lpValueRangeTemp->stValueItem[i].szColName);
	}
	sprintf(m_lpValueRangeTemp->stValueItem[0].szValueName, "A（全%s、全%s、全%s）", szName[0], szName[1], szName[2]);
	sprintf(m_lpValueRangeTemp->stValueItem[1].szValueName, "B（%s、%s、%s有一个不出）", szName[0], szName[1], szName[2]);
	sprintf(m_lpValueRangeTemp->stValueItem[2].szValueName, "C（%s、%s、%s各出一个）", szName[0], szName[1], szName[2]);
}

void CIndicator::_FillValueRange_CountArrangeValue(DWORD dwID, char szName[3][8], BOOL bAscOrder)
{
	int i, n[3] = {0}, nIndex = 0, nMaxCount = 256;
	_AutoFillValueRange_DefaultType(dwID, 0, nMaxCount);
    
	char szTemp[16] = {0};
    
	if(bAscOrder)
	{
		for(n[0]=m_btNumberCount; n[0]>=0; n[0]--)
		{
			for(n[1]=m_btNumberCount-n[0]; n[1]>=0; n[1]--)
			{
				n[2]=m_btNumberCount-n[0]-n[1];
				sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d：", nIndex);
				for(i=0; i<3; i++)
				{
					if(n[i] > 0)
					{
						sprintf(szTemp, "%d%s", n[i], szName[i]);
						strcat(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, szTemp);
					}
				}
				sprintf(szTemp, "(%d%d%d)", n[0], n[1], n[2]);
				strcat(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, szTemp);
				CHECK_STRING_STACK(m_lpValueRangeTemp->stValueItem[nIndex].szValueName);
				
				nIndex ++;
			}
		}
	}
	else
	{
		for(n[0]=0; n[0]<=m_btNumberCount; n[0]++)
		{
			for(n[1]=0; n[1]<=m_btNumberCount-n[0]; n[1]++)
			{
				n[2]=m_btNumberCount-n[0]-n[1];
				sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d：", nIndex);
				for(i=0; i<3; i++)
				{
					if(n[i] > 0)
					{
						sprintf(szTemp, "%d%s", n[i], szName[i]);
						strcat(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, szTemp);
					}
				}
				sprintf(szTemp, "(%d%d%d)", n[0], n[1], n[2]);
				strcat(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, szTemp);
				CHECK_STRING_STACK(m_lpValueRangeTemp->stValueItem[nIndex].szValueName);
				
				nIndex ++;
			}
		}
	}
    
	ASSERT(nIndex < nMaxCount);
	m_lpValueRangeTemp->nItemCount = nIndex;
}

#define FILLRANGE_COUNTARRANGEVALUE_NUMBERS(btNumCountIn) \
if(btItemCount == btNumCountIn) { \
n[btNumCountIn-1] = m_btNumberCount; \
for(i=0; i<btNumCountIn-1; i++) n[btNumCountIn-1] -= n[i]; \
sprintf(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, "%d(", nIndex); \
for(i=0; i<btNumCountIn; i++) { \
sprintf(szTemp, "%d", n[i]); \
strcat(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, szTemp); \
} \
strcat(m_lpValueRangeTemp->stValueItem[nIndex].szValueName, ")"); \
CHECK_STRING_STACK(m_lpValueRangeTemp->stValueItem[nIndex].szValueName); \
nIndex ++; \
continue; \
}

void CIndicator::_FillValueRange_CountArrangeValue_Numbers(DWORD dwID, BYTE btItemCount)
{
	ASSERT(m_btNumberCount > 1);
    
	int i, n[9] = {0}, nIndex = 0, nMaxCount = 1024;
	_AutoFillValueRange_DefaultType(dwID, 0, nMaxCount);
	
	char szTemp[16] = {0};
	
	for(n[0]=m_btNumberCount; n[0]>=0; n[0]--)
	{
		FILLRANGE_COUNTARRANGEVALUE_NUMBERS(2);
		for(n[1]=m_btNumberCount-n[0]; n[1]>=0; n[1]--)
		{
			FILLRANGE_COUNTARRANGEVALUE_NUMBERS(3);
			for(n[2]=m_btNumberCount-n[0]-n[1]; n[2]>=0; n[2]--)
			{
				FILLRANGE_COUNTARRANGEVALUE_NUMBERS(4);
				for(n[3]=m_btNumberCount-n[0]-n[1]-n[2]; n[3]>=0; n[3]--)
				{
					FILLRANGE_COUNTARRANGEVALUE_NUMBERS(5);
					for(n[4]=m_btNumberCount-n[0]-n[1]-n[2]-n[3]; n[4]>=0; n[4]--)
					{
						FILLRANGE_COUNTARRANGEVALUE_NUMBERS(6);
						for(n[5]=m_btNumberCount-n[0]-n[1]-n[2]-n[3]-n[4]; n[5]>=0; n[5]--)
						{
							FILLRANGE_COUNTARRANGEVALUE_NUMBERS(7);
							for(n[6]=m_btNumberCount-n[0]-n[1]-n[2]-n[3]-n[4]-n[5]; n[6]>=0; n[6]--)
							{
								FILLRANGE_COUNTARRANGEVALUE_NUMBERS(8);
								for(n[7]=m_btNumberCount-n[0]-n[1]-n[2]-n[3]-n[4]-n[5]-n[6]; n[7]>=0; n[7]--)
								{
									FILLRANGE_COUNTARRANGEVALUE_NUMBERS(9);
								}
							}
						}
					}
				}
			}
		}
	}
    
	ASSERT(nIndex < nMaxCount);
	m_lpValueRangeTemp->nItemCount = nIndex;
}

void CIndicator::_FillValueRange_Arrange(DWORD /*dwID*/, char szNames[32][8], int nValues[32], BYTE btItemCount)
{
	ASSERT(btItemCount > 1);
    
	// 排列只保存排列数字的范围值
	m_lpValueRangeTemp->nItemCount = btItemCount;
	for(BYTE i=0; i<btItemCount; i++)
	{
		m_lpValueRangeTemp->stValueItem[i].nValue = nValues[i];
		strcpy(m_lpValueRangeTemp->stValueItem[i].szColName, szNames[i]);
		strcpy(m_lpValueRangeTemp->stValueItem[i].szBallName, szNames[i]);
		strcpy(m_lpValueRangeTemp->stValueItem[i].szValueName, szNames[i]);
	}
}

void CIndicator::_FillValueRange_Arrange_Numbers(DWORD /*dwID*/, int nMinValue, int nMaxValue)
{
	ASSERT(nMaxValue > nMinValue);
	
	m_lpValueRangeTemp->nItemCount = nMaxValue - nMinValue + 1;
	for(BYTE i=0; i<m_lpValueRangeTemp->nItemCount; i++)
	{
		m_lpValueRangeTemp->stValueItem[i].nValue = nMinValue + i;
		sprintf(m_lpValueRangeTemp->stValueItem[i].szColName, "%d", m_lpValueRangeTemp->stValueItem[i].nValue);
		strcpy(m_lpValueRangeTemp->stValueItem[i].szBallName, m_lpValueRangeTemp->stValueItem[i].szColName);
		strcpy(m_lpValueRangeTemp->stValueItem[i].szValueName, m_lpValueRangeTemp->stValueItem[i].szColName);
	}
}

void CIndicator::_FillValueRange_TwoPos(DWORD dwID)
{
	int nTypeIndex = (dwID - IID_STC_TWOPOS_BEGIN) / 100;
	DWORD dwIID = dwID - nTypeIndex * 100;
    
	switch(nTypeIndex)
	{
        case TPT_SUM: _FillValueRange_TwoPos_Sum(dwID, dwIID); return;
        case TPT_SUB_ASC:
        case TPT_SUB_DESC:
        case TPT_SUB: _FillValueRange_TwoPos_Sub(dwID, dwIID); return ;
        case TPT_SUBABS:
        case TPT_CIRSUB_ASC: 
        case TPT_CIRSUB_DESC: 
        case TPT_CIRSUB: _FillValueRange_TwoPos_CirSub(dwID, dwIID); return; 
        case TPT_MUL: _FillValueRange_TwoPos_Mul(dwID, dwIID); return;
            
        case TPT_SUM_TAIL:
        case TPT_SUB_TAIL:
        case TPT_SUBABS_TAIL:
        case TPT_CIRSUB_ASC_TAIL:
        case TPT_CIRSUB_DESC_TAIL:
        case TPT_CIRSUB_TAIL:
        case TPT_MUL_TAIL: 
        case TPT_SUMSUB:
        case TPT_SUMCIRSUB:
            _AutoFillValueRange_DefaultType(dwID, 0, 9); break;
	}
}

void CIndicator::_FillValueRange_TwoPos_Sum(DWORD dwID, DWORD dwIID)
{
	int nMin = 0, nMax = 9;
    
	switch(dwIID)
	{
        case IID_STC_TWOPOS_ANY:
        case IID_STC_TWOPOS_MIN:
        case IID_STC_TWOPOS_MID:
        case IID_STC_TWOPOS_MAX:
        case IID_STC_TWOPOS_ARR_SIN:
        case IID_STC_TWOPOS_ARR_GRO:
		{
			if(m_btDataType & DATA_TYPE_REPEATABLE)	// 可重复
			{
				nMin = m_btMinNumber + m_btMinNumber;
				nMax = m_btMaxNumber + m_btMaxNumber;
			}
			else	// 不可重复
			{
				nMin = m_btMinNumber + m_btMinNumber + 1;
				nMax = m_btMaxNumber + m_btMaxNumber - 1;
				switch(dwIID)
				{
                    case IID_STC_TWOPOS_MIN: nMax = 2 * (m_btMaxNumber - m_btNumberCount + 1) + 1; break;
                    case IID_STC_TWOPOS_MID: nMin ++; nMax --; break;
                    case IID_STC_TWOPOS_MAX: nMin = 2 * (m_btMinNumber + m_btNumberCount - 1) - 1; break;
				}
			}
            
			switch(dwIID)
			{
                case IID_STC_TWOPOS_ARR_SIN:
                case IID_STC_TWOPOS_ARR_GRO: _FillValueRange_Arrange_Numbers(dwID, nMin, nMax); break;
                default: _AutoFillValueRange_DefaultType(dwID, nMin, nMax); break;
			}
			return ;
		}
	}
    
	// 指定两位关系
	BYTE btPos1 = BYTE(((dwID - IID_STC_TWOPOS_BEGIN) / 10) % 10), btPos2 = BYTE((dwID - IID_STC_TWOPOS_BEGIN) % 10);
	if(dwIID == IID_STC_TWOPOS_HEADTAIL)	// 首尾
	{
		btPos1 = 0;
		btPos2 = (BYTE)(m_btNumberCount - 1);
	}
    
	nMin = _GetPosMinValue(btPos1) + _GetPosMinValue(btPos2);
	nMax = _GetPosMaxValue(btPos1) + _GetPosMaxValue(btPos2);
	if(!(m_btDataType & DATA_TYPE_REPEATABLE) && !(m_btDataType & DATA_TYPE_ORDERED))
	{
		nMin += 1;
		nMax -= 1;
	}
    
	_AutoFillValueRange_DefaultType(dwID, nMin, nMax);
}

void CIndicator::_FillValueRange_TwoPos_Sub(DWORD dwID, DWORD dwIID)
{
	int nMin = 0, nMax = 9;
	
	switch(dwIID)
	{
        case IID_STC_TWOPOS_ANY:
        case IID_STC_TWOPOS_MIN:
        case IID_STC_TWOPOS_MID:
        case IID_STC_TWOPOS_MAX:
        case IID_STC_TWOPOS_ARR_SIN:
        case IID_STC_TWOPOS_ARR_GRO:
		{
			nMax = m_btMaxNumber - m_btMinNumber;
			if(m_btDataType & DATA_TYPE_REPEATABLE)	// 可重复
			{
				nMin = m_btMinNumber - m_btMaxNumber;
			}
			else	// 不可重复
			{
				if(m_btDataType & DATA_TYPE_ORDERED)	// 排序的
				{
					nMin = 1;
					switch(dwIID)
					{
                        case IID_STC_TWOPOS_MIN: nMax = (m_btMaxNumber - 1) / (m_btNumberCount - 1); break;
                        case IID_STC_TWOPOS_MID: nMin ++; nMax --; break;
                        case IID_STC_TWOPOS_MAX: nMin = m_btNumberCount - 1; break;
					}
				}
				else	// 未排序的
				{
					nMin = m_btMinNumber - m_btMaxNumber;
					switch(dwIID)
					{
                        case IID_STC_TWOPOS_MIN: nMax = (m_btMaxNumber - 1) / (m_btNumberCount - 1); break;
                        case IID_STC_TWOPOS_MID: nMin ++; nMax --; break;
                        case IID_STC_TWOPOS_MAX: nMin = 0 - (m_btMaxNumber - 1) / (m_btNumberCount - 1); break;
					}
				}
			}
			
			switch(dwIID)
			{
                case IID_STC_TWOPOS_ARR_SIN:
                case IID_STC_TWOPOS_ARR_GRO: _FillValueRange_Arrange_Numbers(dwID, nMin, nMax); return;
			}
		} break;
        default:	// 指定两位关系
		{
			BYTE btPos1 = BYTE(((dwID - IID_STC_TWOPOS_BEGIN) / 10) % 10), btPos2 = BYTE((dwID - IID_STC_TWOPOS_BEGIN) % 10);
			if(dwIID == IID_STC_TWOPOS_HEADTAIL)	// 首尾
			{
				btPos1 = 0;
				btPos2 = (BYTE)(m_btNumberCount - 1);
			}
			
			if(m_btDataType & DATA_TYPE_ORDERED)	// 排序的
			{
				nMin = _GetPosMinValue(btPos2) - _GetPosMinValue(btPos1);
				nMax = _GetPosMaxValue(btPos2) - _GetPosMinValue(btPos1);
			}
			else	// 未排序的
			{
				nMax = m_btMaxNumber - m_btMinNumber;
				nMin = m_btMinNumber - m_btMaxNumber;
			}
		}
	}
    
	if(!(m_btDataType & DATA_TYPE_REPEATABLE))
	{
		// 不重复的没有0
		m_lpValueRangeTemp->nItemCount = 1;
		for(int i=nMin; i<=nMax; i++)
		{
			if(i == 0) continue;	// 没有0
			_SetValueRangeItemValue(m_lpValueRangeTemp->nItemCount-1, i);
			m_lpValueRangeTemp->nItemCount ++;
		}
		m_lpValueRangeTemp->nItemCount --;
	}
	else
	{
		_AutoFillValueRange_DefaultType(dwID, nMin, nMax);
	}
}

void CIndicator::_FillValueRange_TwoPos_CirSub(DWORD dwID, DWORD dwIID)
{
	int nMin = 0, nMax = 9;
	
	switch(dwIID)
	{
        case IID_STC_TWOPOS_ANY:
        case IID_STC_TWOPOS_MIN:
        case IID_STC_TWOPOS_MID:
        case IID_STC_TWOPOS_MAX:
        case IID_STC_TWOPOS_ARR_SIN:
        case IID_STC_TWOPOS_ARR_GRO:
		{
			if(m_btDataType & DATA_TYPE_REPEATABLE)	// 可重复
			{
				nMin = 0;
				nMax = m_btMaxNumber - m_btMinNumber;
			}
			else	// 不可重复
			{
				nMin = 1;
				nMax = m_btMaxNumber - m_btMinNumber;
			}
			
			switch(dwIID)
			{
                case IID_STC_TWOPOS_ARR_SIN:
                case IID_STC_TWOPOS_ARR_GRO: _FillValueRange_Arrange_Numbers(dwID, nMin, nMax); break;
                default: _AutoFillValueRange_DefaultType(dwID, nMin, nMax); break;
			}
			return ;
		}
	}
	
	// 指定两位关系
	BYTE btPos1 = BYTE(((dwID - IID_STC_TWOPOS_BEGIN) / 10) % 10), btPos2 = BYTE((dwID - IID_STC_TWOPOS_BEGIN) % 10);
	if(dwIID == IID_STC_TWOPOS_HEADTAIL)	// 首尾
	{
		btPos1 = 0;
		btPos2 = (BYTE)(m_btNumberCount - 1);
	}
	
	nMax = m_btMaxNumber - m_btMinNumber; nMin = (m_btDataType & DATA_TYPE_REPEATABLE) ? 0 : 1;
	if((m_btDataType & DATA_TYPE_ORDERED) && !(m_btDataType & DATA_TYPE_REPEATABLE))
	{
		nMin = m_btMaxNumber - (_GetPosMaxValue(btPos2) - _GetPosMinValue(btPos1));
		nMax = m_btMaxNumber - (btPos2 - btPos1);
	}
    
	_AutoFillValueRange_DefaultType(dwID, nMin, nMax);
}

void CIndicator::_FillValueRange_TwoPos_Mul(DWORD /*dwID*/, DWORD /*dwIID*/)
{
/*	int nMax = 9;
	CByteArray cFlagArray;
	if(m_btDataType & DATA_TYPE_REPEATABLE)
	{
		nMax = m_btMaxNumber * m_btMaxNumber + 1;
		cFlagArray.SetSize(nMax);
		for(int i=m_btMinNumber; i<=m_btMaxNumber; i++)
		{
			for(int j=m_btMinNumber; j<=m_btMaxNumber; j++)
			{
				cFlagArray.SetAt(theApp.m_btProductNums[i] * theApp.m_btProductNums[j], 1);
			}
		}
	}
	else
	{
		ASSERT(FALSE);
	}
    
	// 设置值域
	m_lpValueRangeTemp->nItemCount = 0;
	for(int i=0; i<cFlagArray.GetSize(); i++)
	{
		if(cFlagArray[i] == 1)
		{
			m_lpValueRangeTemp->nItemCount ++;
			_SetValueRangeItemValue(m_lpValueRangeTemp->nItemCount-1, i, NULL, NULL, NULL);
		}
	}*/
}


void CIndicator::_FillValueRange_Remainder(DWORD dwID)
{
	int nDivisorIndex = (dwID - IID_STC_REMM_BEGIN) / 100;
    
	switch(dwID - nDivisorIndex * 100)
	{
        case IID_STC_REMM_POS_1:
        case IID_STC_REMM_POS_2:
        case IID_STC_REMM_POS_3:
        case IID_STC_REMM_POS_4:
        case IID_STC_REMM_POS_5:
        case IID_STC_REMM_POS_6:
        case IID_STC_REMM_POS_7:
        case IID_STC_REMM_POS_8:
        case IID_STC_REMM_POS_ANY: _AutoFillValueRange_DefaultType(dwID, 0, nDivisorIndex + 1); break;
        case IID_STC_REMM_SUM:  _AutoFillValueRange_DefaultType(dwID, 0, (nDivisorIndex + 1) * m_btNumberCount); break;
        case IID_STC_REMM_SUMTAIL: _AutoFillValueRange_DefaultType(dwID, 0, 9); break;
        case IID_STC_REMM_ARR_SIN:
        case IID_STC_REMM_ARR_GRO:
        case IID_STC_REMM_ARR_COUNT: _FillValueRange_Arrange_Numbers(dwID, 0, nDivisorIndex + 1); break;
        case IID_STC_REMM_ARR_SIN_VALUE: _FillValueRange_SinArrangeValue_Numbers(dwID, 0, nDivisorIndex + 1); break;
        case IID_STC_REMM_ARR_GRO_VALUE: _FillValueRange_GroArrangeValue_Numbers(dwID, 0, nDivisorIndex + 1); break;
        case IID_STC_REMM_ARR_COUNT_VALUE: _FillValueRange_CountArrangeValue_Numbers(dwID, BYTE(nDivisorIndex + 2)); break;
        case IID_STC_REMM_MAXLINK: _AutoFillValueRange_DefaultType(dwID, 1, (nDivisorIndex + 2) > m_btNumberCount ? m_btNumberCount : (nDivisorIndex + 2)); break;
        case IID_STC_REMM_MAXNONE: _AutoFillValueRange_DefaultType(dwID, (nDivisorIndex + 2) / (m_btNumberCount + 1), nDivisorIndex + 1); break;
        case IID_STC_REMM_COUNT_NONE: _AutoFillValueRange_DefaultType(dwID, nDivisorIndex + 2 - m_btNumberCount, nDivisorIndex + 1); break;
        case IID_STC_REMM_COUNT_MATCH: _AutoFillValueRange_DefaultType(dwID, 1, fmin(nDivisorIndex + 2, m_btNumberCount)); break;
        case IID_STC_REMM_SHAPE: 
		{
			char szNames[3][8] = {"余0", "余1", "余2"};
			_FillValueRange_3StateShape(dwID, szNames);
		} break;
        default: _AutoFillValueRange_DefaultType(dwID, 0, m_btNumberCount); break; // IID_STC_REMM_COUNT_N
	}
}

void CIndicator::_FillValueRange_DataArea(WORD wUseType, DWORD dwCustomData)
{
	LPDATAAREA lpDataArea = (LPDATAAREA)dwCustomData;
	if(lpDataArea == NULL)
	{
		ASSERT(FALSE);
		return ;
	}
    
	switch(wUseType)
	{
        case CIDAUT_POS_1:
        case CIDAUT_POS_2:
        case CIDAUT_POS_3:
        case CIDAUT_POS_4:
        case CIDAUT_POS_5:
        case CIDAUT_POS_6:
        case CIDAUT_POS_7:
        case CIDAUT_POS_8:
        case CIDAUT_POS_ANY:
		{
			char szName[8] = {0};
			m_lpValueRangeTemp->nItemCount = lpDataArea->btAreaCount;
			for(int i=0; i<m_lpValueRangeTemp->nItemCount; i++)
			{
				sprintf(szName, "%d", i + lpDataArea->btSubAreaMinValue);
				_SetValueRangeItemValue(i, i + lpDataArea->btSubAreaMinValue, szName, szName, lpDataArea->szSubAreaNames[i]);
			}
		} break;
        case CIDAUT_ARR_SIN:
        case CIDAUT_ARR_GRO:
        case CIDAUT_ARR_COUNT: _FillValueRange_Arrange_Numbers(0, lpDataArea->btSubAreaMinValue, lpDataArea->btSubAreaMinValue + lpDataArea->btAreaCount - 1); break;
        case CIDAUT_ARR_SIN_VALUE: _FillValueRange_SinArrangeValue_Numbers(0, lpDataArea->btSubAreaMinValue, lpDataArea->btSubAreaMinValue + lpDataArea->btAreaCount - 1); break;
        case CIDAUT_ARR_GRO_VALUE: _FillValueRange_GroArrangeValue_Numbers(0, lpDataArea->btSubAreaMinValue, lpDataArea->btSubAreaMinValue + lpDataArea->btAreaCount - 1); break;
        case CIDAUT_ARR_COUNT_VALUE: _FillValueRange_CountArrangeValue_Numbers(0, (BYTE)lpDataArea->btAreaCount); break;
        case CIDAUT_MAXLINK: _AutoFillValueRange_DefaultType(0, 0, lpDataArea->btAreaCount > m_btNumberCount ? m_btNumberCount : lpDataArea->btAreaCount); break;
        case CIDAUT_MAXNONE: _AutoFillValueRange_DefaultType(0, lpDataArea->btAreaCount / (m_btNumberCount + 1), lpDataArea->btAreaCount - 1); break;
        case CIDAUT_COUNT_NONE: 
		{
			int nMin = lpDataArea->btAreaCount - 1 - m_btNumberCount;
			if(nMin < 0) nMin = 0;
			_AutoFillValueRange_DefaultType(0, nMin, lpDataArea->btAreaCount - 1);
		} break;
        case CIDAUT_COUNT_MATCH: _AutoFillValueRange_DefaultType(0, 1, fmin(m_btNumberCount, lpDataArea->btAreaCount)); break;
        case CIDAUT_SHAPE: 
		{
			char szNames[3][8] = {"1区", "2区", "3区"};
			_FillValueRange_3StateShape(0, szNames);
		} break;
        default: _AutoFillValueRange_DefaultType(0, 0, m_btNumberCount); break; // CIDAUT_COUNT_N
	}
}

void CIndicator::_FillValueRange_PrevData(DWORD dwID, int nTypeIndex)
{
	int nMaxValue = 9, nMinValue = 0;
	
	switch(dwID)
	{
        case IID_DNC_PREVDATA_POS_1:
        case IID_DNC_PREVDATA_POS_2:
        case IID_DNC_PREVDATA_POS_3:
        case IID_DNC_PREVDATA_POS_4:
        case IID_DNC_PREVDATA_POS_5:
        case IID_DNC_PREVDATA_POS_6:
        case IID_DNC_PREVDATA_POS_7:
        case IID_DNC_PREVDATA_POS_8:
        case IID_DNC_PREVDATA_ANY:
        case IID_DNC_PREVDATA_MIN:
        case IID_DNC_PREVDATA_MID:
        case IID_DNC_PREVDATA_MAX:
        case IID_DNC_PREVDATA_SUM:
            if((!(m_btDataType & DATA_TYPE_REPEATABLE)) && (m_btDataType & DATA_TYPE_ORDERED))
            {
                // 如果是不可重复且已排序的数据，如双色球等，则最大值计算如下
                switch(nTypeIndex)
                {
                    case PDT_DIS: nMaxValue = (m_btMaxNumber - m_btNumberCount + 1) - m_btMinNumber; break;
                    case PDT_CIRDIS: nMaxValue = m_btMaxNumber - m_btMinNumber; break;
                    case PDT_SUB: nMaxValue = (m_btMaxNumber - m_btNumberCount + 1) - m_btMinNumber, nMinValue = 0 - nMaxValue; break;
                    case PDT_SUM: 
                    {
                        if(dwID == IID_DNC_PREVDATA_SUM)
                        {
                            nMinValue = nMaxValue = 0;
                            for(BYTE i=0; i<m_btNumberCount; i++)
                            {
                                nMinValue += 2 * _GetPosMinValue(i);
                                nMaxValue += 2 * _GetPosMaxValue(i);
                            }
                        }
                        else if(dwID >= IID_DNC_PREVDATA_ANY && dwID <= IID_DNC_PREVDATA_MAX)
                        {
                            nMinValue = 2 * _GetPosMinValue(0);
                            nMaxValue = 2 * _GetPosMaxValue((BYTE)(m_btNumberCount-1));
                        }
                        else
                        {
                            BYTE nPos = BYTE(dwID - IID_DNC_PREVDATA_POS_1);
                            nMinValue = 2 * _GetPosMinValue(nPos);
                            nMaxValue = 2 * _GetPosMaxValue(nPos);
                        }
                    }break;
                    case PDT_AXISSUB: nMaxValue = (m_btMaxNumber - m_btMinNumber + 1) / 2; nMinValue = 0 - nMaxValue; break;
                    case PDT_AXISDIS: nMaxValue = (m_btMaxNumber - m_btMinNumber + 1) / 2; break;
                }
            }
            else	// 可重复或未排序
            {
                switch(nTypeIndex)
                {
                    case PDT_DIS:
                    case PDT_CIRDIS: nMaxValue = m_btMaxNumber - m_btMinNumber; break;
                    case PDT_SUB: nMinValue = m_btMinNumber - m_btMaxNumber, nMaxValue = m_btMaxNumber - m_btMinNumber; break;
                    case PDT_SUM: nMinValue = m_btMinNumber * 2, nMaxValue = m_btMaxNumber * 2; break;
                    case PDT_AXISSUB: nMaxValue = (m_btMaxNumber - m_btMinNumber + 1) / 2; nMinValue = 0 - nMaxValue; break;
                    case PDT_AXISDIS: nMaxValue = (m_btMaxNumber - m_btMinNumber + 1) / 2; break;
                }
            }
            if(dwID == IID_DNC_PREVDATA_SUM)
            {
                nMinValue *= m_btNumberCount;
                nMaxValue *= m_btNumberCount;
            }
            break;
        case IID_DNC_PREVDATA_SUMTAIL: nMaxValue = 9; break;
        case IID_DNC_PREVDATA_ARR_SIN:
        case IID_DNC_PREVDATA_ARR_GRO:
		{
			_FillValueRange_Arrange_Numbers(dwID, 0, 9);
		}break;
	}
	
	// 自动填充其他名称
	_AutoFillValueRange_DefaultType(dwID, nMinValue, nMaxValue);
}

void CIndicator::_FillValueRange_3State(DWORD dwID)
{
	int nTypeIndex = (dwID - IID_DNC_XXX_BEGIN) / 100;
	switch(dwID - nTypeIndex * 100)
	{
        case IID_DNC_XXX_POS_1:
        case IID_DNC_XXX_POS_2:
        case IID_DNC_XXX_POS_3:
        case IID_DNC_XXX_POS_4:
        case IID_DNC_XXX_POS_5:
        case IID_DNC_XXX_POS_6:
        case IID_DNC_XXX_POS_7:
        case IID_DNC_XXX_POS_8:
		{
			m_lpValueRangeTemp->nItemCount = 3;
			for(int i=0; i<3; i++)
			{
				m_lpValueRangeTemp->stValueItem[i].nValue = i;
				m_lpValueRangeTemp->stValueItem[i].szBallName[0] = BALLNAMES_3STATE_DYNAMIC[nTypeIndex][i];
				m_lpValueRangeTemp->stValueItem[i].szBallName[1] = 0;
				memcpy(m_lpValueRangeTemp->stValueItem[i].szColName, &NAMES_3STATE_DYNAMIC[nTypeIndex][i*2], 2);
				m_lpValueRangeTemp->stValueItem[i].szColName[2] = 0;
				sprintf(m_lpValueRangeTemp->stValueItem[i].szValueName, "(%hhd)%s码", m_lpValueRangeTemp->stValueItem[i].szBallName[0], m_lpValueRangeTemp->stValueItem[i].szColName);
			}
		} break;
        case IID_DNC_XXX_COUNT_0:
        case IID_DNC_XXX_COUNT_1:
        case IID_DNC_XXX_COUNT_2: _AutoFillValueRange_DefaultType(dwID, 0, m_btNumberCount); break;
        case IID_DNC_XXX_SHAPE: 
		{
			char szNames[3][8] = {"升码", "平码", "降码"};
			for(int i=0; i<3; i++)
			{
				memcpy(szNames[i], &NAMES_3STATE_DYNAMIC[nTypeIndex][i*2], 2);
			}
			_FillValueRange_3StateShape(dwID, szNames);
		} break;
            
        case IID_DNC_XXX_ARR_SIN:
        case IID_DNC_XXX_ARR_GRO:
		{
			char szNames[3][8] = {"升", "平", "降"};
			for(int i=0; i<3; i++)
			{
				memcpy(szNames[i], &NAMES_3STATE_DYNAMIC[nTypeIndex][i*2], 2);
			}
			int nValues[3] = {0, 1, 2};
			_FillValueRange_Arrange(dwID, szNames, nValues, 3);
		} break;
        case IID_DNC_XXX_ARR_SIN_VALUE:
		{
			char szNames[3][8] = {"升", "平", "降"};
			for(int i=0; i<3; i++)
			{
				memcpy(szNames[i], &NAMES_3STATE_DYNAMIC[nTypeIndex][i*2], 2);
			}
			_FillValueRange_SinArrangeValue(dwID, szNames);
		}break;
        case IID_DNC_XXX_ARR_GRO_VALUE:
		{
			char szNames[3][8] = {"升", "平", "降"};
			for(int i=0; i<3; i++)
			{
				memcpy(szNames[i], &NAMES_3STATE_DYNAMIC[nTypeIndex][i*2], 2);
			}
			_FillValueRange_GroArrangeValue(dwID, szNames);
		}break;
            
        case IID_DNC_XXX_ARR_COUNT: _FillValueRange_Arrange_Numbers(dwID, 0, 2); break;
        case IID_DNC_XXX_ARR_COUNT_VALUE:
		{
			char szNames[3][8] = {"升", "平", "降"};
			for(int i=0; i<3; i++)
			{
				memcpy(szNames[i], &NAMES_3STATE_DYNAMIC[nTypeIndex][i*2], 2);
			}
			_FillValueRange_CountArrangeValue(dwID, szNames, TRUE); 
		}break;
	}
}

void CIndicator::_FillValueRange_Skip_Area(DWORD dwIID)
{
	int i = 0;
	char szColName[8] = {0}, szDetails[32] = {0};
    
	switch(dwIID)
	{
        case IID_DNC_SKIPGRO_SUM_AREA:
            _AutoFillValueRange_DefaultType(dwIID, 0, 30);
            for(i=0; i<30; i++)
            {
                sprintf(szColName, "%d", i);
                sprintf(szDetails, "%d(%d)", i, i);
                _SetValueRangeItemValue(i, i, szColName, NULL, szDetails);
            }
            _SetValueRangeItemValue(30, 30, "A", "A", "A(大于等于30)");
            break;
            
        case IID_DNC_SKIPSIN_SUM_AREA:
		{
			int nMax = (m_btNumberCount == 1) ? 50 : m_btNumberCount * 20;
			if(nMax > 90) nMax = 90;
			_AutoFillValueRange_DefaultType(dwIID, 0, nMax);
			for(i=0; i<nMax; i++)
			{
				sprintf(szColName, "%d", i);
				sprintf(szDetails, "%d(%d)", i, i);
				_SetValueRangeItemValue(i, i, szColName, NULL, szDetails);
			}
			sprintf(szDetails, "A(大于等于%d)", nMax);
			_SetValueRangeItemValue(nMax, nMax, "A", "A", szDetails);
		}
            break;
            
        case IID_DNC_SKIPGRO_SUM_AREA10:
            m_lpValueRangeTemp->nItemCount = 10;
            m_lpValueRangeTemp->nMaxValue = 9;
            m_lpValueRangeTemp->nMinValue = 0;
            if(g_pIData->GetNumberCount() >= 4)
            {
                for(i=0; i<9; i++)
                {
                    sprintf(szColName, "%d", i);
                    sprintf(szDetails, "%d：%d分区(%d-%d)", i, i, i*2, i*2+1);
                    _SetValueRangeItemValue(i, i, szColName, NULL, szDetails);
                }
                _SetValueRangeItemValue(9, 9, "9", "9", "9：9分区(大于等于18)");
            }
            else
            {
                for(i=0; i<9; i++)
                {
                    sprintf(szColName, "%d", i);
                    sprintf(szDetails, "%d：%d分区(%d-%d)", i, i, i*3, i*3+2);
                    _SetValueRangeItemValue(i, i, szColName, NULL, szDetails);
                }
                _SetValueRangeItemValue(9, 9, "9", "9", "9：9分区(大于等于27)");
            }
            break;
            
        case IID_DNC_SKIPSIN_SUM_AREA10:
            m_lpValueRangeTemp->nItemCount = 10;
            if(g_pIData->GetNumberCount() >= 4)
            {
                m_lpValueRangeTemp->nMaxValue = 10;
                m_lpValueRangeTemp->nMinValue = 1;
                _SetValueRangeItemValue(0, 1, "1", "1", "1：1分区(0-19)");
                _SetValueRangeItemValue(1, 2, "2", "2", "2：2分区(20-29)");
                _SetValueRangeItemValue(2, 3, "3", "3", "3：3分区(30-39)");
                _SetValueRangeItemValue(3, 4, "4", "4", "4：4分区(40-49)");
                _SetValueRangeItemValue(4, 5, "5", "5", "5：5分区(50-59)");
                _SetValueRangeItemValue(5, 6, "6", "6", "6：6分区(60-69)");
                _SetValueRangeItemValue(6, 7, "7", "7", "7：7分区(70-79)");
                _SetValueRangeItemValue(7, 8, "8", "8", "8：8分区(80-89)");
                _SetValueRangeItemValue(8, 9, "9", "9", "9：9分区(90-99)");
                _SetValueRangeItemValue(9, 10, "A", "A", "A：10分区(大于等于100)");
            }
            else
            {
                m_lpValueRangeTemp->nMaxValue = 9;
                m_lpValueRangeTemp->nMinValue = 0;
                for(i=0; i<9; i++)
                {
                    sprintf(szColName, "%d", i);
                    sprintf(szDetails, "%d：%d分区(%d-%d)", i, i, i*6, i*6+5);
                    _SetValueRangeItemValue(i, i, szColName, NULL, szDetails);
                }
                _SetValueRangeItemValue(9, 9, "9", "9", "9：9分区(大于等于54)");
            }
            break;
            
        default: ASSERT(FALSE); break;
	}
}

