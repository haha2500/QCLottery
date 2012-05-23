//
//  Data.cpp
//  Graph
//
//  Created by 武 孙 on 12-5-9.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "Data.h"
#include "Def_ConditionValue.h"
#include "ICstPubFunc.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CData::CData()
{
	m_pInitDataFactory = NULL;
	m_btSectionIndex = 0;
	m_btDataType = m_btOrgDataType = m_btInitDataType = m_btNumberCount = m_btOrgNumberCount = m_btInitNumberCount = 0;
	m_btInitMaxNumber = m_btInitMinNumber = m_btOrgMaxNumber = m_btOrgMinNumber = m_btMinNumber = m_btMaxNumber = 0;
	m_btDataType = 0;
    
	ZeroMemory(m_szDataName, sizeof(m_szDataName));
    ZeroMemory(&m_stFileHead, sizeof(m_stFileHead));
	
	m_nTestNumsFlag = -1;
	m_nMachineIndex = m_nBallIndex = 0;
	m_btInitNumberCount = m_btOrgNumberCount = m_btNumberCount = 0;
	ZeroMemory(&m_stDataRange, sizeof(m_stDataRange));
	ZeroMemory(&m_btDataOrderType, sizeof(m_btDataOrderType));
	ZeroMemory(&m_stDataDiv, sizeof(m_stDataDiv));
	ZeroMemory(&m_stDataTrans, sizeof(m_stDataTrans));
    ZeroMemory(&m_stLastDataFileItem, sizeof(m_stLastDataFileItem));
	
	m_lpCTXData = m_lpLGCData = m_lpFGZData = m_lpRWLData = NULL;
	m_lpGroupSkipValueData = m_lpSingleSkipValueData = NULL;
	
	m_btIsCracked = (BYTE)(time(NULL) % 0X100);
	m_dwLastCheckTime = 0;
	m_dwSpecifiedIssue = 0;
    
    _InitLotteryProperty();
}

CData::~CData()
{
	_RemoveAll();
	
	SetForecastNums(NULL);		// 清空预测行号码    
}

////////////////////////////////////////////////////////////////////////////
BOOL CData::OpenLotteryFile()
{
    ASSERT(m_pInitDataFactory == NULL);
    
    m_pInitDataFactory = new CDataFactory;
    
    NSString *strFileNameNoPath = @"FC3D.dat";
    
    // 获取数据文件路径
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    m_strDataFileName = [documentsDirectory stringByAppendingPathComponent:strFileNameNoPath];
    
    NSData *data = [NSData dataWithContentsOfFile:m_strDataFileName];
    if (data == nil)  // 文件不存在，则认复制程序包中的数据文件
    {
        NSString *filePathInBundle = [[NSBundle mainBundle]bundlePath];
        NSString *fileNameInBundle = [filePathInBundle stringByAppendingPathComponent:strFileNameNoPath];
        [[NSFileManager defaultManager] copyItemAtPath:fileNameInBundle toPath:m_strDataFileName error:nil];
        
        // 重新打开数据文件
        data = [NSData dataWithContentsOfFile:m_strDataFileName];
        if (data == nil)  // 文件不存在，则认为没有数据
        {
            m_stFileHead.btVersion = DATAFILE_VERSION_CUR;
            m_stFileHead.dwDataItemCount = 0;

            m_pInitDataFactory->Create(0);
            return TRUE;
        }
    }
    
    LPBYTE lpDataBuf = (LPBYTE)[data bytes];
    // 获取文件头
    memcpy(&m_stFileHead, lpDataBuf, sizeof(m_stFileHead));
    // 获取初始数据
    m_pInitDataFactory->Create(m_stFileHead.dwDataItemCount);
    LPDATAFILEITEM lpDataFileItems = (LPDATAFILEITEM)&lpDataBuf[sizeof(m_stFileHead)];
    
    for (DWORD i=0; i<m_stFileHead.dwDataItemCount; i++)
    {
        m_pInitDataFactory->SetDataFileItem(i, &lpDataFileItems[i]);
    }
    
    return TRUE;
}

void CData::CloseLotteryFile(BOOL bSave)
{
    ASSERT(m_pInitDataFactory != NULL);
    
    if (bSave)  // 保存到文件
    {
        m_stFileHead.dwDataItemCount = m_pInitDataFactory->GetCount(FALSE);
        int nLen = sizeof(m_stFileHead) + m_stFileHead.dwDataItemCount * sizeof(DATAFILEITEM);
        NSMutableData *data = [NSMutableData dataWithLength:nLen];
        LPBYTE lpDataBuf = (LPBYTE)[data mutableBytes];
        memcpy(lpDataBuf, &m_stFileHead, sizeof(m_stFileHead));
        DATAFILEITEM stDataFileItem = {0};
        int nIndex = sizeof(m_stFileHead);
        for(int i=0; i<(int)m_stFileHead.dwDataItemCount; i++)
        {
            m_pInitDataFactory->GetDataFileItem(i, &stDataFileItem);
            memcpy(&lpDataBuf[nIndex], &stDataFileItem, sizeof(stDataFileItem));
            nIndex += sizeof(stDataFileItem);
        }
        [data writeToFile:m_strDataFileName atomically:YES];
    }
    
    // 设置最后一期数据
    if (m_stFileHead.dwDataItemCount > 0)
    {
        m_pInitDataFactory->GetDataFileItem(m_stFileHead.dwDataItemCount-1, &m_stLastDataFileItem);
    }
    
    // 释放初始数据
    m_pInitDataFactory->Destroy();
    delete m_pInitDataFactory;
    m_pInitDataFactory = NULL;
}

BOOL CData::AddLtyNums(DWORD dwDataTime, DWORD dwIssue, LPBYTE lpData)
{
    return m_pInitDataFactory->AddItemInfo(dwDataTime, dwIssue, lpData);
}

BOOL CData::ModifyLtyNums(DWORD dwIndex, DWORD dwDataTime, DWORD dwIssue, LPBYTE lpData)
{
     return m_pInitDataFactory->SetItemInfo(dwIndex, dwDataTime, dwIssue, lpData);
}

BOOL CData::DeleteLastLtyNums()
{
    ASSERT(m_pInitDataFactory != NULL);
    return m_pInitDataFactory->DeleteLast();
}

BOOL CData::DeleteAllLtyNums()
{
    ASSERT(m_pInitDataFactory != NULL);
    m_pInitDataFactory->DeleteAll();
    return TRUE;
}

BOOL CData::DownloadLtyNums(BOOL &bModified)
{
    // 建立指定的URL后下载数据
    NSString *strURL = [NSString stringWithFormat:@"http://software.pinble.com/cstdata2010/debug/cstdata_sz.asp?ver=2&lastdate=0&lasttime=0&lotteryid=11000130&lastissue=%d&rn=432381-284322-417377-210241&hdsn=3365103343205087&st=5&it=7&pv=7.2.0&tf=0&globalid=iwd-wxd1a41y3870", m_stLastDataFileItem.dwIssue];
    NSURL *url = [NSURL URLWithString:strURL];
    
    NSData *data = [NSData dataWithContentsOfURL:url];
    if (data == nil)
    {
        return FALSE;       // 下载失败，网络不通？
    }
    int nBufLen = [data length];
    if(nBufLen < 2)
	{
        return FALSE;       // 下载失败，数据长度不够
	}

    LPCSTR lpBuf = (LPCSTR)[data bytes];
    const int nItemLen = 18;	// 每个数据单元长度
    
	if(((nBufLen - 2) % nItemLen) != 0)	// 每个号码信息的长度为nItemLen
	{
        return FALSE;
	}
	int nCount = (nBufLen - 2) / nItemLen - 1;		// 减去第一个特殊数据
    
    // 打开数据文件以便更新
    OpenLotteryFile();
    
	// 添加开奖信息
	int nBufIndex = 2 + 18;		// 跳过结果和第一个特殊数据
	DWORD dwIssue = 0, dwDateTime = 0;
    BYTE btData[MAX_NUMBER_COUNT] = {0};
    
	for(int i=0; i<nCount; i++)
	{
		dwDateTime = _GetValueFromString(&lpBuf[nBufIndex]);
		nBufIndex += 5;
		dwIssue = _GetValueFromIssueString(&lpBuf[nBufIndex]);
		nBufIndex += 5;
		_GetDataFromString(&lpBuf[nBufIndex], 0xFF, btData);
        
        if(!m_pInitDataFactory->AddItemInfo(dwDateTime, dwIssue, btData))
        {
            return FALSE;
        }
        bModified = TRUE;
		nBufIndex += 8;
    }

    CloseLotteryFile(bModified);
    return TRUE;
}

//////////////////////////////////////////////////////////////////////////////
DWORD CData::GetLotteryID()
{    
	return m_stLotteryProperty.dwLotteryID;
}

LPLOTTERYPROPERTY CData::GetLotteryProperty()
{
	return &m_stLotteryProperty;
}

BYTE CData::GetIssueType()
{
	return m_stLotteryProperty.btIssueType;
}

BYTE CData::GetIssueLen(BOOL bOnlyLastXXX)
{
	if(bOnlyLastXXX)
	{
		switch(m_stLotteryProperty.btIssueType)
		{
            case ISSUE_TYPE_YYYYMMDDXX:
            case ISSUE_TYPE_YYMMDDXX: return 2; break;
            case ISSUE_TYPE_YYYYXXX:
            case ISSUE_TYPE_YYMMDDXXX:
            case ISSUE_TYPE_YYYYMMDDXXX: return 3; break;
            case ISSUE_TYPE_YYXXXXXX: return 6; break;
            default: ASSERT(FALSE); break;
		}
	}
	else
	{
		switch(m_stLotteryProperty.btIssueType)
		{
            case ISSUE_TYPE_YYYYXXX: return 7; break;
            case ISSUE_TYPE_YYYYMMDDXX: return 10; break;
            case ISSUE_TYPE_YYXXXXXX: return 8; break;
            case ISSUE_TYPE_YYMMDDXX: return 8; break;
            case ISSUE_TYPE_YYMMDDXXX: return 9; break;
            case ISSUE_TYPE_YYYYMMDDXXX: return 11; break;
            default: ASSERT(FALSE); break;
		}
	}
	
	return 0;	// error
}

