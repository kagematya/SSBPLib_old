#pragma once

#include "SS5PlayerTypes.h"

namespace ss{
class SS5Player;
struct UserData;


/** ロードイベントなどを捕まえるのでこれを継承して作ってください */
class SS5EventListener{
public:
	SS5EventListener(){}
	virtual ~SS5EventListener(){}

		
	virtual TextureID SSTextureLoad(const char* pszFileName) = 0;
	virtual bool SSTextureRelese(TextureID handle) = 0;
	

	/**
	 * ユーザーデータがあったときに呼ばれる
	 * @param player
	 * @param userData	一時オブジェクトなのでコピーして使ってください
	 * @param partName	userDataが設定されているパーツの名前
	 * @param frameNo	userDataが設定されているフレーム
	 */
	virtual void SSonUserData(SS5Player *player, const UserData &userData, const char *partName, int frameNo) = 0;
	//virtual void SSPlayEnd(SS5Player *player) = 0;
	
	/** 最初のフレームに入ったときに呼ばれる */
	virtual void SSonBeginFrame(SS5Player *player) = 0;

	/** 最後のフレームに入ったときに呼ばれる */
	virtual void SSonEndFrame(SS5Player *player) = 0;
};

	
} //namespace ss

