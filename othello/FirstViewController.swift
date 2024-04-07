//
//  FirstViewController.swift
//  othello
//
//  Created by sasa on 2023/10/08.
//

import Cocoa
import Foundation

class FirstViewController: NSViewController
{
    @IBOutlet var strength_select: NSSegmentedControl!
    @IBOutlet var strength_txt: NSTextField!
    @IBOutlet var selectplayer: NSSegmentedControl!
    
    var streng: Int32 = 2
    var player_bot: Int32 = WHITE_TURN
    
    override func viewDidLoad()
    {
        super.viewDidLoad()
        // Do view setup here.
        strength_txt.stringValue = "初心者よりはできる人向け向け"
        DEPTH = 2
    }
    
    @IBAction func select_strength(_ sender: NSSegmentedControl) {
        switch sender.selectedSegment {
        case 0:
            DEPTH = 1
            strength_txt.stringValue = "初心者向け"
        case 1:
            DEPTH = 3
            strength_txt.stringValue = "初心者よりはできる人向け"
        case 2:
            DEPTH = 4
            strength_txt.stringValue = "ある程度できる人向け"
        case 3:
            DEPTH = 8
            strength_txt.stringValue = "少し自信がある人向け"
        case 4:
            DEPTH = 10
            strength_txt.stringValue = "自信がある人向け"
        default:
            DEPTH = 10
        }
    }
    
    @IBAction func select_player(_ sender: NSSegmentedControl) {
        switch sender.selectedSegment {
        case 0:
            player_bot = BLACK_TURN
        case 1:
            player_bot = WHITE_TURN
        default:
            player_bot = WHITE_TURN
        }
    }
    
    @IBAction func tapstart(_ sender: Any)
    {
        performSegue(withIdentifier: "start", sender: self)
    }
    
    override func prepare(for segue: NSStoryboardSegue, sender: Any?) {
        if segue.identifier == "start" {
            if let nextViewController = segue.destinationController as? ViewController {
                nextViewController.strength = streng
                nextViewController.playerbot = player_bot
            }
        }
    }
}
