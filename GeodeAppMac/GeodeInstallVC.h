//
//  ViewController.h
//  GeodeInstallerMac
//
//  Created by Full Name on 2/3/22.
//  Copyright © 2022 GeodeSDK. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "GeodeBaseVC.h"

@interface GeodeInstallVC : NSViewController
@property (atomic) NSString* instructionText;
@property (weak) IBOutlet NSTextField *instructionField;
@property (weak) IBOutlet NSButton *installButton;
@end

