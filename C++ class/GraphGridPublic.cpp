//
//  GraphGridPublic.cpp
//  Graph
//
//  Created by 武 孙 on 12-5-5.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GraphGridPublic.h"

GRAPHGRIDSET CGraphGridPublic::ms_stGraphGridSet = {0};
TRENDGRIDSET CGraphGridPublic::ms_stTrendGridSet = {0};
DISTRIBUTEGRIDSET CGraphGridPublic::ms_stDistributeGridSet = {0};
NUMSTABLESET CGraphGridPublic::ms_stNumsTableSet = {0};

/////////////////////////////////////////////////////////////////////////////////////
CGraphGridPublic::CGraphGridPublic()
{
   	m_lpColInfo = NULL; 
    m_nColCount = 0;
    ZeroMemory(m_szTitle, sizeof(m_szTitle));
    
    _InitGraphGridSettings();
    _InitTrendGridSettings();
    _InitDistributeGridSettings();
    _InitNumsTableSettings();
}

CGraphGridPublic::~CGraphGridPublic()
{
    Destroy();
}

void CGraphGridPublic::Create(int nColCount)
{
    ASSERT(m_lpColInfo == NULL);
    
    m_lpColInfo = new GRAPHGRIDCOLINFO[nColCount];
    ZeroMemory(m_lpColInfo, nColCount * sizeof(GRAPHGRIDCOLINFO));
    
    m_cBallBitmapPointArray = [[NSMutableArray alloc]init];

	for(int i=0; i<nColCount; i++)
	{
		m_lpColInfo[i].btRightBorderWidth = 0;
		m_lpColInfo[i].nWidth = ms_stGraphGridSet.wRowHeight;
	}
    m_nColCount = nColCount;
}

void CGraphGridPublic::Destroy()
{
    DELETE_BUFFER(m_lpColInfo);
    m_nColCount = 0;
    RemoveAllBallBitmaps();
}

/////////////////////////////////////////////////////////////////////////////////////
LPGRAPHGRIDSET CGraphGridPublic::GetGridSettings()
{
	return &ms_stGraphGridSet;
}
LPTRENDGRIDSET CGraphGridPublic::GetTrendGridSettings()
{
    return &ms_stTrendGridSet;
}

LPDISTRIBUTEGRIDSET CGraphGridPublic::GetDistributeGridSettings()
{
    return &ms_stDistributeGridSet;
}

LPNUMSTABLESET CGraphGridPublic::GetNumsTableSettings()
{
    return &ms_stNumsTableSet;
}

void CGraphGridPublic::SetGridSettings(LPGRAPHGRIDSET lpGridSet)
{
	ASSERT(lpGridSet != NULL);
	
	memcpy(&ms_stGraphGridSet, lpGridSet, sizeof(ms_stGraphGridSet));
}

void CGraphGridPublic::SetTrendGridSettings(LPTRENDGRIDSET lpTrendGridSet)
{
    ASSERT(lpTrendGridSet != NULL);
	
	memcpy(&ms_stTrendGridSet, lpTrendGridSet, sizeof(ms_stTrendGridSet));
}

void CGraphGridPublic::SetDistributeGridSettings(LPDISTRIBUTEGRIDSET lpDistributeGridSet)
{
    ASSERT(lpDistributeGridSet != NULL);
	
	memcpy(&ms_stDistributeGridSet, lpDistributeGridSet, sizeof(ms_stDistributeGridSet));
}

void CGraphGridPublic::SetNumsTableSettings(LPNUMSTABLESET lpNumsTableSet)
{
    ASSERT(lpNumsTableSet != NULL);
	
	memcpy(&ms_stNumsTableSet, lpNumsTableSet, sizeof(ms_stNumsTableSet));
}

int CGraphGridPublic::GetRowHeight(BOOL bIsStatRow)
{
	return bIsStatRow ? ms_stGraphGridSet.wStatRowHeight : ms_stGraphGridSet.wRowHeight;
}

LPGRAPHGRIDCOLINFO CGraphGridPublic::GetColumnInfo(int nIndex)
{
    if (nIndex < 0 || nIndex >= m_nColCount) 
    {
        return m_lpColInfo;
    }
    
    return &m_lpColInfo[nIndex];
};

