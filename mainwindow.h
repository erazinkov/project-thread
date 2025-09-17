#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QObject>
#include <QProgressBar>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;


private:
    Ui::MainWindow *ui;
    QPushButton *m_buttonStart;
    QPushButton *m_buttonStop;

    QProgressBar *m_progressBar;
private slots:
    void handleClickedStart();
    void handleClickedStop();
};
#endif // MAINWINDOW_H
