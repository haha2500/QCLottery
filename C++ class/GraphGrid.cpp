//
//  GraphGrid.cpp
//  Graph
//
//  Created by 武 孙 on 12-5-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GraphGrid.h"
#include "IData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGraphGrid::CGraphGrid()
{
	m_lpColInfo = NULL;
	m_ptOrg = CGPointZero;								// 坐标原点
    
	m_lpGridSet = NULL;
	m_bEnableForecast = TRUE;
	ZeroMemory(&m_stGridID, sizeof(m_stGridID));
    
	_Initialize();
}

CGraphGrid::~CGraphGrid()
{
	Destroy();
}

BOOL CGraphGrid::Create(CGPoint ptOrg, int nDataRowCount, int nColCount)
{
	ASSERT(m_hGraphGrid == INVALID_HANDLE_VALUE);
	ASSERT(nDataRowCount >= -1 && nColCount >= 0);
    
	if(nDataRowCount == -1)
    {
		nDataRowCount = g_pIData->GetItemCount();
    }
    
	_Initialize();
		
    // 创建公共信息
	m_cGraphGridPublic.Create(nColCount);
    m_lpGridSet = m_cGraphGridPublic.GetGridSettings();
    m_lpColInfo = m_cGraphGridPublic.GetColumnInfo();
    
    // 创建表头
	if(!m_cGraphGridHead.Create(&m_cGraphGridPublic, TRUE, FALSE))
	{
		return FALSE;
	}

	m_hGraphGrid = (HANDLE)MAKELONG((WORD)ptOrg.x, (WORD)ptOrg.y);
	
	// 创建表身
	if(!m_cGraphGridBody.Create(&m_cGraphGridPublic, nDataRowCount, nColCount))
	{
		return FALSE;
	}
    
	// 创建表尾
	BOOL bIsStatHead = (strstr(GetClassName(), "INumsTable") != NULL) ? TRUE : FALSE;
	if(!m_cGraphGridTail.Create(&m_cGraphGridPublic, FALSE, bIsStatHead))
	{
		return FALSE;
	}
    
	// 设置坐标
	SetOrg(ptOrg.x, ptOrg.y);
    
    OnGridSettingsChanged();
    
	return TRUE;
}

void CGraphGrid::Destroy()
{
	if(m_hGraphGrid == INVALID_HANDLE_VALUE)
	{
		return ;
	}
	
	_ReleaseColumnBuf(-1);
	
	m_cGraphGridHead.Destroy();
	m_cGraphGridTail.Destroy();
	m_cGraphGridBody.Destroy();
    m_cGraphGridPublic.Destroy();
    
	m_ptOrg = CGPointMake(-1, -1);
	m_rectView = CGRectZero;
    
	m_hGraphGrid = INVALID_HANDLE_VALUE;
}

void CGraphGrid::OnGridSettingsChanged()
{
    m_cGraphGridHead.OnGridSettingsChanged();
	m_cGraphGridBody.OnGridSettingsChanged();
	m_cGraphGridTail.OnGridSettingsChanged();
	
	_NeedRecalcRect();
}

int CGraphGrid::GetRowHeight(BOOL bIsStatRow)
{
	return m_cGraphGridPublic.GetRowHeight(bIsStatRow);
}

int CGraphGrid::GetRowCount(BYTE btType)
{
	int nRowCount = (btType & RCT_STAT) ? m_cGraphGridTail.GetStatGrid()->GetRowCount() : 0;
    
	nRowCount += m_cGraphGridBody.GetRowCount(btType);
    
	return nRowCount;
}

int CGraphGrid::GetColumnCount()
{
	return m_cGraphGridPublic.GetColumnCount();
}

void CGraphGrid::SetTitle(LPCSTR lpszTitle)
{
	m_cGraphGridPublic.SetTitle(lpszTitle);
}

LPCSTR CGraphGrid::GetTitle()
{
	return m_cGraphGridPublic.GetTitle();
}

void CGraphGrid::SetColumnName(int nColIndex, LPCSTR lpszName, LPCSTR lpszBallText, LPCSTR lpszInfoTip)
{
    m_cGraphGridPublic.SetColumnName(nColIndex, lpszName, lpszBallText, lpszInfoTip);
}

LPCSTR CGraphGrid::GetColumnName(int nColIndex, LPSTR lpszBallText, LPSTR lpszInfoTip)
{
	return m_cGraphGridPublic.GetColumnName(nColIndex, lpszBallText, lpszInfoTip);
}

