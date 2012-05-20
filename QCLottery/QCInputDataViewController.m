//
//  QCInputDataViewController.m
//  QCLottery
//
//  Created by 武 孙 on 12-5-15.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "QCInputDataViewController.h"
#import "IData.h"

@interface QCInputDataViewController ()

@end

@implementation QCInputDataViewController

- (id)initWithStyle:(UITableViewStyle)style
{
    self = [super initWithStyle:style];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];

    // Uncomment the following line to preserve selection between presentations.
    // self.clearsSelectionOnViewWillAppear = NO;
 
    self.navigationItem.leftBarButtonItem = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemDone target:self action:@selector(clickDone)];
    
    UIBarButtonItem *bbiDelete = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemTrash target:self action:@selector(clickDelete)];
    UIBarButtonItem *bbiAdd = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAdd target:self action:@selector(clickAdd:)];
    self.navigationItem.rightBarButtonItems = [NSArray arrayWithObjects:bbiDelete, bbiAdd, Nil];
  //  self.view.frame = CGRectMake(0, 0, 300, 44 * 20);
    self.tableView.bounces = NO;
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (void)viewWillAppear:(BOOL)animated
{
    g_pIData->OpenLotteryFile();
}

- (void)viewDidAppear:(BOOL)animated
{
    [[self tableView] scrollToRowAtIndexPath:[NSIndexPath indexPathForRow:g_pIData->GetItemCount(DATA_SOURCE_INIT)-1 inSection:0] atScrollPosition:UITableViewScrollPositionTop animated:NO];
}

- (void)viewWillDisappear:(BOOL)animated
{
    g_pIData->CloseLotteryFile(); 
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return YES;
}

#pragma mark - Table view data source
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    // Return the number of rows in the section.
    return g_pIData->GetItemCount(DATA_SOURCE_INIT);
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *cellIdentify = @"DataListCell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellIdentify];
    if (cell == nil)
    {
        cell = [[UITableViewCell alloc]initWithStyle:UITableViewCellStyleSubtitle reuseIdentifier:cellIdentify];
        UIButton *editBtn = [UIButton buttonWithType:UIButtonTypeRoundedRect];
        [editBtn setFrame:CGRectMake(0, 0, 60, 30)];
        [editBtn setTitle:@"修改" forState:UIControlStateNormal];
        [editBtn addTarget:self action:@selector(clickModify:) forControlEvents:UIControlEventTouchUpInside];
        [editBtn setTag:[indexPath row]];
        cell.accessoryView = editBtn;
    }
    
    // 设置标题
    int nIndex = [indexPath row];
    cell.textLabel.text = [NSString stringWithFormat:@"%s期  开奖号：%s", g_pIData->GetItemIssueString(nIndex, DATA_SOURCE_INIT), g_pIData->GetItemNumsString(nIndex, DATA_SOURCE_INIT)];
    
    if (g_pIData->GetLotteryProperty()->btProperty & CSTLPP_TESTNUMS)
    {
        cell.detailTextLabel.text = [NSString stringWithFormat:@"开奖日期：%s  试机号：%s", g_pIData->GetItemDateString(nIndex, DATA_SOURCE_INIT), g_pIData->GetItemTestNumsString(nIndex, DATA_SOURCE_INIT)];
    }
    else
    {
        cell.detailTextLabel.text = [NSString stringWithFormat:@"开奖日期：%s", g_pIData->GetItemDateString(nIndex, DATA_SOURCE_INIT)];

    }
 
    return cell;

}

#pragma mark - Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    // Navigation logic may go here. Create and push another view controller.
    /*
     <#DetailViewController#> *detailViewController = [[<#DetailViewController#> alloc] initWithNibName:@"<#Nib name#>" bundle:nil];
     // ...
     // Pass the selected object to the new view controller.
     [self.navigationController pushViewController:detailViewController animated:YES];
     */
}

#pragma mark - 
- (void)clickDone
{
    if (popoverController != nil)
    {
        [popoverController dismissPopoverAnimated:NO];
    }
    [self dismissModalViewControllerAnimated:YES];
}

- (void)clickDelete
{
    
}

- (void)clickAdd:(id)sender;
{
    [self setupPopoverDataEditVC:-1];
    UIBarButtonItem *bbi = [[[self navigationItem] rightBarButtonItems]objectAtIndex:1];
    [popoverController presentPopoverFromBarButtonItem:bbi permittedArrowDirections:UIPopoverArrowDirectionUp animated:YES];
}

- (void)clickModify:(id)sender;
{
    [self setupPopoverDataEditVC:[sender tag]];
    CGRect rect = [sender bounds];
    [popoverController presentPopoverFromRect:rect inView:sender permittedArrowDirections:UIPopoverArrowDirectionAny animated:YES];
}

- (void)setupPopoverDataEditVC:(NSInteger)dataItemIndex
{
    QCInputDataEditViewController *dataEditVC = [[QCInputDataEditViewController alloc]initWithNibName:@"QCInputDataEditViewController" bundle:nil];
    UINavigationController *navController = [[UINavigationController alloc]initWithRootViewController:dataEditVC];
    dataEditVC.delegate = self;
    dataEditVC.dataItemIndex = dataItemIndex;

   CGSize popoverContentSize = dataEditVC.view.frame.size;
    popoverContentSize.height += 30;
    if (popoverController == nil)
    {
        popoverController = [[UIPopoverController alloc]initWithContentViewController:navController];
        
        [popoverController setPopoverContentSize:popoverContentSize];
    }
}

#pragma mark - QCInputDataEditViewControllerDelegate Method
- (void)inputDataEditVCClose:(BOOL)dataModified
{
    if (dataModified)   // 需要更新数据
    {
        
    }
    
    [popoverController dismissPopoverAnimated:YES];
}
@end
