//
//  CCstPubFunc.h
//  QCLottery
//
//  Created by 武 孙 on 12-5-22.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "ICstPubFunc.h"
#include "INumbers.h"
#include "QuickAddString.h"
#include "MFCClass.h"

class CCstPubFunc : public ICstPubFunc  
{
public:
	CCstPubFunc();
	virtual ~CCstPubFunc();
	
public:	
	// 获取最后执行的某些特定函数返回的错误信息
	virtual LPCSTR GetLastErrorString();
    
	// 文本数据串转为整形数据集合，如果返回FALSE，则错误信息可用GetLastErrorString()获取
	virtual BOOL TextToIntArray(LPCSTR lpszText, CDWordArray &cValueArrayOut, int nMinValue = 0, int nMaxValue = 9, BOOL bFill0Prev = TRUE);
	
	// 文本数据串转为位标志缓冲区，返回缓冲区指针，如果返回NULL，则是错误，错误信息可用GetLastErrorString()获取
	// dwBitCountOut为返回的位标志个数，bFill0Prev：是否需要前补0，比如双色球红球值，1为01，bGroData：是否是组选数据，如果是，每一个值都要转换为组选
	virtual LPBYTE TextToBitBuf(LPCSTR lpszText, DWORD &dwBitCountOut, int nMinValue = 0, int nMaxValue = 9, BOOL bFill0Prev = FALSE, BOOL bGroData = FALSE);
	
	// 整形数据集合转为文本数据串，如果返回FALSE，则错误信息可用GetLastErrorString()获取，bNoSeparator指定是否需要分隔符，如果需要则自动添加逗号
	// nFixLen为每个值文本的固定长度，如果为0则不固定，否则如果长度不够则前补0
	virtual LPCSTR IntArrayToText(CDWordArray &cValueArray, BOOL bNoSeparator = FALSE, int nFixLen = 0);
	
	// 整形数据缓冲区转为文本数据串，如果返回FALSE，则错误信息可用GetLastErrorString()获取，bNoSeparator指定是否不需要分隔符，如果不需要则不自动添加逗号
	// nFixLen为每个值文本的固定长度，如果为0则不固定，否则如果长度不够则前补0
	virtual LPCSTR IntBufToText(const int *pValueBuf, const DWORD dwValueCount, BOOL bNoSeparator = FALSE, int nFixLen = 0);
	
	// 位标志缓冲区转换为文本数据串，返回值临时有效，如果返回NULL，则错误信息可用GetLastErrorString()获取
	// dwBitCount为位标志个数，bNoSeparator指定是否不需要分隔符，如果不需要则不自动添加逗号，bFill0Prev：是否需要前补0，比如双色球红球值，1为01
	virtual LPCSTR BitBufToText(const LPBYTE lpBitBuf, const DWORD dwBitCount, int nMinValue = 0, BOOL bNoSeparator = FALSE, BOOL bFill0Prev = FALSE);
	
	// 检查位标志缓冲区中指定索引的单元数据，如果为1，则返回TRUE
	virtual BOOL CheckBitBufItem(const LPBYTE lpBitBuf, const int nItemIndex = 0);
	// 设置位标志缓冲区中指定索引的单元数据，如果bValid==TRUE，则设置为1否则为0
	virtual void SetBitBufItem(LPBYTE lpBitBuf, const int nItemIndex = 0, BOOL bValid = TRUE);
    
	// 快速查找（折半查找），如果找到则返回索引，没有找到则返回-1
	// bValueSortAsc是否正向排序，否则为反向排序，如果没有排序，则不能调用该函数
	virtual int QuckFind_Half(const int *pValueBuf, const DWORD dwValueCount, int nFindValue, BOOL bValueSortAsc = TRUE);
    
	// 快速排序（返回个数） 待排序的值的缓冲区   值个数          值类型（见宏定义）             是否正向排序		是否去重复
	virtual int QuickSort(LPVOID lpValueBuf, int nValueCount, BYTE btValueType = QSVT_BYTE, BOOL bAscOrder = TRUE, BOOL bRemoveSame = FALSE);
	
	// 获取排列组合值，当n==-1时则为排列值，即m!，否则为组合值，即m选n的组合
	virtual int GetArrangeComboValue(int m, int n = -1);
    
