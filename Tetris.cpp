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


    struct BlockType {
        int sizeX;
        int sizeY;
    };

    struct Block {
        QRect rect;
        QColor color;
        BlockType type;
    };

    Block currentBlock = {QRect(0, 0, 0, 0), Qt::black, {0, 0}};

    std::vector<Block> blocks;

    void spawnNewBlock() {

        // Randomly select a block type

        blockType = std::rand() % 4;
        squareX = 75;
        squareY = 25;
    }

    void saveBlock() {
        QColor color;
        if (blockType == 0) color = Qt::blue;
        else if (blockType == 1) color = Qt::green;
        else if (blockType == 2) color = Qt::red;
        else if (blockType == 3) color = Qt::cyan;

        // Add all squares of the current block with their color
        if (blockType == 0) { // Square
            blocks.push_back({QRect(squareX, squareY, size, size), color});
            blocks.push_back({QRect(squareX + size, squareY, size, size), color});
            blocks.push_back({QRect(squareX, squareY + size, size, size), color});
            blocks.push_back({QRect(squareX + size, squareY + size, size, size), color});
        } else if (blockType == 1) { // L-shape
            blocks.push_back({QRect(squareX, squareY, size, size), color});
            blocks.push_back({QRect(squareX, squareY + size, size, size), color});
            blocks.push_back({QRect(squareX, squareY + 2 * size, size, size), color});
            blocks.push_back({QRect(squareX + size, squareY + 2 * size, size, size), color});
        } else if (blockType == 2) { // T-shape
            blocks.push_back({QRect(squareX, squareY, size, size), color});
            blocks.push_back({QRect(squareX + size, squareY, size, size), color});
            blocks.push_back({QRect(squareX + 2 * size, squareY, size, size), color});
            blocks.push_back({QRect(squareX + size, squareY + size, size, size), color});
        } else if (blockType == 3) { // Vertical line
            blocks.push_back({QRect(squareX, squareY, size, size), color});
            blocks.push_back({QRect(squareX, squareY + size, size, size), color});
            blocks.push_back({QRect(squareX, squareY + 2 * size, size, size), color});
            blocks.push_back({QRect(squareX, squareY + 3 * size, size, size), color});
        }
    }

    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);

        if (blockType == 0) { // Square
            painter.setBrush(Qt::blue);
            painter.drawRect(squareX, squareY, size, size);
            painter.drawRect(squareX + size, squareY, size, size);
            painter.drawRect(squareX, squareY + size, size, size);
            painter.drawRect(squareX + size, squareY + size, size, size);
            currentBlock.type.sizeX = 2 * size;
            currentBlock.type.sizeY = 2 * size;
        } else if (blockType == 1) { // L-shape
            painter.setBrush(Qt::green);
            painter.drawRect(squareX, squareY, size, size);
            painter.drawRect(squareX, squareY + size, size, size);
            painter.drawRect(squareX, squareY + 2 * size, size, size);
            painter.drawRect(squareX + size, squareY + 2 * size, size, size);
            currentBlock.type.sizeX = 2 * size;
            currentBlock.type.sizeY = 3 * size;
        } else if (blockType == 2) { // T-shape
            painter.setBrush(Qt::red);
            painter.drawRect(squareX, squareY, size, size);
            painter.drawRect(squareX + size, squareY, size, size);
            painter.drawRect(squareX + 2 * size, squareY, size, size);
            painter.drawRect(squareX + size, squareY + size, size, size);
            currentBlock.type.sizeX = 3 * size;
            currentBlock.type.sizeY = 2 * size;
        } else if (blockType == 3) { // Vertical line
            painter.setBrush(Qt::cyan);
            painter.drawRect(squareX, squareY, size, size);
            painter.drawRect(squareX, squareY + size, size, size);
            painter.drawRect(squareX, squareY + 2 * size, size, size);
            painter.drawRect(squareX, squareY + 3 * size, size, size);
            currentBlock.type.sizeX = size;
            currentBlock.type.sizeY = 4 * size;
        }
        painter.setPen(Qt::black);
        for (const auto &block : blocks) {
            painter.setBrush(block.color);
            painter.drawRect(block.rect);
        }
    }

    void timerEvent(QTimerEvent *event) override {
        if (event->timerId() == gravityTimerId) {
            squareY += size;
            if (squareY > height() - currentBlock.type.sizeY) {
                saveBlock();
                spawnNewBlock();
            }
            update();
        }
    }

    void keyPressEvent(QKeyEvent *event) override {
        if (event->key() == Qt::Key_Left && squareX > size) squareX -= size;
        if (event->key() == Qt::Key_Right && squareX < gameWidth - currentBlock.type.sizeX) squareX += size;
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