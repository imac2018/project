#include <QMouseEvent>

#include "gui.h"
#include "game.h"
#include "mode.h"

QPixmap makeGrayPicture(const QPixmap& pix){
	QImage temp = pix.toImage();
	for(int i=0;i<temp.width();i++)
		for(int j=0;j<temp.height();j++){
			temp.setPixel(i,j,qGray(temp.pixel(i,j)));
		}
	return QPixmap::fromImage(temp);
}

void drawText(QPainter& painter, const QString& imagePath,
			  const QString& label, int width, int height){
	QFont font = QFont("Arial",height*0.5f);
	QFontMetrics fm(font);
	painter.drawImage(0,0,QImage(imagePath).scaled(width,
													height,
													Qt::IgnoreAspectRatio));
	painter.setPen(QColor(40,20,13));
	painter.setFont(font);

	painter.drawText( QPoint(width/2 - fm.width(label)/2,
							  height/2 + fm.height()/3), label );
}

void Button::makeButtonTexture(Renderer &renderer, const QString& title, float ratio)
{
	QPixmap text(1280,1280*ratio);
	text.fill(Qt::transparent);
	QPainter painter(&text);
	drawText(painter, ":/assets/button.png",title, text.width(), text.height());
	texture = renderer.addTexture(text);
	textureInactiv = renderer.addTexture(makeGrayPicture(text));
	drawText(painter, ":/assets/button_hover.png",title, text.width(), text.height());
	textureHover = renderer.addTexture(text);
	drawText(painter, ":/assets/button_pressed.png",title, text.width(), text.height());
	texturePressed = renderer.addTexture(text);
}

Button::Button(Renderer& r, QString title, QRectF bounds)
	: Object3D({Vertex3D(bounds.left(),bounds.top(),0,0,0),
				 Vertex3D(bounds.left(),bounds.bottom(),0,0,1),
				 Vertex3D(bounds.right(),bounds.top(),0,1,0),
				 Vertex3D(bounds.right(),bounds.bottom(),0,1,1)},
				 Qt::white, GL_TRIANGLE_STRIP), title(title),
		pressed(false), hover(false), inactiv(false),
		invisible(false), bounds(bounds)
{
	makeButtonTexture(r,title, bounds.height() / bounds.width());

}

bool Button::containsPoint(QPointF point)
{
	return bounds.contains(point);
}

void Button::pressedEvent()
{
	pressed = true;
}

void Button::releaseEvent(bool action)
{
	pressed = false;
	if(action)
		this->action();
}

void Button::hoverEvent()
{
	hover = true;
}

void Button::leaveEvent()
{
	hover = false;
}

void Button::draw() const
{
	if(pressed){
		if(texturePressed)
		{
			texturePressed->bind();
			glDrawArrays(drawMode, indexStart, indexCount);
			texturePressed->release();
			return;
		}
	}
	else if(hover){
		if(textureHover)
		{
			textureHover->bind();
			glDrawArrays(drawMode, indexStart, indexCount);
			textureHover->release();
			return;
		}
	}
	if(texture){
		texture->bind();
		glDrawArrays(drawMode, indexStart, indexCount);
		texture->release();
	}
	else
		glDrawArrays(drawMode, indexStart, indexCount);
}

void Button::action()
{}

Label::Label(Renderer &r, QString title, QRectF bounds)
	: Object3D({Vertex3D(bounds.left(),bounds.top(),0,0,0),
				 Vertex3D(bounds.left(),bounds.bottom(),0,0,1),
				 Vertex3D(bounds.right(),bounds.top(),0,1,0),
				 Vertex3D(bounds.right(),bounds.bottom(),0,1,1)},
				 Qt::white, GL_TRIANGLE_STRIP),
	  text(title), bounds(bounds)
{
	QPixmap text(1280,1280*bounds.height() / bounds.width());
	text.fill(Qt::transparent);
	QPainter painter(&text);
	drawText(painter, ":/assets/button.png",title, text.width(), text.height());
	texture = r.addTexture(text);
}

ChangeModeButton::ChangeModeButton(Game &g, Mode *next, Renderer &r,
								   QString title, QRectF bounds)
	: Button(r,title,bounds), next(next), game(g)
{}

void ChangeModeButton::action(){
	game.changeMode(next);
}

Gui::Gui()
	: hoveredButton(NULL), pressedButton(NULL)
{

}

void Gui::append(Button *newBtn)
{
	buttons.append(newBtn);
}

bool Gui::handleMouseMove(QMouseEvent *mouseE)
{
	if(hoveredButton)
	{
		if(hoveredButton->containsPoint(mouseE->localPos())){
			return true;
		}
		else{
			hoveredButton->leaveEvent();
			hoveredButton = NULL;
		}
	}
	QList<Button*>::iterator i;
	for(i=buttons.begin();i!=buttons.end();i++){
		if((*i)->containsPoint(mouseE->localPos())){
			(*i)->hoverEvent();
			hoveredButton = (*i);
			return true;
		}
	}
	return false;
}

bool Gui::handleMousePress(QMouseEvent *mouseE)
{
	QList<Button*>::iterator i;
	for(i=buttons.begin();i!=buttons.end();i++){
		if((*i)->containsPoint(mouseE->localPos())){
			(*i)->pressedEvent();
			pressedButton = (*i);
			return true;
		}
	}
	return false;
}

bool Gui::handleMouseRelease(QMouseEvent *mouseE)
{
	if(pressedButton){
		if(pressedButton->containsPoint(mouseE->localPos())){
			pressedButton->releaseEvent(true);
		}
		else
			pressedButton->releaseEvent(false);
		pressedButton = NULL;
		return true;
	}
	return false;
}

QList<Object3D *> Gui::objects() const
{
	QList<Object3D *> list;
	foreach(Button* b, buttons){
		list.append(b);
	}
	return list;
}

Image::Image(Renderer &r, QRectF bounds, const QString& imagePath)
	: Object3D({Vertex3D(bounds.left(),bounds.top(),0,0,0),
			   Vertex3D(bounds.left(),bounds.bottom(),0,0,1),
			   Vertex3D(bounds.right(),bounds.top(),0,1,0),
			   Vertex3D(bounds.right(),bounds.bottom(),0,1,1)},
			   Qt::white, GL_TRIANGLE_STRIP),
		bounds(bounds)
{
	texture = r.addTexture(imagePath);
}
