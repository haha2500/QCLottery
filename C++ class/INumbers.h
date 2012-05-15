//
//  INumbers.h
//  Graph
//
//  Created by 武 孙 on 12-5-10.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#pragma once

#include "IData.h"
#include "Def_CfgItem_Data.h"
#include "MFCClass.h"

// 定义GetAllDataString中dwType的取值
#define		GADST_NORMAL					1			// 一般类型
#define		GADST_NUMSTYPE					2			// 按号码类型分类

// 定义SetNumber2Data中btMode的取值
#define		N2DM_TOGETHER					1			// 所有的一起添加
#define		N2DM_CIRCLE						2			// 循环添加每一个
#define		N2DM_RANDOM						3			// 随机添加每一个

// 定义GetItemData等函数的btGetFlag的取值
#define		GF_ORG							0			// 原始数据
#define		GF_CUR							1			// 当前数据
#define		GF_AM							10			// 分析方式数据

class INumbers
{
public:
	// 按指定数据源方式创建指定dwCount个数的号码集，dwCount为0，则创建一个空的
	// 当数据源为原始数据源（DATA_SOURCE_ORG）时，btNumberCount和btNumberCount2才有效
	// btNumberCount：主要用于复式个数，为0则表示单式，由系统根据当前号码属性设置该值
	// btNumberCount2：附加号码个数，用于双色球，6+1等，为0则表示没有附加号码，主要用于缩水器号码添加第二区段号码时使用
	virtual BOOL Create(DWORD dwCount, BYTE btDataSource = DATA_SOURCE_CUR, BYTE btNumberCount = 0, BYTE btNumberCount2 = 0) = 0;
    
	// 根据字符串lpszData装载相关数据，返回字符串中号码个数（根据bRemoveSame是否去重复以后）
	// bAppend：是否是添加到当前数据，否则是覆盖
	// bRemoveSame：是否去重复（如果去重复，则自动进行排序）
	virtual int LoadFromString(LPCSTR lpszData, BOOL bAppend = FALSE, BOOL bRemoveSame = TRUE) = 0;
    
	// 自动组号，按当前彩票信息，比如3D，则自动创建所有1000个有效号码，bOnlyCalcCount为TRUE时，则只计算号码个数，并不分配空间和生成数据
	virtual int AutoComboNumbers(BOOL bOnlyCalcCount = FALSE) = 0;	
    
	// 销毁
	virtual void Destroy() = 0;
    
	// 设置号码总数，系统会自动保留和丢弃原有号码
	virtual void SetCount(DWORD dwCount) = 0;
	// 获取号码总数
	virtual DWORD GetCount() = 0;
    
	// 获取数据源
	virtual BYTE GetDataSource() = 0;
	// 获取数据类型
	virtual BYTE GetDataType() = 0;
	// 获取第一区段号码的组成个数
	virtual BYTE GetNumberCount() = 0;
	// 获取第二区段号码的组成个数
	virtual BYTE GetNumberCount2() = 0;
	// 获取最大号码
	virtual BYTE GetMaxNumber() = 0;
	// 获取最小号码
	virtual BYTE GetMinNumber() = 0;
    
	// 获取缓冲区
	virtual const LPBYTE GetBuffer() = 0;
	
	// 添加其它数据      待添加的数据     是否去重复（如果去重复，则自动进行排序）
	virtual BOOL Append(INumbers *pSrc, BOOL bRemoveSame = TRUE) = 0;
	// 复制数据
	virtual void Copy(INumbers *pSrc) = 0;
	// 数据是否相同
	virtual BOOL IsSame(INumbers *pSrc) = 0;
    
	// 设置分隔符，在获取号码相关字符串时使用，如果为NULL，则表示使用缺省值
	virtual void SetSeparator(LPNUMBERSSEPARATOR lpNumbersSep = NULL) = 0;
    
	// 设置通配符，在LoadFromString中有效
	virtual void SetXChar(char caX = '*') = 0;
	// 获取当前设置的通配符
	virtual char GetXChar() = 0;
    
