#pragma once

class Object;

class Component
{
protected:
public:
    Component() {}
    virtual ~Component() {}
    Object* m_parent;
    virtual void Start() {}
    virtual void Update() {}
    virtual void Draw() {}
};