#ifndef RANDOM_H
#define RANDOM_H

#include <boost/random.hpp>

class Random
{
private:
    static boost::random::mt19937 s_generator;

public:
    static boost::random::mt19937& generator() {
        return s_generator;
    }
    
    template<class ContainerT>
    static auto& random_from(ContainerT& cont) {
        boost::random::uniform_int_distribution<> dist(0, cont.size() - 1);
        return cont.at(dist(s_generator));
    }
    
    template<typename FloatT>
    static FloatT random_real(FloatT min = 0.0, FloatT max = 1.0) {
        boost::random::uniform_real_distribution<FloatT> dist(min, max);
        return dist(s_generator);
    }
};

inline boost::random::mt19937 Random::s_generator { static_cast<unsigned long>(std::time(nullptr)) };

#endif // RANDOM_H
