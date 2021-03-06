#pragma once

#include "DxLib.h"
#include "SSBPLib/SS5Renderer.h"
#include "SSBPLib/SS5PlayerTypes.h"

/** 描画機能はここに実装する */
class Renderer : public ss::SS5Renderer{
public:
	Renderer(){}
	~Renderer(){}

	void drawSprite(const ss::SSV3F_C4B_T2F_Quad &q, ss::TextureID handle) override{
		
		VERTEX_3D vertex[4] = {
			vertex3Dfrom(q.tl),
			vertex3Dfrom(q.tr),
			vertex3Dfrom(q.bl),
			vertex3Dfrom(q.br)
		};
		DrawPolygon3DBase(vertex, 4, DX_PRIMTYPE_TRIANGLESTRIP, handle, true);
	}

	bool SSGetTextureSize(ss::TextureID handle, int &w, int &h) override{
		GetGraphSize(handle, &w, &h);
		return true;
	}

	
private:
	VERTEX_3D vertex3Dfrom(const ss::SSV3F_C4B_T2F &vct) const{
		VERTEX_3D v = {
			{vct.vertices.x, vct.vertices.y, vct.vertices.z},
			vct.colors.b, vct.colors.g, vct.colors.r, vct.colors.a,
			vct.texCoords.u, vct.texCoords.v
		};
		return v;
	}
};

