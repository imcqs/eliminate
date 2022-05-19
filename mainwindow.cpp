#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("游戏人生");
    ui->pushButton_restart->setEnabled(false);

    QStringList list;
    list << "超级简单" << "真的一般" << "大概中等" << "有点困难" << "噩梦" << "地狱";
    ui->comboBox->addItems(list);

    QLineEdit *lineEdit = new QLineEdit(this);
    lineEdit->setReadOnly(true);
    lineEdit->setAlignment(Qt::AlignCenter);
    ui->comboBox->setLineEdit(lineEdit);

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));

    mGameScore = 0;
    mGameDifficulty = 0;
    mDifficultyEnable = false;

    for(int i = 0; i < 26; i++)
    {
        m_letterVer << QString('A' + i);
    }

    QList<QScreen *> list_screen =  QGuiApplication::screens();
    QRect rect = list_screen.at(0)->availableGeometry();
    mDesktopWidth = rect.width();
    mDesktopHeight = rect.height();
    mAvailableHeight = static_cast<int>(mDesktopHeight * 0.962);
    mRankX1Distance = static_cast<int>(mDesktopWidth * 0.9375 / 5);
    mRankX2Distance = static_cast<int>(mDesktopWidth * 0.9375 / 10);

    setWindowState(Qt::WindowMaximized);
    toBigScreen();
    installEventFilter(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    Q_UNUSED(obj)

    QEvent::Type eventType = event->type();

    if (eventType == QEvent::WindowStateChange)
    {
        //其实，Qt的事件对象都有一个accept()函数和ignore()函数。
        //前者用来告诉Qt，事件处理函数“接收”了这个事件，不要再传递；
        //后者则告诉Qt，事件处理函数“忽略”了这个事件，需要继续传递，寻找另外的接受者。
        event->accept();
        //event->ignore();
        if(windowState() != Qt::WindowMaximized)
        {
            toSmallScreen();

            if(mDifficultyEnable)
            {
                adjustPainter();
            }

            move(mDesktopWidth / 2 - 960 / 2, mAvailableHeight / 2 - 500 / 2);
            setFixedSize(960, 500);
        }
        else {
            toBigScreen();

            if(mDifficultyEnable)
            {
                adjustPainter();
            }

            setFixedSize(mDesktopWidth, mDesktopHeight - 23);
            showMaximized();
        }
        return true;
    }
    return QWidget::event(event);
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event)
    QPainter painter(this);
    painter.setPen(QColor(0, 0, 255));
    painter.setFont(QFont("", 20));

    for(int i = 0; i < m_strDisplay.size(); i++)
    {
        painter.drawText(m_pointDisplay.at(i), m_strDisplay.at(i));
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    QString strInput = getKeyValue(event);
    int inScore = eliminate(strInput);
    goal(strInput, inScore);
}

void MainWindow::initializeDifficulty()
{
    for(int i = 0; i < m_rank.at(mGameDifficulty).at(0) ; i++)
    {
        int _random = QRandomGenerator::global()->bounded(m_letterVer.size() - 1);
        QString _addLetter = m_letterVer.at(_random);

        m_pointDisplay.insert(i, QPoint(i * m_rank.at(mGameDifficulty).at(1), 0));
        m_strDisplay.insert(i, m_letterVer.at(_random));
    }
}

void MainWindow::adjustPainter()
{
    for(int i = 0; i < m_rank.at(mGameDifficulty).at(0) ; i++)
    {
        m_pointDisplay.replace(i, QPoint(i * m_rank.at(mGameDifficulty).at(1), m_pointDisplay.at(i).y()));
    }
}

void MainWindow::adjustPainterRestart()
{
    for(int i = 0; i < m_rank.at(mGameDifficulty).at(0) ; i++)
    {
        m_pointDisplay.replace(i, QPoint(i * m_rank.at(mGameDifficulty).at(1), 0));
    }
}

//column (x2-x1) (y2-y1)
void MainWindow::toSmallScreen()
{
    mBottomHeight = 500 + 20;
    m_rank = {{5, 180, 2},
              {5, 180, 4},
              {10, 90, 4},
              {10, 90, 8},
              {10, 90, 16},
              {10, 90, 32}};
}

