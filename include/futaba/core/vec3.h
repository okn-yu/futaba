/*
 * Created by okn-yu on 2022/05/02.
 *
 * Vec3クラス
 *
 * Note:
 * レイや座標や色など数値が3つセットで1つのオブジェクトは数多く存在する
 * それらを全てVec3クラスとして実装可能であり個別にクラスを定義しても良い
 * ここでは座標クラスをVec3のエイリアスでPoint3として定義している
 * 座標クラスは実体としてはVec3クラスと等価である
 *
 * C++の文法補足:
 * 変換コンストラクタ
 * constメンバ関数
 * const引数
 * エイリアス宣言
 * inline展開
 */

#ifndef PRACTICEPATHTRACING_VEC3_H
#define PRACTICEPATHTRACING_VEC3_H

#include <array>
#include <cassert>
#include <cmath>
#include <iostream>


class Vec3 {
public:
    std::array<float, 3> elements;

    Vec3() { elements[0] = elements[1] = elements[2] = 0.0f; };

    /*
     * 引数が1つだけのコンストラクタを"変換コンストラクタ"と呼ぶ
     * 変換コンストラクタは意図しない暗黙的な変換を防ぐためexplicit修飾子を先頭に付ける必要がある
     */
    explicit Vec3(float _x) { elements[0] = elements[1] = elements[2] = _x; };

    Vec3(float _x, float _y, float _z) {
        elements[0] = _x;
        elements[1] = _y;
        elements[2] = _z;
    };

    float x() const {
        return elements[0];
    }

    float y() const {
        return elements[1];
    }

    float z() const {
        return elements[2];
    }

    /*
     * メンバ関数の右側にconstをつけたconstメンバ関数では、関数内部でメンバ変数の変更が行われなくなる
     * 意図しない動作を防ぐためにも常にconstをつけるのが望ましい
     */
    float length() const {
        return std::sqrt(elements[0] * elements[0] + elements[1] * elements[1] + elements[2] * elements[2]);
    };

    float squared_length() const {
        //return elements[0] * elements[0] + elements[1] * elements[1] + elements[2] * elements[2];
        return length() * length();
    };

    /*
     * operatorは演算子のオーバーロードに用いる
     */
    Vec3 operator-() const {
        return {-elements[0], -elements[1], -elements[2]};
    }

    /*
     * C++には"値渡し" "ポインタ渡し" "参照渡し"の3種類が存在する
     * "ポインタ渡し"はポインタを直接操作するためnullチェックが必要になる
     * "参照渡し"は”より安全で制約の厳しいポインタ”と見なすことができる
     * そのため原則としてポインタ渡しよりは参照渡しのほうを使うべきである
     * https://qiita.com/agate-pris/items/05948b7d33f3e88b8967
     */

    /*
     * const引数:
     * 引数にconstをつけると関数内部でその引数が変更されないことが保証される
     * 意図しない動作を防ぐためにも常にconstをつけるのが望ましい
     */
    Vec3 &operator+=(const Vec3 &v) {
        elements[0] += v.elements[0];
        elements[1] += v.elements[1];
        elements[2] += v.elements[2];
        /*
         * thisは"thisポインタ"でオブジェクト自身のポインタを表している
         * *をつけることでポインタの参照先を表している
         */
        return *this;
    }

    Vec3 &operator-=(const Vec3 &v) {
        elements[0] -= v.elements[0];
        elements[1] -= v.elements[1];
        elements[2] -= v.elements[2];
        return *this;
    }

    Vec3 &operator*=(const Vec3 &v) {
        elements[0] *= v.elements[0];
        elements[1] *= v.elements[1];
        elements[2] *= v.elements[2];
        return *this;
    };

    Vec3 &operator*=(const float k) {
        elements[0] *= k;
        elements[1] *= k;
        elements[2] *= k;
        return *this;
    };

    Vec3 &operator/=(const Vec3 &v) {
        elements[0] /= v.elements[0];
        elements[1] /= v.elements[1];
        elements[2] /= v.elements[2];
        return *this;
    };

    Vec3 &operator/=(const float k) {
        elements[0] /= k;
        elements[1] /= k;
        elements[2] /= k;
        return *this;
    };
};

/*
 * operator: vector and vector
 */

