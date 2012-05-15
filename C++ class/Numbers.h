//
//  Numbers.h
//  Graph
//
//  Created by 武 孙 on 12-5-10.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "INumbers.h"
#include "IData.h"
#include "QuickAddString.h"
#include "NumbersUndo.h"

#define		DELETE_ITEM_FLAG                0X80	// 删除标志

// 检查是否是有效的数据单元（即确保不是需要删除的数据单元）
#define		ISVALIDITEM(lpItemData)		(!(lpItemData[0] & DELETE_ITEM_FLAG))

class CNumbers : public INumbers
{
public:
    
	CNumbers();
	virtual ~CNumbers();
	
public:
	
	// 按指定数据源方式创建指定dwCount个数的号码集，dwCount为0，则创建一个空的
	// 当数据源为原始数据源（DATA_SOURCE_ORG）时，btNumberCount和btNumberCount2才有效
	// btNumberCount：主要用于复式个数，为0则表示单式，由系统根据当前号码属性设置该值
	// btNumberCount2：附加号码个数，用于双色球，6+1等，为0则表示没有附加号码，主要用于缩水器号码添加第二区段号码时使用
	virtual BOOL Create(DWORD dwCount, BYTE btDataSource = DATA_SOURCE_CUR, BYTE btNumberCount = 0, BYTE btNumberCount2 = 0);
    
	// 根据字符串lpszData装载相关数据，返回字符串中号码个数（根据bRemoveSame是否去重复以后）
	// bAppend：是否是添加到当前数据，否则是覆盖
	// bRemoveSame：是否去重复（如果去重复，则自动进行排序）
	virtual int LoadFromString(LPCSTR lpszData, BOOL bAppend = FALSE, BOOL bRemoveSame = TRUE);
    
	// 自动组号，按当前彩票信息，比如3D，则自动创建所有1000个有效号码，bOnlyCalcCount为TRUE时，则只计算号码个数，并不分配空间和生成数据
	virtual int AutoComboNumbers(BOOL bOnlyCalcCount = FALSE);	
    
	// 销毁
	virtual void Destroy();
    
	// 设置号码总数，系统会自动保留和丢弃原有号码
	virtual void SetCount(DWORD dwCount);
	// 获取号码总数
	virtual DWORD GetCount(){return m_dwCount;};
    
	// 获取数据源
	virtual BYTE GetDataSource(){return m_btDataSource;};
	// 获取数据类型
	virtual BYTE GetDataType(){return m_btDataType;};
	// 获取第一区段号码的组成个数
	virtual BYTE GetNumberCount(){return m_btNumberCount;};
	// 获取第二区段号码的组成个数
	virtual BYTE GetNumberCount2(){return m_btNumberCount2;};
	// 获取最大号码
	virtual BYTE GetMaxNumber(){return m_btMaxNumber;};
	// 获取最小号码
	virtual BYTE GetMinNumber(){return m_btMinNumber;};
    
	// 获取缓冲区
	virtual const LPBYTE GetBuffer(){return (const LPBYTE)m_lpDataItemArray;};
	
	// 添加其它数据      待添加的数据     是否去重复（如果去重复，则自动进行排序）
	virtual BOOL Append(INumbers *pSrc, BOOL bRemoveSame = TRUE);
	// 复制数据
	virtual void Copy(INumbers *pSrc);
	// 数据是否相同
	virtual BOOL IsSame(INumbers *pSrc);
    
	// 设置分隔符，在获取号码相关字符串时使用，如果为NULL，则表示使用缺省值
	virtual void SetSeparator(LPNUMBERSSEPARATOR lpNumbersSep = NULL);
    
	// 设置通配符，在LoadFromString中有效
	virtual void SetXChar(char caX = '*');
	// 获取当前设置的通配符
	virtual char GetXChar();
    
	// 统计所有号码出现的次数，调用完成后号码会自动排序并去重复，可以使用GetItemCustomData获取对应号码的次数
	// bSortByCount：是否按统计的次数排序，如果为TRUE，则进行排序（此时：bAsc为TRUE则次数值从小到大，否则从大到小）
	// 返回最大次数，lpMinCount：接收最小次数
	virtual DWORD StatAllItems(BOOL bSortByCount = FALSE, BOOL bAsc = TRUE, LPDWORD lpMinCount = NULL);
    
