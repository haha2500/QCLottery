//
//  Def_ConditionCustom.h
//  QCLottery
//
//  Created by 武 孙 on 12-5-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

// 定义CIP_HEAD::wFlag的取值
#define		CIPHF_AREA						1	// 自定义分区
#define		CIPHF_REMAINDER					2	// 自定义余数

// ===========================================================================
// 以下定义自定义指标相关数据结构



// ===========================================================================
// 以下定义自定义指标参数相关数据结构

#pragma pack(1)

typedef struct tagCIP_HEAD						// 自定义指标参数头
{
	DWORD		dwDiffValues[2];				// 区分数据
	WORD		wFlag;							// 标志信息，见宏定义，如：CIPHF_AREA
	WORD		wRes;							// 系统预留
	DWORD		dwDataLen;						// 数据长度
	LPBYTE		lpDataBuf;						// 数据缓冲区
    
	tagCIP_HEAD()
	{
		ZeroMemory(this, sizeof(tagCIP_HEAD));
	};
	~tagCIP_HEAD()
	{
		Destroy();
	};
    
	void Create(WORD wFlag, DWORD dwDataLen)
	{
		Destroy();
        
		this->dwDiffValues[0] = time(NULL);
		this->dwDiffValues[1] = rand();
		this->wFlag = wFlag;
		this->dwDataLen = dwDataLen;
		if(dwDataLen > 0)
		{
			this->lpDataBuf = new BYTE[dwDataLen];
		}
	};
    
	void Destroy()
	{
		if(dwDataLen > 0)
		{
			delete []lpDataBuf;
			dwDataLen = 0;
		}
		ZeroMemory(this, sizeof(tagCIP_HEAD));
	};
	
	BOOL IsSameID(tagCIP_HEAD *lpSrcCIPHead) const
	{
		ASSERT(lpSrcCIPHead != NULL);
		return ((dwDiffValues[0] == lpSrcCIPHead->dwDiffValues[0]) && dwDiffValues[1] == lpSrcCIPHead->dwDiffValues[1]) ? TRUE : FALSE;
	};
    
	void operator=(const tagCIP_HEAD &stSrcCIPHead)
	{
		Destroy();
        
		memcpy(this, &stSrcCIPHead, sizeof(tagCIP_HEAD));
		if(dwDataLen > 0)
		{
			lpDataBuf = new BYTE[dwDataLen];
			memcpy(lpDataBuf, stSrcCIPHead.lpDataBuf, dwDataLen);
		}
	};
    
	int GetLen()		// 获取整个数据结构大小
	{
		return (sizeof(tagCIP_HEAD) - sizeof(lpDataBuf) + dwDataLen);
	};
	
	int LoadData(LPBYTE lpDataBufIn)		// 从缓冲区中装载，返回长度
	{
		Destroy();
		
		ASSERT(lpDataBufIn != NULL);
		int nLen = 0, nTempLen = sizeof(tagCIP_HEAD) - sizeof(lpDataBuf);
		memcpy(this, &lpDataBufIn[nLen], nTempLen);
		nLen += nTempLen;
		lpDataBuf = new BYTE[dwDataLen];
		memcpy(this->lpDataBuf, &lpDataBufIn[nLen], dwDataLen);
        
		return (nLen + dwDataLen);
	};
	
	int SaveData(LPBYTE lpDataBufIn)	// 保存到缓冲区，返回长度
	{
		ASSERT(lpDataBufIn != NULL);
		int nLen = 0, nTempLen = sizeof(tagCIP_HEAD) - sizeof(lpDataBuf);
		memcpy(&lpDataBufIn[nLen], this, nTempLen);
		nLen += nTempLen;
		memcpy(&lpDataBufIn[nLen], lpDataBuf, dwDataLen);
		nLen += dwDataLen;
		
		return nLen;
	};
}CIP_HEAD, *LPCIP_HEAD;

typedef struct tagCIP_CUSTOMAREA				// 自定义分区
{
	WORD		wAreaCount;						// 分区个数
	BYTE		btRes;							// 保留
	char		szName[33];						// 名称
	char		szShortName[17];				// 简称
	int			nMinValue;						// 最小值
	int			nMaxValue;						// 最大值，如果为CDTVALUE_INVALID，则表示无限
	BYTE		btSubAreaMinValue;				// 子分区的最小值，即决定分区的起始值，比如系统缺省为1，可以设为0以方便一些自定义使用
	WORD		wAreaDataCount;					// 分区数据个数
	BYTE		btBufBegin[1];					// 缓冲区开始，前wAreaDataCount个字为分区数据缓冲区，每个元素为对应值所在的分区索引（btSubAreaMinValue+n为第n分区），第一个元素对应于最小值，如果最大值为无限值，则最后一个元素为无限值所在分区（通常为最后一个分区）
    //             后17*wAreaCount个字节为子分区名称
    
	int GetLen()
	{
		return sizeof(tagCIP_CUSTOMAREA) - sizeof(btBufBegin) + wAreaDataCount * sizeof(WORD) + 17 * wAreaCount;
	};
    
	LPCSTR GetSubAreaName(int nSubIndex)
	{
		ASSERT(nSubIndex >= 0 && nSubIndex < wAreaCount);
		return (LPCSTR)&btBufBegin[wAreaDataCount * sizeof(WORD) + nSubIndex * 17];
	};
    
	int GetSubAreaIndex(int nValue)
	{
		ASSERT(nValue >= nMinValue && nValue <= nMaxValue);
		int nData = nValue - nMinValue;
		if(nData < 0 || nData >= wAreaDataCount)
		{
			return (wAreaCount - 1 + btSubAreaMinValue);		// 不在数据范围内的统一定位最后一个分区
		}
		return ((LPWORD)btBufBegin)[nData];
	};
    
}CIP_CUSTOMAREA, *LPCIP_CUSTOMAREA;

typedef struct tagCIP_CUSTOMREMAINDER				// 自定义余数
{
	int			nDivisor;						// 除数，目前最大为10000
	int			nMinValue;						// 最小值
	int			nMaxValue;						// 最大值，如果为CDTVALUE_INVALID，则表示无限
}CIP_CUSTOMREMAINDER, *LPCIP_CUSTOMREMAINDER;

#pragma pack()

