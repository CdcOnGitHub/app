//
//  GeodeManageVC.h
//  GeodeAppMac
//
//  Created by Full Name on 2/16/22.
//  Copyright © 2022 GeodeSDK. All rights reserved.
//

#import "GeodeBaseVC.h"
#import "ContextStub.h"

NS_ASSUME_NONNULL_BEGIN

@interface GeodeManageVC : GeodeBaseVC <NSTableViewDataSource, NSTableViewDelegate>
@property (weak) IBOutlet NSTableView *logTable;
@property (weak) IBOutlet NSOutlineView *modTable;
@property (atomic) ContextStub* ctx;

- (void)openLog:(id)nid;
@end

NS_ASSUME_NONNULL_END
