#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mainMenu(nullptr)
{
    ui->setupUi(this);

    FOR_INFO = new QGridLayout(this);
    FILE_MANAGER = new QGridLayout(this);

    dialogForHotkeys = new HotKeys;
    dialogForHotkeys->setMinimumSize(250,250);
    dialogForHotkeys->setMaximumSize(250,250);

    keys = dialogForHotkeys->getKeys();

    filename = "";
    file.setFileName(filename);

    labelForInfo = new QLabel;
    labelForInfo->resize(200,120);

    QFile info("about.txt");
    if(info.open(QIODevice::ReadOnly))
        labelForInfo->setText(info.readAll());
    else
        labelForInfo->setText("Error open file");

    forInfo = new QWidget;
    forInfo->setMaximumSize(200,200);
    forInfo->setMinimumSize(200,200);
    forInfo->setLayout(FOR_INFO);

    FOR_INFO->addWidget(labelForInfo);

    currentPath = QDir::currentPath();

    controllerl = new Controller(this);
    fileManager = new QWidget;
    fileManager->setMaximumSize(1100,550);
    fileManager->setMinimumSize(1100,550);
    fileManager->setLayout(FILE_MANAGER);

    disks = new QComboBox(this);
    disks->setMinimumSize(100,25);
    disks->setMaximumSize(100,25);
    if (QSysInfo::productType() == "windows"){
        QFileInfoList diskList = QDir::drives();
        for(auto &it:diskList)
            disks->addItem(it.path());
    }
    else {
        QStringList str = {"/", "/home/", "/mount/", "/opt/"};
        for(auto &it:str)
            disks->addItem(it);
    }
    FILE_MANAGER->addWidget(disks,0,0,1,1,Qt::AlignLeft);
    connect(disks,SIGNAL(textActivated(const QString&)),this,SLOT(changeDir(const QString&)));

    outResult = new QTreeView(this);
    outResult->setMinimumSize(500,400);
    outResult->setMaximumSize(500,400);
    changeDir(currentPath);
    connect(outResult,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(selectDirDoubleClick(QModelIndex)));
    FILE_MANAGER->addWidget(outResult,1,0,1,0,Qt::AlignLeft);

    finedOut = new QTextEdit(this);
    finedOut->setMinimumSize(500,400);
    finedOut->setMaximumSize(500,400);
    finedOut->setReadOnly(true);
    FILE_MANAGER->addWidget(finedOut,1,2,1,1,Qt::AlignRight);

    searchEnter = new QLineEdit(this);
    searchEnter->setMinimumSize(250,25);
    searchEnter->setMaximumSize(250,25);
    FILE_MANAGER->addWidget(searchEnter,2,1,1,1,Qt::AlignCenter);

    searchButton = new QPushButton(this);
    searchButton->setMinimumSize(250,25);
    searchButton->setMaximumSize(250,25);
    connect(searchButton,SIGNAL(clicked()),this,SLOT(findFileSlot()));
    FILE_MANAGER->addWidget(searchButton,3,1,1,1,Qt::AlignCenter);

    connect(controllerl, SIGNAL(genPathOfFile(QString)), this,SLOT(addFinedtoList(QString)));
    connect(controllerl, SIGNAL(changFindPath(QString)), this,SLOT(finishFind(QString)));

//////////////////////////////////////////
    mainMenu = new QMenu(this);
    ui->pushButton_3->setMenu(mainMenu);
    connect(mainMenu,SIGNAL(triggered(QAction*)),this,SLOT(clickMenu(QAction*)));

    formatTextMenu = new QMenu(this);
    ui->pushButton_6->setMenu(formatTextMenu);
    connect(formatTextMenu,SIGNAL(triggered(QAction*)),this,SLOT(clickFormatText(QAction*)));
//////////////////////////////////////////
    MainWindow::retranslateUi();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete forInfo;
    delete labelForInfo;
    delete dialogForHotkeys;
    delete mainMenu;
    delete formatTextMenu;
    delete outResult;
    delete searchEnter;
    delete disks;
    delete searchButton;
    delete fileManager;
    delete FOR_INFO;
    delete FILE_MANAGER;
    delete controllerl;
    delete finedOut;
}

