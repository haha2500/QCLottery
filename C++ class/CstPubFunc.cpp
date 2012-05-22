//
//  CCstPubFunc.cpp
//  QCLottery
//
//  Created by 武 孙 on 12-5-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "CstPubFunc.h"
#include "Def_ConditionValue.h"
#include <math.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

const BYTE BYTEBITS[] = {0X80, 0X40, 0X20, 0X10, 0X08, 0X04, 0X02, 0X01};

BYTE CCstPubFunc::m_btQuickSortBufLen = 0;

CCstPubFunc::CCstPubFunc()
{
	m_szPublicSaveFilenameInfo[0] = m_szLastErrorString[0] = 0;
}

CCstPubFunc::~CCstPubFunc()
{
    
}

/////////////////////////////////////////////////////////////////////////////////////////
LPCSTR CCstPubFunc::GetLastErrorString()
{
	if(m_szLastErrorString[0] == 0)
	{
		strcpy(m_szLastErrorString, "成功！");
	}
    
	return m_szLastErrorString;
}

BOOL CCstPubFunc::TextToIntArray(LPCSTR lpszText, CDWordArray &cValueArrayOut, int nMinValue, int nMaxValue, BOOL bFill0Prev)
{
	if(nMaxValue <= 1000)
	{
		// 文本转换为位标志
		DWORD dwBitCount = 0;
		LPBYTE lpBitBuf = TextToBitBuf(lpszText, dwBitCount, nMinValue, nMaxValue, bFill0Prev);
		if(lpBitBuf == NULL)
		{
			return FALSE;
		}
        
		// 位标志信息转换为整形数组
		_BitBufToIntArray(lpBitBuf, dwBitCount, nMinValue, cValueArrayOut);
        
		DELETE_BUFFER(lpBitBuf);
        
		return TRUE;
	}
	else
	{
		return _TextToIntArray(lpszText, cValueArrayOut, nMinValue, nMaxValue, bFill0Prev);
	}
}

