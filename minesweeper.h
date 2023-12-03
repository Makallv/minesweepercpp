#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include <QMainWindow>
#include <QGridLayout>
#include <QVector>
#include <QPushButton>
#include <QMessageBox>
#include <QMenu>

QT_BEGIN_NAMESPACE
namespace Ui {
class MineSweeper;
}
QT_END_NAMESPACE

class MineSweeper : public QMainWindow
{
    Q_OBJECT

public:
    MineSweeper(QWidget *parent = nullptr);
    ~MineSweeper();

private slots:
    void handleButtonClick(int row, int col);
    void handleRightClick(int row, int col, const QPoint& pos);

private:
    void setupGame();
    void generateMines();
    void updateAdjacentMinesCount();
    void revealEmptyCells(int row, int col);
    void placeFlag(int row, int col);
    void checkWin();

    Ui::MineSweeper *ui;
    QGridLayout *gridLayout;
    QVector<QVector<QPushButton*>> buttons;
    QVector<QVector<int>> mines;
    int rows;
    int cols;
    int numMines;
    int unrevealedNonMineTiles;
};

#endif // MINESWEEPER_H
