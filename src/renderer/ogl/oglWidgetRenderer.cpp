/* 
* File:   oglWidgetRender.cpp
* Author: brandon
* 
* Created on May 3, 2009, 4:05 PM
*/

#include <GL/glew.h> //needed on windows, Dont delete

#include "oglBase.h"
#include "oglWidgetRenderer.h"

#include "../../theme.h"
#include "../../gui/window.h"


#include <GL/gl.h>
#include <GL/glext.h>

#include <cassert>
#include <stdexcept>

/**
* The data structor to use with the VBO
* Spacers are there to make extendening the data
* require little modification to the program, and
* also allow for 32bit alligment prefered by GPU's
*/
struct WidgetData {
	float x, y, z; //position
	float spacer1;
	float s, t; //texture
	float spacer2[2];
	float r, g, b, a; //color
	float spacer3[4];
};

oglWidgetRenderer::oglWidgetRenderer() : TotalObjects(0), Buffer() {
	base = new oglBase();

	shader = new Shader("guiAnimation");

	shader->GetUniformLoc(0, "projection");
	shader->GetUniformLoc(1, "modelview");
	shader->GetUniformLoc(2, "tex0");
	shader->GetAttributeLoc(0, "vertex");
	shader->GetAttributeLoc(1, "tcoord");
	shader->GetAttributeLoc(2, "tcolor");
}

oglWidgetRenderer::~oglWidgetRenderer() { }

/**
* Adds a object to the list
* @param obj a pointer to the new object, cannot be null.
*/
void oglWidgetRenderer::AddObject(void* obj) {
	if (obj == NULL) throw std::invalid_argument("oglWidgetRenderer::AddObject::obj_is_null");

	TotalObjects += static_cast<Widget*> (obj)->Size();
	Objects.push_back(static_cast<Widget*> (obj));
}

/**
* Removes the object from the list
* @param obj the object to be removed
* @BUG does not remove the data from the vbo..
*/
void oglWidgetRenderer::RemoveObject(void* obj) {
	assert(obj != NULL);

	TotalObjects -= static_cast<Widget*> (obj)->Size();
	std::vector<Widget*>::iterator it = Objects.begin();
	while (it != Objects.end()) {
		if (*it == obj) {
			it = Objects.erase(it);
		} else {
			it++;
		}
	}

	Refresh();
}

/**
* Binds and sets up all, known, data for rendering
*/
void oglWidgetRenderer::Begin() {
	Camera* c = base->GetCamera();

	shader->Bind();
	shader->SetProjection(c->GetOrthofv());
	shader->SetModelview(c->GetModelviewfv());

	//We bind the theme image
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Theme::GetImageID());

	Buffer.Bind();

	glEnableVertexAttribArray(shader->attribute[0]);
	glEnableVertexAttribArray(shader->attribute[1]);
	glEnableVertexAttribArray(shader->attribute[2]);

	glVertexAttribPointer(shader->attribute[0], 3, GL_FLOAT, GL_FALSE, sizeof(WidgetData), 0);
	glVertexAttribPointer(shader->attribute[1], 2, GL_FLOAT, GL_FALSE, sizeof(WidgetData), (GLvoid*) (4 * sizeof(float)));
	glVertexAttribPointer(shader->attribute[2], 4, GL_FLOAT, GL_FALSE, sizeof(WidgetData), (GLvoid*) (8 * sizeof(float)));
}

/**
* Renders all objects
*/
void oglWidgetRenderer::Render() {
	if (TotalObjects > 0)
		glDrawArrays(GL_QUADS, 0, TotalObjects * 4);
}

/**
* Unbinds all, known, data
*/
void oglWidgetRenderer::End() {
	glDisableVertexAttribArray(shader->attribute[0]);
	glDisableVertexAttribArray(shader->attribute[1]);
	glDisableVertexAttribArray(shader->attribute[2]);

	Buffer.Unbind();
	shader->Unbind();
}

/**
* Does a full draw cycle, same as calling Begin(), Render(), End()
*/
void oglWidgetRenderer::Draw() {
	Begin();
	Render();
	End();
}

/**
* 'Refreshes' the buffer. Clearning it before readding all data.
*/
void oglWidgetRenderer::Refresh() {
	Buffer.Bind();
	Buffer.RemoveData(0, Buffer.GetSize());
	Buffer.Unbind();

	TotalObjects = 0;

	size_t size = Objects.size();
	for (unsigned int i = 0; i < size; i++) {
		TotalObjects += Objects[i]->Size();
		Update(Objects[i], RENDERER_ADD);
	}
}

