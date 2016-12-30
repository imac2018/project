#include <QMatrix4x4>
#include <QImage>
#include <QDebug>

#include <assimp/types.h>
#include <assimp/matrix4x4.h>

#include "renderer.h"
#include "object3d.h"
#include "glexception.h"

#include "tools.h"

Renderer::Renderer()
	: projectionType(NoProjection),
	  currentShaders(NoShader),
	  vbo(QGLBuffer::VertexBuffer),
	  lock(false)
{}

void Renderer::initialize()
{
	prepareShaderProgram(shaders[Minimal], ":/shaders/minimal_vertex.vs.glsl",
										 ":/shaders/minimal_fragment.fs.glsl");
	prepareShaderProgram(shaders[Normal], ":/shaders/vertex.vs.glsl",
										":/shaders/fragment.fs.glsl");
	prepareShaderProgram(shaders[Advanced], ":/shaders/advanced_vertex.vs.glsl",
										  ":/shaders/advanced_fragment.fs.glsl");

	shaders[Advanced].bindAttributeLocation("position", A_Position);
	shaders[Advanced].bindAttributeLocation("color", A_Color);
	shaders[Advanced].bindAttributeLocation("texpos", A_TexturePosition);
	shaders[Advanced].bindAttributeLocation("normal", A_Normal);
	shaders[Advanced].bindAttributeLocation("tangente", A_Tangente);
	shaders[Advanced].bindAttributeLocation("bitangente", A_Bitangente);

	GL = QOpenGLContext::currentContext()->functions();
	resetGlobal();
	glEnable(GL_DEPTH_TEST);
	glEnable (GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	//GL->glActiveTexture(GL_TEXTURE0);
}

void Renderer::resetGlobal()
{
	if(!lock){
		globalTransform = QMatrix4x4();
		projectionMatrix = QMatrix4x4();
		globalIllumination = QColor::fromRgbF(1,1,1);
		ambiantLight = QColor::fromRgbF(0.3f,0.25f,0.38f);
	}
}

void Renderer::addObject(Object3D &o)
{
	setIndex(o);
	vertexList.append(o.vertex);
	o.vertex.clear();
}

QOpenGLTexture *Renderer::addTexture(QString filename)
{
	QImage pic(filename);
	QPixmap pix(pic.width(),pic.height());
	pix.fill(Qt::transparent);

	QPainter p(&pix);
	p.drawImage(0,0,pic);
	p.end();

	return addTexture(pix);
}

QOpenGLTexture *Renderer::addTexture(const QPixmap& pix)
{
	QImage pic = pix.toImage();
	QOpenGLTexture *texture = new QOpenGLTexture(pic);
	if(!texture->textureId())
		throw GLException("Texture Loading Error",
						  "The texture for anonyme pix cannot be loaded");

	texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	texture->setMagnificationFilter(QOpenGLTexture::Linear);
	texturesList.append(texture);
	return texture;
}

void Renderer::initializeVBO()
{
	vbo.create();
	vbo.setUsagePattern( QGLBuffer::StaticDraw );
	if ( !vbo.bind() )
	{
		throw GLException("Vertex Buffer Object",
						  "Could not bind vertex buffer to the context");
	}
	QVector<Vertex3D> vertexArray = vertexList.toVector();
	/*foreach(Vertex3D v, vertexList){
		qDebug() << v.toString();
	}*/

	vbo.allocate( vertexArray.constData(), vertexList.length() * sizeof( Vertex3D ) );
	vbo.release();
	vertexArray.clear();
	vertexList.clear();
}

void Renderer::initializeVAO()
{

	vao.create();
	vao.bind();
	GL->glEnableVertexAttribArray(A_Position);
	GL->glEnableVertexAttribArray(A_Color);
	GL->glEnableVertexAttribArray(A_TexturePosition);
	GL->glEnableVertexAttribArray(A_Normal);

	vbo.bind();
	GL->glVertexAttribPointer(A_Position, 3, GL_FLOAT, GL_FALSE,
						  sizeof(Vertex3D), (GLvoid*) offsetof(Vertex3D, position));
	GL->glVertexAttribPointer(A_Color, 3, GL_FLOAT, GL_FALSE,
						  sizeof(Vertex3D),  (GLvoid*) offsetof(Vertex3D, color));
	GL->glVertexAttribPointer(A_TexturePosition, 2, GL_FLOAT, GL_FALSE,
						  sizeof(Vertex3D),  (GLvoid*) offsetof(Vertex3D, texCoord));
	GL->glVertexAttribPointer(A_Normal, 3, GL_FLOAT, GL_FALSE,
						  sizeof(Vertex3D), (GLvoid*) offsetof(Vertex3D, normal));
	vbo.release();

	vao.release();

}

void Renderer::initRender()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	vao.bind();
	currentMVMatrix = globalTransform;
	currentMVPMatrix = projectionMatrix * currentMVMatrix;
	bindShader(Minimal);
}

