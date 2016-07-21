#include "GameScene.h"

USING_NS_CC;
#define SCALE_RATIO 32.0
Scene* GameScene::createScene()
{
    auto scene = Scene::create();
    
    auto layer = GameScene::create();
    
    scene->addChild(layer);
    
    return scene;
}

bool GameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    b2Vec2 gravity = b2Vec2(0.0f, -10.0f);
    world = new b2World(gravity);
    debugDraw = new GLESDebugDraw(SCALE_RATIO);
    world->SetDebugDraw(debugDraw);
    uint32  flags  =  0 ;
    flags +=  b2Draw::e_shapeBit ;
    flags += b2Draw::e_jointBit;
    flags += b2Draw::e_centerOfMassBit;
    this->debugDraw->SetFlags (flags);
    
    ballX = 500;
    ballY = 200;
    ball =Sprite::create("player.png");
    ball->setPosition(Point(ballX,ballY));
    this->addChild(ball);
    
    addWall(visibleSize.width ,10,(visibleSize.width / 2) ,0); //BOTTOM
    addWall(visibleSize.width ,10,(visibleSize.width / 2) ,visibleSize.height); //TOP
    addWall(10 ,visibleSize.height ,0,(visibleSize.height / 2) ); //LEFT
    addWall(10 ,visibleSize.height ,visibleSize.width,(visibleSize.height / 2) ); //RIGHT
    
    GameScene::defineBall();
    
    //SET MOUSE LISTENER
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(GameScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(GameScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(GameScene::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    //END MOUSE LISTENER
    
    scheduleUpdate();
    this->schedule(schedule_selector(GameScene::addPipe), 1.0);
    return true;
}

void GameScene::draw(Renderer *renderer, const Mat4 &transform, uint32_t flags)
{
    Layer::draw(renderer, transform, flags);
    Director* director = Director::getInstance();
    GL::enableVertexAttribs(GL::VERTEX_ATTRIB_FLAG_POSITION );
    director->pushMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
    world->DrawDebugData();
    director->popMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_MODELVIEW);
}

void GameScene::addPipe(float dt) {
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    Sprite *topPipe =Sprite::create("pipetop.png");
    Sprite *bottomPipe =Sprite::create("pipe.png");
    
    topPipe->setPosition(Point(visibleSize.width,visibleSize.height));
    topPipe->setAnchorPoint(Vec2(0.5, 1));
    bottomPipe->setPosition(Point(visibleSize.width,0));
    bottomPipe->setAnchorPoint(Vec2(0.5, 0));

    auto random = CCRANDOM_0_1( ) ;
    
    if ( random < 0.35 )
    {
        random = 0.35;
    }
    else if ( random > 0.65 )
    {
        random = 0.65;
    }

    auto topPipePosition = ( random * visibleSize.height/ SCALE_RATIO ) + ( topPipe->getContentSize( ).height/ SCALE_RATIO );
    CCLOG("%f : %f", visibleSize.height/SCALE_RATIO,(topPipe->getContentSize().height/SCALE_RATIO - (topPipePosition - ( topPipe->getContentSize( ).height/ SCALE_RATIO ))));

    Point topPosition = Vec2(topPipe->getPosition().x / SCALE_RATIO, topPipePosition );
    Point bottomPosition = Vec2(bottomPipe->getPosition().x / SCALE_RATIO+50,  0);//NEED BOTTOM LOGIC
 
    
    b2BodyDef topBodyDef;
    topBodyDef.type = b2_kinematicBody;
    topBodyDef.position.Set(topPosition.x, topPosition.y);
    topBodyDef.userData = topPipe;
    b2Body *topBodyHero = world->CreateBody(&topBodyDef);
    b2PolygonShape topRectBox;

    topRectBox.SetAsBox(topPipe->getContentSize().width/SCALE_RATIO / 2, topPipe->getContentSize().height/SCALE_RATIO);
    b2FixtureDef topFix;
    topFix.shape = &topRectBox;
    topFix.density = 0;
    topFix.friction = 0;
    topFix.restitution = 0;
    topBodyHero->CreateFixture(&topFix);
 
    b2BodyDef botBodyDef;
    botBodyDef.type = b2_kinematicBody;
    botBodyDef.position.Set(bottomPosition.x, bottomPosition.y);
    botBodyDef.userData = bottomPipe;
    b2Body *botBodyHero = world->CreateBody(&botBodyDef);
    b2PolygonShape botRectBox;
    botRectBox.SetAsBox(bottomPipe->getContentSize().width/SCALE_RATIO / 2, bottomPipe->getContentSize().height/SCALE_RATIO);
    b2FixtureDef botFix;
    botFix.shape = &botRectBox;
    botFix.density = 0;
    botFix.friction = 0;
    botFix.restitution = 0;
    botBodyHero->CreateFixture(&botFix);

    this->addChild(topPipe);
    this->addChild(bottomPipe);

    b2Vec2 force = b2Vec2(-10,0);
    topBodyHero->SetLinearVelocity(force);
    botBodyHero->SetLinearVelocity(force);

}

void GameScene::addWall(float w,float h,float px,float py) {
    
    b2PolygonShape floorShape;
    
    floorShape.SetAsBox(w/ SCALE_RATIO,h/ SCALE_RATIO);
    b2FixtureDef floorFixture;
    floorFixture.density=0;
    floorFixture.friction=10;
    floorFixture.restitution=0.5;
    floorFixture.shape=&floorShape;
    b2BodyDef floorBodyDef;
    
    floorBodyDef.position.Set(px/ SCALE_RATIO,py/ SCALE_RATIO);
    b2Body *floorBody = world->CreateBody(&floorBodyDef);
    
    floorBody->CreateFixture(&floorFixture);
    
}

void GameScene::defineBall(){
    ballShape.m_radius = 30 / SCALE_RATIO;
    b2FixtureDef ballFixture;
    ballFixture.density=0;
    ballFixture.friction=0;
    ballFixture.restitution=0;
    ballFixture.shape=&ballShape;
    
    ballBodyDef.type= b2_dynamicBody;
    ballBodyDef.userData=ball;
    
    ballBodyDef.position.Set(ball->getPosition().x/SCALE_RATIO,ball->getPosition().y/SCALE_RATIO);
    
    ballBody = world->CreateBody(&ballBodyDef);
    ballBody->CreateFixture(&ballFixture);
    ballBody->SetGravityScale(10);
}

void GameScene::update(float dt){
    int positionIterations = 10;
    int velocityIterations = 10;
    
    deltaTime = dt;
    world->Step(dt, velocityIterations, positionIterations);
    
    for (b2Body *body = world->GetBodyList(); body != NULL; body = body->GetNext())
        if (body->GetUserData())
        {
            Sprite *sprite = (Sprite *) body->GetUserData();
            sprite->setPosition(Point(body->GetPosition().x * SCALE_RATIO,body->GetPosition().y * SCALE_RATIO));
            sprite->setRotation(-1 * CC_RADIANS_TO_DEGREES(body->GetAngle()));
        }
    world->ClearForces();
    world->DrawDebugData();        
}

bool GameScene::onTouchBegan(cocos2d::Touch *touch,
                              cocos2d::Event * event)
{
    Point touchLocation = touch->getLocation();
    
    ballX = touchLocation.x;
    ballY = touchLocation.y;
    CCLOG("%f : %f",ballX,ballY);
    ball->setPosition(Point(ballX ,ballY));

    b2Vec2 vel = ballBody->GetLinearVelocity();
    vel.y = 35;
    ballBody->SetLinearVelocity( vel );

    return true;
}
void GameScene::onTouchMoved(cocos2d::Touch *touch,cocos2d::Event * event)
{
   

}
void GameScene::onTouchEnded(cocos2d::Touch *touch,
                              cocos2d::Event * event)
{
}
void GameScene::onTouchCancelled(cocos2d::Touch *touch,
                                  cocos2d::Event * event)
{
    onTouchEnded(touch, event);
}
