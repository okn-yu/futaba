#----------
# 基本事項
#----------

# CMakeではソースコードのディレクトリとは別にビルド専用のディレクトリが作成される
# ビルド結果は全てビルド専用のディレクトリに出力される
# これによりソースコードのファイル群とビルド結果のファイル群が完全に分離されるのでメンテナンス性が向上される

# ビルドとコンパイルの違い:
# ビルドは実行ファイルの作成までを含む
# コンパイルはプログラムを機械語まで変換するプロセスを指す
# そのためコンパイルはビルドのプロセスの一部

#-------------
# CMakeの用語
#-------------

# target:
# ビルドには「複数の実行ファイルを作成する」「外部から呼び出されるためのライブラリファイルを作成する」「テストコードを実行する」などの複数の種類が存在する
# 上記のようにビルドを実行するゴールを指定したものがターゲット

# 代表的なターゲットは以下の3つ
#  実行ファイルの生成(add_executable)
#  ライブラリファイルの生成(add_library)
#  カスタムターゲットの実行(add_custom_target): 実行ファイルやライブラリファイルを生成しない


#----------------------
# CMakeの非推奨コマンド群
#----------------------

# include_directories
# add_definitions, add_compile_definitions, add_compile_options
# link_directories
# link_libraries

#----------------
# CMakeの参考URL
#----------------

# https://kamino.hatenablog.com/entry/cmake_tutorial1
# https://qiita.com/shohirose/items/45fb49c6b429e8b204ac
# https://qiita.com/shohirose/items/5b406f060cd5557814e9

#---------------------
# CXXのコンパイルの忘備録
#---------------------

# CMakeを記載するためにはC言語のコンパイルの手順を把握しておく必要がある

# オブジェクトファイル(XXX.o):
# コンパイラがプログラムを処理した結果生成される中間的なファイル
# オブジェクトファイル群をリンクすることで最終的に実行可能な実行ファイルが作成される

# まず「実行ファイルの作成」と「オブジェクトファイルの作成」と「静的ライブラリおよび共有ライブラリの作成」はすべてgccのコマンドは異なる
# g++では「オブジェクトリファイル（XXX.o）を作成するコマンド」と「ライブラリ（libXXX.soおよびlibXXX.a）を作成するコマンド」は異なる
# オブジェクトファイルとライブラリをリンクすることで実行ファイルが作成される

# 「プログラムファイル(XXX.cおよびXXX.cpp)」はコンパイルするとオブジェクトファイル(XXX.o)がそれぞれ独立して作成される
# そのため各プログラムファイル内で関数のIFはヘッダファイルで仮宣言して、プログラムファイルのスコープ内で独立して解決可能な状態にしておく必要がある

# 原則論としてはヘッダファイル（XXX.h）にはインタフェースのみが定義されていて、実装は別のプログラムファイル(XXX.cpp)に記載するのが望ましい
# 一般的には「呼び出されるincludeディレクトリ（XXX.h）」と「呼び出し元のプログラムファイル（XXX.cpp）」とは別のフォルダで管理されている
# そのためg++ではオブジェクトファイルの作成時にincludeディレクトリのパスを指定する必要がある
# そもそも論として「ヘッダファイルのXXX.h」はコンパイルしても、ヘッダファイルのオブジェクトファイル(XXX.o)は作成されない
# そのため.hのみからなるディレクトリ内にCMakeLists.txtは存在しない

# 一般的には「呼び出されるライブラリ(XXX.so or XXX.a)」は「呼び出し元のプログラムファイル(XXX.cpp)」とは別のフォルダで管理されている
# そのため実行ファイル作成時にはライブラリとリンクする必要があるため、ライブラリのパスを指定する必要がある

# -----------------
# CMake
# -----------------

# CMakeのオプションの設定
option(CMAKE_DEBUG_MSG "Enable CMake Debug Message" on)
option(FTB_PYTHON_ENABLE "Enable Python Intafece" on)

message("[OPTION] CMAKE_DEBUG_OPT: ${CMAKE_DEBUG_OPT} ")
message("[OPTION] FTB_PYTHON_ENABLE: ${FTB_PYTHON_ENABLE} ")

# message:デバッグ用途のためコンソールに出力する
if (CMAKE_DEBUG_MSG)
    message("Start /CMake")
endif ()

if (FTB_PYTHON_ENABLE)
    message("Check Python enviroment")
    # Pybind11の利用時には直接add_subdirectoryの指定が必要
    add_subdirectory(ext/pybind11)
endif()

# ${CMAKE_SOURCE_DIR}:CMakeLists.txtのルートのパスを表す
# message("DEBUG: CMAKE_SOURCE_DIR is ${CMAKE_SOURCE_DIR}")

# 対応するcmakeの最小バージョンを指定する
# 指定を満たさない場合はエラーで終了する
# これはルートのCMakeListsにのみ記載する
cmake_minimum_required(VERSION 3.17)

# プロジェクト名とプログラム言語を指定
# これはルートのCMakeListsにのみ記載する
PROJECT(PracticePathTracing CXX)

# 非推奨コマンド
# 定義したライン以降の全てのターゲットが指定したディレクトリをインクルードしてしまう
# んがっ、Mitsubaでは利用されている...!
# include_directories(${CMAKE_SOURCE_DIR}/include)
#include_directories(${CMAKE_SOURCE_DIR}/etc/pybind11/include)


# C++の標準仕様の指定
# Vec3.hのarrayなどは初期化の仕方がC++11以降にしか対応していない
set(CMAKE_CXX_STANDARD 11)

# サブフォルダの登録
add_subdirectory(src)