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

enum {
	RENDERER_ADD, RENDERER_REMOVE, RENDERER_REFRESH
};

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

private:

};

#endif	/* _RENDERER_H */

