#ifndef TOOLS_H
#define TOOLS_H

#include <assimp/types.h>
#include <QMatrix4x4>
#include <QVector3D>

#define PI_f 3.14159265359f
#define HALFPI_f 1.5707963267f
#define PI_d 3.1415926535897932384626433832795
#define toDegCoef 57.295779513f
#define isZero(x) (abs(x) < 0.0001f)

inline float toRadian(float degrees){ return degrees * 0.01745329252f; }

inline aiMatrix4x4 QMatrixToaiMatrix(const QMatrix4x4 &m)
{
	return aiMatrix4x4(m(0,0), m(0,1), m(0,2), m(0,3),
						m(1,0), m(1,1), m(1,2), m(1,3),
						m(2,0), m(2,1), m(2,2), m(2,3),
						m(3,0), m(3,1), m(3,2), m(3,3));
}

inline QVector3D toQVector(const aiVector3D &v){
	return QVector3D(v.x,v.y,v.z);
}
inline aiVector3D toAiVector(const QVector3D v){
	return aiVector3D(v.x(),v.y(),v.z());
}
inline aiVector3D toAiVector(const QVector4D v){
	return aiVector3D(v.x(),v.y(),v.z());
}

inline aiVector2D toAiVector2D(const aiVector3D &v){
	return aiVector2D(v.x,v.y);
}

inline aiColor3D toAiColor3D(const aiColor4D &c){
	return aiColor3D(c.r,c.g,c.b);
}

inline const float* value_ptr(const aiMatrix4x4& m){
	return &m.a1;
}
inline const float* value_ptr(const aiColor3D& m){
	return &m.r;
}
inline const float* value_ptr(const aiVector3D& m){
	return &m.x;
}


#endif // TOOLS_H
