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
        
        NSArray *tabVCs = [NSArray arrayWithObjects:graphMainVC, randomMainVC, historyMainVC, nil];
        [self setViewControllers:tabVCs];
        [self setDelegate:self];
        
        // 创建导航栏上的“开奖号码”按钮
        bbiData = [[UIBarButtonItem alloc]initWithTitle:@"开奖号码" style:UIBarButtonItemStylePlain target:self action:@selector(clickDataButton)];
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
            bbiSep = [[UIBarButtonItem alloc]initWithTitle:@"条件列表" style:UIBarButtonItemStylePlain target:self action:@selector(popoverMainView)];         
        }
    }

    [[self navigationItem] setLeftBarButtonItems:[NSArray arrayWithObjects:bbiData, bbiSep, nil]];
}

- (void)clickDataButton
{
    QCDataManViewController *dataManVC = [[QCDataManViewController alloc]init];
    [dataManVC setModalPresentationStyle:UIModalPresentationFormSheet];
    [self presentModalViewController:dataManVC animated:YES];
}

- (void)popoverMainView
{
    UISplitViewController *splitViewController = (UISplitViewController *)[self selectedViewController];
    UIViewController *mainVC = [[splitViewController viewControllers] objectAtIndex:0];
    popoverController = [[UIPopoverController alloc] initWithContentViewController:mainVC];
    
    UIBarButtonItem *bbi = [[[self navigationItem] leftBarButtonItems] objectAtIndex:1];
    [popoverController presentPopoverFromBarButtonItem:bbi permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
}

#pragma mark -- tabBarControllerDelegate相关函数
- (void)tabBarController:(UITabBarController *)tabBarController didSelectViewController:(UIViewController *)viewController
{
    [self resetLeftBarButtonItems];
}
@end
