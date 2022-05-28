#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <QTranslator>
#include <QApplication>
#include <QKeyEvent>
#include <QVector>
#include <QPair>
#include <QWidget>
#include <QLabel>
#include <QVBoxLayout>
#include <QCoreApplication>
#include <QPrinter>
#include <QPrintDialog>
#include <QPainter>
#include <QMenu>
#include <QDir>
#include <QMessageBox>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QTextCursor>
#include <QTextCharFormat>
#include <QTextFormat>
#include <QFont>
#include <QFontDialog>
#include <QDate>
#include <QTime>
#include <QTreeView>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QThread>
#include <QMutex>
#include "controller.h"
#include "hotkeys.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_7_clicked();

    void on_checkBox_stateChanged(int arg1);

    void clickMenu(QAction*);

    void clickFormatText(QAction*);

    void changeDir(const QString&);

    void selectDirDoubleClick(QModelIndex);

    void findFileSlot();

    void addFinedtoList(QString);

    void finishFind(QString);

private:
    Ui::MainWindow *ui;
    HotKeys *dialogForHotkeys;

    QFile file;
    QString filename;

    QTranslator translater;

    QGridLayout *FOR_INFO;
    QGridLayout *FILE_MANAGER;

    QWidget *forInfo;
    QLabel *labelForInfo;

    QWidget *fileManager;
    QComboBox *disks;
    QTreeView *outResult;
    QTextEdit *finedOut;
    QLineEdit *searchEnter;
    QPushButton *searchButton;
    QStandardItemModel *currentModel;
    Controller *controllerl;
    QString currentPath;

    QVector<qint32> keys; // open,save,new,exit

    QMenu *mainMenu;
    QMenu *formatTextMenu;

    QTextCharFormat textFormat;

    QPair<QString,QTextCharFormat> forPaste;

private:
    void keyPressEvent(QKeyEvent *myKey);

    void createNewFile();

    void retranslateUi();

    void MenuRetranslateAndCreate();

    QString getDateTime();

    void printDirsFiles(const QString&);

    void setNewModel(QStandardItemModel*);
};
#endif // MAINWINDOW_H
