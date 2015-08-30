#pragma once

#include "DxLib.h"
#include "SSBPLib/SS5EventListener.h"
#include "SSBPLib/SS5Player.h"
#include "SSBPLib/SS5PlayerTypes.h"

/**
 * テクスチャのロード、リリースなどのイベントを捕まえて処理する
 */
class EventListener : public ss::SS5EventListener{
public:
	EventListener(){}
	~EventListener(){}

		
	ss::TextureID SSTextureLoad(const char* pszFileName) override{
		return LoadGraph(pszFileName);	//テクスチャ読みこみ
	}
	bool SSTextureRelese(ss::TextureID handle) override{
		DeleteGraph(handle);			//テクスチャ解放
		return true;
	}
	

	
	void SSonUserData(ss::SS5Player *player, const ss::UserData &userData, const char *partName, int frameNo) override{}
	void SSonBeginFrame(ss::SS5Player *player) override{}
	void SSonEndFrame(ss::SS5Player *player) override{}
};