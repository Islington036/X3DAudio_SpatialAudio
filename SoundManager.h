#pragma once
#include <xaudio2.h>
#include <X3DAudio.h>
#include <unordered_map>

#pragma warning(push)
#pragma warning(disable:4005)

// ���w�n���C�u����
#include <D3DX10math.h>

#pragma warning(pop)

#include "SingletonObject.h"
#include "SoundData.h"

#pragma comment(lib, "x3daudio.lib")
#pragma comment(lib, "xaudio2.lib")

using namespace Microsoft;

// BGM�̓����Đ��\��
static const int BGM_Num = 3;

// SE�̓����Đ��\��
static const int SE_Num = 3;

// �T�E���h�}�l�[�W���[
class SoundManager : public SingletonObject<SoundManager>
{
public:
	// �t�����h�N���X�̎w��
	friend class SingletonObject<SoundManager>;

	/**
	 * @brief �T�E���h�}�l�[�W���[�̏�����
	 * @return �������̐��ۂ�����HRESULT
	 */
	HRESULT Init();

	/**
	 * @brief �T�E���h�}�l�[�W���[�̏I������
	 */
	void Uninit();

	/**
	 * @brief �����f�[�^�̒ǉ�
	 * @param name �����̖��O
	 * @param filePass �����f�[�^�ւ̃t�@�C���p�X
	 * @param tag ���̉����̃J�e�S���[�^�O
	 * @param volume �Đ��{�����[���i�f�t�H���g��1.0f�j
	 */
	void AddSoundData(std::string name, std::string filePass, SoundData::Tag tag, float volume = 1.0f);

	/**
	 * @brief �����f�[�^�̒ǉ�
	 * @param data �ǉ�����SoundData�I�u�W�F�N�g�ւ̎Q��
	 */
	void AddSoundData(SoundData& data);

