#ifndef ENTITIES_H
#define ENTITIES_H

#include <helper.h>
#include <periph.h>
#include <serialATmega.h>
#include <timerISR.h>
#include <ili9341.h>
#include <data/sprite.h>
#include <bitmaps.h>
#include <stdio.h>
#include <util.h>
#include <data/sprites/goblin.h>
#include <data/sprites/character.h>

LinkedList<Entity*> entities;

void registerEntity(Entity *entity) {
    entities.push(entity);
}

void entitySetup() {
    registerEntity(new Goblin(100, 100));
    registerEntity(new Character(150, 100));
}

#endif