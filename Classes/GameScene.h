#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include <Box2D/Box2D.h>
#include "GLES-Render.h"

USING_NS_CC;

class GameScene : public cocos2d::Layer, public b2ContactListener
{
public:
    static Scene* createScene();
    virtual bool init();
    
    CREATE_FUNC(GameScene);
    
private:
    GLESDebugDraw *debugDraw;
    b2World *world;
    float deltaTime;
    Sprite *ball;
    float ballX;
    float ballY;
    b2Body *ballBody;
    b2CircleShape ballShape;
    b2BodyDef ballBodyDef;
    void defineBall();
    void addPipe(float dt);
    void update(float dt);
    void addWall(float w,float h,float px,float py);
    bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event * event);
    void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event * event);
    void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event * event);
    void onTouchCancelled(cocos2d::Touch *touch, cocos2d::Event * event);
    void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags);
};

#endif
