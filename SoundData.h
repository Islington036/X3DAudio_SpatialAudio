#pragma once
#include <string>
#include <memory>
#include <wrl.h>
#include <algorithm>

// サウンドデータ
struct SoundData
{
public:
	// シーン別の読み込むタイミングタグ
	enum class Tag
	{
		Any,		// 常時
		Title,		// タイトル
		Game,		// ゲーム
		Result,		// リザルト
	};

	/**
	 * @brief コンストラクタ
	 * @param name サウンドデータの名前
	 * @param filePass ファイルパス
	 * @param tag ロードタイミングのタグ
	 * @param volume 初期ボリューム（0.0fから1.0fの範囲）
	 */
	SoundData(std::string name, std::string filePass, Tag tag, float volume)
		: m_name(name), m_SoundData(nullptr), m_filePass(filePass), m_tag(tag), m_Volume(volume) {}

	/**
	 * @brief ムーブコンストラクタ
	 * @param other 移動元オブジェクト
	 */
	SoundData(SoundData&& other) noexcept
		: m_name(std::move(other.m_name)), m_SoundData(std::move(other.m_SoundData)), m_filePass(other.m_filePass), m_tag(other.m_tag), m_Volume(other.m_Volume), m_loaded(other.m_loaded) {}

	// ムーブ代入演算子
	SoundData& operator=(SoundData&& other) noexcept
	{
		m_name = other.m_name;
		m_SoundData = std::move(other.m_SoundData);
		m_filePass = other.m_filePass;
		m_tag = other.m_tag;
		m_Volume = other.m_Volume;
		m_loaded = other.m_loaded;

		return *this;
	}

	/**
	 * @brief サウンドデータの取得
	 * @return サウンドデータへの参照
	 */
	std::unique_ptr<BYTE[]>& GetSoundData() { return m_SoundData; }

	/**
	 * @brief サウンドデータの設定
	 * @param newSoundData 新しいサウンドデータ
	 */
	void SetSoundData(std::unique_ptr<BYTE[]> newSoundData) { m_SoundData = std::move(newSoundData); }

	/**
	 * @brief 個別ボリュームの取得
	 * @return 現在の個別ボリューム
	 */
	float GetVolume() { return m_Volume; }

	/**
	 * @brief 個別ボリュームの設定
	 * @param newVolume 新しいボリューム（0.0fから1.0fの範囲）
	 */
	void SetVolume(float newVolume) { m_Volume = std::clamp(newVolume, 0.0f, 1.0f); }

	/**
	 * @brief ファイルパスの取得
	 * @return 現在のファイルパス
	 */
	std::string GetFilePass() { return m_filePass; }

	/**
	 * @brief ファイルパスの再設定
	 * @param newPass 新しいファイルパス
	 */
	void SetFilePass(std::string newPass)
	{
		m_filePass = newPass;

		// ロード済み判定をリセット
		m_loaded = false;
	}

	/**
	 * @brief ロード済み判定の取得
	 * @return ロード済みならtrue、そうでなければfalse
	 */
	bool IsLoaded() { return m_loaded; }

	/**
	 * @brief ロード済み判定の設定
	 * @param loaded ロード済みならtrue、そうでなければfalse
	 */
	void SetLoaded(bool loaded) { m_loaded = loaded; }

	// WAVEフォーマット
	WAVEFORMATEX m_wfx = { 0 };

	// 名前
	std::string m_name = "";

	// タグ
	Tag m_tag = Tag::Any;

	// 曲の長さ
	int	m_Length = 0;

	// 再生したい箇所の長さ
	int m_PlayLength = 0;

private:

	// サウンドの変換データ
	std::unique_ptr<BYTE[]> m_SoundData = nullptr;

	// ファイルパス
	std::string m_filePass = "";

	// 個別ボリューム
	float m_Volume = 1.0f;

	// ロード済み判定
	bool m_loaded = false;
};

