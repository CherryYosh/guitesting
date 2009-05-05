/* 
 * File:   base.h
 * Author: brandon
 *
 * Created on May 4, 2009, 6:04 PM
 */

#ifndef _BASE_H
#define	_BASE_H

class Camera;

class Base{
public:
	virtual void CreateContext(){};
	virtual void SetupContext(){};

	virtual void BeginRender(){};
	virtual void EndRender(){};

	virtual void SetCamera(Camera*){};
	virtual Camera* GetCamera(){return 0;};

	virtual void SetViewport(int,int,int,int){};
	virtual int* GetViewport(){return 0;};
private:
};

#endif	/* _BASE_H */

