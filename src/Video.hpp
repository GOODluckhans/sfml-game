#pragma once 
#include "MainLibraries/main.hpp"
#include "EntityFuncition.hpp"

class Video : public EntityFunction{ 
public:
	Video()  {}
	void updload(sf::RenderWindow& window, bool& videoPlaying, const std::string& file = "", const std::string& nameType = "vid_example") {
		if (file.empty() || file == "") {
			std::cout << "[Video] No file provided — skipping.\n";
			videoPlaying = false;
			return;
		}

		std::string cmd = "ffmpeg.exe -i \"assets\\Video\\" + file +"\" -f rawvideo -pix_fmt rgba -s " + std::to_string(window.getSize().x) + "x" + std::to_string(window.getSize().y) + " -loglevel panic -";



		
		pipe = _popen(cmd.c_str(), "rb");
		if (!pipe) {
			std::cerr << "Failed to open pipe for video playback." << std::endl;
			return;
		}
		bufferIDK.resize(window.getSize().x * window.getSize().y * 4); // width * height * 4 (RGBA)
		texture = sf::Texture(sf::Vector2u{window.getSize().x, window.getSize().y}); // construct blank texture
		sf::Sprite videoSp(texture);

		videoSprite.emplace(nameType, videoSp);

	}
	void runVid(bool& videoEnded, sf::Clock& videoClock,Renderer& renderer,bool& videoPlaying) {
		if (pipe == nullptr) {
			videoEnded = true;
			videoPlaying = false;
			return;
		}


		if (videoClock.getElapsedTime().asSeconds() >= 1.f / 30.f) {
			if (!videoEnded) {
				size_t bytesRead = fread(bufferIDK.data(), 1, bufferIDK.size(), pipe);
				if (bytesRead < bufferIDK.size()) {
					videoEnded = true; // video finished
					videoPlaying = false;
					// Remove all sprites for this video
			
					// Optional: close the FFmpeg pipe if you’re done
					if (pipe) {
						_pclose(pipe);
						pipe = nullptr;
					}
				
						videoSprite.clear();             // remove local reference
						renderer.removeDrawable(names); // remove from Renderer
					
				}
				else {
					texture.update(bufferIDK.data());
				}
			}
			videoClock.restart();
		}

	}

	void update(float& dt, World& world, Audio& audio, Input* input = nullptr, Physics* physics = nullptr, SceneType* type = nullptr) override {
	
	}


	void render(sf::RenderWindow& window, Renderer& renderer, Camera* camera = nullptr) override {
		for (auto& [k, v] : videoSprite) {
			if (k.size() <= 0) continue;

			renderer.drawPlayer( v, k);
		}
	}
	FILE* pipe = nullptr;
	sf::Texture texture;
private:
	 std::vector<uint8_t> bufferIDK; // Ensure bufferIDK is defined and persists

	std::string names = "vid_example";
	std::unordered_map<std::string,sf::Sprite> videoSprite;
};