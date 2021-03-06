#include "SimpleViewer.h"
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <string>
#include <algorithm>
#include "SSBPLib/SS5ResourceCache.h"
#include "SSBPLib/SS5Player.h"
#include "EventListener.h"
#include "Renderer.h"
#include "SimpleViewerMenu.h"
using namespace std;
using namespace ss;


SimpleViewer::SimpleViewer()
	: m_ss5ResourceCache(new SS5ResourceCache)
	, m_ss5Player(nullptr)
	, m_eventListener(new EventListener)
	, m_renderer(new Renderer)
	, m_menu(new SimpleViewerMenu::MenuRoot(this))
	, m_isPlaying(false)
{
	fill(m_keyInputs.begin(), m_keyInputs.end(), 0);
}

SimpleViewer::~SimpleViewer(){

}


/*
	再生手順まとめ

	ssbpファイルを読み込み
	SS5ResourceCacheにssbpファイルを登録
	SS5ResourceCache::createPlayer()でプレーヤーを生成
	SS5Player::play()で再生
		あとは、update(), draw() を呼んでください
*/
void SimpleViewer::initialize()
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
		"character_template_sample1/"	//テクスチャのあるフォルダを指定
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


	//アニメーションのリストを作る
	list<string> animlist;
	m_ss5Player->getAnimationList(&animlist);
	m_animlist.resize(animlist.size());
	copy(animlist.begin(), animlist.end(), m_animlist.begin());

	m_isPlaying = true;
}



void SimpleViewer::finalize()
{
	//プレーヤーの破棄
	m_ss5ResourceCache->destroyPlayer(m_ss5Player);

	//登録解除
	m_ss5ResourceCache->unregist("ssbpRegistName");
}



void SimpleViewer::update()
{
	char buf[256];
	GetHitKeyStateAll(buf);
	for (int i = 0; i < 256; ++i){
		buf[i] ? m_keyInputs[i]++ : m_keyInputs[i] = 0;		//押し続けたフレーム数になる
	}

	auto inputEdit = [](int &input){
		return static_cast<int>(input == 1 || input>10);	//押した瞬間と長押しだけに反応させたい
	};
	
	int up		= inputEdit(m_keyInputs[KEY_INPUT_UP]);
	int down	= inputEdit(m_keyInputs[KEY_INPUT_DOWN]);
	int left	= inputEdit(m_keyInputs[KEY_INPUT_LEFT]);
	int right	= inputEdit(m_keyInputs[KEY_INPUT_RIGHT]);
	int enter	= inputEdit(m_keyInputs[KEY_INPUT_Z]);
	int cancel	= inputEdit(m_keyInputs[KEY_INPUT_X]);

	if (m_keyInputs[KEY_INPUT_C] == 1){
		m_isPlaying = !m_isPlaying;		//ポーズ切り替え
	}

	m_menu->action(up, down, left, right, enter, cancel);


	static const int FPS = 60;
	float deltaTime = (m_isPlaying ? (1.0f / FPS) : 0);		//停止中は時間を進ませない
	m_ss5Player->update(deltaTime);		//毎フレームのアップデート
}

void SimpleViewer::draw()
{
	m_ss5Player->draw();				//描画

	stringstream stream;
	stream << "上下左右でメニュー操作。zで決定。xでキャンセル。cで再生/停止" << endl;
	m_menu->draw(stream);

	string str;
	int y = 10;
	while (getline(stream, str)){
		DrawString(10, y, str.c_str(), GetColor(255, 255, 255));
		y += 16;
	}

	
}

