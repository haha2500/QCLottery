//
//  QCGraphMainViewController.m
//  QCLottery
//
//  Created by 武 孙 on 12-5-9.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "QCGraphMainViewController.h"
#import "QCConditionViewController.h"
#import "QCGraphViewController.h"

@interface QCGraphMainViewController ()

@end

@implementation QCGraphMainViewController

- (id)init
{
    if (self = [super init])
    {
        // 设置TabBar
        [[self tabBarItem] setTitle:@"看图选号"];
        
        // 初始化左侧和右侧的视图控制器
        conditionVC = [[QCConditionViewController alloc]init];
        graphVC = [[QCGraphViewController alloc]init];
        NSArray *vcs = [NSArray arrayWithObjects:conditionVC, graphVC, nil];
        [self setViewControllers:vcs];
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

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)toInterfaceOrientation
{
    return YES;
}
@end
