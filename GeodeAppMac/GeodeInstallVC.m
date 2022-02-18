//
//  ViewController.m
//  GeodeInstallerMac
//
//  Created by Camila Weaver on 2/3/22.
//  Copyright © 2022 GeodeSDK. All rights reserved.
//

#import "GeodeInstallVC.h"
#import "AppDelegate.h"

@implementation GeodeInstallVC

- (void)updateInstruction: (NSString*)inst {
    
    if (![inst isEqualToString:self.instructionText]) {
        dispatch_async(dispatch_get_main_queue(), ^(void) {
            self.instructionField.stringValue = inst;
            self.instructionText = inst;
            if ([inst isEqualToString:@""]) {
                self.installButton.enabled = YES;
            } else {
                self.installButton.enabled = NO;
            }
        });
    }
}

- (NSString*)gdStatusCheck:(NSObject* _Nullable)stub {
    NSWorkspace* ws = [NSWorkspace sharedWorkspace];
    
    while (true) {
        // holy shit infinite loop
        NSArray<NSRunningApplication*>* gd = [ws.runningApplications filteredArrayUsingPredicate:[NSPredicate predicateWithBlock:^BOOL(NSRunningApplication* ra, NSDictionary* bindings) {
            return [ra.localizedName isEqualToString:@"Geometry Dash"];
        }]];
        
        if (gd.count == 0) {
            [self updateInstruction: @"Open your GD instance"];
        } else if (gd.count > 1) {
            [self updateInstruction: @"Close one or more GD instances"];
        } else {
            NSString* path = gd[0].bundleURL.path;
            //NSLog(@"the %@", path);
            bool b = false;
            
            NSArray* badIdea;
            if (stub == nil)
                badIdea = NSAppDel.contextList;
            else
                badIdea = ((AppDelegate*)stub).contextList;
            
            for (ContextStub* c in badIdea) {
                if ([c.installPath isEqualToString:path]) {
                    [self updateInstruction:@"A context already exists for this instance"];
                    b = true;
                }
            }
            if (!b) {
                [self updateInstruction: @""];
                if (stub == nil)
                    return path;
            }
        }
        
        if (stub == nil)
            return nil;
    }
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self performSelectorInBackground: @selector(gdStatusCheck:) withObject: NSAppDel];
}

- (IBAction)onInstall:(id)sender {
    
    NSString* path = [self gdStatusCheck: nil];
    if (path == nil) return;
    
    ContextStub* c = [NSAppDel createContext];
    c.installPath = path;
    c.name = @"Geometry Dash";
    
    NSLog(@"sex");
    
    [NSAppDel refreshContexts];
    [[NSNotificationCenter defaultCenter] postNotificationName:@"ReloadContexts" object:self];
    [self dismissController:nil];
}

@end
