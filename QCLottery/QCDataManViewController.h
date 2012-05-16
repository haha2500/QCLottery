//
//  QCDataManViewController.h
//  QCLottery
//
//  Created by 武 孙 on 12-5-14.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

// 定义命令ID
enum DATAMAN_CMDID {DMCMDID_INPUT = 0, DMCMDID_DOWNLOAD, DMCMDID_DATA_RANGE, DMCMDID_DATA_DIV, DMCMDID_DATA_ORDER, DMCMDID_DATA_RWL};

// 定义委托协议
@protocol QCDataManViewControllerDelegate <NSObject>

- (void)dataManVCExecuteCmd:(DATAMAN_CMDID)cmdID;

@end

@interface QCDataManViewController : UITableViewController <UITableViewDataSource, UITableViewDelegate>
{
    NSArray *cmdArray;
}

@property (assign, nonatomic) id<QCDataManViewControllerDelegate> delegate; // 委托对象
@end
