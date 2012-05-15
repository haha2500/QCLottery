//
//  GraphGridBody.cpp
//  Graph
//
//  Created by 武 孙 on 12-5-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GraphGridBody.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGraphGridBody::CGraphGridBody()
{
	m_pGraphGridPublic = NULL;
	m_bIsStatGrid = FALSE;
	m_nRowCount = m_nColCount = m_nSpaceRowCount = 0;
	m_pColPosArray = NULL;
	m_ptOrg = CGPointMake(-1, -1);
}

CGraphGridBody::~CGraphGridBody()
{
	Destroy();
}

BOOL CGraphGridBody::Create(CGraphGridPublic *pGraphGridPublic, int nRowCount, int nColCount, BOOL bIsStatGrid)
{
	ASSERT(m_pGraphGridPublic == NULL && pGraphGridPublic != NULL && nRowCount >= 0 && nColCount >= 0);
	m_pGraphGridPublic = pGraphGridPublic;
	m_bIsStatGrid = bIsStatGrid;
    
	// 初始化数据
	m_rectView = m_rectSelf = CGRectZero;
	m_crColorArray = [[NSMutableArray alloc]init];	
	m_nDataRowCount = m_nRowCount = nRowCount;
    
	if(!bIsStatGrid)
	{
		LPGRAPHGRIDSET lpGridSet = m_pGraphGridPublic->GetGridSettings();
		if(lpGridSet->dwShowMask & GGSSM_BODY_FORECASTROW)	// 增加预测行
		{
			m_nForecastRow = m_nRowCount;
			m_nRowCount += 1;
		}
		else
		{
			m_nForecastRow = -1;
		}
        
		if(lpGridSet->dwShowMask & GGSSM_BODY_SPACE)		// 增加空白行
			m_nRowCount += lpGridSet->wSpaceRowCount;
	}
	
	m_nColCount = nColCount;
    
	m_nViewBeginCol = m_nViewBeginRow = m_nViewEndCol = m_nViewEndRow = 0;
    
	// 开辟列宽集合，并赋初值
	m_pColPosArray = new int[m_nColCount + 1]; 
	if(m_pColPosArray == NULL)
	{
		return FALSE;
	}
    
	m_bNeedRecalcRect = TRUE;
    
	// 开辟单元格元素数组
	m_dwCellCount = m_nRowCount * m_nColCount;
	m_lpCellInfoArray = (LPCELLINFO)new CELLINFO[m_dwCellCount];
	if(m_lpCellInfoArray == NULL)
	{
		Destroy();
		return FALSE;
	}
	ZeroMemory(m_lpCellInfoArray, sizeof(CELLINFO) * m_dwCellCount);
	
	SetBkColor([UIColor whiteColor]);
    
	return TRUE;
}

void CGraphGridBody::Destroy()
{
	if(m_pGraphGridPublic == NULL)
	{
		return ;
	}
        
    [m_crColorArray removeAllObjects];
	for(int i=0; i<(int)m_dwCellCount; i++)
	{
		if(m_lpCellInfoArray[i].btValueType == CGCT_STRING)
		{
			delete []m_lpCellInfoArray[i].lpszValue;
		}
	}
	if(m_lpCellInfoArray != NULL)
	{
		delete []m_lpCellInfoArray;
		m_lpCellInfoArray = NULL;
	}
	if(m_pColPosArray != NULL)
	{
		delete m_pColPosArray;
		m_pColPosArray = NULL;
	}
    
	m_ptOrg = CGPointMake(-1, -1);
	m_rectView = m_rectSelf = CGRectZero;
	m_nRowCount = m_nColCount = m_nSpaceRowCount = 0;
	
	m_pGraphGridPublic = NULL;
}

int CGraphGridBody::GetRowCount(BYTE btType)
{
	int nRowCount = 0;
    
	if(btType & RCT_DATA)
		nRowCount += m_nDataRowCount;
    
	if(btType & RCT_FORECAST)
		nRowCount += 1;
    
	if(btType & RCT_SPACE)
		nRowCount += m_nSpaceRowCount;
	
	return nRowCount;
}

