#include "QuickStart.h"
#include <fstream>
#include <vector>
#include "SSBPLib/SS5ResourceCache.h"
#include "SSBPLib/SS5Player.h"
#include "EventListener.h"
#include "Renderer.h"
using namespace std;
using namespace ss;


QuickStart::QuickStart()
	: m_ss5ResourceCache(new SS5ResourceCache)
	, m_ss5Player(nullptr)
	, m_eventListener(new EventListener)
	, m_renderer(new Renderer)
{
	
}

QuickStart::~QuickStart(){

}


/*
	再生手順まとめ

	ssbpファイルを読み込み
	SS5ResourceCacheにssbpファイルを登録
	SS5ResourceCache::createPlayer()でプレーヤーを生成
	SS5Player::play()で再生
		あとは、update(), draw() を呼んでください
*/
void QuickStart::initialize()
{
	/*
	 * ssbpファイルを読み込んで登録する
	 */
	
	//ファイル読み込み
	ifstream ifs("character_template_sample1/character_template1.ssbp", ios::in | ios::binary);
	assert(ifs);
	ifs.seekg(0, ios::end);
	int fileSize = ifs.tellg();			//ファイルサイズ取得
	ifs.seekg(0, ios::beg);

	vector<char> file(fileSize, 0);
	ifs.read(file.data(), file.size());	//ファイル読み込み

	//登録
	m_ss5ResourceCache->regist(
		file.data(),					//ssbpデータ
		file.size(),					//ssbpデータサイズ
		"ssbpRegistName",				//登録名
		"character_template_sample1/"			//テクスチャのあるフォルダを指定
	);
	

	
	/*
	 * 登録名からプレーヤーを生成する
	 */

	//生成
	m_ss5Player = m_ss5ResourceCache->createPlayer(
		"ssbpRegistName",				//登録名
		m_renderer.get(),				//描画機能の指定
		m_eventListener.get()			//イベント処理の指定
	);



	/*
	 * プレーヤーの設定
	 */
	
	//再生させるにはアニメーション名を指定する
	m_ss5Player->play("character_template_3head/stance");

	//値を変えたい場合は次のようにして設定できます
	m_ss5Player->setPosition(400, 200);			//表示位置
	m_ss5Player->setScale(0.5f, 0.5f);			//スケール
	m_ss5Player->setRotation(0.0f, 0.0f, 0.0f);	//回転
	m_ss5Player->setAlpha(255);					//透明度
	m_ss5Player->setFlip(false, true);			//反転
}



void QuickStart::finalize()
{
	//プレーヤーの破棄
	m_ss5ResourceCache->destroyPlayer(m_ss5Player);

	//登録解除
	m_ss5ResourceCache->unregist("ssbpRegistName");
}



void QuickStart::update()
{
	static const int FPS = 60;
	
	m_ss5Player->update(1.0f / FPS);	//毎フレームのアップデート
}

void QuickStart::draw()
{
	m_ss5Player->draw();				//描画
}