BYTE CData::GetDataType(BYTE btDataSource)
{
	switch(btDataSource)
	{
        case DATA_SOURCE_CUR:		// 当前数据源
            return m_btDataType;
            break;
            
        case DATA_SOURCE_ORG:		// 原始数据源
            return m_btOrgDataType;
            break;
            
        case DATA_SOURCE_INIT:		// 初始数据源
            return m_btInitDataType;
            break;
            
        default: ASSERT(FALSE); break;
	}
	
	return 0;
}

DWORD CData::GetItemCount(BYTE btDataSource)
{
	DWORD dwRtn = 0;
	switch(btDataSource) 
	{
        case DATA_SOURCE_ORG:		// 原始数据源
        case DATA_SOURCE_INIT:		// 初始数据源
            dwRtn = m_pInitDataFactory->GetCount(FALSE);
            break;
        case DATA_SOURCE_CUR:		// 当前数据源
            dwRtn = m_cCurDataFactory.GetCount(TRUE);
            break;
        default:
            ASSERT(FALSE);
            break;
	}
	
#ifndef _FREE
    // TEST
/*	DWORD dwTemp = time(NULL);
	if(dwTemp > m_dwLastCheckTime + 5 * 1000)
	{
        //		DEBUGOUT("need check dwTemp=%d, m_dwLastCheckTime=%d\n", dwTemp, m_dwLastCheckTime);
		m_dwLastCheckTime = dwTemp;
		if(dwTemp > 30 * 60 * 1000)
		{
			// 软件狗方式
			dwTemp -= 30 * 60 * 1000;
			DWORD dwTemp2 = g_pIConfig->GetConfig(CFGID_SYSTEM_REGISTER_DETECTTIME, NULL, 0);
			if((dwTemp2 < dwTemp))
			{
				// 如果30分钟还没有检查注册信息，则表示被破解
				dwRtn = ::GetTickCount();
				m_pInitDataFactory->Destroy();
				for(int i=0; i<20; i++)
				{
					m_cCurDataFactory.SetItemInfo(::GetTickCount() % m_cCurDataFactory.GetCount(TRUE), 0, 0, NULL);
					Sleep(10);
				}
			}
		}
	}*/
#endif
	
	return dwRtn;
}

DWORD CData::GetItemDate(DWORD dwDataIndex, BYTE btDataSource)
{
	if(btDataSource == DATA_SOURCE_CUR)	// 当前数据源
	{
		// 需要转换到原始数据的索引
		dwDataIndex = GetInitItemIndex(dwDataIndex);
	}
    
	return m_pInitDataFactory->GetItemDate(dwDataIndex);
}

LPCSTR CData::GetItemDateString(DWORD dwDataIndex, BYTE btDataSource)
{
	if(btDataSource == DATA_SOURCE_CUR)	// 当前数据源
	{
		// 需要转换到原始数据的索引
		dwDataIndex = GetInitItemIndex(dwDataIndex);
	}
	
	return m_pInitDataFactory->GetItemDateString(dwDataIndex);
}

DWORD CData::GetItemTime(DWORD dwDataIndex, BYTE btDataSource)
{
	if(btDataSource == DATA_SOURCE_CUR)	// 当前数据源
	{
		// 需要转换到原始数据的索引
		dwDataIndex = GetInitItemIndex(dwDataIndex);
	}
	
	return m_pInitDataFactory->GetItemTime(dwDataIndex);
}

LPCSTR CData::GetItemTimeString(DWORD dwDataIndex, BYTE btDataSource)
{
	if(btDataSource == DATA_SOURCE_CUR)	// 当前数据源
	{
		// 需要转换到原始数据的索引
		dwDataIndex = GetInitItemIndex(dwDataIndex);
	}
	
	return m_pInitDataFactory->GetItemTimeString(dwDataIndex);
}

int CData::GetItemDayOfWeek(DWORD dwDataIndex, BYTE btDataSource)
{
	if(btDataSource == DATA_SOURCE_CUR)	// 当前数据源
	{
		// 需要转换到原始数据的索引
		dwDataIndex = GetInitItemIndex(dwDataIndex);
	}
	
	return m_pInitDataFactory->GetItemDayOfWeek(dwDataIndex);
}

LPCSTR CData::GetItemDayOfWeekString(DWORD dwDataIndex, BYTE btDataSource)
{
	if(btDataSource == DATA_SOURCE_CUR)	// 当前数据源
	{
		// 需要转换到原始数据的索引
		dwDataIndex = GetInitItemIndex(dwDataIndex);
	}
	
	return m_pInitDataFactory->GetItemDayOfWeekString(dwDataIndex);
}

void CData::GetItemDateTime(DWORD dwDataIndex, CTime &tmDateTime, BYTE btDataSource)
{
	if(btDataSource == DATA_SOURCE_CUR)	// 当前数据源
	{
		// 需要转换到原始数据的索引
		dwDataIndex = GetInitItemIndex(dwDataIndex);
	}
	
	m_pInitDataFactory->GetItemDateTime(dwDataIndex, tmDateTime);
}

LPCSTR CData::GetItemDateTimeString(DWORD dwDataIndex, BYTE btDataSource)
{
	if(btDataSource == DATA_SOURCE_CUR)	// 当前数据源
	{
		// 需要转换到原始数据的索引
		dwDataIndex = GetInitItemIndex(dwDataIndex);
	}
	
	if(m_stLotteryProperty.btOpenTimesPerDay > 1)
		return m_pInitDataFactory->GetItemDateTimeString(dwDataIndex);		// 高频
	else
		return m_pInitDataFactory->GetItemDateString(dwDataIndex);			// 非高频
}

DWORD CData::GetItemIssue(DWORD dwDataIndex, BYTE btDataSource)
{
	if(btDataSource == DATA_SOURCE_CUR)
	{
		dwDataIndex = GetInitItemIndex(dwDataIndex);
	}
    
	return m_pInitDataFactory->GetItemIssue(dwDataIndex);
}

LPCSTR CData::GetItemIssueString(DWORD dwDataIndex, BYTE btDataSource)
{
	if(btDataSource == DATA_SOURCE_CUR)	// 当前数据源
	{
		// 需要转换到原始数据的索引
		dwDataIndex = GetInitItemIndex(dwDataIndex);
	}
	
	return m_pInitDataFactory->GetItemIssueString(dwDataIndex);
}

LPBYTE CData::GetItemNums(DWORD dwDataIndex, BYTE btDataSource)
{
	ASSERT(btDataSource != DATA_SOURCE_ORG);	// 目前不支持原始数据
	
	if(btDataSource == DATA_SOURCE_CUR)
		return m_cCurDataFactory.GetItemNums(dwDataIndex);
	else
		return m_pInitDataFactory->GetItemNums(dwDataIndex);
}

LPCSTR CData::GetItemNumsString(DWORD dwDataIndex, BYTE btDataSource)
{
	ASSERT(btDataSource != DATA_SOURCE_ORG);	// 目前不支持原始数据
	
	if(btDataSource == DATA_SOURCE_CUR)
		return m_cCurDataFactory.GetItemNumsString(dwDataIndex);
	else
		return m_pInitDataFactory->GetItemNumsString(dwDataIndex);
}

DWORD CData::GetItemNumsValue(DWORD dwDataIndex, BYTE btDataSource)
{
	ASSERT(btDataSource != DATA_SOURCE_ORG);	// 目前不支持原始数据
	
	if(btDataSource == DATA_SOURCE_CUR)
		return m_cCurDataFactory.GetItemNumsValue(dwDataIndex);
	else
		return m_pInitDataFactory->GetItemNumsValue(dwDataIndex);
}

LPBYTE CData::GetItemNums2(DWORD dwDataIndex, BYTE btDataSource)
{
	if(btDataSource == DATA_SOURCE_CUR)	// 当前数据源
	{
		// 需要转换到原始数据的索引
		dwDataIndex = GetInitItemIndex(dwDataIndex);
	}
	
	return m_pInitDataFactory->GetItem2Nums(dwDataIndex);
}

LPCSTR CData::GetItemNums2String(DWORD dwDataIndex, BYTE btDataSource)
{
	if(btDataSource == DATA_SOURCE_CUR)	// 当前数据源
	{
		// 需要转换到原始数据的索引
		dwDataIndex = GetInitItemIndex(dwDataIndex);
	}
	
	return m_pInitDataFactory->GetItemNums2String(dwDataIndex);
}

DWORD CData::GetItemNums2Value(DWORD dwDataIndex, BYTE btDataSource)
{
	if(btDataSource == DATA_SOURCE_CUR)	// 当前数据源
	{
		// 需要转换到原始数据的索引
		dwDataIndex = GetInitItemIndex(dwDataIndex);
	}
	
	return m_pInitDataFactory->GetItemNums2Value(dwDataIndex);
}

LPBYTE CData::GetItemTestNums(DWORD dwDataIndex, BYTE btDataSource)
{
	ASSERT(btDataSource != DATA_SOURCE_ORG);	// 目前不支持原始数据
	
	if(!(m_stLotteryProperty.btProperty & CSTLPP_TESTNUMS))	// 不含试机号的彩票
	{
		return NULL;
	}
	
	if(btDataSource == DATA_SOURCE_CUR)
		return m_cCurDataFactory.GetItemTestNums(dwDataIndex);
	else
		return m_pInitDataFactory->GetItemTestNums(dwDataIndex);
}

LPCSTR CData::GetItemTestNumsString(DWORD dwDataIndex, BYTE btDataSource)
{
	ASSERT(btDataSource != DATA_SOURCE_ORG);	// 目前不支持原始数据
	
	if(!(m_stLotteryProperty.btProperty & CSTLPP_TESTNUMS))	// 不含试机号的彩票
	{
		return "";
	}
	
	if(btDataSource == DATA_SOURCE_CUR)
		return m_cCurDataFactory.GetItemTestNumsString(dwDataIndex);
	else
		return m_pInitDataFactory->GetItemTestNumsString(dwDataIndex);
}

DWORD CData::GetItemTestNumsValue(DWORD dwDataIndex, BYTE btDataSource)
{
	ASSERT(btDataSource != DATA_SOURCE_ORG);	// 目前不支持原始数据
	
	if(!(m_stLotteryProperty.btProperty & CSTLPP_TESTNUMS))	// 不含试机号的彩票
	{
		return CDTVALUE_INVALID;
	}
    
	if(btDataSource == DATA_SOURCE_CUR)
		return m_cCurDataFactory.GetItemTestNumsValue(dwDataIndex);
	else
		return m_pInitDataFactory->GetItemTestNumsValue(dwDataIndex);
}

