//
//  QCDataPickerViewController.h
//  QCLottery
//
//  Created by 武 孙 on 12-5-19.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import <UIKit/UIKit.h>

@interface QCDataPickerViewController : UIViewController <UIPickerViewDataSource, UIPickerViewDelegate>
{
    NSArray *numbersArray;
    int *pValue;
}
@property (weak, nonatomic) IBOutlet UIPickerView *dataPicker;
@property (nonatomic, assign) NSInteger numberCount;

- (void)setValue:(int *)pIntValue;
@end