void CGraphGridBody::Draw(CGContextRef context, BOOL bDrawBody, DWORD dwDrawCellFlag)
{
	RecalcRect();

	[m_crBkColor setFill];
	LPGRAPHGRIDSET lpGridSet = m_pGraphGridPublic->GetGridSettings();
	
	if(bDrawBody)	// 画表体
	{
		DWORD dwFlag = 0;
		if(m_nViewBeginRow <= 0)
		{
			dwFlag |= DBF_TOP;			// 需要画上边框
		}
		
		if(m_nViewEndRow >= m_nRowCount)
		{
			dwFlag |= DBF_BOTTOM;		// 需要画下边框
		}
		
		if(m_nViewBeginCol <= 0)
		{
			dwFlag |= DBF_LEFT;			// 需要画左边框
		}
		
		if(m_nViewEndCol >= m_nColCount)
		{
			dwFlag |= DBF_RIGHT;		// 需要画右边框
		}
		
		LPGRAPHGRIDCOLINFO lpColInfo = m_pGraphGridPublic->GetColumnInfo();
		int nRowHeight = m_pGraphGridPublic->GetRowHeight(m_bIsStatGrid);
		int nBeginX = m_pColPosArray[m_nViewBeginCol]; 
		int nEndX = m_pColPosArray[m_nViewEndCol];
		int nBeginY = m_ptOrg.y + m_nViewBeginRow * (nRowHeight + lpGridSet->btBodyPenWidth) + lpGridSet->btBorderPenWidth;
		int nEndY = m_ptOrg.y + m_nViewEndRow * (nRowHeight + lpGridSet->btBodyPenWidth) + lpGridSet->btBorderPenWidth - lpGridSet->btBodyPenWidth;
		
        [lpGridSet->crBodyPen setStroke];

		int i = 0, y = 0;
		
		// 画横线
		for(i=m_nViewBeginRow; i<=m_nViewEndRow; i++)
		{
			if((i == 0) || (i == m_nRowCount))
			{
				continue ;
			}
			y = m_ptOrg.y + i * nRowHeight + (i - 1) * lpGridSet->btBodyPenWidth + lpGridSet->btBorderPenWidth;
			m_pGraphGridPublic->DrawLine(context, lpGridSet->btBodyPenWidth, nBeginX - lpGridSet->btBodyPenWidth, y, nEndX, y, TRUE);
		}
		
		// 画竖线
		for(i=m_nViewBeginCol; i<=m_nViewEndCol; i++)
		{
			if((i == 0) || (i == m_nColCount))
			{
				continue ;
			}
			m_pGraphGridPublic->DrawLine(context, lpGridSet->btBodyPenWidth + lpColInfo[i-1].btRightBorderWidth, m_pColPosArray[i] - lpColInfo[i-1].btRightBorderWidth, nBeginY - lpGridSet->btBodyPenWidth, m_pColPosArray[i] - lpColInfo[i-1].btRightBorderWidth, nEndY, FALSE);
		}
		
		// 画边框
		_DrawBorder(context, nBeginX, nEndX, nBeginY, nEndY, dwFlag);
	}
	
	if(dwDrawCellFlag & (DF_CELL_BKGND | DF_CELL_VALUE))	// 画单元格
	{
		UIColor *crBack = m_crBkColor, *crText = nil;
		BYTE btRowInfo = 0;
               
		for(int i=m_nViewBeginRow; i<m_nViewEndRow; i++)
		{
			// 获取该行背景色
			btRowInfo = _GetRowInfo(lpGridSet, i, &crText, &crBack);
            
			for(int j=m_nViewBeginCol; j<m_nViewEndCol; j++)
			{
				_DrawCell(context, i, j, dwDrawCellFlag, crText, crBack, btRowInfo);
			}
		}
	}
}

void CGraphGridBody::SetViewRect(CGRect &rectView)
{
    if (CGRectEqualToRect(m_rectView, rectView))
	{
		return ;		// 如果没有改变可见矩形范围
	}
    
	m_rectView = rectView;
    
	m_bNeedRecalcRect = TRUE;
	RecalcRect();
}

CGRect CGraphGridBody::GetRect()
{
	RecalcRect();
    
	return m_rectSelf;
}

void CGraphGridBody::OnGridSettingsChanged()
{
	m_bNeedRecalcRect = TRUE;
}

void CGraphGridBody::RecalcRect()
{
	if(!m_bNeedRecalcRect)
		return ;
    
	int i = 0, nRowHeight = m_pGraphGridPublic->GetRowHeight(m_bIsStatGrid);
	LPGRAPHGRIDSET lpGridSet = m_pGraphGridPublic->GetGridSettings();
	LPGRAPHGRIDCOLINFO lpColInfo = m_pGraphGridPublic->GetColumnInfo();
    
	m_rectSelf = CGRectMake(m_ptOrg.x, m_ptOrg.y, 0, m_nRowCount * nRowHeight + lpGridSet->btBorderPenWidth * 2 + lpGridSet->btBodyPenWidth * (m_nRowCount - 1));
    
	// 重新计算列所在位置
	m_pColPosArray[0] = m_ptOrg.x + lpGridSet->btBorderPenWidth - lpGridSet->btBodyPenWidth;
	if(m_bIsStatGrid && (m_nColCount == 1))	// 可能是统计表头
	{
		m_pColPosArray[1] = m_pColPosArray[0];
		for(i=m_pGraphGridPublic->GetColumnCount(); i>=1; i--)
		{
			m_pColPosArray[1] += (lpColInfo[i-1].btRightBorderWidth + lpColInfo[i-1].nWidth + lpGridSet->btBodyPenWidth);
		}
	}
	else
	{
		for(i=1; i<=m_nColCount; i++)
		{
			m_pColPosArray[i] = m_pColPosArray[i-1] + (lpColInfo[i-1].btRightBorderWidth + lpColInfo[i-1].nWidth + lpGridSet->btBodyPenWidth);
		}
	}
	m_rectSelf.size.width = m_pColPosArray[m_nColCount] + lpGridSet->btBorderPenWidth - m_rectSelf.origin.x;
    
	// 计算可视单元格范围
	m_nViewBeginRow = int((double)(m_rectView.origin.y - m_ptOrg.y) / (double)(nRowHeight + lpGridSet->btBodyPenWidth)) - 1;
	m_nViewEndRow = int((double)(m_rectView.origin.y + m_rectView.size.height - m_ptOrg.y) / (double)(nRowHeight + lpGridSet->btBodyPenWidth)) + 1;
	for(i=0; i<=m_nColCount; i++)
	{
		if(m_pColPosArray[i] > m_rectView.origin.x)
		{
			break;
		}
	}
	m_nViewBeginCol = i - 1;
	for(i=m_nColCount; i>=0; i--)
	{
		if(m_pColPosArray[i] < m_rectView.origin.x + m_rectView.size.width)
		{
			break;
		}
	}
	m_nViewEndCol = i + 1;
	
	// 限制取值
	if(m_nViewBeginCol < 0)
		m_nViewBeginCol = 0;
	if(m_nViewBeginRow < 0)
		m_nViewBeginRow = 0;
	if(m_nViewEndCol > m_nColCount)
		m_nViewEndCol = m_nColCount;
	if(m_nViewEndRow > m_nRowCount)
		m_nViewEndRow = m_nRowCount;
    
	m_bNeedRecalcRect = FALSE;
}

