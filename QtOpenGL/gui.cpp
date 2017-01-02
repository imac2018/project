#include <QMouseEvent>
#include <QApplication>

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
			  const QString& label, int width, int height, int fontsize,
			  QColor c = Qt::black, QRect textBound = QRect()){
	QFont font = QFont("Arial");
	font.setPointSize(fontsize);
	painter.drawImage(0,0,QImage(imagePath).scaled(width,
													height,
													Qt::IgnoreAspectRatio));
	painter.setPen(c);
	painter.setFont(font);
	if(textBound.isNull()){
		textBound = QRect(width*0.1,height*0.1,width*0.8,height*0.8);
	}
	painter.drawText( textBound,  Qt::AlignCenter | Qt::TextWordWrap, label );
}

void drawText(QPainter& painter, const QString& label, int width,
			  int height, int fontsize, QColor c = Qt::black, bool bold=false){
	QFont font = QFont("Arial");
	font.setPointSize(fontsize);
	font.setBold(bold);
	painter.setPen(c);
	painter.setFont(font);
	painter.drawText( QRectF(width*0.1,height*0.1,width*0.8,height*0.8),
					  Qt::AlignCenter | Qt::TextWordWrap, label );
}

void drawTextWithBorder(QPainter& painter, const QString& label, int width,
						int height, int fontsize, QColor c = Qt::black, int borderWidth = 2,
						QColor borderC = Qt::white,bool bold=false){
	QFont font = QFont("Arial");

	font.setPointSize(fontsize + 1);
	painter.setFont(font);
	font.setBold(bold);
	painter.setPen(borderC);
	painter.drawText( QRectF(width*0.1, height*0.1,width*0.8,height*0.8),
					  Qt::AlignCenter | Qt::TextWordWrap, label );

	font.setPointSize(fontsize);
	painter.setFont(font);
	painter.setPen(c);
	painter.drawText( QRectF(width*0.1,height*0.1,width*0.8,height*0.8),
										Qt::AlignCenter | Qt::TextWordWrap, label );
}

void Button::makeButtonTexture(Renderer &renderer, const QString& title,
							   float ratio, bool textonly, int fontsize)
{
	QPixmap text(720,720*ratio);
	text.fill(Qt::transparent);
	QPainter painter(&text);
	if(textonly){
		drawText(painter,title,text.width(),text.height(),fontsize,QColor("#ffdb01"));
		texture = renderer.addTexture(text);
		textureInactiv = renderer.addTexture(makeGrayPicture(text));
		drawText(painter,title,text.width(),text.height(),fontsize+2,QColor("#ffe050"),true);
		textureHover = renderer.addTexture(text);
		drawText(painter,title,text.width(),text.height(),fontsize-1,QColor("#ffd000"),false);
		texturePressed = renderer.addTexture(text);
	}
	else{
		drawText(painter, ":/assets/images/button.png",title,
				 text.width(), text.height(),fontsize, QColor(30,21,2));
		texture = renderer.addTexture(text);
		textureInactiv = renderer.addTexture(makeGrayPicture(text));
		drawText(painter, ":/assets/images/button_hover.png",title, text.width(),
				 text.height(),fontsize, QColor(30,21,2));
		textureHover = renderer.addTexture(text);
		drawText(painter, ":/assets/images/button_pressed.png",title, text.width(),
				 text.height(),fontsize, QColor(30,21,2));
		texturePressed = renderer.addTexture(text);
	}

}

void Button::makeButtonTexture(Renderer &renderer,
							   const QString &btnImagePath, float ratio)
{
	QPixmap text(720,720*ratio);
	text.fill(Qt::transparent);
	QPainter painter(&text);
	painter.drawImage(QRect(0,0,text.width(),text.height()),QImage(btnImagePath));
	texture = renderer.addTexture(text);
	textureInactiv = renderer.addTexture(makeGrayPicture(text));
	QString hoveredImage = btnImagePath;
	hoveredImage.replace(QRegExp("\\.(\\w+)"),"_hover.\\1");
	painter.drawImage(QRect(0,0,text.width(),text.height()),QImage(hoveredImage));
	textureHover = renderer.addTexture(text);
	hoveredImage.replace(QRegExp("_hover\\.(\\w+)"),"_pressed.\\1");
	painter.drawImage(QRect(0,0,text.width(),text.height()),QImage(hoveredImage));
	texturePressed = renderer.addTexture(text);
}

