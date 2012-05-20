//
//  QCDataManViewController.m
//  QCLottery
//
//  Created by 武 孙 on 12-5-14.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "QCDataManViewController.h"
#import "QCSetDataRangeViewController.h"

@interface QCDataManViewController ()

@end

@implementation QCDataManViewController
@synthesize delegate;

- (id)initWithStyle:(UITableViewStyle)style
{
    self = [super initWithStyle:style];
    if (self) {
        // Custom initialization
        cmdArray = [NSArray arrayWithObjects:@"下载开奖号码", @"录入开奖号码", @"号码范围设置", @"号码间隔设置", @"号码排列方式", @"热温冷设置", Nil];
    }
    
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    self.view.frame = CGRectMake(0, 0, 320, 44 * [cmdArray count]);
    self.tableView.bounces = NO;
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

#pragma mark -- UITableViewDelegate and UITableViewDataSource Method
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return [cmdArray count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *cellIdentify = @"DataManCell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellIdentify];
    if (cell == nil)
    {
        cell = [[UITableViewCell alloc]initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:cellIdentify];
    }
    
    // 设置标题
    int nIndex = [indexPath row];
    cell.textLabel.text = [cmdArray objectAtIndex:nIndex];
    
    // 设置详细信息
    cell.detailTextLabel.text = [self cmdDetailText:nIndex];
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    if ([delegate respondsToSelector:@selector(dataManVCExecuteCmd:)])
    {
        DATAMAN_CMDID cmdID = (DATAMAN_CMDID)[indexPath row];
        [delegate dataManVCExecuteCmd:cmdID];
    }
}

#pragma mark - 以下是私有函数
- (NSString *)cmdDetailText:(NSInteger)nIndex
{
    NSString *detailText = nil;
    NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
    
    switch (nIndex)
    {
    case DMCMDID_INPUT: detailText = @"手工录入开奖号码，无需连接网络"; break;
    case DMCMDID_DOWNLOAD: detailText = @"从官方网站下载开奖号码，需要连接网络"; break;
    case DMCMDID_DATA_RANGE:
        {
            NSString *strRangeType = nil;
            switch ([userDefaults integerForKey:DataRangeTypePrefKey])
            {
                case 0: strRangeType = @"全部开奖号码"; break;
                case 1: strRangeType = @"最近一年的号码"; break;
                case 2: strRangeType = [NSString stringWithFormat:@"最近 %d 期号码", [userDefaults integerForKey:DataRangeKeepIssuePrefKey]]; break;
            }
            int nExcludeIssue = [userDefaults integerForKey:DataRangeExcludeIssuePrefKey];
            if (nExcludeIssue > 0)
            {
                detailText = [strRangeType stringByAppendingFormat:@"，排除最近的 %d 期号码", nExcludeIssue];
            }
            else
            {
                detailText = strRangeType;
            }
        }break;
    case DMCMDID_DATA_DIV: detailText = @"无间隔"; break;
    case DMCMDID_DATA_ORDER: detailText = @"按开奖顺序排列"; break;
    case DMCMDID_DATA_RWL: detailText = @"分析期数：10期，温码为出现3次的号码"; break;
    }
        
    return detailText;
}
@end
