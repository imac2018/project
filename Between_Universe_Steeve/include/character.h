#ifndef CHARACTER_H
#define CHARACTER_H

#include <QImage>
#include <QColor>

/**
 * @brief Describe a Game Character with a name and a sprite
 * Never used
 */
class Character
{
	QImage image;
	QString name;
public:
	Character(QImage image, QString name);
};

#endif // CHARACTER_H
