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
        if(winner() == 1)
        {
            win_player.stringValue = "黒の勝ち！"
        }
        else if(winner() == 2)
        {
            win_player.stringValue = "白の勝ち！"
        }
        else if(winner() == 0)
        {
            win_player.stringValue = "引き分け"
        }
        let blackcount = countstoneswift(1)
        let whitecount = countstoneswift(2)
        result.stringValue = "黒: \(blackcount) 白: \(whitecount)"
        print("黒: \(blackcount) 白: \(whitecount)")
    }
    
    @IBAction func tapclose(_ sender: Any)
    {
        self.dismiss(self)
    }
}
