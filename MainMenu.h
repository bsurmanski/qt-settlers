/* 
 * File:   MainMenu.h
 * Author: brandon
 *
 * Created on June 29, 2011, 10:39 AM
 */

#ifndef MAINMENU_H
#define	MAINMENU_H

#include <QtOpenGL/QtOpenGL>
#include <QtCore/QTimer>

class MainMenu : public QGLWidget {
    Q_OBJECT
public:
    MainMenu(QWidget* parent = 0);
    virtual ~MainMenu();

protected slots:
    void update();
    
private:
    void initializeGL();
    void paintGL();
    
    QTimer* timer;
};

#endif	/* MAINMENU_H */

