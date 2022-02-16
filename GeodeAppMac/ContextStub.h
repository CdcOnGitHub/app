//
//  ContextStub.h
//  GeodeAppMac
//
//  Created by Full Name on 2/15/22.
//  Copyright © 2022 GeodeSDK. All rights reserved.
//

#import <CoreData/CoreData.h>
#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface ContextStub : NSManagedObject
@property (nonatomic) NSString* installPath;
@property (nonatomic) NSString* name;
@end

NS_ASSUME_NONNULL_END
