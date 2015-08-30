#include "SimpleViewerMenu.h"
#include <sstream>
#include <algorithm>
#include "SSBPLib/SS5Player.h"
#include "SimpleViewer.h"
using namespace std;

namespace SimpleViewerMenu{

static int wrap(int val, int minVal, int maxVal){
	assert(minVal < maxVal);
	int n = (val - minVal) % (maxVal - minVal);
	return (n >= 0) ? (n + minVal) : (n + maxVal);
}



MenuRoot::MenuRoot(SimpleViewer *owner) : Base(owner)
	, m_activeIndex(0)
{
	m_childMenu.push_back(new AnimeChanger(owner));
	m_childMenu.push_back(new FrameChanger(owner));
	m_childMenu.push_back(new PositionChanger(owner, PositionChanger::XYZ::X));
	m_childMenu.push_back(new PositionChanger(owner, PositionChanger::XYZ::Y));
	m_childMenu.push_back(new RotationChanger(owner, RotationChanger::XYZ::X));
	m_childMenu.push_back(new RotationChanger(owner, RotationChanger::XYZ::Y));
	m_childMenu.push_back(new RotationChanger(owner, RotationChanger::XYZ::Z));
	m_childMenu.push_back(new ScaleChanger(owner, ScaleChanger::XYZ::X));
	m_childMenu.push_back(new ScaleChanger(owner, ScaleChanger::XYZ::Y));
	m_childMenu.push_back(new AlphaChanger(owner));
	m_childMenu.push_back(new FlipChanger(owner));
}

MenuRoot::~MenuRoot()
{
	for(MenuBase *child : m_childMenu){
		delete(child);
	}
	m_childMenu.clear();
}

void MenuRoot::action(int up, int down, int left, int right, int enter, int cancel)
{
	//メニュー項目の選択
	if(down){
		m_activeIndex++;
	}
	else if(up){
		m_activeIndex--;
	}
	m_activeIndex = wrap(m_activeIndex, 0, m_childMenu.size());
	
	MenuBase *current = m_childMenu[m_activeIndex];
	current->action(up, down, left, right, enter, cancel);
}

void MenuRoot::draw(std::ostream &oss)
{
	for(int i = 0; i < m_childMenu.size(); ++i){
		if(i == m_activeIndex){
			oss << "> ";
		}
		else{
			oss << "  ";
		}
		
		m_childMenu[i]->draw(oss);
	}
}


//--
void AnimeChanger::action(int up, int down, int left, int right, int enter, int cancel){
	//再生するモーションを設定
	auto &animlist = m_owner->getAnimList();
	int animNum = animlist.size();
	
	if(left){  m_select--; }
	if(right){ m_select++; }
	m_select = wrap(m_select, 0, animNum);
	m_animeName = animlist[m_select];

	if(enter){
		m_owner->SSP()->play(m_animeName);
	}
}
void AnimeChanger::draw(std::ostream &oss){
	auto ssp = m_owner->SSP();
	oss << "animation: current=" << ssp->getPlayAnimeName() << " --> " << m_animeName << endl;
}
//--
void FrameChanger::action(int up, int down, int left, int right, int enter, int cancel){
	auto *ssp = m_owner->SSP();
	int animax = ssp->getMaxFrame();

	if(left){  m_frame--; }
	if(right){ m_frame++; }
	m_frame = wrap(m_frame, 0, animax);
	
	if(enter){
		ssp->setCurrentFrame(m_frame);
	}
}
void FrameChanger::draw(std::ostream &oss){
	auto ssp = m_owner->SSP();
	oss << "frame: current=" << ssp->getCurrentFrame() << " --> " << m_frame << endl;
}
//--
void PositionChanger::action(int up, int down, int left, int right, int enter, int cancel){
	auto ssp = m_owner->SSP();
	
	if(left){  m_position--; }
	if(right){ m_position++; }
	if(cancel){m_position = 400;}

	if(enter){
		float x, y;
		ssp->getPosition(&x, &y);

		switch(m_xyz){
		case XYZ::X:	x = m_position;	break;
		case XYZ::Y:	y = m_position;	break;
		}
		ssp->setPosition(x, y);
	}
}
void PositionChanger::draw(std::ostream &oss){
	auto ssp = m_owner->SSP();
	float x, y;
	ssp->getPosition(&x, &y);
	switch(m_xyz){
	case XYZ::X:
		oss << "position X: current=" << x << " --> " << m_position << endl;
		break;
	case XYZ::Y:
		oss << "position Y: current=" << y << " --> " << m_position << endl;
		break;
	}
}
//--
void RotationChanger::action(int up, int down, int left, int right, int enter, int cancel){
	auto ssp = m_owner->SSP();
	
	if(left){  m_rotation--; }
	if(right){ m_rotation++; }
	if(cancel){m_rotation = 0;}

	if(enter){
		float x,y,z;
		ssp->getRotation(&x, &y, &z);

		switch(m_xyz){
		case XYZ::X:	x = m_rotation; break;
		case XYZ::Y:	y = m_rotation; break;
		case XYZ::Z:	z = m_rotation; break;
		}
		ssp->setRotation(x, y, z);
	}
}
void RotationChanger::draw(std::ostream &oss){
	auto ssp = m_owner->SSP();
	float x,y,z;
	ssp->getRotation(&x, &y, &z);
	
	std::string text;
	float current;
	switch(m_xyz){
	case XYZ::X:	text = "rotation X"; current=x; break;
	case XYZ::Y:	text = "rotation Y"; current=y; break;
	case XYZ::Z:	text = "rotation Z"; current=z; break;
	}
	oss << text << ": current=" << current << " --> " << m_rotation << endl;
}
//--
void ScaleChanger::action(int up, int down, int left, int right, int enter, int cancel){
	auto ssp = m_owner->SSP();
	
	if(left){  m_scale -= 0.1; }
	if(right){ m_scale += 0.1; }
	if(cancel){m_scale = 0.5;}

	if(enter){
		float x, y;
		ssp->getScale(&x, &y);

		switch(m_xyz){
		case XYZ::X:	x = m_scale; break;
		case XYZ::Y:	y = m_scale; break;
		}
		ssp->setScale(x, y);
	}
}
void ScaleChanger::draw(std::ostream &oss){
	auto ssp = m_owner->SSP();
	float x, y;
	ssp->getScale(&x, &y);
	
	std::string text;
	float current;
	switch(m_xyz){
	case XYZ::X:	text = "scale X"; current=x; break;
	case XYZ::Y:	text = "scale Y"; current=y; break;
	}
	oss << text << ": current=" << current << " --> " << m_scale << endl;
}
//--
void AlphaChanger::action(int up, int down, int left, int right, int enter, int cancel){
	auto *ssp = m_owner->SSP();

	if(left){  m_alpha--; }
	if(right){ m_alpha++; }
	if(cancel){m_alpha = 255; }
	m_alpha = wrap(m_alpha, 0, 256);
	
	if(enter){
		ssp->setAlpha(m_alpha);
	}
}
void AlphaChanger::draw(std::ostream &oss){
	auto ssp = m_owner->SSP();
	oss << "alpha: current=" << ssp->getAlpha() << " --> " << m_alpha << endl;
}
//--
void FlipChanger::action(int up, int down, int left, int right, int enter, int cancel){
	auto *ssp = m_owner->SSP();

	if(left){  m_flipX = !m_flipX; }
	if(right){ m_flipY = !m_flipY; }
	if(cancel){
		m_flipX = false;
		m_flipY = false;
	}

	if(enter){
		ssp->setFlip(m_flipX, m_flipY);
	}
}
void FlipChanger::draw(std::ostream &oss){
	auto ssp = m_owner->SSP();
	bool x,y;
	ssp->getFlip(&x, &y);
	
	oss << "flip: current=(" << x << "," << y << ") --> (" << m_flipX << "," << m_flipY << ")" << endl;
}


} //namespace SimpleViewerMenu
