/***************************************************************************
 *   Copyright (C) 2011~2011 by CSSlayer, ukyoi                            *
 *   wengxt@gmail.com                                                      *
 *   ukyoi@msn.com                                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QWidget>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDebug>
#include <QPainter>
#include <QString>
#include <QFont>
#include <QMessageBox>

#include <fcitx/ui.h>
#include <fcitx/fcitx.h>
#include <fcitx-config/fcitx-config.h>

#include "MainWindow.h"
#include <qvarlengtharray.h>

MainWindow::MainWindow()
{
    this->setupUi ( this );
    connect(openButton, SIGNAL(clicked()), this, SLOT(openButtonPushed()));
    GenList();

}

void MainWindow::openButtonPushed()
{
    /**
     * 按钮被按下时，通过指定的conf文件打开一个指定的皮肤。
     */

    // FIXME:
    // How to create a directory-selecting dialog that can deal with hidden dirs?
    /*
    skinPath=QFileDialog::getExistingDirectory(this, tr("Open Skin Directory"), "~/.config/fcitx/skin/", QFileDialog::HideNameFilterDetails);
    qDebug() << skinPath;
    QFile confFile(skinPath + '/' + "fcitx_skin.conf");
    if (confFile.exists()) {
        redrawButtonPushed();
    } else {
        QMessageBox errorMessage;
        errorMessage.setWindowTitle("Open Directory error");
        errorMessage.setText(tr("This seems not a proper fcitx skin directory."));
        errorMessage.exec();
    }
    */
    skinPath=QFileDialog::getOpenFileName( this, tr("Open Skin Config"), QString(QDir::homePath()+"/.config/fcitx/skin/"), tr("Fcitx skin config file (fcitx_skin.conf)") );
    QFile confFile(skinPath);
    if (skinPath=="") {
        QMessageBox errorMessage;
        errorMessage.setWindowTitle(tr("Open Config File Error"));
        errorMessage.setText(tr("Seems you didn't choose anything."));
        errorMessage.exec();
    } else {
        skinPath.replace("/fcitx_skin.conf", "");
        redrawButtonPushed();
    }

}

void MainWindow::redrawButtonPushed()
{
    /**
     * 按钮被按下时，重新绘制所有图片。
     */

    MyLoadConfig *skinClass=new MyLoadConfig(skinPath);
    DrawAllThings(*skinClass, skinPath);
    delete skinClass;
}

void MainWindow::DrawAllThings(MyLoadConfig skinClass, QString skinPath)
{
    /**
     * 重新绘制所有图片。
     */

    QPixmap inputDestPixmap (0, 0);  // The size of this map should be modified by DrawResizableBackground.
    QPixmap mainBarDestPixmap (0, 0); // The size of this map should be modified by DrawMainBar.
    QPixmap mainBarDestPixmap_2 (0, 0); // The size of this map should be modified by DrawMainBar.
    QPixmap menuDestPixmap (0, 0); // The size of this map should be modified by DrawMenu.

    DrawInputBar(inputDestPixmap, skinClass.skin, skinPath);
    inputWindowLabel->setPixmap(inputDestPixmap);

    DrawMainBar(mainBarDestPixmap, skinClass.skin, skinPath);
    mainBarLabel->setPixmap(mainBarDestPixmap);

    DrawMainBar(mainBarDestPixmap_2, skinClass.skin, skinPath, false, false, false, false, false, false);
    mainBarLabel_2->setPixmap(mainBarDestPixmap_2);

    DrawMenu(menuDestPixmap, skinClass.skin, skinPath);
    menuLabel->setPixmap(menuDestPixmap);
};

QColor MainWindow::GetIntColor(ConfigColor floatColor)
{
    /**
     * 把浮点颜色转化成RGB整数颜色。
     */

    short r=(int)(floatColor.r*256);
    short g=(int)(floatColor.g*256);
    short b=(int)(floatColor.b*256);
    switch (r) {
        case 256 : r=255; break;
    }
    switch (g) {
        case 256 : g=255; break;
    }
    switch (b) {
        case 256 : b=255; break;
    }

    QColor converted(r, g, b);
    return converted;
}

