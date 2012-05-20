//
//  QCSetDataRangeViewController.m
//  QCLottery
//
//  Created by 武 孙 on 12-5-17.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "QCSetDataRangeViewController.h"
#import "QCDataPickerViewController.h"

@interface QCSetDataRangeViewController ()

@end

@implementation QCSetDataRangeViewController
@synthesize btnExcludeIssue;
@synthesize btnKeepIssue;
@synthesize lableExclude;
@synthesize lableExcludeIssue;
@synthesize delegate, lastIndexPath;
@synthesize rangeTableView;
@synthesize switcher;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self)
    {
        // 获取相关配置值
        nExcludeIssue = [NSNumber numberWithInt:[[NSUserDefaults standardUserDefaults] integerForKey:DataRangeExcludeIssuePrefKey]].intValue;
        
        nKeepIssue = [NSNumber numberWithInt:[[NSUserDefaults standardUserDefaults] integerForKey:DataRangeKeepIssuePrefKey]].intValue;
   }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    self.navigationItem.leftBarButtonItem = [[UIBarButtonItem alloc]initWithBarButtonSystemItem:UIBarButtonSystemItemCancel target:self action:@selector(clickCancel)];
    self.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc]initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(clickDone)];
    self.contentSizeForViewInPopover = self.view.frame.size;
    
    NSInteger type = [[NSUserDefaults standardUserDefaults] integerForKey:DataRangeTypePrefKey];
    [self _checkTableCell:[NSIndexPath indexPathForRow:type inSection:0]];
}

- (void)viewDidUnload
{
    [self setSwitcher:nil];
    [self setRangeTableView:nil];
    [self setBtnExcludeIssue:nil];
    [self setBtnKeepIssue:nil];
    [self setLableExclude:nil];
    [self setLableExcludeIssue:nil];
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (void)viewWillAppear:(BOOL)animated
{
    // 重新显示配置中的数据
    NSString *titleExclude = [NSString stringWithFormat:@"%d", abs(nExcludeIssue)];
    [[self switcher] setOn:(nExcludeIssue > 0)];
    [self.btnExcludeIssue setTitle:titleExclude forState:UIControlStateNormal];
    [self switchValueChanged:[self switcher]];
    
    NSString *titleKeep = [NSString stringWithFormat:@"%d", nKeepIssue];
    [btnKeepIssue setTitle:titleKeep forState:UIControlStateNormal];
}

- (void)clickCancel
{
    if ([delegate respondsToSelector:@selector(dataManSubVCDidExecuteCmd:withDataChanged:)])
    {
        [delegate dataManSubVCDidExecuteCmd:DMCMDID_DATA_RANGE withDataChanged:NO];
    }
}

- (void)clickDone
{
    // 保存数据范围设置
    NSUserDefaults *userDefaults = [NSUserDefaults standardUserDefaults];
    [userDefaults setInteger:[lastIndexPath row] forKey:DataRangeTypePrefKey];
    [userDefaults setInteger:nKeepIssue forKey:DataRangeKeepIssuePrefKey];
    [userDefaults setInteger:nExcludeIssue forKey:DataRangeExcludeIssuePrefKey];
    
    if ([delegate respondsToSelector:@selector(dataManSubVCDidExecuteCmd:withDataChanged:)])
    {
        [delegate dataManSubVCDidExecuteCmd:DMCMDID_DATA_RANGE withDataChanged:YES];
    }
}

- (IBAction)switchValueChanged:(id)sender 
{
    BOOL bEnable = self.switcher.on;
    
    btnExcludeIssue.enabled = bEnable;
    lableExclude.enabled = bEnable;
    lableExcludeIssue.enabled = bEnable;
    
    nExcludeIssue = abs(nExcludeIssue);
    if (!bEnable)
    {
        nExcludeIssue = 0 - nExcludeIssue;
    }
 }

- (IBAction)clickIssueBtn:(id)sender
{
    QCDataPickerViewController *dataPickVC = [[QCDataPickerViewController alloc] initWithNibName:@"QCDataPickerViewController" bundle:nil];
    dataPickVC.numberCount = 5;

    if (sender == btnKeepIssue)
    {
         [[dataPickVC navigationItem] setTitle:@"最近期数设置"];
        [dataPickVC setValue:&nKeepIssue];
    }
    else 
    {
        [[dataPickVC navigationItem] setTitle:@"排除期数设置"];
        [dataPickVC setValue:&nExcludeIssue];
    }

    [[self navigationController] pushViewController:dataPickVC animated:YES];
}

#pragma mark - UITableViewDelegata and datasource Method
- (NSString *)tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section
{
    return @"请选择需要使用的开奖号码的范围";
}

- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView
{
    return 1;
}

- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 3;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    UITableViewCellAccessoryType accessoryTypeOfCell = UITableViewCellAccessoryNone;
    if (lastIndexPath != nil && [lastIndexPath row] == [indexPath row])
    {
        accessoryTypeOfCell = UITableViewCellAccessoryCheckmark;
    }
    
    if ([indexPath row] == 2)
    {
        keepLastCell.accessoryType = accessoryTypeOfCell;
        return keepLastCell;
    }
    
    static NSString *cellIdentifier = @"DataRangeCell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellIdentifier];
    if (cell == nil)
    {
       cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellIdentifier];
    }
    
    switch ([indexPath row])
    {
        case 0:
            cell.textLabel.text= @"全部号码";
            cell.detailTextLabel.text= @"所有开奖号码";
            break;
        case 1:
            cell.textLabel.text= @"最近一年的号码";
            cell.detailTextLabel.text= @"最近一年的号码";
            break;
        case 2:
            cell.textLabel.text= @"最近100期号码";
            cell.detailTextLabel.text= @"最近100期号码";
            break;
        default:
            break;
    }
    
    cell.accessoryType = accessoryTypeOfCell;
    cell.selectionStyle = UITableViewCellSelectionStyleNone;
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    [self _checkTableCell:indexPath];
}

#pragma mark - private Method
- (void)_checkTableCell:(NSIndexPath *)indexPath
{
    for (int i=[rangeTableView numberOfRowsInSection:0]-1; i>=0; i--)
    {
        if ([indexPath row] == i)
        {
            [rangeTableView cellForRowAtIndexPath:indexPath].accessoryType = UITableViewCellAccessoryCheckmark;
            lastIndexPath = indexPath;
        }
        else
        {
            [rangeTableView cellForRowAtIndexPath:[NSIndexPath indexPathForRow:i inSection:0]].accessoryType = UITableViewCellAccessoryNone;
        }
    }
}

@end
