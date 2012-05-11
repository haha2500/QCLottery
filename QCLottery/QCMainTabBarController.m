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
        
        // 设置导航按钮
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
    // 重置导航按钮
   [self resetLeftBarButtonItems];
    [super didRotateFromInterfaceOrientation:fromInterfaceOrientation];
}

#pragma mark -- 导航栏相关函数
- (void) resetLeftBarButtonItems
{
    // 增加导航栏的开奖号码按钮和分隔主视图所需按钮
    UIBarButtonItem *bbiData = [[UIBarButtonItem alloc]initWithTitle:@"开奖号码" style:UIBarButtonItemStylePlain target:self action:@selector(clickDataButton)];
    
    UIBarButtonItem *bbiSep = nil;
    UIInterfaceOrientation orientation = [[UIApplication sharedApplication]statusBarOrientation];
    if (orientation == UIInterfaceOrientationPortrait || orientation == UIInterfaceOrientationPortraitUpsideDown) // 竖屏，则显示所需导航按钮
    {
        if ([[self tabBarController] selectedViewController] == graphMainVC)
        {
            bbiSep = [[UIBarButtonItem alloc]initWithTitle:@"条件列表" style:UIBarButtonItemStylePlain target:self action:@selector(showMainView)];         
        }
    }

    [[self navigationItem] setLeftBarButtonItems:[NSArray arrayWithObjects:bbiData, bbiSep, nil]];
}


@end
