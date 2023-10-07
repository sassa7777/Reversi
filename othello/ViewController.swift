//
//  ViewController.swift
//  othello
//
//  Created by sasa on 2023/09/30.
//

import Cocoa
import Foundation

let white = "white"
let null = "null"
let black = "black"

//let y = 8
//let x = 8

@_cdecl("finish")
func finish()
{
    countstone()
    print("黒の数: \(blackc) 白の数: \(whitec)")
    if(blackc > whitec)
    {
        print("黒の勝ち！")
    }
    else if(whitec > blackc)
    {
        print("白の勝ち！")
    }
    else
    {
        print("引き分け")
    }
}

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
    


    

    @IBAction func aa(_ sender: Any)
    {
        putstone(1,1)
        reloadview()
    }
    @IBAction func ab(_ sender: Any)
    {
        putstone(1, 2)
        reloadview()
        
    }
    @IBAction func ac(_ sender: Any)
    {
        putstone(1,3)
        reloadview()
    }
    
    @IBAction func ad(_ sender: Any)
    {
        putstone(1,4)
        reloadview()
    }
    
    @IBAction func ae(_ sender: Any)
    {
        putstone(1,5)
        reloadview()
    }
    
    @IBAction func af(_ sender: Any)
    {
        putstone(1,6)
        reloadview()
    }
    
    @IBAction func ag(_ sender: Any)
    {
        putstone(1,7)
        reloadview()
    }
    
    @IBAction func ah(_ sender: Any)
    {
        putstone(1,8)
        reloadview()
    }
    
    @IBAction func ba(_ sender: Any)
    {
        putstone(2,1)
        reloadview()
    }
    
    @IBAction func bb(_ sender: Any)
    {
        putstone(2,2)
        reloadview()
    }
    
    @IBAction func bc(_ sender: Any)
    {
        putstone(2,3)
        reloadview()
    }
    
    @IBAction func bd(_ sender: Any)
    {
        putstone(2,4)
        reloadview()
    }
    
    @IBAction func be(_ sender: Any)
    {
        putstone(2,5)
        reloadview()
    }
    
    @IBAction func bf(_ sender: Any)
    {
        putstone(2,6)
        reloadview()
    }
    
    @IBAction func bg(_ sender: Any)
    {
        putstone(2,7)
        reloadview()
    }
    
    @IBAction func bh(_ sender: Any)
    {
        putstone(2,8)
        reloadview()
    }
    
    @IBAction func ca(_ sender: Any)
    {
        putstone(3,1)
        reloadview()
    }
    
    @IBAction func cb(_ sender: Any)
    {
        putstone(3,2)
        reloadview()
    }
    
    @IBAction func cc(_ sender: Any)
    {
        putstone(3,3)
        reloadview()
    }
    
    @IBAction func cd(_ sender: Any)
    {
        putstone(3,4)
        reloadview()
    }
    
    @IBAction func ce(_ sender: Any)
    {
        putstone(3,5)
        reloadview()
    }
    
    @IBAction func cf(_ sender: Any)
    {
        putstone(3,6)
        reloadview()
    }
    
    @IBAction func cg(_ sender: Any)
    {
        putstone(3,7)
        reloadview()
    }
    
    @IBAction func ch(_ sender: Any)
    {
        putstone(3,8)
        reloadview()
    }
    
    @IBAction func da(_ sender: Any)
    {
        putstone(4,1)
        reloadview()
    }
    
    @IBAction func db(_ sender: Any)
    {
        putstone(4,2)
        reloadview()
    }
    
    @IBAction func dc(_ sender: Any)
    {
        putstone(4,3)
        reloadview()
    }
    
    @IBAction func dd(_ sender: Any)
    {
        putstone(4,4)
        reloadview()
    }
    
    @IBAction func de(_ sender: Any)
    {
        putstone(4,5)
        reloadview()
    }
    
    @IBAction func df(_ sender: Any)
    {
        putstone(4,6)
        reloadview()
    }
    
    @IBAction func dg(_ sender: Any)
    {
        putstone(4,7)
        reloadview()
    }
    
    @IBAction func dh(_ sender: Any)
    {
        putstone(4,8)
        reloadview()
    }
    
    @IBAction func ea(_ sender: Any)
    {
        putstone(5,1)
        reloadview()
    }
    @IBAction func eb(_ sender: Any)
    {
        putstone(5,2)
        reloadview()
        
    }
    @IBAction func ec(_ sender: Any)
    {
        putstone(5,3)
        reloadview()
    }
    
    @IBAction func ed(_ sender: Any)
    {
        putstone(5,4)
        reloadview()
    }
    
    @IBAction func ee(_ sender: Any)
    {
        putstone(5,5)
        reloadview()
    }
    
    @IBAction func ef(_ sender: Any)
    {
        putstone(5,6)
        reloadview()
    }
    
    @IBAction func eg(_ sender: Any)
    {
        putstone(5,7)
        reloadview()
    }
    
    @IBAction func eh(_ sender: Any)
    {
        putstone(5,8)
        reloadview()
    }
    
    @IBAction func fa(_ sender: Any)
    {
        putstone(6,1)
        reloadview()
    }
    
    @IBAction func fb(_ sender: Any)
    {
        putstone(6,2)
        reloadview()
    }
    
    @IBAction func fc(_ sender: Any)
    {
        putstone(6,3)
        reloadview()
    }
    
    @IBAction func fd(_ sender: Any)
    {
        putstone(6,4)
        reloadview()
    }
    
    @IBAction func fe(_ sender: Any)
    {
        putstone(6,5)
        reloadview()
    }
    
    @IBAction func ff(_ sender: Any)
    {
        putstone(6,6)
        reloadview()
    }
    
    @IBAction func fg(_ sender: Any)
    {
        putstone(6,7)
        reloadview()
    }
    
    @IBAction func fh(_ sender: Any)
    {
        putstone(6,8)
        reloadview()
    }
    
    @IBAction func ga(_ sender: Any)
    {
        putstone(7,1)
        reloadview()
    }
    
    @IBAction func gb(_ sender: Any)
    {
        putstone(7,2)
        reloadview()
    }
    
    @IBAction func gc(_ sender: Any)
    {
        putstone(7,3)
        reloadview()
    }
    
    @IBAction func gd(_ sender: Any)
    {
        putstone(7,4)
        reloadview()
    }
    
    @IBAction func ge(_ sender: Any)
    {
        putstone(7,5)
        reloadview()
    }
    
    @IBAction func gf(_ sender: Any)
    {
        putstone(7,6)
        reloadview()
    }
    
    @IBAction func gg(_ sender: Any)
    {
        putstone(7,7)
        reloadview()
    }
    
    @IBAction func gh(_ sender: Any)
    {
        putstone(7,8)
        reloadview()
    }
    
    @IBAction func ha(_ sender: Any)
    {
        putstone(8,1)
        reloadview()
    }
    
    @IBAction func hb(_ sender: Any)
    {
        putstone(8,2)
        reloadview()
    }
    
    @IBAction func hc(_ sender: Any)
    {
        putstone(8,3)
        reloadview()
    }
    
    @IBAction func hd(_ sender: Any)
    {
        putstone(8,4)
        reloadview()
    }
    
    @IBAction func he(_ sender: Any)
    {
        putstone(8,5)
        reloadview()
    }
    
    @IBAction func hf(_ sender: Any)
    {
        putstone(8,6)
        reloadview()
    }
    
    @IBAction func hg(_ sender: Any)
    {
        putstone(8,7)
        reloadview()
    }
    
    @IBAction func hh(_ sender: Any)
    {
        putstone(8,8)
        reloadview()
    }
    
    
    override func viewDidLoad() 
    {
        reset()
        reloadview()
        //print(board.3.3)
    }
    
    func reloadview()
    {
        rebuild()
        print("[*]場面を再構築中...")
        for i in 1..<19 {
            for j in 1..<9 {
                switch i
                {
                case 1:
                    switch j
                    {
                    case 1:
                        switch board.1.1
                        {
                        case 0:
                            aa.image = NSImage(named: "null")
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
                        case 0:
                            ab.image = NSImage(named: "null")
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
                        case 0:
                            ac.image = NSImage(named: "null")
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
                        case 0:
                            ad.image = NSImage(named: "null")
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
                        case 0:
                            ae.image = NSImage(named: "null")
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
                        case 0:
                            af.image = NSImage(named: "null")
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
                        case 0:
                            ag.image = NSImage(named: "null")
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
                        case 0:
                            ah.image = NSImage(named: "null")
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
                        case 0:
                            ba.image = NSImage(named: "null")
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
                        case 0:
                            bb.image = NSImage(named: "null")
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
                        case 0:
                            bc.image = NSImage(named: "null")
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
                        case 0:
                            bd.image = NSImage(named: "null")
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
                        case 0:
                            be.image = NSImage(named: "null")
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
                        case 0:
                            bf.image = NSImage(named: "null")
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
                        case 0:
                            bg.image = NSImage(named: "null")
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
                        case 0:
                            bh.image = NSImage(named: "null")
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
                        case 0:
                            ca.image = NSImage(named: "null")
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
                        case 0:
                            cb.image = NSImage(named: "null")
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
                        case 0:
                            cc.image = NSImage(named: "null")
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
                        case 0:
                            cd.image = NSImage(named: "null")
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
                        case 0:
                            ce.image = NSImage(named: "null")
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
                        case 0:
                            cf.image = NSImage(named: "null")
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
                        case 0:
                            cg.image = NSImage(named: "null")
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
                        case 0:
                            ch.image = NSImage(named: "null")
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
                        case 0:
                            da.image = NSImage(named: "null")
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
                        case 0:
                            db.image = NSImage(named: "null")
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
                        case 0:
                            dc.image = NSImage(named: "null")
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
                        case 0:
                            dd.image = NSImage(named: "null")
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
                        case 0:
                            de.image = NSImage(named: "null")
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
                        case 0:
                            df.image = NSImage(named: "null")
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
                        case 0:
                            dg.image = NSImage(named: "null")
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
                        case 0:
                            dh.image = NSImage(named: "null")
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
                        case 0:
                            ea.image = NSImage(named: "null")
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
                        case 0:
                            eb.image = NSImage(named: "null")
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
                        case 0:
                            ec.image = NSImage(named: "null")
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
                        case 0:
                            ed.image = NSImage(named: "null")
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
                        case 0:
                            ee.image = NSImage(named: "null")
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
                        case 0:
                            ef.image = NSImage(named: "null")
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
                        case 0:
                            eg.image = NSImage(named: "null")
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
                        case 0:
                            eh.image = NSImage(named: "null")
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
                        case 0:
                            fa.image = NSImage(named: "null")
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
                        case 0:
                            fb.image = NSImage(named: "null")
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
                        case 0:
                            fc.image = NSImage(named: "null")
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
                        case 0:
                            fd.image = NSImage(named: "null")
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
                        case 0:
                            fe.image = NSImage(named: "null")
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
                        case 0:
                            ff.image = NSImage(named: "null")
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
                        case 0:
                            fg.image = NSImage(named: "null")
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
                        case 0:
                            fh.image = NSImage(named: "null")
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
                        case 0:
                            ga.image = NSImage(named: "null")
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
                        case 0:
                            gb.image = NSImage(named: "null")
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
                        case 0:
                            gc.image = NSImage(named: "null")
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
                        case 0:
                            gd.image = NSImage(named: "null")
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
                        case 0:
                            ge.image = NSImage(named: "null")
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
                        case 0:
                            gf.image = NSImage(named: "null")
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
                        case 0:
                            gg.image = NSImage(named: "null")
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
                        case 0:
                            gh.image = NSImage(named: "null")
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
                        case 0:
                            ha.image = NSImage(named: "null")
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
                        case 0:
                            hb.image = NSImage(named: "null")
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
                        case 0:
                            hc.image = NSImage(named: "null")
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
                        case 0:
                            hd.image = NSImage(named: "null")
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
                        case 0:
                            he.image = NSImage(named: "null")
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
                        case 0:
                            hf.image = NSImage(named: "null")
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
                        case 0:
                            hg.image = NSImage(named: "null")
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
                        case 0:
                            hh.image = NSImage(named: "null")
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
//        print("[*]場面を再構築中...")
//        for i in 0..<8 {
//            for j in 0..<8 {
//                switch i 
//                {
//                case 0:
//                    switch j
//                    {
//                    case 0:
//                        switch board.0.0 
//                        {
//                        case 0:
//                            aa.image = NSImage(named: "null")
//                        case 1:
//                            aa.image = NSImage(named: "black")
//                        case 2:
//                            aa.image = NSImage(named: "white")
//                        default:
//                            aa.image = NSImage(named: "null")
//                        }
//                    case 1:
//                        switch board.0.1
//                        {
//                        case 0:
//                            ab.image = NSImage(named: "null")
//                        case 1:
//                            ab.image = NSImage(named: "black")
//                        case 2:
//                            ab.image = NSImage(named: "white")
//                        default:
//                            ab.image = NSImage(named: "null")
//                            
//                        }
//                    case 2:
//                        switch board.0.2
//                        {
//                        case 0:
//                            ac.image = NSImage(named: "null")
//                        case 1:
//                            ac.image = NSImage(named: "black")
//                        case 2:
//                            ac.image = NSImage(named: "white")
//                        default:
//                            ac.image = NSImage(named: "null")
//                            
//                        }
//                    case 3:
//                        switch board.0.3
//                        {
//                        case 0:
//                            ad.image = NSImage(named: "null")
//                        case 1:
//                            ad.image = NSImage(named: "black")
//                        case 2:
//                            ad.image = NSImage(named: "white")
//                        default:
//                            ad.image = NSImage(named: "null")
//                            
//                        }
//                    case 4:
//                        switch board.0.4
//                        {
//                        case 0:
//                            ae.image = NSImage(named: "null")
//                        case 1:
//                            ae.image = NSImage(named: "black")
//                        case 2:
//                            ae.image = NSImage(named: "white")
//                        default:
//                            ae.image = NSImage(named: "null")
//                            
//                        }
//                    case 5:
//                        switch board.0.5
//                        {
//                        case 0:
//                            af.image = NSImage(named: "null")
//                        case 1:
//                            af.image = NSImage(named: "black")
//                        case 2:
//                            af.image = NSImage(named: "white")
//                        default:
//                            af.image = NSImage(named: "null")
//                            
//                        }
//                    case 6:
//                        switch board.0.6
//                        {
//                        case 0:
//                            ag.image = NSImage(named: "null")
//                        case 1:
//                            ag.image = NSImage(named: "black")
//                        case 2:
//                            ag.image = NSImage(named: "white")
//                        default:
//                            ag.image = NSImage(named: "null")
//                            
//                        }
//                    case 7:
//                        switch board.0.7
//                        {
//                        case 0:
//                            ah.image = NSImage(named: "null")
//                        case 1:
//                            ah.image = NSImage(named: "black")
//                        case 2:
//                            ah.image = NSImage(named: "white")
//                        default:
//                            ah.image = NSImage(named: "null")
//                            
//                        }
//                    default:
//                        continue
//                    }
//                case 1:
//                    switch j
//                    {
//                    case 0:
//                        switch board.1.0
//                        {
//                        case 0:
//                            ba.image = NSImage(named: "null")
//                        case 1:
//                            ba.image = NSImage(named: "black")
//                        case 2:
//                            ba.image = NSImage(named: "white")
//                        default:
//                            ba.image = NSImage(named: "null")
//                            
//                        }
//                    case 1:
//                        switch board.1.1
//                        {
//                        case 0:
//                            bb.image = NSImage(named: "null")
//                        case 1:
//                            bb.image = NSImage(named: "black")
//                        case 2:
//                            bb.image = NSImage(named: "white")
//                        default:
//                            bb.image = NSImage(named: "null")
//                            
//                        }
//                    case 2:
//                        switch board.1.2
//                        {
//                        case 0:
//                            bc.image = NSImage(named: "null")
//                        case 1:
//                            bc.image = NSImage(named: "black")
//                        case 2:
//                            bc.image = NSImage(named: "white")
//                        default:
//                            bc.image = NSImage(named: "null")
//                            
//                        }
//                    case 3:
//                        switch board.1.3
//                        {
//                        case 0:
//                            bd.image = NSImage(named: "null")
//                        case 1:
//                            bd.image = NSImage(named: "black")
//                        case 2:
//                            bd.image = NSImage(named: "white")
//                        default:
//                            bd.image = NSImage(named: "null")
//                            
//                        }
//                    case 4:
//                        switch board.1.4
//                        {
//                        case 0:
//                            be.image = NSImage(named: "null")
//                        case 1:
//                            be.image = NSImage(named: "black")
//                        case 2:
//                            be.image = NSImage(named: "white")
//                        default:
//                            be.image = NSImage(named: "null")
//                            
//                        }
//                    case 5:
//                        switch board.1.5
//                        {
//                        case 0:
//                            bf.image = NSImage(named: "null")
//                        case 1:
//                            bf.image = NSImage(named: "black")
//                        case 2:
//                            bf.image = NSImage(named: "white")
//                        default:
//                            bf.image = NSImage(named: "null")
//                            
//                        }
//                    case 6:
//                        switch board.1.6
//                        {
//                        case 0:
//                            bg.image = NSImage(named: "null")
//                        case 1:
//                            bg.image = NSImage(named: "black")
//                        case 2:
//                            bg.image = NSImage(named: "white")
//                        default:
//                            bg.image = NSImage(named: "null")
//                            
//                        }
//                    case 7:
//                        switch board.1.7
//                        {
//                        case 0:
//                            bh.image = NSImage(named: "null")
//                        case 1:
//                            bh.image = NSImage(named: "black")
//                        case 2:
//                            bh.image = NSImage(named: "white")
//                        default:
//                            bh.image = NSImage(named: "null")
//                            
//                        }
//                    default:
//                        continue
//                    }
//                case 2:
//                    switch j
//                    {
//                    case 0:
//                        switch board.2.0
//                        {
//                        case 0:
//                            ca.image = NSImage(named: "null")
//                        case 1:
//                            ca.image = NSImage(named: "black")
//                        case 2:
//                            ca.image = NSImage(named: "white")
//                        default:
//                            ca.image = NSImage(named: "null")
//                            
//                        }
//                    case 1:
//                        switch board.2.1
//                        {
//                        case 0:
//                            cb.image = NSImage(named: "null")
//                        case 1:
//                            cb.image = NSImage(named: "black")
//                        case 2:
//                            cb.image = NSImage(named: "white")
//                        default:
//                            cb.image = NSImage(named: "null")
//                            
//                        }
//                    case 2:
//                        switch board.2.2
//                        {
//                        case 0:
//                            cc.image = NSImage(named: "null")
//                        case 1:
//                            cc.image = NSImage(named: "black")
//                        case 2:
//                            cc.image = NSImage(named: "white")
//                        default:
//                            cc.image = NSImage(named: "null")
//                            
//                        }
//                    case 3:
//                        switch board.2.3
//                        {
//                        case 0:
//                            cd.image = NSImage(named: "null")
//                        case 1:
//                            cd.image = NSImage(named: "black")
//                        case 2:
//                            cd.image = NSImage(named: "white")
//                        default:
//                            cd.image = NSImage(named: "null")
//                            
//                        }
//                    case 4:
//                        switch board.2.4
//                        {
//                        case 0:
//                            ce.image = NSImage(named: "null")
//                        case 1:
//                            ce.image = NSImage(named: "black")
//                        case 2:
//                            ce.image = NSImage(named: "white")
//                        default:
//                            ce.image = NSImage(named: "null")
//                            
//                        }
//                    case 5:
//                        switch board.2.5
//                        {
//                        case 0:
//                            cf.image = NSImage(named: "null")
//                        case 1:
//                            cf.image = NSImage(named: "black")
//                        case 2:
//                            cf.image = NSImage(named: "white")
//                        default:
//                            cf.image = NSImage(named: "null")
//                            
//                        }
//                    case 6:
//                        switch board.2.6
//                        {
//                        case 0:
//                            cg.image = NSImage(named: "null")
//                        case 1:
//                            cg.image = NSImage(named: "black")
//                        case 2:
//                            cg.image = NSImage(named: "white")
//                        default:
//                            cg.image = NSImage(named: "null")
//                            
//                        }
//                    case 7:
//                        switch board.2.7
//                        {
//                        case 0:
//                            ch.image = NSImage(named: "null")
//                        case 1:
//                            ch.image = NSImage(named: "black")
//                        case 2:
//                            ch.image = NSImage(named: "white")
//                        default:
//                            ch.image = NSImage(named: "null")
//                            
//                        }
//                    default:
//                        continue
//                    }
//                case 3:
//                    switch j
//                    {
//                    case 0:
//                        switch board.3.0
//                        {
//                        case 0:
//                            da.image = NSImage(named: "null")
//                        case 1:
//                            da.image = NSImage(named: "black")
//                        case 2:
//                            da.image = NSImage(named: "white")
//                        default:
//                            da.image = NSImage(named: "null")
//                            
//                        }
//                    case 1:
//                        switch board.3.1
//                        {
//                        case 0:
//                            db.image = NSImage(named: "null")
//                        case 1:
//                            db.image = NSImage(named: "black")
//                        case 2:
//                            db.image = NSImage(named: "white")
//                        default:
//                            db.image = NSImage(named: "null")
//                            
//                        }
//                    case 2:
//                        switch board.3.2
//                        {
//                        case 0:
//                            dc.image = NSImage(named: "null")
//                        case 1:
//                            dc.image = NSImage(named: "black")
//                        case 2:
//                            dc.image = NSImage(named: "white")
//                        default:
//                            dc.image = NSImage(named: "null")
//                            
//                        }
//                    case 3:
//                        switch board.3.3
//                        {
//                        case 0:
//                            dd.image = NSImage(named: "null")
//                        case 1:
//                            dd.image = NSImage(named: "black")
//                        case 2:
//                            dd.image = NSImage(named: "white")
//                        default:
//                            dd.image = NSImage(named: "null")
//                            
//                        }
//                    case 4:
//                        switch board.3.4
//                        {
//                        case 0:
//                            de.image = NSImage(named: "null")
//                        case 1:
//                            de.image = NSImage(named: "black")
//                        case 2:
//                            de.image = NSImage(named: "white")
//                        default:
//                            de.image = NSImage(named: "null")
//                            
//                        }
//                    case 5:
//                        switch board.3.5
//                        {
//                        case 0:
//                            df.image = NSImage(named: "null")
//                        case 1:
//                            df.image = NSImage(named: "black")
//                        case 2:
//                            df.image = NSImage(named: "white")
//                        default:
//                            df.image = NSImage(named: "null")
//                            
//                        }
//                    case 6:
//                        switch board.3.6
//                        {
//                        case 0:
//                            dg.image = NSImage(named: "null")
//                        case 1:
//                            dg.image = NSImage(named: "black")
//                        case 2:
//                            dg.image = NSImage(named: "white")
//                        default:
//                            dg.image = NSImage(named: "null")
//                            
//                        }
//                    case 7:
//                        switch board.3.7
//                        {
//                        case 0:
//                            dh.image = NSImage(named: "null")
//                        case 1:
//                            dh.image = NSImage(named: "black")
//                        case 2:
//                            dh.image = NSImage(named: "white")
//                        default:
//                            dh.image = NSImage(named: "null")
//                            
//                        }
//                    default:
//                        continue
//                    }
//                case 4:
//                    switch j
//                    {
//                    case 0:
//                        switch board.4.0
//                        {
//                        case 0:
//                            ea.image = NSImage(named: "null")
//                        case 1:
//                            ea.image = NSImage(named: "black")
//                        case 2:
//                            ea.image = NSImage(named: "white")
//                        default:
//                            ea.image = NSImage(named: "null")
//                            
//                        }
//                    case 1:
//                        switch board.4.1
//                        {
//                        case 0:
//                            eb.image = NSImage(named: "null")
//                        case 1:
//                            eb.image = NSImage(named: "black")
//                        case 2:
//                            eb.image = NSImage(named: "white")
//                        default:
//                            eb.image = NSImage(named: "null")
//                            
//                        }
//                    case 2:
//                        switch board.4.2
//                        {
//                        case 0:
//                            ec.image = NSImage(named: "null")
//                        case 1:
//                            ec.image = NSImage(named: "black")
//                        case 2:
//                            ec.image = NSImage(named: "white")
//                        default:
//                            ec.image = NSImage(named: "null")
//                            
//                        }
//                    case 3:
//                        switch board.4.3
//                        {
//                        case 0:
//                            ed.image = NSImage(named: "null")
//                        case 1:
//                            ed.image = NSImage(named: "black")
//                        case 2:
//                            ed.image = NSImage(named: "white")
//                        default:
//                            ed.image = NSImage(named: "null")
//                            
//                        }
//                    case 4:
//                        switch board.4.4
//                        {
//                        case 0:
//                            ee.image = NSImage(named: "null")
//                        case 1:
//                            ee.image = NSImage(named: "black")
//                        case 2:
//                            ee.image = NSImage(named: "white")
//                        default:
//                            ee.image = NSImage(named: "null")
//                            
//                        }
//                    case 5:
//                        switch board.4.5
//                        {
//                        case 0:
//                            ef.image = NSImage(named: "null")
//                        case 1:
//                            ef.image = NSImage(named: "black")
//                        case 2:
//                            ef.image = NSImage(named: "white")
//                        default:
//                            ef.image = NSImage(named: "null")
//                            
//                        }
//                    case 6:
//                        switch board.4.6
//                        {
//                        case 0:
//                            eg.image = NSImage(named: "null")
//                        case 1:
//                            eg.image = NSImage(named: "black")
//                        case 2:
//                            eg.image = NSImage(named: "white")
//                        default:
//                            eg.image = NSImage(named: "null")
//                            
//                        }
//                    case 7:
//                        switch board.4.7
//                        {
//                        case 0:
//                            eh.image = NSImage(named: "null")
//                        case 1:
//                            eh.image = NSImage(named: "black")
//                        case 2:
//                            eh.image = NSImage(named: "white")
//                        default:
//                            eh.image = NSImage(named: "null")
//                            
//                        }
//                    default:
//                        continue
//                    }
//                case 5:
//                    switch j
//                    {
//                    case 0:
//                        switch board.5.0
//                        {
//                        case 0:
//                            fa.image = NSImage(named: "null")
//                        case 1:
//                            fa.image = NSImage(named: "black")
//                        case 2:
//                            fa.image = NSImage(named: "white")
//                        default:
//                            fa.image = NSImage(named: "null")
//                            
//                        }
//                    case 1:
//                        switch board.5.1
//                        {
//                        case 0:
//                            fb.image = NSImage(named: "null")
//                        case 1:
//                            fb.image = NSImage(named: "black")
//                        case 2:
//                            fb.image = NSImage(named: "white")
//                        default:
//                            fb.image = NSImage(named: "null")
//                            
//                        }
//                    case 2:
//                        switch board.5.2
//                        {
//                        case 0:
//                            fc.image = NSImage(named: "null")
//                        case 1:
//                            fc.image = NSImage(named: "black")
//                        case 2:
//                            fc.image = NSImage(named: "white")
//                        default:
//                            fc.image = NSImage(named: "null")
//                            
//                        }
//                    case 3:
//                        switch board.5.3
//                        {
//                        case 0:
//                            fd.image = NSImage(named: "null")
//                        case 1:
//                            fd.image = NSImage(named: "black")
//                        case 2:
//                            fd.image = NSImage(named: "white")
//                        default:
//                            fd.image = NSImage(named: "null")
//                            
//                        }
//                    case 4:
//                        switch board.5.4
//                        {
//                        case 0:
//                            fe.image = NSImage(named: "null")
//                        case 1:
//                            fe.image = NSImage(named: "black")
//                        case 2:
//                            fe.image = NSImage(named: "white")
//                        default:
//                            fe.image = NSImage(named: "null")
//                            
//                        }
//                    case 5:
//                        switch board.5.5
//                        {
//                        case 0:
//                            ff.image = NSImage(named: "null")
//                        case 1:
//                            ff.image = NSImage(named: "black")
//                        case 2:
//                            ff.image = NSImage(named: "white")
//                        default:
//                            ff.image = NSImage(named: "null")
//                            
//                        }
//                    case 6:
//                        switch board.5.6
//                        {
//                        case 0:
//                            fg.image = NSImage(named: "null")
//                        case 1:
//                            fg.image = NSImage(named: "black")
//                        case 2:
//                            fg.image = NSImage(named: "white")
//                        default:
//                            fg.image = NSImage(named: "null")
//                            
//                        }
//                    case 7:
//                        switch board.5.7
//                        {
//                        case 0:
//                            fh.image = NSImage(named: "null")
//                        case 1:
//                            fh.image = NSImage(named: "black")
//                        case 2:
//                            fh.image = NSImage(named: "white")
//                        default:
//                            fh.image = NSImage(named: "null")
//                            
//                        }
//                    default:
//                        continue
//                    }
//                case 6:
//                    switch j
//                    {
//                    case 0:
//                        switch board.6.0
//                        {
//                        case 0:
//                            ga.image = NSImage(named: "null")
//                        case 1:
//                            ga.image = NSImage(named: "black")
//                        case 2:
//                            ga.image = NSImage(named: "white")
//                        default:
//                            ga.image = NSImage(named: "null")
//                            
//                        }
//                    case 1:
//                        switch board.6.1
//                        {
//                        case 0:
//                            gb.image = NSImage(named: "null")
//                        case 1:
//                            gb.image = NSImage(named: "black")
//                        case 2:
//                            gb.image = NSImage(named: "white")
//                        default:
//                            gb.image = NSImage(named: "null")
//                            
//                        }
//                    case 2:
//                        switch board.6.2
//                        {
//                        case 0:
//                            gc.image = NSImage(named: "null")
//                        case 1:
//                            gc.image = NSImage(named: "black")
//                        case 2:
//                            gc.image = NSImage(named: "white")
//                        default:
//                            gc.image = NSImage(named: "null")
//                            
//                        }
//                    case 3:
//                        switch board.6.3
//                        {
//                        case 0:
//                            gd.image = NSImage(named: "null")
//                        case 1:
//                            gd.image = NSImage(named: "black")
//                        case 2:
//                            gd.image = NSImage(named: "white")
//                        default:
//                            gd.image = NSImage(named: "null")
//                            
//                        }
//                    case 4:
//                        switch board.6.4
//                        {
//                        case 0:
//                            ge.image = NSImage(named: "null")
//                        case 1:
//                            ge.image = NSImage(named: "black")
//                        case 2:
//                            ge.image = NSImage(named: "white")
//                        default:
//                            ge.image = NSImage(named: "null")
//                            
//                        }
//                    case 5:
//                        switch board.6.5
//                        {
//                        case 0:
//                            gf.image = NSImage(named: "null")
//                        case 1:
//                            gf.image = NSImage(named: "black")
//                        case 2:
//                            gf.image = NSImage(named: "white")
//                        default:
//                            gf.image = NSImage(named: "null")
//                            
//                        }
//                    case 6:
//                        switch board.6.6
//                        {
//                        case 0:
//                            gg.image = NSImage(named: "null")
//                        case 1:
//                            gg.image = NSImage(named: "black")
//                        case 2:
//                            gg.image = NSImage(named: "white")
//                        default:
//                            gg.image = NSImage(named: "null")
//                            
//                        }
//                    case 7:
//                        switch board.6.7
//                        {
//                        case 0:
//                            gh.image = NSImage(named: "null")
//                        case 1:
//                            gh.image = NSImage(named: "black")
//                        case 2:
//                            gh.image = NSImage(named: "white")
//                        default:
//                            gh.image = NSImage(named: "null")
//                            
//                        }
//                    default:
//                        continue
//                    }
//                case 7:
//                    switch j
//                    {
//                    case 0:
//                        switch board.7.0
//                        {
//                        case 0:
//                            ha.image = NSImage(named: "null")
//                        case 1:
//                            ha.image = NSImage(named: "black")
//                        case 2:
//                            ha.image = NSImage(named: "white")
//                        default:
//                            ha.image = NSImage(named: "null")
//                            
//                        }
//                    case 1:
//                        switch board.7.1
//                        {
//                        case 0:
//                            hb.image = NSImage(named: "null")
//                        case 1:
//                            hb.image = NSImage(named: "black")
//                        case 2:
//                            hb.image = NSImage(named: "white")
//                        default:
//                            hb.image = NSImage(named: "null")
//                            
//                        }
//                    case 2:
//                        switch board.7.2
//                        {
//                        case 0:
//                            hc.image = NSImage(named: "null")
//                        case 1:
//                            hc.image = NSImage(named: "black")
//                        case 2:
//                            hc.image = NSImage(named: "white")
//                        default:
//                            hc.image = NSImage(named: "null")
//                            
//                        }
//                    case 3:
//                        switch board.7.3
//                        {
//                        case 0:
//                            hd.image = NSImage(named: "null")
//                        case 1:
//                            hd.image = NSImage(named: "black")
//                        case 2:
//                            hd.image = NSImage(named: "white")
//                        default:
//                            hd.image = NSImage(named: "null")
//                            
//                        }
//                    case 4:
//                        switch board.7.4
//                        {
//                        case 0:
//                            he.image = NSImage(named: "null")
//                        case 1:
//                            he.image = NSImage(named: "black")
//                        case 2:
//                            he.image = NSImage(named: "white")
//                        default:
//                            he.image = NSImage(named: "null")
//                            
//                        }
//                    case 5:
//                        switch board.7.5
//                        {
//                        case 0:
//                            hf.image = NSImage(named: "null")
//                        case 1:
//                            hf.image = NSImage(named: "black")
//                        case 2:
//                            hf.image = NSImage(named: "white")
//                        default:
//                            hf.image = NSImage(named: "null")
//                            
//                        }
//                    case 6:
//                        switch board.7.6
//                        {
//                        case 0:
//                            hg.image = NSImage(named: "null")
//                        case 1:
//                            hg.image = NSImage(named: "black")
//                        case 2:
//                            hg.image = NSImage(named: "white")
//                        default:
//                            hg.image = NSImage(named: "null")
//                            
//                        }
//                    case 7:
//                        switch board.7.7
//                        {
//                        case 0:
//                            hh.image = NSImage(named: "null")
//                        case 1:
//                            hh.image = NSImage(named: "black")
//                        case 2:
//                            hh.image = NSImage(named: "white")
//                        default:
//                            hh.image = NSImage(named: "null")
//                            
//                        }
//                    default:
//                        continue
//                    }
//                default:
//                    continue
//                }
//           }
//        }
    }
    
    
}

