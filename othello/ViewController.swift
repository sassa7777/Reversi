//
//  ViewController.swift
//  othello
//
//  Created by sasa on 2023/09/30.
//

import Cocoa
import Foundation


//let y = 8
//let x = 8
//
//

class ViewController: NSViewController
{
    @IBOutlet var aa: NSButton!
    @IBOutlet var ab: NSButton!
    @IBOutlet var ac: NSButton!
    @IBOutlet var ad: NSButton!
    @IBOutlet var ae: NSButton!
    @IBOutlet var af: NSButton!
    @IBOutlet var ag: NSButton!
    @IBOutlet var ah: NSButton!
    @IBOutlet var ba: NSButton!
    @IBOutlet var bb: NSButton!
    @IBOutlet var bc: NSButton!
    @IBOutlet var bd: NSButton!
    @IBOutlet var be: NSButton!
    @IBOutlet var bf: NSButton!
    @IBOutlet var bg: NSButton!
    @IBOutlet var bh: NSButton!
    @IBOutlet var ca: NSButton!
    @IBOutlet var cb: NSButton!
    @IBOutlet var cc: NSButton!
    @IBOutlet var cd: NSButton!
    @IBOutlet var ce: NSButton!
    @IBOutlet var cf: NSButton!
    @IBOutlet var cg: NSButton!
    @IBOutlet var ch: NSButton!
    @IBOutlet var da: NSButton!
    @IBOutlet var db: NSButton!
    @IBOutlet var dc: NSButton!
    @IBOutlet var dd: NSButton!
    @IBOutlet var de: NSButton!
    @IBOutlet var df: NSButton!
    @IBOutlet var dg: NSButton!
    @IBOutlet var dh: NSButton!
    @IBOutlet var ea: NSButton!
    @IBOutlet var eb: NSButton!
    @IBOutlet var ec: NSButton!
    @IBOutlet var ed: NSButton!
    @IBOutlet var ee: NSButton!
    @IBOutlet var ef: NSButton!
    @IBOutlet var eg: NSButton!
    @IBOutlet var eh: NSButton!
    @IBOutlet var fa: NSButton!
    @IBOutlet var fb: NSButton!
    @IBOutlet var fc: NSButton!
    @IBOutlet var fd: NSButton!
    @IBOutlet var fe: NSButton!
    @IBOutlet var ff: NSButton!
    @IBOutlet var fg: NSButton!
    @IBOutlet var fh: NSButton!
    @IBOutlet var ga: NSButton!
    @IBOutlet var gb: NSButton!
    @IBOutlet var gc: NSButton!
    @IBOutlet var gd: NSButton!
    @IBOutlet var ge: NSButton!
    @IBOutlet var gf: NSButton!
    @IBOutlet var gg: NSButton!
    @IBOutlet var gh: NSButton!
    @IBOutlet var ha: NSButton!
    @IBOutlet var hb: NSButton!
    @IBOutlet var hc: NSButton!
    @IBOutlet var hd: NSButton!
    @IBOutlet var he: NSButton!
    @IBOutlet var hf: NSButton!
    @IBOutlet var hg: NSButton!
    @IBOutlet var hh: NSButton!
    @IBOutlet var close: NSButton!

