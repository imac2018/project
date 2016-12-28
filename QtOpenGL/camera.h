#ifndef FREEFLY_CAMERA_H
#define FREEFLY_CAMERA_H

#include <QMatrix4x4>

class Camera {
private:
	QVector3D m_Position;
    float m_fPhi;
    float m_fTheta;
	QVector3D m_FrontVector;
	QVector3D m_LeftVector;
	QVector3D m_UpVector;

    void computeDirectionVectors();

public:
	Camera();
    void moveLeft(float t);
    void moveFront(float t);

	void translateUp(float dy);

	void rotateLeft(float radian);
	void rotateUp(float radian);

	void reset();

	float rotationLeft() const;
	float rotationTop() const;

	inline float x() const { return m_Position.x(); }
	inline float y() const { return m_Position.y(); }
	inline float z() const { return m_Position.z(); }

	const QVector3D& position();
	const QVector3D& frontVector();

	QMatrix4x4 getViewMatrix() const;
};

#endif //FREEFLY_CAMERA_H
