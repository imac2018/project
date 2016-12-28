#include "object3d.h"
#include "assimp/vector2.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "glexception.h"

Object3D::Object3D(QList<Vertex3D> vertex, const QColor &globalColor, int drawMode)
	: vertex(vertex), diffuseColor(globalColor), ambiantColor(globalColor),
	  specularColor(globalColor), drawMode(drawMode), texture(NULL), normalTexture(NULL)
{

}

Object3D::Object3D(const QColor &globalColor, int drawMode)
	: diffuseColor(globalColor), ambiantColor(globalColor),
	  specularColor(globalColor), drawMode(drawMode), texture(NULL), normalTexture(NULL)
{

}

Object3D::Object3D(const Object3D &other)
	: indexStart(other.indexStart),
	  indexCount(other.indexCount),
	  diffuseColor(other.diffuseColor),
	  ambiantColor(other.ambiantColor),
	  specularColor(other.specularColor),
	  texture(other.texture),
	  normalTexture(other.normalTexture),
	  drawMode(other.drawMode)
{}

void Object3D::clone(Object3D &other) const
{
	other.indexStart = indexStart;
	other.indexCount = indexCount;
	other.drawMode = drawMode;
}

void Object3D::copy(const Object3D &other)
{
	this->indexCount = other.indexCount;
	this->indexStart = other.indexStart;
	this->drawMode = other.drawMode;
}

void Object3D::draw() const
{
	if(texture){
		texture->bind();
		glDrawArrays(drawMode, indexStart, indexCount);
		texture->release();
	}
	else
		glDrawArrays(drawMode, indexStart, indexCount);
}

void Object3D::setGlobalColor(const QColor &color)
{
	diffuseColor = ambiantColor = specularColor = color;
}

QList<Object3D *> Object3D::importFromFile(QString filepath)
{
	/*Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile( filepath.toStdString(),
			aiProcess_CalcTangentSpace       |
			aiProcess_Triangulate            |
			aiProcess_JoinIdenticalVertices  |
			aiProcess_SortByPType);

	if( !scene){
		throw GLException("3D Import Error", importer.GetErrorString());
		//throw std::exception(importer.GetErrorString());
	}
*/
	return QList<Object3D*>(); //ToDo
}

Vertex3D::Vertex3D(aiVector3D position, aiColor3D color, aiVector2D texCoord,
				   aiVector3D normal, aiVector3D bitangente, aiVector3D tangente)
	: position(position), color(color), texCoord(texCoord),
	  normal(normal), bitangente(bitangente), tangente(tangente)
{}

Vertex3D::Vertex3D(aiVector3D position, aiColor3D color)
	: position(position), color(color)
{}

Vertex3D::Vertex3D(aiVector3D position, aiVector2D texCoord)
	: position(position), color(1,1,1), texCoord(texCoord)
{}

Vertex3D::Vertex3D(aiVector3D position)
	: position(position), color(1,1,1)
{

}

Vertex3D::Vertex3D()
	: color(1,1,1)
{

}

Vertex3D::Vertex3D(float x, float y, float z)
	: position(x,y,z), color(1,1,1)
{}

Vertex3D::Vertex3D(float x, float y, float z, float u, float v)
	: position(x,y,z), color(1,1,1), texCoord(u,v)
{}

Vertex3D::Vertex3D(float x, float y, float z, float r, float g, float b)
	: position(x,y,z), color(r,g,b)
{}

QString Vertex3D::toString()
{
	return "Position: " + QString("(%1,%2,%3)").arg(position.x)
												.arg(position.y)
												.arg(position.z) +
			"\nColor: " + QString("(%1,%2,%3)").arg(color.r)
												.arg(color.g)
												.arg(color.b) +
			"\nTexCoords: " + QString("(%1,%2)").arg(texCoord.x)
												.arg(texCoord.y);
}
