//
//  GraphGridBody.h
//  Graph
//
//  Created by 武 孙 on 12-5-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "GraphGridPublic.h"
#include "MFCClass.h"

#define		DBF_TOP							0X00000001		// 需要画上边框
#define		DBF_BOTTOM						0X00000002		// 需要画下边框
#define		DBF_LEFT						0X00000004		// 需要画左边框
#define		DBF_RIGHT						0X00000008		// 需要画右边框

// 定义_GetRowInfo的返回值
#define		RI_SELECT						(BYTE)1			// 选择行
#define		RI_FORECAST						(BYTE)2			// 预测行
#define		RI_SPACE						(BYTE)3			// 空白行
#define		RI_OTHER						(BYTE)4			// 其他数据行

class CGraphGridBody
{
public:
	CGraphGridBody();
	virtual ~CGraphGridBody();
	
public:
	virtual BOOL Create(CGraphGridPublic *pGraphGridPublic, int nRowCount, int nColCount, BOOL bIsStatGrid = FALSE);
	virtual void Destroy();
    
	virtual int GetRowCount(BYTE btType = RCT_DATA);
	virtual int GetColCount(){return m_nColCount;};
    
	virtual void Draw(CGContextRef context, BOOL bDrawBody = TRUE, DWORD dwDrawCellFlag = 0X0000F000);
    
public:
	void SetBkColor(UIColor *color);
	void SetOrg(int x, int y);
	CGRect GetRect();
	void SetViewRect(CGRect &rectView);

	void RecalcRect();
    
public:
	int GetForecastRow();
	BOOL IsForecastRow(int nRowIndex);
	void ClearForecastRow(int nForecastCol = -1);
    
	int GetSelectRow();
	void SetSelectRow(int nRowIndex);
    BOOL IsSelectedRow(int nRowIndex);
    
	void OnGridSettingsChanged();

	int GetColumnCellsMaxLen(int nColIndex);

	DWORD HitTest(CGPoint pt, int *pRowIndex, int *pColIndex);
	void GetViewRow(int *pViewBeginRow, int *pViewEndRow);
	CGRect GetCellRect(int nRowIndex, int nColIndex);
    
	void SetCellColor(int nRowIndex, int nColIndex, UIColor *textColor, UIColor *backColor = nil);
	void SetCellText(int nRowIndex, int nColIndex, LPCSTR lpszValue);
	void SetCellInt(int nRowIndex, int nColIndex, int nValue, BYTE btType = CGCT_INT);
	void SetCellFloat(int nRowIndex, int nColIndex, float fValue, BYTE btType = CGCT_FLOAT_2);
	void SetCellBitmap(int nRowIndex, int nColIndex, int nBitmapIndex, BYTE btValueCount = 1);

    UIColor * GetCellColor(int nRowIndex, int nColIndex, UIColor **backColorOut = nil);
	int GetCellInt(int nRowIndex, int nColIndex, LPBYTE lpType = NULL);
	float GetCellFloat(int nRowIndex, int nColIndex, LPBYTE lpType = NULL);
	LPCSTR GetCellText(int nRowIndex, int nColIndex, LPBYTE lpType = NULL);
	BYTE GetCellValueType(int nRowIndex, int nColIndex);
	int GetCellBitmap(int nRowIndex, int nColIndex, LPBYTE lpValueCount = NULL);
    
private:
	void _DrawBorder(CGContextRef context, int nBeginX, int nEndX, int nBeginY, int nEndY, DWORD dwFlag);
	void _DrawCell(CGContextRef context, int nRowIndex, int nColIndex, DWORD dwFlag, UIColor *crText, UIColor *crBack, BYTE btRowInfo);
    
protected:
	typedef struct CELLINFO		// 单元格信息
	{
		BYTE	btValueType;				// 单元格内容类型（取值见宏定义）
		BYTE	btBkColorIndex;				// 背景颜色索引，为0，则表示无颜色
		BYTE	btTextColorIndex;			// 文字颜色索引，为0，则表示无颜色
		union								// 内容
		{
			struct
			{
				short	nBallBitmapIndex;	// 号码位图索引
				BYTE	btValueCount;		// 值个数
			};
			int		nValue;					// 整型数值
			float	fValue;					// 浮点数值
			char	szValue[4];				// 最大长度为3的字符串
			LPSTR	lpszValue;				// 长度超过3的字符串指针
		};	
	} *LPCELLINFO;
    	
private:
	BYTE _GetRowInfo(LPGRAPHGRIDSET lpGridSet, int nRowIndex, UIColor **crTextOut, UIColor **crBackOut);
    
	LPCELLINFO _GetCellInfo(int nRowIndex, int nColIndex)
	{
		ASSERT(nRowIndex >= 0 && nRowIndex < m_nRowCount);
		ASSERT(nColIndex >= 0 && nColIndex < m_nColCount);
		
		return &m_lpCellInfoArray[nRowIndex * m_nColCount + nColIndex];
	};
    
public:
    BOOL		m_bNeedRecalcRect;				// 需要重新计算矩形范围
    
protected:
	CGraphGridPublic  *m_pGraphGridPublic;		// 表格公共对象指针
	BOOL		m_bIsStatGrid;					// 是否是统计表格
	CGPoint		m_ptOrg;						// 坐标原点
    
	int			m_nRowCount;					// 行总数
	int			m_nDataRowCount;				// 数据行总数（即除去预测行和空白行后的行数）
	int			m_nSpaceRowCount;				// 空白行总数
	int			m_nColCount;					// 列总数
	int			*m_pColPosArray;				// 列所在x坐标集合
	
	CGRect		m_rectView;						// 可视矩形范围
	CGRect		m_rectSelf;						// 本身矩形范围
    
	UIColor     *m_crBkColor;					// 背景颜色
    
	int			m_nViewBeginRow;				// 可视的起始行
	int			m_nViewEndRow;					// 可视的最后行
	int			m_nViewBeginCol;				// 可视的起始列
	int			m_nViewEndCol;					// 可视的最后列
    
	int         m_nSelectedRow;                 // 选择行，为-1则没有
	int			m_nForecastRow;                 // 预测行，为-1则没有
    
    NSMutableArray  *m_crColorArray;			// 颜色集合
	LPCELLINFO	m_lpCellInfoArray;				// 单元格集合
	DWORD		m_dwCellCount;					// 单元格个数

    
	char        m_szTempItemText[16];           // 临时使用的文本，用于格式化遗漏值等
};
