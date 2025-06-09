#ifndef AIPLAYER_H
#define AIPLAYER_H
#include "gomokuboard.h"
#include <QPoint>
class AiPlayer{
public:
	AiPlayer(){}
	int evaluatePosition(int x, int y, GomokuBoard::Piece aiPiece,GomokuBoard m_board); 
	QPoint calculateAIMove(GomokuBoard m_board);
};
#endif
