#include <assert.h>

#include "SoundManager.h"

int SearchSourceVoice(IXAudio2SourceVoice** SourceVoice, int size);

//=========================================
// �T�E���h�f�[�^�̒ǉ�
//=========================================
void SoundManager::AddSoundData(std::string name, std::string filePass, SoundData::Tag tag, float volume)
{
	Music_Data.push_back(std::make_unique<SoundData>(name, filePass, tag, volume));
}

//=========================================
// �T�E���h�f�[�^�̒ǉ�
//=========================================
void SoundManager::AddSoundData(SoundData& data)
{
	Music_Data.push_back(std::make_unique<SoundData>(std::move(data)));
}

//=========================================
// ���X�i�[�̈ʒu��ݒ�
//=========================================
void SoundManager::SetListenerPosition(D3DXVECTOR3 pos, D3DXVECTOR3 front, D3DXVECTOR3 top, D3DXVECTOR3 velocity)
{
	m_listener.Position = { pos.x, pos.y, pos.z };
	m_listener.OrientFront = { front.x, front.y, front.z };
	m_listener.OrientTop = { top.x, top.y, top.z };
	m_listener.Velocity = { velocity.x, velocity.y, velocity.z };
}

//=========================================
// ���y�f�[�^�̍Đ�
//=========================================
int SoundManager::PlayBGM(std::string name, bool loop)
{
	// ����g�p����T�E���h�f�[�^
	int soundDataNum = -1;

	// ����Đ����郊�\�[�X
	int sourceVoiceNum = -1;

	// ���������݂��Ȃ��A�������ǂݍ��܂�Ă��Ȃ��ꍇ�͏I��
	if (!SearchSoundData(name, &soundDataNum)) { return -1; }

	// �g�p�ł��郊�\�[�X������
	sourceVoiceNum = SearchSourceVoice(BGM_SourceVoice, BGM_Num);

	// �g�p�ł��郊�\�[�X���Ȃ��ꍇ��-1��Ԃ�
	if(sourceVoiceNum == -1) { return -1; }

	// �Đ���~���A�������͎c��̍Đ������ŏ��̃��\�[�X���g�p
	BGM_SourceVoice[sourceVoiceNum]->Stop();
	BGM_SourceVoice[sourceVoiceNum]->FlushSourceBuffers();

	// �\�[�X�{�C�X�̍쐬
	m_Xaudio->CreateSourceVoice(&BGM_SourceVoice[sourceVoiceNum], &Music_Data[soundDataNum].get()->m_wfx);

	// �o�b�t�@�ݒ�
	XAUDIO2_BUFFER bufinfo = SetBuffer(soundDataNum, loop);

	// �ݒ肵���o�b�t�@�𔽉f
	BGM_SourceVoice[sourceVoiceNum]->SubmitSourceBuffer(&bufinfo, NULL);

	// ���ʐݒ�
	BGM_SourceVoice[sourceVoiceNum]->SetVolume(MasterVolume * Music_Data[soundDataNum].get()->GetVolume());
	
	float outputMatrix[4] = { 0.0f , 0.0f, 1.0f , 0.0f };
	BGM_SourceVoice[sourceVoiceNum]->SetOutputMatrix(m_MasteringVoice, 2, 2, outputMatrix);
	
	// �Đ�
	BGM_SourceVoice[sourceVoiceNum]->Start();

	// �g�p���Ă���BGM���\�[�X�̔ԍ���Ԃ�
	return sourceVoiceNum;
}

