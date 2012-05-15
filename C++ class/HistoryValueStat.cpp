//
//  HeaderHistoryValueStat.cpp
//  Graph
//
//  Created by 武 孙 on 12-5-4.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//
//////////////////////////////////////////////////////////////////////

#include "HistoryValueStat.h"
#include "IData.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHistoryValueStat::CHistoryValueStat()
{
	m_nCount = 0;
	m_lpValueArray = NULL;
}

CHistoryValueStat::~CHistoryValueStat()
{
	Destroy();
}

//////////////////////////////////////////////////////////////////////////
BOOL CHistoryValueStat::Create(int nCount)
{
	if(nCount == -1)
	{
		nCount = g_pIData->GetItemCount();	// 获取系统个数
	}
    
	if(nCount == 0)	// 没有，则销毁当前的并返回成功
	{
		Destroy();
		return TRUE;
	}
    
	if(m_nCount == nCount)	// 如果个数相同，则直接清空所有数据即可
	{
		ResetAllValue();
	}
	else	// 个数不同，则重新创建
	{
		Destroy();
        
		NEW_BUFFER(m_lpValueArray, int, nCount);
		m_nCount = nCount;
	}
    
	return TRUE;
}

void CHistoryValueStat::Destroy()
{
	if(m_nCount == 0)
	{
		return ;
	}
    
	DELETE_BUFFER(m_lpValueArray);
}

int CHistoryValueStat::GetCount()
{
	return m_nCount;
}

void CHistoryValueStat::ResetAllValue()
{
	if(m_lpValueArray == NULL)
	{
		return ;
	}
    
	ZeroMemory(m_lpValueArray, sizeof(int) * m_nCount);
}

void CHistoryValueStat::SetValue(int nIndex, int nValue)
{
	ASSERT(nIndex >= 0 && nIndex < m_nCount);
	ASSERT(m_lpValueArray != NULL);
    
	m_lpValueArray[nIndex] = nValue;
}

int CHistoryValueStat::GetValue(int nIndex)
{
	ASSERT(nIndex >= 0 && nIndex < m_nCount);
	ASSERT(m_lpValueArray != NULL);
    
	return m_lpValueArray[nIndex];
}

void CHistoryValueStat::FillSkip()
{
	int nSkip = 0;
    
	for(int i=0; i<m_nCount; i++)
	{
		if(m_lpValueArray[i] > 0)
		{
			nSkip = 0;
		}
		else
		{
			nSkip ++;
			m_lpValueArray[i] = 0 - nSkip;
		}
	}
}

