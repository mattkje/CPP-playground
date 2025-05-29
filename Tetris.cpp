#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <cstdlib>
#include <ctime>

class TetrisBlock : public QWidget {
    int squareX;
    int squareY;
    const int size = 20;
    int gravityTimerId;
    int blockType; // 0: square, 1: L, 2: T

    int gameWidth = 250;
    int gameHeight = 400;
    int currentObjectSize = 0;

    void spawnNewBlock() {
        blockType = std::rand() % 3;
        squareX = 75;
        squareY = 25;
    }

    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.setBrush(Qt::blue);
        if (blockType == 0) { // Square
            painter.drawRect(squareX, squareY, size, size);
            painter.drawRect(squareX + size, squareY, size, size);
            painter.drawRect(squareX, squareY + size, size, size);
            painter.drawRect(squareX + size, squareY + size, size, size);
            currentObjectSize = 2 * size;
        } else if (blockType == 1) { // L-shape
            painter.drawRect(squareX, squareY, size, size);
            painter.drawRect(squareX, squareY + size, size, size);
            painter.drawRect(squareX, squareY + 2 * size, size, size);
            painter.drawRect(squareX + size, squareY + 2 * size, size, size);
            currentObjectSize = 2 * size;
        } else if (blockType == 2) { // T-shape
            painter.drawRect(squareX, squareY, size, size);
            painter.drawRect(squareX + size, squareY, size, size);
            painter.drawRect(squareX + 2 * size, squareY, size, size);
            painter.drawRect(squareX + size, squareY + size, size, size);
            currentObjectSize = 3 * size;
        }
    }

    void timerEvent(QTimerEvent *event) override {
        if (event->timerId() == gravityTimerId) {
            squareY += size;
            if (squareY > height() - size * 4) {
                spawnNewBlock();
            }
            update();
        }
    }

    void keyPressEvent(QKeyEvent *event) override {
        if (event->key() == Qt::Key_Left && squareX > size) squareX -= size;
        if (event->key() == Qt::Key_Right && squareX < gameWidth - currentObjectSize) squareX += size;
        update();
    }

public:
    TetrisBlock() {
        setFocusPolicy(Qt::StrongFocus);
        std::srand(std::time(nullptr));
        spawnNewBlock();
        gravityTimerId = startTimer(200);
        setFixedSize(gameWidth+size, gameHeight+size);
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    TetrisBlock window;
    window.setWindowTitle("Tetris Clone");
    // Center the window on the screen
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - window.width()) / 2;
    int y = (screenGeometry.height() - window.height()) / 2;
    window.move(x, y);
    window.show();
    return app.exec();
}