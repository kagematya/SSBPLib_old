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




void CustomSprite::setState(const State &state)
{
	if(m_state != state){
		m_state = state;
		m_isStateChanged = true;
	}
}




} //namespace ss
