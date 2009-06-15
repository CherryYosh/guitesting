/* 
 * File:   renderer.h
 * Author: brandon
 *
 * Info: A empty class used to allow for a ambigious rendering path
 *
 * Created on May 3, 2009, 3:21 PM
 */

#ifndef RENDERER_H
#define	RENDERER_H

#include "base.h"

enum {
    RENDERER_ADD, RENDERER_REMOVE, RENDERER_REFRESH
};

class Shader;
class Camera;

class Renderer {
public:
    virtual ~Renderer() { base = NULL; };

    virtual void AddObject(void*) = 0;
    virtual void RemoveObject(void*) = 0;

    virtual void Begin() = 0;
    virtual void Render() = 0;
    virtual void End() = 0;
    virtual void Draw() = 0;

    virtual void Refresh() = 0;
    virtual void Update(void*, unsigned int) = 0;

    virtual Shader* GetShader() = 0;
    virtual void SetShader(Shader*) = 0;

    virtual int* GetViewport() = 0;
    virtual Camera* GetCamera() = 0;
protected:
    Base* base;
};

#endif	/* _RENDERER_H */

