#include "gamewindow.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QElapsedTimer>
#include <QApplication>
#include <fstream>
#include <QLinearGradient>
#include <QRadialGradient>
#include <QKeyEvent> 

GameWindow::GameWindow(QWidget *parent) : 
    QMainWindow(parent),
    m_board(15), 
    m_currentPiece(GomokuBoard::Black), 
    m_hoverPos(-1, -1) {
    setWindowTitle("Gomoku Game");
    setFixedSize(640, 640);
    setMouseTracking(true);
    show();
    rating.ShowRating();

    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "游戏模式 ",
        "Ctrl+z可以悔棋\n请选择游戏模式：\n是 - 人机对战\n否 - 双人对战",
        QMessageBox::Yes | QMessageBox::No
    );
    m_gameMode = (reply == QMessageBox::Yes) ? HumanVsAI : HumanVsHuman;
    
}
void GameWindow::ShowWinner(GomokuBoard::Piece winner) {
    update();
    if (winner == GomokuBoard::Black) {
        if(m_gameMode == HumanVsAI){
            WriteRatingY();
        }

        QMessageBox msgBox;
        msgBox.setText("黑棋获胜！");
        msgBox.setInformativeText("点击确定退出游戏");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);

        connect(&msgBox, &QMessageBox::finished, this, &GameWindow::exitGame);

        msgBox.exec();
       } else if (winner == GomokuBoard::White) {
        if(m_gameMode == HumanVsAI){
            WriteRatingN();
        }

        QMessageBox msgBox;
        msgBox.setText("白棋获胜！");
        msgBox.setInformativeText("点击确定退出游戏");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);

        connect(&msgBox, &QMessageBox::finished, this, &GameWindow::exitGame);

        msgBox.exec(); 
    }
}
void GameWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this); 
    painter.setRenderHint(QPainter::Antialiasing);
    painter.drawPixmap(rect(), QPixmap("baka.png"));
    drawBoard(painter);
    drawPieces(painter);
    drawHoverIndicator(painter);
}
void GameWindow::mousePressEvent(QMouseEvent *event) {
    if (m_gameMode == HumanVsAI && m_currentPiece != GomokuBoard::Black) {
        return;
    }

    int gridSize = width() / (m_board.size() + 1);
    int margin = gridSize;
    
    int x = qRound(static_cast<float>(event->x() - margin) / gridSize);
    int y = qRound(static_cast<float>(event->y() - margin) / gridSize);

  if (x < 0 || x >= m_board.size() || y < 0 || y >= m_board.size()) {
        return;
    }

    if (m_board.placePiece(x, y, m_currentPiece)) {
        if (m_board.checkWin(x, y)) {
            ShowWinner(m_currentPiece);
        } else {

            if (m_gameMode == HumanVsAI) {
				isAIpending = true;
                m_currentPiece = GomokuBoard::White;
				
                QTimer::singleShot(500, this, [this]() { 
					
                    QPoint aiMove = m_aiplayer.calculateAIMove(m_board);
                    if (aiMove.x() != -1) {
                        m_board.placePiece(aiMove.x(), aiMove.y(), GomokuBoard::White);
                        if (m_board.checkWin(aiMove.x(), aiMove.y())) {
                            ShowWinner(GomokuBoard::White);
                        } else {
                            m_currentPiece = GomokuBoard::Black;
                        }
                        update();
                        isAIpending = false;
                    }
                });	
            } else {
                m_currentPiece = (m_currentPiece == GomokuBoard::Black) ? GomokuBoard::White : GomokuBoard::Black;
            }
            update();
        }
    }
}

void GameWindow::mouseMoveEvent(QMouseEvent *event) {
    int gridSize = width() / (m_board.size() + 1);
    int margin = gridSize;

    int x = qRound(static_cast<float>(event->x() - margin) / gridSize);
    int y = qRound(static_cast<float>(event->y() - margin) / gridSize);

    if (x >= 0 && x < m_board.size() && 
        y >= 0 && y < m_board.size() && 
        m_board.pieceAt(x, y) == GomokuBoard::Empty) {

        if (m_hoverPos != QPoint(x, y)) {
            m_hoverPos = QPoint(x, y);
            update(); 
        }
    } else {

        if (m_hoverPos != QPoint(-1, -1)) {
            m_hoverPos = QPoint(-1, -1);
            update();
        }
    }
}
void GameWindow::leaveEvent(QEvent *event) {
    Q_UNUSED(event);

    if (m_hoverPos != QPoint(-1, -1)) {
        m_hoverPos = QPoint(-1, -1);
        update(); 
    }
}

void GameWindow::keyPressEvent(QKeyEvent *event) {
	if(isAIpending){
		return;
	}
	if (event->key() == Qt::Key_Z && event->modifiers() & Qt::ControlModifier) {
		undoLastMove();
	}
	QMainWindow::keyPressEvent(event);
}

