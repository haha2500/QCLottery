//
//  QCHistoryMainViewController.m
//  QCLottery
//
//  Created by 武 孙 on 12-5-10.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "QCHistoryMainViewController.h"

@interface QCHistoryMainViewController ()

@end

@implementation QCHistoryMainViewController

- (id)init
{
    if (self = [super init])
    {
        // 设置TabBar
        [[self tabBarItem] setTitle:@"选号记录"];
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

@end
