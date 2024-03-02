#pragma once

// シングルトン設計のオブジェクト
template <typename T>
class SingletonObject
{
public:
	// インスタンスの生成
	static void Create()
	{
		// インスタンスが存在しない場合
		if (!m_Instance)
		{
			m_Instance = new T;
		}
	}

	// インスタンスの取得
	static T& Instance()
	{
		// インスタンスが存在しない場合
		if (!m_Instance)
		{
			// インスタンスの生成処理を行う
			Create();
		}

		// インスタンスを渡す
		return *m_Instance;
	}

	// インスタンスの削除
	static void InstanceRelease()
	{
		delete m_Instance;
	}

protected:
	SingletonObject() {}
	virtual ~SingletonObject() {}

	// オブジェクト本体
    static T* m_Instance;

	// 代入の制限
	SingletonObject& operator=(const SingletonObject& obj) = delete;

	// コピーコンストラクタの制限
	SingletonObject(const SingletonObject& obj) = delete;

	// ムーブコンストラクタの制限
	SingletonObject(SingletonObject&& obj) = delete;

	// ムーブ代入演算子の制限
	SingletonObject& operator=(SingletonObject&& obj) = delete;
};

// 変数の初期化
template <typename T>
T* SingletonObject<T>::m_Instance = nullptr;