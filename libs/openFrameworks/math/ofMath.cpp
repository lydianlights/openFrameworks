#include "ofMath.h"
#include "ofNoise.h"
#include "ofPolyline.h"
#include <float.h>

#ifndef TARGET_WIN32
	#include <sys/time.h>
#endif

//--------------------------------------------------
int ofNextPow2(int a){
	// from nehe.gamedev.net lesson 43
	int rval=1;
	while(rval<a) rval<<=1;
	return rval;
}

//--------------------------------------------------
void ofSeedRandom() {

	// good info here:
	// http://stackoverflow.com/questions/322938/recommended-way-to-initialize-srand

	#ifdef TARGET_WIN32
		srand(GetTickCount());
	#elif !defined(TARGET_EMSCRIPTEN)
		// use XOR'd second, microsecond precision AND pid as seed
		struct timeval tv;
		gettimeofday(&tv, 0);
		long int n = (tv.tv_sec ^ tv.tv_usec) ^ getpid();
		srand(n);
	#else
		struct timeval tv;
		gettimeofday(&tv, 0);
		long int n = (tv.tv_sec ^ tv.tv_usec);
		srand(n);
	#endif
}

//--------------------------------------------------
void ofSeedRandom(int val) {
	srand((long) val);
}

//--------------------------------------------------
ofFloat ofRandom(ofFloat max) {
	return (max * rand() / ofFloat(RAND_MAX)) * (1.0f - std::numeric_limits<ofFloat>::epsilon());
}

//--------------------------------------------------
ofFloat ofRandom(ofFloat x, ofFloat y) {
	ofFloat high = std::max(x, y);
	ofFloat low = std::min(x, y);
	return std::max(low, (low + ((high - low) * rand() / ofFloat(RAND_MAX))) * (1.0f - std::numeric_limits<ofFloat>::epsilon()));
}

//--------------------------------------------------
ofFloat ofRandomf() {
	return -1.0f + (2.0f * rand() / ofFloat(RAND_MAX)) * (1.0f - std::numeric_limits<ofFloat>::epsilon());
}

//--------------------------------------------------
ofFloat ofRandomuf() {
	return (rand() / ofFloat(RAND_MAX)) * (1.0f - std::numeric_limits<ofFloat>::epsilon());
}

//---- new to 006
//from the forums http://www.openframeworks.cc/forum/viewtopic.php?t=1413

//--------------------------------------------------
ofFloat ofNormalize(ofFloat value, ofFloat min, ofFloat max){
	return ofClamp( (value - min) / (max - min), 0, 1);
}

//check for division by zero???
//--------------------------------------------------
ofFloat ofMap(ofFloat value, ofFloat inputMin, ofFloat inputMax, ofFloat outputMin, ofFloat outputMax, bool clamp) {

	if (fabs(inputMin - inputMax) < std::numeric_limits<ofFloat>::epsilon()) {
		return outputMin;
	}
	else {
		ofFloat outVal = ((value - inputMin) / (inputMax - inputMin) * (outputMax - outputMin) + outputMin);
	
		if( clamp ){
			if(outputMax < outputMin){
				if( outVal < outputMax )outVal = outputMax;
				else if( outVal > outputMin )outVal = outputMin;
			}else{
				if( outVal > outputMax )outVal = outputMax;
				else if( outVal < outputMin )outVal = outputMin;
			}
		}
		return outVal;
	}

}

