#include "minesweeper.h"
#include "ui_minesweeper.h"
#include <QRandomGenerator>

MineSweeper::MineSweeper(QWidget *parent)
    : QMainWindow(parent),
      ui(new Ui::MineSweeper),
      gridLayout(new QGridLayout),
      rows(10),
      cols(10),
      numMines(10),
      unrevealedNonMineTiles(rows * cols - numMines){
    ui->setupUi(this);
    setupGame();
}

MineSweeper::~MineSweeper(){
    delete ui;
}

void MineSweeper::setupGame(){
    generateMines();
    updateAdjacentMinesCount();

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(gridLayout);
    setCentralWidget(centralWidget);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            QPushButton *button = new QPushButton(this);
            button->setFixedSize(40, 40);
            connect(button, &QPushButton::clicked, this, [this, i, j]() { handleButtonClick(i, j); });
            connect(button, &QPushButton::customContextMenuRequested, this, [this, i, j](const QPoint& pos) { handleRightClick(i, j, pos); });
            gridLayout->addWidget(button, i, j);
            buttons[i].append(button);
        }
    }
}

void MineSweeper::generateMines(){
    buttons.resize(rows);
    mines.resize(rows);

    for (int i = 0; i < rows; ++i) {
        mines[i].resize(cols, 0);
    }

    int minesGenerated = 0;
    while (minesGenerated < numMines) {
        int row = QRandomGenerator::global()->bounded(rows);
        int col = QRandomGenerator::global()->bounded(cols);

        if (mines[row][col] != 9) {
            mines[row][col] = 9;
            minesGenerated++;
        }
    }
}

void MineSweeper::updateAdjacentMinesCount(){
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (mines[i][j] != 9) {
                for (int dx = -1; dx <= 1; ++dx) {
                    for (int dy = -1; dy <= 1; ++dy) {
                        int ni = i + dx;
                        int nj = j + dy;
                        if (ni >= 0 && ni < rows && nj >= 0 && nj < cols && mines[ni][nj] == 9) {
                            mines[i][j]++;
                        }
                    }
                }
            }
        }
    }
}

void MineSweeper::handleButtonClick(int row, int col){
    if (mines[row][col] == 9) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                if (mines[i][j] == 9) {
                    buttons[i][j]->setText("💣"); // You can use any symbol to represent bombs
                }
            }
        }

         QMessageBox::information(this, "Game Over", "You hit a mine! Game over.");

        close();
    } else if (mines[row][col] == 0) {
        revealEmptyCells(row, col);
    } else {
        buttons[row][col]->setText(QString::number(mines[row][col]));
        buttons[row][col]->setEnabled(false);
    }

    checkWin();
}

void MineSweeper::revealEmptyCells(int row, int col){
    if (row < 0 || row >= rows || col < 0 || col >= cols || buttons[row][col]->isEnabled() == false) {
        return;
    }

    buttons[row][col]->setEnabled(false);
    buttons[row][col]->setText("");

    if (mines[row][col] == 0) {
        for (int dx = -1; dx <= 1; ++dx) {
            for (int dy = -1; dy <= 1; ++dy) {
                revealEmptyCells(row + dx, col + dy);
            }
        }
    } else {
        buttons[row][col]->setText(QString::number(mines[row][col]));
        unrevealedNonMineTiles--;
    }
}

void MineSweeper::placeFlag(int row, int col){
    if (buttons[row][col]->isEnabled() == false || buttons[row][col]->text() == "🚩") {
        return;
    }

    buttons[row][col]->setText("🚩");
    unrevealedNonMineTiles--;
    checkWin();
}

void MineSweeper::handleRightClick(int row, int col, const QPoint& pos){
    if (row >= 0 && row < rows && col >= 0 && col < cols && buttons[row][col]->isEnabled()) {
        QMenu* contextMenu = new QMenu(this);
        QAction* flagAction = contextMenu->addAction("Place Flag");

        connect(flagAction, &QAction::triggered, this, [row, col, this]() { placeFlag(row, col); });

        contextMenu->exec(mapToGlobal(pos));

        delete contextMenu; // Cleaning up context menu
    }
}



void MineSweeper::checkWin() {
    int revealedNonMineTiles = 0;

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (!buttons[i][j]->isEnabled() && mines[i][j] != 9) {
                revealedNonMineTiles++;
            }
        }
    }
    if (revealedNonMineTiles == (rows * cols - numMines)) {
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                    if (mines[i][j] == 9) {
                        buttons[i][j]->setText("💣"); // You can use any symbol to represent bombs
                    }
            }
        }
        QMessageBox::information(this, "Congratulations", "You won!");
        close();
    }
}