void CGraphGrid::SetColumnWidth(int nColIndex, int nWidth, BYTE btRightBorderWidth)
{
	m_cGraphGridPublic.SetColumnWidth(nColIndex, nWidth, btRightBorderWidth);
    
	_NeedRecalcRect();
}

int CGraphGrid::GetColumnWidth(int nColIndex, LPBYTE lpRightBorderWidth)
{
	return m_cGraphGridPublic.GetColumnWidth(nColIndex, lpRightBorderWidth);
}

void CGraphGrid::SetColumnData(int nColIndex, DWORD dwData)
{
	ASSERT(nColIndex >= 0 && nColIndex < GetColumnCount());
    
	m_lpColInfo[nColIndex].dwData = dwData;
}

DWORD CGraphGrid::GetColumnData(int nColIndex)
{
	ASSERT(nColIndex >= 0 && nColIndex < GetColumnCount());
	
	return m_lpColInfo[nColIndex].dwData;
}

void CGraphGrid::InitColumnTextBuf(int nColIndex, BYTE btMaxItemTextLen)
{
	ASSERT(nColIndex >= 0 && nColIndex < GetColumnCount());
	
	// 释放列中的缓冲区
	_ReleaseColumnBuf(nColIndex);
	
	if(btMaxItemTextLen < 4 || GetRowCount(RCT_DATA | RCT_FORECAST) <= 0)
	{
		return;
	}
	
	if(btMaxItemTextLen == MAXBYTE)		// 压缩型
	{
        ASSERT(FALSE);  // 暂时不提供
/*		m_lpColInfo[nColIndex].pTextArray = new CTypedPtrArray<CPtrArray, LPSTR>;
		m_lpColInfo[nColIndex].pTextArray->RemoveAll();*/
	}
	else								// 一次性缓冲区
	{
		m_lpColInfo[nColIndex].lpTextBuf = new char[(btMaxItemTextLen + 1) * GetRowCount(RCT_DATA | RCT_FORECAST)];
	}
	m_lpColInfo[nColIndex].btMaxItemTextLen = btMaxItemTextLen;
}

void CGraphGrid::AutoSizeColumn(int nColIndex)
{
	ASSERT(nColIndex >= 0 && nColIndex < GetColumnCount());
    
	int nColNameLen = strlen(m_lpColInfo[nColIndex].szName);
	int nGraphGridBodyMaxLen = m_cGraphGridBody.GetColumnCellsMaxLen(nColIndex);
    
	// 计算最大长度
    char szBodyMaxString[32] = {0};
    sprintf(szBodyMaxString, "%0*d", nGraphGridBodyMaxLen, 0);
	LPSTR lpMaxString = szBodyMaxString;
	if(nColNameLen > nGraphGridBodyMaxLen)
		lpMaxString = m_lpColInfo[nColIndex].szName;
    
    NSString *text = [NSString stringWithFormat:@"@s", lpMaxString];
    
	int nCalcWidth = [text sizeWithFont:m_lpGridSet->fontBody].width + 8; //(WORD)(nMaxLen * 7 + 4);
	
	if(nCalcWidth < 20) nCalcWidth = 20;	// 保证至少有20个像素宽
    
	if(m_lpColInfo[nColIndex].nWidth != nCalcWidth)
	{
		// 设置新的宽度
		m_lpColInfo[nColIndex].nWidth = nCalcWidth;
	}
    
	_NeedRecalcRect();
}

void CGraphGrid::AutoSizeAllColumns()
{
	for(int i=GetColumnCount()-1; i>=0; i--)
		AutoSizeColumn(i);
}

