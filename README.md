# Reversi
C++で書かれています。
# 特徴
・NegaScout法
・ビットボード
・深層学習を利用した評価関数のモデル(パターン評価)
・MagicBitboardを利用した高速な評価関数の実行

レベル10以上は残り20手で完全読みを行います。

このアプリケーションは署名されていないため、起動する前に
```
/usr/bin/xattr -cr /path/to/Reversi.app
```
を行う必要があります

依存関係: 
[unordered_dense](https://github.com/martinus/unordered_dense),
[Siv3d](https://siv3d.github.io/)

参考にしたサイト:
[オセロAIの教科書](https://note.com/nyanyan_cubetech/m/m54104c8d2f12),
[bitboard tricks](http://www.amy.hi-ho.ne.jp/okuhara/bitboard.htm),
[bitboardの回転](https://qiita.com/ysuzuk81/items/453b08a14d23fb8c6c11)

評価関数の学習と読み込みには、[OthelloAI_Textbook](https://github.com/Nyanyan/OthelloAI_Textbook)のコードをベースに改変したものを、

学習データの作成には[Egaroucidの自己対戦の棋譜](https://www.egaroucid.nyanyan.dev/ja/technology/transcript/)を作者の許可を得て使用しています。

石評価パターンは[Edax](https://github.com/abulmo/edax-reversi), [Egaroucid](https://www.egaroucid.nyanyan.dev/ja/)を参考にしたものを利用しています。
