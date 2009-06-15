/* 
 * File:   oglRenderer.h
 * Author: brandon
 *
 * Created on May 4, 2009, 12:06 AM
 */

#ifndef _OGLRENDERER_H
#define	_OGLRENDERER_H

#include "../base.h"

class oglBase : public Base {
public:
	oglBase();
	virtual ~oglBase();

	void CreateContext();
	void SetupContext();

	void BeginRender();
	void EndRender();
	
	void SetCamera(Camera*);
	Camera* GetCamera();

	void SetViewport(int,int,int,int);
	int* GetViewport();
private:
        static Camera* camera;
};

#endif	/* _OGLRENDERER_H */

