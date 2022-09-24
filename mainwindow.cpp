#include "mainwindow.h"

#include "ui_mainwindow.h"
#include "Consts.h"

MainWindow::MainWindow()
    : QMainWindow()
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle(TITLE);
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    connect(ui->replayButton, SIGNAL(clicked(bool)), this, SLOT( StartGame() ));
}

void MainWindow::StartGame()
{
    {
        const auto bombsStr = std::to_string(STARTING_BOMBS);
        const auto fieldsStr = std::to_string(STARTING_SIZE * STARTING_SIZE);
        ui->bombsLeft->setText(bombsStr.c_str());
        ui->fieldsLeft->setText(fieldsStr.c_str());
    }

    auto board = ui->boardLayout;
    const auto fieldSize = WINDOW_WIDTH / STARTING_SIZE;
    for (size_t row = 0; row < STARTING_SIZE; ++row)
    {
        for (size_t column = 0; column < STARTING_SIZE; ++column)
        {
            auto field = new QPushButton(this);
            field->setFixedSize(fieldSize, fieldSize);
            board->addWidget(field, row, column);
        }
    }
}

MainWindow::~MainWindow()
{
}

