#import <UIKit/UIKit.h>
#import <GooglePlus/GooglePlus.h>
#import <GooglePlayGames/GooglePlayGames.h>

@class RootViewController;

@interface AppController : NSObject <UIApplicationDelegate, UIAccelerometerDelegate, UIAlertViewDelegate, UITextFieldDelegate, GPPSignInDelegate> {
  UIWindow *window;
  RootViewController    *viewController;
}

@property (nonatomic, retain) UIWindow *window;
@property (nonatomic, retain) RootViewController *viewController;

@end