//
//  WindowController.swift
//  MultiChat
//
//  Created by vlad on 29.11.2018.
//  Copyright © 2018 vlad. All rights reserved.
//

import Cocoa

class WindowController: NSWindowController {
    static var numberOfController = 1;
    override func windowDidLoad() {
        super.windowDidLoad()
    }
    required init?(coder: NSCoder) {
        super.init(coder: coder)
        window?.setFrameOrigin(NSPoint(x: 0, y: 0))
    }

}