//=========================================
// SE�̍Đ�
//=========================================
int SoundManager::PlaySE(std::string name, bool loop)
{
	// ����g�p����T�E���h�f�[�^
	int soundDataNum = -1;

	// ����Đ����郊�\�[�X
	int sourceVoiceNum = -1;

	// ���������݂��Ȃ��A�������ǂݍ��܂�Ă��Ȃ��ꍇ�͏I��
	if (!SearchSoundData(name, &soundDataNum)) { return -1; }

	// �g�p�ł��郊�\�[�X������
	sourceVoiceNum = SearchSourceVoice(SE_SourceVoice, SE_Num);

	// �g�p�ł��郊�\�[�X���Ȃ��ꍇ��-1��Ԃ�
	if (sourceVoiceNum == -1) { return -1; }

	// �Đ���~���A�������͎c��̍Đ������ŏ��̃��\�[�X���g�p
	SE_SourceVoice[sourceVoiceNum]->Stop();
	SE_SourceVoice[sourceVoiceNum]->FlushSourceBuffers();

	// �\�[�X�{�C�X�̍쐬
	m_Xaudio->CreateSourceVoice(&SE_SourceVoice[sourceVoiceNum], &Music_Data[soundDataNum].get()->m_wfx);

	// �o�b�t�@�ݒ�
	XAUDIO2_BUFFER bufinfo = SetBuffer(soundDataNum, loop);

	// �ݒ肵���o�b�t�@�𔽉f
	SE_SourceVoice[sourceVoiceNum]->SubmitSourceBuffer(&bufinfo, NULL);

	// ���ʐݒ�
	SE_SourceVoice[sourceVoiceNum]->SetVolume(MasterVolume * Music_Data[soundDataNum].get()->GetVolume());

	float outputMatrix[4] = { 0.0f , 0.0f, 1.0f , 0.0f };
	SE_SourceVoice[sourceVoiceNum]->SetOutputMatrix(m_MasteringVoice, 2, 2, outputMatrix);

	// �Đ�
	SE_SourceVoice[sourceVoiceNum]->Start();

	// �g�p���Ă���BGM���\�[�X�̔ԍ���Ԃ�
	return sourceVoiceNum;
}

//=========================================
// 3D�T�E���h��SE�Đ�
//=========================================
int SoundManager::PlaySE3D(std::string name, bool loop, D3DXVECTOR3 pos, float soundMaxVolumeRadius,
							float maxVolumeAngleRange, D3DXVECTOR3 front, D3DXVECTOR3 top, D3DXVECTOR3 velocity)
{
	// ����g�p����T�E���h�f�[�^
	int soundDataNum = -1;

	// ����Đ����郊�\�[�X
	int sourceVoiceNum = -1;

	// ���������݂��Ȃ��A�������ǂݍ��܂�Ă��Ȃ��ꍇ�͏I��
	if (!SearchSoundData(name, &soundDataNum)) { return -1; }

	// �g�p�ł��郊�\�[�X������
	sourceVoiceNum = SearchSourceVoice(SE_SourceVoice, SE_Num);

	// �g�p�ł��郊�\�[�X���Ȃ��ꍇ��-1��Ԃ�
	if (sourceVoiceNum == -1) { return -1; }

	// �Đ���~���A�������͎c��̍Đ������ŏ��̃��\�[�X���g�p
	SE_SourceVoice[sourceVoiceNum]->Stop();
	SE_SourceVoice[sourceVoiceNum]->FlushSourceBuffers();

	// �\�[�X�{�C�X�̍쐬
	m_Xaudio->CreateSourceVoice(&SE_SourceVoice[sourceVoiceNum], &Music_Data[soundDataNum].get()->m_wfx);

	// �o�b�t�@�ݒ�
	XAUDIO2_BUFFER bufinfo = SetBuffer(soundDataNum, loop);

	// �ݒ肵���o�b�t�@�𔽉f
	SE_SourceVoice[sourceVoiceNum]->SubmitSourceBuffer(&bufinfo, NULL);

	// ���ʐݒ�
	SE_SourceVoice[sourceVoiceNum]->SetVolume(MasterVolume * Music_Data[soundDataNum].get()->GetVolume());

	// 3D�T�E���h�̏��𔽉f
	Init3DSound(m_defaultEmitter, pos, soundMaxVolumeRadius, maxVolumeAngleRange, front, top, SE_SourceVoice[sourceVoiceNum]);

	// �Đ�
	SE_SourceVoice[sourceVoiceNum]->Start();

	// �g�p���Ă���BGM���\�[�X�̔ԍ���Ԃ�
	return sourceVoiceNum;
}