void CGraphGrid::AutoAdjustColumns(DWORD dwHideFlag, int nMinShowCount)
{
	ASSERT(nMinShowCount > 0);
    
	int nColCount = GetColumnCount(), nRowCount = GetColumnCount();
	if(nColCount <= nMinShowCount)
	{
		return;
	}
    
	// 查找所有没有位图值的列
	LPBYTE lpColumnFlag = new BYTE[nColCount];
	ZeroMemory(lpColumnFlag, nColCount);
	int i, j, nNeedHideCount = nColCount;
    
	for(i=0; i<nColCount; i++)
	{
		if(m_lpColInfo[i].szName[0] == 0)
		{
			// 如果本身是空列
			lpColumnFlag[i] = 1;
			nNeedHideCount --;
			continue;
		}
		for(j=0; j<nRowCount; j++)
		{
			if(m_cGraphGridBody.GetCellValueType(j, i) == CGCT_BITMAP_INDEX)
			{
				// 该列存在位图，则是有值的列
				lpColumnFlag[i] = 2;
				nNeedHideCount --;
				break;
			}
		}
	}
    
	int nCanHideCount = nNeedHideCount;
	if(nColCount - nNeedHideCount < nMinShowCount)
	{
		nCanHideCount = nColCount - nMinShowCount;
	}
    
	// 隐藏最左边的空列
	if(dwHideFlag & AACHF_LEFT)
	{
		for(i=0; i<nColCount; i++)
		{
			if(lpColumnFlag[i] == 1)
			{
				continue;	// 遇到空列，则略过
			}
			if(lpColumnFlag[i] == 2 || nCanHideCount <= 0)
			{
				break;		// 遇到有值的列或者达到最小显示列数，则完成
			}
			// 将没有值的列隐藏
			m_lpColInfo[i].btRightBorderWidth = 0;
			m_lpColInfo[i].nWidth = -1;
			nCanHideCount --;
		}
	}
    
	// 隐藏最右边的空列
	if(dwHideFlag & AACHF_RIGHT)
	{
		for(i=nColCount-1; i>=0; i--)
		{
			if(lpColumnFlag[i] == 1)
			{
				continue;	// 遇到空列，则略过
			}
			if(lpColumnFlag[i] == 2 || nCanHideCount <= 0)
			{
				break;		// 遇到有值的列或者达到最小显示列数，则完成
			}
			// 将没有值的列隐藏
			m_lpColInfo[i].btRightBorderWidth = 0;
			m_lpColInfo[i].nWidth = -1;
			nCanHideCount --;
		}
	}
    
	// 隐藏所有空列
	if(dwHideFlag & AACHF_ALL)
	{
		for(i=0; i<nColCount; i++)
		{
			if(lpColumnFlag[i] == 1)
			{
				continue;	// 遇到空列，则略过
			}
			if(nCanHideCount <= 0)
			{
				break;		// 达到最小显示列数，则完成
			}
			if(lpColumnFlag[i] == 0)
			{
				// 将没有值的列隐藏
				m_lpColInfo[i].btRightBorderWidth = 0;
				m_lpColInfo[i].nWidth = -1;
				nCanHideCount --;
			}
		}
	}
    
	DELETE_BUFFER(lpColumnFlag);
	_NeedRecalcRect();
	_RecalcRect();
}

void CGraphGrid::EnableForecast(BOOL bEnable)
{
	m_bEnableForecast = bEnable;
}

BOOL CGraphGrid::IsSurpportForecast(int nColIndex, BOOL bCheckShowForecastRow)
{
	if(!m_bEnableForecast)		
	{
		return FALSE;			// 该图表本身不支持预测行
	}
    
	if(bCheckShowForecastRow)	// 需要检查预测行处于显示状态
	{
		if(!(m_lpGridSet->dwShowMask & GGSSM_BODY_FORECASTROW))
		{
			return FALSE;	// 没有显示预测行
		}
	}
	
	// 必须有相关球图片，才能支持预测行
	return (m_lpColInfo[nColIndex].szBallText[0] != 0) ? TRUE : FALSE;
}

void CGraphGrid::EnableHideAllCells(BOOL bEnable)
{
	m_bIsHideAllCells = bEnable;
}

BOOL CGraphGrid::IsHideAllCells()
{
	return m_bIsHideAllCells;
}

void CGraphGrid::Draw(CGContextRef context)
{	
	_DrawGraphGridPart(context);
}

DWORD CGraphGrid::HitTest(CGPoint pt, int *pRowIndex, int *pColIndex)
{
	DWORD dwRtn = m_cGraphGridHead.HitTest(pt, pRowIndex, pColIndex);
	if(dwRtn != 0)
	{
		return dwRtn;
	}
	
	dwRtn = m_cGraphGridTail.HitTest(pt, pRowIndex, pColIndex);
	if(dwRtn != 0)
	{
		return dwRtn;
	}
	
	dwRtn = m_cGraphGridBody.HitTest(pt, pRowIndex, pColIndex);
	if(dwRtn != 0)
	{
		return dwRtn;
	}
	
	return GRIDHT_NONE;
}