void Renderer::endRender()
{
	vao.release();
	bindShader(NoShader);
}

void Renderer::bindShader(Renderer::ShaderType shaderType)
{
	if(shaderType != currentShaders){
		if(currentShaders>=0){
			shaders[currentShaders].release();
		}
		currentShaders = shaderType;
		if(currentShaders>=0){
			shaders[currentShaders].bind();
			setCurrentShaderUniformValue();
		}
	}
}

void Renderer::bindDirectionalLight(const DirectionalLight &light)
{
	int location = shaders[currentShaders].uniformLocation("directionalLightDir");
	if(location>=0)
		shaders[currentShaders].setUniformValue(location,light.direction);
	location = shaders[currentShaders].uniformLocation("directionalLightColor");
	if(location>=0)
		shaders[currentShaders].setUniformValue(location,light.color);
}

void Renderer::releaseDirectionalLight()
{
	int location = shaders[currentShaders].uniformLocation("directionalLightColor");
	if(location>=0)
		shaders[currentShaders].setUniformValue(location,QColor(0,0,0));
}

void Renderer::bindTorchLight(const TorchLight &light)
{
	int location = shaders[currentShaders].uniformLocation("torchlightPower");
	if(location>=0)
		shaders[currentShaders].setUniformValue(location,light.power);
	location = shaders[currentShaders].uniformLocation("torchlightColor");
	if(location>=0)
		shaders[currentShaders].setUniformValue(location,light.color);
}

void Renderer::releaseTorchLight()
{
	int location = shaders[currentShaders].uniformLocation("torchlightPower");
	if(location>=0)
		shaders[currentShaders].setUniformValue(location,0.f);
}

void Renderer::bindCamera(const Camera &cam)
{
	currentMVMatrix = cam.getViewMatrix() * globalTransform;
	currentMVPMatrix = projectionMatrix * currentMVMatrix;
	setCurrentShaderUniformValue();
}

void Renderer::releaseCamera()
{
	currentMVMatrix = globalTransform;
	currentMVPMatrix = projectionMatrix * currentMVMatrix;
	setCurrentShaderUniformValue();
}

void Renderer::draw(const Object3D *o)
{
	shaders[currentShaders].setUniformValue("textureMode", o->texture != NULL);
	switch(currentShaders){
	case Minimal:
		minimalDraw(o);
		break;
	case Normal:
		normalDraw(o);
		break;
	case Advanced:
		advancedDraw(o);
		break;
	default:
		break;
	}
}

void Renderer::draw(const QList<Object3D *> &list)
{
	switch(currentShaders){
	case Minimal:
		foreach(Object3D* o, list){
			shaders[currentShaders].setUniformValue("textureMode", o->texture != NULL);
			minimalDraw(o);
		}
		break;
	case Normal:
		foreach(Object3D* o, list){
			shaders[currentShaders].setUniformValue("textureMode", o->texture != NULL);
			normalDraw(o);
		}
		break;
	case Advanced:
		foreach(Object3D* o, list){
			shaders[currentShaders].setUniformValue("textureMode", o->texture != NULL);
			advancedDraw(o);
		}
		break;
	}
}

void Renderer::advancedDraw(const Object3D *)
{}

void Renderer::normalDraw(const Object3D *o)
{
	QMatrix4x4 MV = currentMVMatrix * o->transformM;
	shaders[currentShaders].setUniformValue("objectMVPMatrix", projectionMatrix * MV);
	shaders[currentShaders].setUniformValue("objectMVMatrix", MV);
	shaders[currentShaders].setUniformValue("objectNormalMatrix", MV.inverted().transposed());
	shaders[currentShaders].setUniformValue("objectDiffuseColor", o->diffuseColor);
	shaders[currentShaders].setUniformValue("objectAmbiantColor", o->ambiantColor);
	shaders[currentShaders].setUniformValue("objectSpecularColor",o->specularColor);
	o->draw();
}

void Renderer::minimalDraw(const Object3D *o)
{
	QMatrix4x4 MVP = currentMVPMatrix*o->transformM;
	shaders[currentShaders].setUniformValue("objectMVPMatrix", MVP);
	o->draw();
}

void Renderer::clearVBO()
{
	vbo.destroy();
}

void Renderer::clearVAO()
{
	vao.destroy();
}

void Renderer::clearTextures()
{
	foreach(QOpenGLTexture* texture, texturesList){
		texture->destroy();
	}
	texturesList.clear();
}

void Renderer::setPerspective(float verticalAngle, float aspectRatio,
						 float nearPlane, float farPlane)
{
	projectionMatrix.setToIdentity();
	projectionMatrix.perspective(verticalAngle, aspectRatio, nearPlane, farPlane);
	projectionType = Perspective;
	currentMVPMatrix = projectionMatrix * currentMVMatrix;
}

