//
//  QCInputDataEditViewController.m
//  QCLottery
//
//  Created by 武 孙 on 12-5-16.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "QCInputDataEditViewController.h"
#import "IData.h"

@interface QCInputDataEditViewController ()

@end

@implementation QCInputDataEditViewController
@synthesize delegate, dataItemIndex;
@synthesize issueText;
@synthesize testnumsText;
@synthesize numsText;
@synthesize datePicker;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // Do any additional setup after loading the view from its nib.
    // 设置导航栏按钮
    self.navigationItem.leftBarButtonItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemCancel target:self action:@selector(clickCancel)];
    self.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(clickDone)];
    
    // 设置相关控件的值
    NSDateComponents *comps = [[NSDateComponents alloc] init];
    [comps setDay:6];
    [comps setMonth:5];
    [comps setYear:2012];
    NSCalendar *gregorian = [[NSCalendar alloc]initWithCalendarIdentifier:NSGregorianCalendar];
    NSDate *date = [gregorian dateFromComponents:comps];
    [datePicker setMinimumDate:date];
    [datePicker setMaximumDate:[NSDate date]];

    [issueText setDelegate:self];
    [testnumsText setDelegate:self];
    [numsText setDelegate:self];
}

- (void)viewDidUnload
{
    [self setIssueText:nil];
    [self setTestnumsText:nil];
    [self setNumsText:nil];
    [self setDatePicker:nil];
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return YES;
}

- (void)clickDone
{
    // 更新数据
    if ([delegate respondsToSelector:@selector(inputDataEditVCClose:)])
    {
        [delegate inputDataEditVCClose:YES];
    }
}

- (void)clickCancel
{
    if ([delegate respondsToSelector:@selector(inputDataEditVCClose:)])
    {
        [delegate inputDataEditVCClose:NO];
    }
}

#pragma mark - UITextFieldDelegate Method
- (BOOL)textField:(UITextField *)textField shouldChangeCharactersInRange:(NSRange)range replacementString:(NSString *)string
{
    // 限制输入长度 
    if(textField == issueText && textField.text.length >= g_pIData->GetIssueLen())
    {
        return NO;
    }
    
    if((textField == testnumsText || textField == numsText) && (textField.text.length >= g_pIData->GetNumberCount(DATA_SOURCE_INIT)))
    {
        return NO;
    }

    return YES;
}
@end
