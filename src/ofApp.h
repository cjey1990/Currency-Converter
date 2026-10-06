#pragma once
#include "ofMain.h"
#include <string>

class ofApp : public ofBaseApp {
public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void mousePressed(int x, int y, int button);

    void fetchRates();
    void calculateConversion();
    void handleInput(int key);
    void setCurrencyFromKey(int key);

    // Core application states
    string baseCurrency = "USD";
    string targetCurrency = "EUR";
    string amountInput = "100";
    double convertedResult = 0.0;

    ofJson exchangeRates;
    bool apiSuccess = false;
    bool errorState = false;
    string errorMessage = "";

    // 0 = amount entry, 1 = base currency, 2 = target currency
    int activeSelectionMode = 0;

    // Makes the initial 100 disappear when the user starts typing
    bool clearAmountOnNextInput = true;

    string apiKey = "fca_live_XPrFNsGzm6ZkDk8UhLJs8dZnfjYpEBWzkaabyW9B"; //

    // Clickable UI areas
    ofRectangle basePanel;
    ofRectangle targetPanel;
    ofRectangle amountBox;

    ofTrueTypeFont customFont;
    ofTrueTypeFont bodyFont;

    // Optional image in bin/data/background.jpg; the color is used if absent.
    ofImage backgroundImage;
    bool hasBackgroundImage = true;
};
