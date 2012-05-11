//
//  QCRandomMainViewController.m
//  QCLottery
//
//  Created by 武 孙 on 12-5-10.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "QCRandomMainViewController.h"

@interface QCRandomMainViewController ()

@end

@implementation QCRandomMainViewController

- (id)init
{
    if (self = [super init])
    {
        // 设置TabBar
        [[self tabBarItem] setTitle:@"随机选号"];
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
