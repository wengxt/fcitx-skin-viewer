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

#include "MainWindow.h"

MainWindow::MainWindow()
{
    this->setupUi ( this );

    // QImage inputImage ( QString::fromUtf8 ( "/home/saber/.config/fcitx/skin/plasma/input.png" ) );
    // QImage mainBarImage (QString::fromUTf8 ( "/home/saber/.config/fcitx/skin/plasma/main.png" ) );
    // qDebug() << inputImage.isNull();

    QImage inputImage ( QString::fromUtf8 ( "/home/ukyoi/.config/fcitx/skin/default/input.png" ) );
    QImage mainBarImage ( QString::fromUtf8 ( "/home/ukyoi/.config/fcitx/skin/default/bar.png" ) );


    int inputWidth = 200;
    int inputHeight = 100;
    int mainBarWidth = 50;
    int mainBarHeight = 20;

    inputWindowLabel->resize( inputWidth, inputHeight );
    QPixmap inputDestPixmap ( inputWidth, inputHeight );
    QPixmap mainBarDestPixmap ( mainBarWidth, mainBarHeight );
    DrawResizableBackground(inputDestPixmap, QPixmap::fromImage(inputImage), inputWidth, inputHeight, 14, 16, 14 ,14);
    DrawResizableBackground(mainBarDestPixmap, QPixmap::fromImage(mainBarImage), mainBarWidth, mainBarHeight, 2, 2, 2, 2);
    QSize demoStringSize(10, 20);
    DrawInputBarDemoString(inputDestPixmap, demoStringSize, 14, 14);
    inputWindowLabel->setPixmap(inputDestPixmap);
    mainBarLabel->setPixmap(mainBarDestPixmap);
}

QSize MainWindow::GetInputBarDemoStringSize()
{
}

void MainWindow::DrawInputBarDemoString(
    QPixmap& destPixmap,
    QSize &demoStringSize,
    int marginLeft,
    int marginTop
)
{
    QString engDemoString("shu ru shi li");
    QString chnDemoString( QString::fromUtf8("1.第一候选 2.用户自造 3.其他") );

    QPainter painter ( &destPixmap );
    painter.drawText(
        marginLeft,
        marginTop,
        engDemoString );
    painter.drawText(
        marginLeft,
        // marginTop,
        marginTop + demoStringSize.height() + demoStringSize.height()/2,
        chnDemoString );

    painter.end();
}

void MainWindow::DrawResizableBackground (
    QPixmap& destPixmap,
    const QPixmap& backgroundPixmap,
    int width,
    int height,
    int marginLeft,
    int marginRight,
    int marginTop,
    int marginBottom
    )
{
    destPixmap.fill ( Qt::transparent );

    QPainter painter ( &destPixmap );

    int resizeHeight = backgroundPixmap.height() - marginTop - marginBottom;
    int resizeWidth = backgroundPixmap.width () - marginLeft - marginRight;

    if ( resizeHeight <= 0 )
        resizeHeight = 1;

    if ( resizeWidth <= 0 )
        resizeWidth = 1;

    /* 九宫格
     * 7 8 9
     * 4 5 6
     * 1 2 3
     */

    /* part 1 */
    painter.drawPixmap(
        QRect(0, height - marginBottom, marginLeft, marginBottom),
        backgroundPixmap,
        QRect(0, marginTop + resizeHeight, marginLeft, marginBottom)
    );

    /* part 3 */
    painter.drawPixmap(
        QRect(width - marginRight, height - marginBottom, marginRight, marginBottom),
        backgroundPixmap,
        QRect(marginLeft + resizeWidth, marginTop + resizeHeight, marginRight, marginBottom)
    );

    /* part 7 */
    painter.drawPixmap(
        QRect(0 , 0, marginLeft, marginTop),
        backgroundPixmap,
        QRect(0, 0, marginLeft, marginTop)
    );

    /* part 9 */
    painter.drawPixmap(
        QRect(width - marginRight, 0, marginRight, marginTop),
        backgroundPixmap,
        QRect(marginLeft + resizeWidth, 0, marginRight, marginTop)
    );

    /* part 2 & 8 */
    painter.drawPixmap(
        QRect(marginLeft, 0, width - marginLeft - marginRight, marginTop),
        backgroundPixmap,
        QRect(marginLeft, 0, resizeWidth, marginTop)
    );
    painter.drawPixmap(
        QRect(marginLeft, height - marginBottom, width - marginLeft - marginRight, marginBottom),
        backgroundPixmap,
        QRect(marginLeft, marginTop + resizeHeight, resizeWidth, marginBottom)
    );

    /* part 4 & 6 */
    painter.drawPixmap(
        QRect(0, marginTop , marginLeft, height - marginTop - marginBottom),
        backgroundPixmap,
        QRect(0, marginTop , marginLeft, resizeHeight)
    );

    painter.drawPixmap(
        QRect(width - marginRight, marginTop , marginRight, height - marginTop - marginBottom),
        backgroundPixmap,
        QRect(marginLeft + resizeWidth, marginTop , marginRight, resizeHeight)
    );

    /* part 5 */
    painter.drawPixmap(
        QRect(marginLeft, marginTop , width - marginLeft - marginRight, height - marginTop - marginBottom),
        backgroundPixmap,
        QRect(marginLeft, marginTop , resizeWidth, resizeHeight)
    );

    painter.end();
}

#include "MainWindow.moc"
