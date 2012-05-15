//
//  GraphGridPublic.h
//  Graph
//
//  Created by 武 孙 on 12-5-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "HistoryValueStat.h"
#include "BallBitmap.h"

// 定义缺省行列宽度
#define		GRID_DEFAULT_ROW_HEIGHT			20
#define		GRID_DEFAULT_COL_WIDTH			20

// 定义DrawGraphGridPart(...)中dwDrawFlag的取值
#define		DF_ALL							0XFFFFFFFF		// 画所有
#define		DF_GRIDHEAD						0X00000001		// 画表头
#define		DF_GRIDTAIL						0X00000010		// 画表尾
#define		DF_GRIDBODY						0X00000100		// 画表身
#define		DF_CELL_BKGND					0X00001000		// 画单元格背景
#define		DF_CELL_VALUE					0X00002000		// 画单元格中的值

// 定义HitTest函数的返回值
#define		GRIDHT_NONE						0X00000000		// 不在表的区域内
#define		GRIDHT_BORDER					0X00000001		// 在表的边界上
#define		GRIDHT_HEAD_BORDER				0X00000011		// 在表头的边界上
#define		GRIDHT_HEAD_COLUMN				0X00000012		// 在表头的某列中
#define		GRIDHT_HEAD_TITLE				0X00000014		// 在表头的标题列中
#define		GRIDHT_BODY_BORDER				0X00000101		// 在表格的某个单元格的边界
#define		GRIDHT_BODY_DATA				0X00000102		// 在表格的某个数据单元格中
#define		GRIDHT_BODY_FORECAST			0X00000110		// 在表格的某个预测单元格中
#define		GRIDHT_BODY_SPACE				0X00000120		// 在表格的某个空白单元格中
#define		GRIDHT_STAT_BORDER				0X00001001		// 在表统计信息的某个单元格的边界
#define		GRIDHT_STAT_CELL				0X00001002		// 在表统计信息的某个单元格中
#define		GRIDHT_TAIL_BORDER				0X10000001		// 在表尾的边界上
#define		GRIDHT_TAIL_COLUMN				0X10000002		// 在表尾的某列中
#define		GRIDHT_TAIL_TITLE				0X10000004		// 在表尾的标题列中

// 定义单元格信息中btType的取值
#define		CGCT_NULL               (BYTE)0X00			// 空值
#define		CGCT_BITMAP_INDEX       (BYTE)0X01			// 号码位图索引

#define		CGCT_STRING_1           (BYTE)0X11			// 字符串，长度为1（仅限系统使用）
#define		CGCT_STRING_2			(BYTE)0X12			// 字符串，长度为2（仅限系统使用）
#define		CGCT_STRING_3			(BYTE)0X13			// 字符串，长度为3（仅限系统使用）
#define		CGCT_STRING_BUF			(BYTE)0X1A			// 字符串，一次性开辟的缓冲区
// #define		CGCT_STRING_COMPRESS	(BYTE)0X1B			// 字符串，压缩型（暂不提供）
#define		CGCT_STRING				(BYTE)0X1F			// 字符串

#define		CGCT_INT                (BYTE)0X20			// 整型值，变为字符串时不固定长度
#define		CGCT_INT_1				(BYTE)0X21			// 整型值，变为字符串时固定长度为1个字符，前补0
#define		CGCT_INT_2				(BYTE)0X22			// 整型值，变为字符串时固定长度为2个字符，前补0
#define		CGCT_INT_3				(BYTE)0X23			// 整型值，变为字符串时固定长度为3个字符，前补0
#define		CGCT_INT_4				(BYTE)0X24			// 整型值，变为字符串时固定长度为4个字符，前补0
#define		CGCT_INT_5				(BYTE)0X25			// 整型值，变为字符串时固定长度为5个字符，前补0
#define		CGCT_INT_6				(BYTE)0X26			// 整型值，变为字符串时固定长度为6个字符，前补0
#define		CGCT_INT_7				(BYTE)0X27			// 整型值，变为字符串时固定长度为7个字符，前补0
#define		CGCT_INT_8				(BYTE)0X28			// 整型值，变为字符串时固定长度为8个字符，前补0
#define		CGCT_INT_9				(BYTE)0X29			// 整型值，变为字符串时固定长度为9个字符，前补0
#define		CGCT_INT_10				(BYTE)0X2A			// 整型值，变为字符串时固定长度为10个字符，前补0
#define		CGCT_INT_STAT			(BYTE)0X2B			// 统计值
#define		CGCT_INT_SKIP           (BYTE)0X2C			// 遗漏值

