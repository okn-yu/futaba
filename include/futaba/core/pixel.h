/*
 * Created by okn-yu on 2022/05/08.
 */

/*
 *  ピクセルに関連する用語の整理
 *
 *  解像度:
 *   解像度は2つの解像度（A.画面解像度(1280x960)とB.画像解像度(dpi)）が混在して使われている
 *   A.画面解像度:画面に存在する画素の総数
 *    1280x960なら画面の横に1280画素/縦に720画素存在している
 *    ハイビジョン: 1,280×720
 *    フルハイビジョン:1,920×1,080
 *    4K:3,840×2,160
 *    8K:7,680×4,320
 *
 *   B.画像解像度:単位長さ(1inch)あたりに含まれる画素の総数
 *    画面解像度が与えられた場合、画像解像度がいくつであるかはディスプレイのサイズに依存する
 *
 *  ビット深度:
 *   RGBの各色に割り当てられるビット数
 *   8bitの場合は256階調を表示ができる
 *   ビット深度に関しては以下を参照のこと
 *   RGBの各色に12ビットや14ビットを割り当てる場合もある（ディープカラー）
 */

/*
 * ガンマ補正に関して
 *
 * ガンマ値:
 * 画像の階調の応答特性
 * ディスプレイなどの出力機器では入力値（画像の輝度）と出力値（ディスプレイ上の表示）は線形ではなくV_out = V_in^γの非線形な関係となる
 * このべき乗の値γをガンマ値と呼ぶ
 * CRTディスプレイ（ブラウン管）ではガンマ値は2.2前後（つまり下に凸な関係）
 * 液晶ディスプレイでもCRTディスプレイと同程度になるように調整がされている
 * この場合出力機器上では実際よりも全体として黒ずんで見える
 *
 * 歴史的経緯:
 * テレビ放送の際にCRTディスプレイの上の特性を考慮して放送局側で予めガンマ補整をかけた信号を送信していた（上に凸な補正をかけていた）
 * 一方受信側のCRTディスプレイでは、表示する際に下に凸に補正がかけられ双方が打ち消し合って適切な画像の表示が実現されていた
 * 一方偶然ではあるが人の目は低輝度の変化に敏感で高輝度には鈍感なため都合が良かった(ロウソクの数が倍々に増えていっても人の眼には大体等間隔で明るく見える)
 *
 * ガンマ補正:
 * 画像の色の明暗が出力機器で正しく表示されるように対象機器のガンマ値に応じた色の補正を行うこと
 * ディスプレイなどの出力機器で画像をそのまま表示すると画面が暗くなってしまうため、RGBの値を予め大きくすることで出力機器の特性を排除した画像表示を行う
 * 具体的にはガンマ値とは逆にV_out = V_in^(-γ)の変換を行う
 * ガンマ値の具体的な値はOSにより異なる
 * Windows系のOS:
 *  ガンマ値は2.2のディスプレイを前提にしている
 * Mac系のOS:
 *  ガンマ値は1.8のディスプレイを前提にしている
 * またディスプレイによりガンマ値は異なるため、ガンマ値2.2のディスプレイで最適な画像をガンマ値1.8のディスプレイで表示する場合を考える
 * 実際のガンマ値はγ=1.8/2.2≒0.82となるため意図した画像よりも明るくなる
 *
 * ガンマ補正と画像処理
 *  ガンマ補正は（ディスプレイとは別に）画像解析の手法の1つとしても利用ができる
 *  ガンマ値が小さいと輝度の大きな値が強調されるようになる
 *  ガンマ値が大きいと輝度の小さな値が強調されるようになる
 *  http://rs.aoyaman.com/seminar/about10.html
 *  ガンマ補正の問題点は色深度が8ビット（つまり256階調）ではガンマ補正により低輝度が潰れる
 *
 */

/*
 * C++のコンストラクタの種類
 *
 * デフォルトコンストラクタ:
 * 引数なしのコンストラクタを指す
 * デフォルトコンストラクタを呼び出す場合に()は不要で=の左辺のみでよい
 *
 * 移譲コンストラクタ:
 * 複数のコンストラクタがある場合に内部で特定のコンストラクタを呼び出すコンストラクタ
 * 例えばデフォルトコンストラクタが引数付きのコンストラクタの特殊な場合として扱える場合など
 * 実装の見通しが良くなる
 *
 * コピーコンストラクタ:
 * デフォルトコピーコンストラクタ:
 * 自分と同じクラスのインスタンスを引数とするコンストラクタ
 * メンバ変数をすべてコピーしたものが新しく作られる
 *
 */

/*
 * C++で親クラスのコンストラクタの呼び出し
 * Pythonのsuper()のような呼び出し方はない
 * 子クラスのコンストラクタ():親クラスのコンストラクタ(){}のようにして呼び出す
 */

