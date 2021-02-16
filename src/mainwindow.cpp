#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <findwidget.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->verticalScrollBar->installEventFilter(this);
    ui->plainTextEdit->installEventFilter(this);

}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadFile(QString path){
fileStream = new std::ifstream(path.toStdString(), std::ifstream::binary);
fileStream->seekg(0,std::ios::end);
fileEndPosition = fileStream->tellg();
}

void MainWindow::populateText(int position, int mode){

    // In case where no file is open, ignore populate text
    if(fileStream == nullptr) return;

//    // Move stream cursor to the beginning
//     fileStream->seekg(0,std::ios::beg);

     fileStream->seekg(position);

     // In the case where the end of the file has been reached
     if(fileStream->tellg() >= fileEndPosition && mode == loadMode::forward){
         return;
     }



     if(mode == forward){
         //Move forward one line.
     for(int i = 0; i < 1; i++){
         // Ignore up to newline char
         fileStream->ignore(INT32_MAX,'\n');
     }
     }else if(mode == backward){
         cursorToPrevLine(fileStream);
//         MainWindow::prevStartPosition = inf.tellg();
     }
     qDebug() << fileStream->tellg();
     MainWindow::currentPosition = fileStream->tellg();

     // v2 of text reading
          ui->plainTextEdit->setPlainText("");
          QFont myFont(ui->plainTextEdit->font());
          QFontMetrics fm(myFont);
          QString finalString = "";
          for(int i = 0; i < ui->plainTextEdit->maximumBlockCount() - 1; i++){
//              std::getline(inf, line);
              // While the length of the string + char < line Size
              QString lineString = "";
              QString appendNewLine = (i+1 != ui->plainTextEdit->maximumBlockCount() - 1)?"\n":"";
              while(fileStream->tellg() != fileEndPosition){
                  char c = fileStream->get();
                  QString tstring(lineString + c);
                  int width=fm.horizontalAdvance(tstring);
                  if(c == '\n'){
                      if(tstring == "\n"){
                          finalString += appendNewLine;
                      }else{
                              finalString += lineString + appendNewLine;
                      }
                      break;
                      // Plus the width in which the scrollbar will show
                      // 22 is the measured width of the scrollbar
                      // Maybe programatically calculate it here if possible
                  } else if(width > ui->plainTextEdit->width() - 22){
                      finalString += lineString + appendNewLine;
                      fileStream->seekg(-1, std::ios::cur);
                      break;
                  }else{
                      lineString = tstring;
                      continue;
                  }

              }
          }
          ui->plainTextEdit->setPlainText(finalString);

          if(mode != loadMode::render){ scrollbarControlled = true;
              // Definitely introducing more bugs
     ui->verticalScrollBar->setValue(((float)currentPosition/fileEndPosition)*ui->verticalScrollBar->maximum());
          }


}

void MainWindow::cursorToPrevLine(std::ifstream* stream){
    // 0. store currentaddress
    int startAddress = stream->tellg();

    int currentAddress = startAddress;
    bool prevLineFound = false;
    // 1. Go back (word wrap length + 1)
    for(int i = 0; i < 250; i++){
        if(currentAddress == 0){
            qDebug() << "Beginning found";
            return;
        }
        currentAddress--;
        stream->seekg(-1, std::ios::cur);
        char currentChar = stream->get();
        if(!prevLineFound && currentChar == '\n'){
            prevLineFound = true;
            i = 0;
            qDebug() << "Prev Line Found 1";
            stream->seekg(-1, std::ios::cur);
        }else if(prevLineFound && currentChar == '\n'){
            // Beginning of previous line found
            qDebug() << "Prev Line Found 2";
            break;
        }else{
            stream->seekg(-1, std::ios::cur);
        }
    }
return;
}

bool MainWindow::eventFilter(QObject *object, QEvent *event)
{

    // Initial Load to calculate the bounds of the textbox. Do when the textbox is activated so it can be calculated.
    if(object == ui->plainTextEdit && event->type() == QEvent::WindowActivate){
         recalcBounds();
    }
    if((object == ui->plainTextEdit && event->type() == QEvent::Wheel)){
        if( ((QWheelEvent*) event)->angleDelta().y() < 0){
            populateText(MainWindow::currentPosition,loadMode::forward);
        }else{
            populateText(MainWindow::currentPosition,loadMode::backward);
        }

    }
    return false;
}

void MainWindow::recalcBounds(){
    QFont myFont(ui->plainTextEdit->font());
    QFontMetrics fm(myFont);
    int fontHeight=fm.boundingRect("string").height();
    int numOfLinesPossible = ui->plainTextEdit->height() / fontHeight;
    ui->plainTextEdit->setMaximumBlockCount(numOfLinesPossible);
    qDebug() << "recalc text";
    populateText(currentPosition,loadMode::render);

}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    // Called when program first starts aswell
    qDebug() << "Recalc";
    recalcBounds();
}

void MainWindow::on_actionOpen_triggered()
{
    qDebug() << "Triggered";
    loadFile();
    populateText(0,loadMode::render);
}

void MainWindow::on_verticalScrollBar_valueChanged(int value)
{
    // In the case where the scrollbar was programatically updated
    // Ignore the value changed event.
    if(scrollbarControlled){
        scrollbarControlled = false;
        return;
    }

    qDebug() << "value: " << (double)value/ui->verticalScrollBar->maximum();

    double percentage = (double)value/ui->verticalScrollBar->maximum();
    currentPosition = percentage * fileEndPosition;

    qDebug() << "newPosition: " << currentPosition;

    populateText(currentPosition,loadMode::render);

}

void MainWindow::on_actionFind_triggered()
{
    FindWidget *w = new FindWidget;
    w->setAttribute(Qt::WA_DeleteOnClose);
    w->show();
}

void MainWindow::on_actionExit_triggered()
{
    exit(0);
}
