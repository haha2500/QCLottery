//
//  GraphGridHead.cpp
//  Graph
//
//  Created by 武 孙 on 12-5-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GraphGridHead.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGraphGridHead::CGraphGridHead()
{
	m_pGraphGridPublic = NULL;
    
	m_bIsHead = TRUE;
	m_bIsStatHead = FALSE;
	m_btShowFlag = 0;
    
	m_ptOrg = CGPointMake(-1, -1);
}

CGraphGridHead::~CGraphGridHead()
{
	Destroy();
}

BOOL CGraphGridHead::Create(CGraphGridPublic *pGraphGridPublic, BOOL bIsHead, BOOL bIsStatHead)
{
	ASSERT(m_pGraphGridPublic == NULL && pGraphGridPublic != NULL);
	m_pGraphGridPublic = pGraphGridPublic;
    
	m_bIsHead = bIsHead;
	m_bIsStatHead = bIsStatHead;
	
	int nColCount = m_pGraphGridPublic->GetColumnCount();
    
	if(!m_bIsHead)	// 如果是表尾，则创建统计信息格
	{
		LPGRAPHGRIDSET lpGridSet = m_pGraphGridPublic->GetGridSettings();
        
		if(m_bIsStatHead && (nColCount > 0))	// 统计表头，号码表下的统计名
		{
			if(!m_cGridStat.Create(m_pGraphGridPublic, lpGridSet->btStatItemCount, 1, TRUE))
			{
				return FALSE;
			}
		}
		else // 统计表身，即非号码表下面的统计信息
		{
			if(!m_cGridStat.Create(m_pGraphGridPublic, lpGridSet->btStatItemCount, nColCount, TRUE))
			{
				return FALSE;
			}
		}
	}
    
	SetBkColor([UIColor whiteColor]);
    
	m_rectView = CGRectZero;
    
	return TRUE;
}

void CGraphGridHead::Draw(CGContextRef context)
{
	ASSERT(m_pGraphGridPublic != NULL);
    
	RecalcRect();
    
    if(!CGRectIntersectsRect(m_rectSelf, m_rectView))
	{
		return ;		// 不可见
	}
    
	LPGRAPHGRIDSET lpGridSet = m_pGraphGridPublic->GetGridSettings();
    [lpGridSet->crBorderPen setStroke];
    
	int nBeginX = 0, nEndX = 0, nBeginY = 0, nEndY = 0;
	if(m_btShowFlag & (GHSF_TITLE | GHSF_COLUMN))
	{
		nBeginX = m_rectSelf.origin.x;
		nEndX = m_rectSelf.origin.x + m_rectSelf.size.width - 1;
        
		if(m_bIsHead)	// 作为表头
		{	
			// 画头部边框
			nBeginY = nEndY = m_rectSelf.origin.y;
		}
		else
		{
			// 画底部边框
			nBeginY = nEndY = m_rectSelf.origin.y + m_rectSelf.size.height - lpGridSet->btBorderPenWidth;
		}
        
		m_pGraphGridPublic->DrawLine(context, lpGridSet->btBorderPenWidth, nBeginX, nBeginY, nEndX, nEndY, TRUE);
        
		nBeginY = m_rectSelf.origin.y;
		nEndY = m_rectSelf.origin.y + m_rectSelf.size.height - 1;
        
		// 画左部边框
		nBeginX = nEndX = m_rectSelf.origin.x;
		m_pGraphGridPublic->DrawLine(context, lpGridSet->btBorderPenWidth, nBeginX, nBeginY, nEndX, nEndY, FALSE);
        
		// 画右部边框
		nBeginX = nEndX = m_rectSelf.origin.x + m_rectSelf.size.width - lpGridSet->btBorderPenWidth;
		m_pGraphGridPublic->DrawLine(context, lpGridSet->btBorderPenWidth, nBeginX, nBeginY, nEndX, nEndY, FALSE);
	}
	
	if(m_btShowFlag & GHSF_COLUMN)
	{
        [lpGridSet->crBodyPen setStroke];
       
		// 画分隔线
		nBeginX = m_rectSelf.origin.x;
		nEndX = m_rectSelf.origin.x + m_rectSelf.size.width - 1;
        
		if(m_bIsHead)		// 作为表头
		{
			nBeginY = nEndY = m_rectSelf.origin.y + lpGridSet->btBorderPenWidth + (m_btShowFlag & GHSF_TITLE ? lpGridSet->wRowHeight : 0);
		}
		else	// 作为表尾
		{
			nBeginY = m_rectSelf.origin.y + m_rectSelf.size.height - lpGridSet->btBorderPenWidth;
			if(m_btShowFlag & GHSF_TITLE)
				nBeginY -= lpGridSet->wRowHeight;
			if(!(m_btShowFlag & GHSF_STATGRID))
				nBeginY -= lpGridSet->btBodyPenWidth;
			nEndY = nBeginY;
		}
        
		m_pGraphGridPublic->DrawLine(context, lpGridSet->btBodyPenWidth, nBeginX, nBeginY, nEndX, nEndY, TRUE);
        
		// 画单元格线
		int nColCount = m_pGraphGridPublic->GetColumnCount();
		BYTE btRightBorderWidth = 0;
		nBeginX = nEndX = m_rectSelf.origin.x + lpGridSet->btBorderPenWidth;
        
		if(m_bIsHead)	// 作为表头
		{
			nEndY = nBeginY + lpGridSet->wRowHeight + lpGridSet->btBodyPenWidth - 1;
		}
		else	// 作为表尾
		{
			nBeginY = m_rectSelf.origin.y - 1;
			nEndY = nBeginY + lpGridSet->wRowHeight;
		}
		for(int i=0; i<nColCount-1; i++)
		{
			nBeginX += m_pGraphGridPublic->GetColumnWidth(i, &btRightBorderWidth);
			nEndX = nBeginX;
			m_pGraphGridPublic->DrawLine(context, lpGridSet->btBodyPenWidth+btRightBorderWidth, nBeginX, nBeginY, nEndX, nEndY, FALSE);
			nBeginX += (btRightBorderWidth + lpGridSet->btBodyPenWidth);
		}
	}
    
	if(m_btShowFlag & GHSF_TITLE)
	{
		// 画标题
		_DrawTitle(context);
	}
    
	if(m_btShowFlag & GHSF_COLUMN)
	{
		// 画列名
		_DrawAllColsName(context);
	}
    	
	// 画统计信息
	if(m_btShowFlag & GHSF_STATGRID)
	{
		m_cGridStat.Draw(context);
	}
}