/*
 * inline展開: 高速化のため関数の内容を直接呼び出し元に展開すること
 * inline展開を用いない場合は、関数呼び出しには関数の定義元のアドレスまで参照するため時間がかかる
 * 明示的にinline修飾子をつけることでコンパイラにinline展開を強制させる
 * ただし以下のような欠点も存在する
 * inline展開を多用するとソースコードが肥大化しビルド時間が増える
 * 人ではなくコンパイラにinlineの利用を決定させるほうが高速化する場合がある
 * https://qiita.com/omuRice/items/9e31d9ba17b32703b3b1
 *
 * またinlineを付与すると定義ではなくなるため、duplicate symbolエラーを回避することができる
 * https://qiita.com/Luke02561/items/43bed4008dd707617a94
 * インクルードガードではある.cppファイル内でヘッダファイルの重複は回避できる（逆に言うと一度は読み込まれている）
 * しかし複数の.oファイル同士をリンクする場合は、それぞれで1度だけ展開されたヘッダファイル内の定義同士が衝突しうる
 *
 * 二重定義はコンパイル時とリンク時に起きる問題であり、それぞれ原因が異なる
 * http://www7b.biglobe.ne.jp/~robe/cpphtml/html01/cpp01070.html
 */

inline Vec3 operator+(const Vec3 &v1, const Vec3 &v2) {
    return {v1.elements[0] + v2.elements[0], v1.elements[1] + v2.elements[1], v1.elements[2] + v2.elements[2]};
}

inline Vec3 operator-(const Vec3 &v1, const Vec3 &v2) {
    return {v1.elements[0] - v2.elements[0], v1.elements[1] - v2.elements[1], v1.elements[2] - v2.elements[2]};
}

inline Vec3 operator*(const Vec3 &v1, const Vec3 &v2) {
    return {v1.elements[0] * v2.elements[0], v1.elements[1] * v2.elements[1], v1.elements[2] * v2.elements[2]};
}

inline Vec3 operator/(const Vec3 &v1, const Vec3 &v2) {
    return {v1.elements[0] / v2.elements[0], v1.elements[1] / v2.elements[1], v1.elements[2] / v2.elements[2]};
}

inline bool operator==(const Vec3 &v1, const Vec3 &v2) {
    return v1.elements[0] == v2.elements[0] && v1.elements[1] == v2.elements[1] && v1.elements[2] == v2.elements[2];
}

inline float dot(const Vec3 &v1, const Vec3 &v2) {
    return v1.elements[0] * v2.elements[0] + v1.elements[1] * v2.elements[1] + v1.elements[2] * v2.elements[2];
}

inline Vec3 cross(const Vec3 &v1, const Vec3 &v2) {
    return {v1.elements[1] * v2.elements[2] - v1.elements[2] * v2.elements[1],
            v1.elements[2] * v2.elements[0] - v1.elements[0] * v2.elements[2],
            v1.elements[0] * v2.elements[1] - v1.elements[1] * v2.elements[0]};
}

/*
 * operator: vector and scalar
 */

inline Vec3 operator+(const Vec3 &v, const float k) {
    return {v.elements[0] + k, v.elements[1] + k, v.elements[2] + k};
}

inline Vec3 operator+(const float k, const Vec3 &v) {
    return v + k;
}

inline Vec3 operator-(const Vec3 &v, const float k) {
    return {v.elements[0] - k, v.elements[1] - k, v.elements[2] - k};
}

inline Vec3 operator-(const float k, const Vec3 &v) {
    return -(v - k);
}

inline Vec3 operator*(const Vec3 &v, const float k) {
    return {v.elements[0] * k, v.elements[1] * k, v.elements[2] * k};
}

inline Vec3 operator*(const float k, const Vec3 &v) {
    return v * k;
}

inline Vec3 operator/(const Vec3 &v, const float k) {
    return {v.elements[0] / k, v.elements[1] / k, v.elements[2] / k};
}

inline Vec3 operator/(const float k, const Vec3 &v) {
    return v / k;
}

/*
 * 回転行列
 * 3次元の回転の場合各回転軸に対して回転角を定義することで、回転操作を定義できる
 * 回転軸としてx, y, z軸を選べば回転の自由度は3となる
 * ただし回転の場合は回転の順番により結果が異なることに注意
 * また回転軸としてx, y, z軸以外の任意の軸を指定することもできる
 */

/*
 * rot_x関数
 * x軸回りにthetaラジアン回転させる回転行列
 */

inline Vec3 rotation_x(Vec3 vec, float theta)
{
    float x = vec.x();
    float y = vec.y() * cos(theta) - vec.z() * sin(theta);
    float z = vec.y() * sin(theta) + vec.z() * cos(theta);
    return {x, y, z};
}


