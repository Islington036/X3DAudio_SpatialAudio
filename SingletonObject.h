#pragma once

// �V���O���g���݌v�̃I�u�W�F�N�g
template <typename T>
class SingletonObject
{
public:
	// �C���X�^���X�̐���
	static void Create()
	{
		// �C���X�^���X�����݂��Ȃ��ꍇ
		if (!m_Instance)
		{
			m_Instance = new T;
		}
	}

	// �C���X�^���X�̎擾
	static T& Instance()
	{
		// �C���X�^���X�����݂��Ȃ��ꍇ
		if (!m_Instance)
		{
			// �C���X�^���X�̐����������s��
			Create();
		}

		// �C���X�^���X��n��
		return *m_Instance;
	}

	// �C���X�^���X�̍폜
	static void InstanceRelease()
	{
		delete m_Instance;
	}

protected:
	SingletonObject() {}
	virtual ~SingletonObject() {}

	// �I�u�W�F�N�g�{��
    static T* m_Instance;

	// ����̐���
	SingletonObject& operator=(const SingletonObject& obj) = delete;

	// �R�s�[�R���X�g���N�^�̐���
	SingletonObject(const SingletonObject& obj) = delete;

	// ���[�u�R���X�g���N�^�̐���
	SingletonObject(SingletonObject&& obj) = delete;

	// ���[�u������Z�q�̐���
	SingletonObject& operator=(SingletonObject&& obj) = delete;
};

// �ϐ��̏�����
template <typename T>
T* SingletonObject<T>::m_Instance = nullptr;