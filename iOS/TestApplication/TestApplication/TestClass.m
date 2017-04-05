//
//  TestClass.m
//  TestApplication
//
//  Created by Maxim Makhun on 3/17/17.
//
//

#import "TestClass.h"

@implementation TestClass

+ (void)handleFirstname:(NSString *)firstname
               lastname:(NSString *)lastname {
    NSLog(@"[%s]: Firstname: %@, Lastname: %@", __FUNCTION__, firstname, lastname);
}

@end
