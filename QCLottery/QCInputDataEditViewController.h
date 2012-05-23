//
//  QCInputDataEditViewController.h
//  QCLottery
//
//  Created by 武 孙 on 12-5-16.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

// 委托协议
@protocol QCInputDataEditViewControllerDelegate <NSObject>

// 关闭该对话框，dataModified==YES时则表示数据被更新过
- (void)inputDataEditVCClose:(BOOL)dataModified;

@end


@interface QCInputDataEditViewController : UIViewController <UITextFieldDelegate>
@property (assign, nonatomic) NSInteger dataItemIndex;
@property (assign, nonatomic) id<QCInputDataEditViewControllerDelegate> delegate;
@property (weak, nonatomic) IBOutlet UITextField *issueText;
@property (weak, nonatomic) IBOutlet UITextField *testnumsText;
@property (weak, nonatomic) IBOutlet UITextField *numsText;
@property (weak, nonatomic) IBOutlet UIDatePicker *datePicker;

@end
