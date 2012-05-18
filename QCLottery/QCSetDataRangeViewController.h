//
//  QCSetDataRangeViewController.h
//  QCLottery
//
//  Created by 武 孙 on 12-5-17.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "QCDataManViewController.h"

@interface QCSetDataRangeViewController : UIViewController
@property (assign, nonatomic) id<QCDataManViewControllerDelegate> delegate;
@property (weak, nonatomic) IBOutlet UITextField *excludeTextField;
@property (weak, nonatomic) IBOutlet UIStepper *stepper;
@property (weak, nonatomic) IBOutlet UISwitch *switcher;
- (IBAction)stepperValueChanged:(id)sender;
- (IBAction)switchValueChanged:(id)sender;
@end
