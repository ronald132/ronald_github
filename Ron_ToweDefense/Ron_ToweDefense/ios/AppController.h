//
//  Ron_ToweDefenseAppController.h
//  Ron_ToweDefense
//
//  Created by Ronald on 22/8/12.
//  Copyright __MyCompanyName__ 2012. All rights reserved.
//

@class RootViewController;

@interface AppController : NSObject <UIAccelerometerDelegate, UIAlertViewDelegate, UITextFieldDelegate,UIApplicationDelegate> {
    UIWindow *window;
    RootViewController    *viewController;
}

@end

