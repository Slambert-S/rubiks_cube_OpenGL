
#include <iostream>
#include "OIDynamicNullGestureTracker.h"
#include "stdlib.h"
#include "time.h"
//namespace openiss

openiss::OIDynamicNullGestureTracker::OIDynamicNullGestureTracker() : openiss::OINullGestureTracker() {

}

// Will return a completed gesture, either wave or click (NiTE2 Gesture types are very limited)
std::vector<openiss::OIGestureData> openiss::OIDynamicNullGestureTracker::getGestures() {
	this->m_eGestureData.clear();
	srand(time(NULL));
	
	float x_coord, y_coord;
	x_coord = (float)((rand() / 5) % 61);
	y_coord = (float)((rand() / 13) % 61);
	x_coord -= 30.0f;
	y_coord -= 30.0f;
	this->m_eGesture.setGesturePosition(x_coord, y_coord, 10);
	this->m_eGesture.setGestureType(OIGestureType::GESTURE_CLICK);
	this->m_eGesture.setGestureState(OIGestureState::GESTURE_IS_COMPLETE);

	this->m_eGestureData.push_back(this->m_eGesture);
	std::cout << "Random Gesture Click is on screen at (" << m_eGesture.getGesturePosition().x   << "," << m_eGesture.getGesturePosition().y << ")" << std::endl;
	return m_eGestureData;
}

//Will return a vector with a random hand position: 
//-the left hand may be on or off the screen, and may be in one of four cardinal positions
//-the right hand may be in one of 4 cartdinal positions as well
std::vector<openiss::OIHandData> openiss::OIDynamicNullGestureTracker::getHands() {
	m_eHandData.clear();
	srand(time(NULL));
	int track = (rand() % 2);
	if (track == 0) {
		this->m_eHand.setHandID(0);
		this->m_eHand.setHandPosition(20, 10, 10);
		this->m_eHand.setHandState(OIHandState::HAND_IS_LOST);
		std::cout << "Random Hand is off screen" << std::endl; 
	}
	else { //track ==1
		this->m_eHand.setHandID(0);
		float x_coord, y_coord;
		x_coord = (float)((rand() / 7) % 61);
		y_coord = (float)((rand() / 17) % 61);
		x_coord -= 30.0f;
		y_coord -= 30.0f;
		this->m_eHand.setHandPosition(x_coord, y_coord, 10);
		this->m_eHand.setHandState(OIHandState::HAND_IS_TRACKING);
		std::cout << "Random Hand is on screen at ("<<m_eHand.getHandPosition().x   <<","<< m_eHand.getHandPosition().y <<")"<< std::endl;
	}

	this->m_eHandData.push_back(this->m_eHand);


	return m_eHandData;

}