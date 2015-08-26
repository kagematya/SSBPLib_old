#pragma once

namespace ss{
class DataArrayReader;

/** �C���X�^���X�p�[�c�̏�Ԃ�\��(�E�E�E���Ă���̂Ǝv����BSS5Player.cpp���甲������������) */
class InstancePartStatus{
public:
	int m_refKeyframe;	//���̃C���X�^���X���z�u���ꂽ�L�[�t���[���i��Ύ��ԁj
	int m_refStartframe;
	int m_refEndframe;
	float m_refSpeed;
	int m_refloopNum;

	bool m_infinity;	//�������[�v
	bool m_reverse;		//�t�Đ�
	bool m_pingpong;	//����
	bool m_independent;	//�Ɨ�

	
	InstancePartStatus();

	//�C���X�^���X�p�[�c�̏�Ԃ�ǂݎ��
	void readInstancePartStatus(DataArrayReader &readerm, int readFlags);

	//�ǂݎ���ɂȂ񂩌v�Z���Ă镔���𔲂��o����
	int getTime(int time) const;	//time�Ƃ�����frame���Ǝv��

private:
	int endFrame() const;	//�t���O�����݂ďI���t���[����Ԃ�
};


} //namespace ss