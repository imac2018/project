#include "glexception.h"

GLException::GLException(const QString &title, const QString &text)
	: title(title), text(text)
{

}

GLException::~GLException() throw()
{}

const char *GLException::what() const throw()
{
	return QString(title+" - "+text).toStdString().c_str();
}
