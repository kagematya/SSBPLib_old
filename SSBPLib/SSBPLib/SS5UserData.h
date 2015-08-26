#pragma once


namespace ss{
class DataArrayReader;
class ToPointer;


/**
 * UserData
 */
struct UserData{
	
	enum {
		FLAG_INTEGER	= 1 << 0,
		FLAG_RECT		= 1 << 1,
		FLAG_POINT		= 1 << 2,
		FLAG_STRING		= 1 << 3
	};

	int			m_flags;		/// �܂܂��f�[�^�̎��
	int			m_partIndex;	/// ���̃f�[�^�̃p�[�c�C���f�b�N�X
	int			m_integer;		/// ����
	int			m_rect[4];		/// ��`�f�[�^
	int			m_point[2];		/// ���W�f�[�^
	int			m_strSize;		/// ������̃T�C�Y (byte count)
	const char*	m_str;			/// ������ (zero terminated)


	//reader����ăf�[�^��ǂݎ��
	void readData(DataArrayReader &reader, const ToPointer &ptr);

private:
	//�[���N���A
	void init();

};


} //namespace ss