void CGraphGridHead::RecalcRect()
{
	if(!m_bNeedRecalcRect)
		return ;
    
	ASSERT(m_pGraphGridPublic != NULL);
	LPGRAPHGRIDSET lpGridSet = m_pGraphGridPublic->GetGridSettings();
    
	int i = 0, nCount = 0, nColCount = m_pGraphGridPublic->GetColumnCount();
	BYTE btRightBorderWidth = 0;
	if(nColCount > 0)
	{
		for(i=0; i<nColCount - 1; i++)
		{
			nCount += m_pGraphGridPublic->GetColumnWidth(i, &btRightBorderWidth);
			nCount += (lpGridSet->btBodyPenWidth + btRightBorderWidth);
		}
		nCount += m_pGraphGridPublic->GetColumnWidth(i, NULL);
	}
	m_rectSelf = CGRectMake(m_ptOrg.x, m_ptOrg.y, nCount + lpGridSet->btBorderPenWidth * 2, 0);
	
	if(m_bIsHead)	// 表头
	{
		if(m_btShowFlag & GHSF_TITLE)
		{
			m_rectSelf.size.height += lpGridSet->wRowHeight;
		}
		if(m_btShowFlag & GHSF_COLUMN)
		{
			m_rectSelf.size.height += lpGridSet->wRowHeight;
		}	
		if(IS_INCLUDE_ALL_FLAG(m_btShowFlag, GHSF_TITLE | GHSF_COLUMN))
		{
			m_rectSelf.size.height += lpGridSet->btBodyPenWidth;	// 标题和列分割线
		}
	}
	else	// 表尾
	{
		if(m_btShowFlag & GHSF_COLUMN)
		{
			m_rectSelf.size.height += lpGridSet->wRowHeight;
		}	
		if(m_btShowFlag & GHSF_STATGRID)	// 含统计表格
		{
			m_rectSelf.size.height += (m_cGridStat.GetRect().size.height);
		}
		else
		{	
			if(m_btShowFlag & GHSF_TITLE )
			{
				if(m_btShowFlag & GHSF_COLUMN)
				{
					m_rectSelf.size.height += lpGridSet->btBodyPenWidth;	// 标题和列分割线
				}
			}
		}
		if(m_btShowFlag & GHSF_TITLE)
		{
			m_rectSelf.size.height += lpGridSet->wRowHeight;
		}
	}
	
	if(m_rectSelf.size.height > 0)
	{
		m_rectSelf.size.height += lpGridSet->btBorderPenWidth;	// 边框
	}
    
	m_bNeedRecalcRect = FALSE;
}