	// 统计所有号码出现的次数，调用完成后号码会自动排序并去重复，可以使用GetItemCustomData获取对应号码的次数
	// bSortByCount：是否按统计的次数排序，如果为TRUE，则进行排序（此时：bAsc为TRUE则次数值从小到大，否则从大到小）
	// 返回最大次数，lpMinCount：接收最小次数
	virtual DWORD StatAllItems(BOOL bSortByCount = FALSE, BOOL bAsc = TRUE, LPDWORD lpMinCount = NULL) = 0;
    
	// 获取统计后的字符串，nColumn：列数（0为不分列）
	// bSortByCount：是否按统计的次数排序，如果为TRUE，则进行排序（此时：bAsc为TRUE则次数值从小到大，否则从大到小）
	virtual LPSTR GetStatString(int nColumn = 10, BOOL bSortByCount = TRUE, BOOL bAsc = TRUE) = 0;
	// 释放统计后的字符串
	virtual void ReleaseStatString(LPSTR lpszStatString) = 0;
    
	// 获取所有数据组成的的文本串      列数（0为不分列）      获取类型见宏定义     每个M行（高字节）空N行（低字节）（0为不启用）
	virtual LPCSTR GetAllDataString(int nColCount = 0, DWORD dwType = GADST_NORMAL, WORD wAutoAddSpaceRow = 0) = 0;
	// 释放所有数据组成的的文本串 
	virtual void ReleaseAllDataString() = 0;
	
	// 设置指定数据
	virtual void SetItemData(DWORD dwIndex, LPBYTE lpData) = 0;
	// 增加单个数据，返回数据所在索引，bNoAddIfExist：当存在相同数据不增加。
	virtual int AddItemData(LPBYTE lpData, BOOL bNoAddIfExist = TRUE) = 0;
	// 修改指定索引的数据，bFailIfExists：如果已经存在和需要修改的数据相同的数据，则返回失败
	virtual BOOL ModifyItemData(DWORD dwIndex, LPBYTE lpData, BOOL bFailIfExists = TRUE) = 0;
	// 设置指定数据的自定义数据
	virtual void SetItemCustomData(DWORD dwIndex, DWORD dwCustomData) = 0;
	// 重置所有自定义数据
	virtual void ResetAllCustomData(DWORD dwCustomData = 0) = 0;
    
#ifdef _PROFESSIONAL	// 专业版
	// 获取指定数据，btGetFlag为获取数据标志，见宏定义
	virtual LPBYTE GetItemData(DWORD dwIndex, BYTE btGetFlag = GF_ORG) = 0;
	// 获取指定数据的自定义数据
	virtual DWORD GetItemCustomData(DWORD dwIndex) = 0;
	// 获取指定数据的文本串，btGetFlag为获取数据标志，见宏定义
	virtual LPCSTR GetItemString(DWORD dwIndex, BYTE btGetFlag = GF_ORG) = 0;
	// 获取指定数据的值，btGetFlag为获取数据标志，见宏定义
	virtual DWORD GetItemValue(DWORD dwIndex, BYTE btGetFlag = GF_ORG) = 0;
#else
	// 获取指定数据的自定义数据
	virtual DWORD GetItemCustomData(DWORD dwIndex) = 0;
	// 获取指定数据的值，btGetFlag为获取数据标志，见宏定义
	virtual DWORD GetItemValue(DWORD dwIndex, BYTE btGetFlag = GF_ORG) = 0;
	// 获取指定数据，btGetFlag为获取数据标志，见宏定义
	virtual LPBYTE GetItemData(DWORD dwIndex, BYTE btGetFlag = GF_ORG) = 0;
	// 获取指定数据的文本串，btGetFlag为获取数据标志，见宏定义
	virtual LPCSTR GetItemString(DWORD dwIndex, BYTE btGetFlag = GF_ORG) = 0;
    
#endif
	
	// 删除数据，只设置删除标志，需要调用RemoveData才会真正的删除
	virtual void DeleteItemData(DWORD dwIndex) = 0;	
	// 清除所有需要被删除的数据，如果pRemovedData不为NULL，则被删除数据保存在pRemovedData中，返回删除总数
	virtual int RemoveData(INumbers *pRemovedData = NULL) = 0;	
	// 恢复所有被删除数据标志，将所有数据置为不删除标志
	virtual void UnRemoveData() = 0;
	// 清除所有数据
	virtual void RemoveAllData() = 0;
    
