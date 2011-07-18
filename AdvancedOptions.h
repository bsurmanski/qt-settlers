/* 
 * File:   AdvancedOptions.h
 * Author: brandon
 *
 * Created on July 17, 2011, 6:30 PM
 */

#ifndef ADVANCEDOPTIONS_H
#define	ADVANCEDOPTIONS_H
#include <QtGui/QDialog>
#include <QtGui/QSpinBox>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QCheckBox>
#include <QtGui/QLabel>
#include <QtGui/QFormLayout>

class AdvancedOptions : public QDialog {
public:
    AdvancedOptions();
    virtual ~AdvancedOptions();
    void apply();
private:
    QFormLayout* layout;
    QLabel* radiusLabel;
    QSpinBox* boardRadius;
    QLabel* separationLabel;
    QDoubleSpinBox* tileSeparation;
    QLabel* borderLabel;
    QCheckBox* waterBorder;
    QLabel* islandLabel;
    QCheckBox* islands;
    QLabel* bridgesLabel;
    QCheckBox* bridges;
    QLabel* boatsLabel;
    QCheckBox* boats;
    QLabel* startingResourcesLabel;
    QCheckBox* startingResources;
    QPushButton* closeButton;
};

#endif	/* ADVANCEDOPTIONS_H */

