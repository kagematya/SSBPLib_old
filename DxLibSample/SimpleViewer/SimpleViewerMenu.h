#pragma once

#include <memory>
#include <iostream>
#include <vector>
#include <list>
#include <string>
#include "SSBPLib/SS5Player.h"
#include "MenuBase.h"
#include "SimpleViewer.h"

namespace SimpleViewerMenu{

using Base = MenuBase<SimpleViewer>;

//メニューのセレクタ
class MenuRoot : public Base{
public:
	MenuRoot(SimpleViewer *owner);
	~MenuRoot();

	void action(int up, int down, int left, int right, int enter, int cancel) override;
	void draw(std::ostream &oss) override;

private:
	std::vector<Base *> m_childMenu;
	int m_activeIndex;
};


//--
struct AnimeChanger : public Base{
	AnimeChanger(SimpleViewer *owner) : Base(owner), m_select(0){}
	void action(int up, int down, int left, int right, int enter, int cancel) override;
	void draw(std::ostream &oss) override;
	
	std::string m_animeName;
	int m_select;
};

struct FrameChanger : public Base{
	FrameChanger(SimpleViewer *owner) : Base(owner), m_frame(0){}
	void action(int up, int down, int left, int right, int enter, int cancel) override;
	void draw(std::ostream &oss) override;

	int m_frame;
};

struct PositionChanger : public Base{
	enum class XYZ{ X, Y/*, Z*/ };
	PositionChanger(SimpleViewer *owner, XYZ xyz) : Base(owner), m_xyz(xyz), m_position(300){}
	void action(int up, int down, int left, int right, int enter, int cancel) override;
	void draw(std::ostream &oss) override;

	XYZ m_xyz;
	int m_position;
};

struct RotationChanger : public Base{
	enum class XYZ{ X, Y, Z };
	RotationChanger(SimpleViewer *owner, XYZ xyz) : Base(owner), m_xyz(xyz), m_rotation(0){}
	void action(int up, int down, int left, int right, int enter, int cancel) override;
	void draw(std::ostream &oss) override;

	XYZ m_xyz;
	int m_rotation;
};

struct ScaleChanger : public Base{
	enum class XYZ{ X, Y/*, Z*/ };
	ScaleChanger(SimpleViewer *owner, XYZ xyz) : Base(owner), m_xyz(xyz), m_scale(1){}
	void action(int up, int down, int left, int right, int enter, int cancel) override;
	void draw(std::ostream &oss) override;

	XYZ m_xyz;
	double m_scale;
};

struct AlphaChanger : public Base{
	AlphaChanger(SimpleViewer *owner) : Base(owner), m_alpha(255){}
	void action(int up, int down, int left, int right, int enter, int cancel) override;
	void draw(std::ostream &oss) override;

	int m_alpha;
};

struct FlipChanger : public Base{
	FlipChanger(SimpleViewer *owner) : Base(owner), m_flipX(false), m_flipY(false){}
	void action(int up, int down, int left, int right, int enter, int cancel) override;
	void draw(std::ostream &oss) override;

	bool m_flipX;
	bool m_flipY;
};



} //namespace SimpleViewerMenu
