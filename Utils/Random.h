// Copyright (c) Lion Kortlepel 2020
// This software is free software and licensed under GPL-3.0.
// You should have received a copy of the GNU General Public License along
// with this program. If not, see <https://www.gnu.org/licenses/>.

#ifndef RANDOM_H
#define RANDOM_H

#include <random>

/// Random number and chance generator helpers.
class Random
{
private:
    static std::mt19937 s_generator;

public:
    /// Gets the mersenne-twister generator itself.
    static std::mt19937& generator() {
        return s_generator;
    }

    /// Random element from the container.
    template<class ContainerT>
    static auto& random_from(ContainerT& cont) {
        std::uniform_int_distribution<> dist(0, cont.size() - 1);
        return cont.at(dist(s_generator));
    }

    /// Random integer between min and max.
    template<class IntT>
    static auto random(IntT min, IntT max) {
        std::uniform_int_distribution<IntT> dist(min, max);
        return dist(s_generator);
    }

    /// `true` with a chance of `chance_percent`%.
    static bool chance(int chance_percent) {
        return random<int>(0, 100) <= chance_percent;
    }

    /// Random float between min and max.
    template<typename FloatT>
    static FloatT random_real(FloatT min = 0.0, FloatT max = 1.0) {
        std::uniform_real_distribution<FloatT> dist(min, max);
        return FloatT(dist(s_generator));
    }
};

/// Internally used singleton generator instance.
inline std::mt19937 Random::s_generator { static_cast<unsigned int>(std::time(nullptr)) };

#endif // RANDOM_H
