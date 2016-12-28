#ifndef VERTEX_H
#define VERTEX_H
#include <assimp/types.h>
#include <QString>

struct Vertex3D{
	aiVector3D position;
	aiVector3D normal;
	aiVector3D bitangente;
	aiVector3D tangente;
	aiColor3D color;
	aiVector2D texCoord;
	Vertex3D(aiVector3D position, aiColor3D color, aiVector2D texCoord,
			 aiVector3D normal,	aiVector3D bitangente, aiVector3D tangente);
	Vertex3D(aiVector3D position, aiColor3D color);
	Vertex3D(aiVector3D position,aiVector2D texCoord);
	Vertex3D(aiVector3D position);
	Vertex3D();
	Vertex3D(float x, float y, float z);
	Vertex3D(float x, float y, float z, float u, float v);
	Vertex3D(float x, float y, float z, float r, float g, float b);
	QString toString();
};

#endif // VERTEX_H