void MainWindow::DrawResizableBackground (
    QPixmap &destPixmap,
    QPixmap &backgroundPixmap,
    int marginLeft,
    int marginRight,
    int marginTop,
    int marginBottom,
    int resizeWidth,
    int resizeHeight
)
{
    /**
     * 把一个指定的九宫格图片以resizeWidth和resizeHeight为中央区大小画进destPixmap。
     */

    int originWidth=backgroundPixmap.width() - marginLeft - marginRight;
    int originHeight=backgroundPixmap.height() - marginTop - marginBottom;

    if ( resizeWidth <= 0 )
        resizeWidth = 1;
    if ( resizeHeight <= 0 )
        resizeHeight = 1;

    destPixmap=QPixmap(resizeWidth + marginLeft + marginRight, resizeHeight + marginTop + marginBottom);
    destPixmap.fill ( Qt::transparent );
    QPainter painter ( &destPixmap );


    /* 画背景 */

    /* 九宫格
     * 7 8 9
     * 4 5 6
     * 1 2 3
     */
    /* part 1 */
    painter.drawPixmap(
        QRect(0, marginTop + resizeHeight, marginLeft, marginBottom),
        backgroundPixmap,
        QRect(0, marginTop + originHeight, marginLeft, marginBottom)
    );

    /* part 3 */
    painter.drawPixmap(
        QRect(marginLeft + resizeWidth, marginTop + resizeHeight, marginRight, marginBottom),
        backgroundPixmap,
        QRect(marginLeft + originWidth, marginTop + originHeight, marginRight, marginBottom)
    );

    /* part 7 */
    painter.drawPixmap(
        QRect(0 , 0, marginLeft, marginTop),
        backgroundPixmap,
        QRect(0, 0, marginLeft, marginTop)
    );

    /* part 9 */
    painter.drawPixmap(
        QRect(marginLeft + resizeWidth, 0, marginRight, marginTop),
        backgroundPixmap,
        QRect(marginLeft + originWidth, 0, marginRight, marginTop)
    );

    /* part 8 & 2 */
    painter.drawPixmap(
        QRect(marginLeft, 0, resizeWidth, marginTop),
        backgroundPixmap,
        QRect(marginLeft, 0, originWidth, marginTop)
    );
    painter.drawPixmap(
        QRect(marginLeft, marginTop + resizeHeight, resizeWidth, marginBottom),
        backgroundPixmap,
        QRect(marginLeft, marginTop + originHeight, originWidth, marginBottom)
    );

    /* part 4 & 6 */
    painter.drawPixmap(
        QRect(0, marginTop , marginLeft, resizeHeight),
        backgroundPixmap,
        QRect(0, marginTop , marginLeft, originHeight)
    );

    painter.drawPixmap(
        QRect(marginLeft + resizeWidth, marginTop , marginRight, resizeHeight),
        backgroundPixmap,
        QRect(marginLeft + originWidth, marginTop , marginRight, originHeight)
    );

    /* part 5 */
    painter.drawPixmap(
        QRect(marginLeft, marginTop , resizeWidth, resizeHeight),
        backgroundPixmap,
        QRect(marginLeft, marginTop , originWidth, originHeight)
    );
    painter.end();
}

void MainWindow::DrawWidget (
    QPixmap &destPixmap, QPixmap &widgetPixmap,
    int x, int y
)
{
    /**
     * 在指定位置(x,y)按照原大小画出一个小部件。
     */

    QPainter painter(&destPixmap);
    painter.drawPixmap( x, y, widgetPixmap );
    painter.end();
}