LPBYTE CCstPubFunc::TextToBitBuf(LPCSTR lpszText, DWORD &dwBitCountOut, int nMinValue, int nMaxValue, BOOL bFill0Prev, BOOL bGroData)
{
	ASSERT(lpszText != NULL);
	ASSERT(nMaxValue >= nMinValue);
	
	// 预处理
	int nLen = strlen(lpszText);
	if(nLen <= 0)
	{
		strcpy(m_szLastErrorString, "没有输入有效数字！");
		return NULL;
	}
    
	// 获取字节和位个数
	dwBitCountOut = nMaxValue - nMinValue + 1;
	ASSERT(dwBitCountOut <= 99999999);
    
	int nByteCount = dwBitCountOut / 8;
	if(dwBitCountOut % 8 > 0)
		nByteCount ++;
    
	// 创建标志位缓冲区
	LPBYTE lpBitBuf = new BYTE[nByteCount];
	if(lpBitBuf == NULL)
	{
		strcpy(m_szLastErrorString, "创建标志位缓冲区失败！");
		return NULL;
	}
	ZeroMemory(lpBitBuf, nByteCount);
	
	// 开始分析
	int i = 0, j = 0, nValueMin = 0, nValueMax = 0;
	BOOL bHasValueMin = FALSE, bHasValueMax = FALSE, bIsNegativeMin = FALSE, bIsNegativeMax = FALSE;
	BOOL bSingleNum = (nMinValue >= -9 && nMaxValue <= 9) ? TRUE : FALSE;
    
	if(!bSingleNum && !bFill0Prev)	// 多个数字算一个值
	{
		if(lpszText[0] == '0')
		{
			// 如果第一位为0，则需要特殊处理，将0后的所有连续的数字都按单个计算
			if(!_SetBitValue(lpBitBuf, 0, nMinValue, nMaxValue, bFill0Prev, bGroData))
			{
				DELETE_BUFFER(lpBitBuf);
				return NULL;
			}
			bHasValueMin = TRUE;
			nMinValue = 0;
			for(i=1; i<nLen; i++)
			{
				if(lpszText[i] < '0' || lpszText[i] > '9')		// 非数字
				{
					break;	
				}
				nValueMin = lpszText[i] - '0';
				bHasValueMin = TRUE;
				if(!_SetBitValue(lpBitBuf, nValueMin, nMinValue, nMaxValue, bFill0Prev, bGroData))
				{
					DELETE_BUFFER(lpBitBuf);
					return NULL;
				}
				bHasValueMin = TRUE;
                //				nMinValue = nValueMin;
			}
		}
	}
    
	// 继续处理通常的信息
	for(i=i; i<nLen; i++)
	{
		if(lpszText[i] >= '0' && lpszText[i] <= '9')		// 是数字
		{
			if(bSingleNum)	// 如果是单个数字算一个值
			{
				nValueMin = lpszText[i] - '0';
				if(bIsNegativeMin)	// 负数
					nValueMin = 0 - nValueMin;
				if(!_SetBitValue(lpBitBuf, nValueMin, nMinValue, nMaxValue, bFill0Prev, bGroData))
				{
					DELETE_BUFFER(lpBitBuf);
					return NULL;
				}
				bIsNegativeMin = FALSE;
			}
			else
			{
				nValueMin = nValueMin * 10 + (lpszText[i] - '0');
			}
			bHasValueMin = TRUE;
		}
		else if(lpszText[i] == '-')		// 范围符号或者负号
		{
			if(bHasValueMin)	// 有最小值，则肯定是范围符，则获取范围的最大值
			{
				nValueMax = 0;
				bHasValueMax = FALSE;
				bIsNegativeMax = FALSE;
                
				for(j=i+1; j<nLen; j++)
				{
					if(lpszText[j] >= '0' && lpszText[j] <= '9')		// 是数字
					{
						bHasValueMax = TRUE;
						if(bSingleNum)	// 如果是单个数字算一个值
						{
							nValueMax = lpszText[j] - '0';
							break;
						}
						else
						{
							nValueMax = nValueMax * 10 + (lpszText[j] - '0');
						}
					}
					else if(lpszText[j] == '-')		// 负号
					{
						if(bIsNegativeMax)	// 当前已经是负数，则格式错误
						{
							strcpy(m_szLastErrorString, "输入的数据格式有错误！");
							DELETE_BUFFER(lpBitBuf);
							return NULL;
						}
						bIsNegativeMax = TRUE;		// 负数
					}
					else	// 其他符号都作为分隔符，则是一个值的完成
					{
						if(bHasValueMax)
						{
							break;
						}
					}
				}
                
				// 处理最大值
				if(!bHasValueMax)
				{
					strcpy(m_szLastErrorString, "使用'-'符号时，后面必须有有效数字！");
					DELETE_BUFFER(lpBitBuf);
					return NULL;
				}
				// 负数处理
				if(bIsNegativeMax) nValueMax = 0 - nValueMax;
				if(bIsNegativeMin) nValueMin = 0 - nValueMin;
                
				if(nValueMin >= nValueMax)
				{
					strcpy(m_szLastErrorString, "使用'-'符号指定范围时，前面的数字必须小于后面的数字！");
					DELETE_BUFFER(lpBitBuf);
					return NULL;
				}
                
				if(nValueMin < nMinValue || nValueMax > nMaxValue)
				{
					if(bFill0Prev)
					{
						char szTemp[16] = {0};
						sprintf(szTemp, "%d", nMaxValue);
						int nLen = strlen(szTemp);
						sprintf(m_szLastErrorString, "输入的值必须在 %0*d 到 %0*d 之间！", nLen, nLen, nMinValue, nMaxValue);
					}
					else
					{
						sprintf(m_szLastErrorString, "输入的值必须在 %d 到 %d 之间！", nMinValue, nMaxValue);
					}
					DELETE_BUFFER(lpBitBuf);
					return NULL;
				}
                
				// 添加范围中的所有数值
				for(int k=nValueMin; k<=nValueMax; k++)
					_SetBitValue(lpBitBuf, k, nMinValue, nMaxValue, bFill0Prev, bGroData);
                
				nValueMax = nValueMin = 0;
				bHasValueMax = bHasValueMin = FALSE;
				bIsNegativeMax = bIsNegativeMin = FALSE;
				i = j;	// 重置下标
			}
			else	// 以前无值，则是负号
			{
				if(bIsNegativeMin) // 当前已经是负数，则格式错误
				{
					strcpy(m_szLastErrorString, "输入的数据格式有错误！");
					DELETE_BUFFER(lpBitBuf);
					return NULL;
				}
				bIsNegativeMin = TRUE;		// 负数
			}
		}
		else	// 其他符号都作为分隔符，则是一个值的完成
		{
			if(!bSingleNum && bHasValueMin)
			{
				if(bIsNegativeMin)		// 负数
				{
					nValueMin = 0 - nValueMin;
				}
				if(!_SetBitValue(lpBitBuf, nValueMin, nMinValue, nMaxValue, bFill0Prev, bGroData))
				{
					DELETE_BUFFER(lpBitBuf);
					return NULL;
				}
			}
			nValueMin = 0;
			bHasValueMin = FALSE;
			bIsNegativeMin = FALSE;
		}
	}
    
	if(bIsNegativeMin && !bHasValueMin)
	{
		strcpy(m_szLastErrorString, "使用'-'符号时，后面必须有有效数字！");
		DELETE_BUFFER(lpBitBuf);
		return NULL;
	}
	if(bHasValueMin)
	{
		if(bIsNegativeMin) nValueMin = 0 - nValueMin;
		if(!_SetBitValue(lpBitBuf, nValueMin, nMinValue, nMaxValue, bFill0Prev, bGroData))
		{
			DELETE_BUFFER(lpBitBuf);
			return NULL;
		}
	}
    
	BOOL bHasValue = FALSE;
	for(i=0; i<nByteCount; i++)
	{
		if(lpBitBuf[i] != 0)
		{
			bHasValue = TRUE;
			break;
		}
	}
    
	if(!bHasValue)
	{
		strcpy(m_szLastErrorString, "您没有输入有效数字！");
		DELETE_BUFFER(lpBitBuf);
		return NULL;
	}
    
	return lpBitBuf;
}

