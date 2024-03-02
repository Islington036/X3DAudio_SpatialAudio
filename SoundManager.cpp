#include <assert.h>

#include "SoundManager.h"

int SearchSourceVoice(IXAudio2SourceVoice** SourceVoice, int size);

//=========================================
// サウンドデータの追加
//=========================================
void SoundManager::AddSoundData(std::string name, std::string filePass, SoundData::Tag tag, float volume)
{
	Music_Data.push_back(std::make_unique<SoundData>(name, filePass, tag, volume));
}

//=========================================
// サウンドデータの追加
//=========================================
void SoundManager::AddSoundData(SoundData& data)
{
	Music_Data.push_back(std::make_unique<SoundData>(std::move(data)));
}

//=========================================
// リスナーの位置を設定
//=========================================
void SoundManager::SetListenerPosition(D3DXVECTOR3 pos, D3DXVECTOR3 front, D3DXVECTOR3 top, D3DXVECTOR3 velocity)
{
	m_listener.Position = { pos.x, pos.y, pos.z };
	m_listener.OrientFront = { front.x, front.y, front.z };
	m_listener.OrientTop = { top.x, top.y, top.z };
	m_listener.Velocity = { velocity.x, velocity.y, velocity.z };
}

//=========================================
// 音楽データの再生
//=========================================
int SoundManager::PlayBGM(std::string name, bool loop)
{
	// 今回使用するサウンドデータ
	int soundDataNum = -1;

	// 今回再生するリソース
	int sourceVoiceNum = -1;

	// 音源が存在しない、音源が読み込まれていない場合は終了
	if (!SearchSoundData(name, &soundDataNum)) { return -1; }

	// 使用できるリソースを検索
	sourceVoiceNum = SearchSourceVoice(BGM_SourceVoice, BGM_Num);

	// 使用できるリソースがない場合は-1を返す
	if(sourceVoiceNum == -1) { return -1; }

	// 再生停止中、もしくは残りの再生数が最小のリソースを使用
	BGM_SourceVoice[sourceVoiceNum]->Stop();
	BGM_SourceVoice[sourceVoiceNum]->FlushSourceBuffers();

	// ソースボイスの作成
	m_Xaudio->CreateSourceVoice(&BGM_SourceVoice[sourceVoiceNum], &Music_Data[soundDataNum].get()->m_wfx);

	// バッファ設定
	XAUDIO2_BUFFER bufinfo = SetBuffer(soundDataNum, loop);

	// 設定したバッファを反映
	BGM_SourceVoice[sourceVoiceNum]->SubmitSourceBuffer(&bufinfo, NULL);

	// 音量設定
	BGM_SourceVoice[sourceVoiceNum]->SetVolume(MasterVolume * Music_Data[soundDataNum].get()->GetVolume());
	
	float outputMatrix[4] = { 0.0f , 0.0f, 1.0f , 0.0f };
	BGM_SourceVoice[sourceVoiceNum]->SetOutputMatrix(m_MasteringVoice, 2, 2, outputMatrix);
	
	// 再生
	BGM_SourceVoice[sourceVoiceNum]->Start();

	// 使用しているBGMリソースの番号を返す
	return sourceVoiceNum;
}

