#include "glexception.h"

GLException::GLException(const QString &title, const QString &text)
	: title(title), text(text)
{

}

const char *GLException::what() const
{
	return QString(title+" - "+text).toStdString().c_str();
}
