/*   This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.
	You should have received a copy of the GNU General Public License
	long with this program.  If not, see <http://www.gnu.org/licenses/>

 * 	Copyright 2008,2009 James Brandon Stevenson
 */

/* 
 * File:   vbo.cpp
 * Author: brandon
 * 
 * Created on April 26, 2009, 8:00 PM
 */
#include <GL/glew.h>
#include "vbo.h"

#include <cassert>
#include <iostream>
#include <cstring>

VBO::VBO(GLenum t) : isBound(false), size(0), type(t) {
    glGenBuffers(1, &id);
}

VBO::VBO(const VBO& orig) : isBound(false), size(orig.size), type(orig.type), id(orig.id) { }

VBO::VBO(unsigned int length, void* data, GLenum t) : isBound(false), size(length), type(t) {
    glGenBuffers(1, &id);

    if (length != 0) {
	Bind();
	InitData(length, data);
	Unbind();
    }
}

VBO::~VBO() {
    glDeleteBuffers(1, &id);
}

void VBO::Bind() {
    glBindBuffer(GL_ARRAY_BUFFER, id);
    isBound = true;
}

void VBO::Unbind() {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    isBound = false;
}

/* Sets the vbo data, only used if not set yet */
bool VBO::InitData(unsigned int length, void* data) {
    glBufferData(GL_ARRAY_BUFFER, length, data, type);

    size = length;
    return true;
}

/* Sets the existing portions of the vbo to data */
bool VBO::SetData(unsigned int start, unsigned int length, void* data) {
    assert(isBound);

    void* ptr = glMapBufferRange(GL_ARRAY_BUFFER, start, length, GL_MAP_WRITE_BIT | GL_MAP_UNSYNCHRONIZED_BIT | GL_MAP_FLUSH_EXPLICIT_BIT);

    if (ptr != NULL) {
	memcpy(ptr, data, length);

	glFlushMappedBufferRange(GL_ARRAY_BUFFER, 0, length);
	glUnmapBuffer(GL_ARRAY_BUFFER);
    } else {
	printf("Error: VBO %i could not set data! %i, %u %u\n", id, glGetError(), start, length);
	assert(false);
	return false;
    }

    return true;
}

/* Adds length data to the end of the vbo, increasing the size by length after. */
bool VBO::AddData(unsigned int length, void* data, unsigned int* position) {
    assert(isBound);

    if (position != NULL)
	(*position) = size;

    if (size == 0) {
	return InitData(length, data);
    }

    void* ptr = glMapBufferRange(GL_ARRAY_BUFFER, 0, size, GL_MAP_READ_BIT);

    if (ptr != NULL) {
	char* tdata = new char[ size + length ];

	memcpy(tdata, ptr, size);
	memcpy(&tdata[size], data, length);

	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBufferData(GL_ARRAY_BUFFER, size + length, tdata, type);

	size += length;
	delete [] tdata;
    } else {
	printf("Error: VBO %i could not add data! %i\n", id, glGetError());
	return false;
    }

    return true;
}

/* Inserts data at a given position, with changing lengths. */
bool VBO::InsertData(unsigned int start, unsigned int oldLength, unsigned int newLength, void* data) {
    assert(isBound);

    int change = newLength - oldLength;
    void* ptr = glMapBufferRange(GL_ARRAY_BUFFER, 0, size, GL_MAP_READ_BIT);

    if (ptr != NULL) {
	char* tdata = new char[size + change];

	memcpy(tdata, ptr, start);
	memcpy(&tdata[start], data, newLength);
	memcpy(&tdata[ start + newLength ], ptr, size - (start + oldLength));

	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBufferData(GL_ARRAY_BUFFER, size + change, tdata, type);

	size += change;
    } else {
	printf("Error: VBO %i could not insert data! %i\n", id, glGetError());
	return false;
    }

    return true;
}

bool VBO::RemoveData(unsigned int start, unsigned int length) {
    if (!isBound || size == 0)
	return false;

    char* ptr = (char*) glMapBufferRange(GL_ARRAY_BUFFER, 0, size, GL_MAP_READ_BIT);

    if (ptr != NULL) {
	char* tdata = new char[ size - length ];

	memcpy(tdata, ptr, start);
	memcpy(&tdata[start], &ptr[start + length], size - length - start);

	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBufferData(GL_ARRAY_BUFFER, size - length, tdata, type);

	size -= length;
	delete [] tdata;
    } else {
	printf("Error: VBO %i could not remove data! %i\n", id, glGetError());
	return false;
    }

    return true;
}

const GLuint VBO::GetID() {
    return id;
}

const unsigned int VBO::GetSize() {
    return size;
}