	// 获取统计后的字符串，nColumn：列数（0为不分列）
	// bSortByCount：是否按统计的次数排序，如果为TRUE，则进行排序（此时：bAsc为TRUE则次数值从小到大，否则从大到小）
	virtual LPSTR GetStatString(int nColumn = 10, BOOL bSortByCount = TRUE, BOOL bAsc = TRUE);
	// 释放统计后的字符串
	virtual void ReleaseStatString(LPSTR lpszStatString);
    
	// 获取所有数据组成的的文本串      列数（0为不分列）      获取类型见宏定义     每个M行（高字节）空N行（低字节）（0为不启用）
	virtual LPCSTR GetAllDataString(int nColCount, DWORD dwType = GADST_NORMAL, WORD wAutoAddSpaceRow = 0);
	// 释放所有数据组成的的文本串 
	virtual void ReleaseAllDataString();
	
	// 设置指定数据
	virtual void SetItemData(DWORD dwIndex, LPBYTE lpData);
	// 增加单个数据，返回数据所在索引，bNoAddIfExist：当存在相同数据不增加。
	virtual int AddItemData(LPBYTE lpData, BOOL bNoAddIfExist = TRUE);
	// 修改指定索引的数据，bFailIfExists：如果已经存在和需要修改的数据相同的数据，则返回失败
	virtual BOOL ModifyItemData(DWORD dwIndex, LPBYTE lpData, BOOL bFailIfExists = TRUE);
	// 设置指定数据的自定义数据
	virtual void SetItemCustomData(DWORD dwIndex, DWORD dwCustomData);
	// 重置所有自定义数据
	virtual void ResetAllCustomData(DWORD dwCustomData = 0);
    
	// 获取指定数据，btGetFlag为获取数据标志，见宏定义
	virtual LPBYTE GetItemData(DWORD dwIndex, BYTE btGetFlag = GF_ORG);
	// 获取指定数据的自定义数据
	virtual DWORD GetItemCustomData(DWORD dwIndex);
	// 获取指定数据的文本串，btGetFlag为获取数据标志，见宏定义
	virtual LPCSTR GetItemString(DWORD dwIndex, BYTE btGetFlag = GF_ORG);
	// 获取指定数据的值，btGetFlag为获取数据标志，见宏定义
	virtual DWORD GetItemValue(DWORD dwIndex, BYTE btGetFlag = GF_ORG);
	
	// 删除数据，只设置删除标志，需要调用RemoveData才会真正的删除
	virtual void DeleteItemData(DWORD dwIndex);	
	// 清除所有需要被删除的数据，如果pRemovedData不为NULL，则被删除数据保存在pRemovedData中，返回删除总数
	virtual int RemoveData(INumbers *pRemovedData = NULL);	
	// 恢复所有被删除数据标志，将所有数据置为不删除标志
	virtual void UnRemoveData();
	// 清除所有数据
	virtual void RemoveAllData();
    
	// 取反（即从所有号码中排除当前所有号码），bSingle：单组选标志
	virtual void ReverseAllData(BOOL bSingle = TRUE);
    
	// 排序（按自定义数据和号码统一排序）
	virtual void Sort(BOOL bAsc = TRUE);
	// 排序扩展（按自定义数据和号码分别排序）
	virtual void SortEx(BOOL bAscData = TRUE, BOOL bAscNums = TRUE);
	// 去重复（系统自动排序以后去重复）
	virtual void RemoveSameData();
	// 排序指定的号码数据
	virtual void SortItemData(DWORD dwIndex, BOOL bAsc = TRUE);
    
	// 组单互变，bRemoveSaveNums：排序后删除相同的号码，如果为FASE，则保留所有号码并且不排序
	virtual void SinToGro(BOOL bRemoveSaveNums = TRUE);
	virtual void GroToSin(BOOL bRemoveSaveNums = TRUE);
	
