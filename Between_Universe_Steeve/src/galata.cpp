#include "galata.h"
#include "player.h"
#include "math.h"
#include <QtXml>
#include <QImage>

QImage Galata::spaceBorder(":/images/border.png");

Galata::Galata(Player *p1)
	: _initialShipPosition(5,60), nbPlayer(0), end(false),
	  _break(false), tEnd(0), tInc(0.01f), opacity(1)
{
	_renderer = new GalataRenderer(this);
	if(p1 != NULL)
		addPlayer(p1);
}

Galata::Galata(QFile &file, Player *p1)
	: Galata(NULL)
{
	load(file);
	if(p1 != NULL)
		addPlayer(p1);
}

void Galata::addPlayer(Player *p)
{
	this->players.append(p);
	p->setLevel(this);
	p->addShipToGalata();
	nbPlayer++;
}

bool Galata::inputHandle(QInputEvent* event)
{
	bool _return;
	if(event->type() == QEvent::KeyRelease){
		if(dynamic_cast<QKeyEvent*>(event)->key() == Qt::Key_P){
			_break = !_break;
			return true;
		}
	}

	foreach (Player* p, players) {
		_return |= p->inputHandle(event);
	}
	return _return;
}

bool Galata::update()
{
	if(nbPlayer && !_break){
		if(!end){
			if(nbPlayer>1){
				if (players.length()==1){
					this->end = true;
				}
			}
			_space.animate();
			if(_space.isEnded()){
				opacity -= 0.02f;
				if(opacity<0.5){
					this->end = true;;
				}
			}
			return true;
		}
		else{
			opacity -= 0.01f;
			if(opacity<0){
				opacity=0;
			}
			tEnd += tInc;
			if(tEnd > 1.5){
				//change Mode
			}
			return true;
		}
	}
	return _break;
}

bool Galata::updateStartTransition(float t)
{
	opacity = t;
	return true;
}

bool Galata::updateEndTransition(float t)
{
	opacity = 1 - t;
	return true;
}

QPointF Galata::initialShipPosition()
{
	return _initialShipPosition;
}

void Galata::addShip(Ship *ship)
{
	ship->assert();
	_space.addBlockedObject(ship);
	ship->moveTo(_space.viewBound().topLeft() + initialShipPosition());
	_space.addObject(ship);
}

void Galata::addEnemy(Enemy *e)
{
	_space.addObject(e);
}

void Galata::addItem(Item *i)
{
	_space.addObject(i);
}

void Galata::handlePlayerDeath(Player *p)
{
	if(this->players.length()>1){
		this->players.removeOne(p);
	}
	else{
		this->end = true;
	}
}

void Galata::load(QFile &file)
{
	if(!file.open(QIODevice::ReadOnly |
				  QIODevice::Text)){
		throw std::exception(QString("The file " + file.fileName() +
							 " can't be read\n" + file.errorString())
							 .toStdString().c_str());
	}
	QDomDocument d;
	QDomElement root;
	try{
		if(!file.seek(0))
			throw std::exception(QString("error while reading the file " +
										 file.fileName() +
										" : " + file.errorString())
										.toStdString().c_str());
		QByteArray content = file.readAll();
		d.setContent(content);

		root = d.documentElement();
		_initialShipPosition = QPointF(root.attribute("initialx","0").toFloat(),
									  root.attribute("initialy","0").toFloat());

		_space = Space(QImage(":/" + root.attribute("background","images/background.png")),
					   root.attribute("x","0").toFloat(),
					   root.attribute("width","700").toFloat(),
					   root.attribute("height","500").toFloat(),
					   root.attribute("xend","2000").toFloat(),
					   root.attribute("velocity","1").toFloat());
		QDomNodeList nodes = root.childNodes();
		for(int i=0; i<nodes.length();i++)
		{
			Galata::loadElement(nodes.at(i).toElement());
		}
	}
	catch (QXmlParseException& e){
		file.close();
		throw std::exception(QString("An error has been met with the file "
							 + file.fileName() + " at:" + e.lineNumber() +
							 ":" + e.columnNumber() +"\n"+ e.message())
							 .toStdString().c_str());
	}
	file.close();

}



void Galata::loadElement(const QDomElement &e)
{
	if(e.tagName() == "enemy"){
		Enemy* enemy = Galata::loadEnemy(e);
		if(enemy){
			Galata::loadEnemySupplement(enemy,e);
			addEnemy(enemy);
		}
	}
	else if(e.tagName() == "item"){
		Item* item = Galata::loadItem(e);
		if(item)
			addItem(item);
	}
}

const Space &Galata::space()
{
	return _space;
}

Galata::~Galata()
{

	delete _renderer;
}