void CGraphGridBody::SetOrg(int x, int y)
{
	if(m_pGraphGridPublic == NULL)
	{
		ASSERT(FALSE);
		return ;
	}
    
	if(m_ptOrg.x == x && m_ptOrg.y == y)
	{
		return ;		// 没有改变
	}
    
	m_ptOrg = CGPointMake(x, y);
    
	m_bNeedRecalcRect = TRUE;
}

void CGraphGridBody::SetBkColor(UIColor *color)
{
	m_crBkColor = color;
}

int CGraphGridBody::GetForecastRow()
{
    return m_nForecastRow;
}

BOOL CGraphGridBody::IsForecastRow(int nRowIndex)
{
	return (m_nForecastRow == nRowIndex) ? TRUE : FALSE;
}

int CGraphGridBody::GetSelectRow()
{
	if(!(m_pGraphGridPublic->GetGridSettings()->dwShowMask & GGSSM_BODY_CURSELROW))
	{
		return -1;	// 当前不显示选择行
	}
	
    return m_nSelectedRow;
}

void CGraphGridBody::SetSelectRow(int nRowIndex)
{
	if(!(m_pGraphGridPublic->GetGridSettings()->dwShowMask & GGSSM_BODY_CURSELROW))
	{
		return ;	// 当前不显示选择行
	}
	
    m_nSelectedRow = nRowIndex;
}

BOOL CGraphGridBody::IsSelectedRow(int nRowIndex)
{
	if(!(m_pGraphGridPublic->GetGridSettings()->dwShowMask & GGSSM_BODY_CURSELROW))
	{
		return FALSE;	// 当前不显示选择行
	}
    
	return (m_nSelectedRow == nRowIndex) ? TRUE : FALSE;;
}

void CGraphGridBody::SetCellColor(int nRowIndex, int nColIndex, UIColor *textColor, UIColor *backColor)
{
	LPCELLINFO lpItem = _GetCellInfo(nRowIndex, nColIndex);
    
	// 查询是否已经有该颜色
	if(textColor != nil)
	{
        int nIndex = [m_crColorArray indexOfObject:textColor];
        if(nIndex == NSNotFound)
        {
            [m_crColorArray addObject:textColor];
            lpItem->btTextColorIndex = (BYTE)([m_crColorArray count]);
        }
        else
        {
            lpItem->btTextColorIndex = (BYTE)(nIndex + 1);
        }
        
		ASSERT(lpItem->btTextColorIndex < MAXBYTE);
	}
    
	if(backColor != nil)
	{
        int nIndex = [m_crColorArray indexOfObject:backColor];
        if(nIndex == NSNotFound)
        {
            [m_crColorArray addObject:backColor];
            lpItem->btBkColorIndex = (BYTE)([m_crColorArray count]);
        }
        else
        {
            lpItem->btBkColorIndex = (BYTE)(nIndex + 1);
        }
   
		ASSERT(lpItem->btBkColorIndex < MAXBYTE);
	}
}