    @IBAction func aa(_ sender: Any)
    {
        if(putstone(1, 1) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func ab(_ sender: Any)
    {
        if(putstone(1, 2) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func ac(_ sender: Any)
    {
        if(putstone(1, 3) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func ad(_ sender: Any)
    {
        if(putstone(1, 4) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func ae(_ sender: Any)
    {
        if(putstone(1, 5) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func af(_ sender: Any)
    {
        if(putstone(1, 6) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func ag(_ sender: Any)
    {
        if(putstone(1, 7) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func ah(_ sender: Any)
    {
        if(putstone(1, 8) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func ba(_ sender: Any)
    {
        if(putstone(2, 1) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func bb(_ sender: Any)
    {
        if(putstone(2, 2) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func bc(_ sender: Any)
    {
        if(putstone(2, 3) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func bd(_ sender: Any)
    {
        if(putstone(2, 4) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func be(_ sender: Any)
    {
        if(putstone(2, 5) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func bf(_ sender: Any)
    {
        if(putstone(2, 6) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func bg(_ sender: Any)
    {
        if(putstone(2, 7) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func bh(_ sender: Any)
    {
        if(putstone(2, 8) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func ca(_ sender: Any)
    {
        if(putstone(3, 1) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func cb(_ sender: Any)
    {
        if(putstone(3, 2) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func cc(_ sender: Any)
    {
        if(putstone(3, 3) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func cd(_ sender: Any)
    {
        if(putstone(3, 4) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func ce(_ sender: Any)
    {
        if(putstone(3, 5) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func cf(_ sender: Any)
    {
        if(putstone(3, 6) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func cg(_ sender: Any)
    {
        if(putstone(3, 7) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func ch(_ sender: Any)
    {
        if(putstone(3, 8) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func da(_ sender: Any)
    {
        if(putstone(4, 1) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func db(_ sender: Any)
    {
        if(putstone(4, 2) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func dc(_ sender: Any)
    {
        if(putstone(4, 3) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func dd(_ sender: Any)
    {
        if(putstone(4, 4) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func de(_ sender: Any)
    {
        if(putstone(4, 5) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func df(_ sender: Any)
    {
        if(putstone(4, 6) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func dg(_ sender: Any)
    {
        if(putstone(4, 7) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func dh(_ sender: Any)
    {
        if(putstone(4, 8) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func ea(_ sender: Any)
    {
        if(putstone(5, 1) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func eb(_ sender: Any)
    {
        if(putstone(5, 2) != 0)
        {
            reloadview()
            putai()
        }
        
    }
    @IBAction func ec(_ sender: Any)
    {
        if(putstone(5, 3) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func ed(_ sender: Any)
    {
        if(putstone(5, 4) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func ee(_ sender: Any)
    {
        if(putstone(5, 5) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func ef(_ sender: Any)
    {
        if(putstone(5, 6) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func eg(_ sender: Any)
    {
        if(putstone(5, 7) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func eh(_ sender: Any)
    {
        if(putstone(5, 8) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func fa(_ sender: Any)
    {
        if(putstone(6, 1) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func fb(_ sender: Any)
    {
        if(putstone(6, 2) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func fc(_ sender: Any)
    {
        if(putstone(6, 3) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func fd(_ sender: Any)
    {
        if(putstone(6, 4) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func fe(_ sender: Any)
    {
        if(putstone(6, 5) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func ff(_ sender: Any)
    {
        if(putstone(6, 6) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func fg(_ sender: Any)
    {
        if(putstone(6, 7) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func fh(_ sender: Any)
    {
        if(putstone(6, 8) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func ga(_ sender: Any)
    {
        if(putstone(7, 1) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func gb(_ sender: Any)
    {
        if(putstone(7, 2) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func gc(_ sender: Any)
    {
        if(putstone(7, 3) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func gd(_ sender: Any)
    {
        if(putstone(7, 4) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func ge(_ sender: Any)
    {
        if(putstone(7, 5) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func gf(_ sender: Any)
    {
        if(putstone(7, 6) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func gg(_ sender: Any)
    {
        if(putstone(7, 7) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func gh(_ sender: Any)
    {
        if(putstone(7, 8) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func ha(_ sender: Any)
    {
        if(putstone(8, 1) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func hb(_ sender: Any)
    {
        if(putstone(8, 2) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func hc(_ sender: Any)
    {
        if(putstone(8, 3) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func hd(_ sender: Any)
    {
        if(putstone(8, 4) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func he(_ sender: Any)
    {
        if(putstone(8, 5) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func hf(_ sender: Any)
    {
        if(putstone(8, 6) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func hg(_ sender: Any)
    {
        if(putstone(8, 7) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func hh(_ sender: Any)
    {
        if(putstone(8, 8) != 0)
        {
            reloadview()
            putai()
        }
    }
    @IBAction func close(_ sender: Any)
    {
        self.dismiss(self)
    }
    
    
    override func viewDidLoad() 
    {
        close.isHidden = true
        reset()
        reloadview()
        //print(board.3.3)
    }
    
    func putai()
    {
        DispatchQueue.main.asyncAfter(deadline: .now() + 0.2)
        {
            ai2()
            self.reloadview()
        }
    }
    
    func result()
    {
        if winner() == 1
        {
            print("黒の勝ち！")
        }
        else if winner() == 2
        {
            print("白の勝ち！")
        }
        else
        {
            print("引き分け")
        }
    }
    
    func reloadview()
    {
        rebuild()
        print("[*]場面を再構築中...")
        for i in 1..<9 {
            for j in 1..<9 {
                switch i
                {
                case 1:
                    switch j
                    {
                    case 1:
                        switch board.1.1
                        {
                        case 1:
                            aa.image = NSImage(named: "black")
                        case 2:
                            aa.image = NSImage(named: "white")
                        default:
                            aa.image = NSImage(named: "null")
                        }
                    case 2:
                        switch board.1.2
                        {
                        case 1:
                            ab.image = NSImage(named: "black")
                        case 2:
                            ab.image = NSImage(named: "white")
                        default:
                            ab.image = NSImage(named: "null")
                        }
                    case 3:
                        switch board.1.3
                        {
                        case 1:
                            ac.image = NSImage(named: "black")
                        case 2:
                            ac.image = NSImage(named: "white")
                        default:
                            ac.image = NSImage(named: "null")
                        }
                    case 4:
                        switch board.1.4
                        {
                        case 1:
                            ad.image = NSImage(named: "black")
                        case 2:
                            ad.image = NSImage(named: "white")
                        default:
                            ad.image = NSImage(named: "null")
                        }
                    case 5:
                        switch board.1.5
                        {
                        case 1:
                            ae.image = NSImage(named: "black")
                        case 2:
                            ae.image = NSImage(named: "white")
                        default:
                            ae.image = NSImage(named: "null")
                        }
                    case 6:
                        switch board.1.6
                        {
                        case 1:
                            af.image = NSImage(named: "black")
                        case 2:
                            af.image = NSImage(named: "white")
                        default:
                            af.image = NSImage(named: "null")
                        }
                    case 7:
                        switch board.1.7
                        {
                        case 1:
                            ag.image = NSImage(named: "black")
                        case 2:
                            ag.image = NSImage(named: "white")
                        default:
                            ag.image = NSImage(named: "null")
                        }
                    case 8:
                        switch board.1.8
                        {
                        case 1:
                            ah.image = NSImage(named: "black")
                        case 2:
                            ah.image = NSImage(named: "white")
                        default:
                            ah.image = NSImage(named: "null")
                        }
                    default:
                        continue
                    }
                case 2:
                    switch j
                    {
                    case 1:
                        switch board.2.1
                        {
                        case 1:
                            ba.image = NSImage(named: "black")
                        case 2:
                            ba.image = NSImage(named: "white")
                        default:
                            ba.image = NSImage(named: "null")
                        }
                    case 2:
                        switch board.2.2
                        {
                        case 1:
                            bb.image = NSImage(named: "black")
                        case 2:
                            bb.image = NSImage(named: "white")
                        default:
                            bb.image = NSImage(named: "null")
                        }
                    case 3:
                        switch board.2.3
                        {
                        case 1:
                            bc.image = NSImage(named: "black")
                        case 2:
                            bc.image = NSImage(named: "white")
                        default:
                            bc.image = NSImage(named: "null")
                        }
                    case 4:
                        switch board.2.4
                        {
                        case 1:
                            bd.image = NSImage(named: "black")
                        case 2:
                            bd.image = NSImage(named: "white")
                        default:
                            bd.image = NSImage(named: "null")
                        }
                    case 5:
                        switch board.2.5
                        {
                        case 1:
                            be.image = NSImage(named: "black")
                        case 2:
                            be.image = NSImage(named: "white")
                        default:
                            be.image = NSImage(named: "null")
                        }
                    case 6:
                        switch board.2.6
                        {
                        case 1:
                            bf.image = NSImage(named: "black")
                        case 2:
                            bf.image = NSImage(named: "white")
                        default:
                            bf.image = NSImage(named: "null")
                        }
                    case 7:
                        switch board.2.7
                        {
                        case 1:
                            bg.image = NSImage(named: "black")
                        case 2:
                            bg.image = NSImage(named: "white")
                        default:
                            bg.image = NSImage(named: "null")
                        }
                    case 8:
                        switch board.2.8
                        {
                        case 1:
                            bh.image = NSImage(named: "black")
                        case 2:
                            bh.image = NSImage(named: "white")
                        default:
                            bh.image = NSImage(named: "null")
                        }
                    default:
                        continue
                    }
                case 3:
                    switch j
                    {
                    case 1:
                        switch board.3.1
                        {
                        case 1:
                            ca.image = NSImage(named: "black")
                        case 2:
                            ca.image = NSImage(named: "white")
                        default:
                            ca.image = NSImage(named: "null")
                        }
                    case 2:
                        switch board.3.2
                        {
                        case 1:
                            cb.image = NSImage(named: "black")
                        case 2:
                            cb.image = NSImage(named: "white")
                        default:
                            cb.image = NSImage(named: "null")
                        }
                    case 3:
                        switch board.3.3
                        {
                        case 1:
                            cc.image = NSImage(named: "black")
                        case 2:
                            cc.image = NSImage(named: "white")
                        default:
                            cc.image = NSImage(named: "null")
                        }
                    case 4:
                        switch board.3.4
                        {
                        case 1:
                            cd.image = NSImage(named: "black")
                        case 2:
                            cd.image = NSImage(named: "white")
                        default:
                            cd.image = NSImage(named: "null")
                        }
                    case 5:
                        switch board.3.5
                        {
                        case 1:
                            ce.image = NSImage(named: "black")
                        case 2:
                            ce.image = NSImage(named: "white")
                        default:
                            ce.image = NSImage(named: "null")
                        }
                    case 6:
                        switch board.3.6
                        {
                        case 1:
                            cf.image = NSImage(named: "black")
                        case 2:
                            cf.image = NSImage(named: "white")
                        default:
                            cf.image = NSImage(named: "null")
                        }
                    case 7:
                        switch board.3.7
                        {
                        case 1:
                            cg.image = NSImage(named: "black")
                        case 2:
                            cg.image = NSImage(named: "white")
                        default:
                            cg.image = NSImage(named: "null")
                        }
                    case 8:
                        switch board.3.8
                        {
                        case 1:
                            ch.image = NSImage(named: "black")
                        case 2:
                            ch.image = NSImage(named: "white")
                        default:
                            ch.image = NSImage(named: "null")
                        }
                    default:
                        continue
                    }
                case 4:
                    switch j
                    {
                    case 1:
                        switch board.4.1
                        {
                        case 1:
                            da.image = NSImage(named: "black")
                        case 2:
                            da.image = NSImage(named: "white")
                        default:
                            da.image = NSImage(named: "null")
                        }
                    case 2:
                        switch board.4.2
                        {
                        case 1:
                            db.image = NSImage(named: "black")
                        case 2:
                            db.image = NSImage(named: "white")
                        default:
                            db.image = NSImage(named: "null")
                        }
                    case 3:
                        switch board.4.3
                        {
                        case 1:
                            dc.image = NSImage(named: "black")
                        case 2:
                            dc.image = NSImage(named: "white")
                        default:
                            dc.image = NSImage(named: "null")
                        }
                    case 4:
                        switch board.4.4
                        {
                        case 1:
                            dd.image = NSImage(named: "black")
                        case 2:
                            dd.image = NSImage(named: "white")
                        default:
                            dd.image = NSImage(named: "null")
                        }
                    case 5:
                        switch board.4.5
                        {
                        case 1:
                            de.image = NSImage(named: "black")
                        case 2:
                            de.image = NSImage(named: "white")
                        default:
                            de.image = NSImage(named: "null")
                        }
                    case 6:
                        switch board.4.6
                        {
                        case 1:
                            df.image = NSImage(named: "black")
                        case 2:
                            df.image = NSImage(named: "white")
                        default:
                            df.image = NSImage(named: "null")
                        }
                    case 7:
                        switch board.4.7
                        {
                        case 1:
                            dg.image = NSImage(named: "black")
                        case 2:
                            dg.image = NSImage(named: "white")
                        default:
                            dg.image = NSImage(named: "null")
                        }
                    case 8:
                        switch board.4.8
                        {
                        case 1:
                            dh.image = NSImage(named: "black")
                        case 2:
                            dh.image = NSImage(named: "white")
                        default:
                            dh.image = NSImage(named: "null")
                        }
                    default:
                        continue
                    }
                case 5:
                    switch j
                    {
                    case 1:
                        switch board.5.1
                        {
                        case 1:
                            ea.image = NSImage(named: "black")
                        case 2:
                            ea.image = NSImage(named: "white")
                        default:
                            ea.image = NSImage(named: "null")
                        }
                    case 2:
                        switch board.5.2
                        {
                        case 1:
                            eb.image = NSImage(named: "black")
                        case 2:
                            eb.image = NSImage(named: "white")
                        default:
                            eb.image = NSImage(named: "null")
                        }
                    case 3:
                        switch board.5.3
                        {
                        case 1:
                            ec.image = NSImage(named: "black")
                        case 2:
                            ec.image = NSImage(named: "white")
                        default:
                            ec.image = NSImage(named: "null")
                        }
                    case 4:
                        switch board.5.4
                        {
                        case 1:
                            ed.image = NSImage(named: "black")
                        case 2:
                            ed.image = NSImage(named: "white")
                        default:
                            ed.image = NSImage(named: "null")
                        }
                    case 5:
                        switch board.5.5
                        {
                        case 1:
                            ee.image = NSImage(named: "black")
                        case 2:
                            ee.image = NSImage(named: "white")
                        default:
                            ee.image = NSImage(named: "null")
                        }
                    case 6:
                        switch board.5.6
                        {
                        case 1:
                            ef.image = NSImage(named: "black")
                        case 2:
                            ef.image = NSImage(named: "white")
                        default:
                            ef.image = NSImage(named: "null")
                        }
                    case 7:
                        switch board.5.7
                        {
                        case 1:
                            eg.image = NSImage(named: "black")
                        case 2:
                            eg.image = NSImage(named: "white")
                        default:
                            eg.image = NSImage(named: "null")
                        }
                    case 8:
                        switch board.5.8
                        {
                        case 1:
                            eh.image = NSImage(named: "black")
                        case 2:
                            eh.image = NSImage(named: "white")
                        default:
                            eh.image = NSImage(named: "null")
                        }
                    default:
                        continue
                    }
                case 6:
                    switch j
                    {
                    case 1:
                        switch board.6.1
                        {
                        case 1:
                            fa.image = NSImage(named: "black")
                        case 2:
                            fa.image = NSImage(named: "white")
                        default:
                            fa.image = NSImage(named: "null")
                        }
                    case 2:
                        switch board.6.2
                        {
                        case 1:
                            fb.image = NSImage(named: "black")
                        case 2:
                            fb.image = NSImage(named: "white")
                        default:
                            fb.image = NSImage(named: "null")
                        }
                    case 3:
                        switch board.6.3
                        {
                        case 1:
                            fc.image = NSImage(named: "black")
                        case 2:
                            fc.image = NSImage(named: "white")
                        default:
                            fc.image = NSImage(named: "null")
                        }
                    case 4:
                        switch board.6.4
                        {
                        case 1:
                            fd.image = NSImage(named: "black")
                        case 2:
                            fd.image = NSImage(named: "white")
                        default:
                            fd.image = NSImage(named: "null")
                        }
                    case 5:
                        switch board.6.5
                        {
                        case 1:
                            fe.image = NSImage(named: "black")
                        case 2:
                            fe.image = NSImage(named: "white")
                        default:
                            fe.image = NSImage(named: "null")
                        }
                    case 6:
                        switch board.6.6
                        {
                        case 1:
                            ff.image = NSImage(named: "black")
                        case 2:
                            ff.image = NSImage(named: "white")
                        default:
                            ff.image = NSImage(named: "null")
                        }
                    case 7:
                        switch board.6.7
                        {
                        case 1:
                            fg.image = NSImage(named: "black")
                        case 2:
                            fg.image = NSImage(named: "white")
                        default:
                            fg.image = NSImage(named: "null")
                        }
                    case 8:
                        switch board.6.8
                        {
                        case 1:
                            fh.image = NSImage(named: "black")
                        case 2:
                            fh.image = NSImage(named: "white")
                        default:
                            fh.image = NSImage(named: "null")
                        }
                    default:
                        continue
                    }
                case 7:
                    switch j
                    {
                    case 1:
                        switch board.7.1
                        {
                        case 1:
                            ga.image = NSImage(named: "black")
                        case 2:
                            ga.image = NSImage(named: "white")
                        default:
                            ga.image = NSImage(named: "null")
                        }
                    case 2:
                        switch board.7.2
                        {
                        case 1:
                            gb.image = NSImage(named: "black")
                        case 2:
                            gb.image = NSImage(named: "white")
                        default:
                            gb.image = NSImage(named: "null")
                        }
                    case 3:
                        switch board.7.3
                        {
                        case 1:
                            gc.image = NSImage(named: "black")
                        case 2:
                            gc.image = NSImage(named: "white")
                        default:
                            gc.image = NSImage(named: "null")
                        }
                    case 4:
                        switch board.7.4
                        {
                        case 1:
                            gd.image = NSImage(named: "black")
                        case 2:
                            gd.image = NSImage(named: "white")
                        default:
                            gd.image = NSImage(named: "null")
                        }
                    case 5:
                        switch board.7.5
                        {
                        case 1:
                            ge.image = NSImage(named: "black")
                        case 2:
                            ge.image = NSImage(named: "white")
                        default:
                            ge.image = NSImage(named: "null")
                        }
                    case 6:
                        switch board.7.6
                        {
                        case 1:
                            gf.image = NSImage(named: "black")
                        case 2:
                            gf.image = NSImage(named: "white")
                        default:
                            gf.image = NSImage(named: "null")
                        }
                    case 7:
                        switch board.7.7
                        {
                        case 1:
                            gg.image = NSImage(named: "black")
                        case 2:
                            gg.image = NSImage(named: "white")
                        default:
                            gg.image = NSImage(named: "null")
                        }
                    case 8:
                        switch board.7.8
                        {
                        case 1:
                            gh.image = NSImage(named: "black")
                        case 2:
                            gh.image = NSImage(named: "white")
                        default:
                            gh.image = NSImage(named: "null")
                        }
                    default:
                        continue
                    }
                case 8:
                    switch j
                    {
                    case 1:
                        switch board.8.1
                        {
                        case 1:
                            ha.image = NSImage(named: "black")
                        case 2:
                            ha.image = NSImage(named: "white")
                        default:
                            ha.image = NSImage(named: "null")
                        }
                    case 2:
                        switch board.8.2
                        {
                        case 1:
                            hb.image = NSImage(named: "black")
                        case 2:
                            hb.image = NSImage(named: "white")
                        default:
                            hb.image = NSImage(named: "null")
                        }
                    case 3:
                        switch board.8.3
                        {
                        case 1:
                            hc.image = NSImage(named: "black")
                        case 2:
                            hc.image = NSImage(named: "white")
                        default:
                            hc.image = NSImage(named: "null")
                        }
                    case 4:
                        switch board.8.4
                        {
                        case 1:
                            hd.image = NSImage(named: "black")
                        case 2:
                            hd.image = NSImage(named: "white")
                        default:
                            hd.image = NSImage(named: "null")
                        }
                    case 5:
                        switch board.8.5
                        {
                        case 1:
                            he.image = NSImage(named: "black")
                        case 2:
                            he.image = NSImage(named: "white")
                        default:
                            he.image = NSImage(named: "null")
                        }
                    case 6:
                        switch board.8.6
                        {
                        case 1:
                            hf.image = NSImage(named: "black")
                        case 2:
                            hf.image = NSImage(named: "white")
                        default:
                            hf.image = NSImage(named: "null")
                        }
                    case 7:
                        switch board.8.7
                        {
                        case 1:
                            hg.image = NSImage(named: "black")
                        case 2:
                            hg.image = NSImage(named: "white")
                        default:
                            hg.image = NSImage(named: "null")
                        }
                    case 8:
                        switch board.8.8
                        {
                        case 1:
                            hh.image = NSImage(named: "black")
                        case 2:
                            hh.image = NSImage(named: "white")
                        default:
                            hh.image = NSImage(named: "null")
                        }
                    default:
                        continue
                    }
                default:
                    continue
                }
           }
        }
        if(finishedsw() == 1)
        {
            result()
            performSegue(withIdentifier: "popup", sender: self)
            close.isHidden = false
            return
        }
        if player == 2
        {
            self.putai()
        }
    }
}

