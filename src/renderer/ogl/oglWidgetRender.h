/* 
 * File:   oglWidgetRender.h
 * Author: brandon
 *
 * Created on May 3, 2009, 4:05 PM
 */

#ifndef _OGLWIDGETRENDER_H
#define	_OGLWIDGETRENDER_H

#include "vbo.h"
#include "shader.h"
#include "../renderer.h"
#include "../../camera.h"
#include "../../nvMatrix.h"
#include "../../nvVector.h"
#include "../../gui/control.h"

#include <vector>

class oglWidgetRender : public Renderer {
public:
	oglWidgetRender();
	virtual ~oglWidgetRender();
	
	void AddObject(void*);
	void RemoveObject(void*);

	void Begin();
	void Render();
	void End();
	void Draw();

	void Refresh();
	void Update(void*, unsigned int);

//	Shader* GetShader();
//	void SetShader(Shader*);

	void SetCamera( Camera* );

private:
	Camera* camera;
	std::vector<Control*> Objects;
	VBO Buffer;
	Shader* shader;
};

#endif	/* _OGLWIDGETRENDER_H */