void CGraphGridBody::SetCellText(int nRowIndex, int nColIndex, LPCSTR lpszValue)
{
	LPCELLINFO lpItem = _GetCellInfo(nRowIndex, nColIndex);
	
	if(lpItem->btValueType == CGCT_STRING)
	{
		DELETE_BUFFER(lpItem->lpszValue);
	}
	
	// 赋值
	int nLen = (lpszValue == NULL) ? 0 : strlen(lpszValue);
	if(nLen == 0)	// 空串
	{
		lpItem->btValueType = CGCT_NULL;
		ZeroMemory(lpItem->szValue, sizeof(lpItem->szValue));
		return ;
	}
    
	if(m_bIsStatGrid)	// 如果是统计表格，则特殊处理
	{
		if(nLen < 4)
		{
			lpItem->btValueType = (BYTE)(CGCT_STRING_1 + nLen - 1);
			strcpy(lpItem->szValue, lpszValue);
		}
		else
		{
			lpItem->btValueType = CGCT_STRING;
			lpItem->lpszValue = new char[nLen + 1];
			strcpy(lpItem->lpszValue, lpszValue);
		}
		return ;
	}
    
	// 非统计表格，即正常表体表格
	LPGRAPHGRIDCOLINFO lpColInfo = m_pGraphGridPublic->GetColumnInfo();
	LPGRAPHGRIDCOLINFO lpColItem = &lpColInfo[nColIndex];
	switch(lpColItem->btMaxItemTextLen)
	{
        case 0:		// 正常情况
            if(nLen < 4)
            {
                lpItem->btValueType = (BYTE)(CGCT_STRING_1 + nLen - 1);
                strcpy(lpItem->szValue, lpszValue);
            }
            else
            {
                lpItem->btValueType = CGCT_STRING;
                lpItem->lpszValue = new char[nLen + 1];
                strcpy(lpItem->lpszValue, lpszValue);
            }
            break;
            
        case MAXBYTE:	// 压缩型
		{
			if(nLen < 4)	// 长度小于4的，按正常处理
			{
				lpItem->btValueType = (BYTE)(CGCT_STRING_1 + nLen - 1);
				strcpy(lpItem->szValue, lpszValue);
				break;
			}
			
            ASSERT(FALSE);  // 暂不支持
/*			CTypedPtrArray<CPtrArray, LPSTR> *pTextArray = lpColItem->pTextArray;
			ASSERT(pTextArray != NULL);
			int nCount = pTextArray->GetSize(), nCompareResult = 0;
			int nMid = nCount / 2, nFirst = 0, nLast = nCount;
			lpItem->lpszValue = NULL;
			
			// 折半查找
			while(nFirst < nLast)
			{
				nCompareResult = strcmp(lpszValue, pTextArray->GetAt(nMid));
				if(nCompareResult == 0)
				{
					lpItem->lpszValue = pTextArray->GetAt(nMid);		// 相同串，则直接使用
					break;
				}
				else if(nCompareResult < 0)		// 如果小于当前元素值，则往前折半查找
				{
					nLast = nMid - 1;
				}
				else							// 如果大于当前元素值，则往后折半查找
				{
					nFirst = nMid + 1;
				}
				nMid = (nFirst + nLast) / 2;
			}
			if(lpItem->lpszValue == NULL)
			{
				// 没有找到，则存储并插入到指定位置
				lpItem->lpszValue = new char[nLen + 1];
				strcpy(lpItem->lpszValue, lpszValue);
				pTextArray->InsertAt(nFirst, lpItem->lpszValue);
			}
			lpItem->btValueType = CGCT_STRING_COMPRESS;*/
		} break;
            
        default:		// 一次性缓冲区
		{
			ASSERT(lpColItem->lpTextBuf != NULL);
			if(nLen <= lpColItem->btMaxItemTextLen)
			{
				lpItem->btValueType = CGCT_STRING_BUF;
				lpItem->lpszValue = &lpColItem->lpTextBuf[nRowIndex * (lpColItem->btMaxItemTextLen + 1)];
				memcpy(lpItem->lpszValue, lpszValue, nLen);
				lpItem->lpszValue[nLen] = 0;
			}
			else
			{
				lpItem->btValueType = CGCT_STRING;
				lpItem->lpszValue = new char[nLen + 1];
				strcpy(lpItem->lpszValue, lpszValue);
			}
		} break;
	}
}

void CGraphGridBody::SetCellInt(int nRowIndex, int nColIndex, int nValue, BYTE btType)
{
	ASSERT(btType >= CGCT_INT && btType <= CGCT_INT_SKIP);
    
	LPCELLINFO lpItem = _GetCellInfo(nRowIndex, nColIndex);
    
	if(lpItem->btValueType == CGCT_STRING)
	{
		DELETE_BUFFER(lpItem->lpszValue);
	}
    
	// 赋值
	lpItem->btValueType = btType;
	lpItem->nValue = nValue;
}

void CGraphGridBody::SetCellFloat(int nRowIndex, int nColIndex, float fValue, BYTE btType)
{
	ASSERT(btType >= CGCT_FLOAT_1 && btType <= CGCT_PERCENT_4);
	
	LPCELLINFO lpItem = _GetCellInfo(nRowIndex, nColIndex);
	
	if(lpItem->btValueType == CGCT_STRING)
	{
		DELETE_BUFFER(lpItem->lpszValue);
	}
	
	// 赋值
	lpItem->btValueType = btType;
	lpItem->fValue = fValue;
}

void CGraphGridBody::SetCellBitmap(int nRowIndex, int nColIndex, int nBitmapIndex, BYTE btValueCount)
{
	if(nBitmapIndex == -1)
		nBitmapIndex = nColIndex;
    
	// 有效性检查
	ASSERT(nBitmapIndex >= 0);
    
	// 赋值
	LPCELLINFO lpItem = _GetCellInfo(nRowIndex, nColIndex);
	lpItem->btValueType = CGCT_BITMAP_INDEX;
	lpItem->nBallBitmapIndex = (short)nBitmapIndex;
	
	if(btValueCount != MAXBYTE)
	{
		lpItem->btValueCount = btValueCount;
	}
}

int CGraphGridBody::GetCellBitmap(int nRowIndex, int nColIndex, LPBYTE lpValueCount)
{
	LPCELLINFO lpItem = _GetCellInfo(nRowIndex, nColIndex);
    
	if(lpItem->btValueType != CGCT_BITMAP_INDEX)
	{
		return -1;
	}
    
	if(lpValueCount != NULL)
	{
		(*lpValueCount) = lpItem->btValueCount;
	}
    
	return lpItem->nBallBitmapIndex;
}

