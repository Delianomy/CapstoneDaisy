#pragma once
#include "Actor.h"
#include <string>
class PickableItem : public Actor{
public:
	PickableItem(Component* parent_, std::string name_) : Actor(parent_){
		name = name_; 
	}
		std::string name;
};

