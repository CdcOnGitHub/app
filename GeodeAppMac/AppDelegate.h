//
//  AppDelegate.h
//  GeodeInstallerMac
//
//  Created by Full Name on 2/3/22.
//  Copyright © 2022 GeodeSDK. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "ContextStub.h"

@interface AppDelegate : NSObject <NSApplicationDelegate>

@property (nonatomic, retain) NSManagedObjectModel *managedObjectModel;
@property (nonatomic, retain) NSManagedObjectContext *managedObjectContext;
@property (nonatomic, retain) NSPersistentStoreCoordinator *persistentStoreCoordinator;
@property (nonatomic) NSArray<ContextStub*>* contextList;

- (NSURL *)applicationDocumentsDirectory; // nice to have to reference files for core data

- (void)refreshContexts;
- (ContextStub*)createContext;
@end

#define NSAppDel ((AppDelegate*)[NSApp delegate])
