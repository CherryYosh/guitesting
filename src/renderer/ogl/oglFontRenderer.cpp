/* 
 * File:   oglFontRenderer.cpp
 * Author: brandon
 * 
 * Created on June 2, 2009, 3:13 PM
 */

#include "oglBase.h"
#include "oglFontRenderer.h"

#include "../../gui/label.h"

#include <stdexcept>

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

    std::vector<Control*> labels = static_cast<Control*> (obj)->GetChildrenWith(GUI_HAS_TEXT);
    if (labels.empty()) {
	return 0;
    }

    size1 = labels.size();
    for (unsigned int i = 0; i < size1; i++) {
	strings = static_cast<Label*> (labels[i])->GetText();

	size2 = strings.size();
	for (unsigned int j = 0; j < size2; j++) {
	    ret += strings[j].size();
	}
    }
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
    if (TotalObjects > 0)
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

    TotalObjects = 0; //reset this, will be set in Update()

    size_t size = Objects.size();
    for (unsigned int i = 0; i < size; i++) {
	Update(Objects[i], RENDERER_ADD);
    }
}

void oglFontRenderer::Update(void* obj, unsigned int update) {
    float h;
    int height, lines;

    std::vector<Control*> labels = static_cast<Control*> (obj)->GetChildrenWith(GUI_HAS_TEXT);
    if (labels.empty())
	return;

    unsigned int dataSize = GetTotalChars(obj);
    size_t size1, oldSize;
    size_t size2 = 0;

    FontData* data = new FontData[4 * dataSize];

    size1 = labels.size();
    for (unsigned int i = 0; i < size1; i++) {
	oldSize = size2;
	GenerateStringData(labels[i], data, lines, size2, height);
	for (unsigned int j = oldSize; j < (size2); j += 4) {
	    h = (data[j].y - lines) * (height);

	    //TODO: Remove this hack
	    //Find a better way to clip
	    if (data[j + 1].y + h > labels[i]->GetY()) {
		data[j].y = data[j + 1].y + h;
		data[j + 1].y += h;
		data[j + 2].y += h;
		data[j + 3].y += h;
	    } else {
		data[j].y = 0;
		data[j + 1].y = 0;
		data[j + 2].y = 0;
		data[j + 3].y = 0;
	    }
	}
    }

    Buffer.Bind();

    if (update == RENDERER_ADD) {
	TotalObjects += dataSize;
	Buffer.AddData(sizeof(FontData) * 4 * dataSize, data);
    } else {
	throw std::invalid_argument("oglFontRenderer::Update::wtf_are_you_thinking");
    }

    Buffer.Unbind();

    delete [] data;
}

/**
 * Generates the string data, setting everything that is needed except the first y
 * the first y will be set as a line number and then a second pass must be made to correctly set it and the others
 * @param obj the control object to use
 * @param outLines out - the number of total lines, used when setting the y correctly
 */
void oglFontRenderer::GenerateStringData(Control* obj, FontData* data, int& numLines, size_t& slot, int& height) {
    float x, by, y, z;
    float* color;
    FontChar* c;
    numLines = 1; //new

    std::vector<Control*> labels = obj->GetChildrenWith(GUI_HAS_TEXT);
    if (labels.empty()) {
	return;
    }


    std::vector<colorstring> strings;
    colorstring str;

    Label* label;

    size_t size1, size2, size3;

    size1 = labels.size();
    for (unsigned int i = 0; i < size1; i++) {
	label = static_cast<Label*> (labels[i]);
	strings = label->GetText();
	z = label->GetDepth();

	size2 = strings.size();
	for (unsigned int j = 0; j < size2; j++) {
	    str = strings[j];
	    x = label->GetX();
	    by = label->GetY() + label->GetHeight();

	    size3 = str.size();
	    for (unsigned int l = 0; l < size3; l++) {
		c = font->GetChar(str.at(l).character);
		x += c->bearingX;
		y = by - c->bearingY;

		if ((x + c->width > (label->GetX() + label->GetWidth())) && label->multiline()) {
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

	    if (numLines < size2 && label->multiline()) {
		numLines++;
	    }
	}
    }

    //currently this is easy, but will need to be changed when multi-fonts are allowed
    height = font->GetHeight();
}

Shader* oglFontRenderer::GetShader() {
    return shader;
}

void oglFontRenderer::SetShader(Shader* s) {
    if (s != NULL)
	shader = s;
}

int* oglFontRenderer::GetViewport() {
    return base->GetViewport();
}

Camera* oglFontRenderer::GetCamera() {
    return base->GetCamera();
}