//=========================================
// �Đ�����SE�̈ʒu��ύX
//=========================================
void SoundManager::EmitterReset(int sourceVoiceNum, D3DXVECTOR3 pos, float soundMaxVolumeRadius, float maxVolumeAngleRange, D3DXVECTOR3 front, D3DXVECTOR3 top, D3DXVECTOR3 velocity)
{
	// 3D�T�E���h�̏��𔽉f
	Init3DSound(m_defaultEmitter, pos, soundMaxVolumeRadius, maxVolumeAngleRange, front, top, SE_SourceVoice[sourceVoiceNum]);
}

//=========================================
// �S�Ă�BGM���~
//=========================================
void SoundManager::StopAllBGM()
{
	for (IXAudio2SourceVoice* BGM : BGM_SourceVoice)
	{
		BGM->Stop();
		BGM->FlushSourceBuffers();
	}
}

//=========================================
// �S�Ă�SE���~
//=========================================
void SoundManager::StopAllSE()
{
	for (IXAudio2SourceVoice* SE : SE_SourceVoice)
	{
		SE->Stop();
		SE->FlushSourceBuffers();
	}
}

//=========================================
// ���\�[�X�ԍ��w���BGM���y�f�[�^���~
//=========================================
void SoundManager::StopBGM(int resourceNum)
{
	BGM_SourceVoice[resourceNum]->Stop();
	BGM_SourceVoice[resourceNum]->FlushSourceBuffers();
}

//=========================================
// ���\�[�X�ԍ��w���SE���y�f�[�^���~
//=========================================
void SoundManager::StopSE(int resourceNum)
{
	SE_SourceVoice[resourceNum]->Stop();
	SE_SourceVoice[resourceNum]->FlushSourceBuffers();
}

//=========================================
// �T�E���h�f�[�^�Ǎ�
//=========================================
void SoundManager::Load(SoundData::Tag loadScene)
{
	WAVEFORMATEX wfx = { 0 };

	HMMIO hmmio = NULL;
	MMIOINFO mmioinfo = { 0 };
	MMCKINFO riffchunkinfo = { 0 };
	MMCKINFO datachunkinfo = { 0 };
	MMCKINFO mmckinfo = { 0 };
	UINT32 buflen;
	LONG readlen;

	for (unsigned int i = 0; i < Music_Data.size(); i++)
	{
		// ���[�h�^�C�~���O�^�O�ƈ�v���Ȃ��ꍇ�̓X�L�b�v
		if(Music_Data[i].get()->m_tag != loadScene) { continue; }

		// �t�@�C�����J��
		hmmio = mmioOpen((LPSTR)Music_Data[i].get()->GetFilePass().c_str(), &mmioinfo, MMIO_READ);
		assert(hmmio);

		// RIFF�`�����N�̏����擾
		riffchunkinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		mmioDescend(hmmio, &riffchunkinfo, NULL, MMIO_FINDRIFF);

		// �t�H�[�}�b�g�`�����N�̏����擾
		mmckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmioDescend(hmmio, &mmckinfo, &riffchunkinfo, MMIO_FINDCHUNK);

		// �t�H�[�}�b�g���̓ǂݍ���
		if (mmckinfo.cksize >= sizeof(WAVEFORMATEX))
		{
			mmioRead(hmmio, (HPSTR)&wfx, sizeof(wfx));
		}
		else
		{
			PCMWAVEFORMAT pcmwf = { 0 };
			mmioRead(hmmio, (HPSTR)&pcmwf, sizeof(pcmwf));
			memset(&wfx, 0x00, sizeof(wfx));
			memcpy(&wfx, &pcmwf, sizeof(pcmwf));
			wfx.cbSize = 0;
		}

		// �f�[�^�`�����N�̏����擾
		mmioAscend(hmmio, &mmckinfo, 0);
		datachunkinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
		mmioDescend(hmmio, &datachunkinfo, &riffchunkinfo, MMIO_FINDCHUNK);

		// �f�[�^�̒������擾
		buflen = datachunkinfo.cksize;

		// �������m��
		Music_Data[i].get()->SetSoundData(std::make_unique<BYTE[]>(buflen));

		// �f�[�^�̓ǂݍ���
		readlen = mmioRead(hmmio, (HPSTR)Music_Data[i].get()->GetSoundData().get(), buflen);

		// �Ȃ̒�����ݒ�
		Music_Data[i].get()->m_Length = readlen;
		Music_Data[i].get()->m_PlayLength = readlen / wfx.nBlockAlign;

		// �t�@�C�������
		mmioClose(hmmio, 0);

		// �I�[�f�B�I�f�[�^�̃t�H�[�}�b�g����ݒ�
		Music_Data[i].get()->m_wfx = wfx;

		// ���[�h�ς݃t���O�𗧂Ă�
		Music_Data[i].get()->SetLoaded(true);

		hmmio = NULL;
	}
}

