#include <vector>
#include <math.h>

namespace Shapes {
    static const bool x = true;
    static const bool o = false;

    struct Shape {
        Shape(const std::vector<std::vector<bool>> forms, int width)
            : forms{forms},
              size{forms[0].size()},
              width{width}
              {}
        const std::vector<bool>& data() const {
            return forms[current_form];
        }
        void rotate() {
            current_form = (current_form + 1) % 4;
        }
        
        const std::vector<std::vector<bool>> forms;
        const size_t size;
        const int width;
        int current_form = 0;
    };

    const static Shape L({{o, x, o,
                           o, x, o,
                           o, x, x},
                          {o, o, o,
                           x, x, x,
                           x, o, o},
                          {x, x, o,
                           o, x, o,
                           o, x, o},
                          {o, o, x,
                           x, x, x,
                           o, o, o}}, 3);

    const static Shape T({{o, x, o,
                           x, x, x,
                           o, o, o},
                          {o, x, o,
                           o, x, x,
                           o, x, o},
                          {o, o, o,
                           x, x, x,
                           o, x, o},
                          {o, x, o,
                           x, x, o,
                           o, x, o}}, 3);

    const static Shape BackwardsL({{o, x, o,
                                    o, x, o,
                                    x, x, o},
                                   {x, o, o,
                                    x, x, x,
                                    o, o, o},
                                   {o, x, x,
                                    o, x, o,
                                    o, x, o},
                                   {o, o, o,
                                    x, x, x,
                                    o, o, x}}, 3);

    const static Shape Box({{x, x,
                             x, x},
                            {x, x,
                             x, x},
                            {x, x,
                             x, x},
                            {x, x,
                             x, x}}, 2);

    const static Shape I({{o, x, o, o,
                           o, x, o, o,
                           o, x, o, o,
                           o, x, o, o},
                          {o, o, o, o,
                           o, o, o, o,
                           x, x, x, x,
                           o, o, o, o},
                          {o, x, o, o,
                           o, x, o, o,
                           o, x, o, o,
                           o, x, o, o},
                          {o, o, o, o,
                           o, o, o, o,
                           x, x, x, x,
                           o, o, o, o}}, 4);

    const std::vector<Shape> all_shapes = {L, T, BackwardsL, Box, I};
    
 
    Shape random_shape() {
        size_t n_shapes = all_shapes.size();
        return all_shapes[rand() % n_shapes];

    }
}