#pragma once

#include <memory>
#include <string.h>
#include "SSBPLib/common/ssplayer_matrix.h"
#include "SSBPLib/SS5PlayerTypes.h"
#include "SSBPLib/SSPlayerStatus.h"

namespace ss{
class SS5Player;

/**
 * CustomSprite
 * ���t���[���ł̃f�[�^��Parts���ɂ����ɓ����
 */
class CustomSprite{
public:
	SSMatrix			m_mat;			//���̂Ƃ���Acell��worldMatrix���Ċ������B
	State				m_state;
	bool				m_isStateChanged;
	float				m_liveFrame;
	CustomSprite*		m_parent;
	std::shared_ptr<ss::SS5Player> m_ssplayer;	//customsprite�̓R�s�[�\�ɂ��Ƃ������̂�shared_ptr
	
	void initialize();
	void finalize();

	void matrixUpdate(const SSMatrix& parentMatrix);

public:
	CustomSprite();
	~CustomSprite();


	void setState(const State& state);


	SSV3F_C4B_T2F_Quad m_quad;		/// ���_�f�[�^�A���W�A�J���[�l�AUV���܂܂��i���_�ό`�A�T�C�YXY�AUV�ړ�XY�AUV�X�P�[���AUV��]�A���]�����f�ρj
	TextureID m_texture;			/// �Z���ɑΉ������e�N�X�`���ԍ��i�Q�[�����ŊǗ����Ă���ԍ���ݒ肷��j
	SSRect m_rect;					/// �Z���ɑΉ������e�N�X�`�����̕\���̈�i�J�n���W�A�������j
	int m_blendfunc;				/// �p�[�c�ɐݒ肳�ꂽ�u�����h���@
};


} //namespace ss