void MainWindow::DrawMainBar( QPixmap &destPixmap, FcitxSkin &skin, QString skinPath,
                             bool chnIsActive,
                             bool vkIsActive,
                             bool chttransIsActive,
                             bool puncIsActive,
                             bool fullwidthIsActive,
                             bool remindIsActive )
{
    QPixmap *mainBarIcons=new QPixmap[7];
    QPixmap mainBarPixmap( QString(skinPath + '/' + skin.skinMainBar.backImg) );
    mainBarIcons[0]=( QString(skinPath + '/' + skin.skinMainBar.logo) );
    if (chnIsActive)
        mainBarIcons[1]=( QString(skinPath + '/' + skin.skinMainBar.active) );
    else
        mainBarIcons[1]=( QString(skinPath + '/' + skin.skinMainBar.eng) );
    if (chttransIsActive)
        mainBarIcons[2]=( QString(skinPath + "/chttrans_active.png") );
    else
        mainBarIcons[2]=( QString(skinPath + "/chttrans_inactive.png") );
    if (vkIsActive)
        mainBarIcons[3]=( QString(skinPath + "/vk_active.png") );
    else
        mainBarIcons[3]=( QString(skinPath + "/vk_inactive.png") );
    if (puncIsActive)
        mainBarIcons[4]=( QString(skinPath + "/punc_active.png") );
    else
        mainBarIcons[4]=( QString(skinPath + "/punc_inactive.png") );
    if (fullwidthIsActive)
        mainBarIcons[5]=( QString(skinPath + "/fullwidth_active.png") );
    else
        mainBarIcons[5]=( QString(skinPath + "/fullwidth_inactive.png") );
    if (remindIsActive)
        mainBarIcons[6]=( QString(skinPath + "/remind_active.png") );
    else
        mainBarIcons[6]=( QString(skinPath + "/remind_inactive.png") );

    int marginLeft=skin.skinMainBar.marginLeft;
    int marginRight=skin.skinMainBar.marginRight;
    int marginTop=skin.skinMainBar.marginTop;
    int marginBottom=skin.skinMainBar.marginBottom;

    int resizeWidth=0;
    int resizeHeight=0;

    for (int i=0; i<7; i++ ) {
        resizeWidth += mainBarIcons[i].width();
    }
    for (int i=0; i<7; i++ ) {
        if (resizeHeight < mainBarIcons[i].height()) {
            resizeHeight = mainBarIcons[i].height();
        }
    }

    int totalWidth=resizeWidth+marginTop+marginBottom;
    int totalHeight=resizeHeight+marginLeft+marginRight;

    destPixmap=QPixmap(totalWidth, totalHeight);

    DrawResizableBackground(destPixmap, mainBarPixmap,
                            marginLeft, marginRight, marginTop, marginBottom,
                            resizeWidth, resizeHeight
    );

    int offset=0;
    for (int i=0; i<7; i++) {
        DrawWidget(destPixmap, mainBarIcons[i], marginLeft + offset, marginTop);
        offset += mainBarIcons[i].width();
    }

    delete [] mainBarIcons;
}

