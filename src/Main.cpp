#include <Siv3D.hpp> // Siv3D v0.6.15
#include "reversi.h"
#include <array>

using namespace std;
using App = SceneManager<String>;

void DrawBoard(const Texture &null, const Texture &nullb, const Texture &black, const Texture &blackb, const Texture &white, const Texture &whiteb, const auto &stone_size, const auto &stone_edge) {
    uint64_t mask = 0x8000000000000000ULL;
    uint64_t legalboard = makelegalboard(b.playerboard, b.opponentboard);
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if(nowTurn == BLACK_TURN) {
                if(b.playerboard & mask) {
                    if(tmpy == y && tmpx == x) {
                        blackb.draw(stone_edge+stone_size*x, 10+stone_size*y);
                    } else {
                        black.draw(stone_edge+stone_size*x, 10+stone_size*y);
                    }
                } else if(b.opponentboard & mask) {
                    if(tmpy == y && tmpx == x) {
                        whiteb.draw(stone_edge+stone_size*x, 10+stone_size*y);
                    } else {
                        white.draw(stone_edge+stone_size*x, 10+stone_size*y);
                    }
                } else if((botplayer == WHITE_TURN && (legalboard & mask) != 0)) {
                    nullb.draw(stone_edge+stone_size*x, 10+stone_size*y);
                } else {
                    null.draw(stone_edge+stone_size*x, 10+stone_size*y);
                }
            } else {
                if(b.opponentboard & mask) {
                    if(tmpy == y && tmpx == x) {
                        blackb.draw(stone_edge+stone_size*x, 10+stone_size*y);
                    } else {
                        black.draw(stone_edge+stone_size*x, 10+stone_size*y);
                    }
                } else if(b.playerboard & mask) {
                    if(tmpy == y && tmpx == x) {
                        whiteb.draw(stone_edge+stone_size*x, 10+stone_size*y);
                    } else {
                        white.draw(stone_edge+stone_size*x, 10+stone_size*y);
                    }
                } else if((botplayer == BLACK_TURN && (legalboard & mask) != 0)) {
                    nullb.draw(stone_edge+stone_size*x, 10+stone_size*y);
                } else {
                    null.draw(stone_edge+stone_size*x, 10+stone_size*y);
                }
            }
            mask >>=1;
        }
    }
    return;
}

void Main()
{
    firstDEPTH = 10;
    botplayer = WHITE_TURN;
    //盤面初期化
    reset();
    fixedstone_table_init();
    //テキストのフォント
    Font font{FontMethod::MSDF, 48};
    //画像読み込み
    const Texture null(U"../assets/null.png");
    const Texture nullb(U"../assets/nullb.png");
    const Texture white(U"../assets/white.png");
    const Texture whiteb(U"../assets/whiteb.png");
    const Texture black(U"../assets/black.png");
    const Texture blackb(U"../assets/blackb.png");
    const Texture title(U"../assets/title.png");
    //サイズ取得 & 計算
    const auto stone_edge = (800-(null.width()*8))/2;
    const auto stone_size = null.width();
    const auto title_size = title.width();
    const auto title_edge = (800-title.width())/2;
    Window::Resize(800, 700);
    //石の座標の配列
    Array<Rect> button_array(0);
    button_array.reserve(64);
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            button_array.emplace_back(Rect(stone_edge+stone_size*x, 10+stone_size*y, stone_size, stone_size));
        }
    }
    //AIのレベル
    size_t level_index = 4;
    const Array<String> AI_level = {U"⭐︎1", U"⭐︎2", U"⭐︎3", U"⭐︎4", U"⭐︎5", U"⭐︎6"};
    constexpr array<int, 6> level_to_depth = {1, 3, 5, 7, 10, 12};
    //白黒
    size_t bot_turn = 1;
    const Array<String> player_turn = {U"白", U"黒"};
    //ゲームが開始しているか
    int game_status = 0;
    //AIの実行結果
    AsyncTask<int> result;
    
    while (System::Update()) {
        if (game_status == 0) {
            title.draw(title_edge, 0);
            font(U"レベル").draw(25, 10, 455);
            font(U"プレイヤー").draw(25, 200, 455);
            if (SimpleGUI::RadioButtons(level_index, AI_level, Vec2(100, 455))) {
                firstDEPTH = level_to_depth[level_index];
                Level = level_index+1;
            }
            if (SimpleGUI::RadioButtons(bot_turn, player_turn, Vec2(340, 455))) {
                botplayer = bot_turn;
            }
            if (SimpleGUI::Button(U"スタート", Vec2(350, 600))) {
                game_status = 1;
                reset();
            }
        } else if (game_status == 1) {
            if (isFinished()) {
                game_status = 3;
            }
            if (nowTurn == botplayer) {
                if (!result.isValid()) {
                    System::Sleep(200);
                    result = Async(ai);
                }
                if (result.isReady()) {
                    if (result.get() == 1) {
                        swapboard();
                    }
                }
            }
            for (int i = 0; i < 64; ++i) {
                if (button_array[i].leftClicked() && button_array[i].mouseOver()) {
                    if(putstone(i/8, i%8) == 1) {
                        swapboard();
                    }
                }
            }
            if (isPass()) swapboard();
            if (SimpleGUI::Button(U"リセット", Vec2{350, 550}, 100)) {
                reset();
            }
            DrawBoard(null, nullb, black, blackb, white, whiteb, stone_size, stone_edge);
        } else {
            if (nowTurn == BLACK_TURN) {
                font(U"黒: {}, 白: {}"_fmt(__builtin_popcountll(b.playerboard),__builtin_popcountll(b.opponentboard))).draw(300, 550);
            }
            DrawBoard(null, nullb, black, blackb, white, whiteb, stone_size, stone_edge);
        }
    }
    if (result.isValid()) {
        result.wait();
    }
    
}