//=========================================
// SEの再生
//=========================================
int SoundManager::PlaySE(std::string name, bool loop)
{
	// 今回使用するサウンドデータ
	int soundDataNum = -1;

	// 今回再生するリソース
	int sourceVoiceNum = -1;

	// 音源が存在しない、音源が読み込まれていない場合は終了
	if (!SearchSoundData(name, &soundDataNum)) { return -1; }

	// 使用できるリソースを検索
	sourceVoiceNum = SearchSourceVoice(SE_SourceVoice, SE_Num);

	// 使用できるリソースがない場合は-1を返す
	if (sourceVoiceNum == -1) { return -1; }

	// 再生停止中、もしくは残りの再生数が最小のリソースを使用
	SE_SourceVoice[sourceVoiceNum]->Stop();
	SE_SourceVoice[sourceVoiceNum]->FlushSourceBuffers();

	// ソースボイスの作成
	m_Xaudio->CreateSourceVoice(&SE_SourceVoice[sourceVoiceNum], &Music_Data[soundDataNum].get()->m_wfx);

	// バッファ設定
	XAUDIO2_BUFFER bufinfo = SetBuffer(soundDataNum, loop);

	// 設定したバッファを反映
	SE_SourceVoice[sourceVoiceNum]->SubmitSourceBuffer(&bufinfo, NULL);

	// 音量設定
	SE_SourceVoice[sourceVoiceNum]->SetVolume(MasterVolume * Music_Data[soundDataNum].get()->GetVolume());

	float outputMatrix[4] = { 0.0f , 0.0f, 1.0f , 0.0f };
	SE_SourceVoice[sourceVoiceNum]->SetOutputMatrix(m_MasteringVoice, 2, 2, outputMatrix);

	// 再生
	SE_SourceVoice[sourceVoiceNum]->Start();

	// 使用しているBGMリソースの番号を返す
	return sourceVoiceNum;
}

//=========================================
// 3DサウンドのSE再生
//=========================================
int SoundManager::PlaySE3D(std::string name, bool loop, D3DXVECTOR3 pos, float soundMaxVolumeRadius,
							float maxVolumeAngleRange, D3DXVECTOR3 front, D3DXVECTOR3 top, D3DXVECTOR3 velocity)
{
	// 今回使用するサウンドデータ
	int soundDataNum = -1;

	// 今回再生するリソース
	int sourceVoiceNum = -1;

	// 音源が存在しない、音源が読み込まれていない場合は終了
	if (!SearchSoundData(name, &soundDataNum)) { return -1; }

	// 使用できるリソースを検索
	sourceVoiceNum = SearchSourceVoice(SE_SourceVoice, SE_Num);

	// 使用できるリソースがない場合は-1を返す
	if (sourceVoiceNum == -1) { return -1; }

	// 再生停止中、もしくは残りの再生数が最小のリソースを使用
	SE_SourceVoice[sourceVoiceNum]->Stop();
	SE_SourceVoice[sourceVoiceNum]->FlushSourceBuffers();

	// ソースボイスの作成
	m_Xaudio->CreateSourceVoice(&SE_SourceVoice[sourceVoiceNum], &Music_Data[soundDataNum].get()->m_wfx);

	// バッファ設定
	XAUDIO2_BUFFER bufinfo = SetBuffer(soundDataNum, loop);

	// 設定したバッファを反映
	SE_SourceVoice[sourceVoiceNum]->SubmitSourceBuffer(&bufinfo, NULL);

	// 音量設定
	SE_SourceVoice[sourceVoiceNum]->SetVolume(MasterVolume * Music_Data[soundDataNum].get()->GetVolume());

	// 3Dサウンドの情報を反映
	Init3DSound(m_defaultEmitter, pos, soundMaxVolumeRadius, maxVolumeAngleRange, front, top, SE_SourceVoice[sourceVoiceNum]);

	// 再生
	SE_SourceVoice[sourceVoiceNum]->Start();

	// 使用しているBGMリソースの番号を返す
	return sourceVoiceNum;
}

//=========================================
// 再生中のSEの位置を変更
//=========================================
void SoundManager::EmitterReset(int sourceVoiceNum, D3DXVECTOR3 pos, float soundMaxVolumeRadius, float maxVolumeAngleRange, D3DXVECTOR3 front, D3DXVECTOR3 top, D3DXVECTOR3 velocity)
{
	// 3Dサウンドの情報を反映
	Init3DSound(m_defaultEmitter, pos, soundMaxVolumeRadius, maxVolumeAngleRange, front, top, SE_SourceVoice[sourceVoiceNum]);
}