void MainWindow::toBigScreen()
{
    mBottomHeight = mAvailableHeight + 40;
    m_rank = {{5, mRankX1Distance, 4},
              {5, mRankX1Distance, 8},
              {10, mRankX2Distance, 8},
              {10, mRankX2Distance, 16},
              {10, mRankX2Distance, 32},
              {10, mRankX2Distance, 64}};
}

void MainWindow::timerUpdate()
{
    if(mGameScore <= -50)
    {
        m_timer->stop();
        ui->comboBox->setEnabled(true);
        ui->pushButton_control->setText("开始");
        ui->pushButton_control->setEnabled(false);
        QMessageBox::critical(this, "失败", "你的积分为：-50，换个难度吧，求你了！", "确定");
    }
    else {
        for(int i = 0; i < m_pointDisplay.size(); i++)
        {
            int _pointY = m_pointDisplay.at(i).y() + m_rank.at(mGameDifficulty).at(2);

            if(_pointY > mBottomHeight)
            {
                goal(m_strDisplay.at(i), -1);
                addNewLetter(i);
            }
            else {
                m_pointDisplay.replace(i, QPoint(m_pointDisplay.at(i).x(), _pointY));
            }
        }
        update();
    }
}

void MainWindow::goal(QString letter, int score)
{
    mGameScore += score;
    ui->textEdit->append(letter + " : " + QString::number(score));
    ui->lcdNumber->display(mGameScore);
}

int MainWindow::eliminate(QString input)
{
    QHash<int, int> repeatLetter;
    QVector<int> repeatColumn;
    int res = -1;

    for(int i = 0; i < m_strDisplay.size(); i++)
    {
        if(input == m_strDisplay.at(i))
        {
            repeatLetter.insert(i ,m_pointDisplay.at(i).y());
            repeatColumn.append(m_pointDisplay.at(i).y());
            res = 1;
        }
    }

    if(res == -1)
    {
        return res;
    }

    int pointColumn = 0;
    if(repeatColumn.size() == 1)
    {
        pointColumn = repeatLetter.constBegin().key();
    }
    else {
        mySort(repeatColumn);
        for(auto i = repeatLetter.constBegin(); i != repeatLetter.constEnd(); i++)
        {
            int _column = repeatColumn.last();
            if(_column == i.value())
            {
                pointColumn = i.key();
                break;
            }
        }
    }

    addNewLetter(pointColumn);
    return res;
}

void MainWindow::addNewLetter(int column)
{
    int random = QRandomGenerator::global()->bounded(m_letterVer.size() - 1);
    QString letter = m_letterVer.at(random);

    m_strDisplay.replace(column, letter);
    m_pointDisplay.replace(column, QPoint(column * m_rank.at(mGameDifficulty).at(1), 0));
}

void MainWindow::mySort(QVector<int>& ver)
{
    for(int i = 0; i < ver.size() - 1; i++)
    {
        for(int j = i + 1; j < ver.size(); j++)
        {
            if(ver[j] < ver[i])
            {
                int _temp;
                _temp = ver[j];
                ver[j] = ver[i];
                ver[i] = _temp;
            }
        }
    }
}

void MainWindow::btnInitializeControl()
{
    if(ui->pushButton_control->text() == "开始")
    {
        m_timer->start(100);
        ui->pushButton_control->setText("暂停");
        ui->comboBox->setEnabled(false);
    }
    else {
        m_timer->stop();
        ui->pushButton_control->setText("开始");
    }
}

void MainWindow::on_pushButton_control_clicked(bool checked)
{
    Q_UNUSED(checked)

    if(!mDifficultyEnable)
    {
        initializeDifficulty();
        mDifficultyEnable = true;
        ui->pushButton_restart->setEnabled(true);
        btnInitializeControl();
    }
    else {
        adjustPainter();
        btnInitializeControl();
    }
}

