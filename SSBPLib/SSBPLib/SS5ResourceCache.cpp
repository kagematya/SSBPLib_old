#include "SS5ResourceCache.h"
#include <string>
#include "common/SSUtil.h"
#include "SS5PlayerData.h"
#include "SSPlayerImplement/ToPointer.h"
#include "SSPlayerImplement/CellCache.h"
#include "SSPlayerImplement/AnimeCache.h"
#include "SSPlayerImplement/ResourceSet.h"
#include "SS5Player.h"
#include "SS5Renderer.h"
#include "SS5EventListener.h"


static void splitPath(std::string& directoty, std::string& filename, const std::string& path)
{
    std::string f = path;
    std::string d = "";

    size_t pos = path.find_last_of("/");
	if (pos == std::string::npos) pos = path.find_last_of("\\");	// for win

    if (pos != std::string::npos)
    {
        d = path.substr(0, pos+1);
        f = path.substr(pos+1);
    }

	directoty = d;
	filename = f;
}



namespace ss{


const std::string SS5ResourceCache::s_null;


SS5ResourceCache::SS5ResourceCache()
{
}

SS5ResourceCache::~SS5ResourceCache()
{
	unregistAll();
}



void SS5ResourceCache::regist(const void *data, size_t dataSize, const std::string &dataKey, const std::string &imageBaseDir)
{
	SS_ASSERT2(data, "Invalid data");
	//登録済みかどうかの判定
	if(m_dataDic.find(dataKey) != m_dataDic.end()){
		RefcountResourceSet *ref = m_dataDic.at(dataKey);
		ref->incCount();	//登録済みの場合はカウントアップするだけ。dataの内容は無視(最初に登録されてたもの優先)
		return;
	}

	/***** 新規登録 *****/
	
	//画像ファイルのディレクトリパスを作る
	std::string baseDir = getImageBaseDir(imageBaseDir, static_cast<const ProjectData*>(data));

	//データを作って登録
	RefcountResourceSet* rs = new RefcountResourceSet(static_cast<const char*>(data), dataSize, baseDir);
	
	m_dataDic.insert(std::make_pair(dataKey, rs));
}


void SS5ResourceCache::unregist(const std::string& dataKey)
{
	auto it = m_dataDic.find(dataKey);
	assert(it != m_dataDic.end());

	RefcountResourceSet *ref = it->second;
	ref->decCount();				//
	assert(ref->getCount() >= 0);	//マイナスにはならないはず

	if(ref->getCount() == 0){
		//このdataKeyは消してOK。なので消す
		SS_SAFE_DELETE(ref);
		m_dataDic.erase(it);
	}
}

void SS5ResourceCache::unregistAll()
{
	for(auto &str_rs : m_dataDic){
		SS_SAFE_DELETE(str_rs.second);
	}
	m_dataDic.clear();
}




void SS5ResourceCache::getTextureList(std::vector<std::string> *textureList, const std::string &dataKey) const
{
	const ResourceSet *rs = findData(dataKey);
	const CellCache *cellCache = rs->m_cellCache;

	//todo:ss5playerにも似たコードがある・・・resourcesetあたりに一覧取得機能持たせた方がいいかもしれない
	int cellMapNum = cellCache->getCellMapNum();
	textureList->resize(cellMapNum);
	for (int i = 0; i < cellMapNum; ++i){
		(*textureList)[i] = cellCache->getTexturePath(i);
	}
}



SS5Player* SS5ResourceCache::createPlayer(const std::string &dataKey, SS5Renderer *renderer, SS5EventListener *eventListener) const
{
	const ResourceSet *rs = findData(dataKey);
	return new SS5Player(rs, renderer, eventListener);
}

void SS5ResourceCache::destroyPlayer(SS5Player *&player) const
{
	delete player;
	player = nullptr;
}





std::string SS5ResourceCache::getImageBaseDir(const std::string &imageBaseDir, const ProjectData *data) const
{
	if (imageBaseDir == s_null){	// imageBaseDirの指定がないときはパスを作る
		
		if (data->imageBaseDir){
			// コンバート時に指定されたパスを使用する
			ToPointer ptr(data);
			return ptr.toString(data->imageBaseDir);
		}
		
		//// プロジェクトファイルと同じディレクトリを指定する
		//std::string directory;
		//std::string filename;
		//splitPath(directory, filename, ssbpFilepath);
		//return directory;
	}
	return imageBaseDir;
}


const ResourceSet* SS5ResourceCache::findData(const std::string& dataKey) const
{
	auto it = m_dataDic.find(dataKey);
	assert(it != m_dataDic.end());
	
	RefcountResourceSet *rrs = it->second;
	return rrs->getResourceSet();	//特にカウンタの上げ下げなどはしません
}


} //namespace ss