void CGraphGridHead::Destroy()
{
	if(m_pGraphGridPublic == NULL)
	{
		return ;
	}
    
	m_cGridStat.Destroy();
    
	m_ptOrg = CGPointMake(-1, -1);
	m_rectSelf = m_rectView = CGRectZero;
    
	m_pGraphGridPublic = NULL;
}

void CGraphGridHead::SetBkColor(UIColor *bkColor)
{
	m_crBkColor = bkColor;
}

void CGraphGridHead::SetOrg(int x, int y)
{
	if(m_pGraphGridPublic == NULL)
	{
		ASSERT(FALSE);
		return ;
	}
    
	LPGRAPHGRIDSET lpGridSet = m_pGraphGridPublic->GetGridSettings();
    
	if(m_btShowFlag & GHSF_STATGRID)	// 含有统计表格
	{
		// 设置统计表格位置
		int nY = y;
		if(m_btShowFlag & GHSF_COLUMN)	// 含有列
		{
			nY += lpGridSet->wRowHeight;
		}
		else	// 不含列，则需要将统计表上边框和表体的上边框重合
		{
			nY -= lpGridSet->btBorderPenWidth;
		}
        m_cGridStat.SetOrg(x, nY);
	}
    
	if(m_ptOrg.x == x && m_ptOrg.y == y)
	{
		return ;		// 没有改变
	}
    
	m_ptOrg = CGPointMake(x, y);
	
	m_bNeedRecalcRect = TRUE;
}

void CGraphGridHead::SetViewRect(CGRect &rectView)
{
	if(CGRectEqualToRect(m_rectView, rectView))
	{
		return ;		// 如果没有改变可见矩形范围
	}
    
	m_rectView = rectView;
    
	if(m_btShowFlag & GHSF_STATGRID)
		m_cGridStat.SetViewRect(rectView);
}

CGRect CGraphGridHead::GetRect()
{
	ASSERT(m_pGraphGridPublic != NULL);
    
	RecalcRect();
    
	return m_rectSelf;
}

CGRect CGraphGridHead::GetCellRect(int nColIndex)
{
	ASSERT(m_pGraphGridPublic != NULL);
    
	LPGRAPHGRIDSET lpGridSet = m_pGraphGridPublic->GetGridSettings();
	CGRect rect = CGRectZero;
    
	if(nColIndex == -1)		// 获取标题区域
	{
		if(m_btShowFlag & GHSF_TITLE)	// 含标题
		{
			rect = GetRect();
			rect.origin.x += lpGridSet->btBorderPenWidth;
			rect.size.width -= 2 * lpGridSet->btBorderPenWidth;
			if(m_bIsHead)	// 作为表头 
			{
				rect.origin.y += lpGridSet->btBorderPenWidth;
				rect.size.height = lpGridSet->wRowHeight;
			}
			else // 作为表尾
			{
                rect.origin.y += (rect.size.height - lpGridSet->wRowHeight - lpGridSet->btBorderPenWidth);
				rect.size.height = lpGridSet->wRowHeight;
            }
		}
	}
	else	// 获取指定列名区域
	{
		if(m_btShowFlag & GHSF_COLUMN)	// 含列名
		{
			int nColCount = m_pGraphGridPublic->GetColumnCount();
			ASSERT(nColIndex >= 0 && nColIndex < nColCount);
			rect = GetRect();
            
			if(m_bIsHead)	// 作为表头
			{
				rect.origin.y += lpGridSet->btBorderPenWidth;
				if(m_btShowFlag & GHSF_TITLE)	// 含标题
				{
					rect.origin.y += (lpGridSet->wRowHeight + lpGridSet->btBodyPenWidth);
				}
			}
			rect.size.height = lpGridSet->wRowHeight;
            
			BYTE btExtWidth = 0;
			rect.origin.x += lpGridSet->btBorderPenWidth;
			for(int i=0; i<nColIndex; i++)
			{
				rect.origin.x += m_pGraphGridPublic->GetColumnWidth(i, &btExtWidth);
				rect.origin.x += (btExtWidth + lpGridSet->btBodyPenWidth);
			}
			rect.size.width = m_pGraphGridPublic->GetColumnWidth(nColIndex, &btExtWidth);
		}
	}
    
    rect.origin.y --;   // 由于Y坐标没有0值，所以需要1个偏移点
	return rect;
}

