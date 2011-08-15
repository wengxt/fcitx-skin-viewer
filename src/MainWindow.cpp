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

#include <fcitx/ui.h>
#include <fcitx/fcitx.h>
#include <fcitx-config/fcitx-config.h>

#include "MainWindow.h"
#include <qvarlengtharray.h>

MainWindow::MainWindow()
{
    this->setupUi ( this );

    // QImage inputImage ( QString::fromUtf8 ( "/home/saber/.config/fcitx/skin/plasma/input.png" ) );
    // QImage mainBarImage (QString::fromUTf8 ( "/home/saber/.config/fcitx/skin/plasma/main.png" ) );
    // qDebug() << inputImage.isNull();

    // 下面是测试用的边框、字体颜色
    // QImage inputImage ( QString::fromUtf8 ( "/home/ukyoi/.config/fcitx/skin/dark/input.png" ) );
    // QImage mainBarImage ( QString::fromUtf8 ( "/home/ukyoi/.config/fcitx/skin/dark/bar.png" ) );
    skinPath="/usr/share/fcitx/skin/dark";
    MyLoadConfig skinClass(skinPath);
    
    QPixmap inputDestPixmap (0, 0);  // The size of this map should be modified by DrawResizableBackground func.
    DrawInputBar(inputDestPixmap, skinClass.skin, skinPath);

#if 0
    // inputWindowLabel->resize( inputWidth, inputHeight );
    // QPixmap mainBarDestPixmap (0, 0); // The size of this map should be modified by DrawMainBar.
    DrawResizableBackground(skinClass.skin, inputDestPixmap, skinPath);
    inputWindowLabel->setPixmap(inputDestPixmap);
    // mainBarLabel->setPixmap(mainBarDestPixmap);
#endif

}

QSize MainWindow::GetInputBarDemoStringSize()
{
}

//TODO:
/*
void MainWindow::DrawInputBarDemoString(
    FcitxSkin skin,
    QPixmap &destPixmap
)
{
    QString engDemoString("shu ru shi li");
    QString chnDemoString( QString::fromUtf8("1.第一候选 2.用户自造 3.其他") );

    QPainter painter ( &destPixmap );
    painter.setPen( QColor(engColor.r, engColor.g, engColor.b) );
    painter.drawText(
        marginLeft,
        marginTop,
        engDemoString );
    painter.setPen( QColor(chnColor.r, chnColor.g, chnColor.b) );
    painter.drawText(
        marginLeft,
        // marginTop,
        marginTop + demoStringSize.height() + demoStringSize.height()/2,
        chnDemoString );

    painter.end();
}
*/

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
     * 把一个指定的九宫格图片以resizeWidth和resizeHeight为中央区大小画进destPixmap
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
    QPixmap &destPixmap,
    QPixmap &widgetPixmap,
    int x,
    int y
)
{
    /**
     * 在指定位置(x,y)按照原大小画出一个小部件，
     */
    
    QPainter painter(&destPixmap);
    painter.drawPixmap( x, y, widgetPixmap );
    painter.end();
}

#if 0
//TODO:
void drawInputBar() {
    QString backgroundPixmapPath=skinPath + '/' + skin.skinInputBar.backImg;
    qDebug() << backgroundPixmapPath;
    QPixmap backgroundPixmap(backgroundPixmapPath);
    
    // int resizeWidth = backgroundPixmap.width () - marginLeft - marginRight;
    // int resizeHeight = backgroundPixmap.height() - marginTop - marginBottom;
    int resizeWidth=120;
    int resizeHeight=40;
}
#endif
void MainWindow::DrawInputBar(QPixmap &destPixmap, FcitxSkin& skin, QString skinPath)
{
    int marginLeft=skin.skinInputBar.marginLeft;
    int marginRight=skin.skinInputBar.marginRight;
    int marginTop=skin.skinInputBar.marginTop;
    int marginBottom=skin.skinInputBar.marginBottom;
    int resizeWidth=180;
    int resizeHeight=40;
    
    QString inputBarPath=skinPath + '/' + skin.skinInputBar.backImg;
    QPixmap inputBarPixmap(inputBarPath);
    int totalWidth=marginLeft + marginRight + resizeWidth;
    int totalHeight=marginTop + marginBottom + resizeHeight;
    destPixmap=QPixmap(totalWidth, totalHeight);
    destPixmap.fill(Qt::transparent);
    QPainter painter(&inputBarPixmap);
    DrawResizableBackground(destPixmap, inputBarPixmap,
                            marginLeft, marginRight, marginTop, marginBottom,
                            resizeWidth, resizeHeight
    );
    
    QString backArrowPath=skinPath + '/' + skin.skinInputBar.backArrow;
    QString forwardArrowPath=skinPath + '/' + skin.skinInputBar.forwardArrow;
    QPixmap backArrowPixmap(backArrowPath);
    QPixmap forwardArrowPixmap(forwardArrowPath);
    DrawWidget(destPixmap, backArrowPixmap,
               totalWidth - skin.skinInputBar.iBackArrowX, skin.skinInputBar.iBackArrowY
    );
    DrawWidget(destPixmap, forwardArrowPixmap,
               totalWidth - skin.skinInputBar.iForwardArrowX, skin.skinInputBar.iForwardArrowY
    );
    qDebug() << backArrowPath;
    mainBarLabel->setPixmap(destPixmap);
}

  

#include "MainWindow.moc"
