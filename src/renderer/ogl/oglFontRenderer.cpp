/* 
 * File:   oglFontRenderer.cpp
 * Author: brandon
 * 
 * Created on June 2, 2009, 3:13 PM
 */

#include "oglBase.h"
#include "oglFontRenderer.h"

#include "../../gui/label.h"

struct FontData {
	float x, y, z;
	float spacer1;
	float s, t;
	float spacer2[2];
	float r, g, b, a;
	float spacer3[4];
};

oglFontRenderer::oglFontRenderer() : TotalObjects(0) {
	base = new oglBase;

	shader = new Shader("textShader");

	shader->GetUniformLoc(0, "projection");
	shader->GetUniformLoc(1, "modelview");
	shader->GetUniformLoc(2, "tex0");
	shader->GetAttributeLoc(0, "vertex");
	shader->GetAttributeLoc(1, "tcoord");
	shader->GetAttributeLoc(2, "tcolor");
}

oglFontRenderer::oglFontRenderer(const oglFontRenderer& orig) : shader(orig.shader) { }

oglFontRenderer::~oglFontRenderer() { }

unsigned int oglFontRenderer::GetTotalChars(void* obj) {
	unsigned int ret = 0;

	std::vector<colorstring> strings;
	size_t size1, size2;

	std::vector<Label*> labels = static_cast<Control*> (obj)->GetTextObjs();
	if (labels.empty()){
		return 0;
	}

	size1 = labels.size();
	for (unsigned int i = 0; i < size1; i++) {
		strings = labels[i]->GetText();

		size2 = strings.size();
		for (unsigned int j = 0; j < size2; j++) {
			//printf("size is %i\n", strings[j].size());
			ret += strings[j].size();
		}
	}

	//printf("returning %i\n", ret);
	return ret;
}

/**
 * Adds a object to the list
 * @param obj a pointer to the new object, cannot be null.
 */
void oglFontRenderer::AddObject(void* obj) {
	assert(obj);

	TotalObjects += GetTotalChars(obj);
	Objects.push_back(static_cast<Control*> (obj));
}

/**
 * Removes the object from the list
 * @param obj the object to be removed
 */
void oglFontRenderer::RemoveObject(void* obj) {
	assert(obj != NULL);

	std::vector<Control*>::iterator it = Objects.begin();
	while (it != Objects.end()) {
		if (*it == obj) {
			it = Objects.erase(it);
		} else {
			it++;
		}
	}

	//removes all old data :)
	Refresh();
}

void oglFontRenderer::Begin() {
	Camera* c = base->GetCamera();

	shader->Bind();
	shader->SetProjection(c->GetOrthofv());
	shader->SetModelview(c->GetModelviewfv());

	//We bind the theme image
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, font->GetImageID());

	Buffer.Bind();

	glEnableVertexAttribArray(shader->attribute[0]);
	glEnableVertexAttribArray(shader->attribute[1]);
	glEnableVertexAttribArray(shader->attribute[2]);

	glVertexAttribPointer(shader->attribute[0], 3, GL_FLOAT, GL_FALSE, sizeof(FontData), 0);
	glVertexAttribPointer(shader->attribute[1], 2, GL_FLOAT, GL_FALSE, sizeof(FontData), (GLvoid*) (4 * sizeof(float)));
	glVertexAttribPointer(shader->attribute[2], 4, GL_FLOAT, GL_FALSE, sizeof(FontData), (GLvoid*) (8 * sizeof(float)));
}

void oglFontRenderer::Render() {
	glDrawArrays(GL_QUADS, 0, TotalObjects * 4);
}

void oglFontRenderer::End() {
	glDisableVertexAttribArray(shader->attribute[0]);
	glDisableVertexAttribArray(shader->attribute[1]);
	glDisableVertexAttribArray(shader->attribute[2]);

	Buffer.Unbind();
	shader->Unbind();
}

void oglFontRenderer::Draw() {
	Begin();
	Render();
	End();
}

/**
 * 'Refreshes' the buffer. Clearning it before readding all data.
 */
void oglFontRenderer::Refresh() {
	Buffer.Bind();
	Buffer.RemoveData(0, Buffer.GetSize());
	Buffer.Unbind();

	size_t size = Objects.size();
	for (unsigned int i = 0; i < size; i++) {
		Update(Objects[0], RENDERER_ADD);
	}
}