LPSTATRESULT CHistoryValueStat::Stat(int nBeginIndex, BOOL bFillSkip)
{
	ASSERT(nBeginIndex >= 0 && nBeginIndex < m_nCount);
    
	m_stStatResult.Initialize();
    
	int nCount = m_nCount - nBeginIndex;
	if(nCount <= 0 || m_lpValueArray == NULL)
	{
		return &m_stStatResult;
	}
    
	// 初始化相关最小值
	m_stStatResult.dwMinSkip = m_stStatResult.dwMinLink = MAXDWORD;
	m_stStatResult.dwMinSkipNo0 = m_stStatResult.dwMinLinkNo1 = m_stStatResult.dwMinLinkSkip = MAXDWORD;
    
	DWORD dwMatchCountSingle = 0;	// 单个值出现总数，即同一期的数据多个值算一个
    
	for(int i=nBeginIndex; i<m_nCount; i++)
	{
		if(m_lpValueArray[i] > 0)		// 中出
		{
			// 计算中出次数 
			m_stStatResult.dwMatchCount += m_lpValueArray[i];
			dwMatchCountSingle ++;
            
			m_stStatResult.dwCurLink ++;		// 当前连出次数加1
            
			// 计算最大遗漏值
			if(m_stStatResult.dwMaxSkip < m_stStatResult.dwCurSkip)
			{
				m_stStatResult.dwMaxSkip = m_stStatResult.dwCurSkip;
			}
            
			// 计算最小遗漏值
			if(m_stStatResult.dwMinSkip > m_stStatResult.dwCurSkip)
			{
				m_stStatResult.dwMinSkip = m_stStatResult.dwCurSkip;
			}
            
			// 计算不含0的最小遗漏
			if(m_stStatResult.dwCurSkip != 0 && m_stStatResult.dwCurSkip < m_stStatResult.dwMinSkipNo0)
			{
				m_stStatResult.dwMinSkipNo0 = m_stStatResult.dwCurSkip;
			}
            
			// 设置上期遗漏
			m_stStatResult.dwPrevSkip = m_stStatResult.dwCurSkip;
			
			m_stStatResult.dwCurSkip = 0;	// 当前遗漏复位
            
			// 计算连出遗漏
			if(m_stStatResult.dwCurLink > 1)
			{
				// 计算最大连出遗漏值
				if(m_stStatResult.dwCurLinkSkip > 0 && (m_stStatResult.dwCurLinkSkip - 1 > m_stStatResult.dwMaxLinkSkip))
				{
					m_stStatResult.dwMaxLinkSkip = m_stStatResult.dwCurLinkSkip - 1;
				}
                
				// 计算最小连出遗漏值
				if(m_stStatResult.dwCurLinkSkip > 1 && m_stStatResult.dwCurLinkSkip < m_stStatResult.dwMinLinkSkip)
					m_stStatResult.dwMinLinkSkip = m_stStatResult.dwCurLinkSkip - 1;
                
				m_stStatResult.dwCurLinkSkip = 0;	// 当前连出遗漏值复位
			}
			else
			{
				m_stStatResult.dwCurLinkSkip ++;	// 当前连出遗漏值加1
			}
		}
		else // 未中出
		{
			m_stStatResult.dwCurSkip ++;		// 当前遗漏值加1
            
			if(bFillSkip)	// 填充遗漏值
				m_lpValueArray[i] = 0 - m_stStatResult.dwCurSkip;
            
			// 计算最大连出值
			if(m_stStatResult.dwCurLink > m_stStatResult.dwMaxLink)
			{
				m_stStatResult.dwMaxLink = m_stStatResult.dwCurLink;
			}
	           
			// 计算最小连出值
			if(m_stStatResult.dwCurLink != 0 && m_stStatResult.dwCurLink < m_stStatResult.dwMinLink)
			{
				m_stStatResult.dwMinLink = m_stStatResult.dwCurLink;
			}
            
			// 计算不含1的最小连出值
			if(m_stStatResult.dwCurLink > 1 && m_stStatResult.dwCurLink < m_stStatResult.dwMinLinkNo1)
			{
				m_stStatResult.dwMinLinkNo1 = m_stStatResult.dwCurLink;
			}
            
			m_stStatResult.dwCurLink = 0;		// 当前连出复位
            
			m_stStatResult.dwCurLinkSkip ++;	// 当前连出遗漏值加1
		}
	}
    
	// 最小值有效性检查
	if(m_stStatResult.dwMinSkip == MAXDWORD)
		m_stStatResult.dwMinSkip = 0;
	if(m_stStatResult.dwMinLink == MAXDWORD)
		m_stStatResult.dwMinLink = 0;
	if(m_stStatResult.dwMinSkipNo0 == MAXDWORD)
		m_stStatResult.dwMinSkipNo0 = 0;
	if(m_stStatResult.dwMinLinkNo1 == MAXDWORD)
		m_stStatResult.dwMinLinkNo1 = 0;
    
	// 计算中出概率
	m_stStatResult.fMatchProb = double(double(m_stStatResult.dwMatchCount) / double(nCount) * 100.0);
    
	// 计算平均遗漏
	if(dwMatchCountSingle == 0)
		m_stStatResult.fAvgSkip = 0.0;
	else
	{
		int nTemp = nCount - 1 - dwMatchCountSingle - m_stStatResult.dwCurSkip;
		if(nTemp < 0)
			m_stStatResult.fAvgSkip = 0.0;
		else
			m_stStatResult.fAvgSkip = double(nTemp) / double(dwMatchCountSingle);
	}
    
	// 计算实际遗漏价值
	m_stStatResult.fRealSkipValue = m_stStatResult.dwCurSkip * m_stStatResult.fMatchProb / 100;
    
	// 计算极值偏态指数
	if(m_stStatResult.dwMaxSkip == 0)
		m_stStatResult.fExtremeSkewnessIndex = 0.0;
	else
		m_stStatResult.fExtremeSkewnessIndex = (double)m_stStatResult.dwCurSkip / (double)m_stStatResult.dwMaxSkip * 100.0;
    
	// 计算实际偏态指数
	if(dwMatchCountSingle == 0 || m_stStatResult.fAvgSkip == 0)
		m_stStatResult.fRealSkewnessIndex = 0.0;
	else
		m_stStatResult.fRealSkewnessIndex = (double)m_stStatResult.dwCurSkip / (double)m_stStatResult.fAvgSkip * 100.0;
    
	return &m_stStatResult;
}

UIColor * CHistoryValueStat::GetStatValueColor(DWORD dwValueType, UIColor *defaultColor)
{
	return m_stStatResult.GetValueColor(dwValueType, defaultColor);
}

DWORD CHistoryValueStat::GetSeriSkip(int nSeriIssue, DWORD &dwSeriCurSkip, DWORD &dwSeriMaxSkip)
{
/*	dwSeriCurSkip = 0;
	dwSeriMaxSkip = 0;
    
	int nSkipValIndex = 0;
	int nTemp=0;
	int i=0, j=0;
    
	CUIntArray uSkipValAry;
	uSkipValAry.SetSize(m_nCount+1);
    
	for(i=0; i<m_nCount; i++)
	{
		if(m_lpValueArray[i] > 0)
		{
			uSkipValAry[nSkipValIndex++] = nTemp;
			nTemp = 0;
		}
		else
		{
			nTemp ++;
		}
        
		if(i == m_nCount-1)
		{
			uSkipValAry[nSkipValIndex++] = nTemp;
		}
	}
	uSkipValAry.SetSize(nSkipValIndex);
    
	//////////////////////////////////////////////////////////////////////////
	// 计算连续当前遗漏
	if(nSkipValIndex < nSeriIssue)
	{
		dwSeriCurSkip = 0;
	}
	else
	{
		for(i=0; i<nSeriIssue; i++)
		{
			dwSeriCurSkip += uSkipValAry[nSkipValIndex-1-i];
		}
	}
	//////////////////////////////////////////////////////////////////////////
	// 计算连续最大遗漏
	
	nTemp = 0;
	dwSeriMaxSkip = 0;
    
	if(nSkipValIndex <= nSeriIssue)
	{
		dwSeriMaxSkip = 0;
	}
	else
	{
		for(i=0; i<nSkipValIndex-nSeriIssue; i++)
		{
			for(j=0; j<nSeriIssue; j++)
			{
				nTemp += uSkipValAry[i+j];
			}
			
			if(nTemp > (int)dwSeriMaxSkip)
			{
				dwSeriMaxSkip = nTemp;
			}
			nTemp = 0;
		}
	}
    */
	return 0;
}

