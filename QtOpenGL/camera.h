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

	void setPosition(const QVector3D &pos);

	inline float x() const { return m_Position.x(); }
	inline float y() const { return m_Position.y(); }
	inline float z() const { return m_Position.z(); }

	const QVector3D& position() const;
	const QVector3D& frontVector() const;

	QMatrix4x4 getViewMatrix() const;

	static inline QVector3D computeFrontVector(float angleY){
		return QVector3D(sinf(angleY),0,cosf(angleY));
	}
};

#endif //FREEFLY_CAMERA_H
