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
    
    // 获取当期数据信息
    NSDate *date = nil;
    CTime tmDate, tmMinDate, tmMaxDate;
    int nIssue = 0;
    if (dataItemIndex == -1)    // 用于添加
    {
        g_pIData->GetItemDateTime(DATA_INDEX_LAST, tmDate, DATA_SOURCE_INIT);
        tmDate.TimeSpan(1);
        tmMinDate = tmDate;
        tmMaxDate = CTime([NSDate date]);
        nIssue = g_pIData->GetNextIssue(DATA_SOURCE_INIT);
        
    }
    else    // 用于修改
    {
        g_pIData->GetItemDateTime(dataItemIndex, tmDate, DATA_SOURCE_INIT);
        nIssue = g_pIData->GetItemIssue(dataItemIndex, DATA_SOURCE_INIT);
        if (dataItemIndex > 1)
        {
            g_pIData->GetItemDateTime(dataItemIndex-1, tmMinDate, DATA_SOURCE_INIT);
            tmMinDate.TimeSpan(1);
        }
        else
        {
            tmMinDate = tmDate;
        }
        if (dataItemIndex >= g_pIData->GetItemCount(DATA_SOURCE_INIT) - 1)
        {
            tmMaxDate = CTime([NSDate date]);
        }
        else
        {
            g_pIData->GetItemDateTime(dataItemIndex+1, tmMaxDate, DATA_SOURCE_INIT);
            tmMaxDate.TimeSpan(-1);
        }
        testnumsText.text = [NSString stringWithUTF8String:g_pIData->GetItemTestNumsString(dataItemIndex, DATA_SOURCE_INIT)];
        numsText.text = [NSString stringWithUTF8String:g_pIData->GetItemNumsString(dataItemIndex, DATA_SOURCE_INIT)];
    }
    date = tmDate.GetNSDate();
    
    // 设置相关控件的值
    [datePicker setMinimumDate:tmMinDate.GetNSDate()];
    [datePicker setMaximumDate:tmMaxDate.GetNSDate()];
    [datePicker setDate:tmDate.GetNSDate() animated:YES];

    [issueText setDelegate:self];
    [issueText setPlaceholder:[NSString stringWithFormat:@"%d位数字", g_pIData->GetIssueLen()]];
    issueText.text = [NSString stringWithFormat:@"%d", nIssue];

    [testnumsText setDelegate:self];
    NSString *strNumsInfo = [NSString stringWithFormat:@"%d位数字", g_pIData->GetNumberCount()];
    [testnumsText setPlaceholder:strNumsInfo];
    [numsText setDelegate:self];
    [numsText setPlaceholder:strNumsInfo];
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
    // 检查并获取数据
    BYTE btData[MAX_NUMBER_COUNT] = {0};
    CTime tmDate([datePicker date]);

    DWORD dwIssue = issueText.text.intValue;
    
    // 更新数据
    if (dataItemIndex == -1)    // 增加数据
    {
        g_pIData->AddLtyNums(tmDate.GetYearMonthDayHourMinute(), dwIssue, btData);
    }
    else // 修改数据
    {
        g_pIData->ModifyLtyNums(dataItemIndex, tmDate.GetYearMonthDayHourMinute(), dwIssue, btData);
    }
    
    // 通知委托对象
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

- (BOOL)textFieldShouldReturn:(UITextField *)textField
{
    if (textField == issueText)
    {
        if (textField.text.length != g_pIData->GetIssueLen())
        {
            return NO;
        }
        [testnumsText becomeFirstResponder];
    }
    else if (textField == testnumsText)
    {
        if (textField.text.length != g_pIData->GetNumberCount(DATA_SOURCE_INIT))
        {
            return NO;
        }
        [numsText becomeFirstResponder];
    }
    else if (textField == numsText)
    {
        if (textField.text.length != g_pIData->GetNumberCount(DATA_SOURCE_INIT))
        {
            return NO;
        }
        [numsText resignFirstResponder];
        [self clickDone];
    }
    
    return YES;
}
@end