CGRect CGraphGridBody::GetCellRect(int nRowIndex, int nColIndex)
{
	int nRowHeight = m_pGraphGridPublic->GetRowHeight(m_bIsStatGrid);
	LPGRAPHGRIDSET lpGridSet = m_pGraphGridPublic->GetGridSettings();
    //	LPGRAPHGRIDCOLINFO lpColInfo = m_pGraphGridPublic->GetColumnInfo();
    
	ASSERT(nRowIndex >=0 && nRowIndex < m_nRowCount);
	ASSERT(nColIndex >=0 && nColIndex < m_nColCount);
    
	RecalcRect();
    
	LPGRAPHGRIDCOLINFO lpColInfo = m_pGraphGridPublic->GetColumnInfo(nColIndex);
    
	CGRect rect;
	rect.origin.x = m_pColPosArray[nColIndex] + lpGridSet->btBodyPenWidth;
	rect.size.width = m_pColPosArray[nColIndex + 1] - lpColInfo->btRightBorderWidth - rect.origin.x;
	rect.origin.y = m_ptOrg.y + lpGridSet->btBorderPenWidth + (nRowHeight + lpGridSet->btBodyPenWidth) * nRowIndex;
	rect.size.height = nRowHeight;
	
    rect.origin.y --;   // 由于Y坐标没有0值，所以需要1个偏移点
    
	return rect;
}

void CGraphGridBody::GetViewRow(int *pViewBeginRow, int *pViewEndRow)
{
	*pViewBeginRow = m_nViewBeginRow;
	*pViewEndRow = m_nViewEndRow;
}

UIColor * CGraphGridBody::GetCellColor(int nRowIndex, int nColIndex, UIColor **backColorOut)
{
	LPCELLINFO lpCellItem = _GetCellInfo(nRowIndex, nColIndex);
	
	if(backColorOut != NULL)
	{
		BYTE btIndex = lpCellItem->btBkColorIndex;
		(*backColorOut) = (btIndex == 0) ? ([UIColor whiteColor]) : [m_crColorArray objectAtIndex:(btIndex - 1)];
	}
	
	return (lpCellItem->btTextColorIndex == 0) ? [UIColor blackColor] : [m_crColorArray objectAtIndex:(lpCellItem->btTextColorIndex - 1)];
}

int CGraphGridBody::GetCellInt(int nRowIndex, int nColIndex, LPBYTE lpType)
{
	LPCELLINFO lpCellItem = _GetCellInfo(nRowIndex, nColIndex);
    
	if(lpType != NULL)
	{
		*lpType = lpCellItem->btValueType;
	}
	
	if(lpCellItem->btValueType >= CGCT_INT && lpCellItem->btValueType <= CGCT_INT_SKIP)
		return lpCellItem->nValue;
	else if(lpCellItem->btValueType == CGCT_BITMAP_INDEX)
		return lpCellItem->nBallBitmapIndex;
	else
		return -1;
}

float CGraphGridBody::GetCellFloat(int nRowIndex, int nColIndex, LPBYTE lpType)
{
	LPCELLINFO lpCellItem = _GetCellInfo(nRowIndex, nColIndex);
    
	if(lpType != NULL)
	{
		*lpType = lpCellItem->btValueType;
	}
	
	if(lpCellItem->btValueType >= CGCT_FLOAT_1 && lpCellItem->btValueType <= CGCT_PERCENT_4)
		return lpCellItem->fValue;
	else
		return (float)0.0;
}

LPCSTR CGraphGridBody::GetCellText(int nRowIndex, int nColIndex, LPBYTE lpType)
{
	LPCELLINFO lpCellItem = _GetCellInfo(nRowIndex, nColIndex);
    
	if(lpType != NULL)
	{
		if(lpCellItem->btValueType >= CGCT_STRING_1 && lpCellItem->btValueType < CGCT_STRING)
			*lpType = CGCT_STRING;
		else
			*lpType = lpCellItem->btValueType;
	}
	
	switch(lpCellItem->btValueType)
	{
        case CGCT_NULL: 
        case CGCT_STRING_1:
        case CGCT_STRING_2:
        case CGCT_STRING_3:
            return lpCellItem->szValue; break;
            
        case CGCT_STRING:
        case CGCT_STRING_BUF:
//        case CGCT_STRING_COMPRESS:
            return lpCellItem->lpszValue; break;
            
        case CGCT_BITMAP_INDEX: 
            sprintf(m_szTempItemText, "号码球%d", lpCellItem->nBallBitmapIndex);
            break;
            
        case CGCT_INT:
        case CGCT_INT_SKIP:
        case CGCT_INT_STAT:
            sprintf(m_szTempItemText, "%d", lpCellItem->nValue);
            break;
            
        case CGCT_INT_1:
        case CGCT_INT_2:
        case CGCT_INT_3:
        case CGCT_INT_4:
        case CGCT_INT_5:
        case CGCT_INT_6:
        case CGCT_INT_7:
        case CGCT_INT_8:
        case CGCT_INT_9:
        case CGCT_INT_10:
            sprintf(m_szTempItemText, "%0*d", lpCellItem->btValueType - CGCT_INT, lpCellItem->nValue);
            break;
            
        case CGCT_FLOAT_1: sprintf(m_szTempItemText, "%.1f", lpCellItem->fValue); break;
        case CGCT_FLOAT_2: sprintf(m_szTempItemText, "%.2f", lpCellItem->fValue); break;
        case CGCT_FLOAT_3: sprintf(m_szTempItemText, "%.3f", lpCellItem->fValue); break;
        case CGCT_FLOAT_4: sprintf(m_szTempItemText, "%.4f", lpCellItem->fValue); break;
            
        case CGCT_PERCENT: sprintf(m_szTempItemText, "%d%%", (int)lpCellItem->fValue); break;
        case CGCT_PERCENT_1: sprintf(m_szTempItemText, "%.1f%%", lpCellItem->fValue); break;
        case CGCT_PERCENT_2: sprintf(m_szTempItemText, "%.2f%%", lpCellItem->fValue); break;
        case CGCT_PERCENT_3: sprintf(m_szTempItemText, "%.3f%%", lpCellItem->fValue); break;
        case CGCT_PERCENT_4: sprintf(m_szTempItemText, "%.4f%%", lpCellItem->fValue); break;
            
        default: ASSERT(FALSE); break;
	}
	
	return m_szTempItemText;
}