BOOL CData::IsTestNumsItem(DWORD dwDataIndex, BYTE btDataSource)
{
	if(!(m_stLotteryProperty.btProperty & CSTLPP_TESTNUMS))	// 不含试机号的彩票
	{
		return FALSE;
	}
	
	if(btDataSource == DATA_SOURCE_CUR)
		return m_cCurDataFactory.IsTestNumsItem(dwDataIndex);
	else
		return FALSE;	// 原始数据没有试机号单元
}

BYTE CData::GetItemMachineIndex(DWORD dwDataIndex, BYTE btDataSource)
{
	ASSERT(btDataSource != DATA_SOURCE_ORG);	// 目前不支持原始数据
	
	if(btDataSource == DATA_SOURCE_CUR)
		return m_cCurDataFactory.GetItemMachineIndex(dwDataIndex);
	else
		return m_pInitDataFactory->GetItemMachineIndex(dwDataIndex);
}

BYTE CData::GetItemBallIndex(DWORD dwDataIndex, BYTE btDataSource)
{
	ASSERT(btDataSource != DATA_SOURCE_ORG);	// 目前不支持原始数据
    
	if(btDataSource == DATA_SOURCE_CUR)
		return m_cCurDataFactory.GetItemBallIndex(dwDataIndex);
	else
		return m_pInitDataFactory->GetItemBallIndex(dwDataIndex);
}

DWORD CData::GetInitItemIndex(DWORD dwCurDataIndex)
{
	return m_cCurDataFactory.GetItemInitIndex(dwCurDataIndex);
}

void CData::SetForecastNums(LPBYTE lpData)
{
	m_cCurDataFactory.SetForecastNums(lpData);
}

LPBYTE CData::GetForecastNums()
{
	return m_cCurDataFactory.GetForecastNums();
}

void CData::SetSectionIndex(BYTE btSectionIndex)
{
	ASSERT(btSectionIndex < m_stLotteryProperty.btSectionCount);
	
	m_btSectionIndex = btSectionIndex;
	
	// 初始化初始数据
	m_btInitDataType = m_stLotteryProperty.stSectionInfo[btSectionIndex].btDataType;
	m_btInitNumberCount = m_stLotteryProperty.stSectionInfo[btSectionIndex].btNumberCount;
	m_btInitMinNumber = m_stLotteryProperty.stSectionInfo[btSectionIndex].btMinNumber;
	m_btInitMaxNumber = m_stLotteryProperty.stSectionInfo[btSectionIndex].btMaxNumber;
}

void CData::SetPlayType(LPPLAYTYPEITEM lpPTItem)
{
	ASSERT(lpPTItem != NULL);
    
	memcpy(&m_stPTItem, lpPTItem, sizeof(PLAYTYPEITEM));
	
	// 初始化原始数据
	m_btOrgDataType = m_btInitDataType;
	m_btOrgNumberCount = lpPTItem->btCount;
	m_btOrgMaxNumber = m_btInitMaxNumber;
	m_btOrgMinNumber = m_btInitMinNumber;
	m_btDataOrderType = DATA_ORDERTYPE_NONE;
    
	if(m_btSectionIndex == 0 && m_stLotteryProperty.stSectionInfo[1].btFlag & CSTLTF_JOIN)	// 特码作为第一区段的，需要将特码加入
	{
		m_btOrgNumberCount ++;
	}
    
	// 根据玩法标志设置相关原始数据值
	_TreatPosItem(&m_stPTItem, m_btOrgDataType, m_btOrgMinNumber, m_btOrgMaxNumber);
    
	// 设置原始分析方式
	ANALYSISMODEITEM stAMItem = {LPIF_ORG, LPIF_ORG, m_btOrgNumberCount};
	for(BYTE i=0; i<m_btOrgNumberCount; i++)
		stAMItem.btPosArray[i] = i;
	strcpy(stAMItem.szName, "原始数据");
	SetAnalysisMode(&stAMItem);
}

void CData::SetAnalysisMode(LPANALYSISMODEITEM lpAMItem)
{
	ASSERT(lpAMItem != NULL);
    
	memcpy(&m_stAMItem, lpAMItem, sizeof(ANALYSISMODEITEM));
	
	// 初始化当前数据
	m_btAMDataType = m_btOrgDataType;
	m_btAMNumberCount = lpAMItem->btCount;
	m_btAMMaxNumber = m_btOrgMaxNumber;
	m_btAMMinNumber = m_btOrgMinNumber;
	
	// 根据分析方式标志设置相关当前数据值
	_TreatPosItem(&m_stAMItem, m_btAMDataType, m_btAMMinNumber, m_btAMMaxNumber);
    
	// 设置原始数据变换
	DATATRANSSET stDTItem = {0};
	stDTItem.btType = DATATRANS_TYPE_ORG;
	stDTItem.wID = DATATRANS_ID_ORG;
	stDTItem.btItemCount = m_btAMNumberCount;
	strcpy(stDTItem.szName, "原始数据");
    /*	for(int i=0; i<stDTItem.btItemCount; i++) 
     {
     stDTItem.cdtIDs[i].InitSystemID(IID_STC_POS_1 + i);
     strcpy(stDTItem.szItemNames[i], g_pIConditionMan->GetConditionName(stDTItem.cdtIDs[i]));
     }
     */
	SetDataTrans(&stDTItem);
}

void CData::SetDataTrans(LPVOID lpDataTrans)
{
	ASSERT(lpDataTrans != NULL);
    
	memcpy(&m_stDataTrans, lpDataTrans, sizeof(m_stDataTrans));
    
	m_btDataType = m_btAMDataType;
	m_btDataOrderType = DATA_ORDERTYPE_NONE;
    
	if(m_stDataTrans.btType != DATATRANS_TYPE_ORG)
	{
		m_btDataType &= ~DATA_TYPE_ORDERED;
	}
	
	m_btNumberCount = m_stDataTrans.btItemCount;
	m_btMaxNumber = m_btAMMaxNumber;
	m_btMinNumber = m_btAMMinNumber;
}

void CData::SetDataOrderType(BYTE btDataOrderType)
{
    // TEST
/*	if(!g_pICstMain->IsSupportFunc(FUNCID_SET_DATAORDER))
	{
		m_btDataType |= DATA_TYPE_ORDERED;		// 不支持数据排列方式，则一定是已排序的
		return ;
	}
  */  
	m_btDataOrderType = btDataOrderType;
    
	if(m_btDataOrderType == DATA_ORDERTYPE_ASC)
	{
		m_btDataType |= DATA_TYPE_ORDERED;
	}
	else
	{
		m_btDataType &= ~DATA_TYPE_ORDERED;
	}
}

BYTE CData::GetCurrentSectionIndex()
{
	return m_btSectionIndex;
}

BOOL CData::IsOrgData(BOOL bIgnorePlayType)
{
	// 检查排列方式
	if(m_btDataOrderType != DATA_ORDERTYPE_NONE)
	{
		return FALSE;	// 非默认排列方式
	}
    
	// 检查玩法
	if(!bIgnorePlayType)
	{
		if(!IsOrgPlayType())
		{
			return FALSE;
		}
	}
    
	// 检查分析方式
	if(!IsOrgAnalysisMode())
	{
		return FALSE;
	}
    
	// 检查数据变换
	if(m_stDataTrans.btType != DATATRANS_TYPE_ORG)
	{
		return FALSE;
	}
    
	return TRUE;
}

LPPLAYTYPEITEM CData::GetCurrentPlayType(LPDWORD lpPTValueOut, LPSTR lpszPTNameOut)
{
	if(lpPTValueOut != NULL)
	{
		*lpPTValueOut = _GetValueFromPosItem(&m_stPTItem);
	}
	
	if(lpszPTNameOut != NULL)
	{
		strcpy(lpszPTNameOut, m_stPTItem.szName);
	}
	
	return &m_stPTItem;
}

BOOL CData::IsOrgPlayType(LPPLAYTYPEITEM lpPTItem)
{
	if(lpPTItem == NULL)
	{
		// 查询当前玩法
		lpPTItem = &m_stPTItem;
	}
    
	return (lpPTItem->btFlagPrev == LPIF_ORG) ? TRUE : FALSE;
}

LPANALYSISMODEITEM CData::GetCurrentAnalysisMode(LPDWORD lpAMValueOut, LPSTR lpszAMNameOut)
{
	if(lpAMValueOut != NULL)
	{
		*lpAMValueOut = _GetValueFromPosItem(&m_stAMItem);
	}
	
	if(lpszAMNameOut != NULL)
	{
		strcpy(lpszAMNameOut, m_stAMItem.szName);
	}
	
	return &m_stAMItem;
}

BOOL CData::IsOrgAnalysisMode(LPANALYSISMODEITEM lpAMItem)
{
	if(lpAMItem == NULL)
	{
		// 查询当前分析方式
		lpAMItem = &m_stAMItem;
	}
	
	return (lpAMItem->btFlagPrev == LPIF_ORG) ? TRUE : FALSE;
}

LPVOID CData::GetCurrentDataTrans()
{
	return &m_stDataTrans;
}

BOOL CData::IsOrgDataTrans(LPVOID lpDTItem)
{
	LPDATATRANSSET lpDataTrans = (lpDTItem == NULL) ? &m_stDataTrans : (LPDATATRANSSET)lpDTItem;
    
	return (lpDataTrans->btType == DATATRANS_TYPE_ORG) ? TRUE : FALSE;
}

BYTE CData::GetCurrentDataOrderType()
{
	return m_btDataOrderType;
}

