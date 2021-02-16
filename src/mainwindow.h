#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <fstream>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

enum loadMode
{   render = 0,
    forward = 1,
    backward = 2
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_actionOpen_triggered();

    void on_verticalScrollBar_valueChanged(int value);

    void on_actionFind_triggered();

    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;
    bool eventFilter(QObject *object, QEvent *event);
    void populateText(int position = 0, int mode = 0);
    void recalcBounds();
    void cursorToPrevLine(std::ifstream* stream);
    void resizeEvent(QResizeEvent* event);
    void loadFile(QString path = "");

    int currentPosition = 0;
    QString filePath;
    std::ifstream *fileStream = nullptr;
    int fileEndPosition;
    bool scrollbarControlled = false;
};
#endif // MAINWINDOW_H
