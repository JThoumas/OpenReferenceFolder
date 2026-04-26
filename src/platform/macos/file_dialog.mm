#include "platform/file_dialog.h"
#import <AppKit/AppKit.h>
#import <Foundation/Foundation.h>

namespace orf::platform {

std::vector<std::string> openFileDialog(const std::string& title, bool multiSelect) {
    std::vector<std::string> result;
    
    NSOpenPanel* panel = [NSOpenPanel openPanel];
    [panel setTitle:[NSString stringWithUTF8String:title.c_str()]];
    [panel setCanChooseFiles:YES];
    [panel setCanChooseDirectories:NO];
    [panel setAllowsMultipleSelection:multiSelect ? YES : NO];
    
    if ([panel runModal] == NSModalResponseOK) {
        for (NSURL* url in [panel URLs]) {
            result.push_back([[url path] UTF8String]);
        }
    }
    
    return result;
}

std::optional<std::string> openFolderDialog(const std::string& title) {
    NSOpenPanel* panel = [NSOpenPanel openPanel];
    [panel setTitle:[NSString stringWithUTF8String:title.c_str()]];
    [panel setCanChooseFiles:NO];
    [panel setCanChooseDirectories:YES];
    [panel setAllowsMultipleSelection:NO];
    
    if ([panel runModal] == NSModalResponseOK) {
        return [[[panel URL] path] UTF8String];
    }
    
    return std::nullopt;
}

} // namespace orf::platform
