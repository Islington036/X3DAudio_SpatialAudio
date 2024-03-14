# X3DAudio_SpatialAudio
XAudio2とX3DAudioを使用して、**3Dオーディオを実現したクラス**です。

各機能を使いやすくした、すぐに使えるオーディオクラスとして利用できます。

## 機能
- XAudio2を用いた、単純なサウンド再生
- X3DAudioを用いた、**3Dサウンドの再生**
- 音源データのロード管理、情報の管理
- 3Dサウンド設定の簡略化

## メモ
開発環境ではDirectX11を使用してテスト・開発を行いました。  
DirectX12環境でも動作するかと思われますが、動作テストは行なっていません。

数学系ライブラリはD3DXMathを用いていますが、必要に応じて入れ替えてもらっても構いません。  
その場合は、適宜ソースコード内の使用箇所も入れ替えてください。

今回はシングルトン設計を使用しています。開発環境に適さない場合は、適当に改造してください。  

## サンプルコード
```cpp
// 初期化処理
SoundManager::Instance().Init();

// 楽曲名、保存先のパス、タグ、音源ごとの個別ボリュームを指定
SoundManager::Instance().AddSoundData("Walk", "asset\\music\\walk.wav", SoundData::Tag::Title, 1.0f);

// タグが指定された音源をロード
SoundManager::Instance().Load(SoundData::Tag::Game);

// リスナー（プレイヤー）の位置、前方ベクトル、上方向ベクトルをセット
SoundManager::Instance().SetListenerPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f));

// 楽曲名、ループ再生、音源の位置、音が聞こえる範囲を指定して、SEを再生
int num = SoundManager::Instance().PlaySE3D("Walk", true, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 10000.0f);

// 終了処理
SoundManager::Instance().Uninit();
```

## 使い方
1. 使用したい**wavファイル**の音源を保存します。
2. `SoundData.h`にある`Tag`で、音源を読み込みたいタイミングのタグを追加します。
3. `SoundManager`クラスの初期化処理を行います。
4. `AddSoundData`メソッドで音源を`SoundManager`に追加します。
5. `Load`メソッドで音源のロード処理を行います。
6. `SetListenerPosition`メソッドでリスナー位置を指定します（3Dサウンドを使用しない場合は必要ありません）。
7. `PlaySE3D`メソッドで音源（エミッター）の位置を指定し、再生します。
8. `SoundManager`クラスの終了処理を行います。

## 使い方メモ
### 3Dサウンド
3Dサウンドではリスナーとエミッターの2種類の位置情報が存在し、  
それぞれ**音源を聞く位置**と**音源を発する位置の役割**となります。  

エミッター情報は音源の再生中でも更新することが可能で、  
例えば「移動中の歩行音」などは発信源の位置情報をその都度更新する必要があります。  
`EmitterReset`メソッドで音源ごとの位置情報を再指定できるので、音源を移動させたい場合はそちらの関数を使ってください。  
  
### 音声ソース
音源はXAudio2の機能である音声ソース（`IXAudio2SourceVoice`）で管理されており、  
`SoundManager`クラスでは音源を同時再生できるよう、**音声ソースを複数所有**しています。  
どの音声ソースが使用されているかは、Play~~から始まる再生メソッドの戻り値で取得できます。  

先に挙げた`EmitterReset`メソッドなど、再生中の音源を途中で変更する際は   
使用している音声ソースの番号が必要になるので、**戻り値を記録しておくことをおすすめ**します。

### シングルトンパターン
このクラスでは

## リファレンス
### 管理系メソッド
| メソッド  |  役割   |
| -------- | ----------  |
| [`Init`](#Init) | XAudio2とX3DAudioの初期化。 |
| [`Uninit`](#Uninit) | XAudio2とX3DAudioの終了処理。 |
| [`AddSoundData`](#AddSoundData) | 音源データをSoundManagerに追加。 |
| [`DeleteMusicData`](#DeleteMusicData) | 読み込んだ音源データを削除。 |
| [`Load`](#Load) | シーンタグが指定された音源データを読み込み。 |

### 再生系メソッド
| メソッド  |  役割   |
| -------- | ----------  |
| [`PlayBGM`](#PlayBGM) | BGMを再生。 |
| [`PlaySE`](#PlaySE) | SEを再生。 |
| [`PlaySE3D`](#PlaySE3D) | 3DサウンドのSEを再生。 |
| [`StopAllBGM`](#StopAllBGM) | 全てのBGMを停止。 |
| [`StopAllSE`](#StopAllSE) | 全てのSEを停止。 |
| [`StopBGM`](#StopBGM) | リソース番号を指定してBGMを停止。 |
| [`StopSE`](#StopSE) | リソース番号を指定してSEを停止。 |
| [`SetListenerPosition`](#SetListenerPosition) | リスナーのポジションを設定。 |
| [`EmitterReset`](#EmitterReset) | 3Dサウンドの位置情報を修正。 |
| [`GetEmitter`](#GetEmitter) | 汎用エミッターの情報を取得。（細かい設定に使ってください） |
| [`SetMasterVolume`](#SetMasterVolume) | 全体ボリュームを設定。 |
| [`CalculateAngleBetweenVectors`](#CalculateAngleBetweenVectors) | 2点間の角度計算。（PlaySE3DのmaxVolumeAngleRange引数にどうぞ） |

### 音源データ
| パラメータ  |  役割   |
| -------- | ----------  |
| [`m_name`](#m_name) | ローディングの際に指定するニックネーム。 |
| [`m_tag`](#m_tag) | 読み込むシーンを決定するタグ。 |
| [`m_Volume`](#m_Volume) | 音源ごとの個別ボリューム。 |
