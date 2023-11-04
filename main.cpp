#include <SFML/Graphics.hpp>
using namespace sf;

const int mapHeight = 19;
const int mapWidth = 50;
float offsetX;
const int windowW = 800;
const int windowH = 600;
const int rectangleSize = 32;

String tailMap[] =
{
  "B                                                B",
  "B                                                B",
  "B                                                B",
  "B                                                B",
  "B                                                B",
  "B                                                B",
  "B                                                B",
  "B                                                B",
  "B                                                B",
  "B                                           O    B",
  "B                                        BBBB    B",
  "B       O       O                            B   B",
  "B     BBBB                                       B",
  "B                                  O            OB",
  "B            B        B  BB     BBBB            BB",
  "B              BBBBB     BB                    B B",
  "B         BB             BB               BBB    B",
  "B       BB               BB            B         B",
  "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB",
};
class Player {
public:
    float dx, dy; //speed
    bool isOnGroud; 
    Sprite sprite; 
    FloatRect rect; // rect object to store width, height, X and Y axis position of sprite
    

    Player(Texture &image)
    {
        sprite.setTexture(image);
        rect.left = 100;
        //rect.width = 44.0f;
        //rect.height = 68.4f;
        rect.width = 32.0f;
        rect.height = 58.0f;
        rect.top = 500;
        sprite.setTextureRect(IntRect(0,0, 32, 58));
        currentFrame = 0;
        isOnGroud = true;
        dx = dy = 0;
    }

    void update(float &time)
    {
        rect.left += dx * time; // moves player on X axis
        collision(0); // check for collision with in-game objects on X axis
        currentFrame += 0.000015f * time; 
        if (currentFrame > 6) { currentFrame -= 6; }
        
        if (!isOnGroud) { dy += 0.000004f; } // gravity
        rect.top += dy * time; // moves player on Y axis
        isOnGroud = false;
        collision(1); // check for collision with in-game objects on Y axis

        if (dx == 0 && dy == 0) { sprite.setTextureRect(IntRect(0, 204, 44, 68)); }
        if (dx > 0) { sprite.setTextureRect(IntRect((int(currentFrame) * 32), 0, 32, 58)); }
        if (dx < 0) { sprite.setTextureRect(IntRect((int(currentFrame) * 32) + 32, 0, -32, 58)); }

        sprite.setPosition(rect.left - offsetX, rect.top);
        dx = 0;
    }
private:
    float currentFrame; // current frame of sprite when mooving
    void collision(int dir)
    {
        for (int i = rect.top / 32;i < (rect.top + rect.height) / rectangleSize; ++i)
            for (int j = rect.left / 32; j < (rect.left + rect.width) / rectangleSize; ++j)
            {
                if (tailMap[i][j] == 'O') { tailMap[i][j] = ' '; }
                if (tailMap[i][j] == 'B') {
                    if (dx > 0 && dir == 0) { rect.left = j * rectangleSize - rect.width; }
                    if (dx < 0 && dir == 0) { rect.left = j * rectangleSize + rectangleSize; }
                    if (dy > 0 && dir == 1) { rect.top = i * rectangleSize - rect.height; dy = 0; isOnGroud = true; }
                    if (dy < 0 && dir == 1) { rect.top = i * rectangleSize + rectangleSize; }
                }
            }
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(windowW, windowH), "Platformer game");
    Texture texture;
    texture.loadFromFile("sprite.png");

    Player player(texture);
    Clock clock;

    while (window.isOpen())
    {
        float time = clock.getElapsedTime().asMicroseconds();
        clock.restart();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        
        if (Keyboard::isKeyPressed(Keyboard::Right))
            player.dx = 0.0003f;
        if (Keyboard::isKeyPressed(Keyboard::Left))
            player.dx = -0.0003f;
        if (Keyboard::isKeyPressed(Keyboard::Up))
        {
            if (player.isOnGroud)
            {
                player.dy = -0.0012f;
                player.isOnGroud = false;
            }
        }
        
        RectangleShape box;
        box.setSize(Vector2f(rectangleSize, rectangleSize));

        player.update(time);
        if (player.rect.left >= windowW / 2 - rectangleSize && player.rect.left <= (mapWidth * rectangleSize - windowW / 2) - rectangleSize)
        {
            offsetX = player.rect.left - (windowW / 2) + rectangleSize;
        }
        

        window.clear(Color::White);

        for (int i = 0; i < mapHeight; ++i)
            for (int j = 0; j < mapWidth; ++j)
            {
                if (tailMap[i][j] == 'B') { box.setFillColor(Color::Black); }
                if (tailMap[i][j] == 'O') { box.setFillColor(Color::Green); }
                if (tailMap[i][j] == ' ') continue;
                box.setPosition(j* rectangleSize - offsetX, i* rectangleSize);
                window.draw(box);
            }
        window.draw(player.sprite);
        window.display();
    }

    return 0;
}