//=========================================
// 全てのBGMを停止
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
// 全てのSEを停止
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
// リソース番号指定でBGM音楽データを停止
//=========================================
void SoundManager::StopBGM(int resourceNum)
{
	BGM_SourceVoice[resourceNum]->Stop();
	BGM_SourceVoice[resourceNum]->FlushSourceBuffers();
}

//=========================================
// リソース番号指定でSE音楽データを停止
//=========================================
void SoundManager::StopSE(int resourceNum)
{
	SE_SourceVoice[resourceNum]->Stop();
	SE_SourceVoice[resourceNum]->FlushSourceBuffers();
}

//=========================================
// サウンドデータ読込
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
		// ロードタイミングタグと一致しない場合はスキップ
		if(Music_Data[i].get()->m_tag != loadScene) { continue; }

		// ファイルを開く
		hmmio = mmioOpen((LPSTR)Music_Data[i].get()->GetFilePass().c_str(), &mmioinfo, MMIO_READ);
		assert(hmmio);

		// RIFFチャンクの情報を取得
		riffchunkinfo.fccType = mmioFOURCC('W', 'A', 'V', 'E');
		mmioDescend(hmmio, &riffchunkinfo, NULL, MMIO_FINDRIFF);

		// フォーマットチャンクの情報を取得
		mmckinfo.ckid = mmioFOURCC('f', 'm', 't', ' ');
		mmioDescend(hmmio, &mmckinfo, &riffchunkinfo, MMIO_FINDCHUNK);

		// フォーマット情報の読み込み
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

		// データチャンクの情報を取得
		mmioAscend(hmmio, &mmckinfo, 0);
		datachunkinfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
		mmioDescend(hmmio, &datachunkinfo, &riffchunkinfo, MMIO_FINDCHUNK);

		// データの長さを取得
		buflen = datachunkinfo.cksize;

		// メモリ確保
		Music_Data[i].get()->SetSoundData(std::make_unique<BYTE[]>(buflen));

		// データの読み込み
		readlen = mmioRead(hmmio, (HPSTR)Music_Data[i].get()->GetSoundData().get(), buflen);

		// 曲の長さを設定
		Music_Data[i].get()->m_Length = readlen;
		Music_Data[i].get()->m_PlayLength = readlen / wfx.nBlockAlign;

		// ファイルを閉じる
		mmioClose(hmmio, 0);

		// オーディオデータのフォーマット情報を設定
		Music_Data[i].get()->m_wfx = wfx;

		// ロード済みフラグを立てる
		Music_Data[i].get()->SetLoaded(true);

		hmmio = NULL;
	}
}

//=========================================
// サウンドデータの削除
//=========================================
void SoundManager::deleteMusicData(std::string name)
{
	for (unsigned int i = 0; i < Music_Data.size(); i++)
	{
		// 名前が一致したら削除
		if (Music_Data[i].get()->m_name == name)
		{
			Music_Data.erase(Music_Data.begin() + i);
			break;
		}
	}
}

