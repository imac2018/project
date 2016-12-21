#include "weapon.h"
#include "enemy.h"
#include "ship.h"
#include "math.h"

void Weapon::shootLaser1(Weapon& w, BulletCollisionListener&  parent, Space &world,
						   QPointF origin){
	world.addObject(new LaserBullet(parent, 1, 400,15,60,0, w.position + origin));
}
void Weapon::shootLaser2(Weapon& w,BulletCollisionListener&  parent, Space &world,
						   QPointF origin){
	world.addObject(new LaserBullet(parent, 1.1f, 400,17,70,0, w.position + origin));
}
void Weapon::shootLaser3(Weapon& w,BulletCollisionListener&  parent, Space &world,
						   QPointF origin){
	world.addObject(new LaserBullet(parent, 0.8f, 250,14,55,10, w.position + origin));
	world.addObject(new LaserBullet(parent, 1.1f,400,17,70,0, w.position + origin));
	world.addObject(new LaserBullet(parent, 0.8f, 250,14,55,-10, w.position + origin));
}
void Weapon::shootLaser4(Weapon& w,BulletCollisionListener&  parent, Space &world,
						   QPointF origin){
	world.addObject(new LaserBullet(parent,0.9f, 400,17,70,12, w.position + origin));
	world.addObject(new LaserBullet(parent,1.2f, 400,23,80,0, w.position + origin));
	world.addObject(new LaserBullet(parent,0.9f, 400,17,70,-12, w.position + origin));
}

void Weapon::shootWave1(Weapon& w, BulletCollisionListener&  parent, Space &world,
						   QPointF origin){
	world.addObject(new WaveBullet(parent,0.5f, 100,5,5,40,0, 2.f, w.position + origin));
}
void Weapon::shootWave2(Weapon& w,BulletCollisionListener&  parent, Space &world,
						   QPointF origin){
	world.addObject(new WaveBullet(parent,0.75f, 120,9,6,40,0,2.75f, w.position + origin));
}
void Weapon::shootWave3(Weapon& w,BulletCollisionListener&  parent, Space &world,
						   QPointF origin){
	world.addObject(new LaserBullet(parent,0.75f, 250,14,60,10, w.position + origin));
	world.addObject(new WaveBullet(parent,0.8f, 150,9,6,50,0,3.f, w.position + origin));
	world.addObject(new LaserBullet(parent,0.75f, 250,14,60,-10, w.position + origin));
}
void Weapon::shootWave4(Weapon& w,BulletCollisionListener&  parent, Space &world,
						   QPointF origin){
	world.addObject(new LaserBullet(parent,0.75f, 250,14,60,10, w.position + origin));
	world.addObject(new WaveBullet(parent,1.0f, 200,10,7,55,0,3.5f, w.position + origin));
	world.addObject(new LaserBullet(parent,0.75f, 250,14,60,10, w.position + origin));
}

Weapon::Weapon(QString name, QPointF position, QList<ShootFunction> shootFonctions)
	: level(1), _activated(false), currentDelay(0),
	  delayMax(12), position(position), _name(name),
	  levelMax(shootFonctions.length()+1),
	  shootFonctions(shootFonctions)
{
	this->currentShootFunction = this->shootFonctions.begin();
}

Weapon::Weapon(QString name, QPointF position, int levelMax, ShootFunction shootFonctions[])
	: Weapon(name, position,QList<ShootFunction>())
{
	this->levelMax = levelMax;
	for(int i=0;i<levelMax;i++){
		this->shootFonctions.append(shootFonctions[i]);
	}
	this->currentShootFunction = this->shootFonctions.begin();
}

void Weapon::shoot(BulletCollisionListener& parent,Space &world, QPointF origin)
{
	if(!currentDelay){
		(*currentShootFunction)(*this, parent, world, origin);
		currentDelay = delayMax - level*2;
	}
	else
		currentDelay--;
}

void Weapon::activate()
{
	currentDelay = 0;
	_activated = true;
}

void Weapon::stop()
{
	_activated = false;
}

bool Weapon::activated()
{
	return _activated;
}

void Weapon::levelUp()
{
	if(level < levelMax && (currentShootFunction+1) != shootFonctions.end()){
		this->level++;
		this->currentShootFunction++;
	}
}

QString Weapon::name() const
{
	return _name;
}

Weapon *Weapon::instanciate(QPointF pos) const
{
	return new Weapon(_name, pos, shootFonctions);
}

Bullet::Bullet(BulletCollisionListener &parent, int life, QSizeF size)
	: SpaceObject(size), parent(parent), life(life)
{

}

void Bullet::animate()
{
	life -= 5;
}

bool Bullet::isDead() const{
	return life <= 0;
}

LaserBullet::LaserBullet(BulletCollisionListener& parent,
						 float damage, int life, float width,
						 float vx, float vy)
	: Bullet(parent, life, QSize(width,1)), line(Space::origin,QPointF(width,0)),
	  velocity(vx,vy)
{
	this->damage = std::min<float>(1.5,damage);
}

LaserBullet::LaserBullet(BulletCollisionListener &parent, float damage, int life, float width,
						 float vx, float vy,	 QPointF position)
	: LaserBullet(parent, damage, life, width, vx, vy)
{
	moveTo(position);
}

bool LaserBullet::isInView(float x1, float x2) const
{
	return actualPosition().x() + line.x1() < x2
			&& actualPosition().x() + line.x2() > x1;
}

void LaserBullet::paint(QPainter &context) const
{
	context.drawLine(line);
}

ColliderObject *LaserBullet::collider() const
{
	return new ColliderLine(QLine(line.x1(),line.y1(),
								  line.x2() + velocity.x() * 0.75,
								  line.y2() + velocity.y() * 0.75));
}

void LaserBullet::animate()
{
	Bullet::animate();
	moveTo(actualPosition() + velocity);
}

void LaserBullet::handleCollision(SpaceObject *o)
{
	if(parent.handleBulletCollision(o, damage)){
		if(this->life>5)
			this->life = 5;
		velocity =(o->actualPosition() - actualPosition()) * 0.5;
	}
}

WaveBullet::WaveBullet(BulletCollisionListener& parent,
					   float damage, int life, float width, float initialHeight,
					   float vx, float vy, float heightInc)
	: Bullet(parent, life,QSize(width,initialHeight)),
	  rect(Space::origin,QSize(width,initialHeight)),
	  velocity(vx,vy), inc(heightInc)
{
	this->damage = std::min<float>(2,damage);
}

WaveBullet::WaveBullet(BulletCollisionListener &parent, float damage,
					   int life, float width, float initialHeight,
						 float vx, float vy, float heightInc, QPointF position)
	: WaveBullet(parent, damage, life, width, initialHeight, vx, vy, heightInc)
{
	moveTo(position);
}

bool WaveBullet::isInView(float x1, float x2) const
{
	return actualPosition().x() + rect.left() < x2
			&& actualPosition().x() + rect.right() > x1;
}

void WaveBullet::paint(QPainter &context) const
{
	context.drawRect(rect);
}

ColliderObject *WaveBullet::collider() const
{
	return new ColliderRect(rect.translated(velocity * 0.75));
}

void WaveBullet::animate()
{
	Bullet::animate();
	rect.setTop(rect.top() - inc * 0.5);
	rect.setHeight(rect.height() + inc);
	_size += QSizeF(0,inc);
	moveTo(actualPosition() + velocity);
}

void WaveBullet::handleCollision(SpaceObject *o)
{
	if(parent.handleBulletCollision(o, damage)){
		if(this->life>5)
			this->life *= 0.5;
	}
}
