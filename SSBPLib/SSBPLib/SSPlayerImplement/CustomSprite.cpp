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

	//行列計算
	SSMatrix mat;
	m_state.matrixCompute(&mat);
	mat *= parentMatrix;

	m_mat = mat;
	m_state.m_mat = mat;

	
	//アルファの伝播
	m_state.m_opacity = ( m_state.m_opacity * parentOpacity ) / 255;

	//インスタンスパーツの親を設定
	if (m_ssplayer){
		float x, y;
		m_mat.getTranslation(&x, &y);

		m_ssplayer->setPosition(x, y);
		m_ssplayer->setScale(m_state.m_scaleX, m_state.m_scaleY);
		m_ssplayer->setRotation(m_state.m_rotationX, m_state.m_rotationY, m_state.m_rotationZ);
	}
	

	//cellの原点計算を行う
	SSMatrix tmp;
	tmp.setTranslation(
		m_rect.size.width * (0.5f - m_state.m_anchorX),	//デフォルトがanchorX == 0.5になってる
		m_rect.size.height * (0.5f - m_state.m_anchorY)
	);
	m_state.m_mat = tmp  * m_state.m_mat;	//cellの原点移動をしてからmatrixが適用される形

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
