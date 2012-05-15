//
//  TrendGrid.cpp
//  Graph
//
//  Created by 武 孙 on 12-5-7.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "TrendGrid.h"
#include "HistoryValueStat.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTrendGrid::CTrendGrid()
{
	m_bSettingsChanged = FALSE;
	m_lpConnectValueArray = NULL;
	m_nGroupIndex = 0;
}

CTrendGrid::~CTrendGrid()
{
	Destroy();
}

/////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTrendGrid::CreateTrendGrid(int nColCount, int nDataRowCount)
{
	if(!CGraphGrid::Create(CGPointZero, nDataRowCount, nColCount))
	{
		return FALSE;
	}
	
	int nConnectCount = GetRowCount(RCT_DATA | RCT_FORECAST);
    
	m_lpConnectValueArray = new WORD[nConnectCount];
	if(m_lpConnectValueArray == NULL)
	{
		Destroy();
		return FALSE;
	}
	memset(m_lpConnectValueArray, 0xFFFF, nConnectCount * sizeof(WORD));
    
    OnTrendGridSettingsChanged();
    
	return TRUE;
}

void CTrendGrid::SetGroupIndex(int nIndex)
{
	ASSERT(nIndex >= 0);
    
	if(m_nGroupIndex != nIndex)
	{
		m_nGroupIndex = nIndex;
		m_bSettingsChanged = TRUE;
	}
}

void CTrendGrid::OnTrendGridSettingsChanged()
{
	m_bSettingsChanged = TRUE;
}

void CTrendGrid::SetTrendGridCell(int nRowIndex, int nColIndex)
{
	ASSERT(m_hGraphGrid != INVALID_HANDLE_VALUE);
    
	if(nColIndex == -1)
	{
		// 去除连接线信息
		m_lpConnectValueArray[nRowIndex] = 0xFFFF;
		return ;
	}
    
	ASSERT(nRowIndex >= 0 && nColIndex >= 0 && nColIndex < GetColumnCount());
    
	// 保存
	m_lpConnectValueArray[nRowIndex] = (WORD)nColIndex;
	
	CGraphGrid::SetCellBitmap(nRowIndex, nColIndex, nColIndex);
}

int CTrendGrid::GetTrendGridCell(int nRowIndex)
{
	ASSERT(m_lpConnectValueArray != NULL);
	ASSERT(nRowIndex >= 0);
    
	return m_lpConnectValueArray[nRowIndex];
}

