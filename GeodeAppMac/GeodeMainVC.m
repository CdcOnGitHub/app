//
//  GeodeMainVC.m
//  GeodeAppMac
//
//  Created by Full Name on 2/14/22.
//  Copyright © 2022 GeodeSDK. All rights reserved.
//

#import "GeodeMainVC.h"
#import "AppDelegate.h"
#import "GeodeManageVC.h"

@interface GeodeMainVC ()

@end

@implementation GeodeMainVC

- (void)viewDidLoad {
    [super viewDidLoad];
    [NSAppDel refreshContexts];
    [self.ContextTable setDataSource: self];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(reloadContexts:) name:@"ReloadContexts" object:nil];
    
    [NSTimer scheduledTimerWithTimeInterval:1.0/60.0
    target:self selector:@selector(updateOnFrame) userInfo:nil repeats:YES];
}

- (void)reloadContexts: (NSNotification*)nt {
    if ([nt.name isEqualToString:@"ReloadContexts"]) {
        [self.ContextTable reloadData];
    }
}

- (NSInteger)numberOfRowsInTableView:(NSTableView *)tableView {
    NSLog(@"the %lu", NSAppDel.contextList.count);
    return NSAppDel.contextList.count;
}

- (NSView *)tableView:(NSTableView *)tableView viewForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row {

 

    // Get an existing cell with the MyView identifier if it exists

    NSTextField *result = [tableView makeViewWithIdentifier:@"MyView" owner:self];

 

    // There is no existing cell to reuse so create a new one

    if (result == nil) {

        result = [[NSTextField alloc] initWithFrame:NSMakeRect(0, 0, self.ContextTable.bounds.size.width, 0)];

        result.identifier = @"MyView";

    }
    result.stringValue = [[NSAppDel.contextList objectAtIndex:row] name];
    
    return result;
}


- (void)updateOnFrame {
    NSInteger row = self.ContextTable.selectedRow;
    if (row == -1 && self.manageContextButton.enabled) {
        self.manageContextButton.enabled = NO;
        //self.manageContextButton.borderNormalColor = [NSColor colorWithRed:293.0/255.0 green:183.0/255.0 blue:120.0/255.0 alpha:0.25];
        //self.manageContextButton.isEnabled = NO;
    } else if (row != -1 && !self.manageContextButton.enabled) {
        self.manageContextButton.enabled = YES;
        //self.manageContextButton.borderNormalColor = [NSColor colorWithRed:193.0/255.0 green:183.0/255.0 blue:120.0/255.0 alpha:1];
        //self.manageContextButton.isEnabled = YES;
    }
}
- (IBAction)openContext:(id)sender {
    GeodeManageVC* v = (GeodeManageVC*)[[NSStoryboard storyboardWithName:@"Main" bundle:[NSBundle mainBundle]] instantiateControllerWithIdentifier:@"manageVc"];

    [v setCtx:[NSAppDel.contextList objectAtIndex: self.ContextTable.selectedRow]];
    
    [self presentViewControllerAsSheet:v];
}

@end