LPCSTR CData::GetCurrentDataName(BYTE btFlag)
{
	switch(btFlag) 
	{
        case GCDNF_NAME_LOTTERY:		// 彩票名称
            return m_stLotteryProperty.szLotteryName;
            break;
            
        case GCDNF_NAME_SECTION:		// 彩票区段名称
            return m_stLotteryProperty.stSectionInfo[m_btSectionIndex].szName;
            break;
            
        case GCDNF_NAME_PLAYTYPE:		// 玩法名称
            GetCurrentPlayType(NULL, m_szDataName);
            break;
            
        case GCDNF_NAME_ANALYSISMODE:	// 分析方式名称
            GetCurrentAnalysisMode(NULL, m_szDataName);
            break;
            
        case GCDNF_NAME_DATATRANS:		// 数据变换名称
            return m_stDataTrans.szName;
            break;
            
        case GCDNF_NAME_LOTTERYANDSECTION:	// 彩票名称和区段名称
		{
            /*			if(m_stLotteryProperty.btSectionCount > 1 && m_stLotteryProperty.stSectionInfo[1].btFlag & CSTLTF_NEED)
             {
             // 如果区段个数大于1，并且第二区段是开奖号的必要组成部分
             sprintf(m_szDataName, "%s%s", m_stLotteryProperty.szLotteryName, m_stLotteryProperty.stSectionInfo[m_btSectionIndex].szName);
             }
             else	// 忽略区段名
             {
             return m_stLotteryProperty.szLotteryName;
             }*/
			if(m_stLotteryProperty.btSectionCount > 1)
			{
				// 如果区段个数大于1
				sprintf(m_szDataName, "%s%s", m_stLotteryProperty.szLotteryName, m_stLotteryProperty.stSectionInfo[m_btSectionIndex].szName);
			}
			else	// 忽略区段名
			{
				return m_stLotteryProperty.szLotteryName;
			}
		}
            break;
        case GCDNF_NAME_WHOLE:				// 数据全称
		{
			strcpy(m_szDataName, GetCurrentDataName(GCDNF_NAME_LOTTERYANDSECTION));
            
			if(m_stPTItem.btFlagPrev != LPIF_ORG)	// 非原始玩法
				strcat(m_szDataName, m_stPTItem.szName);
            
			if(m_stAMItem.btFlagPrev != LPIF_ORG)	// 非原始分析方式
				strcat(m_szDataName, m_stAMItem.szName);
            
			if(m_stDataTrans.btType != DATATRANS_TYPE_ORG)	// 非原始数据变换
				strcat(m_szDataName, m_stDataTrans.szName);
		}
            break;
        default:
            ASSERT(FALSE);
            break;
	}
	
	CHECK_STRING_STACK(m_szDataName);
	
	return m_szDataName;
}

#define		BEGIN_ADDDATADIFFVALUE()  int nIndex = 0;
#define		ADDDATADIFFVALUE_BYTE(dwDataDiffMaskIn, btValueIn) \
                if(dwDataDiffMask & (dwDataDiffMaskIn)) { \
                    lpValueOut->btData[nIndex] = (btValueIn); \
                    nIndex += sizeof(BYTE); \
                    ASSERT(nIndex < sizeof(lpValueOut->btData)); \
                }
#define		ADDDATADIFFVALUE_DWORD(dwDataDiffMaskIn, dwValueIn) \
                if(dwDataDiffMask & (dwDataDiffMaskIn)) { \
                    memcpy(&lpValueOut->btData[nIndex], &(dwValueIn), sizeof(DWORD)); \
                    nIndex += sizeof(DWORD); \
                    ASSERT(nIndex < sizeof(lpValueOut->btData)); \
                }
#define		ADDDATADIFFVALUE_WORD(dwDataDiffMaskIn, wValueIn) \
                if(wValueIn != 0 && (dwDataDiffMask & (dwDataDiffMaskIn))) { \
                    memcpy(&lpValueOut->btData[nIndex], &(wValueIn), sizeof(WORD)); \
                    nIndex += sizeof(WORD); \
                    ASSERT(nIndex <= sizeof(lpValueOut->btData)); \
                }
#define		END_ADDDATADIFFVALUE()

void CData::GetCurrentDataDiffValue(LPDATADIFFVALUE lpValueOut, DWORD dwDataDiffMask)
{
	ASSERT(lpValueOut != NULL);
    
	// 初始化所有值
	ZeroMemory(lpValueOut, sizeof(DATADIFFVALUE));
    
	if(dwDataDiffMask == DDLM_NONE)
	{
		return ;	// 无区分
	}
    
	// 获取玩法和分析方式标志数据
	DWORD dwPTFlag = 0, dwAMFlag = 0;
	if(dwDataDiffMask & DDLM_PLAYTYPE)
		GetCurrentPlayType(&dwPTFlag);
	if(dwDataDiffMask & DDLM_ANALYSISMODE)
		GetCurrentAnalysisMode(&dwAMFlag);
    
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	BEGIN_ADDDATADIFFVALUE()
    ADDDATADIFFVALUE_DWORD(DDLM_LOTTERYID, m_stLotteryProperty.dwLotteryID);			// 彩票ID号
    
    ADDDATADIFFVALUE_BYTE(DDLM_LOTTERYPROPERTY, m_stLotteryProperty.btProperty);		// 彩票属性
    ADDDATADIFFVALUE_BYTE(DDLM_ISHIGHFREQ, BYTE(m_stLotteryProperty.btOpenTimesPerDay > 1 ? 1 : 0));	// 是否高频彩
    ADDDATADIFFVALUE_BYTE(DDLM_SECTIONCOUNT, BYTE(IsOrgData() ? m_stLotteryProperty.btSectionCount : 1));		// 彩票区段总数
    ADDDATADIFFVALUE_BYTE(DDLM_ISORGDATA, BYTE(IsOrgData() ? 1 : 0));					// 是否原始数据
    
    ADDDATADIFFVALUE_BYTE(DDLM_SECTION1_DATATYPE, m_stLotteryProperty.stSectionInfo[0].btDataType);		// 彩票第一区段数据类型
    ADDDATADIFFVALUE_BYTE(DDLM_SECTION1_NUMBERCOUNT, m_stLotteryProperty.stSectionInfo[0].btNumberCount);	// 彩票第一区段号码个数
    ADDDATADIFFVALUE_BYTE(DDLM_SECTION1_MINNUMBER, m_stLotteryProperty.stSectionInfo[0].btMinNumber);		// 彩票第一区段最小号码
    ADDDATADIFFVALUE_BYTE(DDLM_SECTION1_MAXNUMBER, m_stLotteryProperty.stSectionInfo[0].btMaxNumber);		// 彩票第一区段最大号码
    
    ADDDATADIFFVALUE_BYTE(DDLM_SECTION2_DATATYPE, m_stLotteryProperty.stSectionInfo[1].btDataType);		// 彩票第二区段数据类型
    ADDDATADIFFVALUE_BYTE(DDLM_SECTION2_NUMBERCOUNT, m_stLotteryProperty.stSectionInfo[1].btNumberCount);	// 彩票第二区段号码个数
    ADDDATADIFFVALUE_BYTE(DDLM_SECTION2_MINNUMBER, m_stLotteryProperty.stSectionInfo[1].btMinNumber);		// 彩票第二区段最小号码
    ADDDATADIFFVALUE_BYTE(DDLM_SECTION2_MAXNUMBER, m_stLotteryProperty.stSectionInfo[1].btMaxNumber);		// 彩票第二区段最大号码
    
    ADDDATADIFFVALUE_BYTE(DDLM_SECTIONINDEX, m_btSectionIndex);								// 当前彩票区段索引
    
    ADDDATADIFFVALUE_DWORD(DDLM_PLAYTYPE, dwPTFlag);										// 玩法设置
    ADDDATADIFFVALUE_BYTE(DDLM_ISORGPT, (BYTE)(g_pIData->IsOrgPlayType() ? 1 : 0));			// 是否原始玩法
    ADDDATADIFFVALUE_BYTE(DDLM_ORG_DATATYPE, m_btOrgDataType);								// 原始数据类型
    ADDDATADIFFVALUE_BYTE(DDLM_ORG_NUMBERCOUNT, m_btOrgNumberCount);						// 原始号码个数
    ADDDATADIFFVALUE_BYTE(DDLM_ORG_MINNUMBER, m_btOrgMinNumber);							// 原始数据最小号码
    ADDDATADIFFVALUE_BYTE(DDLM_ORG_MAXNUMBER, m_btOrgMaxNumber);							// 原始数据最大号码
    
    ADDDATADIFFVALUE_DWORD(DDLM_ANALYSISMODE, dwAMFlag);									// 分析方式
    ADDDATADIFFVALUE_BYTE(DDLM_ISORGAM, (BYTE)(g_pIData->IsOrgAnalysisMode() ? 1 : 0));		// 是否原始分析方式
    ADDDATADIFFVALUE_BYTE(DDLM_CAM_DATATYPE, m_btAMDataType);								// 当前数据类型
    ADDDATADIFFVALUE_BYTE(DDLM_CAM_NUMBERCOUNT, m_btAMNumberCount);							// 当前号码个数
    ADDDATADIFFVALUE_BYTE(DDLM_CAM_MINNUMBER, m_btAMMinNumber);								// 当前数据最小号码
    ADDDATADIFFVALUE_BYTE(DDLM_CAM_MAXNUMBER, m_btAMMaxNumber);								// 当前数据最大号码
    
    ADDDATADIFFVALUE_WORD(DDLM_DATATRANS, m_stDataTrans.wID);								// 数据变换
    ADDDATADIFFVALUE_BYTE(DDLM_DATATRANS, m_stDataTrans.btItemCount);						// 数据变换
    ADDDATADIFFVALUE_BYTE(DDLM_DATATRANS, m_btMinNumber);									// 数据变换
    ADDDATADIFFVALUE_BYTE(DDLM_DATATRANS, m_btMaxNumber);									// 数据变换
    
    ADDDATADIFFVALUE_BYTE(DDLM_CUR_DATATYPE, m_btDataType);									// 当前数据类型
    ADDDATADIFFVALUE_BYTE(DDLM_CUR_NUMBERCOUNT, m_btNumberCount);							// 当前号码个数
    ADDDATADIFFVALUE_BYTE(DDLM_CUR_MINNUMBER, m_btMinNumber);								// 当前数据最小号码
    ADDDATADIFFVALUE_BYTE(DDLM_CUR_MAXNUMBER, m_btMaxNumber);								// 当前数据最大号码
    
    ADDDATADIFFVALUE_BYTE(DDLM_CUR_ORDERTYPE, m_btDataOrderType);							// 当前数据排列方式
	END_ADDDATADIFFVALUE()
}

