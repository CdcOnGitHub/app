//
//  GeodeVersionVC.h
//  GeodeAppMac
//
//  Created by Full Name on 2/18/22.
//  Copyright © 2022 GeodeSDK. All rights reserved.
//

#import "GeodeBaseVC.h"

NS_ASSUME_NONNULL_BEGIN

typedef enum VersionType {
    kGeodeLoader,
    kGeodeAPI
} VersionType;

@interface GeodeVersionVC : GeodeBaseVC {
    VersionType type;
}
@property (weak) IBOutlet NSTextField *manageLabel;
@end

NS_ASSUME_NONNULL_END
