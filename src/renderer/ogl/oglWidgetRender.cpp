/* 
 * File:   oglWidgetRender.cpp
 * Author: brandon
 * 
 * Created on May 3, 2009, 4:05 PM
 */

#include "oglWidgetRender.h"

#include "../../thememgr.h"

#include <GL/gl.h>
#include <GL/glext.h>

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

oglWidgetRender::oglWidgetRender() {
	shader = new Shader("guiAnimation");

	shader->GetUniformLoc(0, "projection");
	shader->GetUniformLoc(1, "modelview");
	shader->GetUniformLoc(2, "tex0");
	shader->GetAttributeLoc(0, "vertex");
	shader->GetAttributeLoc(1, "tcoord");
	shader->GetAttributeLoc(2, "tcolor");
}

oglWidgetRender::~oglWidgetRender() { }

/** * @param TODO TODO
 * @returns TODO
 * Adds a object to the list
 * @param obj a pointer to the new object, cannot be null.
 */
void oglWidgetRender::AddObject(void* obj) {
	if (obj == NULL)
		throw 19;

	Objects.push_back(static_cast<Control*> (obj));
	Update(obj, RENDERER_ADD);
}

/**
 * Removes the object from the list
 * @param obj the object to be removed
 */
void oglWidgetRender::RemoveObject(void* obj) {
	if (obj == NULL)
		throw 20;

	std::vector<Control*>::iterator it = Objects.begin();
	while (it != Objects.end()) {
		if (*it == obj) {
			it = Objects.erase(it);
			return;
		} else {
			it++;
		}
	}

	printf("Unable to remove object, object not found! \n");
}

/**
 * Binds and sets up all, known, data for rendering
 */
void oglWidgetRender::Begin() {
	shader->Bind();
	shader->SetProjection(camera->GetOrtho());
	shader->SetModelview(camera->GetModelview());

	//We bind the theme image
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ThemeMgr_GetImage());

	Buffer.Bind();

	glEnableVertexAttribArray(shader->attribute[0]);
	glEnableVertexAttribArray(shader->attribute[1]);
	glEnableVertexAttribArray(shader->attribute[2]);

	glVertexAttribPointer(shader->attribute[0], 2, GL_FLOAT, GL_FALSE, sizeof(WidgetData), 0);
        glVertexAttribPointer(shader->attribute[1], 2, GL_FLOAT, GL_FALSE, sizeof(WidgetData), (GLvoid*) (4 * sizeof(float)));
        glVertexAttribPointer(shader->attribute[2], 4, GL_FLOAT, GL_FALSE, sizeof(WidgetData), (GLvoid*) (8 * sizeof(float)));
}

/**
 * Renders all objects
 */
void oglWidgetRender::Render() { 
	glDrawArrays(GL_QUADS, 0, 7 * 4 );
}

/**
 * Unbinds all, known, data
 */
void oglWidgetRender::End() {
	glDisableVertexAttribArray(shader->attribute[0]);
	glDisableVertexAttribArray(shader->attribute[1]);
	glDisableVertexAttribArray(shader->attribute[2]);

	Buffer.Unbind();
	shader->Unbind();
}

/**
 * Does a full render cycle, same as calling Begin(), Render(), End()
 */
void oglWidgetRender::Draw() {
	Begin();
	Render();
	End();
}

/**
 * 'Refreshes' the buffer. Clearning it before readding all data.
 * Mainly used for stream drawing.
 */
void oglWidgetRender::Refresh() {
	Buffer.RemoveData(0, Buffer.GetSize());

	size_t size = Objects.size();
	for (unsigned int i = 0; i < size; i++) {
		Update(Objects[i], RENDERER_ADD);
	}
}

/**
 * Updates a object in the Buffer
 * @param obj the pointer of the object that needs updating
 * @param update a enum for the type of update to use.
 */
void oglWidgetRender::Update(void* obj, unsigned int update) {
	Control* object = static_cast<Control*> (obj);
	Control* child;

	unsigned int dataSize = object->TotalChildren() * 4;
	WidgetData* data = new WidgetData[ dataSize ];

	//these are for cleaner code
	//also SHOULD allow for easyer SSE complation
	unsigned int slot = 0;
	float vx, vx2, vy, vy2;
	float vs, vs2, vt, vt2;
	float* c;

	unsigned int j = 0;
	size_t size1 = object->NumChildren();
	size_t size2 = 0;
	for (unsigned int i = 0; i < size1; i++) {
		child = object->GetChild(i);
		size2 = child->TotalChildren();

		j = 0;
		do {
			vx = child->x;
			vx2 = (child->x + child->GetWidth());
			vy = child->y;
			vy2 = (child->y + child->GetHeight());

			vs = child->s;
			vs2 = child->s2;
			vt = child->t;
			vt2 = child->t2;

			//get the color
			c = child->GetColorv();

			//top left
			data[slot].x = vx;
			data[slot].y = vy;
			data[slot].s = vs;
			data[slot].t = vt;
			data[slot].r = c[0];
			data[slot].g = c[1];
			data[slot].b = c[2];
			data[slot].a = c[3];
			slot++;

			//top right
			data[slot].x = vx2;
			data[slot].y = vy;
			data[slot].s = vs2;
			data[slot].t = vt;
			data[slot].r = c[0];
			data[slot].g = c[1];
			data[slot].b = c[2];
			data[slot].a = c[3];
			slot++;

			//bottom right
			data[slot].x = vx2;
			data[slot].y = vy2;
			data[slot].s = vs2;
			data[slot].t = vt2;
			data[slot].r = c[0];
			data[slot].g = c[1];
			data[slot].b = c[2];
			data[slot].a = c[3];
			slot++;

			//bottom left
			data[slot].x = vx;
			data[slot].y = vy2;
			data[slot].s = vs;
			data[slot].t = vt2;
			data[slot].r = c[0];
			data[slot].g = c[1];
			data[slot].b = c[2];
			data[slot].a = c[3];
			slot++;

			child = child->GetChild(j);

			j++;
		} while (j <= size2);
	}

	Buffer.Bind();

	if (update == RENDERER_ADD) {
		Buffer.AddData(dataSize * sizeof(WidgetData), data);
	} else if (update == RENDERER_REFRESH) {
		unsigned int start = 0;

		int i = 0;
		while (obj != Objects[i]) {
			start += sizeof(WidgetData);
			i++;
		}

		Buffer.SetData(start, dataSize*sizeof(WidgetData), data);
	}

	Buffer.Unbind();

	delete [] data;
}

void oglWidgetRender::SetCamera(Camera* cam){
	if( cam == NULL )
		throw 0;
	camera = cam;
}