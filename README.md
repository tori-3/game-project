# アンナのお菓子な大冒険
## Linuxでのビルド方法

`~/work/` で作業するものとする。

### Siv3Dのインストール

[https://siv3d.github.io/ja-jp/download/ubuntu/](https://siv3d.github.io/ja-jp/download/ubuntu/)に従って以下のようにインストールする。

```bash
sudo apt update
sudo apt install -y ninja-build libasound2-dev libavcodec-dev libavformat-dev libavutil-dev libboost-dev libcurl4-openssl-dev libgtk-3-dev libgif-dev libglu1-mesa-dev libharfbuzz-dev libmpg123-dev libopencv-dev libopus-dev libopusfile-dev libsoundtouch-dev libswresample-dev libtiff-dev libturbojpeg0-dev libvorbis-dev libwebp-dev libxft-dev uuid-dev xorg-dev
cd ~/work
git clone git@github.com:Siv3D/OpenSiv3D.git
cd OpenSiv3D/Linux
# ビルドのセットアップ
mkdir build && cd build
cmake -GNinja -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
cd ..
# ビルド&インスコ
cmake --build build
sudo cmake --install build
```

### リポジトリのセットアップ&リソースファイルのコピー

```bash
cd ~/work
git clone git@github.com:ponzu840w/game-project.git
cd game-project
git checkout linux_build
cp -r ~/work/OpenSiv3D/Linux/App/resources ~/work/game-project/アンナのお菓子な大冒険/App/
cp -r ~/work/OpenSiv3D/Linux/App/resources ~/work/game-project/ステージエディタ改/App/
```

### ゲーム本体をビルド

```bash
cd ~/work/game-project/アンナのお菓子な大冒険
# ビルドのセットアップ
mkdir build && cd build
cmake -GNinja -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
cd ..
# ビルド
cmake –build build
```

### ゲーム本体を起動

```bash
cd ~/work/game-project/アンナのお菓子な大冒険/App
./Annas_Candy_Adventrue
```

### ステージエディタをビルド
```bash
cd ~/work/game-project/ステージエディタ改
# ビルドのセットアップ
mkdir build && cd build
cmake -GNinja -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
cd ..
# ビルド
cmake –build build
```

### ステージエディタを起動

```bash
cd ~/work/game-project/ステージエディタ改/App
./Stage_Editor_Kai
```
