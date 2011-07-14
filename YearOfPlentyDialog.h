/* 
 * File:   YearOfPlentyDialog.h
 * Author: brandon
 *
 * Created on June 22, 2011, 9:40 PM
 */

#ifndef YEAROFPLENTYDIALOG_H
#define	YEAROFPLENTYDIALOG_H


#include <QtCore/QObject>
#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QGridLayout>
#include <QtGui/QComboBox>
#include <QtGui/QDialogButtonBox>

class YearOfPlentyDialog : public QDialog {
    Q_OBJECT
public:
    YearOfPlentyDialog(QWidget* parent = 0);
    ~YearOfPlentyDialog();

    static QPair<int, int> getResourcePair(QWidget* parent = 0);

protected slots:
    void accept();
    void cancel();

private:
    bool ok;
    QGridLayout* layout;
    QLabel* text;
    QComboBox* first;
    QComboBox* second;
    QDialogButtonBox* buttonBox;
};

#endif	/* YEAROFPLENTYDIALOG_H */

