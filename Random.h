#ifndef RANDOM_H
#define RANDOM_H

#include <boost/random.hpp>

/// Random number and chance generator helpers.
class Random
{
private:
    static boost::random::mt19937 s_generator;

public:
    /// Gets the mersenne-twister generator itself.
    static boost::random::mt19937& generator() {
        return s_generator;
    }
    
    /// Random element from the container.
    template<class ContainerT>
    static auto& random_from(ContainerT& cont) {
        boost::random::uniform_int_distribution<> dist(0, cont.size() - 1);
        return cont.at(dist(s_generator));
    }
    
    /// Random integer between min and max.
    template<class IntT>
    static auto random(IntT min, IntT max) {
        boost::random::uniform_int_distribution<> dist(min, max);
        return dist(s_generator);
    }
    
    /// `true` with a chance of `chance_percent`%.
    static bool chance(int chance_percent) {
        return random<int>(0, 100) <= chance_percent;
    }
    
    /// Random float between min and max.
    template<typename FloatT>
    static FloatT random_real(FloatT min = 0.0, FloatT max = 1.0) {
        boost::random::uniform_real_distribution<FloatT> dist(min, max);
        return dist(s_generator);
    }
};

/// Internally used singleton generator instance.
inline boost::random::mt19937 Random::s_generator { static_cast<unsigned long>(std::time(nullptr)) };

#endif // RANDOM_H
