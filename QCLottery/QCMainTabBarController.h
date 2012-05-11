//
//  QCMainTabBarController.h
//  QCLottery
//
//  Created by 武 孙 on 12-5-9.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@class QCGraphMainViewController;
@class QCRandomMainViewController;
@class QCHistoryMainViewController;

@interface QCMainTabBarController : UITabBarController <UISplitViewControllerDelegate>
{
    QCGraphMainViewController *graphMainVC;
    QCRandomMainViewController *randomMainVC;
    QCHistoryMainViewController *historyMainVC;
}
@end
