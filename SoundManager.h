#pragma once
#include <xaudio2.h>
#include <X3DAudio.h>
#include <unordered_map>

#pragma warning(push)
#pragma warning(disable:4005)

// 数学系ライブラリ
#include <D3DX10math.h>

#pragma warning(pop)

#include "SingletonObject.h"
#include "SoundData.h"

#pragma comment(lib, "x3daudio.lib")
#pragma comment(lib, "xaudio2.lib")

using namespace Microsoft;

// BGMの同時再生可能数
static const int BGM_Num = 3;

// SEの同時再生可能数
static const int SE_Num = 3;

// サウンドマネージャー
class SoundManager : public SingletonObject<SoundManager>
{
public:
	// フレンドクラスの指定
	friend class SingletonObject<SoundManager>;

	/**
	 * @brief サウンドマネージャーの初期化
	 * @return 初期化の成否を示すHRESULT
	 */
	HRESULT Init();

	/**
	 * @brief サウンドマネージャーの終了処理
	 */
	void Uninit();

	/**
	 * @brief 音源データの追加
	 * @param name 音源の名前
	 * @param filePass 音源データへのファイルパス
	 * @param tag この音源のカテゴリータグ
	 * @param volume 再生ボリューム（デフォルトは1.0f）
	 */
	void AddSoundData(std::string name, std::string filePass, SoundData::Tag tag, float volume = 1.0f);

	/**
	 * @brief 音源データの追加
	 * @param data 追加するSoundDataオブジェクトへの参照
	 */
	void AddSoundData(SoundData& data);

