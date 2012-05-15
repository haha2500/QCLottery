//
//  HeaderHistoryValueStat.h
//  Graph
//
//  Created by 武 孙 on 12-5-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

// 定义GetValueColor中btValueType的取值
enum{HVT_MatchProb = 0, HVT_MatchCount, 
    HVT_CurSkip, HVT_PrevSkip, HVT_MinSkip, HVT_MinSkipNo0, HVT_MaxSkip, HVT_AvgSkip, 
    HVT_CurLink, HVT_PrevLink, HVT_MinLink, HVT_MinLinkNo1, HVT_MaxLink, 
    HVT_CurLinkSkip, HVT_MinLinkSkip, HVT_MaxLinkSkip, HVT_RealSkipValue, HVT_ExtremeSkewIndex, HVT_RealSkewIndex,
};

#pragma pack(1)

// 定义统计结果数据结构
typedef struct tagSTATRESULT				// 统计结果
{
	DWORD			dwMatchCount;			// 中出次数
	double			fMatchProb;				// 中出概率
    
	DWORD			dwCurSkip;				// 当前遗漏
	DWORD			dwPrevSkip;				// 上期遗漏
	DWORD			dwMinSkip;				// 最小遗漏
	DWORD			dwMinSkipNo0;			// 最小遗漏不含0
	DWORD			dwMaxSkip;				// 最大遗漏
	double			fAvgSkip;				// 平均遗漏
    
	DWORD			dwCurLink;				// 当前连出
	DWORD			dwPrevLink;				// 上期连出
	DWORD			dwMinLink;				// 最小连出
	DWORD			dwMinLinkNo1;			// 最小连出不含1
	DWORD			dwMaxLink;				// 最大连出
    
	DWORD			dwCurLinkSkip;			// 当前连出遗漏
	DWORD			dwMinLinkSkip;			// 最小连出遗漏
	DWORD			dwMaxLinkSkip;			// 最大连出遗漏
    
	double			fRealSkipValue;			// 实际遗漏价值
	double			fExtremeSkewnessIndex;	// 极值偏态指数
	double			fRealSkewnessIndex;		// 实际偏态指数
		
	tagSTATRESULT()
	{
		Initialize();
	}
    
	void Initialize()	// 初始化所有元素
	{
		ZeroMemory(this, sizeof(tagSTATRESULT));
	}
	UIColor * GetValueColor(DWORD dwValueType = HVT_CurSkip, UIColor *defaultColor = nil)
	{
        UIColor *crRed = [UIColor redColor];
        if (defaultColor == nil) defaultColor = [UIColor blackColor];
        
		switch(dwValueType)
		{
            case HVT_CurSkip: return (dwCurSkip >= dwMaxSkip) ? crRed : (((double)dwCurSkip >= fAvgSkip) ? [UIColor blueColor] : defaultColor);		
            case HVT_MaxSkip: return (dwCurSkip >= dwMaxSkip) ? crRed : defaultColor;
            case HVT_AvgSkip: return ((double)dwCurSkip >= fAvgSkip) ? crRed : defaultColor;
            case HVT_CurLink: return (dwCurLink >= dwMaxLink) ? crRed : defaultColor;
            case HVT_MaxLink: return (dwCurLink >= dwMaxLink) ? crRed : defaultColor;
            case HVT_RealSkewIndex: return ((double)dwCurSkip >= fAvgSkip) ? crRed : defaultColor;
		}
		
		return defaultColor;
	}
    
	BOOL GetValueRemarks(LPSTR lpszRemarks, DWORD dwValueType = HVT_CurSkip)
	{
		ASSERT(lpszRemarks != NULL);
        
		switch(dwValueType)
		{
            case HVT_MatchProb:	sprintf(lpszRemarks, "中出概率：%.2f%%。", fMatchProb); break;
            case HVT_MatchCount: sprintf(lpszRemarks, "中出次数：%lu。", dwMatchCount); break;
            case HVT_CurSkip: sprintf(lpszRemarks, "当前遗漏值：%lu", dwCurSkip); break;
            case HVT_PrevSkip: sprintf(lpszRemarks, "上期遗漏：%lu。", dwPrevSkip); break;
            case HVT_MinSkip: sprintf(lpszRemarks, "历史最小遗漏：%lu。", dwMinSkip); break;
            case HVT_MinSkipNo0: sprintf(lpszRemarks, "历史最小遗漏(不含0)：%lu。", dwMinSkipNo0); break;
            case HVT_MaxSkip: 
                if(dwCurSkip >= dwMaxSkip)
                    sprintf(lpszRemarks, "历史最大遗漏值：%lu，红色数字表示当前遗漏值大于等于最大遗漏值。", dwMaxSkip);
                else
                    sprintf(lpszRemarks, "历史最大遗漏值：%lu，黑色数字表示当前遗漏值小于最大遗漏值。", dwMaxSkip);;
                break;
                
            case HVT_AvgSkip:
                if((double)dwCurSkip >= fAvgSkip)
                    sprintf(lpszRemarks, "历史平均遗漏值：%.2f，红色数字表示当前遗漏值大于等于平均遗漏值。", fAvgSkip);
                else
                    sprintf(lpszRemarks, "历史平均遗漏值：%.2f，黑色数字表示当前遗漏值小于平均遗漏值。", fAvgSkip);
                break;
            case HVT_CurLink: 
                if(dwCurLink >= dwMaxLink)
                    sprintf(lpszRemarks, "当前连出值：%lu，红色数字表示当前连出值大于等于最大连出值。", dwCurLink);
                else
                    sprintf(lpszRemarks, "当前连出值：%lu，黑色数字表示当前连出值小于最大连出值。", dwCurLink);
                break;
            case HVT_PrevLink: sprintf(lpszRemarks, "上期连出：%lu。", dwPrevLink); break;
            case HVT_MinLink: sprintf(lpszRemarks, "历史最小连出：%lu。", dwMinLink); break;
            case HVT_MinLinkNo1: sprintf(lpszRemarks, "历史最小连出(不含1)：%lu。", dwMinLinkNo1); break;
            case HVT_MaxLink:
                sprintf(lpszRemarks, "历史最大连出值：%lu。", dwMaxLink);
                break;
            case HVT_CurLinkSkip: sprintf(lpszRemarks, "当前连出遗漏：%lu。", dwCurLinkSkip); break;
            case HVT_MinLinkSkip: sprintf(lpszRemarks, "历史最小连出遗漏：%lu。", dwMinLinkSkip); break;
            case HVT_MaxLinkSkip:
                sprintf(lpszRemarks, "历史最大连出遗漏：%lu。", dwMaxLinkSkip);
                break;
            case HVT_RealSkipValue: sprintf(lpszRemarks, "实际遗漏价值：%.3f。", fRealSkipValue); break;
            case HVT_ExtremeSkewIndex: sprintf(lpszRemarks, "极值偏态指数：%.3f%%。", fExtremeSkewnessIndex); break;
            case HVT_RealSkewIndex: sprintf(lpszRemarks, "实际偏态指数：%.3f%%。", fRealSkewnessIndex); break;
            default: return FALSE;
		}
        
		return TRUE;
	};
    
	const tagSTATRESULT& operator =(const tagSTATRESULT& stValue)
	{
		memcpy(this, &stValue, sizeof(stValue));
		return *this;
	};
} STATRESULT, *LPSTATRESULT;

