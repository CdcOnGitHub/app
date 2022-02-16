//
//  GeodeMainVC.h
//  GeodeAppMac
//
//  Created by Full Name on 2/14/22.
//  Copyright © 2022 GeodeSDK. All rights reserved.
//

#import "GeodeBaseVC.h"
#import "ContextStub.h"
#include "SYFlatButton.h"

NS_ASSUME_NONNULL_BEGIN

@interface GeodeMainVC : GeodeBaseVC <NSTableViewDataSource, NSTableViewDelegate>
@property (weak) IBOutlet NSTableView *ContextTable;
@property (weak) IBOutlet NSButton *manageContextButton;

-(void)updateOnFrame;

@end

NS_ASSUME_NONNULL_END
