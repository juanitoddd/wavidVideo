#include "MuiL.h" // L for layout ^^

class TestLayout : public mui::Container{
    public:

        mui::Container * wrapper;

        // Vector of UDP Clients for each esp
		std::vector<mui::Button*> ESPs;

        mui::Button * ESP;

        void onTestESP(ofTouchEventArgs & args);
    
        TestLayout();
        ~TestLayout();
        
        // call on the first frame, and whenever the size of the window changes
        void layout();
};