void MainWindow::on_pushButton_clicked()
{
    filename = QFileDialog::getOpenFileName(this,tr("Select file txt"),QDir::currentPath(),"Text(*.txt)");
    if(filename.size()){
        file.setFileName(filename);
        if(file.open(QIODevice::ReadWrite)){
            QByteArray text = file.readAll();
            ui->textEdit->setPlainText(text);
        }
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    filename = QFileDialog::getSaveFileName(this,tr("Save file txt"),filename,"Text(*.txt)");
    if(filename.size()){
        file.setFileName(filename);
        if(file.open(QIODevice::WriteOnly)){
            QString str = ui->textEdit->toPlainText();
            QByteArray text = str.toUtf8();
            file.write(text);
            file.close();
            ui->textEdit->setPlainText("");
        }
    }
}


void MainWindow::on_pushButton_3_clicked(){

}


void MainWindow::on_pushButton_4_clicked()
{
    forPaste.first = ui->textEdit->textCursor().selectedText();
    forPaste.second = ui->textEdit->textCursor().charFormat();
}


void MainWindow::on_pushButton_5_clicked()
{
    if(!forPaste.second.isEmpty())
        ui->textEdit->textCursor().insertText(forPaste.first,forPaste.second);
}


void MainWindow::on_pushButton_7_clicked()
{
    fileManager->show();
}


void MainWindow::on_checkBox_stateChanged(int arg1)
{
    ui->textEdit->setReadOnly(arg1);
}


void MainWindow::keyPressEvent(QKeyEvent *myKey){
    bool ctrl = myKey->modifiers() & Qt::ControlModifier;

    if(ctrl){
        for(int i=0;i<4;i++)
            if(keys[i] == myKey->key())
                switch (i) {
                case 0: MainWindow::on_pushButton_clicked();break;
                case 1: MainWindow::on_pushButton_2_clicked();break;
                case 2: createNewFile(); break;
                case 3: MainWindow::close();
                }
    }
}


void MainWindow::createNewFile(){
    filename = QFileDialog::getSaveFileName(this,tr("Create file txt"),filename,"Text(*.txt)");
    if(filename.size()){
        file.setFileName(filename);
        if(file.open(QIODevice::ReadWrite)){
            QByteArray text = file.readAll();
            ui->textEdit->setPlainText(text);
        }
    }
}


void MainWindow::MenuRetranslateAndCreate(){
    mainMenu->clear();

    mainMenu->addAction(tr("1) Information"));
    mainMenu->addAction(tr("2) Hotkeys"));
    QMenu *languageMenu = mainMenu->addMenu(tr("3) Language"));
    languageMenu->addAction("3.1) ENG");
    languageMenu->addAction("3.2) RUS");
    QMenu *themeMenu = mainMenu->addMenu(tr("4) Theme"));
    themeMenu->addAction(tr("4.1) Light"));
    themeMenu->addAction(tr("4.2) Dark"));
    mainMenu->addAction(tr("5) Print"));

    formatTextMenu->clear();
    QMenu *colors = formatTextMenu->addMenu(tr("1) Colors"));
    colors->addAction(tr("1.1) Red"));
    colors->addAction(tr("1.2) Black"));
    colors->addAction(tr("1.3) Yellow"));
    colors->addAction(tr("1.4) Green"));
    QMenu *colorsBack = formatTextMenu->addMenu(tr("2) Colors Backgroud"));
    colorsBack->addAction(tr("2.1) Red"));
    colorsBack->addAction(tr("2.2) Black"));
    colorsBack->addAction(tr("2.3) Yellow"));
    colorsBack->addAction(tr("2.4) Green"));
    QMenu *alignments = formatTextMenu->addMenu(tr("3) Alignment"));
    alignments->addAction(tr("3.1) Left"));
    alignments->addAction(tr("3.2) Right"));
    alignments->addAction(tr("3.3) Midl"));
    formatTextMenu->addAction(tr("4) Size"));
    formatTextMenu->addAction(tr("5) Insert time"));

}


QString MainWindow::getDateTime(){
    QDate date = QDate::currentDate();
    QTime time = QTime::currentTime();

    QString get {""};

    if(date.day()<10) get = "0";
    get+= QString::number(date.day()) + ".";

    if(date.month()<10) get+="0";
    get+= QString::number(date.month()) + "." + QString::number(date.year()) + " ";


    if(time.hour()<10) get+="0";
    get+= QString::number(time.hour()) + ":";

    if(time.minute()<10) get+="0";
    get+= QString::number(time.minute()) + ":";

    if(time.second()<10) get+="0";
    get+= QString::number(time.second());

    return get;
}


void MainWindow::retranslateUi(){
    setWindowTitle(tr("Text editer"));

    ui->pushButton   ->  setText(tr("Open file"));
    ui->pushButton_2 ->  setText(tr("Save file"));
    ui->pushButton_3 ->  setText(tr("Menu"));
    ui->pushButton_4 ->  setText(tr("Copy"));
    ui->pushButton_5 ->  setText(tr("Paste"));
    ui->pushButton_6 ->  setText(tr("Format text"));
    ui->pushButton_7 ->  setText(tr("Search"));
    ui->checkBox     ->  setText(tr("Only read"));
    forInfo          ->  setWindowTitle(tr("Info"));
    fileManager      ->  setWindowTitle(tr("File Manager"));
    searchButton     ->  setText(tr("Search file"));

    MenuRetranslateAndCreate();

    dialogForHotkeys->retranslateUi();

}


void MainWindow::clickMenu(QAction *action){
    QString text = action->text();

    if(text[0] == '1'){
        forInfo->show();
    }
    else if(text[0] == '2'){
        dialogForHotkeys->setModal(true);
        dialogForHotkeys->exec();
        keys = dialogForHotkeys->getKeys();
    }
    else if(text[0] == '3'){
        if(text[2] == '1'){
            qApp->removeTranslator(&translater);
            dialogForHotkeys->RemoveLanguage(translater);
        }
        else{
            if(translater.load("switch_ru")){
                qApp->installTranslator(&translater);
                dialogForHotkeys->SetLanguage(translater);
            }
        }
        MainWindow::retranslateUi();
    }
    else if(text[0] == '4'){
        if(text[2] == '1')
            qApp->setStyleSheet("");
        else {
            qApp->setStyleSheet("QPushButton,QComboBox,QPlainTextEdit,QLabel,QCheckBox,QLineEdit {background-color: #2F4F4F; color: #FFFFFF; border-color: white} "
                                    "QMainWindow,QDialog {background-color: black}");
        }
    }
    else if(text[0] == '5'){
        QPrinter printer;

        QPrintDialog dlg(&printer,this);
        dlg.setWindowTitle("Print");

        if(dlg.exec()==QDialog::Accepted)
            ui->textEdit->print(&printer);
    }
}


void MainWindow::clickFormatText(QAction *action){
    QString text = action->text();

    if(text[0] == '1'){
        if(text[2] == '1'){
            textFormat = ui->textEdit->textCursor().charFormat();
            textFormat.setForeground(QBrush(QColor("red")));
            ui->textEdit->textCursor().setCharFormat((textFormat));
        }
        else if(text[2] == '2'){
            textFormat = ui->textEdit->textCursor().charFormat();
            textFormat.setForeground(QBrush(QColor("black")));
            ui->textEdit->textCursor().setCharFormat((textFormat));
        }
        else if(text[2] == '3'){
            textFormat = ui->textEdit->textCursor().charFormat();
            textFormat.setForeground(QBrush(QColor("yellow")));
            ui->textEdit->textCursor().setCharFormat((textFormat));
        }
        else if(text[2] == '4'){
            textFormat = ui->textEdit->textCursor().charFormat();
            textFormat.setForeground(QBrush(QColor("green")));
            ui->textEdit->textCursor().setCharFormat((textFormat));
        }
    }
    else if(text[0] == '2'){
        if(text[2] == '1'){
            textFormat = ui->textEdit->textCursor().charFormat();
            textFormat.setBackground(QBrush(QColor("red")));
            ui->textEdit->textCursor().setCharFormat((textFormat));
        }
        if(text[2] == '2'){
            textFormat = ui->textEdit->textCursor().charFormat();
            textFormat.setBackground(QBrush(QColor("black")));
            ui->textEdit->textCursor().setCharFormat((textFormat));
        }
        else if(text[2] == '3'){
            textFormat = ui->textEdit->textCursor().charFormat();
            textFormat.setBackground(QBrush(QColor("yellow")));
            ui->textEdit->textCursor().setCharFormat((textFormat));
        }
        else if(text[2] == '4'){
            textFormat = ui->textEdit->textCursor().charFormat();
            textFormat.setBackground(QBrush(QColor("green")));
            ui->textEdit->textCursor().setCharFormat((textFormat));
        }
    }
    else if(text[0] == '3'){
        if(text[2] == '1')
            ui->textEdit->setAlignment(Qt::AlignLeft);
        else if(text[2] == '2')
            ui->textEdit->setAlignment(Qt::AlignRight);
        else if(text[2] == '3')
            ui->textEdit->setAlignment(Qt::AlignCenter);
    }
    else if(text[0] == '4'){
        QFont font = ui->textEdit->textCursor().charFormat().font();

        QFontDialog fontDialog(font,this);
        bool *b = new bool{true};

        font = fontDialog.getFont(b);

        if(*b){
            textFormat = ui->textEdit->textCursor().charFormat();
            textFormat.setFont(font);
            ui->textEdit->textCursor().setCharFormat(textFormat);
        }
    }
    else if(text[0] == '5'){
        ui->textEdit->textCursor().insertText(getDateTime());
    }
}


void MainWindow::setNewModel(QStandardItemModel *newModel){
    currentModel = newModel;
    outResult->setModel(currentModel);
}


void MainWindow::printDirsFiles(const QString& path){
    currentPath = path;

    QStandardItemModel *model = new QStandardItemModel(this);
    QList<QStandardItem*> items;

    items.append(new QStandardItem(QIcon(QApplication::style()->standardIcon(QStyle::SP_DriveHDIcon)),path));

    model->appendRow(items);

    QDir dir(path);
    dir.setFilter(QDir::Hidden | QDir::NoSymLinks | QDir::Dirs);
    QStringList list = dir.entryList();
    QList<QStandardItem*>folders;
    for (auto &it:list){
        QStandardItem* f = new QStandardItem(QIcon(QApplication::style()->standardIcon(QStyle::SP_DirIcon)),it);
        folders.append(f);
    }

    items.at(0)->appendRows(folders);

    dir.setFilter(QDir::Hidden | QDir::NoSymLinks | QDir::Files);
    list = dir.entryList();
    QList<QStandardItem*>files;

    for (auto &it:list){
        QStandardItem* f = new QStandardItem(QIcon(QApplication::style()->standardIcon(QStyle::SP_FileIcon)),it);
        files.append(f);
    }

    items.at(0)->appendRows(files);
    setNewModel(model);
}


void MainWindow::changeDir(const QString& selectDisk){
    printDirsFiles(selectDisk);
}


void MainWindow::selectDirDoubleClick(QModelIndex it){
    QString DirName = it.data().toString();

    if(DirName == "." || DirName == ".."){
        auto end = currentPath.end() - 2;
        while(end != currentPath.begin() && *end != '/') end--;
        currentPath.erase(end+1,currentPath.end());
        changeDir(currentPath);
    }
    else{
        QString tempPath = currentPath + DirName + '/';
        QDir tempDir;
        if(tempDir.cd(tempPath)){
            currentPath = tempPath;
            changeDir(currentPath);
        }
    }
}


void MainWindow::findFileSlot()
{
    QString linesearch = searchEnter->text();
    if (linesearch.length() == 0) return;
    finedOut->setText("Start finding\n\n");
    controllerl->startFind(currentPath, linesearch);
}


void MainWindow::addFinedtoList(QString path){
    finedOut->setText(finedOut->toPlainText()+path+"\n\n");
}


void MainWindow::finishFind(QString str){
    if(str == "FINISH"){
        finedOut->setText(finedOut->toPlainText() + "\nFINISH");
    }
}
