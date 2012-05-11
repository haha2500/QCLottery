//
//  QCAppDelegate.h
//  QCLottery
//
//  Created by 武 孙 on 12-5-9.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
@class QCMainTabBarController;

@interface QCAppDelegate : UIResponder <UIApplicationDelegate, UITabBarControllerDelegate>
{
    QCMainTabBarController *mainTabBarController;
}
@property (strong, nonatomic) UIWindow *window;

@end