//=========================================
// �T�E���h�f�[�^�̍폜
//=========================================
void SoundManager::deleteMusicData(std::string name)
{
	for (unsigned int i = 0; i < Music_Data.size(); i++)
	{
		// ���O����v������폜
		if (Music_Data[i].get()->m_name == name)
		{
			Music_Data.erase(Music_Data.begin() + i);
			break;
		}
	}
}

//=========================================
// ����������
//=========================================
HRESULT SoundManager::Init()
{
	HRESULT hr;

	// COM������
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(hr)) { return hr; }

	// XAudio����
	hr = XAudio2Create(&m_Xaudio, 0);
	if (FAILED(hr)) { return hr; }

	// �}�X�^�����O�{�C�X����
	hr = m_Xaudio.Get()->CreateMasteringVoice(&m_MasteringVoice);
	if (FAILED(hr)) { return hr; }

	// �K���Ȓl�ŏ�����
	WAVEFORMATEX waveFormat = {};
	waveFormat.wFormatTag = WAVE_FORMAT_PCM;
	waveFormat.nChannels = 2; 
	waveFormat.nSamplesPerSec = 44100;
	waveFormat.wBitsPerSample = 16;
	waveFormat.nBlockAlign = (waveFormat.nChannels * waveFormat.wBitsPerSample) / 8;
	waveFormat.nAvgBytesPerSec = waveFormat.nSamplesPerSec * waveFormat.nBlockAlign;
	waveFormat.cbSize = 0;

	for (int i = 0; i < BGM_Num; i++)
	{
		// �\�[�X�{�C�X����
		hr = m_Xaudio.Get()->CreateSourceVoice(&BGM_SourceVoice[i], &waveFormat);
		if (FAILED(hr)) { return hr; }
	}

	for (int i = 0; i < SE_Num; i++)
	{
		// �\�[�X�{�C�X����
		hr = m_Xaudio.Get()->CreateSourceVoice(&SE_SourceVoice[i], &waveFormat);
		if (FAILED(hr)) { return hr; }
	}

	// �X�s�[�J�[���̎����擾
	// �g�p���Ă���SDK�̃o�[�W�����ɂ���Ă̓G���[���o��\��������̂ŁA�R�����g�A�E�g���Ă���܂�
	//pMasterVoice->GetChannelMask(&speakerChannelMask);

	//  X3DAudio�̏�����
	X3DAudioInitialize(m_speakerChannelMask, X3DAUDIO_SPEED_OF_SOUND, m_x3dAudioHandle);

	// ���X�i�[�̏�����
	ZeroMemory(&m_listener, sizeof(X3DAUDIO_LISTENER));
	m_listener.Position = { 0.0f, 0.0f, 0.0f };			// ���X�i�[�̈ʒu
	m_listener.OrientFront = { 0.0f, 0.0f, 1.0f };		// ���X�i�[�̑O����
	m_listener.OrientTop = { 0.0f, 1.0f, 0.0f };		// ���X�i�[�̏����
	m_listener.Velocity = { 0.0f, 0.0f, 0.0f };			// ���X�i�[�̑��x
	m_listener.pCone = nullptr;							// ���X�i�[�̃R�[�����

	// �G�~�b�^�[�̏�����
	ZeroMemory(&m_defaultEmitter, sizeof(X3DAUDIO_EMITTER));
	m_defaultEmitter.Position = { 0.0f, 0.0f, 0.0f };			// �G�~�b�^�[�̈ʒu
	m_defaultEmitter.Velocity = { 0.0f, 0.0f, 0.0f };			// �G�~�b�^�[�̑��x
	m_defaultEmitter.OrientFront = { 0.0f, 0.0f, 1.0f };		// �G�~�b�^�[�̑O����
	m_defaultEmitter.OrientTop = { 0.0f, 1.0f, 0.0f };			// �G�~�b�^�[�̏����
	m_defaultEmitter.pCone = nullptr;							// �G�~�b�^�[�̃R�[�����
	m_defaultEmitter.CurveDistanceScaler = 1.0f;				// �G�~�b�^�[�̌����J�[�u�����X�P�[��
	m_defaultEmitter.DopplerScaler = 1.0f;						// �G�~�b�^�[�̃h�b�v���[�X�P�[��
	m_defaultEmitter.pChannelAzimuths = nullptr;				// �G�~�b�^�[�̕��ʊp

	// �����ł̓X�e���I�I�[�f�B�I��z��
	m_defaultEmitter.ChannelCount = 2;							// �G�~�b�^�[�̃`�����l�����i�X�e���I�j
	m_defaultEmitter.pChannelAzimuths = new float[2] { -X3DAUDIO_PI / 2, X3DAUDIO_PI / 2 };	// �G�~�b�^�[�̕��ʊp

	// DSP�ݒ�̏�����
	ZeroMemory(&m_dspSettings, sizeof(X3DAUDIO_DSP_SETTINGS));
	
	// GetChannelMask�ŃX�s�[�J�[�̃`�����l�����������擾�ł���ꍇ�͉��ς��������߂��܂�
	m_dspSettings.SrcChannelCount = m_sourceChannelNum;
	m_dspSettings.DstChannelCount = m_speakerChannelNum;
	m_dspSettings.pMatrixCoefficients = new float[m_speakerChannelNum * m_sourceChannelNum];
	m_dspSettings.pDelayTimes = new float[m_speakerChannelNum];

	return hr;
}