void CGraphGridPublic::SetColumnWidth(int nColIndex, int nWidth, BYTE btRightBorderWidth)
{
	ASSERT(nColIndex >= 0 && nColIndex < GetColumnCount());
    
	if(nWidth != -1)
		m_lpColInfo[nColIndex].nWidth = nWidth;
	if(btRightBorderWidth != MAXBYTE)
		m_lpColInfo[nColIndex].btRightBorderWidth = btRightBorderWidth;
}

int CGraphGridPublic::GetColumnWidth(int nColIndex, LPBYTE lpRightBorderWidth)
{
	ASSERT(nColIndex >= 0 && nColIndex < GetColumnCount());
    
	if(lpRightBorderWidth != NULL)
	{
		*lpRightBorderWidth = m_lpColInfo[nColIndex].btRightBorderWidth;
	}
	
	return m_lpColInfo[nColIndex].nWidth;
}

void CGraphGridPublic::SetColumnName(int nColIndex, LPCSTR lpszName, LPCSTR lpszBallText, LPCSTR lpszInfoTip)
{
	ASSERT(nColIndex >= 0 && nColIndex < GetColumnCount());
    
	if(lpszName != NULL)
	{
		ASSERT(strlen(lpszName) < sizeof(m_lpColInfo[nColIndex].szName));
		strcpy(m_lpColInfo[nColIndex].szName, lpszName);
		CHECK_STRING_STACK(m_lpColInfo[nColIndex].szName);
	}
    
	if(lpszBallText != NULL)
	{
		ASSERT(strlen(lpszBallText) < sizeof(m_lpColInfo[nColIndex].szBallText));
		strcpy(m_lpColInfo[nColIndex].szBallText, lpszBallText);
		CHECK_STRING_STACK(m_lpColInfo[nColIndex].szBallText);
	}
	else
	{
		if(strlen(m_lpColInfo[nColIndex].szName) < sizeof(m_lpColInfo[nColIndex].szBallText))
		{
			strcpy(m_lpColInfo[nColIndex].szBallText, m_lpColInfo[nColIndex].szName);
		}
		else
		{
			m_lpColInfo[nColIndex].szBallText[0] = 0;
		}
	}
	
	if(lpszInfoTip != NULL)
	{
		ASSERT(strlen(lpszInfoTip) < sizeof(m_lpColInfo[nColIndex].szInfoTip));
		strcpy(m_lpColInfo[nColIndex].szInfoTip, lpszInfoTip);
		CHECK_STRING_STACK(m_lpColInfo[nColIndex].szInfoTip);
	}
	else
	{
		strcpy(m_lpColInfo[nColIndex].szInfoTip, m_lpColInfo[nColIndex].szName);
	}
}

LPCSTR CGraphGridPublic::GetColumnName(int nColIndex, LPSTR lpszBallText, LPSTR lpszInfoTip)
{
	ASSERT(nColIndex >= 0 && nColIndex < GetColumnCount());
    
	if(lpszBallText != NULL)
	{
		strcpy(lpszBallText, m_lpColInfo[nColIndex].szBallText);
	}
    
	if(lpszInfoTip != NULL)
	{
		strcpy(lpszInfoTip, m_lpColInfo[nColIndex].szInfoTip);
	}
    
	return m_lpColInfo[nColIndex].szName;
}

void CGraphGridPublic::SetTitle(LPCSTR lpszTitle)
{
	if(strlen(lpszTitle) >= sizeof(m_szTitle))
    {
        ASSERT(FALSE);
        memcpy(m_szTitle, lpszTitle, sizeof(m_szTitle) - 1);
        m_szTitle[sizeof(m_szTitle) - 1] = 0;
    }
	else
    {
        strcpy(m_szTitle, lpszTitle);
    }
}

LPCSTR CGraphGridPublic::GetTitle()
{
	return m_szTitle;
}

