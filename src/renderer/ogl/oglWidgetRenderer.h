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
#include "../../utils/matrix.h"
#include "../../utils/vector.h"
#include "../../gui/control.h"

#include <vector>

class oglWidgetRenderer : public Renderer {
public:
	oglWidgetRenderer();
	virtual ~oglWidgetRenderer();
	
	void AddObject(void*);
	void RemoveObject(void*);

	void Begin();
	void Render();
	void End();
	void Draw();

	void Refresh();
	void Update(void*, unsigned int = RENDERER_REFRESH);

	Shader* GetShader();
	void SetShader(Shader*);

	int* GetViewport();

	Camera* GetCamera();
private:
	unsigned int TotalObjects;	//the total number of objects, including all children
	std::vector<Control*> Objects;
	VBO Buffer;
	Shader* shader;
};

#endif	/* _OGLWIDGETRENDER_H */