BYTE CGraphGridBody::GetCellValueType(int nRowIndex, int nColIndex)
{
	if(nRowIndex < 0 || nRowIndex >= m_nRowCount || nColIndex < 0 || nColIndex >= m_nColCount)
	{
		return CGCT_NULL;
	}
    
	return _GetCellInfo(nRowIndex, nColIndex)->btValueType;
}

DWORD CGraphGridBody::HitTest(CGPoint pt, int *pRowIndex, int *pColIndex)
{
    if(CGRectContainsPoint(m_rectSelf, pt))
	{
		return 0;
	}
    
	int nRowHeight = m_pGraphGridPublic->GetRowHeight(m_bIsStatGrid);
	LPGRAPHGRIDSET lpGridSet = m_pGraphGridPublic->GetGridSettings();
    
	*pRowIndex = (pt.y - m_ptOrg.y - lpGridSet->btBorderPenWidth) / (nRowHeight + lpGridSet->btBodyPenWidth);
	if(*pRowIndex < m_nViewBeginRow || *pRowIndex > m_nViewEndRow || *pRowIndex >= m_nRowCount)
	{
		return 0;
	}
	
	for(int i=0; i<m_nColCount; i++)
	{
        if(CGRectContainsPoint(GetCellRect(*pRowIndex, i), pt))
		{
			*pColIndex = i;
			if(m_bIsStatGrid)
			{
				return GRIDHT_STAT_CELL;
			}
			if(IsForecastRow(*pRowIndex))
			{
				return GRIDHT_BODY_FORECAST;		// 在表格的某个预测单元格中
			}
			else if(*pRowIndex - m_nSpaceRowCount >= m_nDataRowCount)
			{
				return GRIDHT_BODY_SPACE;			// 在某个空白单元格中
			}
			else
			{
				return GRIDHT_BODY_DATA;			// 在某个数据单元格中
			}
		}
	}
    
	return m_bIsStatGrid ? GRIDHT_STAT_BORDER : GRIDHT_BODY_BORDER;					// 在边框上
}

void CGraphGridBody::ClearForecastRow(int nForecastCol)
{
	int m_nColCount = m_pGraphGridPublic->GetColumnCount();
	ASSERT(nForecastCol >= -1 && nForecastCol < m_nColCount);
    
	if(nForecastCol == -1)
	{
		// 去除预测行所有的
		for(int i=0; i<m_nColCount; i++)
		{
			SetCellText(m_nForecastRow, i, NULL);
		}
	}
	else
	{
		// 去除预测行中指定的某列信息
		SetCellText(m_nForecastRow, nForecastCol, NULL);
	}
}

