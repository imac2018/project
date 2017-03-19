#ifndef GLEXCEPTION_H
#define GLEXCEPTION_H

#include <exception>
#include <QString>

class GLException : std::exception
{
public:
	const QString title;
	const QString text;
	GLException(const QString& title, const QString& text);
    ~GLException() throw();
	virtual const char* what() const throw();
};

#endif // GLEXCEPTION_H