int CGraphGridPublic::AddBallBitmap(BYTE btRadius, LPCSTR lpszBallText, UIFont *font, UIColor *crBkcolor, UIColor *crBorderColor, UIColor *crTextColor)
{
	if(lpszBallText == NULL || strlen(lpszBallText) == 0)   // 添加一个空的
	{
		[m_cBallBitmapPointArray addObject:[NSNumber numberWithInt:0]];
        return [m_cBallBitmapPointArray count] - 1;
	}
    
	CBallBitmap *pBallBitmap = new CBallBitmap;
    
	pBallBitmap->Create(btRadius, crBkcolor, lpszBallText, font, crTextColor, crBorderColor);
	
    [m_cBallBitmapPointArray addObject:[NSNumber numberWithInt:(int)pBallBitmap]];
    return [m_cBallBitmapPointArray count] - 1;
}

void CGraphGridPublic::RemoveAllBallBitmaps()
{
    for(NSNumber *ball in m_cBallBitmapPointArray)
    {
        if([ball intValue] == 0)
        {
            continue;
        }
        delete (CBallBitmap *)[ball intValue];
    }
    [m_cBallBitmapPointArray removeAllObjects];
}

CBallBitmap *CGraphGridPublic::GetBallBitmap(int nIndex)
{
    if(nIndex < 0 || nIndex >= [m_cBallBitmapPointArray count])
    {
        ASSERT(FALSE);      // 越界
        return NULL;
    }
    
    NSNumber *number = [m_cBallBitmapPointArray objectAtIndex:nIndex];
    
    return (CBallBitmap *)[number intValue];
}

/////////////////////////////////////////////////////////
void CGraphGridPublic::DrawLine(CGContextRef context, int nWidth, int nSrcX, int nSrcY, int nDestX, int nDestY, BOOL bLeftToRight)
{
    CGContextSetLineWidth(context, 1);
	if(bLeftToRight)
	{
		for(int i=0; i<nWidth; i++)
		{
            CGContextMoveToPoint(context, nSrcX, nSrcY + i);
            CGContextAddLineToPoint(context, nDestX, nDestY + i);
		}
	}
	else
	{
		for(int i=0; i<nWidth; i++)
		{
            CGContextMoveToPoint(context, nSrcX + i, nSrcY);
            CGContextAddLineToPoint(context, nDestX + i, nDestY);
		}
	}
    
    CGContextSetShouldAntialias(context, NO);
    CGContextStrokePath(context);
}

void CGraphGridPublic::DrawText(CGContextRef context, CGRect rect, LPCSTR lpszText, UIFont *font, UIColor *textColor, UIColor *backColor, BOOL bAddUnderLine)
{
    ASSERT(lpszText != NULL && font != nil && textColor != nil);
    
    // 填充背景色
    if(backColor != nil)
    {
        [backColor setFill];
        CGContextFillRect(context, rect);
    }
    
    // 将文本专为NSString
    NSString *text = [NSString stringWithUTF8String:lpszText];
    
    // 计算文字所需矩形大小
    CGSize sizeText = [text sizeWithFont:font];
    CGPoint ptOrg = CGPointMake(rect.origin.x + (rect.size.width - sizeText.width) / 2, rect.origin.y + (rect.size.height - sizeText.height) / 2);
    
    [textColor set];
    if(bAddUnderLine)   // 增加下划线
    {
        CGContextSetLineWidth(context, 2);
        CGContextMoveToPoint(context, ptOrg.x, ptOrg.y + sizeText.height - 3);
        CGContextAddLineToPoint(context, ptOrg.x + sizeText.width, ptOrg.y + sizeText.height - 3);
        CGContextStrokePath(context);
        ptOrg.y -= 1;
    }
    
    // 居中显示文字
    CGContextSetShouldAntialias(context, YES);
    [text drawAtPoint:ptOrg withFont:font]; 
}

