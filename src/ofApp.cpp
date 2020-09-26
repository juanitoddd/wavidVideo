#include "ofApp.h"

#include <typeinfo>

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(0,0,0);
	ofSetVerticalSync(true);
	// Uncomment this to show movies with alpha channels
	// movie.setPixelFormat(OF_PIXELS_RGBA);

    /* ----- SETUP --------*/
    // Initialize String Array 
    // scenes.push_back( Scene("Initial", "initial", 0.0) );

    // scene_frames[0] = 0; //intro
    // scene_frames[1] = 3900; //welcome
    // scene_frames[2] = 11500; //countdown
    // scene_frames[3] = 11800; //takeoff
    // scene_frames[3] = 12700; //takeoff2
    // scene_frames[4] = 13352; //cruising
    // scene_frames[5] = 19600; //off

    scene_frames[0] = 0; //intro
    scene_frames[1] = 3900; //welcome
    scene_frames[2] = 11500; //countdown
    scene_frames[3] = 11780; //takeoff
    scene_frames[4] = 12600; //takeoff 2
    scene_frames[5] = 13200; //cruising
    scene_frames[6] = 19000; //off

    scene_frames[7] = 20046; //failure -> 20046
    scene_frames[8] = 21020; //off2
    scene_frames[9] = 21489; //restart2 ? Landing
    scene_frames[10] = 33480; //End

    for(int st = 0; st < 12; st++) {
        scene_timeouts[st] = true;
    }

    scenes.push_back( Scene("Intro", "intro", scene_frames[0]) );
    scenes.push_back( Scene("Welcome", "welcome", scene_frames[1]) );
    scenes.push_back( Scene("Countdown", "countdown", scene_frames[2]) );
    scenes.push_back( Scene("Takeoff", "takeoff", scene_frames[3]) );
    scenes.push_back( Scene("Cruising", "cruising", scene_frames[5]) );
    scenes.push_back( Scene("Off1", "off1", scene_frames[6])  );
    scenes.push_back( Scene("Failure", "failure", scene_frames[7])  );
    scenes.push_back( Scene("Off2", "off2", scene_frames[8])  );
    scenes.push_back( Scene("Restart2", "restart2", scene_frames[9])  );
    scenes.push_back( Scene("Landing", "landing", scene_frames[10])  );
    // scenes.push_back( Scene("Off", "off", 0) );
    // scenes.push_back( Scene("off 1", "off1", 0.591443)  );
    

    /* ----- INTERFACE ----*/
    mui_init();    
    root = new mui::Root();

    // menu = new mui::Container(0, 0, ofGetWidth(), ofGetHeight());
    menu = new mui::Container(0, 0, ofGetWidth(), 50);
    menu->bg = ofColor(161, 154, 154);
    menu->needsLayout = 1;
    menu->opaque = 1;

    playMovieBtn = new mui::Button( "Play Wavid20", 10, 10, 300, 30 );
    playMovieBtn->bg = ofColor(94, 94, 94);
    ofAddListener( playMovieBtn->onPress, this, &ofApp::onPlayMovie);
    menu->add( playMovieBtn );

    
    testScenesBtn = new mui::Button( "Ping ESPs", 320, 10, 300, 30 );
    testScenesBtn->bg = ofColor(94, 94, 94);
    ofAddListener( testScenesBtn->onPress, this, &ofApp::onTestScenes);
    menu->add( testScenesBtn );
    

    testLightsBtn = new mui::Button( "Test Lights", 630, 10, 300, 30 );
    testLightsBtn->bg = ofColor(94, 94, 94);
    ofAddListener( testLightsBtn->onPress, this, &ofApp::onTestLights);
    menu->add( testLightsBtn );

    root->add( menu );

    layoutTestLights = new mui::Container(0,50,ofGetWidth(), ofGetHeight() - 60);
    // wrapper->needsLayout = 1;
    layoutTestLights->opaque = 1;
    layoutTestLights->bg = ofColor(100,100,100);

    layoutTestScenes = new mui::Container(0,50,160, ofGetHeight() - 60);
    // wrapper->needsLayout = 1;
    layoutTestScenes->opaque = 1;
    layoutTestScenes->bg = ofColor(100,100,100);

    /* --------- ESPs -------------- */

    // ESPs UDP Clients
    int total_eps = 12;

    for(int k = 0; k < total_eps; k++){
        char ip[13];
        sprintf (ip, "192.168.0.%d", (k + 10));
        espTestBtn.push_back( new mui::Button( ip, 10, 10 + (k * 40), 300, 30 ) );
    }
    int j = 0;
    std::vector<mui::Button*>::iterator kt = espTestBtn.begin();
    for (; kt != espTestBtn.end(); ++kt){
        (*kt)->bg = ofColor(161, 154, 154);
        (*kt)->_id = j;
        ofAddListener( (*kt)->onPress, this, &ofApp::onTestESP);
        layoutTestLights->add( *kt );
        j++;
    }

    /* --------- Scenes -------------- */
    
    for(int s = 0; s < scenes.size(); s++){
        scenesBtns.push_back( new mui::Button( scenes.at(s).name, 10, 10 + (s * 60), 140, 50 ) );
    }
    j = 0;
    std::vector<mui::Button*>::iterator st = scenesBtns.begin();
    for (; st != scenesBtns.end(); ++st){
        if(scenes.at(j).slug == "failure") {
            (*st)->bg = ofColor(120, 54, 54);
        } else if(scenes.at(j).slug == "takeoff") {
            (*st)->bg = ofColor(61, 54, 154);
        } else {
            (*st)->bg = ofColor(161, 154, 154);
        }
        (*st)->frame = scenes.at(j).frame;
        (*st)->slug = scenes.at(j).slug;
        ofAddListener( (*st)->onPress, this, &ofApp::onPlayScene);
        layoutTestScenes->add( *st );
        j++;
    }
    
    /*
    lightsOffBtn = new mui::Button( "Lights Off", 320, 10, 300, 30 );
    lightsOffBtn->bg = ofColor(161, 154, 154);
    ofAddListener( lightsOffBtn->onPress, this, &ofApp::lightsOff);
    layoutTestLights->add( lightsOffBtn );
    */
    root->add(layoutTestLights);
    layoutTestLights->visible = false;

    root->add(layoutTestScenes);

    layoutTestScenes->visible = false;
    menu->visible = false;
    
    /* ----- MOVIE ----*/
	movie.load("movies/Final.m4v");
    // movie.load("movies/wavid_final.mp4");
    ofLog() << "Total Movie frames: " << movie.getTotalNumFrames();
    // ofSetFrameRate(30);

    /* ----- NETWORK ----*/

    // OpenFramworks UDP Client
    udpClient.Create();
    udpClient.Bind(5000);
    udpClient.SetNonBlocking(true);
    
    for(int k = 0; k < total_eps - 1; k++){ // 0 to 10
        udpESPs.push_back( ofxUDPManager() );
    }
    int i = 0;
    std::vector<ofxUDPManager>::iterator it = udpESPs.begin();
    for (; it != udpESPs.end(); ++it){
        char ip[13];
        sprintf (ip, "192.168.0.%d", (i + 10));
        // ofLog() << ip;
        (*it).Create();
        (*it).Connect(ip, 4422);
        (*it).SetNonBlocking(true);
        i++;
    }

    udpFloor.Create();
    udpFloor.Connect("192.168.0.99", 4422);
    udpFloor.SetNonBlocking(true);
    
    // MQTT
    mqttClient.begin("192.168.0.111", 1883);
    //mqttClient.connect("openframeworks", "juanddd", "manonegra");

    ESPsOFF();
    ESPsOFF(); // Two Times
    string floor_msg = "on";
    udpFloor.Send(floor_msg.c_str(), floor_msg.length());
    // --- UTILS ----
    randomIndex = 0;

    ofHideCursor();
    // Action
    playMovie();
}