//--------------------------------------------------
ofFloat ofDist(ofFloat x1, ofFloat y1, ofFloat x2, ofFloat y2) {
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

//--------------------------------------------------
ofFloat ofDist(ofFloat x1, ofFloat y1, ofFloat z1, ofFloat x2, ofFloat y2, ofFloat z2) {
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
}

//--------------------------------------------------
ofFloat ofDistSquared(ofFloat x1, ofFloat y1, ofFloat x2, ofFloat y2) {
	return ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

//--------------------------------------------------
ofFloat ofDistSquared(ofFloat x1, ofFloat y1, ofFloat z1, ofFloat x2, ofFloat y2, ofFloat z2) {
	return ((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) + (z1 - z2) * (z1 - z2));
}

//--------------------------------------------------
ofFloat ofClamp(ofFloat value, ofFloat min, ofFloat max) {
	return value < min ? min : value > max ? max : value;
}

//--------------------------------------------------
int ofSign(ofFloat n) {
	if( n > 0 ) return 1;
	else if(n < 0) return -1;
	else return 0;
}

//--------------------------------------------------
bool ofInRange(ofFloat t, ofFloat min, ofFloat max) {
	return t>=min && t<=max;
}

//--------------------------------------------------
ofFloat ofRadToDeg(ofFloat radians) {
	return glm::degrees(radians);
}

//--------------------------------------------------
ofFloat ofDegToRad(ofFloat degrees) {
	return glm::radians(degrees);
}

//--------------------------------------------------
ofFloat ofLerp(ofFloat start, ofFloat stop, ofFloat amt) {
	return start + (stop - start) * amt;
}

ofFloat ofWrap(ofFloat value, ofFloat from, ofFloat to) {
	// algorithm from http://stackoverflow.com/a/5852628/599884
	if (from > to) {
		std::swap(from, to);
	}
	ofFloat cycle = to - from;
	if(ofIsFloatEqual<ofFloat>(cycle, 0.0f)){
		return to;
	}
	return value - cycle * floor((value - from) / cycle);
}

//--------------------------------------------------
ofFloat ofWrapRadians(ofFloat angle, ofFloat from, ofFloat to) {
	return ofWrap(angle, from, to);
}

ofFloat ofWrapDegrees(ofFloat angle, ofFloat from, ofFloat to) {
	return ofWrap(angle, from, to);
}

//--------------------------------------------------
ofFloat ofLerpDegrees(ofFloat currentAngle, ofFloat targetAngle, ofFloat pct) {
	return currentAngle + ofAngleDifferenceDegrees(currentAngle, targetAngle) * pct;
}

//--------------------------------------------------
ofFloat ofLerpRadians(ofFloat currentAngle, ofFloat targetAngle, ofFloat pct) {
	return currentAngle + ofAngleDifferenceRadians(currentAngle,targetAngle) * pct;
}

//--------------------------------------------------
float ofNoise(float x){
	return _slang_library_noise1(x)*0.5f + 0.5f;
}

//--------------------------------------------------
float ofNoise(float x, float y){
	return _slang_library_noise2(x,y)*0.5f + 0.5f;
}

//--------------------------------------------------
float ofNoise(const glm::vec2& p){
	return ofNoise( p.x, p.y );
}

//--------------------------------------------------
float ofNoise(float x, float y, float z){
	return _slang_library_noise3(x,y,z)*0.5f + 0.5f;
}

//--------------------------------------------------
float ofNoise(const glm::vec3& p){
	return ofNoise( p.x, p.y, p.z );
}

//--------------------------------------------------
float ofNoise(float x, float y, float z, float w){
	return _slang_library_noise4(x,y,z,w)*0.5f + 0.5f;
}

//--------------------------------------------------
float ofNoise(const glm::vec4& p){
	return ofNoise( p.x, p.y, p.z, p.w );
}

//--------------------------------------------------
float ofSignedNoise(float x){
	return _slang_library_noise1(x);
}

//--------------------------------------------------
float ofSignedNoise(float x, float y){
	return _slang_library_noise2(x,y);
}

//--------------------------------------------------
float ofSignedNoise(const glm::vec2& p){
	return ofSignedNoise( p.x, p.y );
}

//--------------------------------------------------
float ofSignedNoise(float x, float y, float z){
	return _slang_library_noise3(x,y,z);
}

//--------------------------------------------------
float ofSignedNoise(const glm::vec3& p){
	return ofSignedNoise( p.x, p.y, p.z );
}

//--------------------------------------------------
float ofSignedNoise(float x, float y, float z, float w){
	return _slang_library_noise4(x,y,z,w);
}

//--------------------------------------------------
float ofSignedNoise(const glm::vec4& p){
	return ofSignedNoise( p.x, p.y, p.z, p.w );
}

//--------------------------------------------------
ofFloat ofAngleDifferenceDegrees(ofFloat currentAngle, ofFloat targetAngle) {
	return ofWrapDegrees(targetAngle - currentAngle);
}

//--------------------------------------------------
ofFloat ofAngleDifferenceRadians(ofFloat currentAngle, ofFloat targetAngle) {
	return  ofWrapRadians(targetAngle - currentAngle);
}
