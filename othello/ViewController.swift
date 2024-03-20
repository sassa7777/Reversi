//
//  ViewController.swift
//  othello
//
//  Created by sasa on 2023/09/30.
//

import Cocoa
import Foundation


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
	@IBOutlet var hakotext: NSTextField!
	
	@IBAction func put(_ sender: NSButton)
	{
		var results: Int32 = 0
		switch sender
		{
		case aa: results = putstone(1, 1)
		case ab: results = putstone(1, 2)
		case ac: results = putstone(1, 3)
		case ad: results = putstone(1, 4)
		case ae: results = putstone(1, 5)
		case af: results = putstone(1, 6)
		case ag: results = putstone(1, 7)
		case ah: results = putstone(1, 8)
			
		case ba: results = putstone(2, 1)
		case bb: results = putstone(2, 2)
		case bc: results = putstone(2, 3)
		case bd: results = putstone(2, 4)
		case be: results = putstone(2, 5)
		case bf: results = putstone(2, 6)
		case bg: results = putstone(2, 7)
		case bh: results = putstone(2, 8)

		case ca: results = putstone(3, 1)
		case cb: results = putstone(3, 2)
		case cc: results = putstone(3, 3)
		case cd: results = putstone(3, 4)
		case ce: results = putstone(3, 5)
		case cf: results = putstone(3, 6)
		case cg: results = putstone(3, 7)
		case ch: results = putstone(3, 8)

		case da: results = putstone(4, 1)
		case db: results = putstone(4, 2)
		case dc: results = putstone(4, 3)
		case dd: results = putstone(4, 4)
		case de: results = putstone(4, 5)
		case df: results = putstone(4, 6)
		case dg: results = putstone(4, 7)
		case dh: results = putstone(4, 8)

		case ea: results = putstone(5, 1)
		case eb: results = putstone(5, 2)
		case ec: results = putstone(5, 3)
		case ed: results = putstone(5, 4)
		case ee: results = putstone(5, 5)
		case ef: results = putstone(5, 6)
		case eg: results = putstone(5, 7)
		case eh: results = putstone(5, 8)

		case fa: results = putstone(6, 1)
		case fb: results = putstone(6, 2)
		case fc: results = putstone(6, 3)
		case fd: results = putstone(6, 4)
		case fe: results = putstone(6, 5)
		case ff: results = putstone(6, 6)
		case fg: results = putstone(6, 7)
		case fh: results = putstone(6, 8)

		case ga: results = putstone(7, 1)
		case gb: results = putstone(7, 2)
		case gc: results = putstone(7, 3)
		case gd: results = putstone(7, 4)
		case ge: results = putstone(7, 5)
		case gf: results = putstone(7, 6)
		case gg: results = putstone(7, 7)
		case gh: results = putstone(7, 8)

		case ha: results = putstone(8, 1)
		case hb: results = putstone(8, 2)
		case hc: results = putstone(8, 3)
		case hd: results = putstone(8, 4)
		case he: results = putstone(8, 5)
		case hf: results = putstone(8, 6)
		case hg: results = putstone(8, 7)
		case hh: results = putstone(8, 8)

		default: break
		}
		if(results != 0)
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
		putai()
		reloadview()
	}
	
	func switchbutton(switch: Bool)
	{
		let swiftbuttons: [[NSButton]] = [
			[aa, ab, ac, ad, ae, af, ag, ah],
			[ba, bb, bc, bd, be, bf, bg, bh],
			[ca, cb, cc, cd, ce, cf, cg, ch],
			[da, db, dc, dd, de, df, dg, dh],
			[ea, eb, ec, ed, ee, ef, eg, eh],
			[fa, fb, fc, fd, fe, ff, fg, fh],
			[ga, gb, gc, gd, ge, gf, gg, gh],
			[ha, hb, hc, hd, he, hf, hg, hh]
		]
		
		let canputswift: [[Bool]] = [
			[canPut.1.1, canPut.1.2, canPut.1.3, canPut.1.4, canPut.1.5, canPut.1.6, canPut.1.7, canPut.1.8],
			[canPut.2.1, canPut.2.2, canPut.2.3, canPut.2.4, canPut.2.5, canPut.2.6, canPut.2.7, canPut.2.8],
			[canPut.3.1, canPut.3.2, canPut.3.3, canPut.3.4, canPut.3.5, canPut.3.6, canPut.3.7, canPut.3.8],
			[canPut.4.1, canPut.4.2, canPut.4.3, canPut.4.4, canPut.4.5, canPut.4.6, canPut.4.7, canPut.4.8],
			[canPut.5.1, canPut.5.2, canPut.5.3, canPut.5.4, canPut.5.5, canPut.5.6, canPut.5.7, canPut.5.8],
			[canPut.6.1, canPut.6.2, canPut.6.3, canPut.6.4, canPut.6.5, canPut.6.6, canPut.6.7, canPut.6.8],
			[canPut.7.1, canPut.7.2, canPut.7.3, canPut.7.4, canPut.7.5, canPut.7.6, canPut.7.7, canPut.7.8],
			[canPut.8.1, canPut.8.2, canPut.8.3, canPut.8.4, canPut.8.5, canPut.8.6, canPut.8.7, canPut.8.8],
		]
		
		for i in swiftbuttons {
			for swiftbutton in i {
				(swiftbutton.cell as? NSButtonCell)?.imageDimsWhenDisabled = false
			}
		}
		
		for i in 0..<8 {
			for j in 0..<8 {
				if(canputswift[i][j] == true && `switch` == true)
				{
					swiftbuttons[i][j].isEnabled = true
				}
				else
				{
					swiftbuttons[i][j].isEnabled = false
				}
			}
		}
	}
	
	func putai()
	{
		if(finishedsw() != 1)
		{
			DispatchQueue.main.async()
			{
				self.switchbutton(switch: false)
				self.hakotext.stringValue = "考え中...\n(時間がかかることがあります)"
			}
		}
		DispatchQueue.global().asyncAfter(deadline: .now())
		{
			if(ai() == 1)
			{
				DispatchQueue.main.asyncAfter(deadline: .now() + 0.4)
				{
					self.reloadview()
					self.putai()
				}
			}
			else
			{
				if finishedsw() != 1
				{
					DispatchQueue.main.async()
					{
						self.switchbutton(switch: true)
						self.hakotext.stringValue = "君(プレイヤー)の番だよ！\n置く場所を選んでね！"
					}
				}
			}
		}
	}
	
	func result()
	{
		if winner() == 1
		{
			print("黒の勝ち！")
			self.hakotext.stringValue = "君の勝ち！\n遊んでくれてありがとう！"
		}
		else if winner() == 2
		{
			print("白の勝ち！")
			self.hakotext.stringValue = "僕の勝ち！\n遊んでくれてありがとう！"
		}
		else
		{
			print("引き分け")
			self.hakotext.stringValue = "引き分け！\n遊んでくれてありがとう！"
		}
		switchbutton(switch: false)
		hakotext.isEditable = false
	}
	
	func reloadview()
	{
		rebuild()
		print("[*]リロード中...")
		let buttons: [[NSButton]] = [
			[aa, ab, ac, ad, ae, af, ag, ah],
			[ba, bb, bc, bd, be, bf, bg, bh],
			[ca, cb, cc, cd, ce, cf, cg, ch],
			[da, db, dc, dd, de, df, dg, dh],
			[ea, eb, ec, ed, ee, ef, eg, eh],
			[fa, fb, fc, fd, fe, ff, fg, fh],
			[ga, gb, gc, gd, ge, gf, gg, gh],
			[ha, hb, hc, hd, he, hf, hg, hh]
		]
		
		let boardswift: [[Int8]] = [
			[board.1.1, board.1.2, board.1.3, board.1.4, board.1.5, board.1.6, board.1.7, board.1.8],
			[board.2.1, board.2.2, board.2.3, board.2.4, board.2.5, board.2.6, board.2.7, board.2.8],
			[board.3.1, board.3.2, board.3.3, board.3.4, board.3.5, board.3.6, board.3.7, board.3.8],
			[board.4.1, board.4.2, board.4.3, board.4.4, board.4.5, board.4.6, board.4.7, board.4.8],
			[board.5.1, board.5.2, board.5.3, board.5.4, board.5.5, board.5.6, board.5.7, board.5.8],
			[board.6.1, board.6.2, board.6.3, board.6.4, board.6.5, board.6.6, board.6.7, board.6.8],
			[board.7.1, board.7.2, board.7.3, board.7.4, board.7.5, board.7.6, board.7.7, board.7.8],
			[board.8.1, board.8.2, board.8.3, board.8.4, board.8.5, board.8.6, board.8.7, board.8.8],
		]
		
		let canputswift: [[Bool]] = [
			[canPut.1.1, canPut.1.2, canPut.1.3, canPut.1.4, canPut.1.5, canPut.1.6, canPut.1.7, canPut.1.8],
			[canPut.2.1, canPut.2.2, canPut.2.3, canPut.2.4, canPut.2.5, canPut.2.6, canPut.2.7, canPut.2.8],
			[canPut.3.1, canPut.3.2, canPut.3.3, canPut.3.4, canPut.3.5, canPut.3.6, canPut.3.7, canPut.3.8],
			[canPut.4.1, canPut.4.2, canPut.4.3, canPut.4.4, canPut.4.5, canPut.4.6, canPut.4.7, canPut.4.8],
			[canPut.5.1, canPut.5.2, canPut.5.3, canPut.5.4, canPut.5.5, canPut.5.6, canPut.5.7, canPut.5.8],
			[canPut.6.1, canPut.6.2, canPut.6.3, canPut.6.4, canPut.6.5, canPut.6.6, canPut.6.7, canPut.6.8],
			[canPut.7.1, canPut.7.2, canPut.7.3, canPut.7.4, canPut.7.5, canPut.7.6, canPut.7.7, canPut.7.8],
			[canPut.8.1, canPut.8.2, canPut.8.3, canPut.8.4, canPut.8.5, canPut.8.6, canPut.8.7, canPut.8.8],
		]
		
		for i in 0..<8
		{
			for j in 0..<8
			{
				switch boardswift[i][j]
				{
				case 1:
					buttons[i][j].image = NSImage(named: "black")
					if(tmpy == i+1 && tmpx == j+1)
					{
						buttons[i][j].image = NSImage(named: "blackb")
					}
				case 2:
					buttons[i][j].image = NSImage(named: "white")
					if(tmpy == i+1 && tmpx == j+1)
					{
						buttons[i][j].image = NSImage(named: "whiteb")
					}
				default:
					buttons[i][j].image = NSImage(named: "null")
					if canputswift[i][j] == true && returnplayer() == 1
					{
						buttons[i][j].image = NSImage(named: "null2")
					}
				}
			}
		}
		print("[*]完了")
		if(finishedsw() == 1)
		{
			result()
			performSegue(withIdentifier: "popup", sender: self)
			close.isHidden = false
			return
		}
	}
	
	@objc class SwiftFunctions: NSObject {
		@objc class func thinking_hako() {
			DispatchQueue.main.async {
				if let viewController = NSApplication.shared.keyWindow?.contentViewController as? ViewController {
					viewController.hakotext.stringValue = "考え中... (\(think_percent)%)"
				}
			}
		}
	}
	
}

