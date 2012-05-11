//
//  QCGraphMainViewController.h
//  QCLottery
//
//  Created by 武 孙 on 12-5-9.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
@class QCConditionViewController;
@class QCGraphViewController;

@interface QCGraphMainViewController : UISplitViewController
{
    QCConditionViewController *conditionVC;
    QCGraphViewController *graphVC;
}
@end