/**
* Updates a object in the Buffer
* @param obj the pointer of the object that needs updating
* @param update a enum for the type of update to use.
*/
void oglWidgetRenderer::Update(void* obj, unsigned int update) {
	Widget* object = static_cast<Widget*> (obj);
	int i = 0, j = 0;
	unsigned int slot = 0;
	Widget *root, *node;
	size_t size1;

	//these are for cleaner code
	//also SHOULD allow for easyer SSE complation
	util::vec4<float> v1, v2, v3, v4;
	float vs, vs2, vt, vt2;
	float* c;

	unsigned int dataSize = object->Size() * 4;
	WidgetData* data = new WidgetData[ dataSize ];

	//contrainers only hold objects, they do not render
	//will need to change this if I allow window in window or
	//add another container type
	if(object->attributes(GUI_CONTAINER)){
		root = node = object->GetChild(0);
		i = 1;
	} else {
		root = node = object;
	}

	size1 = object->ChildrenSize();
	for (; i <= size1; i++) {
		do {
			v1 = util::vec4<float>(node->GetX(), node->GetY(), node->GetZ() + node->GetLayer());
			v2 = util::vec4<float>(node->GetX() + node->GetWidth(), node->GetY(), node->GetZ() + node->GetLayer());
			v3 = util::vec4<float>(node->GetX() + node->GetWidth(), node->GetY() + node->GetHeight(), node->GetZ() + node->GetLayer());
			v4 = util::vec4<float>(node->GetX(), node->GetY() + node->GetHeight(), node->GetZ() + node->GetLayer());

			//Will apply the roots rotation to the vbo so we dont need to pass
			//the matrix to the gpu every call
			if (node->GetRoot() != NULL) {
				v1 = (*node->GetRotation()) * v1;
				v2 = (*node->GetRotation()) * v2;
				v3 = (*node->GetRotation()) * v3;
				v4 = (*node->GetRotation()) * v4;
			}

			vs = node->s;
			vs2 = node->s2;
			vt = node->t;
			vt2 = node->t2;

			//get the color
			c = node->GetColorv();

			//top left
			data[slot].x = v1.x;
			data[slot].y = v1.y;
			data[slot].z = v1.z;
			data[slot].s = vs;
			data[slot].t = vt;
			data[slot].r = c[0];
			data[slot].g = c[1];
			data[slot].b = c[2];
			data[slot].a = c[3];
			slot++;

			//top right
			data[slot].x = v2.x;
			data[slot].y = v2.y;
			data[slot].z = v2.z;
			data[slot].s = vs2;
			data[slot].t = vt;
			data[slot].r = c[0];
			data[slot].g = c[1];
			data[slot].b = c[2];
			data[slot].a = c[3];
			slot++;

			//bottom right
			data[slot].x = v3.x;
			data[slot].y = v3.y;
			data[slot].z = v3.z;
			data[slot].s = vs2;
			data[slot].t = vt2;
			data[slot].r = c[0];
			data[slot].g = c[1];
			data[slot].b = c[2];
			data[slot].a = c[3];
			slot++;

			//bottom left
			data[slot].x = v4.x;
			data[slot].y = v4.y;
			data[slot].z = v4.z;
			data[slot].s = vs;
			data[slot].t = vt2;
			data[slot].r = c[0];
			data[slot].g = c[1];
			data[slot].b = c[2];
			data[slot].a = c[3];
			slot++;

			j++;
			node = root->IterateChildren(j);
		} while (node != NULL);

		j = 0;
		root = node = object->GetChild(i);
	}

	Buffer.Bind();

	if (update == RENDERER_ADD) {
		Buffer.AddData(dataSize * sizeof(WidgetData), data);
	} else if (update == RENDERER_REFRESH) {
		int pos = 0, r = -1;

		size_t size = Objects.size();
		for (unsigned int j = 0; j < size; j++) {
			r = Objects[j]->IterateChildren(object);

			if (r == -1) {
				pos += Objects[j]->Size();
			} else {
				pos += r;
				break;
			}
		}

		unsigned int start = pos * 4 * sizeof(WidgetData);
		Buffer.SetData(start, dataSize * sizeof(WidgetData), data);
	}

	Buffer.Unbind();
	delete [] data;
}

Shader* oglWidgetRenderer::GetShader() {
	return shader;
}

void oglWidgetRenderer::SetShader(Shader* s) {
	if (s == NULL)
		return;

	shader = s;
}

int* oglWidgetRenderer::GetViewport() {
	return base->GetViewport();
}

Camera* oglWidgetRenderer::GetCamera() {
	return base->GetCamera();
}