Enemy *Galata::loadEnemy(const QDomElement &e)
{
	QString enemyType = e.attribute("type");
	Enemy* enemy = NULL;
	int life = e.attribute("life","5").toInt();
	float damage = e.attribute("damage","1").toFloat();
	QPointF p = QPointF(e.attribute("x","0").toFloat(),
						e.attribute("y","0").toFloat());
	QColor c(e.attribute("r","0").toInt(),
			 e.attribute("g","255").toInt(),
			 e.attribute("b","0").toInt());
	if(enemyType=="circle"){
		if(e.hasAttribute("sprite"))
			enemy = new CircleEnemy(life,damage,e.attribute("radius","10").toFloat(),
									QImage(":/" + e.attribute("sprite")),
									loadRange(e),p);
		else
			enemy = new CircleEnemy(life, damage,e.attribute("radius","10").toFloat(),
									c,e.hasAttribute("fill"), loadRange(e), p);
	}
	else if(enemyType == "rect"){
		if(e.hasAttribute("sprite"))
			enemy = new SquareEnemy(life,damage,
									QSizeF(e.attribute("width","10").toFloat(),
									   e.attribute("height","10").toFloat()),
									QImage(":/" + e.attribute("sprite")),
									loadRange(e),p);
		else
			enemy = new SquareEnemy(life,damage,
									QSizeF(e.attribute("width","10").toFloat(),
									   e.attribute("height","10").toFloat()),
									c,e.hasAttribute("fill"),
									loadRange(e),p);
	}
	return enemy;
}

PositionRange Galata::loadRange(const QDomElement &e)
{
	QPointF p1 = QPointF(e.attribute("rangex1","0").toFloat(),
						 e.attribute("rangey1","0").toFloat());
	QString defaultX2 = QString::number(p1.x() + _space.viewBound().width());
	QString defaultY2 = QString::number(p1.y() + _space.viewBound().height());
	QPointF p2 = QPointF(e.attribute("rangex2",defaultX2).toFloat(),
								 e.attribute("rangey2",defaultY2).toFloat());
	return PositionRange(p1,p2);
}

Item *Galata::loadItem(const QDomElement &e)
{
	QString itemType = e.attribute("type");
	Item* item = NULL;
	int life = e.attribute("life","60").toInt();
	QPointF p = QPointF(e.attribute("x","0").toFloat(),
						e.attribute("y","0").toFloat());
	if(itemType=="up"){
		item = new I_LevelUp(life,p, loadRange(e));
	}
	else if(itemType == "shield"){
		item = new I_Shield(life,p, loadRange(e));
	}
	return item;
}

Behavior *Galata::loadBehavior(const QDomElement &e)
{
	Behavior* b = NULL;
	QString type = e.attribute("type");
	if(type=="sinusoidal"){
		b = new B_sinusoidal(e.attribute("phase","0.5").toFloat(),
							 e.attribute("module","10").toFloat(),
							 e.attribute("coefcos","1").toFloat(),
							 e.attribute("coefsin","1").toFloat());
	}
	else if(type.contains("string")){
		B_String* bs;
		if(type.endsWith("cycle"))
			bs = new B_StringCycle(e.attribute("delaybetween","0").toInt());
		else
			bs = new B_String();
		QDomNodeList nodes = e.childNodes();
		QDomElement e2;
		for(int i=0; i<nodes.length();i++)
		{
			e2 = nodes.at(i).toElement();
			bs->addBehavior(loadBehavior(nodes.at(i).toElement()),
							e.attribute("duration","60").toInt());
		}
		b = bs;
	}
	else if(type == "translate"){
		b = new B_Translate(QPointF(e.attribute("x","0").toFloat(),
									e.attribute("y","0").toFloat()));
	}
	else if(type == "translatealt"){
		b = new B_TranslateAlternate(e.attribute("start","0").toInt(),
										e.attribute("end","60").toInt(),
									 QPointF(e.attribute("x","0").toFloat(),
									 e.attribute("y","0").toFloat()));
	}
	else if(type=="rotate"){
		b = new B_TurnAround(e.attribute("speed","0.1").toFloat(),
							 e.attribute("distance","10").toFloat(),
							 e.attribute("initialangle","0").toFloat());
	}
	if(b){
		if(e.hasAttribute("delay")){
			if(e.hasAttribute("cycle"))
				b = new B_TemporalCycle(b,0,e.attribute("delay","60").toInt(),
										e.attribute("cycle","90").toInt());
			else
				b = new B_Temporal(b,0,e.attribute("delay","60").toInt());
		}
	}
	return b;
}

Galata::GalataRenderer::GalataRenderer(Galata *p) : parent(p)
{

}

