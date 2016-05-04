#pragma once

#include <string>
#include <map>
#include <memory>
#include "SSPlayerImplement/ResourceSet.h"

namespace ss{
struct ProjectData;
class SS5Player;
class SS5Renderer;
class SS5EventListener;

	
/** addData時に、anime,cellの生成を行う。SS5Playerはここからリソースをfindして生成する */
class SS5ResourceCache{
public:
	SS5ResourceCache();
	~SS5ResourceCache();

	
	static const std::string s_null;

	/**
	 * ssbpファイルを読み込み管理対象とします。多重登録対策のために参照カウンタによる管理になります
	 *
	 * @param  data			ssbpのデータ(中でコピーされます)
	 * @param  dataSize		dataのサイズ
	 * @param  dataKey		登録名
	 * @param  imageBaseDir 画像ファイルの読み込み元ルートパス. 省略時はコンバート時に指定されたパスを使用する.(指定するのがおすすめ)
	 */
	void regist(const void *data, size_t dataSize, const std::string &dataKey, const std::string &imageBaseDir = s_null);

	
	/** 指定データを解放します。登録名を指定してください */
	void unregist(const std::string& dataKey);

	/** 全てのデータを解放します */
	void unregistAll();


	/** 指定したデータが必要とするテクスチャ名のリストを取得 */
	void getTextureList(std::vector<std::string> *textureList, const std::string &dataKey) const;
	

	/** SS5Playerの生成 */
	SS5Player* createPlayer(const std::string &dataKey, SS5Renderer *renderer, SS5EventListener *eventListener) const;
	void destroyPlayer(SS5Player *&player) const;
	

private:
	//imageBaseDirの指定がないときはdataの中を見てディレクトリを返す
	std::string getImageBaseDir(const std::string &imageBaseDir, const ProjectData *data) const;
	
	//名前に対応するデータを取得
	const ResourceSet* findData(const std::string& dataKey) const;


	
	/** regist数をカウントするための構造 */
	class RefcountResourceSet{
	public:
		RefcountResourceSet(const char *data, size_t dataSize, const std::string &imageBaseDir)
		: m_refCount(0), m_resourceSet(new ResourceSet(data, dataSize, imageBaseDir)){
			incCount();
		}
		~RefcountResourceSet(){}
		
		int getCount() const{ return m_refCount; }
		void incCount(){ ++m_refCount; }
		void decCount(){ --m_refCount; }

		ResourceSet* getResourceSet() const{
			return m_resourceSet.get();
		}

	private:
		int m_refCount;
		std::unique_ptr<ResourceSet> m_resourceSet;
	};
	
	std::map<std::string, RefcountResourceSet*>	m_dataDic;	//ここにデータを登録する


private: //non copyable
	SS5ResourceCache(const SS5ResourceCache &o) = delete;
	SS5ResourceCache& operator=(const SS5ResourceCache &o) = delete;
};


} //namespace ss
