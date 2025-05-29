#ifndef AIPLAYER_H
#define AIPLAYER_H
#include "gomokuboard.h"
#include <QPoint>
class AiPlayer{
public:
	AiPlayer(){}
	int evaluatePosition(int x, int y, GomokuBoard::Piece aiPiece,GomokuBoard m_board);  // 位置评估函数
	QPoint calculateAIMove(GomokuBoard m_board);  // AI计算落子位置
};
#endif