//=========================================
// 初期化処理
//=========================================
HRESULT SoundManager::Init()
{
	HRESULT hr;

	// COM初期化
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(hr)) { return hr; }

	// XAudio生成
	hr = XAudio2Create(&m_Xaudio, 0);
	if (FAILED(hr)) { return hr; }

	// マスタリングボイス生成
	hr = m_Xaudio.Get()->CreateMasteringVoice(&m_MasteringVoice);
	if (FAILED(hr)) { return hr; }

	// 適当な値で初期化
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
		// ソースボイス生成
		hr = m_Xaudio.Get()->CreateSourceVoice(&BGM_SourceVoice[i], &waveFormat);
		if (FAILED(hr)) { return hr; }
	}

	for (int i = 0; i < SE_Num; i++)
	{
		// ソースボイス生成
		hr = m_Xaudio.Get()->CreateSourceVoice(&SE_SourceVoice[i], &waveFormat);
		if (FAILED(hr)) { return hr; }
	}

	// スピーカー環境の自動取得
	// 使用しているSDKのバージョンによってはエラーが出る可能性があるので、コメントアウトしてあります
	//pMasterVoice->GetChannelMask(&speakerChannelMask);

	//  X3DAudioの初期化
	X3DAudioInitialize(m_speakerChannelMask, X3DAUDIO_SPEED_OF_SOUND, m_x3dAudioHandle);

	// リスナーの初期化
	ZeroMemory(&m_listener, sizeof(X3DAUDIO_LISTENER));
	m_listener.Position = { 0.0f, 0.0f, 0.0f };			// リスナーの位置
	m_listener.OrientFront = { 0.0f, 0.0f, 1.0f };		// リスナーの前方向
	m_listener.OrientTop = { 0.0f, 1.0f, 0.0f };		// リスナーの上方向
	m_listener.Velocity = { 0.0f, 0.0f, 0.0f };			// リスナーの速度
	m_listener.pCone = nullptr;							// リスナーのコーン情報

	// エミッターの初期化
	ZeroMemory(&m_defaultEmitter, sizeof(X3DAUDIO_EMITTER));
	m_defaultEmitter.Position = { 0.0f, 0.0f, 0.0f };			// エミッターの位置
	m_defaultEmitter.Velocity = { 0.0f, 0.0f, 0.0f };			// エミッターの速度
	m_defaultEmitter.OrientFront = { 0.0f, 0.0f, 1.0f };		// エミッターの前方向
	m_defaultEmitter.OrientTop = { 0.0f, 1.0f, 0.0f };			// エミッターの上方向
	m_defaultEmitter.pCone = nullptr;							// エミッターのコーン情報
	m_defaultEmitter.CurveDistanceScaler = 1.0f;				// エミッターの減衰カーブ距離スケール
	m_defaultEmitter.DopplerScaler = 1.0f;						// エミッターのドップラースケール
	m_defaultEmitter.pChannelAzimuths = nullptr;				// エミッターの方位角

	// ここではステレオオーディオを想定
	m_defaultEmitter.ChannelCount = 2;							// エミッターのチャンネル数（ステレオ）
	m_defaultEmitter.pChannelAzimuths = new float[2] { -X3DAUDIO_PI / 2, X3DAUDIO_PI / 2 };	// エミッターの方位角

	// DSP設定の初期化
	ZeroMemory(&m_dspSettings, sizeof(X3DAUDIO_DSP_SETTINGS));
	
	// GetChannelMaskでスピーカーのチャンネル数を自動取得できる場合は改変をおすすめします
	m_dspSettings.SrcChannelCount = m_sourceChannelNum;
	m_dspSettings.DstChannelCount = m_speakerChannelNum;
	m_dspSettings.pMatrixCoefficients = new float[m_speakerChannelNum * m_sourceChannelNum];
	m_dspSettings.pDelayTimes = new float[m_speakerChannelNum];

	return hr;
}

//=========================================
// 終了処理
//=========================================
void SoundManager::Uninit()
{
	// 再生停止
	StopAllBGM();
	StopAllSE();

	// BGMリソースの解放
	for (auto SourceVoice : BGM_SourceVoice)
	{
		SourceVoice->DestroyVoice();
	}

	// SEリソースの解放
	for (auto SourceVoice : SE_SourceVoice)
	{
		SourceVoice->DestroyVoice();
	}

	m_MasteringVoice->DestroyVoice();
	m_Xaudio.Get()->Release();

	CoUninitialize();
}

//=========================================
// 2つのベクトルの角度を計算
//=========================================
float SoundManager::CalculateAngleBetweenVectors(const D3DXVECTOR3& from, const D3DXVECTOR3& to)
{
	// ベクトルを正規化
	D3DXVECTOR3 normFrom, normTo;
	D3DXVec3Normalize(&normFrom, &from);
	D3DXVec3Normalize(&normTo, &to);

	// 内積を計算
	float dot = D3DXVec3Dot(&normFrom, &normTo);

	// 角度（ラジアン）を計算
	float radian = acos(dot);

	// ラジアンを度数法に変換
	float degree = radian * (180.0f / float(D3DX_PI));

	return degree;
}

