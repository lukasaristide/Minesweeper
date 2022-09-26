#ifndef FIELD_H
#define FIELD_H

#include <QPushButton>
#include "types.h"

class Field : public QPushButton
{
    Q_OBJECT
public:
    Field(QWidget* parent, const size_t id, FieldsBoard& board);

    void MakeBomb();
    bool IsBomb();
    bool IsFlagged();
    std::string GetClickedText();

signals:
    void rightClicked();
    void clicked();
    void shouldUpdate();

private slots:
    void OnClick();
    void ToggleFlag();
    void mousePressEvent(QMouseEvent *e);

private:
    size_t m_id;
    FieldsBoard& m_board;
    bool m_isBomb;
    bool m_isFlagged;
};

#endif // FIELD_H
