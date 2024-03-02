#pragma once
#include <string>
#include <memory>
#include <wrl.h>
#include <algorithm>

// �T�E���h�f�[�^
struct SoundData
{
public:
	// �V�[���ʂ̓ǂݍ��ރ^�C�~���O�^�O
	enum class Tag
	{
		Any,		// �펞
		Title,		// �^�C�g��
		Game,		// �Q�[��
		Result,		// ���U���g
	};

	/**
	 * @brief �R���X�g���N�^
	 * @param name �T�E���h�f�[�^�̖��O
	 * @param filePass �t�@�C���p�X
	 * @param tag ���[�h�^�C�~���O�̃^�O
	 * @param volume �����{�����[���i0.0f����1.0f�͈̔́j
	 */
	SoundData(std::string name, std::string filePass, Tag tag, float volume)
		: m_name(name), m_SoundData(nullptr), m_filePass(filePass), m_tag(tag), m_Volume(volume) {}

	/**
	 * @brief ���[�u�R���X�g���N�^
	 * @param other �ړ����I�u�W�F�N�g
	 */
	SoundData(SoundData&& other) noexcept
		: m_name(std::move(other.m_name)), m_SoundData(std::move(other.m_SoundData)), m_filePass(other.m_filePass), m_tag(other.m_tag), m_Volume(other.m_Volume), m_loaded(other.m_loaded) {}

	// ���[�u������Z�q
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
	 * @brief �T�E���h�f�[�^�̎擾
	 * @return �T�E���h�f�[�^�ւ̎Q��
	 */
	std::unique_ptr<BYTE[]>& GetSoundData() { return m_SoundData; }

	/**
	 * @brief �T�E���h�f�[�^�̐ݒ�
	 * @param newSoundData �V�����T�E���h�f�[�^
	 */
	void SetSoundData(std::unique_ptr<BYTE[]> newSoundData) { m_SoundData = std::move(newSoundData); }

	/**
	 * @brief �ʃ{�����[���̎擾
	 * @return ���݂̌ʃ{�����[��
	 */
	float GetVolume() { return m_Volume; }

	/**
	 * @brief �ʃ{�����[���̐ݒ�
	 * @param newVolume �V�����{�����[���i0.0f����1.0f�͈̔́j
	 */
	void SetVolume(float newVolume) { m_Volume = std::clamp(newVolume, 0.0f, 1.0f); }

	/**
	 * @brief �t�@�C���p�X�̎擾
	 * @return ���݂̃t�@�C���p�X
	 */
	std::string GetFilePass() { return m_filePass; }

	/**
	 * @brief �t�@�C���p�X�̍Đݒ�
	 * @param newPass �V�����t�@�C���p�X
	 */
	void SetFilePass(std::string newPass)
	{
		m_filePass = newPass;

		// ���[�h�ςݔ�������Z�b�g
		m_loaded = false;
	}

	/**
	 * @brief ���[�h�ςݔ���̎擾
	 * @return ���[�h�ς݂Ȃ�true�A�����łȂ����false
	 */
	bool IsLoaded() { return m_loaded; }

	/**
	 * @brief ���[�h�ςݔ���̐ݒ�
	 * @param loaded ���[�h�ς݂Ȃ�true�A�����łȂ����false
	 */
	void SetLoaded(bool loaded) { m_loaded = loaded; }

	// WAVE�t�H�[�}�b�g
	WAVEFORMATEX m_wfx = { 0 };

	// ���O
	std::string m_name = "";

	// �^�O
	Tag m_tag = Tag::Any;

	// �Ȃ̒���
	int	m_Length = 0;

	// �Đ��������ӏ��̒���
	int m_PlayLength = 0;

private:

	// �T�E���h�̕ϊ��f�[�^
	std::unique_ptr<BYTE[]> m_SoundData = nullptr;

	// �t�@�C���p�X
	std::string m_filePass = "";

	// �ʃ{�����[��
	float m_Volume = 1.0f;

	// ���[�h�ςݔ���
	bool m_loaded = false;
};

