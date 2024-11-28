#include <Siv3D.hpp> // Siv3D v0.6.15
#include "reversi.hpp"

using namespace std;
using App = SceneManager<String>;

// builtin functions
// if you are using C++20, you can just use std::popcount(x) for popcount
#ifdef __GNUC__
#define popcountll(x) __builtin_popcountll(x)
#else
#define    popcountll(x) __popcnt64(x)
#endif

int stone_edge, stone_size;

void DrawBoard() {
    uint64_t mask = 0x8000000000000000ULL;
//    uint64_t legalboard = makelegalboard(b.playerboard, b.opponentboard);
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {
            if(nowTurn == BLACK_TURN) {
                if(b.playerboard & mask) {
                    if(tmpy == y && tmpx == x) {
                        TextureAsset(U"blackb").draw(stone_edge+stone_size*x, 10+stone_size*y);
                    } else {
                        TextureAsset(U"black").draw(stone_edge+stone_size*x, 10+stone_size*y);
                    }
                } else if(b.opponentboard & mask) {
                    if(tmpy == y && tmpx == x) {
                        TextureAsset(U"whiteb").draw(stone_edge+stone_size*x, 10+stone_size*y);
                    } else {
                        TextureAsset(U"white").draw(stone_edge+stone_size*x, 10+stone_size*y);
                    }
                } else if((botplayer == WHITE_TURN && (legalboard & mask) != 0)) {
                    TextureAsset(U"nullb").draw(stone_edge+stone_size*x, 10+stone_size*y);
                } else {
                    TextureAsset(U"null").draw(stone_edge+stone_size*x, 10+stone_size*y);
                }
            } else {
                if(b.opponentboard & mask) {
                    if(tmpy == y && tmpx == x) {
                        TextureAsset(U"blackb").draw(stone_edge+stone_size*x, 10+stone_size*y);
                    } else {
                        TextureAsset(U"black").draw(stone_edge+stone_size*x, 10+stone_size*y);
                    }
                } else if(b.playerboard & mask) {
                    if(tmpy == y && tmpx == x) {
                        TextureAsset(U"whiteb").draw(stone_edge+stone_size*x, 10+stone_size*y);
                    } else {
                        TextureAsset(U"white").draw(stone_edge+stone_size*x, 10+stone_size*y);
                    }
                } else if((botplayer == BLACK_TURN && (legalboard & mask) != 0)) {
                    TextureAsset(U"nullb").draw(stone_edge+stone_size*x, 10+stone_size*y);
                } else {
                    TextureAsset(U"null").draw(stone_edge+stone_size*x, 10+stone_size*y);
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
    Font result_font{FontMethod::MSDF, 20};
    //画像読み込み
    TextureAsset::Register(U"null", Resource(U"null.png"));
    TextureAsset::Register(U"nullb", Resource(U"nullb.png"));
    TextureAsset::Register(U"white", Resource(U"white.png"));
    TextureAsset::Register(U"whiteb", Resource(U"whiteb.png"));
    TextureAsset::Register(U"black", Resource(U"black.png"));
    TextureAsset::Register(U"blackb", Resource(U"blackb.png"));
    TextureAsset::Register(U"title", Resource(U"title.png"));
    TextureAsset::Register(U"hako_default", Resource(U"hako.png"));
    TextureAsset::Register(U"hako_thinking", Resource(U"thinking.png"));
    TextureAsset::Register(U"hako_lose", Resource(U"lose.png"));
    TextureAsset::Register(U"hako_win", Resource(U"win.png"));
    //サイズ取得 & 計算
    stone_edge = (800-(TextureAsset(U"null").width()*8))/2;
    stone_size = TextureAsset(U"null").width();
//    const auto title_size = TextureAsset(U"title").width();
    const auto title_edge = (800-TextureAsset(U"title").width())/2;
    Window::Resize(800, 700);
    Window::SetTitle(U"オセロ");
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
    Level = (int)level_index+1;
    const Array<String> AI_level = {U"⭐︎1", U"⭐︎2", U"⭐︎3", U"⭐︎4", U"⭐︎5", U"⭐︎6"};
    constexpr array<int, 6> level_to_depth = {1, 2, 4, 6, 10, 12};
    //白黒
    size_t bot_turn = 1;
    const Array<String> player_turn = {U"白", U"黒"};
    //ゲームが開始しているか
    int game_status = 0;
    int winner = 0;
    //AIの実行結果
    AsyncTask<int> result;
    //ハコくんのテキストBox
    constexpr Rect hako_text_box(260, 475, 400, 100);
    //ハコくんのテキスト
    const String hako_text_default = U"君の番だよ！\n置く場所を選んでね！";
    const String hako_text_win = U"君の勝ち！";
    const String hako_text_lose = U"僕の勝ち！";
    const String hako_text_draw = U"引き分け！";
    
    int black_stone_count = 0, white_stone_count = 0;
    
    
    while (System::Update()) {
        //メインメニュー
        if (game_status == 0) {
            TextureAsset(U"title").draw(title_edge, 0);
            font(U"レベル").draw(25, 10, 455);
            font(U"プレイヤー").draw(25, 200, 455);
            if (SimpleGUI::RadioButtons(level_index, AI_level, Vec2(100, 455))) {
                firstDEPTH = level_to_depth[level_index];
                Level = (int)level_index+1;
            }
            if (SimpleGUI::RadioButtons(bot_turn, player_turn, Vec2(340, 455))) {
                botplayer = (int)bot_turn;
            }
            if (SimpleGUI::Button(U"スタート", Vec2(350, 600))) {
                game_status = 1;
                reset();
            }
        //ゲーム中
        } else if (game_status == 1) {
            if (isFinished()) {
                game_status = 3;
                if (nowTurn == BLACK_TURN) {
                    black_stone_count = __builtin_popcountll(b.playerboard);
                    white_stone_count = __builtin_popcountll(b.opponentboard);
                } else {
                    white_stone_count = __builtin_popcountll(b.playerboard);
                    black_stone_count = __builtin_popcountll(b.opponentboard);
                }
                if ((botplayer == 0 && black_stone_count > white_stone_count) || (botplayer == 1 && white_stone_count > black_stone_count)) {
                    winner = 0;
                } else if ((botplayer == 1 && black_stone_count > white_stone_count) || (botplayer == 0 && white_stone_count > black_stone_count)) {
                        winner = 1;
                } else {
                    winner = 2;
                }
            }
            if (nowTurn == botplayer) {
                TextureAsset(U"hako_thinking").draw(200, 475);
                result_font(U"考え中...({:0>2}%)\n※時間がかかる場合があります"_fmt(think_percent)).draw(hako_text_box, Palette::White);
                if (!result.isValid()) {
                    result = Async(ai);
                }
                if (result.isReady()) {
                    if (result.get() == 1) {
                        swapboard();
                    }
                }
            } else {
                TextureAsset(U"hako_default").draw(200, 475);
                result_font(hako_text_default).draw(hako_text_box, Palette::White);
                
            }
            for (int i = 0; i < 64; ++i) {
                if (button_array[i].leftClicked() && button_array[i].mouseOver()) {
                    if(putstone(i/8, i%8) == 1) {
                        swapboard();
                    }
                }
            }
            if (isPass()) {
                swapboard();
            }
            if (SimpleGUI::Button(U"リセット", Vec2{350, 550}, 100)) {
                if (!result.isValid()) {
                    reset();
                }
            }
            if (SimpleGUI::Button(U"閉じる", Vec2{350, 600}, 100)) {
                if (!result.isValid()) {
                    game_status = 0;
                }
            }
            DrawBoard();
        //ゲーム終了
        } else {
            result_font(U"黒: {:0>2}, 白: {:0>2}"_fmt(black_stone_count, white_stone_count)).drawAt(400, 600);
            result_font(U"勝者: {}"_fmt((black_stone_count > white_stone_count) ? U"黒" : (black_stone_count < white_stone_count) ? U"白" : U"引き分け")).drawAt(400, 625);
            switch (winner) {
                case 0:
                    TextureAsset(U"hako_lose").draw(200, 475);
                    result_font(hako_text_lose).draw(hako_text_box, Palette::White);
                    break;
                case 1:
                    TextureAsset(U"hako_win").draw(200, 475);
                    result_font(hako_text_win).draw(hako_text_box, Palette::White);
                    break;
                default:
                    TextureAsset(U"hako_draw").draw(200, 475);
                    result_font(hako_text_draw).draw(hako_text_box, Palette::White);
                    break;
            }
            if (SimpleGUI::Button(U"閉じる", Vec2{350, 650}, 100)) {
                game_status = 0;
            }
            DrawBoard();
        }
    }
    if (result.isValid()) {
        result.wait();
    }
    
}
