#include "utils.h"
#include "constant.h"
#include <SFML/Graphics/Color.hpp>
#include <random>

std::random_device random_device;
std::mt19937 random_generator(random_device());

namespace Utils {

int random_int(int min, int max)
{
    std::uniform_int_distribution<int> random_number_range(min, max);
    return random_number_range(random_generator);
}

sf::Color random_color()
{
    std::uniform_int_distribution<int> random_number_range(0, Colors.size() - 1);
    return Colors[random_number_range(random_generator)];
}

} // namespace Utils
