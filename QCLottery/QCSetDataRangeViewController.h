//
//  QCSetDataRangeViewController.h
//  QCLottery
//
//  Created by 武 孙 on 12-5-17.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "QCDataManViewController.h"

// 定义配置KEY值
NSString * const DataRangeTypePrefKey = @"DataRange_Type";
NSString * const DataRangeKeepIssuePrefKey = @"DataRange_KeepIssue";
NSString * const DataRangeExcludeIssuePrefKey = @"DataRange_ExcludeIssue";

@interface QCSetDataRangeViewController : UIViewController <UITableViewDelegate, UITableViewDataSource>
{
    IBOutlet UITableViewCell *keepLastCell;
    NSIndexPath *lastIndexPath;
    int nKeepIssue;
    int nExcludeIssue;
}

@property (assign, nonatomic) id<QCDataManViewControllerDelegate> delegate;
@property (weak, nonatomic) IBOutlet UITableView *rangeTableView;
@property (weak, nonatomic) IBOutlet UISwitch *switcher;
@property (weak, nonatomic) IBOutlet UIButton *btnExcludeIssue;
@property (weak, nonatomic) IBOutlet UIButton *btnKeepIssue;
@property (weak, nonatomic) IBOutlet UILabel *lableExclude;
@property (weak, nonatomic) IBOutlet UILabel *lableExcludeIssue;
@property (strong, nonatomic) NSIndexPath *lastIndexPath;


- (IBAction)switchValueChanged:(id)sender;
- (IBAction)clickIssueBtn:(id)sender;

@end