////////////////////////////////////////////////////////////////////////////////////////////////////
void CGraphGridPublic::_InitGraphGridSettings()
{
    if (ms_stGraphGridSet.fontTitle != nil)
    {
        return;     // 已经初始化过
    }
  
    UIColor *blackColor = [UIColor blackColor], *whiteColor = [UIColor whiteColor];
    UIFont *titleFont = [UIFont systemFontOfSize:18], *normalFont = [UIFont systemFontOfSize:16];
    UIFont *skipBigFont = [UIFont systemFontOfSize:12], *skipSmallFont = [UIFont systemFontOfSize:10];

    ms_stGraphGridSet.dwShowMask = GGSSM_DEFAULT;					// 显示掩码，见宏定义，如：GGSSM_HEAD_TITLE
	ms_stGraphGridSet.btLockMask = 0;								// 固定显示掩码，见宏定义，如：GGSLM_HEAD
	
 	////////////////////////////////////////////////////////////////////////
	ms_stGraphGridSet.fontTitle = titleFont;                        // 标题字体
	ms_stGraphGridSet.fontColumn = ms_stGraphGridSet.fontTitle;		// 列名字体
	ms_stGraphGridSet.crTitleText = blackColor;                     // 标题文本色
	ms_stGraphGridSet.crTitleBK = whiteColor;                       // 标题背景色
	ms_stGraphGridSet.crColumnText = blackColor;					// 列名文本色
	ms_stGraphGridSet.crColumnBK = whiteColor;                      // 列名背景色
	
	////////////////////////////////////////////////////////////////////////
	ms_stGraphGridSet.wRowHeight = 22;								// 表格行高
	ms_stGraphGridSet.fontBody = normalFont;                        // 表格文本字体
	ms_stGraphGridSet.crBodyText = blackColor;                      // 表格文本色（如果启用了交替行，则忽略）
	ms_stGraphGridSet.crBodyBK = whiteColor;                        // 表格背景色（如果启用了交替行，则忽略）
	
	ms_stGraphGridSet.fontSkipBig = skipBigFont;                    // 遗漏值大字体
	ms_stGraphGridSet.fontSkipSmall = skipSmallFont;                // 遗漏值小字体
	ms_stGraphGridSet.crSkipText = [UIColor grayColor];             // 遗漏值文本色（如果启用了交替行，则忽略）
	
	ms_stGraphGridSet.crCurSelRowText = blackColor;                 // 当前选择行文本色
	ms_stGraphGridSet.crCurSelRowBK = _ColorFromRGB(183, 188, 251);	// 当前选择行背景色
	
	ms_stGraphGridSet.btForecastRowCount = 1;						// 预测行总数
	ms_stGraphGridSet.crForecastRowText = [UIColor redColor];		// 预测行文本色
	ms_stGraphGridSet.crForecastRowBK = _ColorFromRGB(240, 240, 240);// 预测行背景色
	
	ms_stGraphGridSet.wAltRowCount = 5;                             // 交替行数，如果为负值则是未启用
	ms_stGraphGridSet.crAltRowText[0] = blackColor;                 // 交替行1文本色
	ms_stGraphGridSet.crAltRowText[1] = blackColor;                 // 交替行2文本色
	ms_stGraphGridSet.crAltRowBK[0] = whiteColor;                   // 交替行1背景色
	ms_stGraphGridSet.crAltRowBK[1] = _ColorFromRGB(200, 255, 215);	// 交替行2背景色
    
	ms_stGraphGridSet.wSpaceRowCount = 1;							// 空行数
	
	////////////////////////////////////////////////////////////////////////
	ms_stGraphGridSet.wStatRowHeight = 16;							// 统计信息行高
	ms_stGraphGridSet.crStatText = blackColor;                      // 统计信息文本色（如果启用了交替行，则忽略）
	ms_stGraphGridSet.crStatBK = whiteColor;                        // 统计信息背景色（如果启用了交替行，则忽略）
	
	ms_stGraphGridSet.fontStatBig = ms_stGraphGridSet.fontSkipBig;		// 统计信息值大字体
	ms_stGraphGridSet.fontStatSmall = ms_stGraphGridSet.fontSkipSmall;	// 统计信息值小字体
	
	ms_stGraphGridSet.wStatAltRowCount = 1;                         // 统计信息交替行数，如果为负值则是未启用
	ms_stGraphGridSet.crStatAltRowText[0] = blackColor;             // 统计信息交替行1文本色
	ms_stGraphGridSet.crStatAltRowText[1] = blackColor;             // 统计信息交替行2文本色
	ms_stGraphGridSet.crStatAltRowBK[0] = _ColorFromRGB(255, 255, 192);		// 统计信息交替行1背景色
	ms_stGraphGridSet.crStatAltRowBK[1] = _ColorFromRGB(192, 255, 255);		// 统计信息交替行2背景色
	
    // 以下设置统计信息栏项目
    ms_stGraphGridSet.btStatItemCount = 5;
    ms_stGraphGridSet.stStatItems[0].dwID = HVT_MatchCount;
    ms_stGraphGridSet.stStatItems[0].btState = AIS_SHOW;
    strcpy(ms_stGraphGridSet.stStatItems[0].szText, "出现次数");
    ms_stGraphGridSet.stStatItems[1].dwID = HVT_CurSkip;
    ms_stGraphGridSet.stStatItems[1].btState = AIS_SHOW;
    strcpy(ms_stGraphGridSet.stStatItems[1].szText, "当前遗漏");
    ms_stGraphGridSet.stStatItems[2].dwID = HVT_AvgSkip;
    ms_stGraphGridSet.stStatItems[2].btState = AIS_SHOW;
    strcpy(ms_stGraphGridSet.stStatItems[2].szText, "历史平均遗漏");
    ms_stGraphGridSet.stStatItems[3].dwID = HVT_MaxSkip;
    ms_stGraphGridSet.stStatItems[3].btState = AIS_SHOW;
    strcpy(ms_stGraphGridSet.stStatItems[3].szText, "历史最大遗漏");
    ms_stGraphGridSet.stStatItems[4].dwID = HVT_MaxLink;
    ms_stGraphGridSet.stStatItems[4].btState = AIS_SHOW;
    strcpy(ms_stGraphGridSet.stStatItems[4].szText, "历史最大连出");
	////////////////////////////////////////////////////////////////////////
//	ms_stGraphGridSet.btBodyPenStyle = PS_SOLID;					// 表格线画笔类型（比如：PS_SOLID）
	ms_stGraphGridSet.btBodyPenWidth = 1;							// 表格线画笔宽度
	ms_stGraphGridSet.crBodyPen = blackColor;                       // 表格线画笔颜色
	
//	ms_stGraphGridSet.btBorderPenStyle = PS_SOLID;					// 表格线画笔类型（比如：PS_SOLID）
	ms_stGraphGridSet.btBorderPenWidth = 2;                         // 边框线画笔宽度
	ms_stGraphGridSet.crBorderPen = blackColor;                     // 边框线画笔颜色
}

