#include "camera.h"
#include "tools.h"


void Camera::computeDirectionVectors()
{
	m_FrontVector = QVector3D(cosf(m_fTheta)*sinf(m_fPhi),
								sinf(m_fTheta),
								cosf(m_fTheta)*cosf(m_fPhi));
	m_LeftVector = QVector3D(sinf(m_fPhi+(PI_f*0.5f)),
								0,
								cosf(m_fPhi+(PI_f*0.5f)));
	m_UpVector = QVector3D::crossProduct(m_FrontVector,m_LeftVector);
}

Camera::Camera()
{
	reset();
}

void Camera::moveLeft(float t)
{
    m_Position += m_LeftVector*t;
}

void Camera::moveFront(float t)
{
	m_Position += m_FrontVector*t;
}

void Camera::translateUp(float dy)
{
	m_Position += QVector3D(0,dy,0);
}

void Camera::rotateLeft(float radian)
{
	m_fPhi += radian;
    computeDirectionVectors();
}

void Camera::rotateUp(float radian)
{
	m_fTheta += radian;
	computeDirectionVectors();
}

void Camera::reset()
{
	m_Position = QVector3D(0,0,0);
	m_fPhi = PI_f;
	m_fTheta = 0;
	computeDirectionVectors();
}

float Camera::rotationLeft() const
{
	return m_fPhi;
}

float Camera::rotationTop() const
{
	return m_fTheta;
}

void Camera::setPosition(const QVector3D& pos)
{
	m_Position = pos;
}

const QVector3D &Camera::position() const
{
	return m_Position;
}

const QVector3D &Camera::frontVector() const
{
	return m_FrontVector;
}

QMatrix4x4 Camera::getViewMatrix() const
{
	QMatrix4x4 m;
	m.lookAt(m_Position, m_Position + m_FrontVector, m_UpVector);
	return m;
}
