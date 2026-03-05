# Cpp Game

## 動作確認環境

- Windows 11

以下をインストール済み

- msys2
  - mingw-w64のC++コンパイラ，make
- CMake
- Git

## Usage

1. cmake, git, C++コンパイラ以下をinstallする
    1. CMake
    1. Git
    1. C++コンパイラ
    1. buildシステム (makeなど)

1. それぞれのアーキテクチャに対応している`installscript/install`スクリプトを実行する

### windows-gcc

**powershellのversion**

5.1 or 7.x

**powershellでスクリプトを実行するための準備**

管理者権限でPowerShellを起動し，以下のコマンドを実行する．
ポリシーを変更する．

```PowerShell
Set-ExecutionPolicy -Scope Process -ExecutionPolicy Bypass
```

or

```PowerShell
powershell -ExecutionPolicy Bypass -File .\installscript\install.ps1
```
