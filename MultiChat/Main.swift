//
//  Main.swift
//  MultiChat
//
//  Created by vlad on 29.11.2018.
//  Copyright © 2018 vlad. All rights reserved.
//

import Cocoa

class Main: NSViewController {

    @IBOutlet weak var imageOut: NSImageView!

    @IBOutlet weak var buttonClient: NSButton!
    @IBOutlet weak var buttonServer: NSButton!
    @IBAction func serverSelected(_ sender: Any) {
        selectingServer(button: sender)
    }
    @IBAction func clientSelected(_ sender: Any) {
        selectingClient(button: sender)
    }
    var sound:NSSound!
    override func viewDidLoad() {
        super.viewDidLoad()
        self.view.layer = CALayer()
        self.view.layer?.contentsGravity = kCAGravityResizeAspectFill;
        self.view.layer?.contents = NSImage(named: "image1")
        self.view.wantsLayer = true;
        let path = Bundle.main.path(forResource: "sound", ofType: "mp3")
        sound = NSSound(contentsOfFile: path! , byReference: false)
        buttonServer.sound = sound
    }
    
    func selectingServer(button:Any){
    performSegue(withIdentifier: "to server", sender: button)
    }
    
    func selectingClient(button:Any){
    performSegue(withIdentifier: "to client", sender: button)
    }
    
}