#define		CGCT_FLOAT_1			(BYTE)0X41			// 浮点值，保留到小数点后1位
#define		CGCT_FLOAT_2			(BYTE)0X42			// 浮点值，保留到小数点后2位
#define		CGCT_FLOAT_3			(BYTE)0X43			// 浮点值，保留到小数点后3位
#define		CGCT_FLOAT_4			(BYTE)0X44			// 浮点值，保留到小数点后4位
#define		CGCT_FLOAT_STAT			(BYTE)0X48			// 浮点值(用于统计栏)，保留到小数点后1位

#define		CGCT_PERCENT			(BYTE)0X4A			// 百分值，不保留小数
#define		CGCT_PERCENT_1			(BYTE)0X4B			// 百分值，保留到小数点后1位
#define		CGCT_PERCENT_2			(BYTE)0X4C			// 百分值，保留到小数点后2位
#define		CGCT_PERCENT_3			(BYTE)0X4D			// 百分值，保留到小数点后3位
#define		CGCT_PERCENT_4			(BYTE)0X4E			// 百分值，保留到小数点后4位

// 定义GetRowCount中btType的取值
#define		RCT_DATA				(BYTE)0X01		// 数据行
#define		RCT_FORECAST			(BYTE)0X02		// 预测行
#define		RCT_SPACE				(BYTE)0X04		// 空白行
#define		RCT_STAT				(BYTE)0X10		// 统计行

// 定义GetRect(DWORD dwFlag)中dwFlag的值
#define		GRF_ALL							0				// 整个表格
#define		GRF_HEAD						1				// 整个表头
#define		GRF_BODY						2				// 整个表体
#define		GRF_TAIL						3				// 整个表尾

// 定义AutoAdjustColumns(...) 中 dwHideFlag的取值
#define		AACHF_NONE						0X00000000		// 不隐藏列
#define		AACHF_LEFT						0X00000001		// 隐藏左边没有值的列
#define		AACHF_RIGHT						0X00000002		// 隐藏右边没有值的列
#define		AACHF_ALL						0X00000010		// 隐藏所有没有值的列

// 定义SetBkColor(...)中dwFlag的取值
#define		SBI_GRIDALL						0xFFFFFFFF		// 设置表所有背景
#define		SBI_GRIDHEAD					0X00000001		// 设置表头背景
#define		SBI_GRID						0X00000010		// 设置表格背景
#define		SBI_GRIDTAIL					0X00000100		// 设置表尾背景

// 定义GetCellRect中的btCellType的取值
#define		CPICT_HEAD_TITLE				(BYTE)1			// 表头标题单元格
#define		CPICT_HEAD_COLUMN				(BYTE)2			// 表头列名单元格
#define		CPICT_TAIL_TITLE				(BYTE)3			// 表尾标题单元格
#define		CPICT_TAIL_COLUMN				(BYTE)4			// 表尾列名单元格
#define		CPICT_ROW_DATA					(BYTE)10		// 数据行单元格
#define		CPICT_ROW_FORECAST				(BYTE)11		// 预测行单元格
#define		CPICT_ROW_SPACE					(BYTE)12		// 空白行单元格
#define		CPICT_ROW_STAT					(BYTE)20		// 统计行单元格

