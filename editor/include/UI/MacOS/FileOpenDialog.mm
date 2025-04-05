#include <string>

#import <Cocoa/Cocoa.h>
#include <iostream>

std::string OpenMacFileDialog(bool &open) {

  @autoreleasepool {
    NSOpenPanel *panel = [NSOpenPanel openPanel];
    [panel setCanChooseFiles:YES];
    [panel setCanChooseDirectories:NO];
    [panel setAllowsMultipleSelection:NO];
    [panel setAllowedFileTypes:@[ @"*" ]]; // All filessher

    std::cout << "closing" << std::endl;

    if ([panel runModal] == NSModalResponseOK) {
      NSURL *url = [[panel URLs] firstObject];
      if (url) {
        return std::string([[url path] UTF8String]);
      }
      open = false;
    }
  }

  return "";
}