Button::Button(Renderer& r, QString title, QRectF bounds,
					bool textonly, int fontSize)
	: GuiElement({Vertex3D(bounds.left(),bounds.top(),0,0,0),
				 Vertex3D(bounds.left(),bounds.bottom(),0,0,1),
				 Vertex3D(bounds.right(),bounds.top(),0,1,0),
				 Vertex3D(bounds.right(),bounds.bottom(),0,1,1)},
				 Qt::white, GL_TRIANGLE_STRIP), title(title),
		pressed(false), hover(false), bounds(bounds)
{
	makeButtonTexture(r,title, bounds.height() / bounds.width(),textonly, fontSize);
}

Button::Button(Renderer &r, QString btnImagePath, QRectF bounds)
	: GuiElement({Vertex3D(bounds.left(),bounds.top(),0,0,0),
				 Vertex3D(bounds.left(),bounds.bottom(),0,0,1),
				 Vertex3D(bounds.right(),bounds.top(),0,1,0),
				 Vertex3D(bounds.right(),bounds.bottom(),0,1,1)},
				 Qt::white, GL_TRIANGLE_STRIP),
		pressed(false), hover(false), bounds(bounds)
{
	makeButtonTexture(r,btnImagePath, bounds.height() / bounds.width());
}

bool Button::containsPoint(QPointF point)
{
	if(invisible)
		return false;
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
	if(!invisible){
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
		GuiElement::draw();
	}
}

void Button::action()
{}

Label::Label(Renderer &r, QString title,
					QRectF bounds, int fontsize, QColor color,
					QColor border)
	: GuiElement({Vertex3D(bounds.left(),bounds.top(),0,0,0),
				 Vertex3D(bounds.left(),bounds.bottom(),0,0,1),
				 Vertex3D(bounds.right(),bounds.top(),0,1,0),
				 Vertex3D(bounds.right(),bounds.bottom(),0,1,1)},
				 Qt::white, GL_TRIANGLE_STRIP),
	  text(title), bounds(bounds)
{
	QPixmap text(720,720*bounds.height() / bounds.width());
	text.fill(Qt::transparent);
	QPainter painter(&text);
	drawTextWithBorder(painter,title, text.width(), text.height(),fontsize,
							color, 3, border);
	texture = r.addTexture(text);
}

ChangeModeButton::ChangeModeButton(Game& g, Mode* next, Renderer &r, QString title, QRectF bounds,
								   bool textonly, int fontSize)
	: Button(r,title,bounds,textonly,fontSize), next(next), game(g)
{}

ChangeModeButton::ChangeModeButton(Game &g, Mode *next, Renderer &r,
								   QString btnImagePath, QRectF bounds)
: Button(r,btnImagePath,bounds), next(next), game(g)
{}

void ChangeModeButton::action(){
	game.changeMode(next);
}

Gui::Gui()
	: hoveredButton(NULL), pressedButton(NULL)
{

}

void Gui::appendBtn(Button *newBtn)
{
	buttons.append(newBtn);
	appendElement(newBtn);
}

void Gui::appendElement(GuiElement *elemnt)
{
	guiElements.append(elemnt);
}

void Gui::appendTmpElement(int delay, GuiElement *element)
{
	temporaryElements.append(QPair<int,GuiElement*>(delay,element));
}

void Gui::addObjectsToRenderer(Renderer &r)
{
	foreach(GuiElement* o, guiElements){
		r.addObject(*o);
	}
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
	foreach(GuiElement* e, guiElements){
		list.append(e);
	}
	QPair<int,GuiElement*> te;
	foreach(te, temporaryElements){
		list.append(te.second);
	}
	foreach(Button* b, buttons){
		list.append(b);
	}
	return list;
}

void Gui::render(Game & game) const
{
	Renderer& renderer = game.renderer();
	glDisable(GL_DEPTH_TEST);
	renderer.projectionType = Renderer::Ortho;
	game.updateProjection();
	renderer.releaseCamera();
	renderer.bindShader(Renderer::Minimal);
	QList<Object3D *> objects;
	foreach(GuiElement* e, guiElements){
		objects.append(e);
	}
	QPair<int,GuiElement*> te;
	foreach(te, temporaryElements){
		objects.append(te.second);
	}
	renderer.draw(objects);
}

void Gui::update()
{
	QMutableListIterator<	QPair<int,GuiElement*> > i(temporaryElements);
	while(i.hasNext()){
		QPair<int,GuiElement*>& element = i.next();
		element.first--;
		if(element.first<0){
			i.remove();
		}
	}
}

