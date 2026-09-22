#include "../Headers/IRender.h"

class MapRenderer : public IRender {
public:
    MapRenderer(uint16_t *map, Vector2 worldSize, Vector2 screenSize, Vector2 &player)
    : map(map), worldSize(worldSize), screenSize(screenSize), player(player){
    }

    void Render(VirtualScreen &vs) override {
        for (int y = 0; y < screenSize.y; y++) {
            for (int x = 0; x < screenSize.x; x++) {
                Vector2 pos = player + Vector2(x, y) - screenSize / 2;

                if (pos.x < 0 || pos.x >= worldSize.x) continue;
                if (pos.y < 0 || pos.y >= worldSize.y) continue;

                uint16_t tileIndex = map[pos.y * worldSize.x + pos.x];

                vs.DrawTileOnGrid(pos, tileIndex);
            }
        }
    }

private:
    uint16_t *map;
    Vector2 worldSize;
    Vector2 screenSize;
    Vector2 &player;
};