	// 取反（即从所有号码中排除当前所有号码），bSingle：单组选标志
	virtual void ReverseAllData(BOOL bSingle = TRUE) = 0;
    
	// 排序（按自定义数据和号码统一排序）
	virtual void Sort(BOOL bAsc = TRUE) = 0;
	// 排序扩展（按自定义数据和号码分别排序）
	virtual void SortEx(BOOL bAscData = TRUE, BOOL bAscNums = TRUE) = 0;
	// 去重复（系统自动排序以后去重复）
	virtual void RemoveSameData() = 0;
	// 排序指定的号码数据
	virtual void SortItemData(DWORD dwIndex, BOOL bAsc = TRUE) = 0;
    
	// 组单互变，bRemoveSaveNums：排序后删除相同的号码，如果为FASE，则保留所有号码并且不排序
	virtual void SinToGro(BOOL bRemoveSaveNums = TRUE) = 0;
	virtual void GroToSin(BOOL bRemoveSaveNums = TRUE) = 0;
	
	// 保存数据文件
	virtual BOOL SaveData(LPCSTR lpszFilename) = 0;
	virtual BOOL LoadData(LPCSTR lpszFilename) = 0;
    
	// 导入和导出数据，用于保存条件时保存缩水器号码
	virtual BOOL ImportData(LPBYTE lpDataBuf, int nBufLen) = 0;
	virtual int ExportData(LPBYTE lpDataBuf, int nBufLen) = 0;
	
	// 填充随机号码				被填充的对象               是否需要系统产生随机种子
	virtual void FillRandomData(INumbers *pDestNumbers, BOOL bNeedSeed = TRUE) = 0;
	
	// 随机号码处理，只保留dwRandomCount个随机数据     是否需要系统产生随机种子
	virtual void TreatRandomData(DWORD dwRandomCount, BOOL bNeedSeed = TRUE) = 0;
	
	// 获取中出个数，以数字三为例，返回3D中出个数，lpMatchData：比较的数据，一般为开奖号
	// lpNDCountArray：返回的ND中出个数，依次为0D、1D和2D的中出个数，bSingle：为TRUE时是单选，否则为组选
	virtual DWORD GetMatchCount(LPBYTE lpMatchData, LPDWORD lpNDCountArray, BOOL bSingle = TRUE) = 0;
	
	// 保存撤销信息
	virtual void SaveUndo() = 0;
	// 撤销
	virtual void Undo() = 0;
	// 恢复
	virtual void Redo() = 0;
	// 是否可以撤销
	virtual BOOL CanUndo() = 0;
	// 是否可以恢复
	virtual BOOL CanRedo() = 0;
	
	// 当前数据转换为原始数据，pIAMNumbers：保存转换后的数据，如果为NULL，则直接覆盖当前数据
	virtual void TransCurToOrg(INumbers *pIOrgNumbers = NULL) = 0;
	// 原始数据转换为当前数据，pICurNumbers：保存转换后的数据，如果为NULL，则直接覆盖当前数据
	virtual void TransOrgToCur(INumbers *pICurNumbers = NULL) = 0;
	
	// 获取保存文本文件是的标志串（用于双色球等记录号码个数的信息串），lpszTextOut为接收缓冲区，注意长度不要少于128
	// 如果不需要标志串，则返回FALSE
	virtual BOOL GetTextFileFlagString(LPSTR lpszTextOut) = 0;
    
	// 复式分解成单式
	virtual void ReduceComplex() = 0;
    
	// 设置第二类型号码数据           数据信息集合             模式，见宏定义
	virtual void SetNumber2Data(CByteArray *pNumber2DataArray, BYTE btMode = N2DM_TOGETHER) = 0;
	// 获取第二类型号码数据，返回模式
	virtual BYTE GetNumber2Data(CByteArray *pNumber2DataArray) = 0;
	
	// 查询数据单元，返回索引，未找到则返回-1，bHalfFind：是否是折半查找，如果是，则数据必须是正序以后的。
	virtual int FindItem(LPBYTE lpData, BOOL bHalfFind = TRUE) = 0;
};