LPCSTR CCstPubFunc::IntArrayToText(CDWordArray &cValueArray, BOOL bNoSeparator, int nFixLen)
{
	return IntBufToText((const int *)cValueArray.GetData(), cValueArray.GetSize(), bNoSeparator, nFixLen);
}

LPCSTR CCstPubFunc::IntBufToText(const int *pValueBuf, const DWORD dwValueCount, BOOL bNoSeparator, int nFixLen)
{
	if(pValueBuf == NULL || dwValueCount == 0)
	{
		return "";
	}
    
	CDWordArray cValueArray;
	cValueArray.SetSize(dwValueCount);
	for(DWORD i=0; i<dwValueCount; i++)
	{
		cValueArray.SetAt(i, pValueBuf[i]);
	}
    
	// 排序
	qsort(cValueArray.GetData(), cValueArray.GetSize(), sizeof(int), _Sort_INT_Asc);
	
	// 去重复
	for(int i=dwValueCount-1; i>0; i--)
	{
		if(cValueArray.GetAt(i) == cValueArray.GetAt(i-1))
		{
			cValueArray.RemoveAt(i);
		}
	}
    
	return _OrderedIntArrayToText(cValueArray, bNoSeparator, nFixLen);
}

LPCSTR CCstPubFunc::BitBufToText(const LPBYTE lpBitBuf, const DWORD dwBitCount, int nMinValue, BOOL bNoSeparator, BOOL bFill0Prev)
{
	if(lpBitBuf == NULL || dwBitCount == 0)
	{
		return "";
	}
    
	CDWordArray cValueArray;
    
	// 位标志信息转换为整形数组
	_BitBufToIntArray(lpBitBuf, dwBitCount, nMinValue, cValueArray);
	
	int nFill0Count = 0;
	if(bFill0Prev)
	{
		char szTemp[16] = {0};
		sprintf(szTemp, "%lu", nMinValue + dwBitCount - 1);
		nFill0Count = strlen(szTemp);
	}
    
	return _OrderedIntArrayToText(cValueArray, bNoSeparator, nFill0Count);
}

BOOL CCstPubFunc::CheckBitBufItem(const LPBYTE lpBitBuf, const int nItemIndex)
{
	ASSERT(lpBitBuf != NULL);
    
	int nByteIndex = nItemIndex / 8, nBitIndex = nItemIndex % 8;
	
	if(lpBitBuf[nByteIndex] & BYTEBITS[nBitIndex])
	{
		return TRUE;
	}
    
	return FALSE;
}

void CCstPubFunc::SetBitBufItem(LPBYTE lpBitBuf, const int nItemIndex, BOOL bValid)
{
	ASSERT(lpBitBuf != NULL && nItemIndex >= 0);
    
	int nByteIndex = nItemIndex / 8, nBitIndex = nItemIndex % 8;
	
	if(bValid)
	{
		lpBitBuf[nByteIndex] |= BYTEBITS[nBitIndex];
	}
	else
	{
		lpBitBuf[nByteIndex] &= ~BYTEBITS[nBitIndex];
	}
}

int CCstPubFunc::QuckFind_Half(const int *pValueBuf, const DWORD dwValueCount, int nFindValue, BOOL bValueSortAsc)
{
	ASSERT(pValueBuf != NULL && dwValueCount > 0);
    
#ifdef _DEBUG	// 确保是排序的，且没有重复的
	{
		for(DWORD i=0; i<dwValueCount-1; i++)
		{
			if(bValueSortAsc)
			{
				ASSERT(pValueBuf[i] <= pValueBuf[i + 1]);
			}
			else
			{
				ASSERT(pValueBuf[i] >= pValueBuf[i + 1]);
			}
		}
	}
#endif
    
	if(dwValueCount < 5)		// 5个值以内则使用顺序查找
	{
		for(DWORD i=0; i<dwValueCount; i++)
		{
			if(nFindValue == pValueBuf[i])
				return i;	// find
		}
        
		return -1;	// not find
	}
    
	// 折半查找
	int nMid = dwValueCount / 2, nFirst = 0, nLast = dwValueCount;
	
	if(bValueSortAsc)		// 正向折半查找
	{
		while(nFirst <= nLast && nMid < (int)dwValueCount)
		{
			if(nFindValue == pValueBuf[nMid])
			{
				return nMid;		// find
			}
			else if(nFindValue < pValueBuf[nMid])		// 如果小于当前元素值，则往前折半查找
			{
				nLast = nMid - 1;
			}
			else							// 如果大于当前元素值，则往后折半查找
			{
				nFirst = nMid + 1;
			}
			nMid = (nFirst + nLast) / 2;
		}
	}
	else		// 反向折半查找
	{
		while(nFirst <= nLast && nMid < (int)dwValueCount)
		{
			if(nFindValue == pValueBuf[nMid])
			{
				return nMid;		// find
			}
			else if(nFindValue > pValueBuf[nMid])	// 如果大于当前元素值，则往前折半查找
			{
				nLast = nMid - 1;
			}
			else									// 如果小于当前元素值，则往后折半查找
			{
				nFirst = nMid + 1;
			}
			nMid = (nFirst + nLast) / 2;
		}
	}
	
	return -1;		// not find
}