	// 保存数据文件
	virtual BOOL SaveData(LPCSTR lpszFilename);
	virtual BOOL LoadData(LPCSTR lpszFilename);
    
	// 导入和导出数据，用于保存条件时保存缩水器号码
	virtual BOOL ImportData(LPBYTE lpDataBuf, int nBufLen);
	virtual int ExportData(LPBYTE lpDataBuf, int nBufLen);
    
	// 填充随机号码				被填充的对象               是否需要系统产生随机种子
	virtual void FillRandomData(INumbers *pDestNumbers, BOOL bNeedSeed = TRUE);
	
	// 随机号码处理，只保留dwRandomCount个随机数据     是否需要系统产生随机种子
	virtual void TreatRandomData(DWORD dwRandomCount, BOOL bNeedSeed = TRUE);
	
	// 获取中出个数，以数字三为例，返回3D中出个数，lpMatchData：比较的数据，一般为开奖号
	// lpNDCountArray：返回的ND中出个数，依次为0D、1D和2D的中出个数，bSingle：为TRUE时是单选，否则为组选
	virtual DWORD GetMatchCount(LPBYTE lpMatchData, LPDWORD lpNDCountArray, BOOL bSingle = TRUE);
	
	// 保存撤销信息
	virtual void SaveUndo();
	// 撤销
	virtual void Undo();
	// 恢复
	virtual void Redo();
	// 是否可以撤销
	virtual BOOL CanUndo();
	// 是否可以恢复
	virtual BOOL CanRedo();
	
	// 当前数据转换为原始数据，pIAMNumbers：保存转换后的数据，如果为NULL，则直接覆盖当前数据
	virtual void TransCurToOrg(INumbers *pOrgNumbers = NULL);
	// 原始数据转换为当前数据，pICurNumbers：保存转换后的数据，如果为NULL，则直接覆盖当前数据
	virtual void TransOrgToCur(INumbers *pCurNumbers = NULL);
	
	// 获取保存文本文件是的标志串（用于双色球等记录号码个数的信息串），lpszTextOut为接收缓冲区，注意长度不要少于128
	// 如果不需要标志串，则返回FALSE
	virtual BOOL GetTextFileFlagString(LPSTR lpszTextOut);
    
	// 复式分解成单式
	virtual void ReduceComplex();
    
	// 设置第二类型号码数据           数据信息集合             模式，见宏定义
	virtual void SetNumber2Data(CByteArray *pNumber2DataArray, BYTE btMode = N2DM_TOGETHER);
	// 获取第二类型号码数据，返回模式
	virtual BYTE GetNumber2Data(CByteArray *pNumber2DataArray);
	
	// 查询数据单元，返回索引，未找到则返回-1，bHalfFind：是否是折半查找，如果是，则数据必须是正序以后的。
	virtual int FindItem(LPBYTE lpData, BOOL bHalfFind = TRUE);
    
private:	// 排序相关私有函数
	static int _DescendSortNums(const void *arg1, const void *arg2);
	static int _AscendSortNums(const void *arg1, const void *arg2);
	static int _AscendSortItem(const void *arg1, const void *arg2);
	static int _DescendSortItem(const void *arg1, const void *arg2);
	static BYTE ms_btWholeNumberCount;	// 号码总个数，快速排序时使用
	static BOOL ms_bAscSortData;		// 正向排序自定义数据，用于SortEx
	
private:	// 彩票相关私有函数
	int _AutoComboNums_SZ(BOOL bOnlyCalcCount);
	int _AutoComboNums_LT(BOOL bOnlyCalcCount);
	int _AutoComboNums_POSLT(BOOL bOnlyCalcCount);
	
	int _LoadFromString_SZ(LPCSTR lpszData, CNumbers *pNumbers);
	int _LoadFromString_LT(LPCSTR lpszData, CNumbers *pNumbers);
	int _LoadFromString_POSLT(LPCSTR lpszData, CNumbers *pNumbers);
	
	void _ExpandFromXItem_SZ(LPBYTE lpXData);	// 展开通配符
	
