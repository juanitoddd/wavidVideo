#include "MuiL.h" // L for layout ^^

class MyForm : public mui::Container{
    public:

        mui::Label * sizeLabel; 
        mui::Slider * sizeSlider; 
        
        mui::Label * hueLabel; 
        mui::Slider * hueSlider; 
        
        MyForm();
        ~MyForm();
        
        // call on the first frame, and whenever the size of the window changes
        void layout();
};