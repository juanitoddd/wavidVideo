#pragma once
#include <chrono>
#include <thread>
#include "ofMain.h"
#include "ofxMQTT.h"
#include "ofxNetwork.h"
#include "ofxMightyUI.h"
#include "ofxAsync.h"
#include "ui/Layout.h"
#include "ui/TestLayout.h"
#include "scene/Scene.h"

class ofApp : public ofBaseApp{

	public:
		/* ----- GLOBAL STATE ----*/
		int state = 0; // state of the app
		/*
		0 - Debug
		1 - Play
		*/

		/* ----- NETWORK ----*/

		// MQTT Client for easy com with HA, Android, Etc
		ofxMQTT mqttClient;
		void onMqttMessage(ofxMQTTMessage &msg);
		void onMqttOnline();
		void onMqttOffline();
		// UDP Client to get responses from ESPs
		ofxUDPManager udpClient;
		// Vector of UDP Clients for each esp
		std::vector<ofxUDPManager> udpESPs;
		// UDP Message to be send
  		string udpMessage;
		// Function to Ping All ESPs
		void pingESPs();
		void pingESP(int _which);
		void sendToESP(int _which, string _message);

		void onTestESP(const void * sender, ofTouchEventArgs & args);
		void lightsOff(ofTouchEventArgs & args);

		/* --------- SCENE NETWORK UTILS----------*/

		void setLightScene();
		void setFloorScene();

		int randomIndex;
		void ESPsOFF();
		void ESPsON();

		ofxUDPManager udpFloor;
		bool blink = true;

		/* ----- INTERFACE ----*/

		mui::Root * root;
		mui::Container * menu;
		mui::Button * playMovieBtn;
		mui::Button * testLightsBtn;
		mui::Button * testScenesBtn;

		mui::Container * layoutTestLights;
		mui::Container * layoutTestScenes;
		// Pin Lights
		std::vector<mui::Button*> espTestBtn;
		// Load Scences
		std::vector<mui::Button*> scenesBtns;
		std::vector<Scene> scenes;
		// std::vector<mui::Label*> espTestLbl;
		// string scenes[10];
		mui::Button * lightsOffBtn;
		// TestLayout * testLayout ;

		void onPlayMovie(ofTouchEventArgs & args);
		void onTestLights(ofTouchEventArgs & args);
		void onTestScenes(ofTouchEventArgs & args);
		void onPlayScene(const void * sender, ofTouchEventArgs & args);

		void playMovie();
		void testLights();
		void testScenes();
		void initialScreen();

		/* -------------------*/

		/* ----- OF DEFAULTS ----*/

		void setup();
		void update();
		void draw();	
		void keyReleased(int key);
		
		/* ----- VIDEO ----*/
		int scene_frames[12];
		bool scene_timeouts[12];
		ofVideoPlayer 		movie;
};