	int _ByteToChar_SZ(LPSTR lpString, LPBYTE lpBytes, int nByteCount, LPCSTR lpszSep, int nSepLen);
	int _ByteToChar_LT(LPSTR lpString, LPBYTE lpBytes, int nByteCount, LPCSTR lpszSep, int nSepLen);
	
	void _ReduceComplex_LT(BYTE btSingleNumberCount);
	
	DWORD _GetItemValue_POSLT(LPBYTE lpData);			// 获取按位乐透型值
	DWORD _GetItemValue_LT(LPBYTE lpData);				// 获取乐透型值，优化过的
	
	DWORD _LT_To_Index(LPBYTE lpData, BYTE btNumberCount = 4);		// 乐透转化为索引
	DWORD _GetItemValue_LT_Prev4_Index(LPBYTE lpData);	// 获取前4位乐透型索引值
	void _Initialize_LT_Prev4_ValueArray();				// 初始化优化前处理数据集合
	
	DWORD _Comb_LT(int m, int n);		// 乐透组合（从m中选n）
	DWORD _Comb_POSLT(int m, int n);	// 按位乐透组合（从m中选n，排列）
	DWORD _CalcXN(int m, int n, int k);	// 计算C(m, n) + C(m-1, n) + ... + C(m-k, n)
	
private:	// 其他私有函数
    
    void _GetNumbersSeparator(LPNUMBERSSEPARATOR lpNumsSep, BYTE btDataType);
    
	void _GetAllDataStringByCustomData(CQuickAddString &strTextOut, int nColCount, WORD wAutoAddSpaceRow, DWORD dwCustomData = MAXDWORD, int *pCountOut = NULL);
	void _GetAllDataStringByNumsType(CQuickAddString &strTextOut, int nColCount);
	
	// 是否是号码数字
	BOOL _IsNumsDigital(char caNumber);
	
	// 根据标志格式化分析数据
	void _TransAMDataByFlag(BYTE btFlag, LPBYTE lpNums, BYTE btNumCount);
	
	// 清除第2区段号码数据
	void _RemoveNumber2Data();
    
private:	
	LPBYTE	m_lpDataItemArray;		// 数据缓冲区，每个单元的数据为：号码+号码2+自定义数据
	DWORD	m_dwCount;				// 数据总数
    
	BYTE	m_btDataSource;			// 数据源
    
	BYTE	m_btNumberCount;		// 号码个数
	BYTE	m_btNumberCount2;		// 号码个数2（用于原始数据源和初始数据源）
    
	BYTE	m_btDataType;			// 号码类型
	BYTE	m_btDataType2;			// 号码类型2（用于原始数据源和初始数据源）
	BYTE	m_btMaxNumber;			// 最大号码
	BYTE	m_btMaxNumber2;			// 最大号码2（用于原始数据源和初始数据源）
	BYTE	m_btMinNumber;			// 最小号码
	BYTE	m_btMinNumber2;			// 最小号码2（用于原始数据源和初始数据源）
    
	BYTE	m_btWholeNumberCount;	// 号码总个数，即m_btNumberCount + m_btNumberCount2
    
	BYTE	m_btItemSize;			// 单个数据长度，包括号码和自定义数据
	char	m_szDataString[128];	// 数据串，用于GetItemString()
    
	static BYTE	ms_btCDSize;		// 自定义数据长度
    
	BYTE	m_btCurData[MAX_NUMBER_COUNT];	// 当前数据临时缓冲区（用于原始数据源号码转为当前数据源时）
    
	NUMBERSSEPARATOR	m_stNumsSep;		// 号码分割符信息
	NUMBERSSEPARATOR	m_stNumsFilterSep;	// 号码分割符信息（用于缩水数据类型，目前用于缩水器的缩水号码显示）
	
	CNumbersUndo	*m_pNumbersUndo;	// 撤销恢复对象
    
	BYTE            m_btNumber2Mode;	// 第二类型数据模式
	CByteArray      m_btNumber2Array;	// 第二类型数据信息
    
	char            m_caX;          // 通配符
    
	CDWordArray     m_dwPrev4LTValueArray;	// 乐透型值优化前处理值集合
    
	CQuickAddString	m_strAllDataString;	// 用于GetAllDataString的返回
};