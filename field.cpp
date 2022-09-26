#include "field.h"
#include "utils.h"
#include <QMouseEvent>

namespace
{
bool IsCoordValid(const size_t boardSize, const size_t coord)
{
    return 0 <= coord && coord < boardSize;
}
bool AreCoordsValid(const size_t boardSize, const size_t row, const size_t column)
{
    return IsCoordValid(boardSize, row) && IsCoordValid(boardSize, column);
}
}

Field::Field(QWidget* parent, const size_t id, FieldsBoard& board)
: QPushButton(parent),
  m_id(id),
  m_board(board),
  m_isBomb(false),
  m_isFlagged(false)
{
}

void Field::MakeBomb()
{
    m_isBomb = true;
}

bool Field::IsBomb()
{
    return m_isBomb;
}

bool Field::IsFlagged()
{
    return m_isFlagged;
}

std::string Field::GetClickedText()
{
    if (m_isBomb)
    {
        return "BOMB";
    }
    const auto size = m_board.size();
    const auto [row, column] = DecodeId(size, m_id);
    size_t bombsAround = 0;
    for (const int rowMod : {-1, 0, 1})
    {
        for (const int columnMod : {-1, 0, 1})
        {
            const auto neighbourRow = row + rowMod;
            const auto neighbourColumn = column + columnMod;
            if ((!rowMod && !columnMod) || !AreCoordsValid(size, neighbourRow, neighbourColumn))
            {
                continue;
            }
            if (m_board[neighbourRow][neighbourColumn]->m_isBomb)
            {
                ++bombsAround;
            }
        }
    }
    return bombsAround == 0 ? "" : std::to_string(bombsAround);
}

void Field::OnClick()
{
    if (!isEnabled() || m_isFlagged)
    {
        return;
    }

    if (m_isBomb)
    {
        for (auto& row : m_board)
        {
            for (auto& field : row)
            {
                const auto clickedText = field->GetClickedText();
                field->setDisabled(true);
                field->setText(clickedText.c_str());
            }
        }
        emit shouldUpdate();
        return;
    }

    const auto clickedText = GetClickedText();
    setDisabled(true);
    setText(clickedText.c_str());

    if (clickedText.empty())
    {
        const auto size = m_board.size();
        const auto [row, column] = DecodeId(size, m_id);
        for (const int rowMod : {-1, 0, 1})
        {
            for (const int columnMod : {-1, 0, 1})
            {
                const auto neighbourRow = row + rowMod;
                const auto neighbourColumn = column + columnMod;
                if (AreCoordsValid(size, neighbourRow, neighbourColumn))
                {
                    m_board[neighbourRow][neighbourColumn]->OnClick();
                }
            }
        }
    }
    emit shouldUpdate();
}

void Field::ToggleFlag()
{
    m_isFlagged = !m_isFlagged;
    if (m_isFlagged)
    {
        setText("FLAG");
    }
    else
    {
        setText("");
    }
    emit shouldUpdate();
}

void Field::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::RightButton)
        emit rightClicked();
    else if (e->button() == Qt::LeftButton)
        emit clicked();
}
