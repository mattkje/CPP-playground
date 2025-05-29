#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QKeyEvent>

class SquareWidget : public QWidget {
    int squareX = 75;
    int squareY = 25;
    const int size = 100;

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.setBrush(Qt::blue);
        painter.drawRect(squareX, squareY, size, size);
    }

    void keyPressEvent(QKeyEvent *event) override {
        switch (event->key()) {
            case Qt::Key_Left: squareX -= 10;
                break;
            case Qt::Key_Right: squareX += 10;
                break;
            case Qt::Key_Up: squareY -= 10;
                break;
            case Qt::Key_Down: squareY += 10;
                break;
        }
        update();
    }

public:
    SquareWidget() { setFocusPolicy(Qt::StrongFocus); }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    SquareWidget window;
    window.resize(250, 150);
    window.setWindowTitle("Tetris Clone");
    window.show();
    return app.exec();
}
