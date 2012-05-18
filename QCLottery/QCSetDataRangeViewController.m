//
//  QCSetDataRangeViewController.m
//  QCLottery
//
//  Created by 武 孙 on 12-5-17.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "QCSetDataRangeViewController.h"

@interface QCSetDataRangeViewController ()

@end

@implementation QCSetDataRangeViewController
@synthesize delegate;
@synthesize excludeTextField;
@synthesize stepper;
@synthesize switcher;

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
    self.navigationItem.leftBarButtonItem = [[UIBarButtonItem alloc]initWithBarButtonSystemItem:UIBarButtonSystemItemCancel target:self action:@selector(clickCancel)];
    self.navigationItem.rightBarButtonItem = [[UIBarButtonItem alloc]initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(clickDone)];
}

- (void)viewDidUnload
{
    [self setExcludeTextField:nil];
    [self setStepper:nil];
    [self setSwitcher:nil];
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return YES;
}

- (void)clickCancel
{
    if ([delegate respondsToSelector:@selector(dataManSubVCDidExecuteCmd:withDataChanged:)])
    {
        [delegate dataManSubVCDidExecuteCmd:DMCMDID_DATA_RANGE withDataChanged:NO];
    }
}

- (void)clickDone
{
    if ([delegate respondsToSelector:@selector(dataManSubVCDidExecuteCmd:withDataChanged:)])
    {
        [delegate dataManSubVCDidExecuteCmd:DMCMDID_DATA_RANGE withDataChanged:YES];
    }
}
- (IBAction)stepperValueChanged:(id)sender
{
    excludeTextField.text = [NSString stringWithFormat:@"%.f", self.stepper.value];
}

- (IBAction)switchValueChanged:(id)sender 
{
    if (self.switcher.on)
    {
        stepper.enabled = YES;
        excludeTextField.enabled = YES;
    }
    else
    {
        stepper.enabled = NO;
        excludeTextField.enabled = NO;
    }
}
@end