void CGraphGridPublic::_InitTrendGridSettings()
{
    if (ms_stTrendGridSet.wBallSize != 0)
    {
        return;     // 已经初始化过
    }
    
    ms_stTrendGridSet.dwFuncMask = TGSFM_DEFAULT;
    
    UIColor *whiteColor = [UIColor whiteColor], *blackColor = [UIColor blackColor], *orangeColor = [UIColor orangeColor], *blueColor = [UIColor blueColor], *redColor = [UIColor redColor];
    for(int i=0; i<4; i++)
    {
        ms_stTrendGridSet.crBacks[i] = whiteColor;
	}
    
//	ms_stTrendGridSet.btPenStyle = PS_SOLID;
	ms_stTrendGridSet.btPenWidth = 2;
	ms_stTrendGridSet.crPenColor = blackColor;
    
	ms_stTrendGridSet.btAltCount = 5;
	ms_stTrendGridSet.crAltColor[0] = blueColor;
	ms_stTrendGridSet.crAltColor[1] = redColor;
    
	ms_stTrendGridSet.wBallSize = MAXWORD;
	
    UIFont *font = [UIFont systemFontOfSize:16];
	for(int i=0; i<4; i++)
	{
		ms_stTrendGridSet.fontBallTexts[i] = font;
	}
    
	ms_stTrendGridSet.crBallBorders[0] = redColor;
	ms_stTrendGridSet.crBallBorders[1] = blueColor;
	ms_stTrendGridSet.crBallBorders[2] = orangeColor;
	ms_stTrendGridSet.crBallBorders[3] = blackColor;
    
	ms_stTrendGridSet.crBallBks[0] = redColor;
	ms_stTrendGridSet.crBallBks[1] = blueColor;
	ms_stTrendGridSet.crBallBks[2] = orangeColor;
	ms_stTrendGridSet.crBallBks[3] = blueColor;
    
	
	ms_stTrendGridSet.crBallTexts[0] = whiteColor;
	ms_stTrendGridSet.crBallTexts[1] = whiteColor;
	ms_stTrendGridSet.crBallTexts[2] = whiteColor;
	ms_stTrendGridSet.crBallTexts[3] = whiteColor;
}

