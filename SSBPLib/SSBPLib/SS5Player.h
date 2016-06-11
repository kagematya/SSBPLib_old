#pragma once

#include "SS5PlayerTypes.h"
#include "SSPlayerStatus.h"
#include "SS5UserData.h"
#include <map>
#include <vector>
#include <list>
#include <string>
#include <assert.h>



namespace ss{
class CustomSprite;
struct AnimeRef;
class ResourceSet;
struct PartState;
class SS5Renderer;
class SS5EventListener;


/**
 * SS5Player
 */
class SS5Player{
private:
	static const int PART_VISIBLE_MAX = 512;		/// １アニメの最大パーツ数

private:
	SS5Renderer *m_renderer;
	SS5EventListener *m_eventListener;

	void initialize();
public:

	/** SS5Playerインスタンスを構築します. ssbpファイル名(拡張子不要)を指定し、リソースの割り当てを行う */
	SS5Player(const ResourceSet *resource, SS5Renderer *renderer, SS5EventListener *eventListener);
	SS5Player(const ResourceSet *resource, SS5Renderer *renderer, SS5EventListener *eventListener, const std::string& animeName);
	~SS5Player();	//memo:なるべくSS5ResourceCache.create, destroyを使ってほしい


	/**
	 * アニメーションの再生を開始します.
	 *
	 * @param  packName      パック名(ssae）
	 * @param  animeName     再生するアニメーション名
	 * @param  startFrameNo  再生を開始するフレームNoの指定. 省略時は0
	 */
	void play(const std::string& packName, const std::string& animeName, int startFrameNo = 0);

	/**
	 * アニメーションの再生を開始します.
	 * アニメーション名から再生するデータを選択します.
	 * "ssae名/アニメーション名" という指定が可能です.
	 * sample.ssaeのanime_1を指定する場合、sample/anime_1となります.
	 * ※アニメーション名のみで指定した場合、同名のアニメーションが複数存在時にどのアニメーションが選択されるかは不定です.
	 *
	 * @param  animeName     再生するアニメーション名
	 * @param  startFrameNo  再生を開始するフレームNoの指定. 省略時は0
	 */
	void play(const std::string& animeName, int startFrameNo = 0);



	/** 再生しているアニメーションのパック名(ssae)を返します */
	const std::string& getPlayPackName() const;

	/** 再生しているアニメーション名を返します */
	const std::string& getPlayAnimeName() const;
	
	
	/** 再生フレーム */
	int getMaxFrame() const;			//アニメーションの総フレームを取得
	void setCurrentFrame(int frame);	//再生フレームの設定
	int getCurrentFrame() const;		//再生フレームを取得

	
	/** パーツへのアクセサ */
	int getLabelToFrame(char* findLabelName);		//ラベル名からフレーム位置を取得
	int getPartNum() const;							//パーツ数を取得
	const char* getPartName(int partId) const;		//indexからパーツ名を取得
	int indexOfPart(const char* partName) const;	//パーツ名からindexを取得
	bool getPartState(PartState& result, const char* name) const;	//パーツ名から、パーツ情報を取得

	/** パーツの表示、非表示を設定します */
	void setPartVisible( int partId, bool flg );

	/** cellの入れ替えをします */
	int indexOfCell(const char* cellName) const;	//cell名からcellIndexを取得
	void changePartCell(int partId, int cellIndex);	//パーツIDを指定し、セルを入れ替える(cellIndex==-1ならデフォルト)

	/** プレイヤー本体の位置を設定します */
	void setPosition(float x, float y);
	void getPosition(float *x, float *y) const;

	/** プレイヤー本体の回転角度を設定します。2Dの回転はZに値を設定してください */
	void setRotation(float x, float y, float z);
	void getRotation(float *x, float *y, float *z) const;
	
	/*プレイヤー本体のスケールを設定します */
	void setScale(float x, float y);
	void getScale(float *x, float *y) const;

	/* プレイヤー本体の透明度を設定します */
	void setAlpha(int alpha);
	int getAlpha() const;

	/**
	 * 名前を指定してパーツの再生するインスタンスアニメを変更します。
	 * 指定したパーツがインスタンスパーツでない場合、falseを返します.
	 * 再生するアニメの名前は"ssae名/アニメーション名"として再生してください。
	 * 現在再生しているアニメを指定することは入れ子となり無限ループとなるためできません。
	 */
	bool changeInstanceAnime(std::string partsname, std::string animeName);

	/* プレイヤー本体の反転を設定します */
	void setFlip(bool flipX, bool flipY);
	void getFlip(bool *flipX, bool *flipY) const;


	/* プレイヤーの更新を行います */
	void update(float dt);

	/* プレイヤーの表示を行います */
	void draw();


	/** 再生可能なアニメーションのリストを返す */
	void getAnimationList(std::list<std::string> *animlist) const;


private:
	void allocParts(int numParts);
	void releaseParts();
	void setPartsParentage();

	void play(const AnimeRef* animeRef, int startFrameNo);
	void updateFrame(float dt);
	void setFrame(int frameNo);			//引数はframe
	void checkUserData(int frameNo);	//ユーザーデータがあれば実行する
	void checkEvent(int frameNo);		//イベントがあれば実行する

	int getAnimeFPS() const;

private:
	const ResourceSet	*m_currentRs;
	const AnimeRef		*m_currentAnimeRef;
	std::vector<CustomSprite> m_parts;
	std::vector<TextureID> m_textures;		//_textures[cellMapIndex] = textureid;

	double				m_nowTime;
	
	
	bool				m_partVisible[PART_VISIBLE_MAX];
	int					m_insteadCellIndex[PART_VISIBLE_MAX];	//cellの差し替わり番号 未使用時は-1

	State				m_state;
	int					m_priorityPartIndex[PART_VISIBLE_MAX];	//描画優先順でpartIndexを保存しておく  priority: [0] > [max]


private: //non copyable
	SS5Player(const SS5Player &o) = delete;
	SS5Player& operator=(const SS5Player &o) = delete;
};


};	// namespace ss

