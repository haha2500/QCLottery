//
//  QCInputDataViewController.h
//  QCLottery
//
//  Created by 武 孙 on 12-5-15.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "QCInputDataEditViewController.h"

@interface QCInputDataViewController : UITableViewController <UITableViewDataSource, UITableViewDelegate, UIPopoverControllerDelegate, QCInputDataEditViewControllerDelegate>
{
    UIPopoverController *popoverController;
}
@end
