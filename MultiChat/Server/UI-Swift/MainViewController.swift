//
//  MainViewController.swift
//  Chat
//
//  Created by vlad on 29.11.2018.
//  Copyright © 2018 vlad. All rights reserved.
//

import Cocoa

class MainViewController: NSViewController {
    static var referenceToSelf:MainViewController? = nil
    
    @IBOutlet weak var imageOut: NSImageView!
    @IBOutlet weak var myText: NSTextField!
        var amountOfString = 0
    var labelText:NSString = "Hello"{
        didSet{
            if (labelText == "Hello"){
                labelText = ""
            }
            amountOfString += 1
            DispatchQueue.main.async { 
                if (self.amountOfString > 7 ){
                    self.myText.stringValue = ""
                    self.amountOfString = 0
                }
                self.myText.stringValue += String(self.labelText);
            }
        }
    }
    override func viewDidLoad() {
        super.viewDidLoad()
        self.view.layer = CALayer()
        self.view.layer?.contentsGravity = kCAGravityResizeAspectFill;
        self.view.layer?.contents = NSImage(named: "image1")
        self.view.wantsLayer = true;
        MainViewController.referenceToSelf = self
        RefreshStructure.startServerProcessing()
    }
    
}



