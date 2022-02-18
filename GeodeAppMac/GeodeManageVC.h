//
//  GeodeManageVC.h
//  GeodeAppMac
//
//  Created by Full Name on 2/16/22.
//  Copyright © 2022 GeodeSDK. All rights reserved.
//

#import "GeodeBaseVC.h"

NS_ASSUME_NONNULL_BEGIN

@interface GeodeManageVC : GeodeBaseVC <NSTableViewDataSource, NSTableViewDelegate>
@property (weak) IBOutlet NSTableView *logTable;
@property (weak) IBOutlet NSOutlineView *modTable;
@property (nonatomic, retain) NSString* geodePath;

- (void)openLog:(id)nid;
@end

NS_ASSUME_NONNULL_END
