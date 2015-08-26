#include "AnimeCache.h"
#include <map>
#include <string>
#include <list>
#include "SSBPLib/SS5PlayerData.h"
#include "SSBPLib/common/SSUtil.h"
#include "ToPointer.h"

namespace ss{


AnimeCache::AnimeCache(const ProjectData *data)
{
	init(data);
}

AnimeCache::~AnimeCache()
{
}


//packNameとanimeNameを指定してAnimeRefを得る
const AnimeRef* AnimeCache::getReference(const std::string& packName, const std::string& animeName) const
{
	std::string key = toPackAnimeKey(packName, animeName);	//todo:animeNameだけに統一したい
	SS_ASSERT2(m_animeRefs.find(key) != m_animeRefs.end(), "Not found animation");
	
	return &(m_animeRefs.at(key));
}

//animeNameのみ指定してAnimeRefを得る
const AnimeRef* AnimeCache::getReference(const std::string& animeName) const
{
	SS_ASSERT2(m_animeRefs.find(animeName) != m_animeRefs.end(), "Not found animation");

	return &(m_animeRefs.at(animeName));
}

//最初のAnimeRefを得る
const AnimeRef* AnimeCache::getFirstReference() const
{
	SS_ASSERT2(m_animeRefs.begin() != m_animeRefs.end(), "No Animation");

	auto it = m_animeRefs.begin();
	return &(it->second);
}

//アニメーションのリストを作る
void AnimeCache::getAnimationList(std::list<std::string> *animlist) const
{
	//animlist->clear();
	for(auto &key_ref : m_animeRefs){
		animlist->push_back(key_ref.first);
	}
}




//dataを元にAnimeRefを構築
void AnimeCache::init(const ProjectData* data)
{
	SS_ASSERT2(data, "Invalid data");
		
	ToPointer ptr(data);
	const AnimePackData* animePacks = ptr.toAnimePackData(data);

	for (int packIndex = 0; packIndex < data->numAnimePacks; packIndex++){
		
		const AnimePackData* pack = &animePacks[packIndex];

		addAnimationData(ptr, pack);		//パッケージからAnimationDataを登録する
	}
}

//パッケージからAnimationDataを登録する
void AnimeCache::addAnimationData(ToPointer ptr, const AnimePackData *pack)
{
	const char* packName = ptr.toString(pack->name);
		
	const PartData* partDatas = ptr.toPartData(pack);				//array
	const AnimationData* animations = ptr.toAnimationData(pack);	//array
	
	for (int animeIndex = 0; animeIndex < pack->numAnimations; animeIndex++){
		const AnimationData* anime = &animations[animeIndex];
		const char* animeName = ptr.toString(anime->name);
			
		AnimeRef ref = {
			packName, animeName, anime, partDatas, pack->numParts
		};
			
		// packName + animeNameでの登録
		std::string key = toPackAnimeKey(packName, animeName);
		SSLOG("anime key: %s", key.c_str());
		m_animeRefs.insert(std::make_pair(key, ref));

		// animeNameのみでの登録
		m_animeRefs.insert(std::make_pair(animeName, ref));
	}
}




//ファイルパス生成
std::string AnimeCache::toPackAnimeKey(const std::string& packName, const std::string& animeName){
	return packName + "/" + animeName;		//return Format("%s/%s", packName.c_str(), animeName.c_str());
}


} //namespace ss
