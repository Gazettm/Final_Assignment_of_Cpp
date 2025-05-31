#ifndef RATING_H
#define RATING_H
#include <QMessageBox>
#include <QString>
class Rating{
public:
	Rating();
	void ShowRating();
private:
	QString message;
	double rating;
	double Y;
	double N;
};
#endif
