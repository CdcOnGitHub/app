//
//  InstallViewController.m
//  GeodeInstallerMac
//
//  Created by Full Name on 2/3/22.
//  Copyright © 2022 GeodeSDK. All rights reserved.
//

#import "InstallViewController.h"
#import "AppDelegate.h"

@interface InstallViewController ()

@end

@implementation InstallViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do view setup here.
    /*NSURL* path_ = [((AppDelegate*)NSApp.delegate).gdPath URLByAppendingPathComponent:@"Contents" isDirectory:YES];
    NSURL* path = [path_ URLByAppendingPathComponent:@"MacOS" isDirectory:YES];
    NSLog(@"%@", ((AppDelegate*)NSApp.delegate).gdPath);*/
    
    //[GTRepository clone]
}

- (IBAction)close:(id)sender {
    [[NSApplication sharedApplication] terminate:nil];
}

@end
