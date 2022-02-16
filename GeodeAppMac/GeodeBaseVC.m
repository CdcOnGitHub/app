//
//  GeodeBaseVC.m
//  GeodeAppMac
//
//  Created by Full Name on 2/13/22.
//  Copyright © 2022 GeodeSDK. All rights reserved.
//

#import "GeodeBaseVC.h"

@interface GeodeBaseVC ()

@end

@implementation GeodeBaseVC

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.wantsLayer = YES;
    // change the background color of the layer
    
    // 25 26 30
    self.view.layer.backgroundColor = [NSColor colorWithRed:25.0/255.0 green:26.0/255.0 blue:30/255.0 alpha:1].CGColor;
}

@end
