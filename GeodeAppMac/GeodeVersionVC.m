//
//  GeodeVersionVC.m
//  GeodeAppMac
//
//  Created by Full Name on 2/18/22.
//  Copyright © 2022 GeodeSDK. All rights reserved.
//

#import "GeodeVersionVC.h"

@interface GeodeVersionVC ()

@end

@implementation GeodeVersionVC

- (void)viewDidLoad {
    [super viewDidLoad];

    type = (VersionType)((NSTabViewController*)self.parentViewController).selectedTabViewItemIndex;

    switch (type) {
        case kGeodeLoader:
            self.manageLabel.stringValue = @"Manage Loader Version";
            break;
        case kGeodeAPI:
            self.manageLabel.stringValue = @"Manage API Version";
            break;
    }
}

@end
