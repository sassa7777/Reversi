#include <Siv3D.hpp> // Siv3D v0.6.16
#include "reversi.hpp"

using namespace std;
using App = SceneManager<String>;

int stone_edge, stone_size;

struct Drawable {
    int x;
    int y;
    String textureKey;
};

vector<Drawable> drawables;

void DrawBoard() {
    for (const auto& drawable : drawables) {
        TextureAsset(drawable.textureKey).draw(stone_edge + stone_size * drawable.x,
                                                10 + stone_size * drawable.y);
    }
}

void sync_board_data() {
    uint64_t mask = 0x8000000000000000ULL;
    for (int y = 0; y < 8; ++y) {
        for (int x = 0; x < 8; ++x) {

            bool isBlackTurn = (nowTurn == BLACK_TURN);
            bool stoneBlack = isBlackTurn ? (b.p & mask) : (b.o & mask);
            bool stoneWhite = isBlackTurn ? (b.o & mask) : (b.p & mask);
            
            bool legal = isBlackTurn ? (AIplayer == WHITE_TURN && (legalboard & mask))
                                     : (AIplayer == BLACK_TURN && (legalboard & mask));
            String textureName;
            if (stoneBlack) {
                textureName = (tmpy == y && tmpx == x) ? U"blackb" : U"black";
            }
            else if (stoneWhite) {
                textureName = (tmpy == y && tmpx == x) ? U"whiteb" : U"white";
            }
            else if (legal) {
                textureName = (hint_y == y && hint_x == x) ? U"nullc" : U"nullb";
            }
            else {
                textureName = U"null";
            }
            
            TextureAsset(textureName).draw(stone_edge + stone_size * x, 10 + stone_size * y);
            drawables.push_back({x, y, textureName});
            mask >>= 1;
        }
    }
}