LPCSTR CData::GetCurrentDataDiffName(DWORD dwDataDiffMask)
{
	m_szDataName[0] = 0;
    
	if(dwDataDiffMask == DDLM_NONE)
	{
		return m_szDataName;	// 无区分
	}
    
	char szTemp[16] = {0};
    
	if(dwDataDiffMask & DDLM_LOTTERYID)									// 彩票ID号
	{
		sprintf(m_szDataName, "ID%08lX_", m_stLotteryProperty.dwLotteryID);
	}
	if(dwDataDiffMask & DDLM_LOTTERYPROPERTY)							// 彩票属性
	{
		sprintf(szTemp, "LP%02X_", m_stLotteryProperty.btProperty);
		strcat(m_szDataName, szTemp);
	}
	if(dwDataDiffMask & DDLM_ISHIGHFREQ)								// 是否高频彩
	{
		if(m_stLotteryProperty.btOpenTimesPerDay > 1)
			strcat(m_szDataName, "GPY_");
		else
			strcat(m_szDataName, "GPN_");
	}
	if(dwDataDiffMask & DDLM_SECTIONCOUNT)								// 彩票区段总数
	{
		sprintf(szTemp, "SC%1d_", IsOrgData() ? m_stLotteryProperty.btSectionCount : 1);
		strcat(m_szDataName, szTemp);
	}
	if(dwDataDiffMask & DDLM_ISORGDATA)									// 是否原始数据
	{
		if(IsOrgData())
			strcat(m_szDataName, "ODY_");
		else
			strcat(m_szDataName, "ODN_");
	}
    
	if(dwDataDiffMask & DDLM_SECTION1_DATATYPE)			// 彩票第一区段数据类型
	{
		sprintf(szTemp, "S1DT%02X_", m_stLotteryProperty.stSectionInfo[0].btDataType);
		strcat(m_szDataName, szTemp);
	}
	if(dwDataDiffMask & DDLM_SECTION1_NUMBERCOUNT)		// 彩票第一区段号码个数
	{
		sprintf(szTemp, "S1NC%1X_", m_stLotteryProperty.stSectionInfo[0].btNumberCount);
		strcat(m_szDataName, szTemp);
	}
	if(dwDataDiffMask & DDLM_SECTION1_MINNUMBER)		// 彩票第一区段最小号码
	{
		sprintf(szTemp, "S1IN%02X_", m_stLotteryProperty.stSectionInfo[0].btMinNumber);
		strcat(m_szDataName, szTemp);
	}
	if(dwDataDiffMask & DDLM_SECTION1_MAXNUMBER)		// 彩票第一区段最大号码
	{
		sprintf(szTemp, "S1XN%02X_", m_stLotteryProperty.stSectionInfo[0].btMaxNumber);
		strcat(m_szDataName, szTemp);
	}
	if(dwDataDiffMask & DDLM_SECTION2_DATATYPE)			// 彩票第二区段数据类型
	{
		sprintf(szTemp, "S2DT%02X_", m_stLotteryProperty.stSectionInfo[1].btDataType);
		strcat(m_szDataName, szTemp);
	}
	if(dwDataDiffMask & DDLM_SECTION2_NUMBERCOUNT)		// 彩票第二区段号码个数
	{
		sprintf(szTemp, "S2NC%1X_", m_stLotteryProperty.stSectionInfo[1].btNumberCount);
		strcat(m_szDataName, szTemp);
	}
	if(dwDataDiffMask & DDLM_SECTION2_MINNUMBER)		// 彩票第二区段最小号码
	{
		sprintf(szTemp, "S2IN%02X_", m_stLotteryProperty.stSectionInfo[1].btMinNumber);
		strcat(m_szDataName, szTemp);
	}
	if(dwDataDiffMask & DDLM_SECTION2_MAXNUMBER)		// 彩票第二区段最大号码
	{
		sprintf(szTemp, "S2XN%02X_", m_stLotteryProperty.stSectionInfo[1].btMaxNumber);
		strcat(m_szDataName, szTemp);
	}
    
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(dwDataDiffMask & DDLM_SECTIONINDEX)					// 彩票区段索引
	{
		sprintf(szTemp, "SI%1X_", m_btSectionIndex);
		strcat(m_szDataName, szTemp);
	}
    
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(dwDataDiffMask & DDLM_PLAYTYPE)						// 玩法设置
	{
		DWORD dwPTFlag = 0;
		GetCurrentPlayType(&dwPTFlag);
		sprintf(szTemp, "PT%08lX_", dwPTFlag);
		strcat(m_szDataName, szTemp);
	}
	if(dwDataDiffMask & DDLM_ISORGPT)						// 是否原始玩法
	{
		if(g_pIData->IsOrgPlayType())
			strcat(m_szDataName, "OPY_");
		else
			strcat(m_szDataName, "OPN_");
	}
	if(dwDataDiffMask & DDLM_ORG_DATATYPE)				// 原始数据类型
	{
		sprintf(szTemp, "ODT%02X_", m_btOrgDataType);
		strcat(m_szDataName, szTemp);
	}
	if(dwDataDiffMask & DDLM_ORG_NUMBERCOUNT)			// 原始号码个数
	{
		sprintf(szTemp, "ONC%1X_", m_btOrgNumberCount);
		strcat(m_szDataName, szTemp);
	}
	if(dwDataDiffMask & DDLM_ORG_MINNUMBER)				// 原始数据最小号码
	{
		sprintf(szTemp, "OIN%02X_", m_btOrgMinNumber);
		strcat(m_szDataName, szTemp);
	}
	if(dwDataDiffMask & DDLM_ORG_MAXNUMBER)				// 原始数据最大号码
	{
		sprintf(szTemp, "OXN%02X_", m_btOrgMaxNumber);
		strcat(m_szDataName, szTemp);
	}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(dwDataDiffMask & DDLM_ANALYSISMODE)					// 分析方式
	{
		DWORD dwAMFlag = 0;
		GetCurrentAnalysisMode(&dwAMFlag);
		sprintf(szTemp, "AM%08lX_", dwAMFlag);
		strcat(m_szDataName, szTemp);
	}
	if(dwDataDiffMask & DDLM_ISORGAM)					// 是否原始分析方式
	{
		if(g_pIData->IsOrgAnalysisMode())
			strcat(m_szDataName, "OAY_");
		else
			strcat(m_szDataName, "OAN_");
	}
	if(dwDataDiffMask & DDLM_CAM_DATATYPE)				// 当前分析方式数据类型
	{
		sprintf(szTemp, "ADT%02X_", m_btDataType);
		strcat(m_szDataName, szTemp);
	}
	if(dwDataDiffMask & DDLM_CAM_NUMBERCOUNT)			// 当前分析方式号码个数
	{
		sprintf(szTemp, "ANC%1X_", m_btNumberCount);
		strcat(m_szDataName, szTemp);
	}
	if(dwDataDiffMask & DDLM_CAM_MINNUMBER)				// 当前分析方式数据最小号码
	{
		sprintf(szTemp, "AIN%02X_", m_btMinNumber);
		strcat(m_szDataName, szTemp);
	}
	if(dwDataDiffMask & DDLM_CAM_MAXNUMBER)				// 当前分析方式数据最大号码
	{
		sprintf(szTemp, "AXN%02X_", m_btMaxNumber);
		strcat(m_szDataName, szTemp);
	}
    
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
	if(dwDataDiffMask & DDLM_DATATRANS)					// 数据变换标志
	{
		if(m_stDataTrans.btType != DATATRANS_TYPE_ORG)	// 非原始数据变换
		{
			sprintf(szTemp, "DT%04X%02X%02X%02X_", m_stDataTrans.wID, m_stDataTrans.btItemCount, m_btMinNumber, m_btMaxNumber);
			strcat(m_szDataName, szTemp);
		}
	}
    /*	if(dwDataDiffMask & DDLM_ISORGDT)					// 是否原始数据变换
     {
     if(g_pIData->IsOrgDataTrans())
     strcat(m_szDataName, "OTY_");
     else
     strcat(m_szDataName, "OTN_");
     }*/
	if(dwDataDiffMask & DDLM_CUR_DATATYPE)				// 当前数据类型
	{
		sprintf(szTemp, "CDT%02X_", m_btDataType);
		strcat(m_szDataName, szTemp);
	}
	if(dwDataDiffMask & DDLM_CUR_NUMBERCOUNT)			// 当前号码个数
	{
		sprintf(szTemp, "CNC%1X_", m_btNumberCount);
		strcat(m_szDataName, szTemp);
	}
	if(dwDataDiffMask & DDLM_CUR_MINNUMBER)				// 当前数据最小号码
	{
		sprintf(szTemp, "CIN%02X_", m_btMinNumber);
		strcat(m_szDataName, szTemp);
	}
	if(dwDataDiffMask & DDLM_CUR_MAXNUMBER)				// 当前数据最大号码
	{
		sprintf(szTemp, "CXN%02X_", m_btMaxNumber);
		strcat(m_szDataName, szTemp);
	}
	if(dwDataDiffMask & DDLM_CUR_ORDERTYPE)				// 当前数据排列方式
	{
		if(m_btDataOrderType != DATA_ORDERTYPE_NONE)
		{
			sprintf(szTemp, "COT%02X_", m_btDataOrderType);
			strcat(m_szDataName, szTemp);
		}
	}
    
	m_szDataName[strlen(m_szDataName) - 1] = 0;		// 去除最后面的"_"
	CHECK_STRING_STACK(m_szDataName);
    
	return m_szDataName;
}

BYTE CData::GetNumberCount(BYTE btDataSource)
{
	switch(btDataSource)
	{
        case DATA_SOURCE_CUR:		// 当前数据源
            return m_btNumberCount;
            break;
            
        case DATA_SOURCE_ORG:		// 原始数据源
            return m_btOrgNumberCount;
            break;
            
        case DATA_SOURCE_INIT:		// 初始数据源
            return m_btInitNumberCount;
            break;
            
        default: ASSERT(FALSE); break;
	}
	
	return 0;
}

BYTE CData::GetMinNumber(BYTE btDataSource)
{
	switch(btDataSource)
	{
        case DATA_SOURCE_CUR:		// 当前数据源
            return m_btMinNumber;
            break;
            
        case DATA_SOURCE_ORG:		// 原始数据源
            return m_btOrgMinNumber;
            break;
            
        case DATA_SOURCE_INIT:		// 初始数据源
            return m_btInitMinNumber;
            break;
            
        default: ASSERT(FALSE); break;
	}
	
	return 0;
}