int CGraphGrid::GetForecastRow()
{
	return m_cGraphGridBody.GetForecastRow();
}

BOOL CGraphGrid::IsForecastRow(int nRowIndex)
{
	return m_cGraphGridBody.IsForecastRow(nRowIndex);
}

int CGraphGrid::GetSelectRow()
{
	return m_cGraphGridBody.GetSelectRow();
}

void CGraphGrid::SetSelectRow(int nRowIndex)
{
	m_cGraphGridBody.SetSelectRow(nRowIndex);
}

BOOL CGraphGrid::IsSelectedRow(int nRowIndex)
{
	return m_cGraphGridBody.IsSelectedRow(nRowIndex);
}

void CGraphGrid::SetViewRect(CGRect &rectView)
{
	m_rectView = rectView;
    
	m_cGraphGridBody.SetViewRect(rectView);
	m_cGraphGridHead.SetViewRect(rectView);
	m_cGraphGridTail.SetViewRect(rectView);
}

void CGraphGrid::SetOrg(int x, int y)
{
	if(m_hGraphGrid == INVALID_HANDLE_VALUE)
	{
		ASSERT(FALSE);
		return ;
	}
    
	if(m_ptOrg.x == x && m_ptOrg.y == y)
	{
		return ;		// 没有改变
	}
    
	m_ptOrg = CGPointMake(x, y);
	m_cGraphGridHead.SetOrg(x, y);
    
    CGRect rect = m_cGraphGridHead.GetRect();
	m_cGraphGridBody.SetOrg(x, rect.origin.y + rect.size.height);
    rect = m_cGraphGridBody.GetRect();
	m_cGraphGridTail.SetOrg(x, rect.origin.y + rect.size.height);
	
	if(y < 0)
	{
		m_cGraphGridHead.SetOrg(x, 0);
	}
}

void CGraphGrid::SetBkColor(UIColor *bkColor, DWORD dwFlag)
{
	if(dwFlag & SBI_GRID)
	{
		m_cGraphGridBody.SetBkColor(bkColor);
	}
    
	if(dwFlag & SBI_GRIDHEAD)
	{
		m_cGraphGridHead.SetBkColor(bkColor);
	}
	
	if(dwFlag & SBI_GRIDTAIL)
	{
		m_cGraphGridTail.SetBkColor(bkColor);
	}
}

void CGraphGrid::SetCellColor(int nRowIndex, int nColIndex, UIColor *crText, UIColor *crBack, BOOL bIsStatRow)
{
	if(bIsStatRow)
		m_cGraphGridTail.GetStatGrid()->SetCellColor(nRowIndex, nColIndex, crText, crBack);
	else
		m_cGraphGridBody.SetCellColor(nRowIndex, nColIndex, crText, crBack);
}

void CGraphGrid::SetCellBitmap(int nRowIndex, int nColIndex, int nBitmapIndex, BYTE btValueCount)
{
	m_cGraphGridBody.SetCellBitmap(nRowIndex, nColIndex, nBitmapIndex, btValueCount);
}

int CGraphGrid::GetCellBitmap(int nRowIndex, int nColIndex, LPBYTE lpValueCount)
{
	return m_cGraphGridBody.GetCellBitmap(nRowIndex, nColIndex, lpValueCount);
}

void CGraphGrid::SetCellText(int nRowIndex, int nColIndex, LPCSTR lpszValue, BOOL bIsStatRow)
{
	if(bIsStatRow)
		m_cGraphGridTail.GetStatGrid()->SetCellText(nRowIndex, nColIndex, lpszValue);
	else
		m_cGraphGridBody.SetCellText(nRowIndex, nColIndex, lpszValue);
}

void CGraphGrid::SetCellInt(int nRowIndex, int nColIndex, int nValue, BYTE btType, BOOL bIsStatRow)
{
	if(bIsStatRow)
		m_cGraphGridTail.GetStatGrid()->SetCellInt(nRowIndex, nColIndex, nValue, btType);
	else
		m_cGraphGridBody.SetCellInt(nRowIndex, nColIndex, nValue, btType);
}