void CTrendGrid::FinishSetValue()
{
#ifdef _FREE		// 免费版
	if(m_bIsHideAllCells)
	{
		return ;
	}
#endif
	// 创建历史数据统计接口进行统计
	CHistoryValueStat cStat;
	int nRowIndex = 0, nColIndex = 0, nDataItemCount = GetRowCount(RCT_DATA), nColCount = GetColumnCount();
	cStat.Create(nDataItemCount);
	
	for(nColIndex=0; nColIndex<nColCount; nColIndex++)
	{
		if(m_lpColInfo[nColIndex].szBallText[0] == 0)
		{
			// 设置统计值
			m_cGraphGridTail.SetColumnStatResult(nColIndex, NULL);
			continue;	// 空列，无需统计
		}
		
		// 根据列中数据设置数值
		for(nRowIndex=0; nRowIndex<nDataItemCount; nRowIndex++)
		{
			if(m_cGraphGridBody.GetCellValueType(nRowIndex, nColIndex) != CGCT_NULL)
			{
				cStat.SetValue(nRowIndex);
			}
		}
		
		// 统计
		cStat.Stat(0, TRUE);
		
		// 设置遗漏值
		for(nRowIndex=0; nRowIndex<nDataItemCount; nRowIndex++)
		{
			if(cStat.GetValue(nRowIndex) < 0)
			{
				m_cGraphGridBody.SetCellInt(nRowIndex, nColIndex, 0 - cStat.GetValue(nRowIndex), CGCT_INT_SKIP);
			}
		}
		
		// 设置统计值
		m_cGraphGridTail.SetColumnStatResult(nColIndex, &cStat);
		
		// 重设所有数据
		cStat.ResetAllValue();
	}
	
	cStat.Destroy();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CTrendGrid::Create(CGPoint /*ptOrg*/, int /*nDataRowCount*/, int /*nColCount*/)
{
	ASSERT(FALSE);	// 禁用该函数，请调用CreateTrendGrid进行创建
    
	return FALSE;
}

void CTrendGrid::Destroy()
{
	DELETE_BUFFER(m_lpConnectValueArray);
    
	CGraphGrid::Destroy();
}

void CTrendGrid::SetForecastColIndex(int nColIndex)
{
	int nForecastRow = GetForecastRow();
    
	BOOL bExist = m_cGraphGridBody.GetCellValueType(nForecastRow, nColIndex) == CGCT_NULL ? FALSE : TRUE;
    
	// 清空当前的预测列信息	
	ClearForecastRow();
    
	// 重新设置预测列
	if(!bExist)
	{
		SetTrendGridCell(nForecastRow, nColIndex);
	}
}

void CTrendGrid::Draw(CGContextRef context)
{
	if(!_PrepareDraw(context))
	{
		return ;
	}
    
	// 画背景和表体
	CGraphGrid::_DrawGraphGridPart(context, DF_CELL_BKGND | DF_GRIDBODY);
	
    LPTRENDGRIDSET lpTrendGridSet = m_cGraphGridPublic.GetTrendGridSettings();
	if(lpTrendGridSet->dwFuncMask & TGSFM_CONNECTLINE_SHOW)
	{
		// 画连接线
		CGPoint ptPrev, ptCur;
		
		int nViewBeginRow = 0, nViewEndRow = 0, nRowCount = GetRowCount(RCT_DATA | RCT_FORECAST);
		m_cGraphGridBody.GetViewRow(&nViewBeginRow, &nViewEndRow);
		
		int nBeginRow = (nViewBeginRow <= 0) ? 1 : nViewBeginRow;
		int nEndRow = (nViewEndRow > nRowCount) ? nRowCount : nViewEndRow;
        BOOL bAltColor = (lpTrendGridSet->dwFuncMask & TGSFM_CONNECTLINE_ALTCOLOR) ? TRUE : FALSE;
        
		CGContextSetLineWidth(context, lpTrendGridSet->btPenWidth);
        CGRect rectCell = CGRectZero; 
		for(int i=nBeginRow; i<nEndRow; i++)
		{
			if(m_lpConnectValueArray[i] == 0XFFFF || m_lpConnectValueArray[i - 1] == 0XFFFF)
			{
				continue;  // 不需要连接的了
			}
            rectCell = GetCellRect(i-1, m_lpConnectValueArray[i - 1]);
			ptPrev.x = CGRectGetMidX(rectCell);
            ptPrev.y = CGRectGetMidY(rectCell);
            rectCell = GetCellRect(i, m_lpConnectValueArray[i]);
			ptCur.x = CGRectGetMidX(rectCell);
            ptCur.y = CGRectGetMidY(rectCell);
			
			// 画连接线
            if(bAltColor)   // 启用交替颜色
            {
                if(((nRowCount - 1 - i) / lpTrendGridSet->btAltCount) % 2 == 1)
                    [lpTrendGridSet->crAltColor[1] setStroke];
                else
                    [lpTrendGridSet->crAltColor[0] setStroke];
            }
            else
            {
                [lpTrendGridSet->crPenColor setStroke];
            }
            
            CGContextMoveToPoint(context, ptPrev.x, ptPrev.y);
            CGContextAddLineToPoint(context, ptCur.x, ptCur.y);
            CGContextStrokePath(context);
		}
	}
	
	// 画其他的
	CGraphGrid::_DrawGraphGridPart(context, DF_CELL_VALUE | DF_GRIDHEAD | DF_GRIDTAIL);
}

void CTrendGrid::AutoSizeColumn(int nColIndex)
{
	if(m_lpColInfo[nColIndex].nWidth != m_lpGridSet->wRowHeight)
	{
		// 设置新的宽度
		m_lpColInfo[nColIndex].nWidth = m_lpGridSet->wRowHeight;
	}
	
	m_bSettingsChanged = TRUE;
    
	_NeedRecalcRect();
}

void CTrendGrid::ClearForecastRow()
{
	CGraphGrid::ClearForecastRow();
    
    int nForecastRow = GetForecastRow();
    
    if(nForecastRow == -1)
    {
        return;
    }
    
	// 去除连接线信息
	m_lpConnectValueArray[nForecastRow] = MAXWORD;
}

/////////////////////////////////////////////////////////////////////////////////////
/*BOOL CTrendGrid::_GetCellPromptInfo(LPSTR lpszPromptText, int nRowIndex, int nColIndex, BYTE btCellType)
{
	ASSERT(lpszPromptText != NULL);
	
	switch(btCellType)
	{
        case CPICT_HEAD_TITLE:
        case CPICT_TAIL_TITLE: sprintf(lpszPromptText, "%s走势图", m_szTitle); break;
        case CPICT_ROW_DATA: 
		{
			switch(GetCellValueType(nRowIndex, nColIndex, FALSE))
			{
                case CGCT_INT_SKIP:
                    sprintf(lpszPromptText, "“%s”的遗漏值：%d", m_lpColInfo[nColIndex].szInfoTip, GetCellInt(nRowIndex, nColIndex));
                    break;
                case CGCT_BITMAP_INDEX: 
                    sprintf(lpszPromptText, "当前值为：%s", m_lpColInfo[nColIndex].szInfoTip);
                    break;
			}
		} break;
        case CPICT_ROW_FORECAST: 
            if(IsSurpportForecast(nColIndex))
            {
                sprintf(lpszPromptText, "点击此处可以设置预测值为：%s，再次点击则可以清除该预测值", m_lpColInfo[nColIndex].szInfoTip);
            }
            break;
        default: return CGraphGrid::_GetCellPromptInfo(lpszPromptText, nRowIndex, nColIndex, btCellType);
	}
	
	return TRUE;
}
*/
/////////////////////////////////////////////////////////////////////////////////////
BOOL CTrendGrid::_PrepareDraw(CGContextRef context)
{
	if(!m_bSettingsChanged)		// 配置没有改变，则直接返回成功
		return TRUE;
    
    LPTRENDGRIDSET lpTrendGridSet = m_cGraphGridPublic.GetTrendGridSettings();
	int nIndex = m_nGroupIndex % 4;
	CGraphGrid::SetBkColor(lpTrendGridSet->crBacks[nIndex], SBI_GRID);
    
	int nColCount = GetColumnCount();
    
	// 重新创建所有号码球
	m_cGraphGridPublic.RemoveAllBallBitmaps();
    
	int nBallSize = m_lpGridSet->wRowHeight / 2 - 1;
	if(lpTrendGridSet->wBallSize != MAXWORD)	// 自定义号码球尺寸
	{
		nBallSize = lpTrendGridSet->wBallSize;
	}
    
	for(int i=0; i<nColCount; i++)
	{
        m_cGraphGridPublic.AddBallBitmap(nBallSize, m_lpColInfo[i].szBallText, lpTrendGridSet->fontBallTexts[nIndex], lpTrendGridSet->crBallBks[nIndex], lpTrendGridSet->crBallBorders[nIndex], lpTrendGridSet->crBallTexts[nIndex]);
	}
    
	m_bSettingsChanged = FALSE;
    
	return TRUE;
}
