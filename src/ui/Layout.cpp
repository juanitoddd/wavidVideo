#include "Layout.h" // L for layout ^^


MyForm::MyForm(){
    sizeLabel = new mui::Label("Size:", 0,0,100,30); 
    add(sizeLabel); 
    
    sizeSlider = new mui::Slider(0,0,100,30, 100,200,150 ); // a slider 100x30 large, values 100.200, default 150
    add(sizeSlider); 
    
    hueLabel = new mui::Label("Hue:", 0,0, 100,30); 
    add(hueLabel); 
    
    hueSlider = new mui::Slider(0,0,100,30, 100,200,150 ); // a slider 100x30 large, values 100.200, default 150
    add(hueSlider); 
}

MyForm::~MyForm(){
    delete sizeLabel; 
    delete sizeSlider; 
    delete hueLabel; 
    delete hueSlider; 
}
	
// call on the first frame, and whenever the size of the window changes
void MyForm::layout(){
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
}
