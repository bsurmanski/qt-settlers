/* 
 * File:   PauseMenu.h
 * Author: brandon
 *
 * Created on May 31, 2011, 1:21 PM
 */

#ifndef PAUSEMENU_H
#define	PAUSEMENU_H
#include "Model.h"

class PauseMenu {
public:
    PauseMenu();
    PauseMenu(const PauseMenu& orig);
    virtual ~PauseMenu();

    bool Update();
    void Draw();
private:
    Model* pauseModel;
    Vector3f rotation;

};

#endif	/* PAUSEMENU_H */

