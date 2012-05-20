//
//  QCDataPickerViewController.m
//  QCLottery
//
//  Created by 武 孙 on 12-5-19.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "QCDataPickerViewController.h"

@interface QCDataPickerViewController ()

@end

@implementation QCDataPickerViewController
@synthesize dataPicker, numberCount;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self)
    {
        // Custom initialization
        numbersArray = [NSArray arrayWithObjects:@"0", @"1", @"2", @"3", @"4", @"5", @"6", @"7", @"8", @"9", Nil];
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    self.contentSizeForViewInPopover = dataPicker.frame.size;
    int nValue = *pValue;
    for(int i=numberCount-1; i>=0; i--)
    {
        [dataPicker selectRow:((nValue % 10) + 500) inComponent:i animated:NO];
        nValue /= 10;
    }
}

- (void)viewDidUnload
{
    [self setDataPicker:nil];
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (void)viewWillDisappear:(BOOL)animated
{
    *pValue = 0;
    for (int i=0; i<numberCount; i++)
    {
        *pValue *= 10;
        *pValue += [dataPicker selectedRowInComponent:i] % 10;
    }
}

#pragma mark - UIPickerViewDelegate and UIPickerViewDataSource Method
- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
    return self.numberCount;
}

- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component
{
    return 1000;
}

- (NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component
{
    return [numbersArray objectAtIndex:(row%10)];
}

#pragma mark - other functions
- (void)setValue:(int *)pIntValue
{
    pValue = pIntValue;
}
@end
