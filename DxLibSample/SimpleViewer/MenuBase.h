#pragma once

#include <iostream>



template <class Owner>
class MenuBase{
public:
	MenuBase(Owner *owner) : m_owner(owner){}
	virtual ~MenuBase(){}
	
	virtual void action(int up, int down, int left, int right, int enter, int cancel) = 0;
	virtual void draw(std::ostream &ost) = 0;
protected:
	Owner *m_owner;	
};

