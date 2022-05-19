#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QtCore/QDebug>
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtGui/QKeyEvent>
#include <QtCore/QRandomGenerator>
#include <QtCore/QTimer>
#include <QtGui/QScreen>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void paintEvent(QPaintEvent*);

    void keyReleaseEvent(QKeyEvent*);
    QString getKeyValue(QKeyEvent*);

    bool eventFilter(QObject*, QEvent*);

    void initializeDifficulty();
    void adjustPainter();
    void adjustPainterRestart();
    void toSmallScreen();
    void toBigScreen();
    int eliminate(QString);
    void goal(QString, int);
    void addNewLetter(int);
    void btnInitializeControl();
    void mySort(QVector<int> &);

public slots:
    void timerUpdate();

private slots:
    void on_pushButton_toMenu_clicked(bool checked);

    void on_pushButton_control_clicked(bool checked);

    void on_pushButton_restart_clicked(bool checked);

    void on_comboBox_currentIndexChanged(const QString &arg1);

signals:
    void toMenu();

private:
    Ui::MainWindow *ui;

    int mDesktopWidth;
    int mDesktopHeight;
    int mAvailableHeight;
    int mRankX1Distance;
    int mRankX2Distance;
    int mBottomHeight;
    int mGameScore;
    int mGameDifficulty;
    bool mDifficultyEnable;

    QTimer *m_timer;

    QVector<QString> m_letterVer;
    QVector<QPoint> m_pointDisplay;
    QVector<QString> m_strDisplay;
    QVector< QVector<int> > m_rank;
};
#endif // MAINWINDOW_H
