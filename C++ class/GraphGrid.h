//
//  GraphGrid.h
//  Graph
//
//  Created by 武 孙 on 12-5-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "GraphGridPublic.h"
#include "GraphGridBody.h"
#include "GraphGridHead.h"
#include "Def_GraphGrid_ID.h"
#include "BallBitmap.h"

class CGraphGrid
{
public:
	CGraphGrid();
	virtual ~CGraphGrid();
    
public:		// 接口函数
    
	// 创建表格
	virtual BOOL Create(CGPoint ptOrg, int nDataRowCount, int nColCount);
	// 销毁
	virtual void Destroy();
    
    // 当表格设置改变后
    virtual void OnGridSettingsChanged();
    
	// 设置图表ID号
	virtual void SetGridID(const GRAPHGRIDID &stGridID){m_stGridID = stGridID;};
	// 获取图表ID号
	virtual GRAPHGRIDID &GetGridID(){return m_stGridID;};
	
	// 获取行高，bIsStatRow为TRUE则获取统计行高度，否则获取表体行高度
	virtual int GetRowHeight(BOOL bIsStatRow = FALSE);
    
	// 获取行总数，bIsStatRow为TRUE则获取统计行总数，否则获取表体行总数
	virtual int GetRowCount(BYTE btType = RCT_DATA);
	// 获取列总数
	virtual int GetColumnCount();
    
	// 设置标题信息
	virtual void SetTitle(LPCSTR lpszTitle);
	// 获取标题名
	virtual LPCSTR GetTitle();
    
	// 设置列名，球文字名，列提示信息
	virtual void SetColumnName(int nColIndex, LPCSTR lpszName, LPCSTR lpszBallText = NULL, LPCSTR lpszInfoTip = NULL);
	// 获取列名
	virtual LPCSTR GetColumnName(int nColIndex, LPSTR lpszBallText = NULL, LPSTR lpszInfoTip = NULL);
	// 设置列宽，列右边框宽度
	virtual void SetColumnWidth(int nColIndex, int nWidth, BYTE btRightBorderWidth = 0);
	// 获取列宽
	virtual int GetColumnWidth(int nColIndex, LPBYTE lpRightBorderWidth = NULL);
	// 设置列数据
	virtual void SetColumnData(int nColIndex, DWORD dwData);
	// 获取列数据
	virtual DWORD GetColumnData(int nColIndex);
	// 初始化列文本缓冲区，用于CGCT_STRING_BUF和CGCT_STRING_COMPRESS，
	// btMaxItemTextLen为MAXBYTE时表示使用CGCT_STRING_COMPRESS，不为0时使用CGCT_STRING_BUF，则为列中每个单元格文本的最大长度
	virtual void InitColumnTextBuf(int nColIndex, BYTE btMaxItemTextLen = MAXBYTE);
    
	// 自动设定指定列宽度
	virtual void AutoSizeColumn(int nColIndex);
	// 自动设定所有列宽度
	virtual void AutoSizeAllColumns();
    
	// 自动调整所有列，目前仅用于走势图中隐藏没有数据的列
	// dwHideFlag：隐藏没有值的列标志，见宏定义，nMinShowCount：最少需要显示的列数
	virtual void AutoAdjustColumns(DWORD dwHideFlag = AACHF_LEFT | AACHF_RIGHT, int nMinShowCount = 4);
    
	// 设置是否允许预测信息，系统缺省允许预测信息
	virtual void EnableForecast(BOOL bEnable = TRUE);
	// 查询制定列是否支持预测信息，bCheckShowForecastRow：为TRUE则需要检查预测行处于显示状态
	virtual BOOL IsSurpportForecast(int nColIndex, BOOL bCheckShowForecastRow = TRUE);
    
	// 允许或禁止隐藏所有表格中的信息，主要用于免费版
	virtual void EnableHideAllCells(BOOL bEnable = FALSE);
	// 是否隐藏表格中的信息
	virtual BOOL IsHideAllCells();
    
	// 画表格
	virtual void Draw(CGContextRef context);
    
	// 鼠标位置检测
	virtual DWORD HitTest(CGPoint pt, int *pRowIndex, int *pColIndex);
	
	// 获取预测行，返回-1，表示没有预测行
	virtual int GetForecastRow();
	// 是否是预测行
	virtual BOOL IsForecastRow(int nRowIndex);
	

	// 获取选择行，返回-1，表示没有选择的行
	virtual int GetSelectRow();
	// 设置选择行
	virtual void SetSelectRow(int nRowIndex);
	// 是否是选择行
	virtual BOOL IsSelectedRow(int nRowIndex);
    
	// 获取矩形范围
	virtual CGRect GetRect(DWORD dwFlag = GRF_ALL);
    
	// 获取类名
	virtual LPCSTR GetClassName(){return "CGraphGrid";};
    
