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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QFileDialog>

#include "ui_MainWindow.h"
#include "MyLoadConfig.h"

#include <fcitx-config/fcitx-config.h>

class MainWindow: public QWidget, public Ui::MainWindow
{
    Q_OBJECT
public:
    MainWindow();
    QString skinPath;
    
    QColor GetIntColor(ConfigColor floatColor);

private:

    void DrawAllThings(MyLoadConfig skinClass, QString skinPath);
    void DrawResizableBackground (
        QPixmap &destPixmap,
        QPixmap &backgroundPixmap,
        int marginLeft,
        int marginRight,
        int marginTop,
        int marginBottom,
        int resizeWidth,
        int resizeHeight
    );

    QSize GetInputBarDemoStringSize();
    void DrawInputBarDemoString (
        FcitxSkin &skin,
        QPixmap &destPixmap,
        QString skinPath
    );
    void DrawWidget (
        QPixmap &destPixmap, QPixmap &widgetPixmap,
        int x, int y
    );
    
    void DrawInputBar(QPixmap &destPixmap, FcitxSkin &skin, QString skinPath);
    void DrawMainBar(QPixmap &destPixmap, FcitxSkin &skin, QString skinPath);
    void DrawMenu(QPixmap &destPixmap, FcitxSkin &skin, QString skinPath);
    
private slots:
    void openButtonPushed();
    void redrawButtonPushed();
};

#endif