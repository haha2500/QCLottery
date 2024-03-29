//
//  QCMainTabBarController.m
//  QCLottery
//
//  Created by 武 孙 on 12-5-9.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "QCMainTabBarController.h"
#import "QCGraphMainViewController.h"
#import "QCRandomMainViewController.h"
#import "QCHistoryMainViewController.h"
#import "QCDataManViewController.h"
#import "QCInputDataViewController.h"
#import "QCSetDataRangeViewController.h"
#import "IData.h"

@interface QCMainTabBarController ()

@end

@implementation QCMainTabBarController

- (id)init
{
    if (self = [super init])
    {
        // 初始化TAB页相关子视图控制器
        graphMainVC = [[QCGraphMainViewController alloc]init];
        [graphMainVC setDelegate:self];
        randomMainVC = [[QCRandomMainViewController alloc]init];
        historyMainVC = [[QCHistoryMainViewController alloc]init];
        
        NSArray *tabVCs = [NSArray arrayWithObjects:graphMainVC, randomMainVC, historyMainVC, Nil];
        [self setViewControllers:tabVCs];
        [self setDelegate:self];
        
        // 创建导航栏上的“开奖号码”按钮
        bbiData = [[UIBarButtonItem alloc]initWithTitle:@"数据管理" style:UIBarButtonItemStylePlain target:self action:@selector(clickDataButton)];
        [self resetLeftBarButtonItems];
    }
    
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return YES;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation
{
    [super didRotateFromInterfaceOrientation:fromInterfaceOrientation];
    
    // 重置导航按钮
    [self resetLeftBarButtonItems];
}

#pragma mark -- 导航栏相关函数
- (void)resetLeftBarButtonItems
{
    // 设置导航栏的开奖号码按钮和分隔主视图所需按钮
    UIBarButtonItem *bbiSep = nil;
    UIInterfaceOrientation orientation = [[UIApplication sharedApplication]statusBarOrientation];
    if (orientation == UIInterfaceOrientationPortrait || orientation == UIInterfaceOrientationPortraitUpsideDown) // 竖屏，则显示所需导航按钮
    {
        if ([self selectedViewController] == graphMainVC)
        {
            bbiSep = [[UIBarButtonItem alloc]initWithTitle:@"条件列表" style:UIBarButtonItemStylePlain target:self action:@selector(popoverMasterView)];         
        }
    }

    [[self navigationItem] setLeftBarButtonItems:[NSArray arrayWithObjects:bbiData, bbiSep, Nil]];
}

- (void)clickDataButton
{
    QCDataManViewController *dataManVC = [[QCDataManViewController alloc]initWithStyle:UITableViewStylePlain];
    [dataManVC setDelegate:self];
    
   // [dataManVC setModalPresentationStyle:UIModalPresentationFormSheet];
   // [self presentModalViewController:dataManVC animated:YES];
    UIBarButtonItem *bbi = [[self navigationItem] leftBarButtonItem];
    [self showPopoverController:dataManVC atBarButtonItem:bbi];
}

- (void)popoverMasterView
{
    UISplitViewController *splitViewController = (UISplitViewController *)[self selectedViewController];
    UIViewController *masterVC = [[splitViewController viewControllers] objectAtIndex:0];
    
    UIBarButtonItem *bbi = [[[self navigationItem] leftBarButtonItems] objectAtIndex:1];
    [self showPopoverController:masterVC atBarButtonItem:bbi];
}

- (void)showPopoverController:(UIViewController *)viewController atBarButtonItem:(UIBarButtonItem *)bbi
{
    if (popoverController != nil)
    {
        [popoverController dismissPopoverAnimated:NO];
        popoverController = nil;
    }
    popoverController = [[UIPopoverController alloc] initWithContentViewController:viewController];
    [popoverController setDelegate:self];
    [popoverController setPopoverContentSize:viewController.view.frame.size];
    
    [popoverController presentPopoverFromBarButtonItem:bbi permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
}

- (void)showPopoverControllerInCenter:(UIViewController *)viewController
{
    if (popoverController != nil)
    {
        [popoverController dismissPopoverAnimated:NO];
        popoverController = nil;
    }
    popoverController = [[UIPopoverController alloc] initWithContentViewController:viewController];
    [popoverController setDelegate:self];
    [popoverController setPopoverContentSize:viewController.view.frame.size];
    
    CGRect rectView = self.view.frame;
    CGRect rect = CGRectMake(rectView.size.width / 2 - 2, rectView.size.height / 2 - 2, 4, 4);
    [popoverController presentPopoverFromRect:rect inView:self.view permittedArrowDirections:0 animated:YES];
}

#pragma mark -- UIPopoverControllerDelegate相关函数
- (void)popoverControllerDidDismissPopover:(UIPopoverController *)popoverController
{

}

#pragma mark -- QCDataManViewControllerDelegate相关函数
- (void)dataManVCExecuteCmd:(DATAMAN_CMDID)cmdID
{ 
    if (popoverController != nil)
    {
        [popoverController dismissPopoverAnimated:NO];
        popoverController = nil;
    }

    switch (cmdID)
    {
        case DMCMDID_INPUT: [self dataManFunc_InputData]; break;
        case DMCMDID_DOWNLOAD: [self dataManFunc_DownloadData]; break;
        case DMCMDID_DATA_RANGE: [self dataManFunc_SetDataRange]; break;
        case DMCMDID_DATA_DIV: break;
        case DMCMDID_DATA_ORDER: break;
        case DMCMDID_DATA_RWL: break;
    }
}

- (void)dataManSubVCDidExecuteCmd:(DATAMAN_CMDID)cmdID withDataChanged:(BOOL)dataChanged
{
    if (popoverController != nil)
    {
        [popoverController dismissPopoverAnimated:NO];
        popoverController = nil;
    }
    
    if (dataChanged)  // 数据改变，则重新装载当前数据
    {
        
    }
}

#pragma mark -- 数据管理相关函数
- (void)dataManFunc_InputData
{
    QCInputDataViewController *inputDataVC = [[QCInputDataViewController alloc] initWithStyle:UITableViewStylePlain];
    UINavigationController *nav = [[UINavigationController alloc] initWithRootViewController:inputDataVC];
    [nav setModalPresentationStyle:UIModalPresentationFormSheet];
  //  [self presentModalViewController:nav animated:YES];
    
    [self presentViewController:nav animated:YES completion:NULL];
}

- (void)dataManFunc_DownloadData
{
    // 显示等待窗口
    UIAlertView *waitingDialog = [[UIAlertView alloc] initWithTitle:nil
												 message:@"正在下载最新开奖号码，请等待 ..."
												delegate:self
									   cancelButtonTitle:nil
									   otherButtonTitles:nil];
		[waitingDialog show];
	UIActivityIndicatorView *activityIndicator = [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleWhiteLarge];
     activityIndicator.center = CGPointMake(waitingDialog.bounds.size.width / 2.0f, waitingDialog.bounds.size.height - 46.0f);   
    [activityIndicator startAnimating];
	
	[waitingDialog addSubview:activityIndicator];
    
    // 下载数据
       
    BOOL bModified = FALSE;
    if(!g_pIData->DownloadLtyNums(bModified))
    {
        // 下载失败
        UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"提示信息" message:@"下载开奖号码失败，请稍后再试。" delegate:self cancelButtonTitle:@"关闭" otherButtonTitles:nil];
        [alert show];
    }
    else if (bModified)
    {
        // 需要更新数据显示
        // TEST
    }
    
    // 关闭等待窗口
   [waitingDialog dismissWithClickedButtonIndex:0 animated:NO];
}

- (void)dataManFunc_SetDataRange
{
    QCSetDataRangeViewController *setDataRangeVC = [[QCSetDataRangeViewController alloc] initWithNibName:@"QCSetDataRangeViewController" bundle:nil];
    [setDataRangeVC setDelegate:self];
    UINavigationController *nav = [[UINavigationController alloc] initWithRootViewController:setDataRangeVC];
    CGRect rect = setDataRangeVC.view.frame;
    rect.size.height += nav.navigationBar.frame.size.height;
    nav.view.frame = rect;
    
    [self showPopoverControllerInCenter:nav];
}
    
#pragma mark -- UITabBarControllerDelegate相关函数
- (void)tabBarController:(UITabBarController *)tabBarController didSelectViewController:(UIViewController *)viewController
{
    [self resetLeftBarButtonItems];
}
@end
