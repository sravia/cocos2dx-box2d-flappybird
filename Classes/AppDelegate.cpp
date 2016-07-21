#include "AppDelegate.h"
#include "GameScene.h"
        
USING_NS_CC;

AppDelegate::AppDelegate() {
    
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("Physics world sample");
        director->setOpenGLView(glview);
    }
    
    director->setDisplayStats(true);
    director->setAnimationInterval(1.0 / 60);
    
    auto fileUtils = FileUtils::getInstance( );
    auto screenSize = glview->getFrameSize( );
    std::vector<std::string> resDirOrders;
    
    fileUtils->setSearchPaths(resDirOrders);
    
    auto scene = GameScene::createScene();
    
    director->runWithScene(scene);
    
    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
}