	/**
	* @brief リスナーの位置を設定
	* @param pos リスナーの位置
	* @param front リスナーの前方向ベクトル
	* @param top リスナーの上方向ベクトル
	* @param velocity リスナーの移動速度
	*/
	void SetListenerPosition(D3DXVECTOR3 pos,
							 D3DXVECTOR3 front = D3DXVECTOR3(0.0f, 0.0f, 1.0f),
							 D3DXVECTOR3 top = D3DXVECTOR3(0.0f, 1.0f, 0.0f),
							 D3DXVECTOR3 velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	/**
	 * @brief 音源の名前を指定してBGM音楽データを再生
	 * @param name 再生するBGMの名前
	 * @param loop BGMをループするかどうか（デフォルトはtrue）
	 * @return int 使用したBGMのリソース番号
	 */
	int PlayBGM(std::string name, bool loop = true);

	/**
	 * @brief 音源の名前を指定してSE音楽データを再生
	 * @param name 再生するSEの名前
	 * @return int 使用したSEのリソース番号
	 */
	int PlaySE(std::string name, bool loop = false);

	/**
	* @brief 3Dサウンドの再生
	* @param name 再生するSEの名前
	* @param pos 再生する位置
	* @param soundMaxVolumeRadius 音の最大ボリュームが適用される半径
	* @param maxVolumeAngleRange 最大ボリュームが適用される角度範囲
	* @param front エミッターの前方向ベクトル
	* @param top エミッターの上方向ベクトル
	* @param velocity エミッターの移動速度
	* @return int 使用したSEのリソース番号
	*/
	int PlaySE3D(std::string name,
				 bool loop,
				 D3DXVECTOR3 pos,
				 float soundMaxVolumeRadius,
				 float maxVolumeAngleRange = 360.0f,
				 D3DXVECTOR3 front = D3DXVECTOR3(0.0f, 0.0f, 1.0f),
				 D3DXVECTOR3 top = D3DXVECTOR3(0.0f, 1.0f, 0.0f),
				 D3DXVECTOR3 velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	/**
	* @brief 再生中のSEの位置を変更
	* @param sourceVoiceNum 再生位置を変更するSEのリソース番号
	* @param pos 再生する位置
	* @param soundMaxVolumeRadius 音の最大ボリュームが適用される半径
	* @param maxVolumeAngleRange 最大ボリュームが適用される角度範囲
	* @param front エミッターの前方向ベクトル
	* @param top エミッターの上方向ベクトル
	* @param velocity エミッターの移動速度
	*/
	void EmitterReset(int sourceVoiceNum,
					  D3DXVECTOR3 pos,
					  float soundMaxVolumeRadius,
					  float maxVolumeAngleRange = 360.0f,
					  D3DXVECTOR3 front = D3DXVECTOR3(0.0f, 0.0f, 1.0f),
					  D3DXVECTOR3 top = D3DXVECTOR3(0.0f, 1.0f, 0.0f),
					  D3DXVECTOR3 velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	/**
	 * @brief 全てのBGMを停止
	 */
	void StopAllBGM();

	/**
	* @brief 全てのSEを停止
	*/
	void StopAllSE();

	/**
	* @brief リソース番号指定でBGM音楽データを停止
	* @param resourceNum 停止するBGMのリソース番号
	*/
	void StopBGM(int resourceNum);

	/**
	* @brief リソース番号指定でSE音楽データを停止
	* @param resourceNum 停止するSEのリソース番号
	*/
	void StopSE(int resourceNum);

	/**
	* @brief 特定のシーン用の音楽データをロード
	* @param loadScene ロードする音源のシーンタグ
	*/
	void Load(SoundData::Tag loadScene);

	/**
	* @brief 登録している音楽データを削除
	* @param name 削除する音楽データの名前
	*/
	void deleteMusicData(std::string name);

	/**
	 * @brief デフォルトで設定されているエミッターの取得
	 * @return エミッターオブジェクトへのポインタ
	 */
	X3DAUDIO_EMITTER* GetEmitter() { return &m_defaultEmitter; }

	/**
	 * @brief マスターボリュームの設定
	 * @param volume 新しいマスターボリューム（0.0fから1.0fの範囲）
	 */
	void SetMasterVolume(float volume){ MasterVolume = std::clamp(volume, 0.0f, 1.0f); }

	/**
	 * @brief ベクトル間の角度計算（PlaySE3DのmaxVolumeAngleRange引数に使えます）
	 * @param from 始点ベクトル
	 * @param to 終点ベクトル
	 * @return 二つのベクトル間の角度（度数法）
	 */
	float CalculateAngleBetweenVectors(const D3DXVECTOR3& from, const D3DXVECTOR3& to);

private:
	SoundManager() {}
	~SoundManager() {}

	// シングルトンクラスのコピー禁止
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;

	// XAudio本体
	WRL::ComPtr<IXAudio2> m_Xaudio = nullptr;

	// マスタリングボイス
	IXAudio2MasteringVoice* m_MasteringVoice = nullptr;

	// BGM再生リソース
	IXAudio2SourceVoice* BGM_SourceVoice[BGM_Num] = { nullptr };

	// SE再生リソース
	IXAudio2SourceVoice* SE_SourceVoice[SE_Num] = { nullptr };

	// 音源データ
	std::vector<std::unique_ptr<SoundData>> Music_Data = {};

	// 実行環境のスピーカーの情報
	// デフォルトでは5.1チャンネルサラウンド設定
	UINT32 m_speakerChannelMask = SPEAKER_5POINT1;

	// スピーカーチャンネル数（6は5.1チャンネル設定）
	UINT32 m_speakerChannelNum = 6;

	// 音源のチャンネル数（2はステレオ設定）
	UINT32 m_sourceChannelNum = 2;

	// 3Dサウンドのハンドル
	X3DAUDIO_HANDLE m_x3dAudioHandle;

	// リスナー情報
	X3DAUDIO_LISTENER m_listener;

	// エミッター情報
	X3DAUDIO_EMITTER m_defaultEmitter;

	// エミッターの音源情報
	X3DAUDIO_DSP_SETTINGS m_dspSettings;

	// マスターボリューム
	float MasterVolume = 0.5f;

	// 音源の再生処理


	// 3Dサウンドの初期設定
	void Init3DSound(X3DAUDIO_EMITTER emitter, D3DXVECTOR3 pos, float soundMaxVolumeRadius, 
					 float maxVolumeAngleRange, D3DXVECTOR3 front, D3DXVECTOR3 top,
					 IXAudio2SourceVoice* SourceVoice);

	// 音源データの検索
	bool SearchSoundData(std::string name, int* soundDataNum);

	// 音源データからデータを読み込んで初期設定
	XAUDIO2_BUFFER SetBuffer(int soundDataNum, bool loop);
};