// 定义ARRANGEITEM中btState的取值
#define		AIS_HIDE			0X00		// 隐藏状态
#define		AIS_SHOW			0X01		// 显示状态
#define		AIS_MUST			0x02		// 必选状态
#define		AIS_FIX_LEFT		0XF0		// 左侧固定列数，此时dwID为列数
#define		AIS_FIX_RIGHT		0XF1		// 右侧固定列数，此时dwID为列数
#define		AIS_FIX_TOP			0XF2		// 上侧固定行数，此时dwID为行数
#define		AIS_FIX_BOTTOM		0XF3		// 下侧固定行数，此时dwID为行数

// 定义周期表标志
#define		PTF_WEEK		0X02000001
#define		PTF_TENDAY		0X02000002
#define		PTF_MONTH		0X02000003
#define		PTF_CUSTOM		0X02000010

// 定义GRAPHGRIDCOLINFO中btFlag的取值
#define		CIF_NONE						(BYTE)0X00		// 无标志
#define		CIF_LOCK_TEXTCOLOR				(BYTE)0X01		// 锁定文本颜色，即选择时不改变本列文本颜色，比如号码列
#define		CIF_LOCK_BACKCOLOR				(BYTE)0X02		// 锁定背景颜色，即选择时不改变本列背景颜色，比如号码列

// 定义GRAPHGRIDSET中btShowMask的取值
#define		GGSSM_HEAD_TITLE				0X00000001						// 显示表头标题
#define		GGSSM_HEAD_COLUMN				0X00000002						// 显示表头列名
#define		GGSSM_TAIL_TITLE				0X00000004						// 显示表尾标题
#define		GGSSM_TAIL_COLUMN				0X00000008						// 显示表尾列名
#define		GGSSM_BODY_SKIP					0X00000010						// 显示遗漏值
#define		GGSSM_BODY_CURSELROW			0X00000020						// 显示当前选择行
#define		GGSSM_BODY_FORECASTROW			0X00000040						// 显示预测行
#define		GGSSM_BODY_ALTROWCOLOR			0X01000000						// 启用交替行颜色
#define		GGSSM_BODY_SPACE				0X02000000						// 显示空行
#define		GGSSM_STAT						0X00000100						// 显示统计信息
#define		GGSSM_STAT_ALTROWCOLOR			0X00000200						// 启用统计信息交替行颜色

#define		GGSSM_DEFAULT					0X00FFFFFF						// 缺省显示值

// 定义GRAPHGRIDSET中btLockMask的取值
#define		GGSLM_HEAD						BYTE(0X01)						// 锁定表头
#define		GGSLM_TAIL						BYTE(0X02)						// 显示表尾

// 定义NUMSTABLESET中dwFuncMask的取值
#define		NTSFM_SHOW						0X00000001						// 显示号码表
#define		NTSFM_ALTCOLCOLOR				0X00000002						// 启用交替列颜色
#define		NTSFM_ENABLENUMSCOLOR			0X00000004						// 启用自定义开奖号码颜色设置
#define		NTSFM_CONDITION					0X00000008						// 根据条件设置开奖号码颜色设置
#define		NTSFM_LOCK						0X01000000						// 固定号码表
#define		NTSFM_IGNOREBASE				0X02000000						// 忽略总体设置中交替行颜色
#define		NTSFM_DEFAULT					0X00FFFFFF						// 缺省值

// 定义TRENDGRIDSET中dwFuncMask的取值
#define		TGSFM_CONNECTLINE_SHOW			0X00000001						// 显示连接线
#define		TGSFM_CONNECTLINE_ALTCOLOR		0X00010000						// 启用交替颜色
#define		TGSFM_DEFAULT					0X0000FFFF						// 缺省值

// 定义DISTRIBUTEGRIDSET中dwFuncMask的取值
#define		DGSFM_DEFAULT					0X0000FFFF						// 缺省值

