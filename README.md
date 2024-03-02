# X3DAudio_SpatialAudio
XAudio2とX3DAudioを使用して、**3Dオーディオを実現したクラス**です。

各機能を使いやすくした、すぐに使えるオーディオクラスとして利用できます。

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
