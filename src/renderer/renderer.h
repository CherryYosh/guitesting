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
	Renderer(){};
	virtual ~Renderer(){};

	virtual void AddObject(void*){};
	virtual void RemoveObject(void*){};

	virtual void Begin(){};
	virtual void Render(){};
	virtual void End(){};
	virtual void Draw(){};

	virtual void Refresh(){};
	virtual void Update(void*, unsigned int){};

	virtual const Shader* GetShader(){return 0;};
	virtual void SetShader(Shader*){};

	virtual int* GetViewport(){return 0;};
	virtual const Camera* GetCamera(){return 0;};
protected:
	Base* base;
};

#endif	/* _RENDERER_H */