	// 设置表格背景颜色
	virtual void SetBkColor(UIColor *bkColor, DWORD dwFlag = SBI_GRID);
	// 设置左上角位置
	virtual void SetOrg(int x, int y);
	// 设置可见矩形区域
	virtual void SetViewRect(CGRect &rectView);
    
	// 获取单元格信息
	virtual BYTE GetCellValueType(int nRowIndex, int nColIndex, BOOL bIsStatRow = FALSE);
	// 获取单元颜色，返回文本颜色
	virtual UIColor * GetCellColor(int nRowIndex, int nColIndex, UIColor **lpBackColorOut = NULL, BOOL bIsStatRow = FALSE);
	// 获取单元格的整型值，lpType返回值类型，如果不是整型值，则返回-1
	virtual int GetCellInt(int nRowIndex, int nColIndex, LPBYTE lpType = NULL, BOOL bIsStatRow = FALSE);
	// 获取单元格的浮点值，lpType返回值类型，如果不是浮点值，则返回0.0
	virtual float GetCellFloat(int nRowIndex, int nColIndex, LPBYTE lpType = NULL, BOOL bIsStatRow = FALSE);
	// 获取单元格中文本，所有类型的数据都会转换为对应的显示的文本
	virtual LPCSTR GetCellText(int nRowIndex, int nColIndex, LPBYTE lpType = NULL, BOOL bIsStatRow = FALSE);
	// 获取单元格矩形范围
	virtual CGRect GetCellRect(int nRowIndex, int nColIndex, BYTE btCellType = CPICT_ROW_DATA);
    
	// 设置单元格颜色
	virtual void SetCellColor(int nRowIndex, int nColIndex, UIColor *crText, UIColor *crBack = nil, BOOL bIsStatRow = FALSE);
	// 设置单元格文本，lpszValue：文本字符串，如果为NULL，则表示空串
	virtual void SetCellText(int nRowIndex, int nColIndex, LPCSTR lpszValue, BOOL bIsStatRow = FALSE);
	// 设置单元格整型值，nValue：整型值，btType：数据类型，见宏定义
	virtual void SetCellInt(int nRowIndex, int nColIndex, int nValue, BYTE btType = CGCT_INT, BOOL bIsStatRow = FALSE);
	// 设置单元格浮点型值或者百分比值，fValue：浮点值，btType：数据类型，见宏定义
	virtual void SetCellFloat(int nRowIndex, int nColIndex, float fValue, BYTE btType = CGCT_FLOAT_2, BOOL bIsStatRow = FALSE);
	// 设置单元格位图索引
	virtual void SetCellBitmap(int nRowIndex, int nColIndex, int nBitmapIndex, BYTE btValueCount = 1);
	// 获取单元格位图索引，如果为-1则表示非位图单元，lpValueCount保存返回的值个数
	virtual int GetCellBitmap(int nRowIndex, int nColIndex, LPBYTE lpValueCount = NULL);
    
	// 设置预测行信息
	virtual void SetForecastColIndex(int /*nColIndex*/){ASSERT(FALSE);}; // 只能在走势图和分布图中使用
	
	// 清空预测行信息
	virtual void ClearForecastRow();
    
	// 获取提示信息，返回FALSE则表示当前点不在此图表上
/*	virtual BOOL GetPromptInfo(CPoint &point, LPSTR lpszPromptText, CRect *pValidRect, int *pRowIndex, int *pColIndex);
    
	// 获取图表说明信息，返回FALSE则表示没有相关信息
	virtual BOOL GetExplainText(CString &strExplain);*/
    
protected:	// 派生类可重置的函数
//	virtual BOOL _GetCellPromptInfo(LPSTR lpszPromptText, int nRowIndex, int nColIndex, BYTE btCellType = CPICT_ROW_DATA);
//	virtual BOOL _GetExplainText(CString &strExplain);
    
protected:
	void _NeedRecalcRect();
	void _DrawGraphGridPart(CGContextRef context, DWORD dwDrawFlag = DF_ALL);
    
private:
	void _Initialize();
	void _RecalcRect();
	void _ReleaseColumnBuf(int nColIndex);
    
protected:
	HANDLE				m_hGraphGrid;			// 句柄
	
	CGPoint				m_ptOrg;				// 坐标原点
	LPGRAPHGRIDSET		m_lpGridSet;			// 表格设置指针
    
	GRAPHGRIDID			m_stGridID;				// 图表ID号
	
	LPGRAPHGRIDCOLINFO	m_lpColInfo;			// 列信息
	
	CGRect				m_rectView;				// 可视矩形范围
	
	CGraphGridHead		m_cGraphGridHead;		// 表头
	CGraphGridHead		m_cGraphGridTail;		// 表尾
	CGraphGridBody		m_cGraphGridBody;		// 表身
    CGraphGridPublic    m_cGraphGridPublic;     // 表公共
	
	BOOL				m_bEnableForecast;		// 允许预测信息
	BOOL				m_bIsHideAllCells;		// 是否隐藏所有单元格信息
};