#pragma pack(1)
typedef struct tagGRAPHGRIDCOLINFO			// 图表列信息
{
	char		szName[33];					// 列名
	char		szBallText[4];				// 球文字
	char		szInfoTip[65];				// 列提示信息数组
	int			nWidth;						// 列宽
	BYTE		btRightBorderWidth;			// 列右边框宽度（增加的）
	DWORD		dwData;						// 列自定义数据
	BYTE		btMaxItemTextLen;			// 列中单元格最大文本长度，如果为MAXBYTE，则表示压缩文本，不为0则为一次性文本缓冲区，为0则表示正常文本
	BYTE		btFlag;						// 标志，见宏定义
	LPSTR		lpTextBuf;                  // 一次性文本缓冲区，用于CGCT_STRING_BUF
    
	STATRESULT	stStatResult;				// 统计信息
    
} GRAPHGRIDCOLINFO, *LPGRAPHGRIDCOLINFO;

typedef struct tagARRANGEITEM                   // 排列单元
{
	DWORD dwID;                                 // ID号
	char szText[33];                            // 文本
	BYTE  btState;                              // 状态，见宏定义
}ARRANGEITEM, *LPARRANGEITEM;

typedef struct tagGRAPHGRIDSET					// 图表总体设置
{
	DWORD		dwShowMask;						// 显示掩码，见宏定义，如：GGSSM_HEAD_TITLE
	BYTE		btLockMask;						// 固定显示掩码，见宏定义，如：GGSLM_HEAD
    
	////////////////////////////////////////////////////////////////////////
	UIFont      *fontTitle;						// 标题字体
	UIFont      *fontColumn;					// 列名字体
	UIColor     *crTitleText;					// 标题文本色
	UIColor     *crTitleBK;						// 标题背景色
	UIColor     *crColumnText;					// 列名文本色
	UIColor     *crColumnBK;					// 列名背景色
    
	////////////////////////////////////////////////////////////////////////
	WORD		wRowHeight;						// 表格行高
	UIFont      *fontBody;						// 表格文本字体
	UIColor     *crBodyText;					// 表格文本色（如果启用了交替行，则忽略）
	UIColor     *crBodyBK;						// 表格背景色（如果启用了交替行，则忽略）
    
	UIFont      *fontSkipBig;					// 遗漏值大字体
	UIFont      *fontSkipSmall;					// 遗漏值小字体
	UIColor     *crSkipText;					// 遗漏值文本色（如果启用了交替行，则忽略）
    
	UIColor     *crCurSelRowText;				// 当前选择行文本色
	UIColor     *crCurSelRowBK;					// 当前选择行背景色
    
	BYTE		btForecastRowCount;				// 预测行总数
	UIColor     *crForecastRowText;				// 预测行文本色
	UIColor     *crForecastRowBK;				// 预测行背景色
    
	WORD		wAltRowCount;					// 交替行数
	UIColor     *crAltRowText[2];				// 交替行文本色
	UIColor     *crAltRowBK[2];					// 交替行背景色
    
	WORD		wSpaceRowCount;					// 空行数
    
	////////////////////////////////////////////////////////////////////////
	ARRANGEITEM	stStatItems[32];				// 统计行信息（显示的）
	BYTE		btStatItemCount;				// 统计行数目，0-32，如果为0，则需要重新从对应配置中读取
    
	WORD		wStatRowHeight;					// 统计信息行高
	UIColor     *crStatText;					// 统计信息文本色（如果启用了交替行，则忽略）
	UIColor     *crStatBK;						// 统计信息背景色（如果启用了交替行，则忽略）
	
	UIFont      *fontStatBig;					// 统计信息值大字体
	UIFont      *fontStatSmall;					// 统计信息值小字体
    
	WORD		wStatAltRowCount;				// 统计信息交替行数
	UIColor     *crStatAltRowText[2];			// 统计信息交替行文本色
	UIColor     *crStatAltRowBK[2];				// 统计信息交替行背景色
    
	////////////////////////////////////////////////////////////////////////
	BYTE		btBodyPenStyle;					// 表格线画笔类型（比如：PS_SOLID）
	BYTE		btBodyPenWidth;					// 表格线画笔宽度
	UIColor     *crBodyPen;						// 表格线画笔颜色
    
	BYTE		btBorderPenStyle;				// 表格线画笔类型（比如：PS_SOLID）
	BYTE		btBorderPenWidth;				// 边框线画笔宽度
	UIColor     *crBorderPen;					// 边框线画笔颜色
} GRAPHGRIDSET, *LPGRAPHGRIDSET;