void oglFontRenderer::Update(void* obj, unsigned int update) {
	float h;
	int height;
	int lines = 0;

	std::vector<Label*> labels = static_cast<Control*> (obj)->GetTextObjs();
	if (labels.empty())
		return;
	
	TotalObjects = GetTotalChars(obj);

	size_t size1, size2;

	FontData* data = new FontData[4 * TotalObjects];

	size1 = labels.size();
	for (unsigned int i = 0; i < size1; i++) {
		height = labels[i]->GetHeight();
		GenerateStringData(static_cast<Control*>(labels[i]), data, lines, size2);
		for (unsigned int j = 0; j < size2; j++) {
			h = (data[j*4].y - lines) * height;
			data[j*4].y = data[j*4+1].y + h;
			data[j*4+1].y += h;
			data[j*4+2].y += h;
			data[j*4+3].y += h;
		}
	}

	Buffer.Bind();
	Buffer.AddData(sizeof(FontData) * 4 * TotalObjects, data);
	Buffer.Unbind();

	delete [] data;
}

/**
 * Generates the string data, setting everything that is needed except the first y
 * the first y will be set as a line number and then a second pass must be made to correctly set it and the others
 * @param obj the control object to use
 * @param outLines out - the number of total lines, used when setting the y correctly
 */
void oglFontRenderer::GenerateStringData(Control* obj, FontData* data, int& outLines, size_t& size) {
	float x, by, y, z;
	float* color;
	FontChar* c;
	int slot = 0;
	int numLines = 0;

	std::vector<Label*> labels = obj->GetTextObjs();
	if(labels.empty()){
		outLines = 0;
		size = 0;
		return;
	}


	std::vector<colorstring> strings;
	colorstring str;

	Label* label;

	size_t size1, size2, size3;

	//unsigned int numChars = GetTotalChars(obj);
	//printf("numChars is %i %i\n", numChars, labels.size());

	size1 = labels.size();
	for (unsigned int i = 0; i < size1; i++) {
		label = labels[i];
		strings = label->GetText();
		z = label->GetDepth();

		size2 = strings.size();
		for (unsigned int j = 0; j < size2; j++) {
			numLines++;
			str = strings[j];
			x = label->GetX();
			by = label->GetY();

			size3 = str.size();
			for (unsigned int l = 0; l < size3; l++) {
				c = font->GetChar(str.at(l).character);
				x += c->bearingX;
				y = by - c->bearingY;

				if (x + c->width >= (label->GetX() + label->GetWidth())) {
					x = label->GetX();
					numLines++;
				}

				color = str.at(l).color.array();

				data[slot].x = x;
				data[slot].y = numLines;
				data[slot].z = z;
				data[slot].s = c->s;
				data[slot].t = c->t;
				data[slot].r = color[0];
				data[slot].g = color[1];
				data[slot].b = color[2];
				data[slot].a = color[3];
				slot++;

				data[slot].x = x + c->width;
				data[slot].y = y;
				data[slot].z = z;
				data[slot].s = c->s2;
				data[slot].t = c->t;
				data[slot].r = color[0];
				data[slot].g = color[1];
				data[slot].b = color[2];
				data[slot].a = color[3];
				slot++;

				data[slot].x = x + c->width;
				data[slot].y = y + c->height;
				data[slot].z = z;
				data[slot].s = c->s2;
				data[slot].t = c->t2;
				data[slot].r = color[0];
				data[slot].g = color[1];
				data[slot].b = color[2];
				data[slot].a = color[3];
				slot++;

				data[slot].x = x;
				data[slot].y = y + c->height;
				data[slot].z = z;
				data[slot].s = c->s;
				data[slot].t = c->t2;
				data[slot].r = color[0];
				data[slot].g = color[1];
				data[slot].b = color[2];
				data[slot].a = color[3];
				slot++;

				x += c->advance;

				//check the length to prevent a overflow
				if ((l + 1) < size3)
					x += font->GetKerning(str.at(l).character, str.at(l + 1).character);
			}
		}
	}
	outLines = numLines;
	size = slot * 0.25;
}

Shader* oglFontRenderer::GetShader() {
	return shader;
}

void oglFontRenderer::SetShader(Shader* s) {
	if (s == NULL)
		return;
	shader = s;
}

int* oglFontRenderer::GetViewport() {
	return base->GetViewport();
}

Camera* oglFontRenderer::GetCamera() {
	return base->GetCamera();
}