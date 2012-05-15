//
//  QCDataManViewController.h
//  QCLottery
//
//  Created by 武 孙 on 12-5-14.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface QCDataManViewController : UITableViewController <UITableViewDataSource, UITableViewDelegate>
{
    NSArray *cmdArray;
}

@property (strong, nonatomic) UIPopoverController *popoverContorller;
@end