void CGraphGrid::SetCellFloat(int nRowIndex, int nColIndex, float fValue, BYTE btType, BOOL bIsStatRow)
{
	if(bIsStatRow)
		m_cGraphGridTail.GetStatGrid()->SetCellFloat(nRowIndex, nColIndex, fValue, btType);
	else
		m_cGraphGridBody.SetCellFloat(nRowIndex, nColIndex, fValue, btType);
}

BYTE CGraphGrid::GetCellValueType(int nRowIndex, int nColIndex, BOOL bIsStatRow)
{
	if(bIsStatRow)
		return m_cGraphGridTail.GetStatGrid()->GetCellValueType(nRowIndex, nColIndex);
	else
		return m_cGraphGridBody.GetCellValueType(nRowIndex, nColIndex);
}

UIColor * CGraphGrid::GetCellColor(int nRowIndex, int nColIndex, UIColor **lpBackColorOut, BOOL bIsStatRow)
{
	if(bIsStatRow)
		return m_cGraphGridTail.GetStatGrid()->GetCellColor(nRowIndex, nColIndex, lpBackColorOut);
	else
		return m_cGraphGridBody.GetCellColor(nRowIndex, nColIndex, lpBackColorOut);
}

int CGraphGrid::GetCellInt(int nRowIndex, int nColIndex, LPBYTE lpType, BOOL bIsStatRow)
{
	if(bIsStatRow)
		return m_cGraphGridTail.GetStatGrid()->GetCellInt(nRowIndex, nColIndex, lpType);
	else
		return m_cGraphGridBody.GetCellInt(nRowIndex, nColIndex, lpType);
}	

float CGraphGrid::GetCellFloat(int nRowIndex, int nColIndex, LPBYTE lpType, BOOL bIsStatRow)
{
	if(bIsStatRow)
		return m_cGraphGridTail.GetStatGrid()->GetCellFloat(nRowIndex, nColIndex, lpType);
	else
		return m_cGraphGridBody.GetCellFloat(nRowIndex, nColIndex, lpType);
}

LPCSTR CGraphGrid::GetCellText(int nRowIndex, int nColIndex, LPBYTE lpType, BOOL bIsStatRow)
{
	if(bIsStatRow)
		return m_cGraphGridTail.GetStatGrid()->GetCellText(nRowIndex, nColIndex, lpType);
	else
		return m_cGraphGridBody.GetCellText(nRowIndex, nColIndex, lpType);
}

CGRect CGraphGrid::GetCellRect(int nRowIndex, int nColIndex, BYTE btCellType)
{
	_RecalcRect();
    
	switch(btCellType)
	{
        case CPICT_HEAD_TITLE: return m_cGraphGridHead.GetCellRect();
        case CPICT_HEAD_COLUMN: return m_cGraphGridHead.GetCellRect(nColIndex);
        case CPICT_TAIL_TITLE: return m_cGraphGridTail.GetCellRect();
        case CPICT_TAIL_COLUMN: return m_cGraphGridTail.GetCellRect(nColIndex);
        case CPICT_ROW_DATA:
        case CPICT_ROW_FORECAST:
        case CPICT_ROW_SPACE: return m_cGraphGridBody.GetCellRect(nRowIndex, nColIndex);
        case CPICT_ROW_STAT: return m_cGraphGridTail.GetStatGrid()->GetCellRect(nRowIndex, nColIndex);
	}
    
	ASSERT(FALSE);
	return CGRectZero;
}

CGRect CGraphGrid::GetRect(DWORD dwFlag)
{
	_RecalcRect();
    
	CGRect rect = CGRectZero;
	switch(dwFlag) 
	{
        case GRF_ALL:
            rect = m_cGraphGridBody.GetRect();
            rect.origin = m_ptOrg;
            rect.size.height += m_cGraphGridHead.GetRect().size.height;
            rect.size.height += m_cGraphGridTail.GetRect().size.height;
            break;
        case GRF_HEAD:
            rect = m_cGraphGridHead.GetRect();
            break;
        case GRF_BODY:
            rect = m_cGraphGridBody.GetRect();
            break;
        case GRF_TAIL:
            rect = m_cGraphGridTail.GetRect();
            break;
        default:
            ASSERT(FALSE);
            break;
	}
	return rect;
};

void CGraphGrid::ClearForecastRow()
{
	if(!m_bEnableForecast || !(m_lpGridSet->dwShowMask & GGSSM_BODY_FORECASTROW))
	{
		return ;	// 不支持或者没有预测行
	}
    
	m_cGraphGridBody.ClearForecastRow();
}

