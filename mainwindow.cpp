#include "mainwindow.h"

#include <random>

#include "ui_mainwindow.h"
#include "consts.h"
#include "types.h"
#include "field.h"
#include "utils.h"

namespace
{
std::random_device rd;
}

MainWindow::MainWindow()
:   QMainWindow(),
    m_ui(new Ui::MainWindow),
    m_boardSize(STARTING_SIZE),
    m_totalBombs(STARTING_BOMBS),
    m_currentBombs(0),
    m_board()
{
    m_ui->setupUi(this);
    setWindowTitle(TITLE);
    setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    connect(m_ui->replayButton, SIGNAL(clicked(bool)), this, SLOT( StartGame() ));
}

void MainWindow::StartGame()
{
    m_ui->replayButton->setText(REPLAY);
    auto board = m_ui->boardLayout;
    for (auto& fieldsRow : m_board)
    {
        for (auto& field : fieldsRow)
        {
            board->removeWidget(field);
        }
    }

    m_board = FieldsBoard(m_boardSize, FieldsRow(m_boardSize));
    {
        const auto bombsStr = std::to_string(m_totalBombs);
        const auto fieldsStr = std::to_string(m_boardSize * m_boardSize);
        m_ui->bombsLeft->setText(bombsStr.c_str());
        m_ui->fieldsLeft->setText(fieldsStr.c_str());
    }

    const auto fieldSize = WINDOW_WIDTH / m_boardSize;
    for (size_t row = 0; row < m_boardSize; ++row)
    {
        for (size_t column = 0; column < m_boardSize; ++column)
        {
            auto field = m_board[row][column]
                    = new Field(this, CalculateId(m_boardSize, row, column), m_board);
            field->setFixedSize(fieldSize, fieldSize);
            board->addWidget(field, row, column);
            connect(field, SIGNAL( clicked() ), field, SLOT( OnClick() ));
            connect(field, SIGNAL( rightClicked() ), field, SLOT( ToggleFlag() ));
            connect(field, SIGNAL( shouldUpdate() ), this, SLOT( UpdateCounts() ));
        }
    }

    const auto maxId = m_boardSize * m_boardSize;
    m_currentBombs = 0;
    while (m_currentBombs < m_totalBombs)
    {
        const auto id = rd() % maxId;
        const auto [row, column] = DecodeId(m_boardSize, id);
        auto& field = m_board[row][column];
        if (!field->IsBomb())
        {
            field->MakeBomb();
            ++m_currentBombs;
        }
    }
}

void MainWindow::UpdateCounts()
{
    size_t bombsLeft = m_totalBombs;
    size_t fieldsLeft = 0;
    for (auto& fieldsRow : m_board)
    {
        for (auto& field : fieldsRow)
        {
            if (field->isEnabled())
            {
                ++fieldsLeft;
            }
            if (field->IsFlagged())
            {
                --bombsLeft;
            }
        }
    }

    const auto bombsLeftText = std::to_string(bombsLeft);
    m_ui->bombsLeft->setText(bombsLeftText.c_str());

    const auto fieldsLeftText = std::to_string(fieldsLeft);
    m_ui->fieldsLeft->setText(fieldsLeftText.c_str());

    if (bombsLeft == 0 && fieldsLeft == m_totalBombs)
    {
        m_ui->replayButton->setText(VICTORY);
        for (auto& fieldsRow : m_board)
        {
            for (auto& field : fieldsRow)
            {
                const auto fieldText = field->GetClickedText();
                field->setText(fieldText.c_str());
                field->setDisabled(true);
            }
        }
    }
    else if (fieldsLeft == 0)
    {
        m_ui->replayButton->setText(DEFEAT);
    }
}

MainWindow::~MainWindow()
{
}