//=========================================
// �I������
//=========================================
void SoundManager::Uninit()
{
	// �Đ���~
	StopAllBGM();
	StopAllSE();

	// BGM���\�[�X�̉��
	for (auto SourceVoice : BGM_SourceVoice)
	{
		SourceVoice->DestroyVoice();
	}

	// SE���\�[�X�̉��
	for (auto SourceVoice : SE_SourceVoice)
	{
		SourceVoice->DestroyVoice();
	}

	m_MasteringVoice->DestroyVoice();
	m_Xaudio.Get()->Release();

	CoUninitialize();
}

//=========================================
// 2�̃x�N�g���̊p�x���v�Z
//=========================================
float SoundManager::CalculateAngleBetweenVectors(const D3DXVECTOR3& from, const D3DXVECTOR3& to)
{
	// �x�N�g���𐳋K��
	D3DXVECTOR3 normFrom, normTo;
	D3DXVec3Normalize(&normFrom, &from);
	D3DXVec3Normalize(&normTo, &to);

	// ���ς��v�Z
	float dot = D3DXVec3Dot(&normFrom, &normTo);

	// �p�x�i���W�A���j���v�Z
	float radian = acos(dot);

	// ���W�A����x���@�ɕϊ�
	float degree = radian * (180.0f / float(D3DX_PI));

	return degree;
}