/*BOOL CGraphGrid::GetPromptInfo(CPoint &point, LPSTR lpszPromptText, CRect *pValidRect, int *pRowIndex, int *pColIndex)
{
	if(m_bIsHideAllCells)
	{
		return FALSE;		// 免费版中不显示的图表
	}
    
	ASSERT(lpszPromptText != NULL);
    
	int nRowIndex = -1, nColIndex = -1;
    
	DWORD dwHitTest = HitTest(point, &nRowIndex, &nColIndex);
	BYTE btCellType = 0;
	lpszPromptText[0] = 0;
    
	switch(dwHitTest)
	{
        case GRIDHT_NONE: return FALSE; break;
        case GRIDHT_BORDER:					
        case GRIDHT_HEAD_BORDER:
        case GRIDHT_BODY_BORDER:
        case GRIDHT_STAT_BORDER:
        case GRIDHT_TAIL_BORDER: return TRUE; break;
        case GRIDHT_HEAD_TITLE: btCellType = CPICT_HEAD_TITLE; break;
        case GRIDHT_HEAD_COLUMN: btCellType = CPICT_HEAD_COLUMN; break;
        case GRIDHT_TAIL_TITLE: btCellType = CPICT_TAIL_TITLE; break;
        case GRIDHT_TAIL_COLUMN: btCellType = CPICT_TAIL_COLUMN; break;
        case GRIDHT_BODY_DATA: btCellType = CPICT_ROW_DATA; break;
        case GRIDHT_BODY_FORECAST: btCellType = CPICT_ROW_FORECAST; break;
        case GRIDHT_BODY_SPACE: btCellType = CPICT_ROW_SPACE; break;
        case GRIDHT_STAT_CELL:  btCellType = CPICT_ROW_STAT; break;
	}
    
	if(!_GetCellPromptInfo(lpszPromptText, nRowIndex, nColIndex, btCellType))
		return FALSE;
    
	// 返回相关信息
	if(pRowIndex != NULL) (*pRowIndex) = nRowIndex;
	if(pColIndex != NULL) (*pColIndex) = nColIndex;
	if(pValidRect != NULL)
	{
		*pValidRect = GetCellRect(nRowIndex, nColIndex, btCellType);
	}
    
	return TRUE;
}

BOOL CGraphGrid::GetExplainText(CString &strExplain)
{
	strExplain.Empty();
    
	switch(m_stGridID.btType)
	{
        case GGTYPE_CONDITION:	// 条件图表
		{
			// 获取值说明
			ICondition *pICondition = g_pIConditionMan->GetICondition(m_stGridID.cdtID);
			CString strValueRemarks;
			strExplain.Format("%s%s中：", GetTitle(), pICondition->IsMultipleValue() ? "分布图" : "走势图");
			if(pICondition->GetValueExplain(strValueRemarks, strExplain.GetLength()))
			{
				strExplain += strValueRemarks;
				return TRUE;
			}
		}
            break;
        case GGTYPE_TREND:
        case GGTYPE_DISTRIBUTE:
		{
			return _GetExplainText(strExplain);
		} break;
        case GGTYPE_DATATABLE: break;
	}
    
	strExplain.Empty();
	return FALSE;
}
*/

