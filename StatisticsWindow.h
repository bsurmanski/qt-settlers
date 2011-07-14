/* 
 * File:   StatisticsWindow.h
 * Author: brandon
 *
 * Created on June 27, 2011, 10:50 PM
 */

#ifndef STATISTICSWINDOW_H
#define	STATISTICSWINDOW_H
#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QFormLayout>

class StatisticsWindow : public QDialog{
    Q_OBJECT
public:
    StatisticsWindow();
    virtual ~StatisticsWindow();
private:
    QLabel* longestLabel;
    QLineEdit* longestNum;
    
    QLabel* knightsLabel;
    QLineEdit* knightsNum;
    QFormLayout* layout;

};

#endif	/* STATISTICSWINDOW_H */

