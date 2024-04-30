//
//  ViewController.swift
//  othello
//
//  Created by sasa on 2023/09/30.
//

import Cocoa
import Foundation
import AVFoundation


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
	@IBOutlet var hakoface: NSImageView!
	@IBOutlet var close: NSButton!
	@IBOutlet var restart: NSButton!
	@IBOutlet var hakotext: NSTextField!
	var music_player:AVAudioPlayer!
	
	@IBAction func put(_ sender: NSButton)
	{
		var results: Int32 = 0
		switch sender
		{
		case aa: results = putstone(0, 0)
		case ab: results = putstone(0, 1)
		case ac: results = putstone(0, 2)
		case ad: results = putstone(0, 3)
		case ae: results = putstone(0, 4)
		case af: results = putstone(0, 5)
		case ag: results = putstone(0, 6)
		case ah: results = putstone(0, 7)
			
		case ba: results = putstone(1, 0)
		case bb: results = putstone(1, 1)
		case bc: results = putstone(1, 2)
		case bd: results = putstone(1, 3)
		case be: results = putstone(1, 4)
		case bf: results = putstone(1, 5)
		case bg: results = putstone(1, 6)
		case bh: results = putstone(1, 7)

		case ca: results = putstone(2, 0)
		case cb: results = putstone(2, 1)
		case cc: results = putstone(2, 2)
		case cd: results = putstone(2, 3)
		case ce: results = putstone(2, 4)
		case cf: results = putstone(2, 5)
		case cg: results = putstone(2, 6)
		case ch: results = putstone(2, 7)

		case da: results = putstone(3, 0)
		case db: results = putstone(3, 1)
		case dc: results = putstone(3, 2)
		case dd: results = putstone(3, 3)
		case de: results = putstone(4, 4)
		case df: results = putstone(3, 5)
		case dg: results = putstone(3, 6)
		case dh: results = putstone(3, 7)

		case ea: results = putstone(4, 0)
		case eb: results = putstone(4, 1)
		case ec: results = putstone(4, 2)
		case ed: results = putstone(4, 3)
		case ee: results = putstone(4, 4)
		case ef: results = putstone(4, 5)
		case eg: results = putstone(4, 6)
		case eh: results = putstone(4, 7)

		case fa: results = putstone(5, 0)
		case fb: results = putstone(5, 1)
		case fc: results = putstone(5, 2)
		case fd: results = putstone(5, 3)
		case fe: results = putstone(5, 4)
		case ff: results = putstone(5, 5)
		case fg: results = putstone(5, 6)
		case fh: results = putstone(5, 7)

		case ga: results = putstone(6, 0)
		case gb: results = putstone(6, 1)
		case gc: results = putstone(6, 2)
		case gd: results = putstone(6, 3)
		case ge: results = putstone(6, 4)
		case gf: results = putstone(6, 5)
		case gg: results = putstone(6, 6)
		case gh: results = putstone(6, 7)

		case ha: results = putstone(7, 0)
		case hb: results = putstone(7, 1)
		case hc: results = putstone(7, 2)
		case hd: results = putstone(7, 3)
		case he: results = putstone(7, 4)
		case hf: results = putstone(7, 5)
		case hg: results = putstone(7, 6)
		case hh: results = putstone(7, 7)

		default: break
		}
		if(results == 1)
		{
			putsound();
			swapboard();
			if(isPass() == true) {
				swapboard();
			}
			reloadview()
			putai()
		}
	}
	
	@IBAction func close(_ sender: Any) {
		self.dismiss(self)
	}
	@IBAction func restart(_ sender: Any) {
		self.viewDidLoad()
	}
	
	func putsound(){
		var asset: NSDataAsset!
		if(nowTurn == BLACK_TURN) {
			asset = NSDataAsset(name:"stone2")
		} else {
			asset = NSDataAsset(name:"stone1")
		}
		music_player = try! AVAudioPlayer(data:asset!.data, fileTypeHint:"mp3")
		music_player.play()
	}
	
	func shell(_ command: String) -> String {
		let task = Process()
		task.launchPath = "/bin/sh"
		task.arguments = ["-c", command]
		
		let pipe = Pipe()
		task.standardOutput = pipe
		task.launch()
		
		let data = pipe.fileHandleForReading.readDataToEndOfFile()
		let output = String(data: data, encoding: .utf8)!
		return output.trimmingCharacters(in: .whitespacesAndNewlines)
	}
	
	
	override func viewDidLoad()
	{
		let command1 = "arch"
		if(shell(command1) == "arm64") {
			let command2 = "sysctl -n hw.perflevel0.physicalcpu"
			let cpu_coreString = shell(command2)
			cpu_core = Int32(cpu_coreString) ?? 1
		} else {
			let command2 = "sysctl -n hw.logicalcpu"
			let cpu_coreString = shell(command2)
			cpu_core = Int32(cpu_coreString) ?? 1
			cpu_core = cpu_core/2
		}
		hakoface.image = NSImage(named: "hako")
		close.isHidden = true
		restart.isHidden = true
		reset()
		if(botplayer == BLACK_TURN) {
			reloadview()
			DispatchQueue.main.asyncAfter(deadline: .now()+0.3) {
				self.putai()
			}
		} else {
			putai()
		}
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
		
		
		for i in swiftbuttons {
			for swiftbutton in i {
				(swiftbutton.cell as? NSButtonCell)?.imageDimsWhenDisabled = false
			}
		}
		let legalboard: UInt64 = makelegalBoard(&playerboard, &oppenentboard)
		var mask: UInt64 = 0x8000000000000000
		for i in 0..<8 {
			for j in 0..<8 {
				if((legalboard & mask) != 0) {
					if(`switch` == true) {
						swiftbuttons[i][j].isEnabled = true
					}
				} else {
					swiftbuttons[i][j].isEnabled = false
				}
				mask = mask >> 1
			}
		}
	}
	
	func putai()
	{
		if(isFinished() == false)
		{
			self.switchbutton(switch: false)
			self.hakotext.stringValue = "考え中...\n(時間がかかることがあります)"
			hakoface.image = NSImage(named: "thinking")
			DispatchQueue.global().asyncAfter(deadline: .now())
			{
				if(ai() == 1)
				{
					DispatchQueue.main.asyncAfter(deadline: .now() + 0.4)
					{
						self.putsound();
						swapboard();
						if(isPass() == true) {
							swapboard();
						}
						self.reloadview()
						self.putai()
					}
				}
				else
				{
					if isFinished() == false
					{
						DispatchQueue.main.async()
						{
							self.switchbutton(switch: true)
							self.hakotext.stringValue = "君(プレイヤー)の番だよ！\n置く場所を選んでね！"
							self.hakoface.image = NSImage(named: "hako")
						}
					}
				}
			}
		}
	}
	
	func result()
	{
		if(botplayer == WHITE_TURN) {
			switch winner() {
			case 1:
				print("黒の勝ち！")
				self.hakotext.stringValue = "君の勝ち！\n遊んでくれてありがとう！"
				self.hakoface.image = NSImage(named: "win")
			case 2:
				print("白の勝ち！")
				self.hakotext.stringValue = "僕の勝ち！\n遊んでくれてありがとう！"
				self.hakoface.image = NSImage(named: "lose")
			default:
				print("引き分け")
				self.hakotext.stringValue = "引き分け！\n遊んでくれてありがとう！"
				self.hakoface.image = NSImage(named: "draw")
			}
		} else {
			switch winner() {
			case 1:
				print("黒の勝ち！")
				self.hakotext.stringValue = "僕の勝ち！\n遊んでくれてありがとう！"
				self.hakoface.image = NSImage(named: "lose")
			case 2:
				print("白の勝ち！")
				self.hakotext.stringValue = "君の勝ち！\n遊んでくれてありがとう！"
				self.hakoface.image = NSImage(named: "win")
			default:
				print("引き分け")
				self.hakotext.stringValue = "引き分け！\n遊んでくれてありがとう！"
				self.hakoface.image = NSImage(named: "draw")
			}
		}
		switchbutton(switch: false)
		hakotext.isEditable = false
	}
	
	func reloadview()
	{
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
		
		let legalboard: UInt64 = makelegalBoard(&playerboard, &oppenentboard)
		var mask: UInt64 = 0x8000000000000000
		
		for i in 0..<8
		{
			for j in 0..<8
			{
				if(nowTurn == BLACK_TURN) {
					if((playerboard & mask) != 0) {
						if(tmpx == j && tmpy == i) {
							buttons[i][j].image = NSImage(named: "blackb")
						} else {
							buttons[i][j].image = NSImage(named: "black")
						}
					} else if((oppenentboard & mask) != 0) {
						if(tmpx == j && tmpy == i) {
							buttons[i][j].image = NSImage(named: "whiteb")
						} else {
							buttons[i][j].image = NSImage(named: "white")
						}
					} else {
						if(botplayer == WHITE_TURN && (legalboard & mask) != 0) {
							buttons[i][j].image = NSImage(named: "null2")
						} else {
							buttons[i][j].image = NSImage(named: "null")
						}
					}
				} else {
					if((oppenentboard & mask) != 0) {
						if(tmpx == j && tmpy == i) {
							buttons[i][j].image = NSImage(named: "blackb")
						} else {
							buttons[i][j].image = NSImage(named: "black")
						}
					} else if((playerboard & mask) != 0) {
						if(tmpx == j && tmpy == i) {
							buttons[i][j].image = NSImage(named: "whiteb")
						} else {
							buttons[i][j].image = NSImage(named: "white")
						}
					} else {
						if(botplayer == BLACK_TURN && (legalboard & mask) != 0) {
							buttons[i][j].image = NSImage(named: "null2")
						} else {
							buttons[i][j].image = NSImage(named: "null")
						}
					}
				}
				mask = mask >> 1
			}
		}
		print("[*]完了")
		if(isFinished() == true)
		{
			result()
			performSegue(withIdentifier: "popup", sender: self)
			close.isHidden = false
			restart.isHidden = false
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
		@objc class func error_hako() {
			DispatchQueue.main.async {
				if let viewController = NSApplication.shared.keyWindow?.contentViewController as? ViewController {
					viewController.hakotext.stringValue = "ERROR"
				}
			}
		}
		@objc class func play_put_sound() {
			DispatchQueue.main.async {
				if let viewController = NSApplication.shared.keyWindow?.contentViewController as? ViewController {
					viewController.putsound()
				}
			}
		}
	}
}