//===================================================
// �����̃\�[�X�{�C�X����g�p�ł��郊�\�[�X������
//===================================================
int SearchSourceVoice(IXAudio2SourceVoice** SourceVoice, int size)
{
	// ����Đ����郊�\�[�X
	int sourceVoiceNum = -1;

	// ���\�[�X�̃o�b�t�@
	unsigned int soundBuffer = 0;

	// �g�p�ł���Đ����\�[�X������
	for (int i = 0; i < size; i++)
	{
		// ���݂̏�Ԃ��擾
		XAUDIO2_VOICE_STATE state;
		SourceVoice[i]->GetState(&state);

		// �o�b�t�@��0�Ȃ�΍Đ��\�Ɣ��f
		if (state.BuffersQueued == 0)
		{
			sourceVoiceNum = i;
			break;
		}
		else
		{
			// �����l�������̓o�b�t�@���ŏ��̏ꍇ�͓���ւ�
			if (soundBuffer == 0 || soundBuffer > state.BuffersQueued)
			{
				soundBuffer = state.BuffersQueued;
				sourceVoiceNum = i;
			}
		}
	}

	return sourceVoiceNum;
}

//=========================================
// 3D�T�E���h�̏�����
//=========================================
void SoundManager::Init3DSound(X3DAUDIO_EMITTER emitter, D3DXVECTOR3 pos, float soundMaxVolumeRadius, float maxVolumeAngleRange,D3DXVECTOR3 front, D3DXVECTOR3 top, IXAudio2SourceVoice* SourceVoice)
{
	// 3D�T�E���h�̏��𔽉f
	emitter.Position = { pos.x, pos.y, pos.z };
	emitter.OrientFront = { front.x, front.y, front.z };
	emitter.OrientTop = { top.x, top.y, top.z };
	emitter.InnerRadius = soundMaxVolumeRadius;
	emitter.InnerRadiusAngle = maxVolumeAngleRange * (float(D3DX_PI) / 180.0f);	// ���W�A���ɕϊ�

	// 3D�T�E���h�̐ݒ�
	X3DAudioCalculate(m_x3dAudioHandle, &m_listener, &emitter,
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_DELAY | X3DAUDIO_CALCULATE_REVERB | X3DAUDIO_CALCULATE_EMITTER_ANGLE,
		&m_dspSettings);

	// 3D�T�E���h�̐ݒ���\�[�X�{�C�X�ɓK�p
	SourceVoice->SetOutputMatrix(m_MasteringVoice, m_sourceChannelNum, m_speakerChannelNum, m_dspSettings.pMatrixCoefficients);
}

//=========================================
// �T�E���h�f�[�^�̌���
//=========================================
bool SoundManager::SearchSoundData(std::string name, int* soundDataNum)
{
	// ������
	*soundDataNum = -1;

	// �T�E���h�f�[�^�̌���
	for (unsigned int i = 0; i < Music_Data.size(); i++)
	{
		if (Music_Data[i].get()->m_name == name)
		{
			*soundDataNum = i;
			break;
		}
	}

	// �������Č�����Ȃ������ꍇ�͏I��
	if (*soundDataNum == -1) { return false; }

	// �������ǂݍ��܂�Ă��Ȃ��ꍇ�͏I��
	if (!Music_Data[*soundDataNum].get()->IsLoaded()) { return false; }

	return true;
}

//===============================================
// �����f�[�^����f�[�^��ǂݍ���ŏ����ݒ�
//===============================================
XAUDIO2_BUFFER SoundManager::SetBuffer(int soundDataNum, bool loop)
{
	// �o�b�t�@�ݒ�
	XAUDIO2_BUFFER bufinfo;

	// �o�b�t�@�̏�����
	memset(&bufinfo, 0x00, sizeof(bufinfo));
	bufinfo.AudioBytes = Music_Data[soundDataNum].get()->m_Length;
	bufinfo.pAudioData = Music_Data[soundDataNum].get()->GetSoundData().get();
	bufinfo.PlayBegin = 0;
	bufinfo.PlayLength = Music_Data[soundDataNum].get()->m_PlayLength;

	// ���[�v�ݒ�
	if (loop)
	{
		bufinfo.LoopBegin = 0;
		bufinfo.LoopLength = Music_Data[soundDataNum].get()->m_PlayLength;
		bufinfo.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	return bufinfo;
}