BYTE CData::GetMaxNumber(BYTE btDataSource)
{
	switch(btDataSource)
	{
        case DATA_SOURCE_CUR:		// 当前数据源
            return m_btMaxNumber;
            break;
            
        case DATA_SOURCE_ORG:		// 原始数据源
            return m_btOrgMaxNumber;
            break;
            
        case DATA_SOURCE_INIT:		// 初始数据源
            return m_btInitMaxNumber;
            break;
            
        default: ASSERT(FALSE); break;
	}
	
	return 0;
}

BYTE CData::GetAllNumberCount(BYTE btDataSource)
{
	return (BYTE)(GetMaxNumber(btDataSource) - GetMinNumber(btDataSource) + 1);
}

LPCSTR CData::GetNumberText(int nIndex, BYTE btNumber, BYTE btDataSource)
{
	if(nIndex != -1)
	{
		ASSERT(nIndex >= 0 && nIndex < GetAllNumberCount(btDataSource));
		btNumber = (BYTE)(GetMinNumber(btDataSource) + nIndex);
	}
    
	switch(GetDataType(btDataSource) & DATA_TYPE_NUM_MASK)
	{
        case DATA_TYPE_NUM_LT: sprintf(m_szNumberText, "%02d", btNumber); break;
        case DATA_TYPE_NUM_PK:
		{
			switch(btNumber)
			{
                case 1:	m_szNumberText[0] = 'A'; break;
                case 10: m_szNumberText[0] = 'T'; break;
                case 11: m_szNumberText[0] = 'J'; break;
                case 12: m_szNumberText[0] = 'Q'; break;
                case 13: m_szNumberText[0] = 'K'; break;
                default: m_szNumberText[0] = char(btNumber + '0');
			}
			m_szNumberText[1] = 0;
		} break;
        default: m_szNumberText[0] = char(btNumber + '0'); m_szNumberText[1] = 0; break;
	}
    
	CHECK_STRING_STACK(m_szNumberText);
	return m_szNumberText;
}

BYTE CData::GetNumberValue(LPCSTR lpszNumText, BYTE btDataSource)
{
	ASSERT(lpszNumText != NULL);
    
	BYTE btNumber = MAXBYTE;
    
	if(GetDataType(btDataSource) & DATA_TYPE_NUM_PK)
	{
		switch(lpszNumText[0])
		{
            case 'a':
            case 'A': return 1;
            case 't':
            case 'T': return 10;
            case 'j':
            case 'J': return 11;
            case 'q':
            case 'Q': return 12;
            case 'k':
            case 'K': return 13;
		}
	}
    
	btNumber = (BYTE)atoi(lpszNumText);
	if(btNumber >= GetMinNumber() && btNumber <= GetMaxNumber())
		return btNumber;
	
	return MAXBYTE;
}


DWORD CData::GetNextIssue(BYTE btDataSource)
{
	DWORD dwNextIssue = 0;
    
	CTime tmCurDateTime;
	GetItemDateTime(DATA_INDEX_LAST, tmCurDateTime, btDataSource);
    
	if(btDataSource == DATA_SOURCE_CUR)	// 当前数据
	{
        
	}
	else		// 原始或初始数据
	{
	}
    
	m_dwSpecifiedIssue = GetItemIssue(DATA_INDEX_LAST, btDataSource);
	dwNextIssue = GetNextSpecifiedIssue(tmCurDateTime);
	m_dwSpecifiedIssue = 0;
    
	return dwNextIssue;
}

DWORD CData::GetFirstSpecifiedIssue(CTime &tmDateTime, DWORD dwIssue)
{
	int nCount = GetItemCount(DATA_SOURCE_INIT);
	m_dwSpecifiedIssue = 0;		// 初始化
    
	if(dwIssue == 0)			// tmDateTime有效，则根据时间查询开奖号码索引
	{
		// 获取最后一期的时间
		CTime tmLastDateTime;
		GetItemDateTime(nCount - 1, tmLastDateTime, DATA_SOURCE_INIT);
		DWORD dwDateTime = tmDateTime.GetYear() * 10000 + tmDateTime.GetMonth() * 100 + tmDateTime.GetDay();
		DWORD dwLastDateTime = tmLastDateTime.GetYear() * 10000 + tmLastDateTime.GetMonth() * 100 + tmLastDateTime.GetDay();
		if(m_stLotteryProperty.btOpenTimesPerDay > 1)	// 高频彩
		{
			dwDateTime -= 20000000;
			dwDateTime *= 10000;
			dwDateTime += tmDateTime.GetHour() * 100 + tmDateTime.GetMinute();
			dwLastDateTime -= 20000000;
			dwLastDateTime *= 10000;
			dwLastDateTime += tmLastDateTime.GetHour() * 100 + tmLastDateTime.GetMinute();
		}
		if(dwDateTime <= dwLastDateTime)	// 历史时间，则查询是否存在
		{
            int i = 0;
			for(i=nCount-1; i>=0; i--)
			{
				GetItemDateTime(i, tmLastDateTime, DATA_SOURCE_INIT);
				dwLastDateTime = tmLastDateTime.GetYear() * 10000 + tmLastDateTime.GetMonth() * 100 + tmLastDateTime.GetDay();
				if(m_stLotteryProperty.btOpenTimesPerDay > 1)	// 高频彩
				{
					dwLastDateTime -= 20000000;
					dwLastDateTime *= 10000;
					dwLastDateTime += tmLastDateTime.GetHour() * 100 + tmLastDateTime.GetMinute();
				}
				if(dwLastDateTime == dwDateTime)
				{
					break;	// find
				}
			}
			if(i < 0)
			{			
				return 0;	// 没有找到则返回错误
			}
			m_dwSpecifiedIssue = GetItemIssue(i, DATA_SOURCE_INIT);
			return m_dwSpecifiedIssue;
		}
		else	// 未开出的时间，则以最后一期为基准获取
		{
			m_dwSpecifiedIssue = GetItemIssue(nCount - 1, DATA_SOURCE_INIT);
			DWORD dwDateTime = tmDateTime.GetYear() * 10000 + tmDateTime.GetMonth() * 100 + tmDateTime.GetDay();
			DWORD dwLastDateTime = tmLastDateTime.GetYear() * 10000 + tmLastDateTime.GetMonth() * 100 + tmLastDateTime.GetDay();
			if(m_stLotteryProperty.btOpenTimesPerDay > 1)	// 高频彩
			{
				dwDateTime -= 20000000;
				dwDateTime *= 10000;
				dwDateTime += tmDateTime.GetHour() * 100 + tmDateTime.GetMinute();
				dwLastDateTime -= 20000000;
				dwLastDateTime *= 10000;
				dwLastDateTime += tmLastDateTime.GetHour() * 100 + tmLastDateTime.GetMinute();
			}
			while(dwLastDateTime < dwDateTime)
			{
				GetNextSpecifiedIssue(tmLastDateTime);
				dwLastDateTime = tmLastDateTime.GetYear() * 10000 + tmLastDateTime.GetMonth() * 100 + tmLastDateTime.GetDay();
				if(m_stLotteryProperty.btOpenTimesPerDay > 1)	// 高频彩
				{
					dwLastDateTime -= 20000000;
					dwLastDateTime *= 10000;
					dwLastDateTime += tmLastDateTime.GetHour() * 100 + tmLastDateTime.GetMinute();
				}
			}
			if(dwLastDateTime != dwDateTime)
			{
				m_dwSpecifiedIssue = 0;
				return 0;	// 该时间不是特定的开奖时间
			}
			return m_dwSpecifiedIssue;
		}
	}
	else					// dwIssue有效
	{
		// 获取最后一期的期号
		DWORD dwLastIssue = GetItemIssue(nCount - 1, DATA_SOURCE_INIT);
		if(dwIssue <= dwLastIssue)	// 历史期号，则查询是否存在
		{
            int i = 0;
			for(i=nCount-1; i>=0; i--)
			{
				if(GetItemIssue(i, DATA_SOURCE_INIT) == dwIssue)
				{
					GetItemDateTime(i, tmDateTime, DATA_SOURCE_INIT);
					break;	// find
				}
			}
			if(i < 0)
			{
				return 0;	// 没有找到则返回错误
			}
			m_dwSpecifiedIssue = dwIssue;
			return dwIssue;
		}
		else	// 未开出的期号，则以最后一期为基准获取
		{
			m_dwSpecifiedIssue = dwLastIssue;
			GetItemDateTime(nCount - 1, tmDateTime, DATA_SOURCE_INIT);
			while(m_dwSpecifiedIssue < dwIssue)
			{
				GetNextSpecifiedIssue(tmDateTime);
			}
			if(m_dwSpecifiedIssue != dwIssue)
			{
				m_dwSpecifiedIssue = 0;
				return 0;	// 该时间不是特定的开奖期号
			}
            
			return m_dwSpecifiedIssue;
		}
	}
    
	return 0;
}

