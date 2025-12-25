#pragma once 
#include "MainLibraries/main.hpp"
#include "GUI.hpp"
#include "Renderer.hpp"
struct keyboardInput {
	bool Collect = false;
    bool removeItem = false;
};
class Collectable_item {
public:
	Collectable_item() {}
	
	keyboardInput input;
	void  catchingEvent(sf::Event* event) {
		if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            // Example: check for a specific key, e.g., Space
            if (keyPressed->scancode == sf::Keyboard::Scan::E) {
				if (!input.Collect) {
					input.Collect = true;
				}
           
            }
            if (keyPressed->scancode == sf::Keyboard::Scan::Q) {
                if (!input.removeItem) {
                    input.removeItem = true;
                }

            }
        }
	}
	slots slot;
    void Collecting(const std::string& nameItem, GUI& gui) {
        if (!input.Collect) return;
		if (slot.slot1) return; 
        for (auto& [key, val] : gui.sprites) {
            if (key == "ui_inventory0") {

                std::string path = "assets/Textures/" + nameItem + ".png";
                auto texture = std::make_shared<sf::Texture>();
                if (!texture->loadFromFile(path)) {
                    std::cout << "Failed to load texture: " << path << "\n";
                    return;
                }

                sf::Sprite sprite(*texture);
                sprite.setScale({ 0.15f, 0.15f });

                // PASANG POSITION DULU sebelum cek collision
                sf::FloatRect slotBounds = val.getGlobalBounds();
                sf::FloatRect spriteBounds = sprite.getGlobalBounds();
                sprite.setPosition({
                    slotBounds.position.x + slotBounds.size.x / 2.f - spriteBounds.size.x / 2.f,
                    slotBounds.position.y + slotBounds.size.y / 2.f - spriteBounds.size.y / 2.f }
                    );

                // sekarang cek slot
                if (!checkingSlotAppearance(val, sprite)) {
                    std::cout << "[Collected] " << nameItem << "\n";

                    slot.slot1 = false;  // atau tetap true
                    input.Collect = false;
                }
                else {
                    gui.sprites.emplace("Collected_" + nameItem, sprite);
                    textures.push_back(texture);
                    std::cout << "Slot 1 is occupied!\n";
                    slot.slot1 = true;  // atau tetap true
                    input.Collect = false;
                }
            }
        }
    }

    void removeItem(GUI& gui,Renderer & renderer){
        if (!input.removeItem) return;

        auto slotIt = gui.sprites.find("ui_inventory0");
        auto itemIt = gui.sprites.find("Collected_keyExample");

        if (slotIt == gui.sprites.end() || itemIt == gui.sprites.end()) {
            std::cout << "Slot or item sprite missing.\n";
            input.removeItem = false;
            return;
        }

        sf::Sprite& slotSprite = slotIt->second;
        sf::Sprite& itemSprite = itemIt->second;

        if (checkingSlotAppearance(slotSprite, itemSprite)) {
            input.removeItem = false;
			slot.slot1 = false;
            gui.sprites.erase(itemIt);
            renderer.removeDrawable("Collected_keyExample");
            std::cout << "Item removed.\n";
        }


        
    }

    bool checkingSlotAppearance(sf::Sprite& slot, sf::Sprite& item2) {
        // collision sekarang valid karena sprite sudah positioned dan scaled
        if (slot.getGlobalBounds().findIntersection(item2.getGlobalBounds())) {
            return true;
        }
        return false;
    }

private:
	std::unordered_map<std::string, sf::Sprite> collectedItems;
	std::vector<std::shared_ptr<sf::Texture>> textures;
};