void MainWindow::on_pushButton_restart_clicked(bool checked)
{
    Q_UNUSED(checked)

    mGameScore = 0;
    ui->comboBox->setEnabled(false);
    ui->textEdit->clear();
    ui->pushButton_control->setText("暂停");
    ui->pushButton_control->setEnabled(true);
    ui->lcdNumber->display(mGameScore);
    adjustPainterRestart();
    m_timer->start(100);
}

void MainWindow::on_pushButton_toMenu_clicked(bool checked)
{
    Q_UNUSED(checked)

    emit toMenu();
}

void MainWindow::on_comboBox_currentIndexChanged(const QString &arg1)
{
    Q_UNUSED(arg1)

    mGameDifficulty = ui->comboBox->currentIndex();
}

QString MainWindow::getKeyValue(QKeyEvent* event)
{
    int keyValue;
    QString Keyboard_Key;

    keyValue = event->key();
    Keyboard_Key = QString(keyValue);

    switch (keyValue)
    {
    case Qt::Key_Return:
        Keyboard_Key = QString("Enter(main)");
        break;
    case Qt::Key_Enter:
        Keyboard_Key = QString("Enter(num)");
        break;
    case Qt::Key_Escape:
        Keyboard_Key = QString("Esc");
        break;
    case Qt::Key_Tab:
        Keyboard_Key = QString("Tab");
        break;
    case Qt::Key_CapsLock:
        Keyboard_Key = QString("CapsLock");
        break;
    case Qt::Key_Shift:
        Keyboard_Key = QString("Shift");
        break;
    case Qt::Key_Control:
        Keyboard_Key = QString("Ctrl");
        break;
    case Qt::Key_Alt:
        Keyboard_Key = QString("Alt");
        break;
    case Qt::Key_Backspace:
        Keyboard_Key = QString("Backspace");
        break;
    case Qt::Key_Meta:
        Keyboard_Key = QString("Win");
        break;
    case Qt::Key_Home:
        Keyboard_Key = QString("Home");
        break;
    case Qt::Key_End:
        Keyboard_Key = QString("End");
        break;
    case Qt::Key_PageUp:
        Keyboard_Key = QString("PageUp");
        break;
    case Qt::Key_PageDown:
        Keyboard_Key = QString("PageDown");
        break;
    case Qt::Key_Insert:
        Keyboard_Key = QString("Insert");
        break;
    case Qt::Key_Up:
        Keyboard_Key = QString("↑");
        break;
    case Qt::Key_Right:
        Keyboard_Key = QString("→");
        break;
    case Qt::Key_Left:
        Keyboard_Key = QString("←");
        break;
    case Qt::Key_Down:
        Keyboard_Key = QString("↓");
        break;
    case Qt::Key_Delete:
        Keyboard_Key = QString("Delete");
        break;
    case Qt::Key_Space:
        Keyboard_Key = QString("Space");
        break;
    case Qt::Key_F1:
        Keyboard_Key = QString("F1");
        break;
    case Qt::Key_F2:
        Keyboard_Key = QString("F2");
        break;
    case Qt::Key_F3:
        Keyboard_Key = QString("F3");
        break;
    case Qt::Key_F4:
        Keyboard_Key = QString("F4");
        break;
    case Qt::Key_F5:
        Keyboard_Key = QString("F5");
        break;
    case Qt::Key_F6:
        Keyboard_Key = QString("F6");
        break;
    case Qt::Key_F7:
        Keyboard_Key = QString("F7");
        break;
    case Qt::Key_F8:
        Keyboard_Key = QString("F8");
        break;
    case Qt::Key_F9:
        Keyboard_Key = QString("F9");
        break;
    case Qt::Key_F10:
        Keyboard_Key = QString("F10");
        break;
    case Qt::Key_F11:
        Keyboard_Key = QString("F11");
        break;
    case Qt::Key_F12:
        Keyboard_Key = QString("F12");
        break;
    case Qt::Key_NumLock:
        Keyboard_Key = QString("NumLock");
        break;
    case Qt::Key_ScrollLock:
        Keyboard_Key = QString("ScrollLock");
        break;
    case Qt::Key_Pause:
        Keyboard_Key = QString("Pause|Break");
        break;
    }

    return Keyboard_Key;
}