DWORD CData::GetNextSpecifiedIssue(CTime &tmDateTime)
{
	ASSERT(m_dwSpecifiedIssue != 0);		// 必须已经调用了GetFirstSpecifiedIssue
    
	if(m_stLotteryProperty.btOpenTimesPerDay <= 1)		// 非高频彩
	{
		ASSERT(m_stLotteryProperty.btIssueType == ISSUE_TYPE_YYYYXXX);
		// 获取下期开奖日期
		BYTE btFlags[] = {CSTLPOF_DAY0, CSTLPOF_DAY1, CSTLPOF_DAY2, CSTLPOF_DAY3, CSTLPOF_DAY4, CSTLPOF_DAY5, CSTLPOF_DAY6};
		for(int i=1; i<=7; i++)
		{
			tmDateTime.TimeSpan(1);
			if(m_stLotteryProperty.btOpenFlag & btFlags[tmDateTime.GetDayOfWeek() - 1])
				break;		// 该日开奖
		}
		
		if(m_dwSpecifiedIssue / 1000 == (DWORD)tmDateTime.GetYear())
		{
			m_dwSpecifiedIssue ++;
		}
		else	// 跨年
		{
			m_dwSpecifiedIssue = tmDateTime.GetYear() * 1000 + 1;
		}
		return m_dwSpecifiedIssue;
	}
    
	// 高频彩
	CTime tmCurDateTime = tmDateTime;
	if(!_GetGPLotteryNextTime(tmCurDateTime, tmDateTime))	// 当天的期号
	{
		m_dwSpecifiedIssue ++;
	}
	else	// 处理跨天第一期
	{
		switch(m_stLotteryProperty.btIssueType)
		{
            case ISSUE_TYPE_YYXXXXXX:
			{
				if(tmDateTime.GetYear() != tmCurDateTime.GetYear())
				{
					// 跨年，则转入该年第一期
					m_dwSpecifiedIssue = (tmDateTime.GetYear() % 100) * 1000000 + 1;
				}
				else
				{
					m_dwSpecifiedIssue ++;
				}
			} break;
                
            case ISSUE_TYPE_YYYYMMDDXX:
			{
				m_dwSpecifiedIssue = tmDateTime.GetYear() * 1000000 + tmDateTime.GetMonth() * 10000 + tmDateTime.GetDay() * 100 + 1;
			} break;
            case ISSUE_TYPE_YYMMDDXX:
			{
				m_dwSpecifiedIssue = (tmDateTime.GetYear() - 2000) * 1000000 + tmDateTime.GetMonth() * 10000 + tmDateTime.GetDay() * 100 + 1;
			} break;
            case ISSUE_TYPE_YYYYMMDDXXX:	// 数值同ISSUE_TYPE_YYMMDDXXX
            case ISSUE_TYPE_YYMMDDXXX:
			{
				m_dwSpecifiedIssue = (tmDateTime.GetYear() - 2000) * 10000000 + tmDateTime.GetMonth() * 100000 + tmDateTime.GetDay() * 1000 + 1;
			} break;
            default: ASSERT(FALSE); break;
		}
	}
    
	return m_dwSpecifiedIssue;		// 返回下一期期号
}


LPCSTR CData::GetIssueString(DWORD dwIssue)
{
	sprintf(m_szIssueString, "%0*lu", GetIssueLen(), dwIssue);
	
	CHECK_STRING_STACK(m_szIssueString);
	return m_szIssueString;
}

LPCSTR CData::GetDateTimeString(CTime &tmDateTime, BYTE btFlag)
{
	switch(btFlag)
	{
        case DTSF_DATE:
            sprintf(m_szDateTimeString, "%04d-%02d-%02d", tmDateTime.GetYear(), tmDateTime.GetMonth(), tmDateTime.GetDay());
            break;
        case DTSF_TIME:
            if(m_stLotteryProperty.btOpenTimesPerDay > 1)
                sprintf(m_szDateTimeString, "%02d:%02d", tmDateTime.GetHour(), tmDateTime.GetMinute());
            else
                m_szDateTimeString[0] = 0;
            break;
        case DTSF_DATETIME:
            if(m_stLotteryProperty.btOpenTimesPerDay > 1)
                sprintf(m_szDateTimeString, "%04d-%02d-%02d %02d:%02d", tmDateTime.GetYear(), tmDateTime.GetMonth(), tmDateTime.GetDay(), tmDateTime.GetHour(), tmDateTime.GetMinute());
            else
                sprintf(m_szDateTimeString, "%04d-%02d-%02d", tmDateTime.GetYear(), tmDateTime.GetMonth(), tmDateTime.GetDay());
            break;
        default: ASSERT(FALSE); break;
	}
    
	CHECK_STRING_STACK(m_szDateTimeString);
	return m_szDateTimeString;
}

DWORD CData::GetPrevDataIndex(DWORD dwDataIndex, BYTE btDataSource)
{
	DWORD dwPrevDataIndex = DATA_INDEX_INVALID;
	DWORD dwItemCount = GetItemCount(btDataSource);
	
	switch(dwDataIndex)
	{
        case 0:
            dwPrevDataIndex = DATA_INDEX_INVALID;
            break;
            
        case DATA_INDEX_LAST:
        case DATA_INDEX_ONLYTESTNUMS:
            dwPrevDataIndex = (dwItemCount >= 2) ? (dwItemCount - 2) : DATA_INDEX_INVALID;
            break;
            
        case DATA_INDEX_FORECAST:
        case DATA_INDEX_INVALID:
            dwPrevDataIndex = (dwItemCount >= 1) ? (dwItemCount - 1) : DATA_INDEX_INVALID;
            break;
            
        default:
		{
			if(dwDataIndex <= dwItemCount)
			{
				dwPrevDataIndex = dwDataIndex - 1;
			}
		} break;
	}
	
	return dwPrevDataIndex;
}

DWORD CData::GetNextDataIndex(DWORD dwDataIndex, BYTE btDataSource)
{
	DWORD dwNextDataIndex = DATA_INDEX_INVALID;
    
	switch(dwDataIndex)
	{
        case DATA_INDEX_INVALID:
        case DATA_INDEX_FORECAST:
            dwNextDataIndex = DATA_INDEX_INVALID;
            break;
            
        case DATA_INDEX_LAST:
        case DATA_INDEX_ONLYTESTNUMS:
            dwNextDataIndex = (btDataSource == DATA_SOURCE_CUR) ? DATA_INDEX_FORECAST : DATA_INDEX_INVALID;
            break;
            
        default:
		{
			DWORD dwItemCount = GetItemCount(btDataSource);
			if(dwDataIndex < dwItemCount - 1)
			{
				dwNextDataIndex = dwDataIndex + 1;
			}
			else if(dwDataIndex == dwItemCount - 1)	// 最后一期
			{
				dwNextDataIndex = DATA_INDEX_FORECAST;
			}
		} break;
	}
    
	return dwNextDataIndex;
}

void CData::ResetCurUsedData(DWORD dwFlag)
{
	_ResetCurUsedData(dwFlag, FALSE);
	
	if(m_cCurDataFactory.GetCount(TRUE) == 0)
	{
// TEST		::MessageBox(g_pICstMain->GetMainWnd(), "注意：当前没有可使用的数据，系统将自动使用全部数据。", "提示信息", MB_ICONINFORMATION);
		_ResetCurUsedData(dwFlag, TRUE);
	}
}

int CData::GetSkipValue(DWORD dwFlag, DWORD dwIssueIndex, LPBYTE lpFilterNums, BOOL bSingle)
{
	LPDWORD lpSkipValueData = bSingle ? _CalcSkipValueData_Single() : _CalcSkipValueData_Group();
	ASSERT(lpSkipValueData != NULL);
    
	int nRtn = CDTVALUE_INVALID, nDataItemCount = m_cCurDataFactory.GetCount(TRUE);
	if(dwIssueIndex >= (DWORD)nDataItemCount)
	{
		dwIssueIndex = DATA_INDEX_INVALID;	// 看做缩水数据，也即预测行数据
	}
    
	if(dwIssueIndex != DATA_INDEX_INVALID)
	{
		// 获取历史号码的遗漏值
		ASSERT(dwIssueIndex <= (DWORD)nDataItemCount);
		int nIndex = dwIssueIndex * (m_btNumberCount + 2);
		switch(dwFlag) 
		{
            case GSKF_SUM:
                nRtn = lpSkipValueData[nIndex + m_btNumberCount];
                break;
                
            case GSKF_SUM_TAIL:
                nRtn = lpSkipValueData[nIndex + m_btNumberCount + 1];
                break;
                
            case GSKF_TAILARR_GROUP:		// 遗漏值尾组选排列
			{
				int nRatio = 1;
				BYTE btSkipTails[8] = {0};
                
				for(int i=m_btNumberCount-1; i>=0; i--)
				{
					btSkipTails[i] = BYTE(lpSkipValueData[nIndex+i] % 10);
				}
                
				g_pICstPubFunc->QuickSort(btSkipTails, m_btNumberCount, QSVT_BYTE);
                
				nRtn = 0;
				for(int i=m_btNumberCount-1; i>=0; i--)
				{
					nRtn += btSkipTails[i] * nRatio;
					nRatio *= 10;
				}
			}
                break;
                
            case GSKF_TAILARR_SINGLE:		// 遗漏值尾单选排列
			{
				int nRatio = 1;
				nRtn = 0;
				for(int i=m_btNumberCount-1; i>=0; i--)
				{
					nRtn += (lpSkipValueData[nIndex+i] % 10) * nRatio;
					nRatio *= 10;
				}
			}
                break;
                
            case GSKF_MIN:
			{
				nRtn = lpSkipValueData[nIndex++];
				for(int i=1; i<m_btNumberCount; i++)
				{
					nRtn = fmin(nRtn, (int)lpSkipValueData[nIndex]);
					nIndex ++;
				}
			} break;
            case GSKF_TAIL_MIN:
			{
				nRtn = lpSkipValueData[nIndex++] % 10;
				for(int i=1; i<m_btNumberCount; i++)
				{
					nRtn = fmin(nRtn, (int)lpSkipValueData[nIndex] % 10);
					nIndex ++;
				}
			} break;
            case GSKF_MAX:
			{
				nRtn = lpSkipValueData[nIndex++];
				for(int i=1; i<m_btNumberCount; i++)
				{
					nRtn = fmax(nRtn, (int)lpSkipValueData[nIndex]);
					nIndex ++;
				}
			} break;
            case GSKF_TAIL_MAX:
			{
				nRtn = lpSkipValueData[nIndex++] % 10;
				for(int i=1; i<m_btNumberCount; i++)
				{
					nRtn = fmax(nRtn, (int)(lpSkipValueData[nIndex] % 10));
					nIndex ++;
				}
			} break;
                
            default:		// GSKF_POS_1 ... GSKF_POS_8
                nRtn = lpSkipValueData[nIndex + (dwFlag - GSKF_POS_1)];
                break;
		}
	}
	else	// 指定缩水号码的遗漏值
	{
		ASSERT(lpFilterNums != NULL);
		int i = 0, nIndex = nDataItemCount * (m_btNumberCount + 2);
		int nOffset = bSingle ? (m_btMaxNumber + 1) : 0;
        
		switch(dwFlag) 
		{
            case GSKF_SUM:
            case GSKF_SUM_TAIL:
                nRtn = 0;
                for(i=0; i<m_btNumberCount; i++)
                {
                    nRtn += lpSkipValueData[nIndex + lpFilterNums[i]];
                    nIndex += nOffset;
                }
                
                if(dwFlag == GSKF_SUM_TAIL)
                {
                    nRtn %= 10;
                }
                break;
                
            case GSKF_TAILARR_GROUP:		// 遗漏值尾组选排列
			{
				nRtn = 0;
				BYTE btSkipTails[8] = {0};
                
				for(i=0; i<m_btNumberCount; i++)
				{
					btSkipTails[i] = (BYTE)(lpSkipValueData[nIndex + lpFilterNums[i]] % 10);
					nIndex += nOffset;
				}
                
				g_pICstPubFunc->QuickSort(btSkipTails, m_btNumberCount);
				
				int nRatio = 1;
				for(i=m_btNumberCount-1; i>=0; i--)
				{
					nRtn += btSkipTails[i] * nRatio;
					nRatio *= 10;
				}
			} break;
                
            case GSKF_TAILARR_SINGLE:		// 遗漏值尾单选排列
			{
				nRtn = 0;
				int nTemp = 1;
				nIndex += (m_btNumberCount-1) * nOffset;
				for(i=m_btNumberCount-1; i>=0; i--)
				{
					nRtn += (lpSkipValueData[nIndex + lpFilterNums[i]] % 10) * nTemp;
					nIndex -= nOffset;
					nTemp *= 10;
				}
			}
                break;
                
            case GSKF_MIN:
			{
				nRtn = lpSkipValueData[nIndex + lpFilterNums[0]];
				nIndex += nOffset;
				for(i=1; i<m_btNumberCount; i++)
				{
					nRtn = fmin(nRtn, (int)lpSkipValueData[nIndex + lpFilterNums[i]]);
					nIndex += nOffset;
				}
			} break;
            case GSKF_TAIL_MIN:
			{
				nRtn = lpSkipValueData[nIndex + lpFilterNums[0]] % 10;
				nIndex += nOffset;
				for(i=1; i<m_btNumberCount; i++)
				{
					nRtn = fmin(nRtn, (int)lpSkipValueData[nIndex + lpFilterNums[i]] % 10);
					nIndex += nOffset;
				}
			} break;
            case GSKF_MAX:
			{
				nRtn = lpSkipValueData[nIndex + lpFilterNums[0]];
				nIndex += nOffset;
				for(i=1; i<m_btNumberCount; i++)
				{
					nRtn = fmax(nRtn, (int)lpSkipValueData[nIndex + lpFilterNums[i]]);
					nIndex += nOffset;
				}
			} break;
            case GSKF_TAIL_MAX:
			{
				nRtn = lpSkipValueData[nIndex + lpFilterNums[0]] % 10;
				nIndex += nOffset;
				for(i=1; i<m_btNumberCount; i++)
				{
					nRtn = fmax(nRtn, (int)lpSkipValueData[nIndex + lpFilterNums[i]] % 10);
					nIndex += nOffset;
				}
			} break;
                
            default:		// GSKF_ONE ... GSKF_EIGHT
                if(bSingle)
                    nRtn = lpSkipValueData[nIndex + (dwFlag - GSKF_POS_1) * (m_btMaxNumber + 1) + lpFilterNums[dwFlag - GSKF_POS_1]];
                else
                    nRtn = lpSkipValueData[nIndex + lpFilterNums[dwFlag - GSKF_POS_1]];
                break;
		}
	}	
    
	return nRtn;
}

