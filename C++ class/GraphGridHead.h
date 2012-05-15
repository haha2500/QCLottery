//
//  GraphGridHead.h
//  Graph
//
//  Created by 武 孙 on 12-5-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "HistoryValueStat.h"
#include "GraphGridBody.h"

// 定义显示信息标志的取值
#define		GHSF_TITLE			0X01			// 显示标题信息
#define		GHSF_COLUMN			0X02			// 显示列信息
#define		GHSF_STATGRID		0X10			// 显示统计表

class CGraphGridHead
{
public:
	CGraphGridHead();
	virtual ~CGraphGridHead();
    
public:
	BOOL Create(CGraphGridPublic *pGraphGridPublic, BOOL bIsHead = TRUE, BOOL bIsStatHead = FALSE);
	void Destroy();
    
	CGraphGridBody * GetStatGrid(){return &m_cGridStat;};
    
	void SetViewRect(CGRect &rectView);
	void Draw(CGContextRef context);
    
	void SetOrg(int x, int y);
	void RecalcRect();
	CGRect GetRect();
    
	// 获取单元格矩形，如果nColIndex==-1则是标题矩形，否则是对应列名矩形
	CGRect GetCellRect(int nColIndex = -1);
    
	void SetBkColor(UIColor *bkColor);
    
	void SetColumnStatResult(int nColIndex, CHistoryValueStat *pHistoryValueStat);
    
	void OnGridSettingsChanged();
    
	DWORD HitTest(CGPoint pt, int *pRowIndex, int *pColIndex);
    
private:
	void _DrawAllColsName(CGContextRef context);
	void _DrawTitle(CGContextRef context);
    
public:
	BOOL	m_bNeedRecalcRect;						// 需要重新计算矩形范围
    
private:
	CGraphGridPublic  *m_pGraphGridPublic;          // 表格公共对象指针
	CGPoint	m_ptOrg;								// 坐标原点
	BOOL	m_bIsHead;								// 是否用作表头，否则作为表尾
	BOOL	m_bIsStatHead;							// 是否用作统计表头，目前为号码表下的统计表头
	
	CGRect	m_rectView;								// 可视矩形范围
	CGRect	m_rectSelf;								// 本身矩形范围
    
	UIColor *m_crBkColor;							// 背景颜色
    
	BYTE	m_btShowFlag;							// 显示信息标志，见宏定义
	
	CGraphGridBody	m_cGridStat;					// 统计表格
};
