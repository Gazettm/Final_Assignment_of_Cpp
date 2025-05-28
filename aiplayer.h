#ifndef AIPLAYER_H
#define AIPLAYER_H

#include <QPoint>
#include "gomokuboard.h"

class AIPlayer {
public:
	explicit AIPlayer(const GomokuBoard& board);
	QPoint calculateMove(GomokuBoard::Piece aiPiece);
	
private:
	int evaluatePosition(int x, int y, GomokuBoard::Piece aiPiece) const;
	
	const GomokuBoard& m_board;
};

#endif // AIPLAYER_H
