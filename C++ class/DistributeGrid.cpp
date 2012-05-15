//
//  DistributeGrid.cpp
//  Graph
//
//  Created by 武 孙 on 12-5-7.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "DistributeGrid.h"
#include "HistoryValueStat.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDistributeGrid::CDistributeGrid()
{
	m_bSettingsChanged = FALSE;
	m_nGroupIndex = 0;
}

CDistributeGrid::~CDistributeGrid()
{
	Destroy();
}

BOOL CDistributeGrid::CreateDistributeGrid(int nColCount, int nDataRowCount)
{
	if(!CGraphGrid::Create(CGPointZero, nDataRowCount, nColCount))
	{
		return FALSE;
	}
	
	OnDistributeGridSettingsChanged();
    
	return TRUE;
}

void CDistributeGrid::SetGroupIndex(int nIndex)
{
	ASSERT(nIndex >= 0);
    
	if(m_nGroupIndex != nIndex)
	{
		m_nGroupIndex = nIndex;
		m_bSettingsChanged = TRUE;
	}
}

void CDistributeGrid::OnDistributeGridSettingsChanged()
{
	m_bSettingsChanged = TRUE;
}

void CDistributeGrid::SetDistributeGridCell(int nRowIndex, int nColIndex, BYTE btValueCount)
{
	ASSERT(nRowIndex >= 0 && nRowIndex < GetRowCount(RCT_DATA | RCT_FORECAST));
	ASSERT(nColIndex >= 0 && nColIndex < GetColumnCount());
	
	int nBitmapIndex = ((btValueCount < 4) ? (btValueCount - 1) : 3 ) * GetColumnCount() + nColIndex ;
	CGraphGrid::SetCellBitmap(nRowIndex, nColIndex, nBitmapIndex, btValueCount);
}

void CDistributeGrid::FinishSetValue()
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
		BYTE btValueCount = 0;
		for(nRowIndex=0; nRowIndex<nDataItemCount; nRowIndex++)
		{
			if(m_cGraphGridBody.GetCellBitmap(nRowIndex, nColIndex, &btValueCount) != -1)
			{
				cStat.SetValue(nRowIndex, btValueCount);
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
BOOL CDistributeGrid::Create(CGPoint /*ptOrg*/, int /*nDataRowCount*/, int /*nColCount*/)
{
	ASSERT(FALSE);	// 禁用该函数，请调用CreateDistributeGrid进行创建
	
	return FALSE;
}

void CDistributeGrid::Destroy()
{	
	CGraphGrid::Destroy();
}

void CDistributeGrid::SetForecastColIndex(int nColIndex)
{
	int nForecastRow = CGraphGrid::GetForecastRow();
	
	if(m_cGraphGridBody.GetCellValueType(nForecastRow, nColIndex) == CGCT_NULL)
	{
		// 设置
		m_cGraphGridBody.SetCellBitmap(nForecastRow, nColIndex, nColIndex);
	}
	else
	{
		// 清除
		m_cGraphGridBody.ClearForecastRow(nColIndex);
	}
}

void CDistributeGrid::Draw(CGContextRef context)
{
	if(!_PrepareDraw(context))
	{
		return ;
	}
    
	CGraphGrid::Draw(context);
}

void CDistributeGrid::AutoSizeColumn(int nColIndex)
{
	if(m_lpColInfo[nColIndex].nWidth != m_lpGridSet->wRowHeight)
	{
		// 设置新的宽度
		m_lpColInfo[nColIndex].nWidth = m_lpGridSet->wRowHeight;
	}
	
	m_bSettingsChanged = TRUE;
    
	_NeedRecalcRect();
}

/////////////////////////////////////////////////////////////////////////////////////
/*BOOL CDistributeGrid::_GetCellPromptInfo(LPSTR lpszPromptText, int nRowIndex, int nColIndex, BYTE btCellType)
{
	ASSERT(lpszPromptText != NULL);
	
	switch(btCellType)
	{
        case CPICT_HEAD_TITLE:
        case CPICT_TAIL_TITLE: sprintf(lpszPromptText, "%s分布图", m_szTitle); break;
        case CPICT_ROW_DATA: 
		{
			switch(GetCellValueType(nRowIndex, nColIndex, FALSE))
			{
                case CGCT_INT_SKIP:
                    sprintf(lpszPromptText, "“%s”的遗漏值：%d", m_lpColInfo[nColIndex].szInfoTip, GetCellInt(nRowIndex, nColIndex));
                    break;
                case CGCT_BITMAP_INDEX: 
				{
					BYTE btValueCount = 0;
					GetCellBitmap(nRowIndex, nColIndex, &btValueCount);
					sprintf(lpszPromptText, "“%s”出现%d次", m_lpColInfo[nColIndex].szInfoTip, btValueCount);
				}
                    break;
			}
		} break;
        case CPICT_ROW_FORECAST: 
            if(IsSurpportForecast(nColIndex))
            {
                if(GetCellValueType(nRowIndex, nColIndex, FALSE) == CGCT_BITMAP_INDEX)
                    sprintf(lpszPromptText, "点击此处可以清除该预测值，再次点击则可以重新设置该预测值为：%s", m_lpColInfo[nColIndex].szBallText);
                else
                    sprintf(lpszPromptText, "点击此处可以设置预测值为：%s，再次点击则可以清除该预测值", m_lpColInfo[nColIndex].szBallText);
            }
            break;
        default: return CGraphGrid::_GetCellPromptInfo(lpszPromptText, nRowIndex, nColIndex, btCellType);
	}
	
	return TRUE;
}
*/
/////////////////////////////////////////////////////////////////////////////////////
BOOL CDistributeGrid::_PrepareDraw(CGContextRef context)
{	
	if(!m_bSettingsChanged)		// 配置没有改变，则直接返回成功
		return TRUE;
	
    LPDISTRIBUTEGRIDSET lpDistributeGridSet = m_cGraphGridPublic.GetDistributeGridSettings();
    
	CGraphGrid::SetBkColor(lpDistributeGridSet->crBacks[m_nGroupIndex % 4], SBI_GRID);
    
	int nColCount = GetColumnCount();
	
	// 重新创建所有号码球
	m_cGraphGridPublic.RemoveAllBallBitmaps();
	
	int nBallSize = m_lpGridSet->wRowHeight / 2 - 1;
	
	if(lpDistributeGridSet->wBallSize != MAXWORD)	// 自定义号码球尺寸
	{
		nBallSize = lpDistributeGridSet->wBallSize;
	}
    
	for(int j=0; j<4; j++)
	{
		for(int i=0; i<nColCount; i++)
		{
            m_cGraphGridPublic.AddBallBitmap(nBallSize, m_lpColInfo[i].szBallText, lpDistributeGridSet->fontBallText[j], lpDistributeGridSet->crBallBk[j], lpDistributeGridSet->crBallBorder[j], lpDistributeGridSet->crBallText[j]);
		}
	}
	
	m_bSettingsChanged = FALSE;
    
	return TRUE;
}
