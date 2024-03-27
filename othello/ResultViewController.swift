//
//  ResultViewController.swift
//  othello
//
//  Created by sasa on 2023/10/08.
//

import Cocoa
import Foundation

class ResultViewController: NSViewController
{
    @IBOutlet var result: NSTextField!
    @IBOutlet var win_player: NSTextField!
    
    override func viewDidLoad()
    {
        super.viewDidLoad()
        switch winner() {
        case 1:
            win_player.stringValue = "黒の勝ち！"
        case 2:
            win_player.stringValue = "白の勝ち！"
        default:
            win_player.stringValue = "引き分け"
        }
        result.stringValue = "黒: \(countstoneswift(1)) 白: \(countstoneswift(2))"
    }
    
    @IBAction func tapclose(_ sender: Any)
    {
        self.dismiss(self)
    }
}
