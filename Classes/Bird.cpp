//
//  Bird.cpp
//  FlappyBird
//
//  Created by sfbest on 2017/2/9.
//
//  Bird

#include "Bird.hpp"

#define kBirdFlyTag 10
#define kBirdShakeTag 11
#define kBirdRotatoTag 12

Bird* Bird::createBird(BirdColor ExcludeColor)
{
    auto bird = new Bird();

    auto randColor = BirdColor(BirdColor(arc4random_uniform(3)));
    
    while (randColor == ExcludeColor) {
        randColor = BirdColor(BirdColor(arc4random_uniform(3)));
    }
    
    char name[30];
    sprintf(name, "fly_%d_0.png", randColor);
    
    if (bird && bird->initWithSpriteFrameName(name)) {
        bird->color = randColor;
        bird->addComponent(PhysicsBody::createBox(bird->getContentSize()));
        bird->getPhysicsBody()->setDynamic(true);
        bird->getPhysicsBody()->setEnabled(false);
        bird->getPhysicsBody()->setRotationEnable(false);
        bird->getPhysicsBody()->setVelocityLimit(500);
        bird->getPhysicsBody()->addMoment(-1);
        bird->autorelease();
    } else {
        delete bird;
        bird = nullptr;
    }
    
    return bird;
}

void Bird::startFlyAnimation()
{
    Vector<SpriteFrame *>frames;
    for (int i = 0; i < 3; i++) {
        char name[30];
        sprintf(name, "fly_%d_%d.png", color, i);
        auto sf = SpriteFrameCache::getInstance()->getSpriteFrameByName(name);
        frames.pushBack(sf);
    }
    auto animation = Animation::createWithSpriteFrames(frames);
    animation->setDelayPerUnit(0.1);
    animation->setRestoreOriginalFrame(true);
    auto anim = Animate::create(animation);

    auto fly = RepeatForever::create(anim);
    fly->setTag(kBirdFlyTag);
    
    runAction(fly);
}

void Bird::stopFlyAnimation()
{
    stopActionByTag(kBirdFlyTag);
}

void Bird::startShakeAnimation()
{
    if (isShakeing) return;

    auto durtion = 0.7;
    auto moveUp = MoveTo::create(durtion, Vec2(getPosition().x, getPosition().y + 20));
    auto moveDown = MoveTo::create(durtion, Vec2(getPosition().x, getPosition().y - 20));
    RepeatForever *anim = RepeatForever::create(Sequence::create(moveUp, moveDown, NULL));
    anim->setTag(kBirdShakeTag);
    runAction(anim);
    
    isShakeing = true;
}

void Bird::stopShakeAnimation()
{
    if (!isShakeing) return;
        
    stopActionByTag(kBirdShakeTag);
    
    isShakeing = false;
    
    getPhysicsBody()->setEnabled(true);
}

void Bird::click()
{
    stopShakeAnimation();

    getPhysicsBody()->setVelocity(Vec2(0, 300));
    
    // click bird rotate action
    stopActionByTag(kBirdRotatoTag);
    auto rotaUp = RotateTo::create(0.1, -40);
    auto rotaDown = RotateTo::create(3, 90);
    auto seqe = Sequence::create(rotaUp, rotaDown, NULL);
    seqe->setTag(kBirdRotatoTag);
    this->runAction(seqe);
}

