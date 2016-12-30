#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <QString>
#include <QList>
#include <QOpenGLTexture>
#include <assimp/types.h>

#include "renderer.h"
#include "vertex.h"

class Object3D
{

protected:
	GLint indexStart;
	GLsizei indexCount;
	int drawMode;

public:

	QList<Vertex3D> vertex;
	QOpenGLTexture * texture;
	QOpenGLTexture * normalTexture;

	QColor diffuseColor;
	QColor ambiantColor;
	QColor specularColor;

	QMatrix4x4 transformM;

	Object3D(QList<Vertex3D> vertex, const QColor &color, int drawMode = GL_TRIANGLES);
	Object3D(QList<Vertex3D> vertex, int drawMode = GL_TRIANGLES);
	Object3D(const QColor& color, int drawMode = GL_TRIANGLES);
	Object3D(const Object3D& other);

	void clone(Object3D& other) const;
	void copy(const Object3D& other);

	virtual void draw() const;

	void setGlobalColor(const QColor& color);

	static QList<Object3D*> importFromFile(QString filepath);
	friend void Renderer::setIndex(Object3D& o);
};

#endif // OBJECT3D_H
