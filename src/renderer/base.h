/* 
 * File:   base.h
 * Author: brandon
 *
 * Created on May 4, 2009, 6:04 PM
 */

#ifndef _BASE_H
#define	_BASE_H

class Camera;

class Base {
public:
	virtual void CreateContext() = 0;
	virtual void SetupContext() = 0;

	virtual void BeginRender() = 0;
	virtual void EndRender() = 0;

	virtual void SetCamera(Camera*) = 0;
	virtual Camera* GetCamera() = 0;

	virtual void SetViewport(int, int, int, int) = 0;
	virtual int* GetViewport() = 0;
private:
};

#endif	/* _BASE_H */