void MainWindow::DrawInputBar(QPixmap &destPixmap, FcitxSkin& skin, QString skinPath)
{
    /**
     * 绘制输入条
     */

    int marginLeft=skin.skinInputBar.marginLeft;
    int marginRight=skin.skinInputBar.marginRight;
    int marginTop=skin.skinInputBar.marginTop;
    int marginBottom=skin.skinInputBar.marginBottom;

    // Draw Demo string:
    QString inputExample(tr("input example"));
    QString numberStr[3];
    QString candStr[3];
    for (int i=0; i<3; i++) {
        numberStr[i]=QString("%1.").arg(i+1);
    }
    candStr[0]=QString(tr("First candidate "));
    candStr[1]=QString(tr("User phrase "));
    candStr[2]=QString(tr("Others "));
    int offset=marginLeft;

    QFont inputFont("");
    int fontHeight=skin.skinFont.fontSize;
    QFontMetrics metrics(inputFont);
    inputFont.setPixelSize(fontHeight);

    // inputPos & outputPos is the LeftTop position of the text.
    int inputPos=marginTop+skin.skinInputBar.iInputPos-fontHeight;
    int outputPos=marginTop+skin.skinInputBar.iOutputPos-fontHeight;

    QPixmap inputBarPixmap( QString(skinPath + '/' + skin.skinInputBar.backImg) );
    int resizeWidth=0;
    int resizeHeight=skin.skinInputBar.iOutputPos;
    for (int i=0; i<3; i++) {
        resizeWidth+=metrics.width(numberStr[i]);
        resizeWidth+=metrics.width(candStr[i]);
    };
    int totalWidth=marginLeft + marginRight + resizeWidth;
    int totalHeight=marginTop + marginBottom + resizeHeight;

    destPixmap=QPixmap(totalWidth, totalHeight);
    destPixmap.fill(Qt::transparent);
    DrawResizableBackground(destPixmap, inputBarPixmap,
                            marginLeft, marginRight, marginTop, marginBottom,
                            resizeWidth, resizeHeight
    );

    QPixmap backArrowPixmap( QString(skinPath + '/' + skin.skinInputBar.backArrow) );
    QPixmap forwardArrowPixmap( QString(skinPath + '/' + skin.skinInputBar.forwardArrow) );
    DrawWidget(destPixmap, backArrowPixmap,
               totalWidth - skin.skinInputBar.iBackArrowX, skin.skinInputBar.iBackArrowY
    );
    DrawWidget(destPixmap, forwardArrowPixmap,
               totalWidth - skin.skinInputBar.iForwardArrowX, skin.skinInputBar.iForwardArrowY
    );

    QPainter textPainter(&destPixmap);
    textPainter.setFont(inputFont);

    QColor tipColor=GetIntColor(skin.skinFont.fontColor[0]);
    QColor inputColor=GetIntColor(skin.skinFont.fontColor[1]);
    QColor indexColor=GetIntColor(skin.skinFont.fontColor[2]);
    QColor firstCandColor=GetIntColor(skin.skinFont.fontColor[3]);
    QColor userPhraseColor=GetIntColor(skin.skinFont.fontColor[5]);
    QColor codeColor=GetIntColor(skin.skinFont.fontColor[5]);
    QColor otherColor=GetIntColor(skin.skinFont.fontColor[6]);


    textPainter.setPen(inputColor);
    textPainter.drawText(marginLeft, inputPos, resizeWidth, fontHeight, Qt::AlignVCenter, inputExample);

    // Draw candidate number:
    textPainter.setPen(indexColor);
    for (int i=0; i<3; i++) {
        textPainter.drawText(offset, outputPos, resizeWidth, fontHeight, Qt::AlignVCenter, numberStr[i]);
        offset=offset + metrics.width(numberStr[i]) + metrics.width(candStr[i]);
    }

    offset=marginLeft+metrics.width(numberStr[0]);

    textPainter.setPen(firstCandColor);
    textPainter.drawText(offset, outputPos, resizeWidth, fontHeight, Qt::AlignVCenter, candStr[0]);
    offset=offset+metrics.width(candStr[0])+metrics.width(numberStr[1]);

    textPainter.setPen(userPhraseColor);
    textPainter.drawText(offset, outputPos, resizeWidth, fontHeight, Qt::AlignVCenter, candStr[1]);
    offset=offset+metrics.width(candStr[1])+metrics.width(numberStr[2]);

    textPainter.setPen(otherColor);
    textPainter.drawText(offset, outputPos, resizeWidth, fontHeight, Qt::AlignVCenter, candStr[2]);

    textPainter.end();
}

