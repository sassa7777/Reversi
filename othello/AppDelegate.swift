//
//  AppDelegate.swift
//  othello
//
//  Created by sasa on 2023/09/30.
//

import Cocoa

@main
class AppDelegate: NSObject, NSApplicationDelegate {

    


    func applicationDidFinishLaunching(_ aNotification: Notification) 
    {
        // Insert code here to initialize your application
        
    }

    func applicationWillTerminate(_ aNotification: Notification) 
    {
        // Insert code here to tear down your application
    }

    func applicationSupportsSecureRestorableState(_ app: NSApplication) -> Bool 
    {
        return true
    }

    func applicationShouldHandleReopen(_ sender: NSApplication, hasVisibleWindows flag: Bool) -> Bool {
        if(!flag){
            for window in sender.windows{
                window.makeKeyAndOrderFront(self);
            }
        }
        return true;
    }
    
}