//////////////////////////////////////////////////////////////////////////////////
/*BOOL CGraphGrid::_GetCellPromptInfo(LPSTR lpszPromptText, int nRowIndex, int nColIndex, BYTE btCellType)
{
	ASSERT(lpszPromptText != NULL);
    
	switch(btCellType)
	{
        case CPICT_HEAD_TITLE:
        case CPICT_TAIL_TITLE: sprintf(lpszPromptText, m_szTitle); break;
        case CPICT_HEAD_COLUMN:
        case CPICT_TAIL_COLUMN: sprintf(lpszPromptText, "当前列：%s", m_lpColInfo[nColIndex].szInfoTip); break;
        case CPICT_ROW_DATA: strcpy(lpszPromptText, GetCellText(nRowIndex, nColIndex, NULL, FALSE)); break;
        case CPICT_ROW_FORECAST: sprintf(lpszPromptText, GetCellText(nRowIndex, nColIndex, NULL, FALSE)); break;
        case CPICT_ROW_SPACE:
		{
			if(m_lpColInfo[nColIndex].szBallText[0] == 0)
			{
                //				sprintf(lpszPromptText, "空白行");
			}
			else if(IsSurpportForecast(nColIndex))
			{
				sprintf(lpszPromptText, "点击此处可以显示或隐藏当前值");
			}
		}break;
        case CPICT_ROW_STAT: 
		{
			if(m_lpColInfo[nColIndex].szBallText[0] == 0)
			{
				return TRUE;	// 没有统计列
			}
			if(!m_lpColInfo[nColIndex].stStatResult.GetValueRemarks(lpszPromptText, m_lpGridSet->stStatItems[nRowIndex].dwID))
			{
				return FALSE;
			}
		} break;
        default: return FALSE;
	}
    
	return TRUE;
}

BOOL CGraphGrid::_GetExplainText(CString &strExplain)
{
	if(m_stGridID.btType == GGTYPE_TREND)
	{
		if(m_stGridID.dwGraphGridID == GGID_TG_CUSTOMCOMBO)		// 自定义组合
		{
			strExplain = "自定义组合走势图中：";
            
			CStringArray strValueExplainArray;
			int nCount = GetColumnCount();
			strValueExplainArray.SetSize(nCount);
			for(int i=0; i<nCount; i++)
			{
				strValueExplainArray.SetAt(i, m_lpColInfo[i].szInfoTip);
			}
			strExplain += g_pICstPubFunc->GetFormattedGraphGridDetails(strValueExplainArray, strExplain.GetLength());
            
			return TRUE;
		}
	}
	
	// 没有说明
	strExplain.Empty();
    
	return FALSE;
}
*/
//////////////////////////////////////////////////////////////////////////////////
void CGraphGrid::_Initialize()
{
	m_hGraphGrid = INVALID_HANDLE_VALUE;
	m_rectView = CGRectZero;
	m_ptOrg = CGPointMake(-1, -1);
	
	m_bIsHideAllCells = FALSE;
}

void CGraphGrid::_NeedRecalcRect()
{
	m_cGraphGridBody.m_bNeedRecalcRect = TRUE;
	m_cGraphGridHead.m_bNeedRecalcRect = TRUE;
	m_cGraphGridTail.m_bNeedRecalcRect = TRUE;
}

void CGraphGrid::_RecalcRect()
{
	if(!m_cGraphGridHead.m_bNeedRecalcRect)
		return ;
    
	m_cGraphGridHead.SetOrg(m_ptOrg.x, m_ptOrg.y);
	m_cGraphGridHead.RecalcRect();
    CGRect rect = m_cGraphGridHead.GetRect();
	m_cGraphGridBody.SetOrg(m_ptOrg.x, rect.origin.y + rect.size.height);
	m_cGraphGridBody.RecalcRect();
    rect = m_cGraphGridBody.GetRect();
	m_cGraphGridTail.SetOrg(m_ptOrg.x, rect.origin.y + rect.size.height);
	m_cGraphGridTail.RecalcRect();
}

void CGraphGrid::_ReleaseColumnBuf(int nColIndex)
{
	ASSERT(nColIndex >= -1 && nColIndex < GetColumnCount());
	
	if(nColIndex == -1)		// 释放所有列的缓冲区
	{
		for(int i=GetColumnCount()-1; i>=0; i--)
		{
			_ReleaseColumnBuf(i);
		}
	}
	else					// 释放指定列的缓冲区
	{
		if(m_lpColInfo[nColIndex].btMaxItemTextLen == MAXBYTE)
		{
            ASSERT(FALSE);  // 暂不启用
/*			if(m_lpColInfo[nColIndex].pTextArray != NULL)
			{
				for(int i=m_lpColInfo[nColIndex].pTextArray->GetSize()-1; i>=0; i--)
				{
					delete []m_lpColInfo[nColIndex].pTextArray->GetAt(i);
				}
				m_lpColInfo[nColIndex].pTextArray->RemoveAll();
				delete m_lpColInfo[nColIndex].pTextArray;
				m_lpColInfo[nColIndex].pTextArray = NULL;
			}*/
		}
		else if(m_lpColInfo[nColIndex].btMaxItemTextLen > 0)
		{
			DELETE_BUFFER(m_lpColInfo[nColIndex].lpTextBuf);
		}
		m_lpColInfo[nColIndex].btMaxItemTextLen = 0;
	}
}

