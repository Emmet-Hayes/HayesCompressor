#pragma once
#include "CrestFactor.h"
#include "SmoothingFilter.h"


class LevelDetector
{
public:
    LevelDetector() = default;

    // Prepares LevelDetector with a ProcessSpec-Object containing samplerate, blocksize and number of channels
    void prepare(const double& fs);

    // Sets attack time constant
    void setAttack(const double&);

    // Sets release time constant
    void setRelease(const double&);

    // Sets auto attack to enabled/disabled
    void setAutoAttack(bool isEnabled);

    // Sets auto release to enabled/disabled
    void setAutoRelease(bool isEnabled);

    // Gets current attack time constant
    double getAttack();

    // Gets current release time constant
    double getRelease();

    // Gets calculated attack coefficient
    double getAlphaAttack();

    // gets calculated release coefficient
    double getAlphaRelease();

    // Processes a sample with smooth branched peak detector
    float processPeakBranched(const float&);

    // Processes a sample with smooth decoupled peak detector
    float processPeakDecoupled(const float&);

    // Applies ballistics to given buffer
    void applyBallistics(float*, int);

    // Processes crest factor and sets ballistics accordingly
    void processCrestFactor(const float* src, int numSamples);

private:
    CrestFactor crestFactor;
    SmoothingFilter attackSmoothingFilter;
    SmoothingFilter releaseSmoothingFilter;

    double attackTimeInSeconds{0.01}, alphaAttack{0.0};
    double releaseTimeInSeconds{0.14}, alphaRelease{0.0};
    double state01{0.0}, state02{0.0};
    double sampleRate{0.0};
    bool autoAttack{false};
    bool autoRelease{false};
};
