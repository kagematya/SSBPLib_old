#pragma once

#include <memory>
#include <vector>
#include <string>
#include <array>

namespace ss{
	class SS5Player;
	class SS5ResourceCache;
}

class Renderer;
class EventListener;

template <class Owner> class MenuBase;

class SimpleViewer{
public:
	SimpleViewer();
	~SimpleViewer();

	void initialize();
	void finalize();

	void update();
	void draw();

	ss::SS5Player* SSP(){
		return m_ss5Player;
	}
	const std::vector<std::string>& getAnimList() const{
		return m_animlist;
	}

private:
	
	std::unique_ptr<ss::SS5ResourceCache> m_ss5ResourceCache;	//ssbpファイルの登録先
	
	ss::SS5Player *m_ss5Player;							//ssbpファイルの再生制御
	std::unique_ptr<EventListener> m_eventListener;		//SS5Player向けのイベント処理の実装
	std::unique_ptr<Renderer> m_renderer;				//SS5Player向けの描画処理の実装

	std::unique_ptr< MenuBase<SimpleViewer> > m_menu;	//メニュー
	std::vector<std::string> m_animlist;				//アニメーションの一覧

	std::array<int, 256> m_keyInputs;
	bool m_isPlaying;
};

