#include "CustomSprite.h"
#include "SSBPLib/SS5Player.h"
#include "SSBPLib/common/SSUtil.h"

namespace ss{

	
CustomSprite::CustomSprite()
	: m_isStateChanged(true)
	, m_liveFrame(0.0f)
	, m_parent(nullptr)
	, m_ssplayer(nullptr)
{
	initialize();
}

CustomSprite::~CustomSprite()
{
	finalize();
}


void CustomSprite::initialize()
{
	m_mat.setupIdentity();
	m_state.init();
	m_isStateChanged = true;
	m_liveFrame = 0.0f;
	m_parent = nullptr;

	m_ssplayer.reset();

	m_quad = SSV3F_C4B_T2F_Quad();
	m_texture = 0;
	m_rect.size.height = 0;
	m_rect.size.width = 0;
	m_rect.origin.x = 0;
	m_rect.origin.y = 0;
	m_blendfunc = 0;
}

void CustomSprite::finalize()
{
	m_parent = nullptr;
	m_ssplayer.reset();
}


void CustomSprite::updateMatrixAndOpacity(const SSMatrix& rootMatrix, int rootOpacity)
{
	SSMatrix parentMatrix;
	int parentOpacity;
	if(m_parent){
		parentMatrix = m_parent->m_mat;
		parentOpacity = m_parent->m_state.m_opacity;
	}
	else{
		parentMatrix = rootMatrix;
		parentOpacity = rootOpacity;
	}

	//�s��v�Z
	SSMatrix mat;
	m_state.matrixCompute(&mat);
	mat *= parentMatrix;

	m_mat = mat;
	m_state.m_mat = mat;

	
	//�A���t�@�̓`�d
	m_state.m_opacity = ( m_state.m_opacity * parentOpacity ) / 255;

	//�C���X�^���X�p�[�c�̐e��ݒ�
	if (m_ssplayer){
		float x, y;
		m_mat.getTranslation(&x, &y);

		m_ssplayer->setPosition(x, y);
		m_ssplayer->setScale(m_state.m_scaleX, m_state.m_scaleY);
		m_ssplayer->setRotation(m_state.m_rotationX, m_state.m_rotationY, m_state.m_rotationZ);
	}
	

	//cell�̌��_�v�Z���s��
	SSMatrix tmp;
	tmp.setTranslation(
		m_rect.size.width * (0.5f - m_state.m_anchorX),	//�f�t�H���g��anchorX == 0.5�ɂȂ��Ă�
		m_rect.size.height * (0.5f - m_state.m_anchorY)
	);
	m_state.m_mat = tmp  * m_state.m_mat;	//cell�̌��_�ړ������Ă���matrix���K�p�����`

	m_isStateChanged = false;
}




void CustomSprite::setState(const State &state)
{
	if(m_state != state){
		m_state = state;
		m_isStateChanged = true;
	}
}




} //namespace ss
