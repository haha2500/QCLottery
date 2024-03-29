//
//  QCMainTabBarController.h
//  QCLottery
//
//  Created by 武 孙 on 12-5-9.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "QCDataManViewController.h"

@class QCGraphMainViewController;
@class QCRandomMainViewController;
@class QCHistoryMainViewController;


@interface QCMainTabBarController : UITabBarController <UISplitViewControllerDelegate, UITabBarControllerDelegate, UIPopoverControllerDelegate, QCDataManViewControllerDelegate>
{
    QCGraphMainViewController *graphMainVC;         // 看图选号主界面
    QCRandomMainViewController *randomMainVC;       // 随机选号主界面
    QCHistoryMainViewController *historyMainVC;     // 选号记录主界面
    
    UIBarButtonItem *bbiData;                       // 导航栏上的“开奖号码”按钮
    UIPopoverController *popoverController;         // 弹出视图控制器
}
@end