void Galata::loadEnemySupplement(Enemy* enemy,
								 const QDomElement &e)
{
	QDomNodeList nodes = e.childNodes();
	for(int i=0; i<nodes.length();i++)
	{
		const QDomElement& child = nodes.at(i).toElement();
		QString tag = child.tagName();
		if(tag=="behavior"){
			Behavior* b = loadBehavior(child);
			if(b)
				enemy->addBehavior(b);
		}
		else if(tag=="item"){
			Item* i = loadItem(child);
			if(i)
				enemy->addDroppedItems(i);
		}
	}
}

void Galata::GalataRenderer::drawCenteredMessage(QPainter &context,
													QString message,
													int pointSize,
													bool bold,
													QColor color) const{
	QFont f(context.font());
	QFontMetrics fontMetrics(f);
	int x,y;
	context.setPen(color);
	f = context.font();
	f.setPointSize(pointSize);
	f.setBold(bold);
	context.setFont(f);
	fontMetrics = QFontMetrics(f);
	x = context.window().width() * 0.5
			- fontMetrics.width(message) * 0.5;
	y = context.window().height() * 0.5
			- fontMetrics.height() * 0.5;
	context.drawText(x,y,message);
}

void Galata::GalataRenderer::drawCenteredMessage(QPainter &context,
												 QString message,
												 int pointSize,
												 bool bold,
												 QColor color,
												 int tx, int ty) const
{
	QFont f(context.font());
	QFontMetrics fontMetrics(f);
	context.setPen(color);
	f = context.font();
	f.setPointSize(pointSize);
	f.setBold(bold);
	context.setFont(f);
	fontMetrics = QFontMetrics(f);
	tx += context.window().width() * 0.5
			- fontMetrics.width(message) * 0.5;
	ty += context.window().height() * 0.5
			- fontMetrics.height() * 0.5;
	context.drawText(tx,ty,message);
}

void Galata::GalataRenderer::paint(QPainter &context) const
{
	QFont f(context.font()); QFontMetrics fontMetrics(f);
	int x, y1, yInc;
	context.save();
	context.fillRect(context.viewport(),QColor(0,0,0));
	context.setOpacity(parent->opacity);
	QRectF bound = parent->_space.viewBound();
	context.resetMatrix();
	context.resetTransform();


	parent->_space.applyTransform(context, context.window().width(), context.window().height()-50);

	context.setPen(QPen(QColor(20,25,28),3));
	context.drawRect(QRect(0,0,bound.width(),bound.height()));
	context.setPen(QPen(QColor(30,35,38),1));
	context.drawRect(QRect(0,0,bound.width(),bound.height()));

	parent->_space.paint(context);
	context.restore();
	f.setPointSize(13);
	context.setFont(f);
	fontMetrics = QFontMetrics(f);
	x = 10;
	yInc = fontMetrics.height()*1.2;
	int j = 0;
	foreach(Player* p, parent->players){
		if(j>=(parent->players.length()+1/2))
			y1 = context.window().height()-35;
		else
			y1 = fontMetrics.height() * 1.5;
		context.setPen(p->color());
		context.drawText(x,y1, p->name());
		y1+=yInc;
		context.drawText(x,y1, QString::number(p->points()));
		y1+=yInc*0.5;
		context.setBrush(p->color());
		for(int i=0; i<p->life();i++)
			context.drawEllipse(x+i*10,y1,6,6);
		x += std::max<int>(fontMetrics.width(p->name()),
						   fontMetrics.width(QString::number(p->points()))) +40;
		j++;
	}
	if(parent->end){
		if(parent->nbPlayer>1)
		{
			QList<Player*>::ConstIterator playerI = parent->players.constBegin();
			Player* winner = (*playerI);
			playerI++;
			while(playerI != parent->players.constEnd()){
				if((*playerI)->points() > winner->points())
					winner = (*playerI);
				playerI++;
			}

			if(((int)(parent->tEnd*1000))%180<100){
				drawCenteredMessage(context,
									winner->name() + " WINS!",
									40, true, winner->color());
			}

		}
		else{
			if(((int)(parent->tEnd*1000))%180<100){
				if(parent->_space.isEnded())
					drawCenteredMessage(context, "END OF LEVEL",
									40, true, QColor(60,240,180));
				else
					drawCenteredMessage(context,"GAME OVER",
										40, true, QColor(240,600,180));

			}
			if(parent->tEnd>0.5){
				drawCenteredMessage(context,
									parent->players.first()->name() + " : " +
									QString::number(parent->players.first()->points()),
									40, true,
									parent->players.first()->color(),0,60);
			}
		}
	} else if(parent->_break){
		drawCenteredMessage(context,
							"PAUSE",
							40, true, QColor(214,230,180));
	}
}
