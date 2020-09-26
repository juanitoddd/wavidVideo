#include "TestLayout.h" // L for layout ^^


TestLayout::TestLayout(){
    wrapper = new mui::Container(0,50,ofGetWidth(), ofGetHeight() - 60);
    // wrapper->needsLayout = 1;
    wrapper->opaque = 1;
    wrapper->bg = ofColor(100,100,100);    

    int total_eps = 10;
    for(int k = 0; k <= total_eps; k++){
        char ip[13];
        sprintf (ip, "192.168.0.%d", (k + 10));
        ESPs.push_back( new mui::Button( ip, 10, 10 + (k * 40), 300, 30 ) );
    }
    int i = 0;
    std::vector<mui::Button*>::iterator it = ESPs.begin();
    for (; it != ESPs.end(); ++it){
        (*it)->bg = ofColor(161, 154, 154);
        // ofAddListener( (*it)->onPress, this, &TestLayout::onTestESP);
        wrapper->add( *it, i );
        i++;
    }

    ESP = new mui::Button( "TEST", 320, 10 , 300, 30 );
    ESP->bg = ofColor(161, 154, 154);
    ESP->toFront();
    // ofAddListener( ESP->onPress, this, &TestLayout::onTestESP);
    wrapper->add( ESP );
    add(wrapper);
}

TestLayout::~TestLayout(){
    delete wrapper;
}

void TestLayout::onTestESP(ofTouchEventArgs & args){
    ofLog() << " event at ";
}
	
// call on the first frame, and whenever the size of the window changes
void TestLayout::layout(){
    /*
    // move the size label 10 px from top left
    mui::L(sizeLabel).posTL(10,10); 
    
    // place the slider next to the label (5px space), and use all available width (minus ten pixels)
    mui::L(sizeSlider).rightOf(sizeLabel, 5); 
    
    // place the hue label below the size label, leaving a 5 px space. 
    mui::L(hueLabel).below(sizeLabel,5); 
    
    // and the hue slider next to the hue label. 
    mui::L(hueSlider).rightOf(hueLabel, 5); 
    
    // in this case, we want our own size 
    ofRectangle size = getChildBounds(); 
    width = size.width; 
    height = size.height; 
    */
}