int CCstPubFunc::QuickSort(LPVOID lpValueBuf, int nValueCount, BYTE btValueType, BOOL bAscOrder, BOOL bRemoveSame)
{
	int nCompareValueSize = 0;
    
	switch(btValueType)
	{
        case QSVT_BYTE:
            nCompareValueSize = sizeof(BYTE);
            if(bAscOrder)
                qsort(lpValueBuf, nValueCount, nCompareValueSize, _Sort_BYTE_Asc);
            else
                qsort(lpValueBuf, nValueCount, nCompareValueSize, _Sort_BYTE_Desc);
            break;
            
        case QSVT_WORD:
            nCompareValueSize = sizeof(WORD);
            if(bAscOrder)
                qsort(lpValueBuf, nValueCount, nCompareValueSize, _Sort_WORD_Asc);
            else
                qsort(lpValueBuf, nValueCount, nCompareValueSize, _Sort_WORD_Desc);
            break;
            
        case QSVT_DWORD:
            nCompareValueSize = sizeof(DWORD);
            if(bAscOrder)
                qsort(lpValueBuf, nValueCount, nCompareValueSize, _Sort_DWORD_Asc);
            else
                qsort(lpValueBuf, nValueCount, nCompareValueSize, _Sort_DWORD_Desc);
            break;
            
        case QSVT_INT:
            nCompareValueSize = sizeof(int);
            if(bAscOrder)
                qsort(lpValueBuf, nValueCount, nCompareValueSize, _Sort_INT_Asc);
            else
                qsort(lpValueBuf, nValueCount, nCompareValueSize, _Sort_INT_Desc);
            break;
            
        default:
		{
			if(btValueType > QSVT_BUF_LEN_0 && btValueType <= QSVT_BUF_LEN_MAX)
			{
				nCompareValueSize = btValueType - QSVT_BUF_LEN_0;
				m_btQuickSortBufLen = BYTE(nCompareValueSize);
				if(bAscOrder)
					qsort(lpValueBuf, nValueCount, nCompareValueSize, _Sort_BUFFER_Asc);
				else
					qsort(lpValueBuf, nValueCount, nCompareValueSize, _Sort_BUFFER_Desc);
			}
			else
			{
				ASSERT(FALSE);
			}
		} break;
	}
    
	if(bRemoveSame)		// 去重复
	{
		nValueCount = _RemoveSameValue((LPBYTE)lpValueBuf, nValueCount, nCompareValueSize);
	}
    
	return nValueCount;
}

int CCstPubFunc::GetArrangeComboValue(int m, int n)
{
	int nValue = m;
	if(n < 0)	// 排列
	{
		for(int i=m-1; i>1; i--)
		{
			nValue *= i;
		}
	}
	else	// 组合
	{
		if(m < 0 || m < n)
		{
			return 0;
		}
        
		if(n == 0 || m == n)
		{
			return 1;
		}
        
		for(int i=1; i<n; i++)
		{
			// 分开乘除，是为了防止连续相乘时溢出导致结果不正确
			nValue *= (m - i);
			nValue /= (i + 1);
		}
	}
	
	return nValue;	
}

#define	CALCGROUPARRAYCOUNT(btNC) \
            if(btNumberCount == BYTE(btNC)) { \
                nCount ++; \
                continue; \
            }