void Renderer::setOrtho(float left, float right,
				   float bottom, float top,
				   float nearPlane, float farPlane)
{
	projectionMatrix.setToIdentity();
	projectionMatrix.ortho(left, right, bottom, top, nearPlane, farPlane);
	projectionType = Ortho;
	currentMVPMatrix = projectionMatrix * currentMVMatrix;
}

void Renderer::setFrustum(float left, float right,
					 float bottom, float top,
					 float nearPlane, float farPlane)
{
	projectionMatrix.setToIdentity();
	projectionMatrix.frustum(left, right, bottom, top, nearPlane, farPlane);
	projectionType = Frustrum;
	currentMVPMatrix = projectionMatrix * currentMVMatrix;
}

void Renderer::prepareShaderProgram(QGLShaderProgram& program,
									const QString &vertexShaderPath,
									const QString &fragmentShaderPath)
{
	bool result = program.addShaderFromSourceFile( QGLShader::Vertex, vertexShaderPath );
	if ( !result )
		throw GLException("Shader Load Error", program.log());

	result = program.addShaderFromSourceFile( QGLShader::Fragment, fragmentShaderPath );
	if ( !result )
		throw GLException("Shader Load Error", program.log());

	// …and finally we link them to resolve any references.
	result = program.link();
	if ( !result )
		throw GLException("Shader Link Error", program.log());
}

void Renderer::setIndex(Object3D &o)
{
	o.indexStart = vertexList.length();
	o.indexCount = o.vertex.length();
}

Object3D Renderer::makeColoredFace(float rotationX, float rotationY, float rotationZ,
								   float translationX, float translationY, float translationZ,
								   QColor color, float scale)
{
	QList<Vertex3D> vertex = { Vertex3D(-scale,-scale,0,color.redF(),color.greenF(),color.blueF()),
							   Vertex3D(-scale,scale,0,color.redF(),color.greenF(),color.blueF()),
								 Vertex3D(scale,-scale,0,color.redF(),color.greenF(),color.blueF()),
							   Vertex3D(scale,scale,0,color.redF(),color.greenF(),color.blueF()) };
	QList<Vertex3D>::iterator i;
	QMatrix4x4 finalRotationMat;
	finalRotationMat.rotate(rotationX,1,0,0);
	finalRotationMat.rotate(rotationY,0,1,0);
	finalRotationMat.rotate(rotationZ,0,0,1);
	finalRotationMat.translate(translationX, translationY, translationZ);
	QMatrix4x4 normalRotationMat;
	normalRotationMat = finalRotationMat.inverted().transposed();


	for(i=vertex.begin();i!=vertex.end();i++){
		(*i).normal = toAiVector(normalRotationMat * QVector3D(0,0,1));
		(*i).position = toAiVector(toQVector((*i).position) * finalRotationMat);
	}

	Object3D o(vertex,Qt::white,GL_TRIANGLE_STRIP);
	addObject(o);
	return o;
}

Object3D Renderer::makeTexturedFace(float rotationX, float rotationY, float rotationZ,
									  float translationX, float translationY, float translationZ,
									  float scale, float repeat)
{
	//repeat = 1./repeat;
	QList<Vertex3D> vertex = { Vertex3D(-scale,-scale,0,0,0), Vertex3D(-scale,scale,0,0,repeat),
								 Vertex3D(scale,-scale,0,repeat,0), Vertex3D(scale,scale,0,repeat,repeat) };
	QList<Vertex3D>::iterator i;
	QMatrix4x4 finalRotationMat;
	finalRotationMat.rotate(rotationX,1,0,0);
	finalRotationMat.rotate(rotationY,0,1,0);
	finalRotationMat.rotate(rotationZ,0,0,1);
	finalRotationMat.translate(translationX, translationY, translationZ);
	QMatrix4x4 normalRotationMat;
	normalRotationMat = finalRotationMat.inverted().transposed();


	for(i=vertex.begin();i!=vertex.end();i++){
		(*i).normal = toAiVector(normalRotationMat * QVector3D(0,0,1));
		(*i).position = toAiVector(toQVector((*i).position) * finalRotationMat);
	}


	Object3D o(vertex,Qt::white,GL_TRIANGLE_STRIP);
	addObject(o);
	return o;
}

void Renderer::setCurrentShaderUniformValue()
{
	int uniformLocation = shaders[currentShaders].uniformLocation("globalLight");
	if(uniformLocation>=0)
		shaders[currentShaders].setUniformValue(uniformLocation, globalIllumination);

	uniformLocation = shaders[currentShaders].uniformLocation("ambiantLight");
	if(uniformLocation>=0)
		shaders[currentShaders].setUniformValue(uniformLocation, ambiantLight);
}
