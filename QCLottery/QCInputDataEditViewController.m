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
    DWORD dwLastDate = g_pIData->GetItemDate(DATA_INDEX_LAST, DATA_SOURCE_INIT);
    [comps setDay:dwLastDate % 100];
    dwLastDate /= 100;
    [comps setMonth:dwLastDate % 100];
    dwLastDate /= 100;
    [comps setYear:2012];
    NSCalendar *gregorian = [[NSCalendar alloc]initWithCalendarIdentifier:NSGregorianCalendar];
    NSDate *date = [gregorian dateFromComponents:comps];
    
    if (dataItemIndex == -1)    // 用于添加
    {
        
    }
    else    // 用于修改
    {
        
    }
    
    [datePicker setMinimumDate:[NSDate dateWithTimeInterval:24 * 60 * 60 sinceDate:date]];
    [datePicker setMaximumDate:[NSDate date]];

    [issueText setDelegate:self];
    [issueText setPlaceholder:[NSString stringWithFormat:@"%d位数字", g_pIData->GetIssueLen()]];
    issueText.text = [NSString stringWithFormat:@"%d", g_pIData->GetNextIssue(DATA_SOURCE_INIT)];

    [testnumsText setDelegate:self];
    NSString *strNumsInfo = [NSString stringWithFormat:@"%d位数字", g_pIData->GetNumberCount(DATA_SOURCE_INIT)];
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
    DWORD dwDataTime = 0, dwIssue = 0;
    BYTE btData[MAX_NUMBER_COUNT] = {0};
    NSCalendar *calendar = [datePicker calendar];
    NSDateComponents *dateComponents = [[NSDateComponents alloc] init];
    [dateComponents setCalendar:calendar];
    int n = [dateComponents year];
    n = [dateComponents month];
    n = [dateComponents day];
    dwDataTime = [dateComponents year] * 10000 + [dateComponents month] * 100 + [dateComponents day];
    dwDataTime *= 10000;    // 时和分设为0

    dwIssue = issueText.text.intValue;
    
    // 更新数据
    if (dataItemIndex == -1)    // 增加数据
    {
        g_pIData->AddLtyNums(dwDataTime, dwIssue, btData);
    }
    else // 修改数据
    {
        g_pIData->ModifyLtyNums(dataItemIndex, dwDataTime, dwIssue, btData);
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
        [textField resignFirstResponder];
        [self clickDone];
    }
    
    return YES;
}
@end
