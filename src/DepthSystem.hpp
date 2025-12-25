#pragma once 
#include "MainLibraries/main.hpp"
#include "Tiled.hpp"
class DepthSystem {
public:
	DepthSystem() {}
	//std::vector<std::string> collideNOCLIP = { "PUT_PLANT_Lavender" };
	std::unordered_map<std::string, bool> doCollideNOCLIPsystem;
	//template<typename T>
	//void collideHorizontalNOCLIP(std::vector<TiledStruct>& colliders, T& hitBoxChar) {
	//	for (auto& c : colliders) {
	//		for (auto& k : collideNOCLIP) {
	//			doCollideNOCLIPsystem[k] = true;
	//			
	//		}
	//	}			
	//}
	//template<typename T>

	//void interactWithDepth(T& shape,DrawType& drawType) {
	//	drawType = DrawType::FIRST;
	//}     // after maghrib i d like to change it to transparency method

	//void leaveWithDepth(DrawType& drawType) {
	//	drawType = DrawType::LAST;

	//}
	

};