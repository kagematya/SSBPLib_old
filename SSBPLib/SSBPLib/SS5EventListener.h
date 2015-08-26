#pragma once

#include "SS5PlayerTypes.h"

namespace ss{
class SS5Player;
struct UserData;


/** ���[�h�C�x���g�Ȃǂ�߂܂���̂ł�����p�����č���Ă������� */
class SS5EventListener{
public:
	SS5EventListener(){}
	virtual ~SS5EventListener(){}

		
	virtual TextureID SSTextureLoad(const char* pszFileName) = 0;
	virtual bool SSTextureRelese(TextureID handle) = 0;
	

	/**
	 * ���[�U�[�f�[�^���������Ƃ��ɌĂ΂��
	 * @param player
	 * @param userData	�ꎞ�I�u�W�F�N�g�Ȃ̂ŃR�s�[���Ďg���Ă�������
	 * @param partName	userData���ݒ肳��Ă���p�[�c�̖��O
	 * @param frameNo	userData���ݒ肳��Ă���t���[��
	 */
	virtual void SSonUserData(SS5Player *player, const UserData &userData, const char *partName, int frameNo) = 0;
	//virtual void SSPlayEnd(SS5Player *player) = 0;
	
	/** �ŏ��̃t���[���ɓ������Ƃ��ɌĂ΂�� */
	virtual void SSonBeginFrame(SS5Player *player) = 0;

	/** �Ō�̃t���[���ɓ������Ƃ��ɌĂ΂�� */
	virtual void SSonEndFrame(SS5Player *player) = 0;
};

	
} //namespace ss