#ifndef PRACTICEPATHTRACING_PIXEL_H
#define PRACTICEPATHTRACING_PIXEL_H

#include <array>
#include <cmath>
#include <iostream>
#include <map>
#include <stack>
#include "futaba/core/config.h"
#include "futaba/core/util.h"
#include "futaba/core/vec3.h"

/*
 * ガンマ補正用のルックアップテーブル
 * ガンマ補正は指数計算を行うため計算コストが高い
 * そのため予め0-255の各値に対してガンマ補正後の値を計算し、ガンマ補正実行時は参照のみを行う
 */

/*
 * リンク時のシンボルの多重定義エラーを回避するため無名名前空間を利用
 * 無名名前空間のスコープ内で宣言された変数や関数は、内部リンケージを持つ
 * 他のファイルから参照できない変数や関数を宣言可能
 * ファイル内部のみしか参照できないためリンク時のシンボルの多重定義を回避できる
 *
 * https://marycore.jp/prog/cpp/unnamed-namespace/
 * https://qiita.com/elipmoc101/items/01003c82dbd2e464a071
 * http://www7b.biglobe.ne.jp/~robe/cpphtml/html01/cpp01069.html
 */

namespace {
    std::map<uint8_t, uint8_t> Gamma_LUT{
            {0,   0},
            {1,   11},
            {2,   17},
            {3,   21},
            {4,   25},
            {5,   28},
            {6,   31},
            {7,   34},
            {8,   37},
            {9,   39},
            {10,  42},
            {11,  44},
            {12,  46},
            {13,  48},
            {14,  50},
            {15,  52},
            {16,  54},
            {17,  56},
            {18,  58},
            {19,  60},
            {20,  61},
            {21,  63},
            {22,  65},
            {23,  67},
            {24,  68},
            {25,  70},
            {26,  71},
            {27,  73},
            {28,  74},
            {29,  76},
            {30,  77},
            {31,  79},
            {32,  80},
            {33,  81},
            {34,  83},
            {35,  84},
            {36,  85},
            {37,  87},
            {38,  88},
            {39,  89},
            {40,  91},
            {41,  92},
            {42,  93},
            {43,  94},
            {44,  96},
            {45,  97},
            {46,  98},
            {47,  99},
            {48,  100},
            {49,  101},
            {50,  103},
            {51,  104},
            {52,  105},
            {53,  106},
            {54,  107},
            {55,  108},
            {56,  109},
            {57,  110},
            {58,  112},
            {59,  113},
            {60,  114},
            {61,  115},
            {62,  116},
            {63,  117},
            {64,  118},
            {65,  119},
            {66,  120},
            {67,  121},
            {68,  122},
            {69,  123},
            {70,  124},
            {71,  125},
            {72,  126},
            {73,  127},
            {74,  128},
            {75,  129},
            {76,  130},
            {77,  131},
            {78,  132},
            {79,  132},
            {80,  133},
            {81,  134},
            {82,  135},
            {83,  136},
            {84,  137},
            {85,  138},
            {86,  139},
            {87,  140},
            {88,  141},
            {89,  142},
            {90,  142},
            {91,  143},
            {92,  144},
            {93,  145},
            {94,  146},
            {95,  147},
            {96,  148},
            {97,  149},
            {98,  149},
            {99,  150},
            {100, 151},
            {101, 152},
            {102, 153},
            {103, 154},
            {104, 154},
            {105, 155},
            {106, 156},
            {107, 157},
            {108, 158},
            {109, 159},
            {110, 159},
            {111, 160},
            {112, 161},
            {113, 162},
            {114, 163},
            {115, 163},
            {116, 164},
            {117, 165},
            {118, 166},
            {119, 166},
            {120, 167},
            {121, 168},
            {122, 169},
            {123, 170},
            {124, 170},
            {125, 171},
            {126, 172},
            {127, 173},
            {128, 173},
            {129, 174},
            {130, 175},
            {131, 176},
            {132, 176},
            {133, 177},
            {134, 178},
            {135, 179},
            {136, 179},
            {137, 180},
            {138, 181},
            {139, 182},
            {140, 182},
            {141, 183},
            {142, 184},
            {143, 184},
            {144, 185},
            {145, 186},
            {146, 187},
            {147, 187},
            {148, 188},
            {149, 189},
            {150, 189},
            {151, 190},
            {152, 191},
            {153, 191},
            {154, 192},
            {155, 193},
            {156, 194},
            {157, 194},
            {158, 195},
            {159, 196},
            {160, 196},
            {161, 197},
            {162, 198},
            {163, 198},
            {164, 199},
            {165, 200},
            {166, 200},
            {167, 201},
            {168, 202},
            {169, 202},
            {170, 203},
            {171, 204},
            {172, 204},
            {173, 205},
            {174, 206},
            {175, 206},
            {176, 207},
            {177, 208},
            {178, 208},
            {179, 209},
            {180, 210},
            {181, 210},
            {182, 211},
            {183, 212},
            {184, 212},
            {185, 213},
            {186, 214},
            {187, 214},
            {188, 215},
            {189, 215},
            {190, 216},
            {191, 217},
            {192, 217},
            {193, 218},
            {194, 219},
            {195, 219},
            {196, 220},
            {197, 220},
            {198, 221},
            {199, 222},
            {200, 222},
            {201, 223},
            {202, 224},
            {203, 224},
            {204, 225},
            {205, 225},
            {206, 226},
            {207, 227},
            {208, 227},
            {209, 228},
            {210, 228},
            {211, 229},
            {212, 230},
            {213, 230},
            {214, 231},
            {215, 231},
            {216, 232},
            {217, 233},
            {218, 233},
            {219, 234},
            {220, 234},
            {221, 235},
            {222, 236},
            {223, 236},
            {224, 237},
            {225, 237},
            {226, 238},
            {227, 239},
            {228, 239},
            {229, 240},
            {230, 240},
            {231, 241},
            {232, 241},
            {233, 242},
            {234, 243},
            {235, 243},
            {236, 244},
            {237, 244},
            {238, 245},
            {239, 245},
            {240, 246},
            {241, 247},
            {242, 247},
            {243, 248},
            {244, 248},
            {245, 249},
            {246, 249},
            {247, 250},
            {248, 251},
            {249, 251},
            {250, 252},
            {251, 252},
            {252, 253},
            {253, 253},
            {254, 254},
            {255, 255}

    };
}