	// 获取指定的排列值总个数，nArrValueCount为参与排列的值的个数，比如5分区排列，则是5，热温冷排列则是3，奇偶排列则是2等
	// bGroupArray：为TRUE则是组选排列（也可作为个数排列，因为两者的值是一样的），否则为单选排列
	// btNumberCount：号码个数（也即排列的位置数），如果为0，则自动使用当前号码个数
	// bArrValueRepeatable：为TRUE则表示每个位置上的排列的值可以重复，否则不能重复
	virtual int GetArrangeItemCount(int nArrValueCount, BOOL bGroupArray = TRUE, BYTE btNumberCount = 0, BOOL bArrValueRepeatable = TRUE);
    
	// 获取位置名称，nPos为-1则表示整个开奖号码，bShortName是否获取简称
	virtual LPCSTR GetPosName(int nPos = -1, BOOL bShortName = FALSE, BYTE btDataSource = DATA_SOURCE_CUR);
        
    // 获取指定分区数据，返回的低字中保存该分区的最小值，高字中保存该分区的最大值，返回MAXDWORD的则是错误
	// dwAreaIndex：为分区索引，0为第一分区，nMinValue为本分区的最小值，nMaxValue为最大值，以下同
	DWORD GetSubAreaData(DWORD dwAreaCount, DWORD dwAreaIndex, int nMinValue, int nMaxValue);
	// 获取值所在分区索引，0为第一分区，依次类推，小于则是错误
	int GetSubAreaIndex(DWORD dwAreaCount, int nValue, int nMinValue, int nMaxValue);
	
    // 查询数值是否是质数
    virtual BOOL IsPrime(int nValue);
    
private:
	static int _Sort_BYTE_Asc(const void *arg1, const void *arg2);
	static int _Sort_BYTE_Desc(const void *arg1, const void *arg2);
	static int _Sort_WORD_Asc(const void *arg1, const void *arg2);
	static int _Sort_WORD_Desc(const void *arg1, const void *arg2);
	static int _Sort_DWORD_Asc(const void *arg1, const void *arg2);
	static int _Sort_DWORD_Desc(const void *arg1, const void *arg2);
	static int _Sort_INT_Asc(const void *arg1, const void *arg2);
	static int _Sort_INT_Desc(const void *arg1, const void *arg2);
	static int _Sort_BUFFER_Asc(const void *arg1, const void *arg2);
	static int _Sort_BUFFER_Desc(const void *arg1, const void *arg2);
    
private:
	void _BitBufToIntArray(LPBYTE lpBitBuf, DWORD dwBitCount, int nMinValue, CDWordArray &cValueArrayOut);
	LPCSTR _OrderedIntArrayToText(CDWordArray &cValueArray, BOOL bNoSeparator, int nFill0Count = 0);
	BOOL _TextToIntArray(LPCSTR lpszText, CDWordArray &cValueArrayOut, int nMinValue, int nMaxValue, BOOL bFill0Prev);
    
	BOOL _SetBitValue(LPBYTE lpBitBuf, int nValue, int nMinValue, int nMaxValue, BOOL bFill0Prev, BOOL bGroData);
	BOOL _SetIntValue(CDWordArray &dwValueArray, int &nValueIndex, int nValue, int nMinValue, int nMaxValue, BOOL bFill0Prev);
    
//	CString _ReplaceColonInCustomAreaText(CString &strCustomAreaText);
    
	int _RemoveSameValue(LPBYTE lpValueBuf, int nValueCount, int nValueSize);
    
private:
    typedef struct tagSUBAREAINFO				// 分区信息
	{
		DWORD	dwAreaCount;					// 分区个数
		int		nMinValue;						// 最小值
		int		nMaxValue;						// 最大值
		DWORD	dwAreaInfoArray[1];				// 分区信息集合，一共dwAreaCount个元素(依次为第1分区、第2分区...)，每个元素的高字中保存该分区的最小值，低字中保存该分区的最大值
	} SUBAREAINFO, *LPSUBAREAINFO;
    
	CDWordArray m_cSubAreaInfoPointArray;	// 分区信息指针集合
    BYTE	m_btPrimeFlag[1000];			// 0-999的质数标志，为1，则是质数
    
    LPSUBAREAINFO _GetSubAreaInfo(DWORD dwAreaCount, int nMinValue, int nMaxValue);
	BOOL _IsPrime(int nValue);
	
private:
	char	m_szPublicSaveFilenameInfo[128];	// 公共保存文件名
	char	m_szLastErrorString[1025];			// 错误信息文本
	char	m_szBufToText[1024 * 100];			// 数据缓冲区转换后的文本
    
	CQuickAddString	m_strBufToText;				// 数据缓冲区转换后的文本 
    
	static BYTE	m_btQuickSortBufLen;			// 快速排序时缓冲区长度
};

