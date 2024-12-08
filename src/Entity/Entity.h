#pragma once
#include "../Shader/Shader.h"
#include "../Core/Camera.h"
class Entity
{
public:
	Entity() {};
	virtual ~Entity() {}; 
	virtual void Render(const Camera& camera) = 0; // 纯虚函数继承类必须实现

};