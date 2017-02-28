/*
#   clove
#
#   Copyright (C) 2017 Muresan Vlad
#
#   This project is free software; you can redistribute it and/or modify it
#   under the terms of the MIT license. See LICENSE.md for details.
*/
#include "mesh.h"

void graphics_Mesh_new(graphics_Mesh* mesh, int vertexCount, graphics_Vertex* vertices, int indexCount, unsigned int* indices, graphics_Image* image, graphics_MeshDrawMode drawMode) {

    mesh->vertices = 0;
    printf("%s %d", "Index count e: ", indexCount);

    mesh->image = image;
    mesh->drawMode = drawMode;
   
    mesh->vertexCount = 0;
    mesh->indexCount = 0;

    glGenBuffers(1, &mesh->vbo);
    glGenBuffers(1, &mesh->ibo);

    graphics_Mesh_setVertices(mesh, vertices, vertexCount);
    graphics_Mesh_setIndices(mesh, indices, indexCount);    

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(graphics_Vertex), 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(graphics_Vertex), (GLvoid const*)(2*sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(graphics_Vertex), (GLvoid const*)(4*sizeof(float)));

}

void graphics_Mesh_free(graphics_Mesh* mesh) {
    glDeleteBuffers(1, &mesh->ibo);
    glDeleteBuffers(1, &mesh->vbo);
    
    free(mesh->indices);
    free(mesh->vertices);
}

void graphics_Mesh_setVertices(graphics_Mesh* mesh, graphics_Vertex* vertices, int vertexCount) {
    glBindBuffer(GL_ARRAY_BUFFER, mesh->vbo);
    glBufferData(GL_ARRAY_BUFFER, vertexCount * sizeof(graphics_Vertex), vertices, GL_DYNAMIC_DRAW);
    
    if (vertexCount != mesh->vertexCount) {
        free(mesh->vertices);
        mesh->vertices = malloc(sizeof(graphics_Vertex) * vertexCount);
        mesh->vertexCount = vertexCount;
    } 
    
    memcpy(mesh->vertices, vertices, vertexCount * sizeof(graphics_Vertex));
}

void graphics_Mesh_setIndices(graphics_Mesh* mesh, unsigned int* indices, int indexCount) {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_DYNAMIC_DRAW);

    if (indexCount != mesh->indexCount) {
        free(mesh->indices);
        mesh->indices = malloc(sizeof(unsigned int) * indexCount);
        mesh->indexCount = indexCount;
    }
    
    memcpy(mesh->indices, indices, sizeof(unsigned int) * indexCount);
}

static const graphics_Quad quad = {0.0f, 0.0f, 1.0f, 1.0f};
void graphics_Mesh_draw(graphics_Mesh* mesh, float x, float y, float r, float sx, float sy, float ox, float oy, float kx, float ky) {
    
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0); 
    glBindTexture(GL_TEXTURE_2D, mesh->image->texID);
        
    //glBufferData(GL_ARRAY_BUFFER, sizeof(graphics_Vertex) * mesh->vertexCount, mesh->vertices, GL_DYNAMIC_DRAW);
   // glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(graphics_Vertex) * mesh->vertexCount, mesh->vertices);

    mat4x4 tr2d;
    m4x4_newTransform2d(&tr2d, x, y, r, sx , sy, ox, oy, kx, ky);
    graphics_drawArray(&quad, &tr2d, mesh->ibo, mesh->indexCount, mesh->drawMode, GL_UNSIGNED_INT, graphics_getColor(), 1.0f, 1.0f);
}