/*
 * GrayPixel構造体:
 * RGBPixel構造体とは別にGrayPixel構造体の実装も検討した
 * しかしstb_image_write.hでは保存形式がGrayかRGBかは引数で指定ができる
 * そのためすべてRGBPixelとして受け取り必要な場合に引数でGrayとして保存すれば良い
 * よってGrayPixel構造体の実装は削除した
 *
 * RGBPixel構造体:
 * 最終的なレンダリング結果の画像の輝度は0-255の整数値を取る
 * 一方uint8_tは0未満や256以上の整数値に対しては不適切な値に変換されてしまう
 * この事象を防ぐために受け取る値は全てint型で受けて値が適切な範囲内かを確認している
 *
 * ColorクラスからRGBPixel構造体への変換:
 * Colorクラス(Vecクラス)の各RGB値は0以上1以下のfloat型
 * Pixel構造体の各輝度は0以上255以下のint型
 * そのためColorクラスからPixel構造体への変換時に
 *
 */

class Pixel {
public:
    std::array<uint8_t, 3> data{};

    Pixel() {
        data[0] = 0;
        data[1] = 0;
        data[2] = 0;
    }

    explicit Pixel(Color col){
        float r = pixalize(col.x());
        float g = pixalize(col.y());
        float b = pixalize(col.z());


        auto R = static_cast<int>(r * 255);
        auto G = static_cast<int>(g * 255);
        auto B = static_cast<int>(b * 255);

        data[0] = Gamma_LUT[R];
        data[1] = Gamma_LUT[G];
        data[2] = Gamma_LUT[B];
    }

    uint8_t r() {
        return data[0];
    }

    uint8_t g() {
        return data[1];
    }

    uint8_t b() {
        return data[2];
    }

    Pixel &operator=(const Pixel &src) = default;

private:
    virtual float pixalize(float f) const{

        return clamp(f, 0.0f, 1.0f);
    };
};

class RGBPixel : public Pixel{
public:
    RGBPixel():Pixel() {}
    explicit RGBPixel(Color col): Pixel(col){}
/*
private:
    float pixalize(float f)const override{
        return clamp(f, 0.0f, 1.0f);
    }*/
};

class NormalPixel : public Pixel{
public:
    NormalPixel():Pixel() {}
    explicit NormalPixel(Color col): Pixel(col){}

private:
    float pixalize(float f)const override{
        float p =  clamp(f, -1.0f, 1.0f);
        return (p + 1) / 2;
    }
};

//bool operator==(const RGBPixel &src, const RGBPixel &dst) {
//    return src.data == dst.data;
//}
//
//bool operator!=(const RGBPixel &src, const RGBPixel &dst) {
//    return src.data != dst.data;
//}
//
//std::ostream &operator<<(std::ostream &stream, const RGBPixel &pixel) {
//    stream << "(" << pixel.data[0] << pixel.data[1] << pixel.data[2] << ")";
//    return stream;
//}
//
//using NormalPixel = RGBPixel;

#endif //PRACTICEPATHTRACING_PIXEL_H