#ifndef RENDERER_H
#define RENDERER_H

#include <QGLBuffer>
#include <QGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>

#include <assimp/types.h>

#include "camera.h"
#include "vertex.h"

class QMatrix4x4;
class Object3D;

struct DirectionalLight{
	QVector3D direction;
	QColor color;
};

struct TorchLight{
	QColor color;
	float power;
};

class Renderer
{

	friend class Game;
public:
	enum AttributeIndex{
		A_Position=0,
		A_Color,
		A_TexturePosition,
		A_Normal,
		A_Tangente,
		A_Bitangente,
		AttributesCount
	};

	enum ShaderType{
		NoShader=-1,
		Minimal=0,
		Normal,
		Advanced,
		ShadersCount
	};

	enum ProjectionType{
		Perspective,
		Ortho,
		Frustrum,
		NoProjection
	};
private:


	QMatrix4x4 globalTransform;
	QMatrix4x4 projectionMatrix;
	QMatrix4x4 currentMVMatrix;
	QMatrix4x4 currentMVPMatrix;
	QColor globalIllumination;
	QColor ambiantLight;
	bool lock;

	QList<Vertex3D> vertexList;
	QList<QOpenGLTexture*> texturesList;

	QGLBuffer vbo;
	QOpenGLVertexArrayObject  vao;

	QGLShaderProgram shaders[ShadersCount];

	ShaderType currentShaders;

	QOpenGLFunctions* GL;

	void setPerspective(float verticalAngle, float aspectRatio,
				   float nearPlane, float farPlane);
	void setOrtho(float left, float right,
			 float bottom, float top,
			 float nearPlane, float farPlane);
	void setFrustum(float left, float right,
			   float bottom, float top,
			   float nearPlane, float farPlane);

	void prepareShaderProgram(QGLShaderProgram &program, const QString& vertexShaderPath,
								const QString& fragmentShaderPath );



	void setCurrentShaderUniformValue();
	void advancedDraw(const Object3D* o);
	void normalDraw(const Object3D *o);
	void minimalDraw(const Object3D *o);

public:
	ProjectionType projectionType;

	Renderer();
	void initialize();
	void resetGlobal();
	void addObject(Object3D& o);
	QOpenGLTexture* addTexture(QString filename);
	QOpenGLTexture* addTexture(const QPixmap& pix);
	void initializeVBO();
	void initializeVAO();

	void initRender();
	void endRender();

	void bindShader(ShaderType shaderType);

	void bindDirectionalLight(const DirectionalLight& light);
	void releaseDirectionalLight();

	void bindTorchLight(const TorchLight& light);
	void releaseTorchLight();

	void bindCamera(const Camera& cam);
	void releaseCamera();

	void draw(const Object3D* o);
	void draw(const QList<Object3D*> &o);

	void clearVBO();
	void clearVAO();
	void clearTextures();

	void setIndex(Object3D& o);

	Object3D makeColoredFace(float rotationX, float rotationY, float rotationZ,
							 float translationX, float translationY, float translationZ,
							 QColor color, float scale=1);
	Object3D makeTexturedFace(float rotationX, float rotationY, float rotationZ,
							  float translationX, float translationY, float translationZ,
							  float scaleX=1, float scaleY=0, float repeat=1);

};

#endif // RENDERER_H
