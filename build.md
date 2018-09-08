# ビルドシステムについて

## 必須ソフト類

- Visual Studio 2017 Professional またはそれ以上
- CMake 3.12.2
  - `cmake.exe` へのパスが通っていること
- Python 2.7.15 (x86)
  - `.py` の関連付けができていること
- 7-Zip 18.05 (x64)
  - `7z.exe` へのパスが通っていること

## ビルド手順

スタートから `Developer Command Prompt for VS 2017` を起動

実行:

```bat
prepareBuildFiles.py
```

出力:

```txt
Writing PG94_x86_v141_xp/build.cmd
Writing PG94_x64_v141_xp/build.cmd
Writing PG95_x86_v141_xp/build.cmd
Writing PG95_x64_v141_xp/build.cmd
Writing PG96_x86_v141_xp/build.cmd
Writing PG96_x64_v141_xp/build.cmd
Writing PG100_x86_v141_xp/build.cmd
Writing PG100_x64_v141_xp/build.cmd
Writing buildAll.cmd
Writing packer.json
```

実行:

```bat
buildAll.cmd
```

出力は省略

実行:

```bat
packer.py
```

出力は省略

## ビルド成果物

`pack` 以下にビルド成果をデプロイします。

圧縮したものを `packs/20180908.7z` のようなファイル名で作成します。
