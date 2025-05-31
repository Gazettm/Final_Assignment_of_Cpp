#include <QMessageBox>
#include <fstream>
#include "Rating.h"
Rating::Rating(): rating(0), Y(0), N(0){
	std::string filename = "Rating.txt";
	std::ifstream file(filename);
	if (!file) {
		std::ofstream createFile(filename);
		if (createFile) {
			createFile.close();
			file.open(filename);
		}
	}
	std::string line;
	while (std::getline(file, line)) {
		if(line[0] == 'Y')Y++;
		if(line[0] == 'N')N++;
	}
	file.close();
	if (N == 0 && Y == 0) {
		rating = 0;
		message = QString("无信息");
		return;
	} else if (Y == 0) {
		rating = 0;
	} else if (N == 0) {
		rating = 100;
	} else {
		rating = Y / (Y + N) * 100;
	}
	message = QString("您的人机对战胜率为 %1 %").arg(rating, 0, 'f', 2);
}

void Rating::ShowRating(){
	QMessageBox msgBox;
	msgBox.setWindowTitle("胜率");
	msgBox.setText(QString("%1\nYes确认,No清空胜率").arg(message));
	msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	msgBox.setDefaultButton(QMessageBox::Yes);
	int ret = msgBox.exec();
	if (ret == QMessageBox::No) {
		std::ofstream file("Rating.txt", std::ios::trunc);
		if (!file) {
			return;
		}
		file.close();
	}
}

