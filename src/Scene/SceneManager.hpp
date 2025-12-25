#pragma once 
#include "../MainLibraries/main.hpp"

enum class SceneType {
    MENU,
    GAME
  };

struct sceneStruct {
    SceneType currentScene = SceneType::MENU;

    void switchTo(SceneType sceneNew) {
        currentScene = sceneNew;
    }

};