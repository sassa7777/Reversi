# Reversi
C++で書かれていて、NegaScout法/ビットボードを使用しています。

レベル5以上は残り25手で完全読みを行います。

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
[bitboard tricks](http://www.amy.hi-ho.ne.jp/okuhara/bitboard.htm)

評価関数の学習には、[OthelloAI_Textbook](https://github.com/Nyanyan/OthelloAI_Textbook)のscriptを作者の許可を得て改変して使用しています。

石評価パターンは[Logistelloで提案された](https://skatgame.net/mburo/ps/improve.pdf)ものをそのまま利用しています。
