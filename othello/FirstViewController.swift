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
    var streng: Int32 = 2
    
    override func viewDidLoad()
    {
        super.viewDidLoad()
        // Do view setup here.
        strength_txt.stringValue = "オセロ初心者向け"
    }
    
    @IBAction func select_strength(_ sender: NSSegmentedControl) {
        switch sender.selectedSegment {
        case 0:
            streng = 1
            strength_txt.stringValue = "オセロのルールだけ知ってる人向け"
        case 1:
            streng = 2
            strength_txt.stringValue = "オセロ初心者向け"
        case 2:
            streng = 5
            strength_txt.stringValue = "オセロできるぜ！という人向け"
        case 3:
            streng = 8
            strength_txt.stringValue = "オセロに少し自信がある人向け"
        case 4:
            streng = 10
            strength_txt.stringValue = "オセロに自信がある人向け"
        case 5:
            streng = 11
            strength_txt.stringValue = "٩( ᐛ )و"
        default:
            streng = 10
        }
    }
    
    @IBAction func tapstart(_ sender: Any)
    {
        performSegue(withIdentifier: "start", sender: self)
    }
    
    override func prepare(for segue: NSStoryboardSegue, sender: Any?) {
        if segue.identifier == "start" {
            if let nextViewController = segue.destinationController as? ViewController {
                // ここでデータをセットアップ
                nextViewController.strength = streng
            }
        }
    }
}
