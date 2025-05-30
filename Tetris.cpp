#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QKeyEvent>
#include <QMessageBox>
#include <cstdlib>
#include <ctime>

class TetrisBlock : public QWidget {
    int squareX;
    int squareY;
    const int size = 20;
    // 0: Square, 1: L-shape, 2: T-shape, 3: Vertical line 4: S-shape, 5: Z-shape

    int gameWidth = 200;
    int gameHeight = 400;
    const int borderSize = 200;
    int currentObjectSize = 0;

    int score = 0;
    bool gameOver = false;
    bool waitingToStart = true;
    int gravityTimerId = 0;


    struct BlockType {
        int id;
        std::array<std::array<int, 4>, 4> matrix = {};
        int rotation = 0;
    };


    struct Block {
        QRect rect;
        QColor color;
        BlockType type;
    };

    Block currentBlock = {QRect(0, 0, 0, 0), Qt::black, {0, 0}};

    std::vector<Block> blocks;

    // list of all blocks
    std::array<std::array<std::array<int, 4>, 4>, 6> allBlocks = {
        {
            // Square block
            {
                {
                    {{0, 0, 0, 0}},
                    {{0, 1, 1, 0}},
                    {{0, 1, 1, 0}},
                    {{0, 0, 0, 0}}
                }
            },
            // L-shape block
            {
                {
                    {{0, 1, 0, 0}},
                    {{0, 1, 0, 0}},
                    {{0, 1, 1, 0}},
                    {{0, 0, 0, 0}}
                }
            },
            // T-shape block
            {
                {
                    {{0, 0, 0, 0}},
                    {{0, 1, 0, 0}},
                    {{1, 1, 1, 0}},
                    {{0, 0, 0, 0}}
                }
            },
            // Vertical line block
            {
                {
                    {{0, 1, 0, 0}},
                    {{0, 1, 0, 0}},
                    {{0, 1, 0, 0}},
                    {{0, 1, 0, 0}}
                }
            },
            // S-shape block
            {
                {
                    {{0, 0, 1, 0}},
                    {{0, 1, 1, 0}},
                    {{0, 1, 0, 0}},
                    {{0, 0, 0, 0}}
                }
            },
            // Z-shape block
            {
                {
                    {{0, 1, 0, 0}},
                    {{0, 1, 1, 0}},
                    {{0, 0, 1, 0}},
                    {{0, 0, 0, 0}}
                }
            }
        }
    };

    // In TetrisBlock class
    void startGame() {
        blocks.clear();
        score = 0;
        gameOver = false;
        waitingToStart = false;
        spawnNewBlock();
        if (gravityTimerId != 0) {
            killTimer(gravityTimerId);
            gravityTimerId = 0;
        }
        gravityTimerId = startTimer(300);
        update();
    }

