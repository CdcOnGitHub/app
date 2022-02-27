//
//  GeodeManageVC.m
//  GeodeAppMac
//
//  Created by Full Name on 2/16/22.
//  Copyright © 2022 GeodeSDK. All rights reserved.
//

#import "GeodeManageVC.h"
#import "AppDelegate.h"

@interface GeodeManageVC ()

@end

@implementation GeodeManageVC

- (NSArray*)logs {
    NSString* sourcePath = [self.ctx.installPath stringByAppendingPathComponent: @"Contents/geode/log"];

    NSArray* dirs = [[NSFileManager defaultManager] contentsOfDirectoryAtPath:sourcePath
                                                                        error:NULL];
    NSMutableArray *logFiles = [[NSMutableArray alloc] init];
    [dirs enumerateObjectsUsingBlock:^(id obj, NSUInteger idx, BOOL *stop) {
        NSString *filename = (NSString *)obj;
        NSString *extension = [[filename pathExtension] lowercaseString];
        if ([extension isEqualToString:@"log"]) {
            [logFiles addObject:[sourcePath stringByAppendingPathComponent:filename]];
        }
    }];
    
    return logFiles;
}

- (NSString*)formatLog:(NSString*)logFile {
    NSArray* first = [[[logFile lastPathComponent] stringByDeletingPathExtension] componentsSeparatedByString:@"_"];
    if ([first[0] isEqual: @"Geode"]) {
        NSDate* d = [NSDate dateWithTimeIntervalSince1970:[first[1] doubleValue]];
        NSDateFormatter *f = [[NSDateFormatter alloc] init];
        [f setDateFormat:@"E, H:mm:ss MMM d yyyy"];
        return [f stringFromDate:d];
    } else return logFile;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    self.logTable.dataSource = self;
    self.logTable.delegate = self;
    self.logTable.doubleAction = @selector(openLog:);
    // Do view setup here.
}

- (IBAction)onBack:(id)sender {
    [self dismissController:nil];
}

-(NSInteger)numberOfRowsInTableView: (NSTableView*)tv {
    if (tv == self.logTable) {
        return self.logs.count;
    } else return 0;
}

- (NSView *)tableView:(NSTableView *)tableView viewForTableColumn:(NSTableColumn *)tableColumn row:(NSInteger)row {

    // Get an existing cell with the MyView identifier if it exists

    NSTextField *result = [tableView makeViewWithIdentifier:@"MyView" owner:self];

 

    // There is no existing cell to reuse so create a new one

    if (result == nil) {

        result = [[NSTextField alloc] initWithFrame:NSMakeRect(0, 0, tableView.bounds.size.width, 0)];

        result.identifier = @"MyView";
        result.editable = NO;
        result.drawsBackground = NO;
    }
    
    if (tableView == self.logTable) {
        result.stringValue = [self formatLog:self.logs[row]];
    }
    
    return result;
}

- (void)openLog:(id)nid {
    [[NSWorkspace sharedWorkspace] openFile:self.logs[self.logTable.selectedRow] withApplication:@"Console"];
}
- (IBAction)promptDestroy:(id)sender {
    NSAlert *alert = [[NSAlert alloc] init];
    [alert addButtonWithTitle:@"Cancel"];
    [alert addButtonWithTitle:@"Delete"];
    [alert setMessageText:@"Delete the context?"];
    [alert setInformativeText:@"Are you sure you would like to delete this context?"];
    [alert setAlertStyle:NSWarningAlertStyle];
    if ([alert runModal] == NSAlertSecondButtonReturn) {
        [NSAppDel removeContext:self.ctx];
        [self dismissController:nil];
        [[NSNotificationCenter defaultCenter] postNotificationName:@"ReloadContexts" object:self];
    }
}

@end