void Main()
{
    Scene::SetBackground(ColorF{ 0.6, 0.8, 0.7 });
    //テキストのフォント
    Font font{FontMethod::MSDF, 48};
    Font result_font{FontMethod::MSDF, 20};
    //画像読み込み
    TextureAsset::Register(U"null", Resource(U"null.png"));
    TextureAsset::Register(U"nullb", Resource(U"nullb.png"));
    TextureAsset::Register(U"nullc", Resource(U"nullc.png"));
    TextureAsset::Register(U"white", Resource(U"white.png"));
    TextureAsset::Register(U"whiteb", Resource(U"whiteb.png"));
    TextureAsset::Register(U"black", Resource(U"black.png"));
    TextureAsset::Register(U"blackb", Resource(U"blackb.png"));
    TextureAsset::Register(U"title", Resource(U"title.png"));
    TextureAsset::Register(U"hako_default", Resource(U"hako.png"));
    TextureAsset::Register(U"hako_thinking", Resource(U"thinking.png"));
    TextureAsset::Register(U"hako_lose", Resource(U"lose.png"));
    TextureAsset::Register(U"hako_win", Resource(U"win.png"));
    TextureAsset::Register(U"hako_draw", Resource(U"draw.png"));
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
    AIplayer = WHITE_TURN;
    //AIのレベル
    double level_index = 14;
    firstDEPTH = round(level_index);
    Level = level_index;
    const Array<String> AI_level = {U"⭐︎1", U"⭐︎2", U"⭐︎3", U"⭐︎4", U"⭐︎5", U"⭐︎6"};
    //白黒
    size_t AI_turn = 1;
    const Array<String> player_turn = {U"白", U"黒"};
    //ゲームが開始しているか
    int game_status = -1;
    int winner = 0;
    //AIの実行結果
    AsyncTask<int> result;
    AsyncTask<void> init_result;
    //ハコくんのテキストBox
    constexpr Rect hako_text_box(260, 475, 400, 100);
    //ハコくんのテキスト
    const String hako_text_default = U"君の番だよ！\n置く場所を選んでね！";
    const String hako_text_win = U"君の勝ち！";
    const String hako_text_lose = U"僕の勝ち！";
    const String hako_text_draw = U"引き分け！";
    
    int black_stone_count = 0, white_stone_count = 0;
    
    bool running_hint = false;
    first_reset = true;
    while (System::Update()) {
        //メインメニュー
        if (game_status == -1) {
            if (!init_result.isValid()) {
                init_result = Async(reset);
            }
            if (init_result.isReady()) {
                game_status = 0;
            }
            TextureAsset(U"title").draw(title_edge, 0);
            font(U"ロード中...").draw(350, 600);
        } else if (game_status == 0) {
            TextureAsset(U"title").draw(title_edge, 0);
            font(U"レベル").draw(25, 10, 455);
            font(U"プレイヤー").draw(25, 200, 455);
            if (SimpleGUI::Slider(U"レベル {:2}"_fmt(level_index), level_index, 1, 18, Vec2(10, 505), 90.0, 130.0)) {
                level_index = round(level_index);
                firstDEPTH = level_index;
                Level = level_index;
            }
            if (SimpleGUI::RadioButtons(AI_turn, player_turn, Vec2(340, 455))) {
                AIplayer = (int)AI_turn;
            }
            if (SimpleGUI::Button(U"スタート", Vec2(350, 600))) {
                game_status = 1;
                reset();
                sync_board_data();
            }
        //ゲーム中
        } else if (game_status == 1) {
            if (isFinished()) {
                game_status = 3;
                if (nowTurn == BLACK_TURN) {
                    black_stone_count = popcount(b.p);
                    white_stone_count = popcount(b.o);
                } else {
                    white_stone_count = popcount(b.p);
                    black_stone_count = popcount(b.o);
                }
                if ((AIplayer == 0 && black_stone_count > white_stone_count) || (AIplayer == 1 && white_stone_count > black_stone_count)) {
                    winner = 0;
                } else if ((AIplayer == 1 && black_stone_count > white_stone_count) || (AIplayer == 0 && white_stone_count > black_stone_count)) {
                        winner = 1;
                } else {
                    winner = 2;
                }
            }
            if (nowTurn == AIplayer) {
                TextureAsset(U"hako_thinking").draw(200, 475);
                result_font(U"考え中...({:0>2}%)\n※時間がかかる場合があります"_fmt(think_percent)).draw(hako_text_box, Palette::White);
                if (!result.isValid()) {
                    result = Async(ai);
                }
                if (result.isReady()) {
                    if (result.get() == 1) {
                        swapboard();
                        sync_board_data();
                    }
                }
            } else if (running_hint) {
                TextureAsset(U"hako_thinking").draw(200, 475);
                result_font(U"考え中...({:0>2}%)\n※時間がかかる場合があります"_fmt(think_percent)).draw(hako_text_box, Palette::White);
                if (!result.isValid()) {
                    result = Async(ai_hint);
                }
                if (result.isReady()) {
                    running_hint = false;
                    swapboard();
                    sync_board_data();
                }
            } else {
                TextureAsset(U"hako_default").draw(200, 475);
                result_font(hako_text_default).draw(hako_text_box, Palette::White);
                
            }
            if (!running_hint) {
                for (int i = 0; i < 64; ++i) {
                    if (button_array[i].leftClicked() && button_array[i].mouseOver()) {
                        if(putstone(i/8, i%8) == 1) {
                            swapboard();
                            sync_board_data();
                        }
                    }
                }
            }
            if (isPass()) {
                swapboard();
                sync_board_data();
            }
            if (SimpleGUI::Button(U"リセット", Vec2{350, 550}, 100)) {
                if (!result.isValid()) {
                    reset();
                    sync_board_data();
                }
            }
            if (SimpleGUI::Button(U"閉じる", Vec2{350, 600}, 100)) {
                if (!result.isValid()) {
                    game_status = 0;
                }
            }
            if (SimpleGUI::Button(U"1手戻す \U000F054C", Vec2{500, 550}, 100) && nowIndex > 1) {
                b.p = b_back.p;
                b.o = b_back.o;
                tmpx = b_back.put_x;
                tmpy = b_back.put_y;
                legalboard = makelegalboard(b);
                sync_board_data();
            }
            if (SimpleGUI::Button(U"ヒント", Vec2{500, 600}, 100)) {
                running_hint = true;
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
