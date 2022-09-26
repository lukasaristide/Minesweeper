#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "types.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();
    ~MainWindow();

private slots:
    void StartGame();
    void UpdateCounts();

private:
    std::unique_ptr<Ui::MainWindow> m_ui;
    size_t m_boardSize;
    size_t m_totalBombs;
    size_t m_currentBombs;

    FieldsBoard m_board;
};
#endif // MAINWINDOW_H