void MainWindow::DrawMenu(QPixmap &destPixmap, FcitxSkin &skin, QString skinPath)
{
    /**
     * 绘制菜单
     */
#define SET_OFFSET offset+=(fontSize+3);
    int marginLeft=skin.skinMenu.marginLeft;
    int marginRight=skin.skinMenu.marginRight;
    int marginTop=skin.skinMenu.marginTop;
    int marginBottom=skin.skinMenu.marginBottom;
    int offset=marginTop;
    QPixmap backgroundPixmap( QString(skinPath + '/' + skin.skinMenu.backImg) );
    QColor menuFontColor0( GetIntColor(skin.skinFont.menuFontColor[0]) );
    QColor menuFontColor1( GetIntColor(skin.skinFont.menuFontColor[1]) );
    QColor activeColor( GetIntColor(skin.skinMenu.activeColor) );
    QColor lineColor( GetIntColor(skin.skinMenu.lineColor) );

    QString text1(tr("Selected line"));
    QString text2(tr("Unselected line"));
    QString text3(tr("Fcitx Skin Viewer"));

    int fontSize=skin.skinFont.menuFontSize;
    QFont menuFont("");
    menuFont.setPixelSize(fontSize);
    QFontMetrics metrics(menuFont);

    int resizeWidth=metrics.width(text3)+6;
    int resizeHeight=100;

    DrawResizableBackground(destPixmap, backgroundPixmap,
                            marginLeft, marginRight, marginTop, marginBottom,
                            resizeWidth, resizeHeight
    );

    QPainter textPainter(&destPixmap);
    textPainter.setPen(activeColor);
    textPainter.fillRect(marginLeft, marginTop, resizeWidth, fontSize, activeColor );

    //Draw text.
    textPainter.setFont( menuFont );
    textPainter.setPen(menuFontColor0);
    textPainter.drawText(marginLeft, offset, resizeWidth, fontSize, Qt::AlignCenter, text1);
    SET_OFFSET

    textPainter.setPen(menuFontColor1);
    textPainter.drawText(marginLeft, offset, resizeWidth, fontSize, Qt::AlignCenter, text2);
    SET_OFFSET

    //Draw a line.
    textPainter.setPen(lineColor);
    textPainter.fillRect(marginLeft+3, offset, resizeWidth-6, 2, lineColor);
    offset+=5;

    textPainter.setPen(menuFontColor1);
    textPainter.drawText(marginLeft, offset, resizeWidth, fontSize, Qt::AlignCenter, text3);
    SET_OFFSET

    textPainter.end();
}

void MainWindow::GenList()
{
    skinRootDir.setPath(QString(QDir::homePath()+"/.config/fcitx/skin/"));
    skinDirsList=skinRootDir.entryList(QDir::AllDirs | QDir::NoDotAndDotDot);
    qDebug() << skinDirsList.size();

    /*
    QStringList tableStringList;
    tableStringList << tr("Skin Name") << tr("Author") << tr("Description");
    skinChooseTable->setHorizontalHeaderLabels(tableStringList);
    skinChooseTable->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    */

    for (int i=0; i<skinDirsList.size(); i++) {
        QDir skinDir(skinRootDir.absoluteFilePath(skinDirsList[i]));
        QFile skinConfigFile(skinDir.absolutePath()+'/'+"fcitx_skin.conf");
        // TODO:
        if (skinConfigFile.exists()) {
            MyLoadConfig *skinClass=new MyLoadConfig(skinDir.dirName());

            QString skinName=skinClass->skin.skinInfo.skinName;
            QString skinAuthor=skinClass->skin.skinInfo.skinAuthor;
            QString skinDesc=skinClass->skin.skinInfo.skinDesc;

            QTableWidgetItem *skinNameItem=new QTableWidgetItem(skinName);
            QTableWidgetItem *skinAuthorItem=new QTableWidgetItem(skinAuthor);
            QTableWidgetItem *skinDescItem=new QTableWidgetItem(skinDesc);

            int rowCount=skinChooseTable->rowCount();
            int colCount=skinChooseTable->columnCount();

            skinChooseTable->insertRow(rowCount);
            skinChooseTable->setItem(rowCount, 0, skinNameItem);
            skinChooseTable->setItem(rowCount, 1, skinAuthorItem);
            skinChooseTable->setItem(rowCount, 2, skinDescItem);

        }
    }
    connect( skinChooseTable, SIGNAL(cellActivated(int, int)),
                this, SLOT(openFromTable(int, int)) );
    connect( skinChooseTable, SIGNAL(cellEntered(int, int)),
                this, SLOT(openFromTable(int, int)) );
}

void MainWindow::openFromTable(int tableRow, int tableCol)
{
    QDir skinDir( skinRootDir.absolutePath() + '/' + skinDirsList[tableRow] );
    skinPath=skinDir.absolutePath();
    redrawButtonPushed();
}


#include "MainWindow.moc"