/*
 * rot_y関数
 * x軸回りにthetaラジアン回転させる回転行列
 */

inline Vec3 rotation_y(Vec3 vec, float theta)
{
    float x = vec.x() * cos(theta) + vec.z() * sin(theta);
    float y = vec.y();
    float z = -vec.x() * sin(theta) + vec.z() * cos(theta);
    return {x, y, z};
}

/*
 * rot_z関数
 * x軸回りにthetaラジアン回転させる回転行列
 */

inline Vec3 rotation_z(Vec3 vec, float theta)
{
    float x = vec.x() * cos(theta) - vec.y() * sin(theta);
    float y = vec.x() * sin(theta) + vec.y() * sin(theta);
    float z = vec.z();
    return {x, y, z};
}

/*
 * orthonormal_basis関数:
 * 与えられた1つのベクトルから正規直交基底を求める
 *
 * src_vecは(0.0f, 1.0f, 0.0f)と平行の可能性がある
 * そのためtmpは直交するベクトルを2つ用意しておく
 *
 * dot(src_vec, v2)は常に0
 * よってsrc_vecとv2は直交
 *
 * v3はsrc_vecとv2の外積
 * よってsrc_vecとv3は常に直行
 */

inline Vec3 unit_vec(Vec3 v) {
    return v / v.length();
}

inline void orthonormal_basis(const Vec3 &src_vec, Vec3 &base_v2, Vec3 &base_v3) {
    assert(src_vec.length() == 1);
    Vec3 tmp;
    if (std::abs(src_vec.x()) > 0.9f)
        tmp = Vec3(0.0f, 1.0f, 0.0f);
    else
        tmp = Vec3(1.0f, 0.0f, 0.0f);

    base_v2 = unit_vec(tmp - dot(src_vec, tmp) * src_vec);
    base_v3 = cross(src_vec, base_v2);
}

/*
 *  世界系とローカル系の変換
 */

/*
 * world_2_local関数:
 * 与えられた世界系のベクトルを、ローカル系で定義された正規直交基底を用いて書き直す
 * world_vはワールド系の正規直交基底で定義された変換対象の世界系のベクトル
 * s, n, tはワールド系の正規直交基底で定義されたローカル系の正規直交基底
 * 単純に内積を計算して射影している
 */
inline Vec3 world_2_local(const Vec3 &world_v, const Vec3 &s, const Vec3 &n, const Vec3 &t) {
    return {dot(world_v, s), dot(world_v, n), dot(world_v, t)};
}

/*
 * local_2_world関数:
 * 与えられたローカル系のベクトルを、ワールド系で定義された正規直交基底を用いて書き直す
 * vはローカル系の正規直交基底で定義された変換対象のローカル系のベクトル
 * s, n, tはワールド系の正規直交基底で定義されたローカル系の正規直交基底
 * a, b, cはローカル系の正規直交基底を、ワールド系の正規直交基底へ射影したx, y, z成分
 *
 * 手順としてはローカル系の正規直交基底からワールド系の正規直交基底への射影を計算する
 * 内積をかけてワールド系の正規直交基底の射影を算出する
 *
 * vはs,n,tの3成分に分解される
 * 各成分毎にローカル基底のワールド基底への射影を計算し合算すれば良い
 * (v_s * s_x + v_n * n_x + v_t * t_x) : x成分
 *
 */
inline Vec3 local_2_world(Vec3 &v, Vec3 &s, Vec3 &n, Vec3 &t) {
    Vec3 a = Vec3(s.x(), n.x(), t.x());
    Vec3 b = Vec3(s.y(), n.y(), t.y());
    Vec3 c = Vec3(s.z(), n.z(), t.z());

    return {dot(v, a), dot(v, b), dot(v, c)};
}

/*
 *　コンソール出力
 */
inline std::ostream &operator<<(std::ostream &stream, const Vec3 &v) {
    stream << "(" << v.elements[0] << ", " << v.elements[1] << ", " << v.elements[2] << ")";
    return stream;
}

/*
 * エイリアス宣言:
 * typedefと同様に、指定した名前の別名を与える
 * エイリアス宣言はtypedefと同じ意味を持つ
 *
 * ここではPoint3クラスとColorクラスをVec3のエイリアスとして宣言した
 */
using Point3 = Vec3;
using Color = Vec3;

#endif //PRACTICEPATHTRACING_VEC3_H