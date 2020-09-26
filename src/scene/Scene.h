#pragma once

#include <string>

class Scene {
    public:

        std::string name;
        std::string slug;
        float timestamp;
        int frame;

        Scene();
        Scene(std::string _name, std::string _slug, int _frame);
        ~Scene();
        
        // call on the first frame, and whenever the size of the window changes
        void layout();
};