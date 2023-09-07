#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h" 

#include <vector>
#include <random>
#include <set>
#include <memory>
#include <cmath>

class SierpinskiHexagon : public olc::PixelGameEngine
{
public:
  SierpinskiHexagon()
	{
		sAppName = "Sierpinski Hexagon";
	}

private:
  std::array<olc::vi2d, 6> points;
  std::vector<olc::vi2d> dots;
  const std::size_t MAX_DOTS = 20'000; // the more dots we draw, the clearer the fractal, 20k seems reasonable.

public:
	bool OnUserCreate() override
	{
    // Define the six dots (vertices) of the hexagon aligned to the center
    float radius = 450.0f;
    for (int i = 0; i < 6; ++i)
    {
      float angle = static_cast<float>(i) * 2.0f * M_PI / points.size();
      float x = radius * std::cos(angle) + ScreenWidth() / 2.0f;
      float y = radius * std::sin(angle) + ScreenHeight() / 2.0f;
      points[i] = {int(x), int(y)};
    }
    return true;
	}

	bool OnUserUpdate(float delta_time) override {
    if(dots.size() >= MAX_DOTS) return true;

    // 1. Roll a die to select one of the points
    auto die = random<std::size_t>(1, 6);
    const olc::vi2d& point = points[die - 1];
    const olc::vi2d& lastDot = dots.empty() ? points[0] : dots.back();
    if(lastDot == point) return true;

    // 2. Move two thirds (2/3) the distance towards the random point
    olc::vi2d newDot = calculateTwoThirdsPoint(lastDot, point);
    dots.push_back(newDot);


    // Draw
    Clear(olc::BLACK);
    for(int i = 0; i < points.size(); ++i){
      Draw(points[i], olc::WHITE);
      DrawString(points[i] - olc::vi2d{5, 5}, std::to_string(i+1));
    }
    for(const olc::vi2d& d : dots){
      Draw(d, olc::WHITE);
    }

    DrawString(5,5, "Num dots: "+std::to_string(dots.size())+"/"+std::to_string(MAX_DOTS));

    // repeat...
    return true;
  }

	// Called at the end of the game
	bool OnUserDestroy() override
	{
		return true;
	}

private: // Utils/Helpers
  // Function to calculate a point two-thirds of the way between two points
  olc::vi2d calculateTwoThirdsPoint(const olc::vi2d& point1, const olc::vi2d& point2)
  {
    return point1 + (2 * (point2 - point1) / 3);
  }

  // Random value generator
	template<typename T> requires std::is_integral_v<T> or std::is_floating_point_v<T>
  T random(T min, T max)
  {
		static std::random_device seed{"hardware"};
		static std::default_random_engine generator(seed());
    if constexpr (std::is_integral<T>::value)
		{
			std::uniform_int_distribution<T> dist(min, max);
			return dist(generator);
		}
		else
		{
			std::uniform_real_distribution<T> dist(min, max);
			return dist(generator);
		}
	}
};

int main()
{
  SierpinskiHexagon demo;
	if (demo.Construct(1280, 960, 1, 1))
		demo.Start();
	return 0;
}