void CGraphGridHead::OnGridSettingsChanged()
{
	ASSERT(m_pGraphGridPublic != NULL);
	LPGRAPHGRIDSET lpGridSet = m_pGraphGridPublic->GetGridSettings();
    
	if(m_bIsHead)	// 作为表头
	{
		m_btShowFlag = (BYTE)(((lpGridSet->dwShowMask & GGSSM_HEAD_TITLE) ? GHSF_TITLE : 0) + ((lpGridSet->dwShowMask & GGSSM_HEAD_COLUMN) ? GHSF_COLUMN : 0));
	}
	else			// 作为表尾
	{
		m_btShowFlag = (BYTE)(((lpGridSet->dwShowMask & GGSSM_TAIL_TITLE) ? GHSF_TITLE : 0) + ((lpGridSet->dwShowMask & GGSSM_TAIL_COLUMN) ? GHSF_COLUMN : 0) + (((lpGridSet->dwShowMask & GGSSM_STAT) && lpGridSet->btStatItemCount > 0) ? GHSF_STATGRID : 0));
	}
    
	m_bNeedRecalcRect = TRUE;
}

void CGraphGridHead::SetColumnStatResult(int nColIndex, CHistoryValueStat *pHistoryValueStat)
{
	LPGRAPHGRIDSET lpGridSet = m_pGraphGridPublic->GetGridSettings();
    
	if(m_bIsStatHead)	// 统计表头
	{	
		for(int i=0; i<lpGridSet->btStatItemCount; i++)
		{
			m_cGridStat.SetCellText(i, 0, lpGridSet->stStatItems[i].szText);
		}
	}
	else				// 统计表身
	{
		if(pHistoryValueStat != NULL)
		{
			LPSTATRESULT lpStatResult = &m_pGraphGridPublic->GetColumnInfo(nColIndex)->stStatResult;
			*lpStatResult = *pHistoryValueStat->GetStatResult();
            
			for(int i=0; i<lpGridSet->btStatItemCount; i++)
			{
				switch(lpGridSet->stStatItems[i].dwID)
				{
                    case HVT_MatchCount:			// 出现次数
                        m_cGridStat.SetCellInt(i, nColIndex, lpStatResult->dwMatchCount, CGCT_INT_STAT);
                        break;
                    case HVT_MatchProb:				// 出现概率
                        m_cGridStat.SetCellFloat(i, nColIndex, (float)lpStatResult->fMatchProb, CGCT_FLOAT_STAT);
                        break;
                    case HVT_CurSkip:				// 当前遗漏
                        m_cGridStat.SetCellInt(i, nColIndex, lpStatResult->dwCurSkip, CGCT_INT_STAT);
                        break;
                    case HVT_PrevSkip:				// 上期遗漏
                        m_cGridStat.SetCellInt(i, nColIndex, lpStatResult->dwPrevSkip, CGCT_INT_STAT);
                        break;
                    case HVT_AvgSkip:				// 平均遗漏
					{
						int nAvgSkip = int(lpStatResult->fAvgSkip);
						if(lpStatResult->fAvgSkip - nAvgSkip >= 0.5)
						{
							nAvgSkip ++;
						}
						m_cGridStat.SetCellInt(i, nColIndex, nAvgSkip, CGCT_INT_STAT);
					} break;
                    case HVT_MinSkip:				// 最小遗漏
                        m_cGridStat.SetCellInt(i, nColIndex, lpStatResult->dwMinSkip, CGCT_INT_STAT);
                        break;
                    case HVT_MinSkipNo0:			// 最小遗漏(不含0)
                        m_cGridStat.SetCellInt(i, nColIndex, lpStatResult->dwMinSkipNo0, CGCT_INT_STAT);
                        break;
                    case HVT_MaxSkip:				// 最大遗漏
                        m_cGridStat.SetCellInt(i, nColIndex, lpStatResult->dwMaxSkip, CGCT_INT_STAT);
                        break;
                    case HVT_CurLink:				// 当前连出
                        m_cGridStat.SetCellInt(i, nColIndex, lpStatResult->dwCurLink, CGCT_INT_STAT);
                        break;
                    case HVT_PrevLink:				// 上期连出
                        m_cGridStat.SetCellInt(i, nColIndex, lpStatResult->dwPrevLink, CGCT_INT_STAT);
                        break;
                        //				case STAT_AVGLINK:				// 平均连出
                        //					m_cGridStat.SetCellInt(i, nColIndex, lpStatResult->dwMinLinkSkip, CGCT_INT_STAT);
                        //					break;
                    case HVT_MinLink:				// 最小连出
                        m_cGridStat.SetCellInt(i, nColIndex, lpStatResult->dwMinLink, CGCT_INT_STAT);
                        break;
                    case HVT_MinLinkNo1:			// 最小连出(不含1)
                        m_cGridStat.SetCellInt(i, nColIndex, lpStatResult->dwMinLinkNo1, CGCT_INT_STAT);
                        break;
                    case HVT_MaxLink:				// 最大连出
                        m_cGridStat.SetCellInt(i, nColIndex, lpStatResult->dwMaxLink, CGCT_INT_STAT);
                        break;
                    case HVT_CurLinkSkip:			// 当前连出遗漏
                        m_cGridStat.SetCellInt(i, nColIndex, lpStatResult->dwCurLinkSkip, CGCT_INT_STAT);
                        break;
                    case HVT_MinLinkSkip:			// 最小连出遗漏
                        m_cGridStat.SetCellInt(i, nColIndex, lpStatResult->dwMinLinkSkip, CGCT_INT_STAT);
                        break;
                    case HVT_MaxLinkSkip:			// 最大连出遗漏
                        m_cGridStat.SetCellInt(i, nColIndex, lpStatResult->dwMaxLinkSkip, CGCT_INT_STAT);
                        break;
                    case HVT_RealSkipValue:			// 实际遗漏价值
                        m_cGridStat.SetCellFloat(i, nColIndex, (float)lpStatResult->fRealSkipValue, CGCT_FLOAT_STAT);
                        break;
                    case HVT_ExtremeSkewIndex:		// 极值偏态指数
                        m_cGridStat.SetCellFloat(i, nColIndex, (float)lpStatResult->fExtremeSkewnessIndex, CGCT_FLOAT_STAT);
                        break;
                    case HVT_RealSkewIndex:			// 实际偏态指数
                        m_cGridStat.SetCellFloat(i, nColIndex, (float)lpStatResult->fRealSkewnessIndex, CGCT_FLOAT_STAT);
                        break;
                        
                    default: ASSERT(FALSE); break;
				}
                
				// 设置颜色
				m_cGridStat.SetCellColor(i, nColIndex, pHistoryValueStat->GetStatValueColor(lpGridSet->stStatItems[i].dwID));
			}
		}
	}
}

