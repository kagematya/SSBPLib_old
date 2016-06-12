#pragma once

#include "SS5PlayerTypes.h"
#include <string>

namespace ss{
class SS5Player;
struct UserData;
struct PartState;


/** ロードイベントなどを捕まえるのでこれを継承して作ってください */
class SS5EventListener{
public:
	SS5EventListener(){}
	virtual ~SS5EventListener(){}

	//テクスチャのロード・リリースのイベント
	virtual TextureID SSTextureLoad(const char* pszFileName) = 0;
	virtual bool SSTextureRelease(TextureID handle) = 0;
	

	/**
	 * インスタンスアニメーションのロード・リリースのイベント。ロード成功時はtrue返してください
	 * @param partsIndex	親になるパーツのindex
	 * @param partsName		親になるパーツの名前
	 * @param animName		再生アニメーション名
	 */
	virtual bool ChildPlayerLoad(int partsIndex, const std::string& partsName, const std::string& animName) = 0;
	virtual void ChildPlayerRelease(int partIndex, const std::string& partsName) = 0;
	
	/**
	 * 更新時などに呼び出されるSet系のイベント。親のパーツの情報を伝播させるために必要
	 * @param partsIndex	親になるパーツのindex
	 * @param partsName		親になるパーツの名前
	 * @param frame			インスタンスアニメの設定で指定したframeの計算結果
	 * @param independent	インスタンスアニメの設定で独立動作を指定していればtrue
	 * @param partState		親パーツの情報(座標とかが入っているので、子供のPlayerにセットしてください)
	 */
	virtual void ChildPlayerSetFrame(
		int partIndex, const std::string& partsName,
		int frame, bool independent, const PartState& partState
	) = 0;

	/** drawするタイミングでの呼び出し */
	virtual void ChildPlayerDraw(int partIndex, const std::string& partsName) = 0;


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

