#pragma once

#include <map>
#include <string>
#include <list>
#include <assert.h>
#include "ToPointer.h"

namespace ss{
struct ProjectData;
struct AnimationData;
struct PartData;
struct AnimePackData;


/** AnimationDataへのアクセスを提供。パーツデータはパッケージ共通のものだが、利便性のためここで抱えておく */
struct AnimeRef{
	std::string				m_packName;			//todo:stringじゃなくてconst char*の方がよさそう
	std::string				m_animeName;		//todo:stringじゃなくてconst char*の方がよさそう
	const AnimationData*	m_animationData;	//アニメーション

	//const AnimePackData*	animePackData;		//属するパッケージ
	const PartData*			m_partDatas;		//パーツデータへのショートカットaniemPackData->partDatas[]
	int						m_numParts;			//パーツデータ数

	//PartDataへのアクセサ
	const PartData* getPartData(int partIndex) const{
		assert(partIndex >= 0 && partIndex < m_numParts);
		return &m_partDatas[partIndex];
	}
};



/** AnimationDataにアクセスしやすいようにここで抱えておく */
class AnimeCache{
public:
	/** dataを元にAnimeRefを構築 */
	AnimeCache(const ProjectData *data);
	~AnimeCache();
	

	/** packNameとanimeNameを指定してAnimeRefを得る */
	const AnimeRef* getReference(const std::string& packName, const std::string& animeName) const;

	/** animeNameのみ指定してAnimeRefを得る */
	const AnimeRef* getReference(const std::string& animeName) const;
	

	/** 先頭のアニメーションを返す */	//memo:SS5Playerの初期化で必要
	const AnimeRef* getFirstReference() const;

	
	/** アニメーションの登録名のリストを取得する。主にデバッグ用 */
	void getAnimationList(std::list<std::string> *animlist) const;

private:
	/** dataを元にAnimeRefを構築 */
	void init(const ProjectData* data);
	void addAnimationData(ToPointer ptr, const AnimePackData *pack);

	/** ファイルパス生成 */
	static std::string toPackAnimeKey(const std::string& packName, const std::string& animeName);
	

	std::map<std::string, AnimeRef>	m_animeRefs;	//memo:mapなら実体でも問題ないはず   <animekey, animeref>
};


} //namespace ss
