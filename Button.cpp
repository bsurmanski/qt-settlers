/* 
 * File:   Button.cpp
 * Author: brandon
 * 
 * Created on May 8, 2011, 9:52 PM
 */

#include "Button.h"

Button::Button() {
    actionPtrVoid = NULL;
    actionPtrInt = NULL;
}

Button::Button(const Button& orig) {
}

Button::~Button() {
}

Vector2f Button::getLocation() {
    return location;
}

Vector2f Button::getBounds() {
    return bounds;
}

bool Button::cursorInButton(int x, int y) {
    return (x > location.x && x < location.x + bounds.x && y > location.y && y < location.y + bounds.y);
}

void Button::registerAction (void (*actionPtr)(void)){
    this->actionPtrVoid = actionPtr;
}
void Button::registerAction (void (*actionPtr)(int)){
    this->actionPtrInt = actionPtr;
}

void Button::activate() {
    actionPtrVoid();
}

void Button::activate(int param){
    actionPtrInt(param);
}

void Button::Draw(){
    stateUp.Draw();
} 
