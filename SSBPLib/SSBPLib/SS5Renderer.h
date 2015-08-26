#pragma once

#include "SS5PlayerTypes.h"

namespace ss{
struct SSV3F_C4B_T2F_Quad;


/** �`��@�\�͂�����p�����č���Ă������� */
class SS5Renderer{
public:
	SS5Renderer(){}
	virtual ~SS5Renderer(){}

	virtual void drawSprite(const SSV3F_C4B_T2F_Quad &q, TextureID handle) = 0;
	virtual bool SSGetTextureSize(TextureID handle, int &w, int &h) = 0;
};

	
} //namespace ss