int CData::GetThreeStateData(BYTE btTypeIndex, DWORD dwFlag, DWORD dwIssueIndex, LPBYTE lpFilterNums)
{	
	LPBYTE lpTSData = NULL;
	int i = 0, nBeginIndex = 0, nRtn = 0;
    
	switch(btTypeIndex) 
	{
        case GTSDT_LGC:
            lpTSData = _CalcLGCData();
            nBeginIndex = 1;
            break;
        case GTSDT_FGZ:
            lpTSData = _CalcFGZData();
            nBeginIndex = 2;
            break;
        case GTSDT_RWL:
            lpTSData = _CalcRWLData();
            nBeginIndex = m_stRWLInfo.wIssueCount;
            break;
        case GTSDT_CXT:
            lpTSData = _CalcCXTData();
            nBeginIndex = 2;
            break;
        case GTSDT_SPJ:
            return _GetSPJData(dwFlag, dwIssueIndex, lpFilterNums);
            break;
        default:
            ASSERT(FALSE);
            break;
	}
    
	if(lpTSData == NULL)
		return CDTVALUE_INVALID;			// 获取数据失败
    
	LPBYTE lpCurLotteryData = lpFilterNums;
	if(lpCurLotteryData == NULL)
	{
		ASSERT(dwIssueIndex != DATA_INDEX_INVALID);
		lpCurLotteryData = m_cCurDataFactory.GetItemNums(dwIssueIndex);
	}
    
	int nCount = GetItemCount(DATA_SOURCE_CUR);
	if(dwIssueIndex == DATA_INDEX_LAST) 
		dwIssueIndex = nCount - 1;
	else if(dwIssueIndex == DATA_INDEX_INVALID || dwIssueIndex == DATA_INDEX_FORECAST)
		dwIssueIndex = nCount;
    
	if(nCount < nBeginIndex || dwIssueIndex < (DWORD)nBeginIndex)
		return CDTVALUE_INVALID;	// 分析期数不够
    
	int nBallCount = m_btMaxNumber - m_btMinNumber + 1;
	
	switch(dwFlag)
	{
        case GTSDF_ONE:
        case GTSDF_TWO:
        case GTSDF_THREE:
        case GTSDF_FOUR:
        case GTSDF_FIVE:
        case GTSDF_SIX:
        case GTSDF_SEVEN:
        case GTSDF_EIGHT:
            nRtn = lpTSData[dwIssueIndex * nBallCount + lpCurLotteryData[dwFlag - GTSDF_ONE] - m_btMinNumber];
            break;
        case GTSDF_COUNT_0:
        case GTSDF_COUNT_1:
        case GTSDF_COUNT_2:
            for(i=0; i<m_btNumberCount; i++)
            {
                if(lpTSData[dwIssueIndex * nBallCount + lpCurLotteryData[i] - m_btMinNumber] == (dwFlag - GTSDF_COUNT_0))
                    nRtn ++;
            } 
            break;
            
        case GTSDF_ARR_COUNT:
		{
			int nCount[3] = {0}; 
			for(i=0; i<m_btNumberCount; i++)
			{
				nCount[lpTSData[dwIssueIndex * nBallCount + lpCurLotteryData[i] - m_btMinNumber]] ++;
			}
			nRtn = nCount[0] * 100 + nCount[1] * 10 + nCount[2];
		} break;
            
        case GTSDF_ARR_SIN:
		{
			int nRatio = 1;
			for(i=m_btNumberCount-1; i>=0; i--)
			{
				nRtn += lpTSData[dwIssueIndex * nBallCount + lpCurLotteryData[i] - m_btMinNumber] * nRatio;
				nRatio *= 10;
			}
		} break;
            
        case GTSDF_ARR_GRO:
		{
			int nRatio = 1;
			BYTE btTemp[MAX_NUMBER_COUNT] = {0};
			for(i=m_btNumberCount-1; i>=0; i--)
			{
				btTemp[i] = (BYTE)lpTSData[dwIssueIndex * nBallCount + lpCurLotteryData[i] - m_btMinNumber];
			}
			g_pICstPubFunc->QuickSort(btTemp, m_btNumberCount);
			for(i=m_btNumberCount-1; i>=0; i--)
			{
				nRtn += btTemp[i] * nRatio;
				nRatio *= 10;
			}
		} break;
            
        case GTSDF_ARR_SIN_VALUE:
		{
			int j = 1;
			for(i=m_btNumberCount-1; i>=0; i--)
			{
				nRtn += lpTSData[dwIssueIndex * nBallCount + lpCurLotteryData[i] - m_btMinNumber] * j;
				j *= 3;
			}
		} break;
            
        case GTSDF_SHAPE:
		{
			ASSERT(m_btNumberCount == 3);	// 3D专用
			int nCount[3] = {0};
			for(i=0; i<3; i++)
			{
				nCount[lpTSData[dwIssueIndex * nBallCount + lpCurLotteryData[i] - m_btMinNumber]] ++;
			}
			if(nCount[0] == 3 || nCount[1] == 3 || nCount[2] == 3)
			{
				nRtn = 0;		// 全路
			}
			else if(nCount[0] == 1 && nCount[1] == 1)
			{
				nRtn = 2;		// 111路
			}
			else
			{
				nRtn = 1;		// 断一路
			}
		} break;
            
        default:
            if(dwFlag >= m_btMinNumber && dwFlag < (DWORD)nBallCount + m_btMinNumber)
            {
                nRtn = lpTSData[dwIssueIndex * nBallCount + dwFlag - m_btMinNumber];
            }
            else
            {
                nRtn = -1;
            }
            break;
	}
    
	return nRtn;
}

DWORD CData::GetNumsValue(LPBYTE lpNums, BYTE btDataSource)
{
	if(btDataSource == DATA_SOURCE_CUR)
		return m_cCurDataFactory.GetNumsValue(lpNums);
	else
		return m_pInitDataFactory->GetNumsValue(lpNums);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
LPBYTE CData::DataTransTable_GetTransData(DWORD dwDataValue)
{
// TEST	return theApp.m_cDataTransTable.GetTransData(dwDataValue);
    return NULL;
}

BOOL CData::DataTransTable_IsOrgTrans()
{
// TEST	return theApp.m_cDataTransTable.IsOrgTrans();
    return NULL;
}

BOOL CData::DataTransTable_OrderData(LPBYTE lpNumsData, BYTE btNumberCount)
{
// TEST	return theApp.m_cDataTransTable.OrderData(lpNumsData, btNumberCount);
    return NULL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CData::IsCracked()
{
	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
void CData::Initialize(CDataFactory * pInitDataFactory)
{
	ASSERT(pInitDataFactory != NULL);
    
	m_pInitDataFactory = pInitDataFactory;
}

void CData::OnLtyNumsChanged(LPLOTTERYPROPERTY lpLotteryProperty)
{
	ASSERT(lpLotteryProperty != NULL);
    
    _InitLotteryProperty();
    
	// 初始化初始数据
	m_btInitDataType = m_stLotteryProperty.stSectionInfo[0].btDataType;
	m_btInitNumberCount = m_stLotteryProperty.stSectionInfo[0].btNumberCount;
	m_btInitMinNumber = m_stLotteryProperty.stSectionInfo[0].btMinNumber;
	m_btInitMaxNumber = m_stLotteryProperty.stSectionInfo[0].btMaxNumber;
}

