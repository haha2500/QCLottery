//
//  QCAppDelegate.m
//  QCLottery
//
//  Created by 武 孙 on 12-5-9.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#import "QCAppDelegate.h"
#import "QCMainTabBarController.h"
#import "Data.h"
#import "CstPubFunc.h"
#import "ConditionMan.h"

// 全局变量
IData *g_pIData = NULL;
ICstPubFunc *g_pICstPubFunc = NULL;
IConditionMan *g_pIConditionMan = NULL;

@implementation QCAppDelegate

@synthesize window = _window;

- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions
{
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]];
    // Override point for customization after application launch.
    
    // 创建主TAB页界面
    mainTabBarController = [[QCMainTabBarController alloc] init];
    
    // 创建导航视图控制并设为主视图控制器
    UINavigationController *navController = [[UINavigationController alloc] initWithRootViewController:mainTabBarController];
    
    [[self window] setRootViewController:navController];
    [self.window makeKeyAndVisible];
    return YES;
}

- (void)applicationWillResignActive:(UIApplication *)application
{
    // Sent when the application is about to move from active to inactive state. This can occur for certain types of temporary interruptions (such as an incoming phone call or SMS message) or when the user quits the application and it begins the transition to the background state.
    // Use this method to pause ongoing tasks, disable timers, and throttle down OpenGL ES frame rates. Games should use this method to pause the game.
}

- (void)applicationDidEnterBackground:(UIApplication *)application
{
    // Use this method to release shared resources, save user data, invalidate timers, and store enough application state information to restore your application to its current state in case it is terminated later. 
    // If your application supports background execution, this method is called instead of applicationWillTerminate: when the user quits.
}

- (void)applicationWillEnterForeground:(UIApplication *)application
{
    // Called as part of the transition from the background to the inactive state; here you can undo many of the changes made on entering the background.
}

- (void)applicationDidBecomeActive:(UIApplication *)application
{
    // Restart any tasks that were paused (or not yet started) while the application was inactive. If the application was previously in the background, optionally refresh the user interface.
}

- (void)applicationWillTerminate:(UIApplication *)application
{
    // Called when the application is about to terminate. Save data if appropriate. See also applicationDidEnterBackground:.
}

// 覆盖初始化函数，以设置缺省配置
+ (void)initialize
{
    // 初始化缺省配置
    NSString *filePath = [[NSBundle mainBundle]bundlePath];
    NSString *fileName = [filePath stringByAppendingPathComponent:@"Config.plist"];
    NSDictionary *dictionary = [NSDictionary dictionaryWithContentsOfFile:fileName];
    [[NSUserDefaults standardUserDefaults] registerDefaults:dictionary];
    
    // 创建全局公共对象指针
    g_pICstPubFunc = (ICstPubFunc *)new CCstPubFunc;
    
    // 创建数据对象指针，并载入数据
    g_pIData = (IData *)new CData;
    g_pIData->OpenLotteryFile();
    g_pIData->ResetCurUsedData();
    g_pIData->CloseLotteryFile();
    
    // 创建条件管理对象指针
    g_pIConditionMan = (IConditionMan *)new CConditionMan;
}
@end