void Gui::clear()
{
	QList<QPair<int,GuiElement*> >::Iterator i;
	for(i=temporaryElements.begin();i!=temporaryElements.end();i++){
		delete (*i).second;
	}
	temporaryElements.clear();
	QList<GuiElement*>::Iterator j;
	for(j=guiElements.begin();j!=guiElements.end();j++){
		delete (*j);
	}
	guiElements.clear();
	buttons.clear();
	hoveredButton = NULL;
	pressedButton = NULL;
}

Image::Image(Renderer &r, QRectF bounds, const QString& imagePath)
	: GuiElement({Vertex3D(bounds.left(),bounds.top(),0,0,0),
			   Vertex3D(bounds.left(),bounds.bottom(),0,0,1),
			   Vertex3D(bounds.right(),bounds.top(),0,1,0),
			   Vertex3D(bounds.right(),bounds.bottom(),0,1,1)},
			   Qt::white, GL_TRIANGLE_STRIP),
		bounds(bounds)
{
	texture = r.addTexture(imagePath);
}

GuiElement::GuiElement(QList<Vertex3D> vertex, const QColor &globalColor, int drawMode)
	: Object3D(vertex, globalColor, drawMode), invisible(false)
{}

void GuiElement::draw() const
{
	if(!invisible)
		Object3D::draw();
}

DialogFrame::DialogFrame(Renderer &r, QString title, QRectF bounds, int fontsize)
	: GuiElement({Vertex3D(-1,-1,0,0,0),
			   Vertex3D(-1,1,0,0,1),
			   Vertex3D(1,-1,0,1,0),
			   Vertex3D(1,1,0,1,1)},
			   Qt::white, GL_TRIANGLE_STRIP),
	  bounds(bounds)
{
	QPixmap text(720,720*bounds.height() / bounds.width());
	text.fill(Qt::transparent);
	QPainter painter(&text);
	drawText(painter, ":/assets/images/fenetre.png",title, text.width(),
			 text.height(), fontsize, QColor("#ffdb01"), QRect(120,0,440,text.height()));
	texture = r.addTexture(text);
}

ToggleDialogButton::ToggleDialogButton(Dialog* parent, bool show, Renderer &r,
									   QString title, QRectF bounds, bool textonly, int fontSize)
	: Button(r,title,bounds,textonly,fontSize), parent(parent), show(show)
{}

ToggleDialogButton::ToggleDialogButton(Dialog *parent, bool show, Renderer &r,
									   QString btnImagePath, QRectF bounds)
	: Button(r,btnImagePath,bounds), parent(parent), show(show)
{

}

void ToggleDialogButton::action()
{
	if(show)
		parent->show();
	else
		parent->hide();
}

Dialog::Dialog()
	: cancel(NULL), confirm(NULL), frame(NULL)
{}

QRectF Dialog::confirmBtnBounds() const
{
	return QRectF(0.17f,0.3f,0.23f,0.1f);
}

void Dialog::initialize(Renderer& renderer, Button *confirm, QString message)
{
	this->confirm = confirm;
	cancel = new ToggleDialogButton(this, false, renderer,"Cancel",
									QRectF(-0.1f,0.3f,0.2f,0.1f), true, defaultBtnFontSize);
	frame = new DialogFrame(renderer,message, QRectF(-0.5,-0.5,1,1),25);
}

void Dialog::appendToGui(Gui &gui)
{
	gui.appendElement(frame);
	gui.appendBtn(confirm);
	gui.appendBtn(cancel);
}


void Dialog::hide()
{
	cancel->invisible = true;
	confirm->invisible = true;
	frame->invisible = true;
}

void Dialog::show()
{
	cancel->invisible = false;
	confirm->invisible = false;
	frame->invisible = false;
}

void Dialog::showOkDialog()
{
	cancel->invisible = true;
	confirm->invisible = false;
	frame->invisible = false;
}

bool Dialog::isVisible() const
{
	if(frame==NULL)
		return false;
	return !frame->invisible;
}

int Dialog::defaultBtnFontSize = 120;

ExitButton::ExitButton(Renderer &r, QString title, QRectF bounds, bool textonly, int fontSize)
	: Button(r,title,bounds,textonly,fontSize)
{

}

ExitButton::ExitButton(Renderer &r, QString btnImagePath, QRectF bounds)
	: Button(r,btnImagePath,bounds)
{
}

void ExitButton::action()
{
	QApplication::instance()->exit(0);
}
