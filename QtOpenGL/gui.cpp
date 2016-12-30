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
			  const QString& label, int width, int height, int fontsize){
	QFont font = QFont("Arial");
	font.setPointSize(fontsize);
	painter.drawImage(0,0,QImage(imagePath).scaled(width,
													height,
													Qt::IgnoreAspectRatio));
	painter.setPen(QColor(40,20,13));
	painter.setFont(font);
	painter.drawText( QRectF(width*0.1,height*0.1,width*0.8,height*0.8),
					  Qt::AlignCenter | Qt::TextWordWrap, label );
}

void drawText(QPainter& painter,  const QString& imagePath,
			  const QString& label, int width, int height){
	drawText(painter,imagePath, label,width,height,(width*0.7)/label.count());
}

void drawText(QPainter& painter, const QString& label, int width, int height, int fontsize){
	QFont font = QFont("Arial");
	font.setPointSize(fontsize);
	painter.setPen(QColor(40,20,13));
	painter.setFont(font);

	painter.drawText( QRectF(width*0.1,height*0.1,width*0.8,height*0.8),
					  Qt::AlignCenter | Qt::TextWordWrap, label );
}

void drawText(QPainter& painter, const QString& label, int width, int height){
	drawText(painter,label,width,height,(width*0.7)/label.count());
}

void Button::makeButtonTexture(Renderer &renderer, const QString& title, float ratio)
{
	QPixmap text(720,720*ratio);
	text.fill(Qt::transparent);
	QPainter painter(&text);
	QFontMetrics fm(painter.font());
	int size = painter.font().pointSizeF() * qMin<float>(text.width() / fm.width(title),
													   text.height() / fm.height()) * 0.7;
	drawText(painter, ":/assets/button.png",title, text.width(), text.height(), size);
	texture = renderer.addTexture(text);
	textureInactiv = renderer.addTexture(makeGrayPicture(text));
	drawText(painter, ":/assets/button_hover.png",title, text.width(), text.height(), size);
	textureHover = renderer.addTexture(text);
	drawText(painter, ":/assets/button_pressed.png",title, text.width(), text.height(), size);
	texturePressed = renderer.addTexture(text);
}

Button::Button(Renderer& r, QString title, QRectF bounds)
	: GuiElement({Vertex3D(bounds.left(),bounds.top(),0,0,0),
				 Vertex3D(bounds.left(),bounds.bottom(),0,0,1),
				 Vertex3D(bounds.right(),bounds.top(),0,1,0),
				 Vertex3D(bounds.right(),bounds.bottom(),0,1,1)},
				 Qt::white, GL_TRIANGLE_STRIP), title(title),
		pressed(false), hover(false), bounds(bounds)
{
	makeButtonTexture(r,title, bounds.height() / bounds.width());
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

Label::Label(Renderer &r, QString title, QRectF bounds)
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
	drawText(painter,title, text.width(), text.height());
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

void Gui::appendBtn(Button *newBtn)
{
	buttons.append(newBtn);
}

void Gui::appendElement(GuiElement *elemnt)
{
	guiElements.append(elemnt);
}

void Gui::addObjectsToRenderer(Renderer &r)
{
	foreach(Button* o, buttons){
		r.addObject(*o);
	}
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
	renderer.draw(objects());
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
	QList<Button*>::Iterator k;
	for(k=buttons.begin();k!=buttons.end();k++){
		delete (*k);
	}
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
	: GuiElement({Vertex3D(bounds.left(),bounds.top(),0,0,0),
			   Vertex3D(bounds.left(),bounds.bottom(),0,0,1),
			   Vertex3D(bounds.right(),bounds.top(),0,1,0),
			   Vertex3D(bounds.right(),bounds.bottom(),0,1,1)},
			   Qt::white, GL_TRIANGLE_STRIP),
	  bounds(bounds)
{
	QPixmap text(720,720*bounds.height() / bounds.width());
	text.fill(Qt::transparent);
	QPainter painter(&text);
	drawText(painter, ":/assets/button.png",title, text.width(), text.height(), fontsize);
	texture = r.addTexture(text);
}

ToggleDialogButton::ToggleDialogButton(Dialog *parent, bool show, Renderer &r,
									   QString title, QRectF bounds)
	: Button(r,title,bounds), parent(parent), show(show)
{}

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
									QRectF(-0.1f,0.3f,0.2f,0.1f));
	frame = new DialogFrame(renderer,message, QRectF(-0.5,-0.5,1,1),50);
}

void Dialog::appendToGui(Gui &gui)
{
	gui.appendBtn(confirm);
	gui.appendBtn(cancel);
	gui.appendElement(frame);
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