//--------------------------------------------------------------
void ofApp::update(){
    switch(state) {
        case 0: // Test Scenes
            // mqttClient.update()
            break;
        case 1: // Test Lights
            // mqttClient.update();
            break;
        case 2: // Movie Playing 
            movie.update();
            setLightScene();
            setFloorScene();
            break;
    }

    // Listen to Responses from ESPs
    char udpMessage[20];
    udpClient.Receive(udpMessage,20);
    string message = udpMessage;
    if(!message.empty()) {
        ofLog() << "[UDP] Message: ";
        ofLog() << message;
        // ofLog() << "UDP Message: " << message;
        if(message == "play") {
            playMovie();
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    switch(state) {
        case 0: // Test Scenes
            
            break;
        case 1: // Test Lights
            
            break;
        case 2: // Movie Playing 
            movie.draw(0,0);
            break;
    }
}

/* ----- State Functions ------- */

void ofApp::testScenes() {
    menu->visible = true;
    layoutTestScenes->visible = true;
    // mqttClient.connect("openframeworks", "juanddd", "manonegra");
    state = 0;
    // float _pos = movie.getPosition();
    // ofLog() << _pos;
    movie.stop();
    movie.firstFrame();
}

void ofApp::playMovie() {
    menu->visible = false;
    layoutTestScenes->visible = false;
    // mqttClient.disconnect();
    // menu->visible = false;
    // layoutTestLights->visible = false;
    // layoutTestScenes->visible = false;
    state = 2;
    movie.play();
}

//--------------------------------------------------------------
// m: Play Movie
// m: Pause Movie
// q: Main Menu
// s: movie.getPosition()

void ofApp::keyReleased(int key){
    ofLog() << key;
    switch(key){
        case 109: // m
            playMovie();
            movie.setPosition(0);
            break;
        case 111: // o
            ESPsON();
            break;
        case 102: // f
            ESPsOFF();
            break;
        case 112: // p
            movie.setFrame(5700);
            /*
            if(movie.isPlaying()) {
                movie.setPaused(1);
                state = 0;
            } else {
                state = 2;
                movie.play();
            }*/
            break;
        case 113: // q
            
            menu->visible = !menu->visible;
            layoutTestScenes->visible = !layoutTestScenes->visible;
            // testScenes();
            break;
        case 115: // s
            ofLog() << "Current frame: " << movie.getCurrentFrame();
            // ofLog() << movie.getPosition();
            break;
    }
}

void ofApp::onPlayMovie(ofTouchEventArgs & args){
    playMovie();
}

void ofApp::onTestLights(ofTouchEventArgs & args){
	// testLights();
    std::vector<ofxUDPManager>::iterator it = udpESPs.begin();
    for (; it != udpESPs.end(); ++it){
        string hello = "initial";
        (*it).Send(hello.c_str(), hello.length());
    }

}

void ofApp::onTestScenes(ofTouchEventArgs & args){
	// testScenes();
    pingESPs();
}

//----- MQTT Callbacks ------//
void ofApp::onMqttOnline(){
  ofLog() << "[MQTT] Online";
  mqttClient.subscribe("wavid20/of");
  ofLog() << "[MQTT] Subscribed to wavid20/of";
}

void ofApp::onMqttOffline(){
  ofLog() << "[MQTT] Offline";
}

void ofApp::onMqttMessage(ofxMQTTMessage &msg){
  ofLog() << "[MQTT] Message: " << msg.topic << " - " << msg.payload;
  if(msg.payload == "ping") {
    // pingESPs();
  } else if(msg.payload == "start") {

  } else {

  }
}


void ofApp::lightsOff(ofTouchEventArgs & args) {

    int i = 0;
    std::vector<ofxUDPManager>::iterator it = udpESPs.begin();
    for (; it != udpESPs.end(); ++it){
        string hello = "off";
        (*it).Send(hello.c_str(), hello.length());
        i++;
    }
}

void ofApp::sendToESP(int _which, string _message) {
    int t = 0;
    std::vector<ofxUDPManager>::iterator it = udpESPs.begin();
    for (; it != udpESPs.end(); ++it){
        if((int)t == _which) {
            char ip[13];
            sprintf (ip, "192.168.0.%d", (t + 10));
            ofLog() << "[UDP] Sending to " << ip;
            (*it).Send(_message.c_str(), _message.length());
        }
        t++;
    }
}

void ofApp::pingESP(int _which) {
    int t = 0;
    std::vector<ofxUDPManager>::iterator it = udpESPs.begin();
    for (; it != udpESPs.end(); ++it){
        string hello = "who";
        if((int)t == _which) {
            ofLog() << "[UDP] Sending" << t;
            (*it).Send(hello.c_str(), hello.length());
        }
        t++;
    }
}

void ofApp::pingESPs() {

    int i = 0;
    std::vector<ofxUDPManager>::iterator it = udpESPs.begin();
    for (; it != udpESPs.end(); ++it){
        ofLog() << "[UDP] Pinging ESP" << i;
        string hello = "who";
        (*it).Send(hello.c_str(), hello.length());
        i++;
    }
}

void ofApp::onTestESP(const void * sender, ofTouchEventArgs & args){
    mui::Button * button = static_cast<mui::Button*>(const_cast<void*>(sender));  
    // ofLog() << " button->label->text " << button->label->text;
    // ofLog() << " button->id " << button->_id;
    string hello = "who";
    // string hello = "initial";
    udpESPs.at(button->_id).Send(hello.c_str(), hello.length());
}

void ofApp::onPlayScene(const void * sender, ofTouchEventArgs & args){
    mui::Button * button = static_cast<mui::Button*>(const_cast<void*>(sender));  
    button->label->text;
    ofLog() << button->label->text << " Frame: " << button->frame;

    // NOtify ESPs with UDP Message
    std::vector<ofxUDPManager>::iterator it = udpESPs.begin();
    for (; it != udpESPs.end(); ++it){
        (*it).Send(button->slug.c_str(), button->slug.length());
    }

    movie.setFrame(button->frame);
    
    playMovie();
}

    // scene_frames[0] = 0; //intro
    // scene_frames[1] = 3900; //welcome
    // scene_frames[2] = 11500; //countdown
    // scene_frames[3] = 11800; //takeoff
    // scene_frames[3] = 12700; //takeoff2
    // scene_frames[4] = 13352; //cruising
    // scene_frames[5] = 19600; //off

    // scene_frames[6] = 20046; //failure -> 20046
    // scene_frames[7] = 21060; //off2
    // scene_frames[8] = 21489; //restart2
    // scene_frames[9] = 33481; //landing

void ofApp::setLightScene() {
    int _frame = movie.getCurrentFrame();
    if(_frame < scene_frames[1]) { // Intro (Countinuos)
        if(scene_timeouts[0]) {
            scene_timeouts[0] = false;
            ofxAsync::run([this](){

                randomIndex = (int)ofRandom(1, 11);
                sendToESP(randomIndex, "sinelon32_8");
                ofSleepMillis(3500);
                ESPsOFF();
                ofSleepMillis(6500);
                // ofLog() << "Intro. Light after each 5s..." << endl;
                // ofLog() << "Sending Signal to ESP." <<  randomIndex << endl;
                // Send UPD To only one Light
                scene_timeouts[0] = true;
            });
        }

    } else if(_frame > scene_frames[1] && _frame < scene_frames[2]) { // Welcome (Countinuos)

        if(scene_timeouts[1]) {
            scene_timeouts[1] = false;
            ofxAsync::run([&](){

                randomIndex = (int)ofRandom(1, 11);
                sendToESP(randomIndex, "sinelon32_13");
                ofSleepMillis(2000);
                ESPsOFF();
                ofSleepMillis(4000);
                // ofLog() << "Welcome. Light after each 3s..." << endl;
                // Send UPD To only one Light
                scene_timeouts[1] = true;
            });
        }
        
        // string floor_msg = "on";
        // udpFloor.Send(floor_msg.c_str(), floor_msg.length());
        // blink = false;

    } else if(_frame > scene_frames[2] && _frame < scene_frames[3]) { // CountDown (Countinuos)

        std::vector<ofxUDPManager>::iterator it = udpESPs.begin();
        string message = "takeoff_slow";
        for (; it != udpESPs.end(); ++it){
            (*it).Send(message.c_str(), message.length());
        }

    } else if(_frame > scene_frames[3] && _frame < scene_frames[4]) { // TakeOff (Spark)

        std::vector<ofxUDPManager>::iterator it = udpESPs.begin();
        string message = "takeoff_fast_fast";
        for (; it != udpESPs.end(); ++it){
            (*it).Send(message.c_str(), message.length());
        }

    } else if(_frame > scene_frames[4] && _frame < scene_frames[5]) { // TakeOff Slow (Spark)

        std::vector<ofxUDPManager>::iterator it = udpESPs.begin();
        string message = "takeoff_fast";
        for (; it != udpESPs.end(); ++it){
            (*it).Send(message.c_str(), message.length());
        }

    } else if(_frame > scene_frames[5] && _frame < scene_frames[6]) { // Cruising (Spark)  

        if(scene_timeouts[5]) {
            scene_timeouts[5] = false;
            ofxAsync::run([&](){

                randomIndex = (int)ofRandom(1, 11);
                sendToESP(randomIndex, "cruising");
                ofSleepMillis(4000);
                ESPsOFF();
                ofSleepMillis(4000);
                // Send UPD To only one Light
                scene_timeouts[5] = true;
            });
        }

    } else if(_frame > scene_frames[6] && _frame < scene_frames[7]) { // Glitch (Spark)

        ESPsOFF();

    } else if(_frame > scene_frames[7] && _frame < scene_frames[8]) { // Failure (Spark)

        // Notify ESPs with UDP Message
        std::vector<ofxUDPManager>::iterator it = udpESPs.begin();
        string message = "failure2";
        // ofLog() << "Failure ." << endl;
        for (; it != udpESPs.end(); ++it){
            (*it).Send(message.c_str(), message.length());
        }

    } else if(_frame > scene_frames[8] && _frame < scene_frames[9]) { // off2 (Spark)

        ESPsOFF();

    } else if(_frame > scene_frames[9] && _frame < scene_frames[10]) { // Restart 2 / Landing (Continuos)
        
        if(scene_timeouts[9]) {
            scene_timeouts[9] = false;
            ofxAsync::run([&](){
                randomIndex = (int)ofRandom(2, 11);
                // randomIndex = 1;
                string message = "sinelon32_8";
                udpESPs.at(randomIndex).Send(message.c_str(), message.length());
                ofSleepMillis(2000);
                ESPsOFF();
                ofSleepMillis(1000);
                message = "glitchwhite";
                udpESPs.at(randomIndex).Send(message.c_str(), message.length());
                ofSleepMillis(2000);
                ESPsOFF();
                ofLog() << "Restart 2 / Landing. Light after each 5s..." << endl;
                // Send UPD To only one Light
                scene_timeouts[9] = true;
            });
        }
    }
}

void ofApp::setFloorScene() {
    int _frame = movie.getCurrentFrame();
    string floor_msg = "";
    if(_frame > 6910 && _frame < 7700) { 
        if(blink) {
            floor_msg = "blink";
            udpFloor.Send(floor_msg.c_str(), floor_msg.length());
            floor_msg = "blink";
            udpFloor.Send(floor_msg.c_str(), floor_msg.length());
            floor_msg = "blink";
            udpFloor.Send(floor_msg.c_str(), floor_msg.length());
            floor_msg = "blink";
            udpFloor.Send(floor_msg.c_str(), floor_msg.length());
            ofLog() << "Blink" << endl;
            blink = false;
        } 
        
    } else if (_frame > 7700 && _frame < 8800) {

        floor_msg = "on";
        udpFloor.Send(floor_msg.c_str(), floor_msg.length());
    }
}

void ofApp::ESPsOFF() {
    // Notify ESPs with UDP Message
    std::vector<ofxUDPManager>::iterator it = udpESPs.begin();
    string message = "off";
    for (; it != udpESPs.end(); ++it){
        (*it).Send(message.c_str(), message.length());
    }
} 


void ofApp::ESPsON() {
    // Notify ESPs with UDP Message
    std::vector<ofxUDPManager>::iterator it = udpESPs.begin();
    string message = "on";
    for (; it != udpESPs.end(); ++it){
        (*it).Send(message.c_str(), message.length());
    }
} 