#pragma once 
#include "MainLibraries/main.hpp"
#include "Bullet.hpp"
#include "Tiled.hpp"
class SAT {
public:
    SAT() {}
    template<typename T>
    std::array<sf::Vector2f, 4> getOBBPoints(const T& shape) {
        std::array<sf::Vector2f, 4> pts;
        if constexpr (requires { shape.getPoint(0); }) {
            // For convex shapes (e.g., sf::RectangleShape, sf::ConvexShape)
            for (int i = 0; i < 4; i++)
                pts[i] = shape.getTransform().transformPoint(shape.getPoint(i));
        }
        else if constexpr (std::is_same_v<T, sf::Sprite>) {
            // For sf::Sprite, calculate corners from its local bounds
            sf::FloatRect bounds = shape.getLocalBounds();
            pts[0] = shape.getTransform().transformPoint({ bounds.position.x, bounds.position.y });
            pts[1] = shape.getTransform().transformPoint({ bounds.position.x + bounds.size.x, bounds.position.y });
            pts[2] = shape.getTransform().transformPoint({ bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y });
            pts[3] = shape.getTransform().transformPoint({ bounds.position.x, bounds.position.y + bounds.size.y });
        }
        else if constexpr (std::is_same_v<T, TiledStruct>) {
            // For sf::Sprite, calculate corners from its local bounds
            sf::FloatRect bounds = shape.rect;
            pts[0] = { bounds.position.x, bounds.position.y };
            pts[1] = { bounds.position.x + bounds.size.x, bounds.position.y };
            pts[2] = { bounds.position.x + bounds.size.x, bounds.position.y + bounds.size.y };
            pts[3] = { bounds.position.x, bounds.position.y + bounds.size.y };
        }
        return pts;
    }
    
     float dot(const sf::Vector2f& a, const sf::Vector2f& b) {
        return a.x * b.x + a.y * b.y;
    }

     void project(const std::array<sf::Vector2f, 4>& pts,
        const sf::Vector2f& axis,
        float& min, float& max)
    {
        min = max = dot(pts[0], axis);
        for (int i = 1; i < 4; i++) {
            float p = dot(pts[i], axis);
            if (p < min) min = p;
            if (p > max) max = p;
        }
    }

     bool overlaps(float minA, float maxA, float minB, float maxB) {
         return !(maxA < minB || maxB < minA);
     }

     template<typename T2,typename T>
     bool SAT_OBB(const T& A, const T2& B) {
     
         auto a = getOBBPoints(A);
         auto b = getOBBPoints(B);

         // SAT axes (normals of edges)
         sf::Vector2f axes[4] = {
             a[1] - a[0],
             a[3] - a[0],
             b[1] - b[0],
             b[3] - b[0]
         };

         // Normalize
         for (auto& axis : axes) {
             float len = std::sqrt(axis.x * axis.x + axis.y * axis.y);

             if (len > 0.00001f) {  // prevents divide-by-zero
                 axis.x /= len;
                 axis.y /= len;
             }
         }


         for (auto& axis : axes) {
             float minA, maxA, minB, maxB;
             project(a, axis, minA, maxA);
             project(b, axis, minB, maxB);

             if (!overlaps(minA, maxA, minB, maxB))
                 return false;   // separation axis → no collision
         }

         return true; // no axis separated → collision
     }

  
};