/*typedef struct tagNUMSCOLOREITEM				// 开奖号码颜色单元
 {
    int			nValue;							// 值
    CGColorRef	crText;							// 文本色
    CGColorRef	crBack;							// 背景色
 } NUMSCOLOREITEM, *LPNUMSCOLOREITEM;
 */

typedef struct tagNUMSTABLESET					// 号码表设置（2010-12-21）
{
	DWORD			dwFuncMask;					// 功能掩码，见宏定义，如：NTSFM_SHOW
	
	BYTE			btAltColCount;				// 交替列总数
	UIColor         *crAltColText[2];			// 交替列文本色
	UIColor         *crAltColBK[2];				// 交替列背景色
	
	UIColor         *crNumsDefaultText;			// 开奖号码缺省文本色
	UIColor         *crNumsDefaultBK;			// 开奖号码缺省背景色
	
// TEST	CDTID			cdtID;                  // 条件ID
	BYTE			btNumsColorItemCount;		// 开奖号码缺省列颜色单元个数
// TEST	NUMSCOLOREITEM	stNumsColorItems[16];	// 开奖号码缺省列颜色单元集合
	
} NUMSTABLESET, *LPNUMSTABLESET;

typedef struct tagTRENDGRIDSET									// 走势图设置
{
	DWORD			dwFuncMask;									// 功能掩码，见宏定义，如：TGSFM_CONNECTLINE_SHOW
    
	UIColor         *crBacks[4];								// 背景颜色
    
	BYTE			btPenStyle;									// 画笔类型（比如：PS_SOLID）
	BYTE			btPenWidth;									// 画笔宽度
	UIColor         *crPenColor;								// 画笔颜色
    
	BYTE			btAltCount;									// 交替线条数
	UIColor         *crAltColor[2];								// 交替线颜色
    
	WORD			wBallSize;									// 号码球尺寸
    
	UIFont			*fontBallTexts[4];							// 号码球文字字体
	UIColor         *crBallBorders[4];							// 号码球边框色
	UIColor         *crBallBks[4];								// 号码球背景色
	UIColor         *crBallTexts[4];							// 号码球文字色
    
} TRENDGRIDSET, *LPTRENDGRIDSET;

typedef struct tagDISTRIBUTEGRIDSET								// 分布图设置
{
	DWORD			dwFuncMask;									// 功能掩码，见宏定义，如：DGSFM_ENABLE_BALLSIZE
    
	UIColor         *crBacks[4];								// 背景颜色
	WORD			wBallSize;									// 号码球尺寸
	UIColor         *crBallBorder[4];							// 出现N次的号码球边框色
	UIColor         *crBallBk[4];								// 出现N次的号码球背景色
	UIColor         *crBallText[4];								// 出现N次的号码球文字色
	UIFont			*fontBallText[4];							// 出现N次的号码球文字字体
} DISTRIBUTEGRIDSET, *LPDISTRIBUTEGRIDSET;

// 自定义周期表配置
typedef struct tagCUSTOMZQBDATA 
{
	BYTE	btRowCount;			// 行数
	BYTE	btColCount;			// 列数
	BYTE	btBeginPos;			// 起始位置
	BYTE	btType;				// 类型
}CUSTOMZQBDATA, *LPCUSTOMZQBDATA;

#pragma pack()

///////////////////////////////////////////////////////////////

// 定义号码表列索引
enum{CI_INDEX = 0, CI_DATE, CI_TIME, CI_WEEKDAY, CI_ISSUE, CI_TESTCODE, CI_TESTCODESUM, CI_TESTSPAN, \
    CI_MACHINE, CI_BALL, CI_CODE, CI_CODE2, CI_SUM, CI_SPAN};