void CGraphGridPublic::_InitDistributeGridSettings()
{
    if(ms_stDistributeGridSet.wBallSize != 0)
    {
        return;     // 已经初始化过
    }
    
    ms_stDistributeGridSet.dwFuncMask = DGSFM_DEFAULT;
    
    UIColor *whiteColor = [UIColor whiteColor], *blackColor = [UIColor blackColor], *blueColor = [UIColor blueColor], *redColor = [UIColor redColor];
    
	ms_stDistributeGridSet.crBacks[0] = whiteColor;
	ms_stDistributeGridSet.crBacks[1] = whiteColor;
	ms_stDistributeGridSet.crBacks[2] = whiteColor;
	ms_stDistributeGridSet.crBacks[3] = whiteColor;

	ms_stDistributeGridSet.wBallSize = MAXWORD;
    
    UIFont *font = [UIFont systemFontOfSize:16];
    
	ms_stDistributeGridSet.crBallText[0] = blackColor;
	ms_stDistributeGridSet.crBallBk[0] = whiteColor;
	ms_stDistributeGridSet.crBallBorder[0] = blackColor;
	ms_stDistributeGridSet.fontBallText[0] = font;
    
	ms_stDistributeGridSet.crBallText[1] = whiteColor;
	ms_stDistributeGridSet.crBallBk[1] = blueColor;
	ms_stDistributeGridSet.crBallBorder[1] = blueColor;
	ms_stDistributeGridSet.fontBallText[1] = font;
    
	ms_stDistributeGridSet.crBallText[2] = whiteColor;
	ms_stDistributeGridSet.crBallBk[2] = redColor;
	ms_stDistributeGridSet.crBallBorder[2] = redColor;
	ms_stDistributeGridSet.fontBallText[2] = font;
    
	ms_stDistributeGridSet.crBallText[3] = whiteColor;
	ms_stDistributeGridSet.crBallBk[3] = blackColor;
	ms_stDistributeGridSet.crBallBorder[3] = blackColor;
	ms_stDistributeGridSet.fontBallText[3] = font;
}

void CGraphGridPublic::_InitNumsTableSettings()
{
    if(ms_stNumsTableSet.crNumsDefaultText != nil)
    {
        return;     // 已经初始化过
    }
    
    ms_stNumsTableSet.dwFuncMask = NTSFM_DEFAULT;
	
    UIColor *whiteColor = [UIColor whiteColor], *blackColor = [UIColor blackColor], *blueColor = [UIColor blueColor], *redColor = [UIColor redColor];
    
	ms_stNumsTableSet.btAltColCount = 1;
	ms_stNumsTableSet.crAltColText[0] = blackColor;
	ms_stNumsTableSet.crAltColText[1] = _ColorFromRGB(0, 0, 192);
	ms_stNumsTableSet.crAltColBK[0] = whiteColor;
	ms_stNumsTableSet.crAltColBK[1] = whiteColor;

    ms_stNumsTableSet.crNumsDefaultText = 1 ? blackColor : (2 ? redColor : blueColor);  // TEST
    ms_stNumsTableSet.crNumsDefaultBK = whiteColor;
}

UIColor * CGraphGridPublic::_ColorFromRGB(BYTE btRed, BYTE btGreen, BYTE btBlue)
{
    return [UIColor colorWithRed:(float)btRed/255.0 green:(float)btGreen/255.0 blue:(float)btBlue/255.0 alpha:1.0];
}