int CGraphGridBody::GetColumnCellsMaxLen(int nColIndex)
{
	int nMinValue = 0, nMaxValue = 0, nMaxLen = 0, nLen = 0;
	float fMinValue = (float)0.0, fMaxValue = (float)0.0;
	BOOL bHasPercent = FALSE;
    
	LPCELLINFO lpCellItem = NULL;
	for(int i=0; i<m_nRowCount; i++)
	{
		lpCellItem = &m_lpCellInfoArray[i * m_nColCount + nColIndex];
		switch(lpCellItem->btValueType)
		{
            case CGCT_STRING_1:
            case CGCT_STRING_2:
            case CGCT_STRING_3:
                nLen = strlen(lpCellItem->szValue);
                if(nLen > nMaxLen)
                    nMaxLen = nLen;
                break;
            case CGCT_STRING:
            case CGCT_STRING_BUF:
//            case CGCT_STRING_COMPRESS:
                nLen = strlen(lpCellItem->lpszValue);
                if(nLen > nMaxLen)
                    nMaxLen = nLen;
                break;
            case CGCT_INT:
                if(lpCellItem->nValue > nMaxValue)
                    nMaxValue = lpCellItem->nValue;
                if(lpCellItem->nValue < nMinValue)
                    nMinValue = lpCellItem->nValue;
                break;
            case CGCT_INT_1:
            case CGCT_INT_2:
            case CGCT_INT_3:
            case CGCT_INT_4:
            case CGCT_INT_5:
            case CGCT_INT_6:
            case CGCT_INT_7:
            case CGCT_INT_8:
            case CGCT_INT_9:
            case CGCT_INT_10:
                nLen = lpCellItem->btValueType - CGCT_INT;
                if(nLen > nMaxLen)
                    nMaxLen = nLen;
                break;
            case CGCT_INT_STAT:
            case CGCT_INT_SKIP:
            case CGCT_BITMAP_INDEX:
                if(3 > nMaxLen)
                    nMaxLen = 3;
                break;
            case CGCT_FLOAT_1:
            case CGCT_FLOAT_2:
            case CGCT_FLOAT_3:
            case CGCT_FLOAT_4:
                if(lpCellItem->fValue > fMaxValue)
                    fMaxValue = lpCellItem->fValue;
                if(lpCellItem->fValue < fMinValue)
                    fMinValue = lpCellItem->fValue;
                break;
            case CGCT_PERCENT:
            case CGCT_PERCENT_1:
            case CGCT_PERCENT_2:
            case CGCT_PERCENT_3:
            case CGCT_PERCENT_4:
                bHasPercent = TRUE;
                if(lpCellItem->fValue > fMaxValue)
                    fMaxValue = lpCellItem->fValue;
                if(lpCellItem->fValue < fMinValue)
                    fMinValue = lpCellItem->fValue;
                break;
            default: break;
		}
	}
    
	if(nMaxLen < 16)
	{
		char szTemp[16] = {0};
		if(nMaxValue > 0)
		{
			sprintf(szTemp, "%d", nMaxValue);
			nLen = strlen(szTemp);
			if(nLen > nMaxLen)
				nMaxLen = nLen;
		}
		if(nMinValue < 0)
		{
			sprintf(szTemp, "%d", nMinValue);
			nLen = strlen(szTemp);
			if(nLen > nMaxLen)
				nMaxLen = nLen;
		}
		if(fMaxValue > (float)0.0)
		{
			sprintf(szTemp, "%.4f", fMaxValue);
			nLen = strlen(szTemp);
			if(nLen > nMaxLen)
				nMaxLen = nLen;
		}
		if(fMinValue < (float)0.0)
		{
			sprintf(szTemp, "%.4f", fMinValue);
			nLen = strlen(szTemp);
			if(nLen > nMaxLen)
				nMaxLen = nLen;
		}
		if(bHasPercent)
			nMaxLen += 2;
	}
    
	return nMaxLen;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
BYTE CGraphGridBody::_GetRowInfo(LPGRAPHGRIDSET lpGridSet, int nRowIndex, UIColor **crTextOut, UIColor **crBackOut)
{
	if(IsSelectedRow(nRowIndex)) // 选择行
	{
		(*crBackOut) = lpGridSet->crCurSelRowBK;
		(*crTextOut) = lpGridSet->crCurSelRowText;
		return RI_SELECT;
	}
	else if(nRowIndex >= m_nDataRowCount)
	{
		if(IsForecastRow(nRowIndex))	// 预测行
		{
			(*crBackOut) = lpGridSet->crForecastRowBK;
			(*crTextOut) = lpGridSet->crForecastRowText;
			return RI_FORECAST;
		}
		else		// 空白行
		{
			(*crBackOut) = m_crBkColor;
			(*crTextOut) = lpGridSet->crBodyText;
			return RI_SPACE;
		}
	}
	else	// 缺省背景色
	{
		if(m_bIsStatGrid)
		{
			if(lpGridSet->dwShowMask & GGSSM_STAT_ALTROWCOLOR)	// 启用了交替颜色
			{
				int nIndex = ((m_nDataRowCount - nRowIndex - 1) / lpGridSet->wStatAltRowCount) % 2;
				(*crBackOut) = lpGridSet->crStatAltRowBK[nIndex];
				(*crTextOut) = lpGridSet->crStatAltRowText[nIndex];
			}
			else
			{
				(*crBackOut) = lpGridSet->crStatBK;
				(*crTextOut) = lpGridSet->crStatText;
			}
		}
		else
		{
			if(lpGridSet->dwShowMask & GGSSM_BODY_ALTROWCOLOR)	// 启用了交替颜色
			{
				int nIndex = ((m_nDataRowCount - nRowIndex - 1) / lpGridSet->wAltRowCount) % 2;
				(*crBackOut) = lpGridSet->crAltRowBK[nIndex];
				(*crTextOut) = lpGridSet->crAltRowText[nIndex];
			}
			else
			{
				(*crBackOut) = m_crBkColor;
				(*crTextOut) = lpGridSet->crBodyText;
			}
		}
		return RI_OTHER;
	}
}

void CGraphGridBody::_DrawCell(CGContextRef context, int nRowIndex, int nColIndex, DWORD dwFlag, UIColor *crText, UIColor *crBack, BYTE btRowInfo)
{
	LPCELLINFO lpItem = _GetCellInfo(nRowIndex, nColIndex);
	
	// 获取单元格位置
	CGRect rect = GetCellRect(nRowIndex, nColIndex);
	if(rect.size.width <= 0 || rect.size.height <= 0)
	{
		return ;
	}
	
	LPGRAPHGRIDSET lpGridSet = m_pGraphGridPublic->GetGridSettings();
	if(dwFlag & DF_CELL_BKGND)	// 填背景
	{
		if(lpItem->btBkColorIndex != 0)	// 自定义背景色
		{
			if((btRowInfo != RI_SELECT) || (m_pGraphGridPublic->GetColumnInfo(nColIndex)->btFlag & CIF_LOCK_BACKCOLOR))
				crBack = [m_crColorArray objectAtIndex:(lpItem->btBkColorIndex - 1)];
		}
        [crBack setFill];
        CGContextFillRect(context, rect);
	}
	
	if(dwFlag & DF_CELL_VALUE)	// 画单元格
	{		
		switch(lpItem->btValueType) 
		{
            case CGCT_BITMAP_INDEX:
            {
                CBallBitmap *pBallBitmap = m_pGraphGridPublic->GetBallBitmap(lpItem->nBallBitmapIndex);
                if(pBallBitmap != NULL)
                {
                    pBallBitmap->DrawInRect(context, rect);
                }
            } break;
            case CGCT_INT_SKIP:
			{
				if(!(lpGridSet->dwShowMask & GGSSM_BODY_SKIP))
				{
					break;
				}
				
				sprintf(m_szTempItemText, "%d", lpItem->nValue);				
				if((btRowInfo != RI_SELECT) && !(lpGridSet->dwShowMask & GGSSM_BODY_ALTROWCOLOR))
					crText = lpGridSet->crSkipText;
                UIFont *font = (lpItem->nValue < 1000) ? lpGridSet->fontSkipBig : lpGridSet->fontSkipSmall;
				m_pGraphGridPublic->DrawText(context, rect, m_szTempItemText, font, crText);
			} break;
                
            case CGCT_INT_STAT:
			{
				if(lpItem->nValue == MAXDWORD)
				{
                    m_pGraphGridPublic->DrawText(context, rect, "-", lpGridSet->fontSkipBig, crText);
				}
				else
				{
					sprintf(m_szTempItemText, "%d", lpItem->nValue);
					
					if(lpItem->btTextColorIndex != 0)
						crText = [m_crColorArray objectAtIndex:(lpItem->btTextColorIndex - 1)];
					else if(!(lpGridSet->dwShowMask & GGSSM_STAT_ALTROWCOLOR))
						crText = lpGridSet->crStatText;
					
                    UIFont *font = (lpItem->nValue < 1000) ? lpGridSet->fontSkipBig : lpGridSet->fontSkipSmall;
                    m_pGraphGridPublic->DrawText(context, rect, m_szTempItemText, font, crText);
				}
			} break;
            case CGCT_FLOAT_STAT:
			{
				sprintf(m_szTempItemText, "%.1f", lpItem->fValue);
				
				if(lpItem->btTextColorIndex != 0)
					crText = [m_crColorArray objectAtIndex:(lpItem->btTextColorIndex - 1)];
				else if(!(lpGridSet->dwShowMask & GGSSM_STAT_ALTROWCOLOR))
					crText = lpGridSet->crStatText;
				
                UIFont *font = (strlen(m_szTempItemText) <= 3) ? lpGridSet->fontSkipBig : lpGridSet->fontSkipSmall;
                m_pGraphGridPublic->DrawText(context, rect, m_szTempItemText, font, crText);
			} break;
            default:
			{
				if(lpItem->btTextColorIndex != 0)
				{
					if((btRowInfo != RI_SELECT) || (m_pGraphGridPublic->GetColumnInfo(nColIndex)->btFlag & CIF_LOCK_TEXTCOLOR))
						crText = [m_crColorArray objectAtIndex:(lpItem->btTextColorIndex - 1)];
				}
                
                m_pGraphGridPublic->DrawText(context, rect, GetCellText(nRowIndex, nColIndex), lpGridSet->fontBody, crText);
			}break;
		}
	}
}

void CGraphGridBody::_DrawBorder(CGContextRef context, int nBeginX, int nEndX, int nBeginY, int nEndY, DWORD dwFlag)
{
	LPGRAPHGRIDSET lpGridSet = m_pGraphGridPublic->GetGridSettings();
	
    [lpGridSet->crBorderPen setStroke];
    	
	if(nEndX > m_rectSelf.origin.x + m_rectSelf.size.width - lpGridSet->btBorderPenWidth)
	{
		nEndX = m_rectSelf.origin.x + m_rectSelf.size.width - lpGridSet->btBorderPenWidth;
	}
	
	if(nEndY > m_rectSelf.origin.y + m_rectSelf.size.height - lpGridSet->btBorderPenWidth)
	{
		nEndY = m_rectSelf.origin.y + m_rectSelf.size.height - lpGridSet->btBorderPenWidth;
	}
	
	if(dwFlag & DBF_TOP)			// 需要画上边框
	{
		m_pGraphGridPublic->DrawLine(context, lpGridSet->btBorderPenWidth, nBeginX - lpGridSet->btBodyPenWidth, m_rectSelf.origin.y, nEndX, m_rectSelf.origin.y, TRUE);
		nBeginY = m_ptOrg.y;
	}
	else
	{
		nBeginY -= lpGridSet->btBodyPenWidth;
	}
	
	if(dwFlag & DBF_BOTTOM)			// 需要画下边框
	{
        nEndY = m_rectSelf.origin.y + m_rectSelf.size.height;
		m_pGraphGridPublic->DrawLine(context, lpGridSet->btBorderPenWidth, nBeginX - lpGridSet->btBodyPenWidth, nEndY - lpGridSet->btBorderPenWidth, nEndX, nEndY - lpGridSet->btBorderPenWidth, TRUE);
	}
	
	if(dwFlag & DBF_LEFT)			// 需要画左边框
	{
		m_pGraphGridPublic->DrawLine(context, lpGridSet->btBorderPenWidth, m_ptOrg.x, nBeginY, m_ptOrg.x, nEndY-1, FALSE);
	}
	
	if(dwFlag & DBF_RIGHT)			// 需要画右边框
	{
		m_pGraphGridPublic->DrawLine(context, lpGridSet->btBorderPenWidth, nEndX, nBeginY, nEndX, nEndY-1, FALSE);
	}
}