int CCstPubFunc::GetArrangeItemCount(int nArrValueCount, BOOL bGroupArray, BYTE btNumberCount, BOOL bArrValueRepeatable)
{
	if(btNumberCount == 0)
	{
		btNumberCount = g_pIData->GetNumberCount();
	}
	ASSERT(btNumberCount > 0 && btNumberCount <= 10);
    
	if(!bGroupArray)	// 单选排列
	{
		if(bArrValueRepeatable)		// 可重复
		{
			return (int)pow(nArrValueCount, btNumberCount);
		}
		else						// 不可重复
		{
			int nCount = nArrValueCount;
			for(int i=1; i<btNumberCount; i++)
			{
				nCount *= (nArrValueCount - i);
			}
			return nCount;
		}
	}
    
	// 以下为组选排列或者个数排列
	int n[10] = {0}, nCount = 0;
    
	if(bArrValueRepeatable)		// 可重复
	{
		for(n[0]=0; n[0]<nArrValueCount; n[0]++)
		{
			CALCGROUPARRAYCOUNT(1);
			for(n[1]=n[0]; n[1]<nArrValueCount; n[1]++)
			{
				CALCGROUPARRAYCOUNT(2);
				for(n[2]=n[1]; n[2]<nArrValueCount; n[2]++)
				{
					CALCGROUPARRAYCOUNT(3);
					for(n[3]=n[2]; n[3]<nArrValueCount; n[3]++)
					{
						CALCGROUPARRAYCOUNT(4);
						for(n[4]=n[3]; n[4]<nArrValueCount; n[4]++)
						{
							CALCGROUPARRAYCOUNT(5);
							for(n[5]=n[4]; n[5]<nArrValueCount; n[5]++)
							{
								CALCGROUPARRAYCOUNT(6);
								for(n[6]=n[5]; n[6]<nArrValueCount; n[6]++)
								{
									CALCGROUPARRAYCOUNT(7);
									for(n[7]=n[6]; n[7]<nArrValueCount; n[7]++)
									{
										CALCGROUPARRAYCOUNT(8);
										for(n[8]=n[7]; n[8]<nArrValueCount; n[8]++)
										{
											CALCGROUPARRAYCOUNT(9);
											for(n[9]=n[8]; n[9]<nArrValueCount; n[9]++)
											{
												CALCGROUPARRAYCOUNT(10);
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
	}
	else		// 不可重复
	{
		for(n[0]=0; n[0]<=nArrValueCount-btNumberCount; n[0]++)
		{
			CALCGROUPARRAYCOUNT(1);
			for(n[1]=n[0]+1; n[1]<=nArrValueCount-btNumberCount+1; n[1]++)
			{
				CALCGROUPARRAYCOUNT(2);
				for(n[2]=n[1]+1; n[2]<=nArrValueCount-btNumberCount+2; n[2]++)
				{
					CALCGROUPARRAYCOUNT(3);
					for(n[3]=n[2]+1; n[3]<=nArrValueCount-btNumberCount+3; n[3]++)
					{
						CALCGROUPARRAYCOUNT(4);
						for(n[4]=n[3]+1; n[4]<=nArrValueCount-btNumberCount+4; n[4]++)
						{
							CALCGROUPARRAYCOUNT(5);
							for(n[5]=n[4]+1; n[5]<=nArrValueCount-btNumberCount+5; n[5]++)
							{
								CALCGROUPARRAYCOUNT(6);
								for(n[6]=n[5]+1; n[6]<=nArrValueCount-btNumberCount+6; n[6]++)
								{
									CALCGROUPARRAYCOUNT(7);
									for(n[7]=n[6]+1; n[7]<=nArrValueCount-btNumberCount+7; n[7]++)
									{
										CALCGROUPARRAYCOUNT(8);
										for(n[8]=n[7]+1; n[8]<=nArrValueCount-btNumberCount+8; n[8]++)
										{
											CALCGROUPARRAYCOUNT(9);
											for(n[9]=n[8]+1; n[9]<=nArrValueCount-btNumberCount+9; n[9]++)
											{
												CALCGROUPARRAYCOUNT(10);
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
	}
    
    
	return nCount;
}

const char POSNAME[][16] = {"第一位", "第二位", "第三位", "第四位", "第五位", "第六位", "第七位", "第八位"};
const char POSSHORTNAME[][6] = {"一", "二", "三", "四", "五", "六", "七", "八"};

// 获取位置名称，nPos为-1则表示整个开奖号码，bShortName是否获取简称
LPCSTR CCstPubFunc::GetPosName(int nPos, BOOL bShortName, BYTE btDataSource)
{
	if(nPos == -1)
	{
		return bShortName ? "号码" : "开奖号码";
	}
    
	ASSERT(nPos >= 0 && nPos < g_pIData->GetNumberCount(btDataSource));
	
	if(g_pIData->GetMaxNumber(btDataSource) > 9)
	{
		if(g_pIData->GetNumberCount(btDataSource) == 1)
		{
			return "号码"; 
		}
		else
		{
			return bShortName ? POSSHORTNAME[nPos] : POSNAME[nPos];
		}
	}
    
	// 以下是小于10的数字型
	switch(g_pIData->GetNumberCount(btDataSource))
	{
        case 1:
		{
			return "号码"; 
		} break;
        case 2: 
		{
			if(nPos == 0)
			{
				return bShortName ? "十" : "十位";
			}
			else if(nPos == 1)
			{
				return bShortName ? "个" : "个位";
			}
		} break;
        case 3: 
		{
			if(nPos == 0)
			{
				return bShortName ? "百" : "百位";
			}
			else if(nPos == 1)
			{
				return bShortName ? "十" : "十位";
			}
			else if(nPos == 2)
			{
				return bShortName ? "个" : "个位";
			}
		} break;
        default:
		{
			return bShortName ? POSSHORTNAME[nPos] : POSNAME[nPos];
		} break;
	}
	return NULL;
}

/////////////////////////////////////////////////////////////////////////////////////////
#pragma mark -
int CCstPubFunc::_Sort_BYTE_Asc(const void *arg1, const void *arg2)
{
	if((*(BYTE *)arg1) < (*(BYTE *)arg2))
		return -1;
	else if((*(BYTE *)arg1) > (*(BYTE *)arg2))
		return 1;
	
	return 0;
}

int CCstPubFunc::_Sort_BYTE_Desc(const void *arg1, const void *arg2)
{
	if((*(BYTE *)arg1) < (*(BYTE *)arg2))
		return 1;
	else if((*(BYTE *)arg1) > (*(BYTE *)arg2))
		return -1;
	
	return 0;
}

int CCstPubFunc::_Sort_WORD_Asc(const void *arg1, const void *arg2)
{
	if((*(WORD *)arg1) < (*(WORD *)arg2))
		return -1;
	else if((*(WORD *)arg1) > (*(WORD *)arg2))
		return 1;
	
	return 0;
}

int CCstPubFunc::_Sort_WORD_Desc(const void *arg1, const void *arg2)
{
	if((*(WORD *)arg1) < (*(WORD *)arg2))
		return 1;
	else if((*(WORD *)arg1) > (*(WORD *)arg2))
		return -1;
	
	return 0;
}

int CCstPubFunc::_Sort_DWORD_Asc(const void *arg1, const void *arg2)
{
	if((*(DWORD *)arg1) < (*(DWORD *)arg2))
		return -1;
	else if((*(DWORD *)arg1) > (*(DWORD *)arg2))
		return 1;
	
	return 0;
}

int CCstPubFunc::_Sort_DWORD_Desc(const void *arg1, const void *arg2)
{
	if((*(DWORD *)arg1) < (*(DWORD *)arg2))
		return 1;
	else if((*(DWORD *)arg1) > (*(DWORD *)arg2))
		return -1;
	
	return 0;
}

int CCstPubFunc::_Sort_INT_Asc(const void *arg1, const void *arg2)
{
	int nCompValue = (*(int *)arg1) - (*(int *)arg2);
	
	if(nCompValue < 0)
		return -1;
	else if(nCompValue > 0)
		return 1;
	
	return 0;
}

int CCstPubFunc::_Sort_INT_Desc(const void *arg1, const void *arg2)
{
	int nCompValue = (*(int *)arg1) - (*(int *)arg2);
	
	if(nCompValue < 0)
		return 1;
	else if(nCompValue > 0)
		return -1;
	
	return 0;
}

int CCstPubFunc::_Sort_BUFFER_Asc(const void *arg1, const void *arg2)
{
	return memcmp(arg1, arg2, m_btQuickSortBufLen);
}

int CCstPubFunc::_Sort_BUFFER_Desc(const void *arg1, const void *arg2)
{
	return memcmp(arg2, arg1, m_btQuickSortBufLen);
}

void CCstPubFunc::_BitBufToIntArray(LPBYTE lpBitBuf, DWORD dwBitCount, int nMinValue, CDWordArray &cValueArrayOut)
{
	ASSERT(lpBitBuf != NULL);
    
	DWORD dwCount = 0;
	
	for(DWORD i=0; i<dwBitCount; i++)
	{
		if(CheckBitBufItem(lpBitBuf, i))
			dwCount ++;
	}
	
	ASSERT(dwCount > 0);
	
	cValueArrayOut.SetSize(dwCount);
	dwCount = 0;
	for(int i=0; i<dwBitCount; i++)
	{
		if(CheckBitBufItem(lpBitBuf, i))
			cValueArrayOut.SetAt(dwCount++, i + nMinValue);
	}
}

LPCSTR CCstPubFunc::_OrderedIntArrayToText(CDWordArray &cValueArray, BOOL bNoSeparator, int nFill0Count)
{
	int i = 0, j = 0, nLinkCount = 0, nCount = cValueArray.GetSize();
    char szTemp[32] = {0};
    
	ASSERT(nCount > 0);
	
	// 开始分析数据并建立文本串
	sprintf(szTemp, "%0*d", nFill0Count, (int)cValueArray.GetAt(0));
    m_strBufToText = szTemp;

	for(i=1; i<nCount; i++)
	{
		nLinkCount = 0;
		for(j=i; j<nCount; j++)
		{
			// 查询连续个数
			if((int)cValueArray.GetAt(j) - (int)cValueArray.GetAt(j-1) == 1)
			{
				nLinkCount ++;	// 与前一个相连
			}
			else	// 不相连
			{
				break;
			}
		}
		
		if(nLinkCount > 1)
		{
			// 设置连接符
			sprintf(szTemp, "-%0*d", nFill0Count, (int)cValueArray.GetAt(j-1));
            
			m_strBufToText += szTemp;
			i += nLinkCount;
			if(i >= nCount)
				break;
			
			// 设置单个值
			if(bNoSeparator)
				sprintf(szTemp, "%0*d", nFill0Count, (int)cValueArray.GetAt(i));
			else
				sprintf(szTemp, ",%0*d", nFill0Count, (int)cValueArray.GetAt(i));
			m_strBufToText += szTemp;
		}
		else
		{
			// 设置单个值
			if(bNoSeparator)
				sprintf(szTemp, "%0*d", nFill0Count, (int)cValueArray.GetAt(i));
			else
				sprintf(szTemp, ",%0*d", nFill0Count, (int)cValueArray.GetAt(i));
			m_strBufToText += szTemp;
		}
	}
	
	return m_strBufToText;
}

BOOL CCstPubFunc::_TextToIntArray(LPCSTR lpszText, CDWordArray &cValueArrayOut, int nMinValue, int nMaxValue, BOOL bFill0Prev)
{
	ASSERT(lpszText != NULL);
	ASSERT(nMaxValue >= nMinValue);
	
	// 预处理
	int nLen = strlen(lpszText);
	if(nLen <= 0)
	{
		strcpy(m_szLastErrorString, "没有输入有效数字！");
		return FALSE;
	}
    
	// 开始分析
	int i = 0, j = 0, nValueMin = 0, nValueMax = 0, nValueIndex = 0;
	BOOL bHasValueMin = FALSE, bHasValueMax = FALSE, bIsNegativeMin = FALSE, bIsNegativeMax = FALSE;
	BOOL bSingleNum = (nMinValue >= -9 && nMaxValue <= 9) ? TRUE : FALSE;
    
	if(!bSingleNum && !bFill0Prev)	// 多个数字算一个值
	{
		if(lpszText[0] == '0')
		{
			// 如果第一位为0，则需要特殊处理，将0后的所有连续的数字都按单个计算
			if(!_SetIntValue(cValueArrayOut, nValueIndex, 0, nMinValue, nMaxValue, bFill0Prev))
			{
				return FALSE;
			}
			bHasValueMin = TRUE;
			nMinValue = 0;
			for(i=1; i<nLen; i++)
			{
				if(lpszText[i] < '0' || lpszText[i] > '9')		// 非数字
				{
					break;	
				}
				nValueMin = lpszText[i] - '0';
				bHasValueMin = TRUE;
				if(!_SetIntValue(cValueArrayOut, nValueIndex, nValueMin, nMinValue, nMaxValue, bFill0Prev))
				{
					return FALSE;
				}
				bHasValueMin = TRUE;
			}
		}
	}
    
	// 继续处理通常的信息
	for(i=i; i<nLen; i++)
	{
		if(lpszText[i] >= '0' && lpszText[i] <= '9')		// 是数字
		{
			if(bSingleNum)	// 如果是单个数字算一个值
			{
				nValueMin = lpszText[i] - '0';
				if(bIsNegativeMin)	// 负数
					nValueMin = 0 - nValueMin;
				if(!_SetIntValue(cValueArrayOut, nValueIndex, nValueMin, nMinValue, nMaxValue, bFill0Prev))
				{
					return FALSE;
				}
				bIsNegativeMin = FALSE;
			}
			else
			{
				nValueMin = nValueMin * 10 + (lpszText[i] - '0');
			}
			bHasValueMin = TRUE;
		}
		else if(lpszText[i] == '-')		// 范围符号或者负号
		{
			if(bHasValueMin)	// 有最小值，则肯定是范围符，则获取范围的最大值
			{
				nValueMax = 0;
				bHasValueMax = FALSE;
				bIsNegativeMax = FALSE;
                
				for(j=i+1; j<nLen; j++)
				{
					if(lpszText[j] >= '0' && lpszText[j] <= '9')		// 是数字
					{
						bHasValueMax = TRUE;
						if(bSingleNum)	// 如果是单个数字算一个值
						{
							nValueMax = lpszText[j] - '0';
							break;
						}
						else
						{
							nValueMax = nValueMax * 10 + (lpszText[j] - '0');
						}
					}
					else if(lpszText[j] == '-')		// 负号
					{
						if(bIsNegativeMax)	// 当前已经是负数，则格式错误
						{
							strcpy(m_szLastErrorString, "输入的数据格式有错误！");
							return FALSE;
						}
						bIsNegativeMax = TRUE;		// 负数
					}
					else	// 其他符号都作为分隔符，则是一个值的完成
					{
						if(bHasValueMax)
						{
							break;
						}
					}
				}
                
				// 处理最大值
				if(!bHasValueMax)
				{
					strcpy(m_szLastErrorString, "使用'-'符号时，后面必须有有效数字！");
					return FALSE;
				}
				// 负数处理
				if(bIsNegativeMax) nValueMax = 0 - nValueMax;
				if(bIsNegativeMin) nValueMin = 0 - nValueMin;
                
				if(nValueMin >= nValueMax)
				{
					strcpy(m_szLastErrorString, "使用'-'符号指定范围时，前面的数字必须小于后面的数字！");
					return FALSE;
				}
                
				if(nValueMin < nMinValue || nValueMax > nMaxValue)
				{
					if(bFill0Prev)
					{
						char szTemp[16] = {0};
						sprintf(szTemp, "%d", nMaxValue);
						int nLen = strlen(szTemp);
						sprintf(m_szLastErrorString, "输入的值必须在 %0*d 到 %0*d 之间！", nLen, nLen, nMinValue, nMaxValue);
					}
					else
					{
						sprintf(m_szLastErrorString, "输入的值必须在 %d 到 %d 之间！", nMinValue, nMaxValue);
					}
					return FALSE;
				}
                
				// 添加范围中的所有数值
				for(int k=nValueMin; k<=nValueMax; k++)
					_SetIntValue(cValueArrayOut, nValueIndex, k, nMinValue, nMaxValue, bFill0Prev);
                
				nValueMax = nValueMin = 0;
				bHasValueMax = bHasValueMin = FALSE;
				bIsNegativeMax = bIsNegativeMin = FALSE;
				i = j;	// 重置下标
			}
			else	// 以前无值，则是负号
			{
				if(bIsNegativeMin) // 当前已经是负数，则格式错误
				{
					strcpy(m_szLastErrorString, "输入的数据格式有错误！");
					return FALSE;
				}
				bIsNegativeMin = TRUE;		// 负数
			}
		}
		else	// 其他符号都作为分隔符，则是一个值的完成
		{
			if(!bSingleNum && bHasValueMin)
			{
				if(bIsNegativeMin)		// 负数
				{
					nValueMin = 0 - nValueMin;
				}
				if(!_SetIntValue(cValueArrayOut, nValueIndex, nValueMin, nMinValue, nMaxValue, bFill0Prev))
				{
					return FALSE;
				}
			}
			nValueMin = 0;
			bHasValueMin = FALSE;
			bIsNegativeMin = FALSE;
		}
	}
    
	if(bIsNegativeMin && !bHasValueMin)
	{
		strcpy(m_szLastErrorString, "使用'-'符号时，后面必须有有效数字！");
		return FALSE;
	}
	if(bHasValueMin)
	{
		if(bIsNegativeMin) nValueMin = 0 - nValueMin;
		if(!_SetIntValue(cValueArrayOut, nValueIndex, nValueMin, nMinValue, nMaxValue, bFill0Prev))
		{
			return FALSE;
		}
	}
    
	cValueArrayOut.SetSize(nValueIndex);
	if(nValueIndex == 0)
	{
		strcpy(m_szLastErrorString, "您没有输入有效数字！");
		return FALSE;
	}
    
	// 排序并去重复
	QuickSort(cValueArrayOut.GetData(), nValueIndex, QSVT_INT, TRUE, TRUE);
    
	return TRUE;
}

BOOL CCstPubFunc::_SetBitValue(LPBYTE lpBitBuf, int nValue, int nMinValue, int nMaxValue, BOOL bFill0Prev, BOOL bGroData)
{
	if(bGroData)	// 需要将值转换为组选值
	{
		char szTemp[32] = {0};
		sprintf(szTemp, "%d", nValue);
		int nLen = strlen(szTemp);
		QuickSort(szTemp, nLen, QSVT_BYTE, TRUE);
		nValue = atoi(szTemp);
	}
    
	if(nValue < nMinValue || nValue > nMaxValue)
	{
		if(bFill0Prev)
		{
			char szTemp[16] = {0};
			sprintf(szTemp, "%d", nMaxValue);
			int nLen = strlen(szTemp);
			sprintf(m_szLastErrorString, "输入的值必须在 %0*d 到 %0*d 之间！", nLen, nMinValue, nLen, nMaxValue);
		}
		else
		{
			sprintf(m_szLastErrorString, "输入的值必须在 %d 到 %d 之间！", nMinValue, nMaxValue);
		}
		return FALSE;
	}
    
	nValue -= nMinValue;
    
	ASSERT(lpBitBuf != NULL && nValue >= 0);
    
	int nByteIndex = nValue / 8, nBitIndex = nValue % 8;
    
	lpBitBuf[nByteIndex] |= BYTEBITS[nBitIndex];
    
	return TRUE;
}

BOOL CCstPubFunc::_SetIntValue(CDWordArray &dwValueArray, int &nValueIndex, int nValue, int nMinValue, int nMaxValue, BOOL bFill0Prev)
{
	if(nValue < nMinValue || nValue > nMaxValue)
	{
		if(bFill0Prev)
		{
			char szTemp[16] = {0};
			sprintf(szTemp, "%d", nMaxValue);
			int nLen = strlen(szTemp);
			sprintf(m_szLastErrorString, "输入的值必须在 %0*d 到 %0*d 之间！", nLen, nMinValue, nLen, nMaxValue);
		}
		else
		{
			sprintf(m_szLastErrorString, "输入的值必须在 %d 到 %d 之间！", nMinValue, nMaxValue);
		}
		return FALSE;
	}
    
	if(nValueIndex >= dwValueArray.GetSize())
	{
		dwValueArray.SetSize(nValueIndex + 100);
	}
	dwValueArray.SetAt(nValueIndex, nValue);
	nValueIndex ++;
    
	return TRUE;
}

int CCstPubFunc::_RemoveSameValue(LPBYTE lpValueBuf, int nValueCount, int nValueSize)
{
	if(nValueCount < 2)
	{
		return nValueCount;	// 少于等于1个值，无需去重复
	}
    
	int nValidCount = 1;
	LPBYTE lpPreValue = lpValueBuf, lpCurValue = NULL;
	for(int i=1; i<nValueCount; i++)
	{
		lpCurValue = &lpValueBuf[i * nValueSize];
		if(memcmp(lpPreValue, lpCurValue, nValueSize))	// 不同，则保留
		{
			memcpy(&lpValueBuf[nValidCount * nValueSize], lpCurValue, nValueSize);
			nValidCount ++;
			lpPreValue = lpCurValue;
		}
		else	// 相同，则忽略
		{
			continue;
		}
	}
    
	return nValidCount;
}

