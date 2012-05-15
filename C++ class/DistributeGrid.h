//
//  DistributeGrid.h
//  Graph
//
//  Created by 武 孙 on 12-5-7.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "GraphGrid.h"

class CDistributeGrid : public CGraphGrid
{
public:

	CDistributeGrid();
	virtual ~CDistributeGrid();
    
public:		// 分布图接口
    
	// 创建分布图
	virtual BOOL CreateDistributeGrid(int nColCount, int nDataRowCount = -1);
	
	// 设置在图表组中的索引
	virtual void SetGroupIndex(int nIndex);
	
	// 当分布图配置改变后
	virtual void OnDistributeGridSettingsChanged();
	
	// 设置分布图中单元格的值
	virtual void SetDistributeGridCell(int nRowIndex, int nColIndex, BYTE btValueCount = 1);
	
	// 设置值完毕
	virtual void FinishSetValue();
    
public:		// 重载基类接口
	virtual BOOL Create(CGPoint ptOrg, int nRowCount, int nColCount);
	virtual void Destroy();
    
	virtual void SetForecastColIndex(int nColIndex);
	virtual LPCSTR GetClassName(){return "CDistributeGrid";};
	virtual void Draw(CGContextRef context);
    
	virtual void AutoSizeColumn(int nColIndex);
    
protected:	// 重置基类函数
//	virtual _GetCellPromptInfo(LPSTR lpszPromptText, int nRowIndex, int nColIndex, BYTE btCellType);
    
private:
	BOOL _PrepareDraw(CGContextRef context);
    
private:
	int						m_nGroupIndex;					// 在图表组中的索引号
	BOOL					m_bSettingsChanged;				// 设置改变
};
