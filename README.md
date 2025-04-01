# Reversi

# 特徴

・C++

・NegaScout法

・ビットボード

・[Edax](https://github.com/abulmo/edax-reversi)、[Egaroucid](https://github.com/Nyanyan/Egaroucid)を参考にし、線形回帰を利用して最適化した評価関数

・残り20手で完全読み

# 使用方法

macOS版のみビルド済みアプリケーションを配布しています

このアプリケーションは署名されていないため、起動する前に
```
/usr/bin/xattr -cr /path/to/Reversi.app
```
を行う必要があります

# その他

依存関係: 
[Siv3d](https://siv3d.github.io/)

参考文献:

[オセロAIの教科書](https://note.com/nyanyan_cubetech/m/m54104c8d2f12)

[bitboard tricks](http://www.amy.hi-ho.ne.jp/okuhara/bitboard.htm)

[bitboardの回転](https://qiita.com/ysuzuk81/items/453b08a14d23fb8c6c11)

[Chess Programming Wiki](https://www.chessprogramming.org/Magic_Bitboards)

[アルゴリズム解説](https://sealsoft.jp/thell/algorithm.html)

[リバーシ評価関数の最適化](https://sealsoft.jp/thell/learning.pdf)


学習データには[Egaroucidの自己対戦の棋譜](https://www.egaroucid.nyanyan.dev/ja/technology/transcript/)を作者の許可を得て使用しています。

石評価パターンは[Edax](https://github.com/abulmo/edax-reversi), [Egaroucid](https://www.egaroucid.nyanyan.dev/ja/)を参考にしたものを利用しています。