//===================================================
// 複数のソースボイスから使用できるリソースを検索
//===================================================
int SearchSourceVoice(IXAudio2SourceVoice** SourceVoice, int size)
{
	// 今回再生するリソース
	int sourceVoiceNum = -1;

	// リソースのバッファ
	unsigned int soundBuffer = 0;

	// 使用できる再生リソースを検索
	for (int i = 0; i < size; i++)
	{
		// 現在の状態を取得
		XAUDIO2_VOICE_STATE state;
		SourceVoice[i]->GetState(&state);

		// バッファが0ならば再生可能と判断
		if (state.BuffersQueued == 0)
		{
			sourceVoiceNum = i;
			break;
		}
		else
		{
			// 初期値もしくはバッファが最小の場合は入れ替え
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
// 3Dサウンドの初期化
//=========================================
void SoundManager::Init3DSound(X3DAUDIO_EMITTER emitter, D3DXVECTOR3 pos, float soundMaxVolumeRadius, float maxVolumeAngleRange,D3DXVECTOR3 front, D3DXVECTOR3 top, IXAudio2SourceVoice* SourceVoice)
{
	// 3Dサウンドの情報を反映
	emitter.Position = { pos.x, pos.y, pos.z };
	emitter.OrientFront = { front.x, front.y, front.z };
	emitter.OrientTop = { top.x, top.y, top.z };
	emitter.InnerRadius = soundMaxVolumeRadius;
	emitter.InnerRadiusAngle = maxVolumeAngleRange * (float(D3DX_PI) / 180.0f);	// ラジアンに変換

	// 3Dサウンドの設定
	X3DAudioCalculate(m_x3dAudioHandle, &m_listener, &emitter,
		X3DAUDIO_CALCULATE_MATRIX | X3DAUDIO_CALCULATE_DOPPLER | X3DAUDIO_CALCULATE_DELAY | X3DAUDIO_CALCULATE_REVERB | X3DAUDIO_CALCULATE_EMITTER_ANGLE,
		&m_dspSettings);

	// 3Dサウンドの設定をソースボイスに適用
	SourceVoice->SetOutputMatrix(m_MasteringVoice, m_sourceChannelNum, m_speakerChannelNum, m_dspSettings.pMatrixCoefficients);
}

//=========================================
// サウンドデータの検索
//=========================================
bool SoundManager::SearchSoundData(std::string name, int* soundDataNum)
{
	// 初期化
	*soundDataNum = -1;

	// サウンドデータの検索
	for (unsigned int i = 0; i < Music_Data.size(); i++)
	{
		if (Music_Data[i].get()->m_name == name)
		{
			*soundDataNum = i;
			break;
		}
	}

	// 検索して見つからなかった場合は終了
	if (*soundDataNum == -1) { return false; }

	// 音源が読み込まれていない場合は終了
	if (!Music_Data[*soundDataNum].get()->IsLoaded()) { return false; }

	return true;
}

//===============================================
// 音源データからデータを読み込んで初期設定
//===============================================
XAUDIO2_BUFFER SoundManager::SetBuffer(int soundDataNum, bool loop)
{
	// バッファ設定
	XAUDIO2_BUFFER bufinfo;

	// バッファの初期化
	memset(&bufinfo, 0x00, sizeof(bufinfo));
	bufinfo.AudioBytes = Music_Data[soundDataNum].get()->m_Length;
	bufinfo.pAudioData = Music_Data[soundDataNum].get()->GetSoundData().get();
	bufinfo.PlayBegin = 0;
	bufinfo.PlayLength = Music_Data[soundDataNum].get()->m_PlayLength;

	// ループ設定
	if (loop)
	{
		bufinfo.LoopBegin = 0;
		bufinfo.LoopLength = Music_Data[soundDataNum].get()->m_PlayLength;
		bufinfo.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	return bufinfo;
}