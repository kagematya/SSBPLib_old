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


void CustomSprite::matrixUpdate(const SSMatrix& parentMatrix)
{
	SSMatrix tmp;
	SSMatrix mat;
	CustomSprite* sprite = this;

	sprite->m_state.matrixCompute(&mat);
	mat *= parentMatrix;

	sprite->m_mat = mat;
	sprite->m_state.m_mat = mat;

	if (sprite->m_parent)
	{
		CustomSprite* parent = sprite->m_parent;
		//子供は親のステータスを引き継ぐ

		sprite->m_state.m_opacity = ( sprite->m_state.m_opacity * parent->m_state.m_opacity ) / 255;

		//インスタンスパーツの親を設定
		if (sprite->m_ssplayer){
			float x, y;
			sprite->m_mat.getTranslation(&x, &y);

			sprite->m_ssplayer->setPosition(x, y);
			sprite->m_ssplayer->setScale(sprite->m_state.m_scaleX, sprite->m_state.m_scaleY);
			sprite->m_ssplayer->setRotation(sprite->m_state.m_rotationX, sprite->m_state.m_rotationY, sprite->m_state.m_rotationZ);
		}

	}

	//cellの原点計算を行う
	tmp.setTranslation(
		sprite->m_rect.size.width * (0.5f - sprite->m_state.m_anchorX),	//デフォルトがanchorX == 0.5になってる
		sprite->m_rect.size.height * (0.5f - sprite->m_state.m_anchorY)
	);
	sprite->m_state.m_mat = tmp  * sprite->m_state.m_mat;	//cellの原点移動をしてからmatrixが適用される形

	sprite->m_isStateChanged = false;
}




void CustomSprite::setState(const State &state)
{
	if(m_state != state){
		m_state = state;
		m_isStateChanged = true;
	}
}




} //namespace ss