#pragma pack()

class CHistoryValueStat 
{
public:
	CHistoryValueStat();
	virtual ~CHistoryValueStat();
    
public:		// 接口函数
	
	// 创建，如果nCount==-1，则系统使用当前数据总数值
	virtual BOOL Create(int nCount = -1);
	// 销毁
	virtual void Destroy();
	
	// 获取数据总数
	virtual int GetCount();
    
	// 重新设置所有数据为0
	virtual void ResetAllValue();
	
	// 设置指定索引的值，通常为出现次数
	virtual void SetValue(int nIndex, int nValue = 1);
    
	// 获取指定索引的值，如果为负数，则是遗漏值
	virtual int GetValue(int nIndex);
	
	// 填充遗漏值
	virtual void FillSkip();
    
	// 统计
	virtual LPSTATRESULT Stat(int nBeginIndex = 0, BOOL bFillSkip = TRUE);
    
	// 获取统计信息指针
	virtual LPSTATRESULT GetStatResult(){return &m_stStatResult;};
    
	// 获取统计相关值（比如当前遗漏值）的颜色值
	virtual UIColor *GetStatValueColor(DWORD dwValueType = HVT_CurSkip, UIColor *defaultColor = nil);
	
	// 获取连续N期的当前遗漏、最大遗漏
	virtual DWORD GetSeriSkip(int nSeriIssue, DWORD &dwSeriCurSkip, DWORD &dwSeriMaxSkip);
    
private:
	int			m_nCount;					// 总数
	int			*m_lpValueArray;			// 值集合
	STATRESULT	m_stStatResult;				// 统计结果
};