void CGraphGrid::_DrawGraphGridPart(CGContextRef context, DWORD dwDrawFlag)
{	
	if(CGRectIsEmpty(m_rectView))
	{
		return;		// 不可见
	}
	
	if(GetColumnCount() <= 0)
	{
		return ;
	}
	
	CGRect rect = GetRect();
	if(!CGRectIntersectsRect(m_rectView, rect))
	{
		return ;	// 不在可见范围内
	}
	
	if(dwDrawFlag & DF_GRIDBODY)
	{
		// 画表身
		m_cGraphGridBody.Draw(context, TRUE, 0);
				
	/*	if(m_bIsHideAllCells)	// 免费版显示“不提供该功能”图片
		{
            CGRect rectBody = m_cGraphGridBody.GetRect();
			CBitmap *pBitmap = g_pICstMain->GetBitmapFromFile("Graph_NotSupport.bmp");
			CDC cMemDC;
			cMemDC.CreateCompatibleDC(pDC);
			CBitmap * pOldBitmapImage = cMemDC.SelectObject(pBitmap);
			
			BITMAP bitmap = {0};
			pBitmap->GetBitmap(&bitmap);
			
			// 居中下计算
			int x = (rectBody.right + rectBody.left - bitmap.bmWidth + 1) / 2;
			int y = rectBody.bottom - bitmap.bmHeight - 20;
			
			::TransparentBlt(pDC->m_hDC, x, y, bitmap.bmWidth, bitmap.bmHeight, cMemDC.m_hDC, 0, 0, bitmap.bmWidth, bitmap.bmHeight, RGB(255, 255, 255));
			cMemDC.SelectObject(pOldBitmapImage);
		}*/
	}
	
	if(dwDrawFlag & DF_CELL_BKGND)
	{
		// 画单元格背景
		if(!m_bIsHideAllCells)
			m_cGraphGridBody.Draw(context, FALSE, DF_CELL_BKGND);
	}
	
	if(dwDrawFlag & DF_CELL_VALUE)
	{
		// 画单元格值
		if(!m_bIsHideAllCells)
			m_cGraphGridBody.Draw(context, FALSE, DF_CELL_VALUE);
	}
	
	// 画表头
	if(dwDrawFlag & DF_GRIDHEAD)
	{
		if(m_lpGridSet->btLockMask & GGSLM_HEAD)
		{
			// 如果表头超出可视范围顶部，则将表头置于可视范围顶部
			CGRect rect = m_cGraphGridHead.GetRect();
			m_cGraphGridHead.SetOrg(rect.origin.x, m_rectView.origin.y + 1);
			rect = m_cGraphGridHead.GetRect();
            
			// 画分割线
            [m_lpGridSet->crBorderPen setStroke];
            int y = rect.origin.y + rect.size.height, nEndX = rect.origin.x + rect.size.width;
            m_cGraphGridPublic.DrawLine(context, m_lpGridSet->btBorderPenWidth, rect.origin.x, y, nEndX, y, TRUE);
		}
		m_cGraphGridHead.Draw(context);
	}
	
	// 画表尾
	if(dwDrawFlag & DF_GRIDTAIL)
	{
		if(m_lpGridSet->btLockMask & GGSLM_TAIL)
		{
			// 如果表尾超出可视范围底部，则将表尾置于可视范围底部
			CGRect rect = m_cGraphGridTail.GetRect(), rectBody = m_cGraphGridBody.GetRect();
			int nHeight = rect.size.height;
			int nBodyBottom = rectBody.origin.y + rectBody.size.height;
			if(m_rectView.origin.y + m_rectView.size.height < nBodyBottom + nHeight)
			{
				m_cGraphGridTail.SetOrg(rect.origin.x, m_rectView.origin.y + m_rectView.size.height - nHeight - 1);
			}
			else
			{
				m_cGraphGridTail.SetOrg(rect.origin.x, nBodyBottom);
			}
            
			rect = m_cGraphGridTail.GetRect();
            
			// 画分割线
            [m_lpGridSet->crBorderPen setStroke];
            int y = rect.origin.y - m_lpGridSet->btBorderPenWidth, nEndX = rect.origin.x + rect.size.width;
            m_cGraphGridPublic.DrawLine(context, m_lpGridSet->btBorderPenWidth, rect.origin.x, y, nEndX, y, TRUE);
            CGContextStrokePath(context);
		}
        
		m_cGraphGridTail.Draw(context);
	}
}

