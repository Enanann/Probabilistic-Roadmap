#include "common.h"

class TimeData {
public:
    int Nodes{};
    int K{};
    double NG{};
    double KNN{};
    double AS{};
    double PRM{};

    TimeData() : Nodes(), K(), NG(), KNN(), AS(), PRM() {}
};