void GameWindow::drawBoard(QPainter &painter) {
    int gridSize = width() / (m_board.size() + 1);
    int margin = gridSize;
    
    int boardPixels = gridSize * (m_board.size() - 1);
    
    painter.fillRect(margin, margin, 
                   boardPixels, 
                   boardPixels, 
                   QBrush(QColor(210, 180, 140, 180))); 

    painter.setPen(QPen(QColor(13, 102, 171, 150), 4));
    painter.drawRect(margin, margin, 
                   boardPixels, 
                   boardPixels);

    painter.setPen(QPen(QColor(0, 0, 0), 1));
    for (int i = 0; i < m_board.size(); ++i) {
        int pos = margin + i * gridSize;

        painter.drawLine(margin, pos, 
                       margin + boardPixels, 
                       pos);

        painter.drawLine(pos, margin, 
                       pos, 
                       margin + boardPixels);
    }

    painter.setBrush(Qt::black);
    int starPoints[5][2] = {
        {3, 3}, {3, 11}, {7, 7}, {11, 3}, {11, 11} 
    };
    
    for (auto &point : starPoints) {
        int x = margin + point[0] * gridSize;
        int y = margin + point[1] * gridSize;
        painter.drawEllipse(QPoint(x, y), 4, 4); 
    }
}
void GameWindow::drawPieces(QPainter &painter) {
    int gridSize = width() / (m_board.size() + 1);
    int margin = gridSize;
    int pieceRadius = gridSize / 2 - 2;

    for (int x = 0; x < m_board.size(); ++x) {
        for (int y = 0; y < m_board.size(); ++y) {
            if (m_board.pieceAt(x, y) != GomokuBoard::Empty) {

                int centerX = margin + x * gridSize;
                int centerY = margin + y * gridSize;
                
                if (m_board.pieceAt(x, y) == GomokuBoard::Black) {

                    QRadialGradient gradient(centerX, centerY, pieceRadius, 
                                           centerX - pieceRadius/3, centerY - pieceRadius/3);
                    gradient.setColorAt(0, QColor(60, 60, 60)); 
                    gradient.setColorAt(1, Qt::black);          
                    painter.setBrush(gradient);
                } else {

                    QRadialGradient gradient(centerX, centerY, pieceRadius, 
                                           centerX - pieceRadius/3, centerY - pieceRadius/3);
                    gradient.setColorAt(0, Qt::white);
                    gradient.setColorAt(1, QColor(220, 220, 220));
                    painter.setBrush(gradient);
                }

                painter.setPen(QPen(Qt::black, 1));
                painter.drawEllipse(QPoint(centerX, centerY), pieceRadius, pieceRadius);
            }
        }
    }
}
void GameWindow::drawHoverIndicator(QPainter &painter) {

    if (m_hoverPos.x() >= 0 && m_hoverPos.y() >= 0 && 
        m_board.pieceAt(m_hoverPos.x(), m_hoverPos.y()) == GomokuBoard::Empty) {
        
        int gridSize = width() / (m_board.size() + 1);
        int margin = gridSize;
        

        int centerX = margin + m_hoverPos.x() * gridSize;
        int centerY = margin + m_hoverPos.y() * gridSize;
        int pieceRadius = gridSize / 2 - 2;

        painter.setPen(QPen(QColor(255, 97, 0, 250), 5));
        painter.setBrush(Qt::NoBrush);

        painter.drawEllipse(QPoint(centerX, centerY), pieceRadius + 2, pieceRadius + 2);
    }
}
void GameWindow::exitGame() {
    QApplication::quit();
}
void GameWindow::WriteRatingY(){
    std::string filename = "Rating.txt";
    std::string content = "Y";
    std::ofstream file(filename, std::ios::app);
    file << content << std::endl;
    file.close();
}
void GameWindow::WriteRatingN(){
    std::string filename = "Rating.txt";
    std::string content = "N";
    std::ofstream file(filename, std::ios::app);
    file << content << std::endl;
    file.close();
}
void GameWindow::undoLastMove() {
	if (m_gameMode == HumanVsHuman) {
		if (m_board.undoMove()) {
			m_currentPiece = (m_currentPiece == GomokuBoard::Black) 
			? GomokuBoard::White : GomokuBoard::Black;
			update();
		}
	} 
	else if (m_gameMode == HumanVsAI) {
		
		bool undoSuccess = false;
		if (m_board.getMoves().size() >= 2) {
			if (m_board.undoMove()) {
				if (m_board.undoMove()) {
					undoSuccess = true;
				}
			}
		}
		else if (!m_board.getMoves().empty()) {
			if (m_board.undoMove()) {
				undoSuccess = true;
			}
		}
		
		if (undoSuccess) {
			m_currentPiece = GomokuBoard::Black;
			update();
		}
	}
}


