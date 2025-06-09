#include "gamewindow.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include <QElapsedTimer>
#include <QApplication>
#include <fstream>
#include <QLinearGradient>
#include <QRadialGradient>

GameWindow::GameWindow(QWidget *parent) : 
    QMainWindow(parent),
    m_board(15), 
    m_currentPiece(GomokuBoard::Black), 
    m_hoverPos(-1, -1) {
    setWindowTitle("Gomoku Game");
    setFixedSize(640, 640);  // 固定窗口大小
    setMouseTracking(true);   // 启用鼠标跟踪
    show();
    rating.ShowRating();
     
    // 添加模式选择对话框
    QMessageBox::StandardButton reply = QMessageBox::question(
        this,
        "游戏模式",
        "请选择游戏模式：\n是 - 人机对战\n否 - 双人对战",
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
        // 黑棋获胜时弹出消息框
        QMessageBox msgBox;
        msgBox.setText("黑棋获胜！");
        msgBox.setInformativeText("点击确定退出游戏");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);

        // 设置消息框按钮点击后的槽函数
        connect(&msgBox, &QMessageBox::finished, this, &GameWindow::exitGame);

        msgBox.exec();  // 弹出消息框，等待用户点击
    } else if (winner == GomokuBoard::White) {
        if(m_gameMode == HumanVsAI){
            WriteRatingN();
        }
        // 白棋获胜时弹出消息框
        QMessageBox msgBox;
        msgBox.setText("白棋获胜！");
        msgBox.setInformativeText("点击确定退出游戏");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setDefaultButton(QMessageBox::Ok);

        connect(&msgBox, &QMessageBox::finished, this, &GameWindow::exitGame);

        msgBox.exec();  // 弹出消息框，等待用户点击
    }
}
void GameWindow::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.drawPixmap(rect(), QPixmap("baka.png"));
    painter.setRenderHint(QPainter::Antialiasing);  // 启用抗锯齿
    drawBoard(painter);
    drawPieces(painter);
    drawHoverIndicator(painter); // 绘制悬停指示器
}
void GameWindow::mousePressEvent(QMouseEvent *event) {
    // 人机对战且不是玩家回合时忽略点击
    if (m_gameMode == HumanVsAI && m_currentPiece != GomokuBoard::Black) {
        return;
    }

    int gridSize = width() / m_board.size();
    int margin = (width() - (m_board.size() * gridSize)) / 2;  // 计算边距
    
    // 计算点击的网格位置
    int x = (event->x() - margin) / gridSize;
    int y = (event->y() - margin) / gridSize;

    // 确保点击在有效范围内
    if (x < 0 || x >= m_board.size() || y < 0 || y >= m_board.size()) {
        return;
    }

    if (m_board.placePiece(x, y, m_currentPiece)) {
        if (m_board.checkWin(x, y)) {
            ShowWinner(m_currentPiece);
        } else {
            // 人机对战模式触发AI回合
            if (m_gameMode == HumanVsAI) {
                m_currentPiece = GomokuBoard::White;
                QTimer::singleShot(500, this, [this]() {  // 添加0.5秒延迟
                    QPoint aiMove = m_aiplayer.calculateAIMove(m_board);
                    if (aiMove.x() != -1) {
                        m_board.placePiece(aiMove.x(), aiMove.y(), GomokuBoard::White);
                        if (m_board.checkWin(aiMove.x(), aiMove.y())) {
                            ShowWinner(GomokuBoard::White);
                        } else {
                            m_currentPiece = GomokuBoard::Black;
                        }
                        update();
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
    int gridSize = width() / m_board.size();
    int margin = (width() - (m_board.size() * gridSize)) / 2;  // 计算边距
    
    // 计算鼠标所在的网格位置
    int x = (event->x() - margin) / gridSize;
    int y = (event->y() - margin) / gridSize;
    
    // 检查位置是否在棋盘内且为空
    if (x >= 0 && x < m_board.size() && 
        y >= 0 && y < m_board.size() && 
        m_board.pieceAt(x, y) == GomokuBoard::Empty) {
        
        // 更新悬停位置
        if (m_hoverPos != QPoint(x, y)) {
            m_hoverPos = QPoint(x, y);
            update(); // 触发重绘
        }
    } else {
        // 如果位置无效，清除悬停指示
        if (m_hoverPos != QPoint(-1, -1)) {
            m_hoverPos = QPoint(-1, -1);
            update(); // 触发重绘
        }
    }
}
void GameWindow::leaveEvent(QEvent *event) {
    Q_UNUSED(event);
    // 鼠标离开窗口时清除悬停指示
    if (m_hoverPos != QPoint(-1, -1)) {
        m_hoverPos = QPoint(-1, -1);
        update(); // 触发重绘
    }
}
void GameWindow::drawBoard(QPainter &painter) {
    int gridSize = width() / m_board.size();
    int margin = (width() - (m_board.size() * gridSize)) / 2;  // 计算边距
    
    // 绘制棋盘背景
    painter.fillRect(margin, margin, 
                    m_board.size() * gridSize, 
                    m_board.size() * gridSize, 
                    QBrush(QColor(210, 180, 140, 150))); // 棋盘底色

    // 绘制棋盘边框
    painter.setPen(QPen(QColor(13, 102, 171, 150), 4));  // 深棕色边框
    painter.drawRect(margin, margin, 
                    m_board.size() * gridSize, 
                    m_board.size() * gridSize);

    // 绘制网格线
    painter.setPen(QPen(QColor(0, 0, 0), 1));
    for (int i = 0; i < m_board.size(); ++i) {
        // 绘制水平线
        int yPos = margin + i * gridSize;
        painter.drawLine(margin, yPos, 
                        margin + m_board.size() * gridSize, 
                        yPos);
        
        // 绘制垂直线
        int xPos = margin + i * gridSize;
        painter.drawLine(xPos, margin, 
                        xPos, 
                        margin + m_board.size() * gridSize);
    }

    // 绘制星位标记（天元和星）
    painter.setBrush(Qt::black);
    int starPoints[5][2] = {
        {3, 3}, {3, 11}, {7, 7}, {11, 3}, {11, 11}  // 标准五子棋星位
    };
    
    for (auto &point : starPoints) {
        int x = margin + point[0] * gridSize;
        int y = margin + point[1] * gridSize;
        painter.drawEllipse(QPoint(x, y), 4, 4);  // 绘制小黑点
    }
}
void GameWindow::drawPieces(QPainter &painter) {
    int gridSize = width() / m_board.size();
    int margin = (width() - (m_board.size() * gridSize)) / 2;  // 计算边距
    int pieceRadius = gridSize / 2 - 2;  // 棋子半径

    for (int x = 0; x < m_board.size(); ++x) {
        for (int y = 0; y < m_board.size(); ++y) {
            if (m_board.pieceAt(x, y) != GomokuBoard::Empty) {
                // 计算棋子中心位置（在网格方框中心）
                int centerX = margin + x * gridSize + gridSize / 2;
                int centerY = margin + y * gridSize + gridSize / 2;
                
                if (m_board.pieceAt(x, y) == GomokuBoard::Black) {
                    // 绘制黑色棋子（带渐变效果）
                    QRadialGradient gradient(centerX, centerY, pieceRadius, 
                                           centerX - pieceRadius/3, centerY - pieceRadius/3);
                    gradient.setColorAt(0, QColor(60, 60, 60));  // 深灰色
                    gradient.setColorAt(1, Qt::black);           // 黑色
                    painter.setBrush(gradient);
                } else {
                    // 绘制白色棋子（带渐变效果）
                    QRadialGradient gradient(centerX, centerY, pieceRadius, 
                                           centerX - pieceRadius/3, centerY - pieceRadius/3);
                    gradient.setColorAt(0, Qt::white);           // 白色
                    gradient.setColorAt(1, QColor(220, 220, 220));  // 浅灰色
                    painter.setBrush(gradient);
                }
                
                // 绘制棋子轮廓
                painter.setPen(QPen(Qt::black, 1));
                painter.drawEllipse(QPoint(centerX, centerY), pieceRadius, pieceRadius);
            }
        }
    }
}
void GameWindow::drawHoverIndicator(QPainter &painter) {
    // 如果悬停位置有效且为空位
    if (m_hoverPos.x() >= 0 && m_hoverPos.y() >= 0 && 
        m_board.pieceAt(m_hoverPos.x(), m_hoverPos.y()) == GomokuBoard::Empty) {
        
        int gridSize = width() / m_board.size();
        int margin = (width() - (m_board.size() * gridSize)) / 2;  // 计算边距
        
        // 计算悬停位置的中心坐标
        int centerX = margin + m_hoverPos.x() * gridSize + gridSize / 2;
        int centerY = margin + m_hoverPos.y() * gridSize + gridSize / 2;
        int pieceRadius = gridSize / 2 - 2;  // 棋子半径
        
        // 设置半透明绿色边框
        painter.setPen(QPen(QColor(255, 97, 0, 250), 5)); // 半透明绿色
        painter.setBrush(Qt::NoBrush);
        
        // 绘制边框（略大于棋子）
        painter.drawEllipse(QPoint(centerX, centerY), pieceRadius + 2, pieceRadius + 2);
    }
}
void GameWindow::exitGame() {
    QApplication::quit();  // 退出整个应用
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

