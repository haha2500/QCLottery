//
//  TrendGrid.h
//  Graph
//
//  Created by 武 孙 on 12-5-7.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "GraphGrid.h"

class CTrendGrid : public CGraphGrid
{
public:
    
	CTrendGrid();
	virtual ~CTrendGrid();
    
public:		// 走势图接口
    
	// 创建走势图
	virtual BOOL CreateTrendGrid(int nColCount, int nDataRowCount = -1);
		
    // 设置在图表组中的索引
	virtual void SetGroupIndex(int nIndex);

    // 当走势图设置改变后
    virtual void OnTrendGridSettingsChanged();
    
	// 设置走势图连接点所在单元格，如果nColIndex为-1，则表示没有连接点
	virtual void SetTrendGridCell(int nRowIndex, int nColIndex = -1);
	// 获取走势图连接点所在单元格，返回-1则表示该行没有连接点
	virtual int GetTrendGridCell(int nRowIndex);
	
	// 设置值完毕
	virtual void FinishSetValue();
    
public:		// 重载基类接口
    
	virtual BOOL Create(CGPoint ptOrg, int nDataRowCount, int nColCount);	
	virtual void Destroy();
    
	virtual void SetForecastColIndex(int nColIndex);
	virtual LPCSTR GetClassName(){return "CTrendGrid";};
	virtual void Draw(CGContextRef context);
	
	virtual void AutoSizeColumn(int nColIndex);
    
	virtual void ClearForecastRow();
    
protected:	// 重置基类函数
//	virtual _GetCellPromptInfo(LPSTR lpszPromptText, int nRowIndex, int nColIndex, BYTE btCellType);
    
private:
	BOOL _PrepareDraw(CGContextRef context);
    
private:
	int					m_nGroupIndex;					// 在图表中的索引
	LPWORD				m_lpConnectValueArray;			// 连接点集合
    
	BOOL				m_bSettingsChanged;				// 设置改变
};
