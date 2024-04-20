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
        strength_txt.stringValue = "初心者よりはできる人向け向け"
        botplayer = WHITE_TURN
        firstDEPTH = 3
    }
    
    @IBAction func select_strength(_ sender: NSSegmentedControl) {
        switch sender.selectedSegment {
        case 0:
            firstDEPTH = 1
            strength_txt.stringValue = "初心者向け"
        case 1:
            firstDEPTH = 3
            strength_txt.stringValue = "初心者よりはできる人向け"
        case 2:
            firstDEPTH = 4
            strength_txt.stringValue = "ある程度できる人向け"
        case 3:
            firstDEPTH = 8
            strength_txt.stringValue = "少し自信がある人向け"
        case 4:
            firstDEPTH = 10
            strength_txt.stringValue = "自信がある人向け"
        default:
            firstDEPTH = 10
        }
    }
    
    @IBAction func select_player(_ sender: NSSegmentedControl) {
        switch sender.selectedSegment {
        case 0:
            botplayer = BLACK_TURN
        case 1:
            botplayer = WHITE_TURN
        default:
            botplayer = WHITE_TURN
        }
    }
    
    @IBAction func tapstart(_ sender: Any)
    {
        performSegue(withIdentifier: "start", sender: self)
    }
}
