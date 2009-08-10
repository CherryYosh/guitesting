/* 
 * File:   oglFontRenderer.h
 * Author: brandon
 *
 * Created on June 2, 2009, 3:13 PM
 */

#ifndef _OGLFONTRENDERER_H
#define	_OGLFONTRENDERER_H

#include "../../colorstring.h"
#include "../fontrenderer.h"
#include "../../camera.h"
#include "../../gui/widget.h"
#include "shader.h"
#include "vbo.h"

struct FontData;

class oglFontRenderer : public FontRenderer {
public:
    oglFontRenderer();
    oglFontRenderer(const oglFontRenderer& orig);
    virtual ~oglFontRenderer();

    unsigned int GetTotalChars(void*);

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
    void GenerateStringData(Widget*, FontData*, int&, size_t&, int&);

    unsigned int TotalObjects;
    std::vector<Widget*> Objects;
    Shader* shader;
    VBO Buffer;
};

#endif	/* _OGLFONTRENDERER_H */