class CGraphGridPublic
{
public:
    CGraphGridPublic();
    virtual ~CGraphGridPublic();
    
    void Create(int nColCount);
    void Destroy();
public:
    
    // 获取表格属性
	LPGRAPHGRIDSET GetGridSettings();
    LPTRENDGRIDSET GetTrendGridSettings();
    LPDISTRIBUTEGRIDSET GetDistributeGridSettings();
    LPNUMSTABLESET GetNumsTableSettings();
    
	// 设置表格属性
	void SetGridSettings(LPGRAPHGRIDSET lpGridSet);
    void SetTrendGridSettings(LPTRENDGRIDSET lpTrendGridSet);
    void SetDistributeGridSettings(LPDISTRIBUTEGRIDSET lpDistributeGridSet);
    void SetNumsTableSettings(LPNUMSTABLESET lpNumsTableSet);

    // 获取行高，bIsStatRow为TRUE则获取统计行高度，否则获取表体行高度
	int GetRowHeight(BOOL bIsStatRow = FALSE);

    // 获取列信息，传入－1则返回全部列信息的指针，否则返回指定列信息的指针
    LPGRAPHGRIDCOLINFO GetColumnInfo(int nIndex = -1);
    
    // 获取列总数
	int GetColumnCount(){return m_nColCount;};
    
  	// 设置列宽，列右边框宽度
	void SetColumnWidth(int nColIndex, int nWidth, BYTE btRightBorderWidth = 0);
    // 获取列宽
	int GetColumnWidth(int nColIndex, LPBYTE lpRightBorderWidth = NULL);
    
 	// 设置列名，球文字名，列提示信息
	void SetColumnName(int nColIndex, LPCSTR lpszName, LPCSTR lpszBallText = NULL, LPCSTR lpszInfoTip = NULL);

    // 获取列名
    LPCSTR GetColumnName(int nColIndex, LPSTR lpszBallText = NULL, LPSTR lpszInfoTip = NULL);
    
    // 设置标题信息
	void SetTitle(LPCSTR lpszTitle);
	// 获取标题名
	LPCSTR GetTitle();
    
public:     // 号码球相关
    
    // 添加号码球
	int AddBallBitmap(BYTE btRadius, LPCSTR lpszBallText, UIFont *font, UIColor *crBkcolor, UIColor *crBorderColor, UIColor *crTextColor);
    
	// 删除所有号码球
	void RemoveAllBallBitmaps();
    
    // 获取指定索引的号码球指针
    CBallBitmap *GetBallBitmap(int nIndex);
        
public: // 公共绘图函数
    static void DrawLine(CGContextRef context, int nWidth, int nSrcX, int nSrcY, int nDestX, int nDestY, BOOL bLeftToRight);
    
    static void DrawText(CGContextRef context, CGRect rect, LPCSTR lpszText, UIFont *font, UIColor *textColor, UIColor *backColor = nil, BOOL bAddUnderLine = FALSE);

private:
    void _InitGraphGridSettings();
    void _InitTrendGridSettings();
    void _InitDistributeGridSettings();
    void _InitNumsTableSettings();
    
    UIColor * _ColorFromRGB(BYTE btRed, BYTE btGreen, BYTE btBlue);
	
private:   
    char                        m_szTitle[64];              // 标题
    int                         m_nColCount;                // 列总数
    LPGRAPHGRIDCOLINFO          m_lpColInfo;                // 列信息
    NSMutableArray              *m_cBallBitmapPointArray;	// 号码球指针集合
    
    static GRAPHGRIDSET         ms_stGraphGridSet;          // 表格设置
    static TRENDGRIDSET         ms_stTrendGridSet;          // 走势图设置
    static DISTRIBUTEGRIDSET    ms_stDistributeGridSet;     // 分布图设置
    static NUMSTABLESET         ms_stNumsTableSet;          // 号码表设置
};