	/**
	* @brief ���X�i�[�̈ʒu��ݒ�
	* @param pos ���X�i�[�̈ʒu
	* @param front ���X�i�[�̑O�����x�N�g��
	* @param top ���X�i�[�̏�����x�N�g��
	* @param velocity ���X�i�[�̈ړ����x
	*/
	void SetListenerPosition(D3DXVECTOR3 pos,
							 D3DXVECTOR3 front = D3DXVECTOR3(0.0f, 0.0f, 1.0f),
							 D3DXVECTOR3 top = D3DXVECTOR3(0.0f, 1.0f, 0.0f),
							 D3DXVECTOR3 velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	/**
	 * @brief �����̖��O���w�肵��BGM���y�f�[�^���Đ�
	 * @param name �Đ�����BGM�̖��O
	 * @param loop BGM�����[�v���邩�ǂ����i�f�t�H���g��true�j
	 * @return int �g�p����BGM�̃��\�[�X�ԍ�
	 */
	int PlayBGM(std::string name, bool loop = true);

	/**
	 * @brief �����̖��O���w�肵��SE���y�f�[�^���Đ�
	 * @param name �Đ�����SE�̖��O
	 * @return int �g�p����SE�̃��\�[�X�ԍ�
	 */
	int PlaySE(std::string name, bool loop = false);

	/**
	* @brief 3D�T�E���h�̍Đ�
	* @param name �Đ�����SE�̖��O
	* @param pos �Đ�����ʒu
	* @param soundMaxVolumeRadius ���̍ő�{�����[�����K�p����锼�a
	* @param maxVolumeAngleRange �ő�{�����[�����K�p�����p�x�͈�
	* @param front �G�~�b�^�[�̑O�����x�N�g��
	* @param top �G�~�b�^�[�̏�����x�N�g��
	* @param velocity �G�~�b�^�[�̈ړ����x
	* @return int �g�p����SE�̃��\�[�X�ԍ�
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
	* @brief �Đ�����SE�̈ʒu��ύX
	* @param sourceVoiceNum �Đ��ʒu��ύX����SE�̃��\�[�X�ԍ�
	* @param pos �Đ�����ʒu
	* @param soundMaxVolumeRadius ���̍ő�{�����[�����K�p����锼�a
	* @param maxVolumeAngleRange �ő�{�����[�����K�p�����p�x�͈�
	* @param front �G�~�b�^�[�̑O�����x�N�g��
	* @param top �G�~�b�^�[�̏�����x�N�g��
	* @param velocity �G�~�b�^�[�̈ړ����x
	*/
	void EmitterReset(int sourceVoiceNum,
					  D3DXVECTOR3 pos,
					  float soundMaxVolumeRadius,
					  float maxVolumeAngleRange = 360.0f,
					  D3DXVECTOR3 front = D3DXVECTOR3(0.0f, 0.0f, 1.0f),
					  D3DXVECTOR3 top = D3DXVECTOR3(0.0f, 1.0f, 0.0f),
					  D3DXVECTOR3 velocity = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	/**
	 * @brief �S�Ă�BGM���~
	 */
	void StopAllBGM();

	/**
	* @brief �S�Ă�SE���~
	*/
	void StopAllSE();

	/**
	* @brief ���\�[�X�ԍ��w���BGM���y�f�[�^���~
	* @param resourceNum ��~����BGM�̃��\�[�X�ԍ�
	*/
	void StopBGM(int resourceNum);

	/**
	* @brief ���\�[�X�ԍ��w���SE���y�f�[�^���~
	* @param resourceNum ��~����SE�̃��\�[�X�ԍ�
	*/
	void StopSE(int resourceNum);

	/**
	* @brief ����̃V�[���p�̉��y�f�[�^�����[�h
	* @param loadScene ���[�h���鉹���̃V�[���^�O
	*/
	void Load(SoundData::Tag loadScene);

	/**
	* @brief �o�^���Ă��鉹�y�f�[�^���폜
	* @param name �폜���鉹�y�f�[�^�̖��O
	*/
	void deleteMusicData(std::string name);

	/**
	 * @brief �f�t�H���g�Őݒ肳��Ă���G�~�b�^�[�̎擾
	 * @return �G�~�b�^�[�I�u�W�F�N�g�ւ̃|�C���^
	 */
	X3DAUDIO_EMITTER* GetEmitter() { return &m_defaultEmitter; }

	/**
	 * @brief �}�X�^�[�{�����[���̐ݒ�
	 * @param volume �V�����}�X�^�[�{�����[���i0.0f����1.0f�͈̔́j
	 */
	void SetMasterVolume(float volume){ MasterVolume = std::clamp(volume, 0.0f, 1.0f); }

	/**
	 * @brief �x�N�g���Ԃ̊p�x�v�Z�iPlaySE3D��maxVolumeAngleRange�����Ɏg���܂��j
	 * @param from �n�_�x�N�g��
	 * @param to �I�_�x�N�g��
	 * @return ��̃x�N�g���Ԃ̊p�x�i�x���@�j
	 */
	float CalculateAngleBetweenVectors(const D3DXVECTOR3& from, const D3DXVECTOR3& to);

private:
	SoundManager() {}
	~SoundManager() {}

	// �V���O���g���N���X�̃R�s�[�֎~
	SoundManager(const SoundManager&) = delete;
	SoundManager& operator=(const SoundManager&) = delete;

	// XAudio�{��
	WRL::ComPtr<IXAudio2> m_Xaudio = nullptr;

	// �}�X�^�����O�{�C�X
	IXAudio2MasteringVoice* m_MasteringVoice = nullptr;

	// BGM�Đ����\�[�X
	IXAudio2SourceVoice* BGM_SourceVoice[BGM_Num] = { nullptr };

	// SE�Đ����\�[�X
	IXAudio2SourceVoice* SE_SourceVoice[SE_Num] = { nullptr };

	// �����f�[�^
	std::vector<std::unique_ptr<SoundData>> Music_Data = {};

	// ���s���̃X�s�[�J�[�̏��
	// �f�t�H���g�ł�5.1�`�����l���T���E���h�ݒ�
	UINT32 m_speakerChannelMask = SPEAKER_5POINT1;

	// �X�s�[�J�[�`�����l�����i6��5.1�`�����l���ݒ�j
	UINT32 m_speakerChannelNum = 6;

	// �����̃`�����l�����i2�̓X�e���I�ݒ�j
	UINT32 m_sourceChannelNum = 2;

	// 3D�T�E���h�̃n���h��
	X3DAUDIO_HANDLE m_x3dAudioHandle;

	// ���X�i�[���
	X3DAUDIO_LISTENER m_listener;

	// �G�~�b�^�[���
	X3DAUDIO_EMITTER m_defaultEmitter;

	// �G�~�b�^�[�̉������
	X3DAUDIO_DSP_SETTINGS m_dspSettings;

	// �}�X�^�[�{�����[��
	float MasterVolume = 0.5f;

	// �����̍Đ�����


	// 3D�T�E���h�̏����ݒ�
	void Init3DSound(X3DAUDIO_EMITTER emitter, D3DXVECTOR3 pos, float soundMaxVolumeRadius, 
					 float maxVolumeAngleRange, D3DXVECTOR3 front, D3DXVECTOR3 top,
					 IXAudio2SourceVoice* SourceVoice);

	// �����f�[�^�̌���
	bool SearchSoundData(std::string name, int* soundDataNum);

	// �����f�[�^����f�[�^��ǂݍ���ŏ����ݒ�
	XAUDIO2_BUFFER SetBuffer(int soundDataNum, bool loop);
};