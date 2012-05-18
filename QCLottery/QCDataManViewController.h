//
//  QCDataManViewController.h
//  QCLottery
//
//  Created by 武 孙 on 12-5-14.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

// 定义命令ID
enum DATAMAN_CMDID {DMCMDID_DOWNLOAD = 0, DMCMDID_INPUT, DMCMDID_DATA_RANGE, DMCMDID_DATA_DIV, DMCMDID_DATA_ORDER, DMCMDID_DATA_RWL};

// 定义委托协议
@protocol QCDataManViewControllerDelegate <NSObject>

// 执行数据管理列表的命令
- (void)dataManVCExecuteCmd:(DATAMAN_CMDID)cmdID;
// 数据管理列表命令的子视图控制器执行完毕
- (void)dataManSubVCDidExecuteCmd:(DATAMAN_CMDID)cmdID withDataChanged:(BOOL)dataChanged;
@end

@interface QCDataManViewController : UITableViewController <UITableViewDataSource, UITableViewDelegate>
{
    NSArray *cmdArray;
}

@property (assign, nonatomic) id<QCDataManViewControllerDelegate> delegate; // 委托对象
@end