    void spawnNewBlock() {
        currentBlock.type.id = std::rand() % 6;
        currentBlock.type.matrix = allBlocks[currentBlock.type.id];
        squareX = (gameWidth) / 2;
        squareY = 0;

        // Check for collision at spawn
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                if (currentBlock.type.matrix[row][col] == 1) {
                    int cellX = squareX + col * size;
                    int cellY = squareY + row * size;
                    for (const auto &block : blocks) {
                        if (block.rect.x() == cellX && block.rect.y() == cellY) {
                            gameOver = true;
                            if (gravityTimerId != 0) {
                                killTimer(gravityTimerId);
                                gravityTimerId = 0;
                            }
                            QMessageBox::information(this, "Game Over", "Game Over!\nYour score: " + QString::number(score));
                            waitingToStart = true;
                            update();
                            return;
                        }
                    }
                }
            }
        }
    }

    void saveBlock() {
        // Add all squares of the current block with their color
        // Helper to save a block from a matrix
        auto saveBlockFromMatrix = [&](int matrix[4][4], int rows, int cols, QColor color) {
            for (int row = 0; row < rows; ++row) {
                for (int col = 0; col < cols; ++col) {
                    if (matrix[row][col] == 1) {
                        blocks.push_back({QRect(squareX + col * size, squareY + row * size, size, size), color});
                    }
                }
            }
        };

        QColor color;
        if (currentBlock.type.id == 0) color = Qt::yellow;
        else if (currentBlock.type.id == 1) color = Qt::blue;
        else if (currentBlock.type.id == 2) color = Qt::magenta;
        else if (currentBlock.type.id == 3) color = Qt::cyan;
        else if (currentBlock.type.id == 4) color = Qt::green;
        else if (currentBlock.type.id == 5) color = Qt::red;

        int rows = 4;
        int cols = 4;
        int matrix[4][4] = {0};
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
                matrix[r][c] = currentBlock.type.matrix[r][c];

        saveBlockFromMatrix(matrix, rows, cols, color);
    }


    void checkForFullRows() {
        int rowsCleared = 0;
        bool foundFullRow;
        do {
            foundFullRow = false;
            for (int row = (gameHeight / size) - 1; row >= 0; --row) {
                int y = row * size;
                int count = std::count_if(blocks.begin(), blocks.end(),
                                          [y](const Block &b) { return b.rect.y() == y; });
                if (count == 10) {
                    std::erase_if(blocks, [y](const Block &b) { return b.rect.y() == y; });
                    // Move all blocks above down by one row
                    for (auto &block : blocks) {
                        if (block.rect.y() < y) {
                            block.rect.moveTop(block.rect.y() + size);
                        }
                    }
                    rowsCleared++;
                    foundFullRow = true;
                    break; // Start over, as rows have shifted
                }
            }
        } while (foundFullRow);

        // Scoring: 1 row=100, 2=300, 3=500, 4=800
        switch (rowsCleared) {
            case 1: score += 100; break;
            case 2: score += 300; break;
            case 3: score += 500; break;
            case 4: score += 800; break;
            default: break;
        }
    }
    void paintEvent(QPaintEvent *) override {
       QPainter painter(this);
       if (QPixmap bg("/Users/mattikjellstadli/Developer/Desktop Applications/CPP-Playground/background.png"); bg.isNull()) {
           painter.fillRect(rect(), Qt::black);
       } else {
           painter.drawPixmap(rect(), bg);
       }


        if (waitingToStart) {
            QPixmap bg("/Users/mattikjellstadli/Developer/Desktop Applications/CPP-Playground/startBackground.png");
            if (bg.isNull()) {
                painter.fillRect(rect(), Qt::black);
            } else {
                painter.drawPixmap(rect(), bg);
            }
            painter.setPen(Qt::white);
            painter.setFont(QFont("New Amsterdam", 30, QFont::Bold));
            painter.drawText(rect(), Qt::AlignCenter, "Press Space to Start");
            return;
        }

        painter.setPen(Qt::white);
        painter.setFont(QFont("New Amsterdam", 30, QFont::Bold));
        painter.drawText(240, 120 + painter.fontMetrics().ascent(), QString("Score: %1").arg(score));

        QColor color;
        QColor penColor;
        switch (currentBlock.type.id) {
            case 0: color = Qt::yellow;
                penColor = Qt::darkYellow;
                break;
            case 1: color = Qt::blue;
                penColor = Qt::darkBlue;
                break;
            case 2: color = Qt::magenta;
                penColor = Qt::darkMagenta;
                break;
            case 3: color = Qt::cyan;
                penColor = Qt::darkCyan;
                break;
            case 4: color = Qt::green;
                penColor = Qt::darkGreen;
                break;
            case 5: color = Qt::red;
                penColor = Qt::darkRed;
                break;
            default: color = Qt::black;
                penColor = Qt::black;
                break;
        }
        painter.setBrush(color);
        painter.setPen(QPen(penColor, 2));
        for (int row = 0; row < 4; ++row) {
            for (int col = 0; col < 4; ++col) {
                if (currentBlock.type.matrix[row][col] == 1) {
                    painter.drawRoundedRect(squareX + col * size, squareY + row * size, size, size, 5, 5);
                }
            }
        }

        for (const auto &block: blocks) {
            if (block.color == Qt::blue)
                painter.setPen(QPen(Qt::darkBlue, 2));
            else if (block.color == Qt::green)
                painter.setPen(QPen(Qt::darkGreen, 2));
            else if (block.color == Qt::red)
                painter.setPen(QPen(Qt::darkRed, 2));
            else if (block.color == Qt::cyan)
                painter.setPen(QPen(Qt::darkCyan, 2));
            else if (block.color == Qt::magenta)
                painter.setPen(QPen(Qt::darkMagenta, 2));
            else if (block.color == Qt::yellow)
                painter.setPen(QPen(Qt::darkYellow, 2));
            else
                painter.setPen(QPen(Qt::black, 2));

            painter.setBrush(block.color);
            painter.drawRoundedRect(block.rect, 5, 5);
        }
    }

    void timerEvent(QTimerEvent *event) override {
        if (event->timerId() == gravityTimerId) {
            int minRow = 4, maxRow = -1, minCol = 4, maxCol = -1;
            for (int row = 0; row < 4; ++row) {
                for (int col = 0; col < 4; ++col) {
                    if (currentBlock.type.matrix[row][col] == 1) {
                        if (row < minRow) minRow = row;
                        if (row > maxRow) maxRow = row;
                        if (col < minCol) minCol = col;
                        if (col > maxCol) maxCol = col;
                    }
                }
            }

            bool willCollide = false;
            // Check every occupied cell for collision below
            for (int row = minRow; row <= maxRow; ++row) {
                for (int col = minCol; col <= maxCol; ++col) {
                    if (currentBlock.type.matrix[row][col] == 1) {
                        int cellX = squareX + col * size;
                        int cellY = squareY + row * size;
                        // Check if moving down would go out of bounds
                        if (cellY + size >= gameHeight) {
                            willCollide = true;
                            break;
                        }
                        // Check if moving down would overlap with any saved block
                        for (const auto &block : blocks) {
                            if (block.rect.x() == cellX && block.rect.y() == cellY + size) {
                                willCollide = true;
                                break;
                            }
                        }
                    }
                    if (willCollide) break;
                }
                if (willCollide) break;
            }

            if (willCollide) {
                saveBlock();
                checkForFullRows();
                spawnNewBlock();
            } else {
                squareY += size;
            }
            update();
        }
    }

    // Rotates a 4x4 matrix 90 degrees clockwise
    void rotateMatrix(std::array<std::array<int, 4>, 4> &matrix, int &rows, int &cols) {
        std::array<std::array<int, 4>, 4> temp = {};
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
                temp[c][rows - 1 - r] = matrix[r][c];
        int t = rows;
        rows = cols;
        cols = t;
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
                matrix[r][c] = temp[r][c];
    }

   void keyPressEvent(QKeyEvent *event) override {
        if (waitingToStart && event->key() == Qt::Key_Space) {
            startGame();
            return;
        }
        if (waitingToStart) return;
       int minRow = 4, maxRow = -1, minCol = 4, maxCol = -1;
       for (int row = 0; row < 4; ++row) {
           for (int col = 0; col < 4; ++col) {
               if (currentBlock.type.matrix[row][col] == 1) {
                   if (row < minRow) minRow = row;
                   if (row > maxRow) maxRow = row;
                   if (col < minCol) minCol = col;
                   if (col > maxCol) maxCol = col;
               }
           }
       }

       // Soft drop (Down/S) with collision check
       if ((event->key() == Qt::Key_Down || event->key() == Qt::Key_S)) {
           bool canMove = true;
           for (int row = minRow; row <= maxRow && canMove; ++row) {
               for (int col = minCol; col <= maxCol && canMove; ++col) {
                   if (currentBlock.type.matrix[row][col] == 1) {
                       int cellX = squareX + col * size;
                       int cellY = squareY + row * size;
                       if (cellY + size >= gameHeight) {
                           canMove = false;
                       }
                       for (const auto &block : blocks) {
                           if (block.rect.x() == cellX && block.rect.y() == cellY + size) {
                               canMove = false;
                           }
                       }
                   }
               }
           }
           if (canMove) {
               squareY += size;
           }
       }

       // Move left
       if ((event->key() == Qt::Key_Left || event->key() == Qt::Key_A) && squareX + minCol * size > 0)
           squareX -= size;

       // Move right
       if ((event->key() == Qt::Key_Right || event->key() == Qt::Key_D) &&
           squareX + (maxCol + 1) * size < gameWidth)
           squareX += size;

       // Rotate
       if (event->key() == Qt::Key_Space) {
           int rows = 4, cols = 4;
           rotateMatrix(currentBlock.type.matrix, rows, cols);
       }

       // Hard drop (Up/W)
       if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W) {
           bool canMove = true;
           while (canMove) {
               for (int row = minRow; row <= maxRow && canMove; ++row) {
                   for (int col = minCol; col <= maxCol && canMove; ++col) {
                       if (currentBlock.type.matrix[row][col] == 1) {
                           int cellX = squareX + col * size;
                           int cellY = squareY + row * size;
                           if (cellY + size >= gameHeight) {
                               canMove = false;
                           }
                           for (const auto &block : blocks) {
                               if (block.rect.x() == cellX && block.rect.y() == cellY + size) {
                                   canMove = false;
                               }
                           }
                       }
                   }
               }
               if (canMove) {
                   squareY += size;
               }
           }
           // Place the block and spawn a new one
           saveBlock();
           checkForFullRows();
           spawnNewBlock();
       }
       update();
   }

public:
    TetrisBlock() {
        setFocusPolicy(Qt::StrongFocus);
        std::srand(std::time(nullptr));
        waitingToStart = true;
        setFixedSize(gameWidth + borderSize, gameHeight + borderSize);
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
