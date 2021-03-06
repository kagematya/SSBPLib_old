#pragma once

#include <memory>

namespace ss{
	class SS5Player;
	class SS5ResourceCache;
}

class Renderer;
class EventListener;

class QuickStart{
public:
	QuickStart();
	~QuickStart();

	void initialize();
	void finalize();

	void update();
	void draw();

private:
	
	std::unique_ptr<ss::SS5ResourceCache> m_ss5ResourceCache;	//ssbpファイルの登録先
	
	ss::SS5Player *m_ss5Player;							//ssbpファイルの再生制御
	std::unique_ptr<EventListener> m_eventListener;		//SS5Player向けのイベント処理の実装
	std::unique_ptr<Renderer> m_renderer;				//SS5Player向けの描画処理の実装
};