DWORD CGraphGridHead::HitTest(CGPoint pt, int *pRowIndex, int *pColIndex)
{
	if(!CGRectContainsPoint(m_rectSelf, pt))
	{
		return 0;
	}
	
	*pRowIndex = 0;
	*pColIndex = 0;
    
	if(m_btShowFlag & GHSF_TITLE)
	{
		if(CGRectContainsPoint(GetCellRect(-1), pt))
		{
			return m_bIsHead ? GRIDHT_HEAD_TITLE : GRIDHT_TAIL_TITLE;			// 标题区域
		}
	}
    
	if(m_btShowFlag & GHSF_COLUMN)
	{
		for(int i=m_pGraphGridPublic->GetColumnCount()-1; i>=0; i--)
		{
			if(CGRectContainsPoint(GetCellRect(i), pt))
			{
				*pColIndex = i;
				return m_bIsHead ? GRIDHT_HEAD_COLUMN : GRIDHT_TAIL_COLUMN;		// 列名区域
			}
		}
	}
	
	if(m_btShowFlag & GHSF_STATGRID)
	{
		return m_cGridStat.HitTest(pt, pRowIndex, pColIndex);
	}
    
	return m_bIsHead ? GRIDHT_HEAD_BORDER : GRIDHT_TAIL_BORDER;
}

////////////////////////////////////////////////////////////////////////////////////////

void CGraphGridHead::_DrawTitle(CGContextRef context)
{
	ASSERT(m_pGraphGridPublic != NULL);
	LPGRAPHGRIDSET lpGridSet = m_pGraphGridPublic->GetGridSettings();
    CGRect rectTitle = GetCellRect(-1);
 
    m_pGraphGridPublic->DrawText(context, rectTitle, m_pGraphGridPublic->GetTitle(), lpGridSet->fontTitle, lpGridSet->crTitleText, lpGridSet->crTitleBK);
}

void CGraphGridHead::_DrawAllColsName(CGContextRef context)
{
	ASSERT(m_pGraphGridPublic != NULL);
	LPGRAPHGRIDSET lpGridSet = m_pGraphGridPublic->GetGridSettings();
    
	int nColCount = m_pGraphGridPublic->GetColumnCount();
	CGRect rectCol;
	for(int i=0; i<nColCount; i++)
	{
		rectCol = GetCellRect(i);
        m_pGraphGridPublic->DrawText(context, rectCol, m_pGraphGridPublic->GetColumnName(i), lpGridSet->fontColumn, lpGridSet->crColumnText, lpGridSet->crColumnBK);
	}
}

