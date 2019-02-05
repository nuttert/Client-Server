//
//  ClientViewController.swift
//  MultiChat
//
//  Created by vlad on 29.11.2018.
//  Copyright © 2018 vlad. All rights reserved.
//

import Cocoa

class ClientViewController: NSViewController {
    static var referenceToSelf = [ClientViewController]()
    static var amountOfClient:Int = 0
    var clientNumber:Int!
    @IBOutlet weak var printerTextOutlet: NSTextField!
    @IBAction func printerText(_ sender: Any) {
    }
    var amountOfString = 0
    @IBOutlet weak var myText: NSTextField!
    
  var userNumber:Int! = 0{
        didSet{
            DispatchQueue.main.async {
                self.myText.stringValue +=  "User \(String(describing: self.userNumber)):"
            }
        }
    }
    
    
    var labelText:NSString = "Hello"{
        didSet{
             amountOfString += 1    
            DispatchQueue.main.async { [oldValue] in
                let myTextInString = String(self.labelText)
                if ((self.amountOfString > 5 )||(oldValue == "Hello" )){
                    self.myText.stringValue = ""
                    self.amountOfString = 0
                }
                self.myText.stringValue +=  myTextInString
            }
        }
    }
    
    
   
    
    override func viewDidLoad() {
        super.viewDidLoad()
        self.view.layer = CALayer()
        self.view.layer?.contentsGravity = kCAGravityResizeAspectFill;
        self.view.layer?.contents = NSImage(named: "image3")
        self.view.wantsLayer = true;
        ClientViewController.amountOfClient += 1
        ClientViewController.referenceToSelf.append(self)
        clientNumber = ClientViewController.amountOfClient
        myText.maximumNumberOfLines = 5
        startClientProcessing(Int32(ClientViewController.amountOfClient))
    }
    
}

extension ClientViewController : NSTextFieldDelegate{

/*
    override func controlTextDidChange(_ obj: Notification) {
        let amountOfPoint = arc4random_uniform(4)
        var points = String()
        for _ in 0...amountOfPoint {
            points += "."
        }
        commandFromUIToClientModel("Clients are printing"+points ,Int32(clientNumber!))
    }
    */
    override func controlTextDidEndEditing(_ obj: Notification) {
        commandFromUIToClientModel(printerTextOutlet.stringValue,Int32(clientNumber!))
        printerTextOutlet.stringValue = "";
    }
    
    
    
}
