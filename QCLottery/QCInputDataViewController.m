//
//  QCInputDataViewController.m
//  QCLottery
//
//  Created by 武 孙 on 12-5-15.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "QCInputDataViewController.h"

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
    UIBarButtonItem *bbiAdd = [[UIBarButtonItem alloc] initWithBarButtonSystemItem:UIBarButtonSystemItemAdd target:self action:@selector(clickAdd)];
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

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return YES;
}

#pragma mark - Table view data source
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    // Return the number of rows in the section.
    return 10;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *cellIdentify = @"DataListCell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:cellIdentify];
    if (cell == nil)
    {
        cell = [[UITableViewCell alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:cellIdentify];
       // cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;
        UIButton *editBtn = [UIButton buttonWithType:UIButtonTypeRoundedRect];
        [editBtn setFrame:CGRectMake(0, 0, 60, 30)];
        [editBtn setTitle:@"修改" forState:UIControlStateNormal];
        [editBtn addTarget:self action:@selector(clickModify) forControlEvents:UIControlEventTouchUpInside];
        cell.accessoryView = editBtn;
    }
    
    // 设置标题
  //  int nIndex = [indexPath row];
    cell.textLabel.text = @"2010-01-01 2010001 222 333"; // [cmdArray objectAtIndex:nIndex];
    
    // 设置详细信息
   // cell.detailTextLabel.text = [self cmdDetailText:nIndex];
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
    [self dismissModalViewControllerAnimated:YES];
}

- (void)clickDelete
{
    
}

- (void)clickAdd
{
    
}

- (void)clickModify
{
    
}

@end
