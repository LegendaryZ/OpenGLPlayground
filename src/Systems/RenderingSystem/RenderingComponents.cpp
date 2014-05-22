#include "RenderingComponents.h"
#include "..\..\..\gl\include\glew.h"
#include "..\..\..\gl\include\glut.h"

void RenderingComponent::draw()
{
	/*
	unsigned int vertexbuffer;		// This will identify our vertex buffer
	unsigned int indexbuffer;

	glGenBuffers(1, &vertexbuffer);
	glBindVertexArray(vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(mesh.Vertices), &(mesh.Vertices[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &indexbuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexbuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(mesh.Indices), &(mesh.Indices), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);

    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, (void*)0); 
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 16, (void*)0);    

	// Draw the triangle !
	glDrawElements(GL_TRIANGLES, sizeof(mesh.Indices), GL_UNSIGNED_SHORT, (void*)0);
 
	glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

	glDeleteBuffers(1, &vertexbuffer);
	glDeleteBuffers(1, &indexbuffer);
	*/
}