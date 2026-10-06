#include "ofApp.h"
#include <algorithm>
#include <ofxGuiGroup.h>

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetWindowShape(1200, 950);
    ofSetBackgroundColor(18, 43, 67);
	hasBackgroundImage = backgroundImage.load("background.jpg.jpg");
    ofSetWindowTitle("Interactive Currency Converter Pro");

    customFont.load(OF_TTF_SANS, 44, true, true, false, 0.3f, 96);
    bodyFont.load(OF_TTF_SANS, 22, true, true, false, 0.3f, 96);

	//font.load("C:/Users/eghe-/Downloads/debrosee-font/Debrosee-ALPnL.tff", 22, true, true, false, 0.3f, 96);

    // Match the rectangles used in draw().
    float panelWidth = 900;
    float centerX = (ofGetWidth() - panelWidth) / 2.0f;

    basePanel.set(centerX, 240, panelWidth, 120);
    targetPanel.set(centerX, 390, panelWidth, 120);
    amountBox.set(centerX, 580, panelWidth, 120);

    fetchRates();
}

//--------------------------------------------------------------
void ofApp::update() {
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(255);
	ofDrawBitmapString(
		"Rates By Exchange Rate API - https://www.exchangerate-api.com",
		150, 930);

    if (hasBackgroundImage) {
        // Fill the window while keeping the image's proportions; crop edges as needed.
        float scale = std::max(ofGetWidth() / backgroundImage.getWidth(),
                               ofGetHeight() / backgroundImage.getHeight());
        float width = backgroundImage.getWidth() * scale;
        float height = backgroundImage.getHeight() * scale;
        backgroundImage.draw((ofGetWidth() - width) / 2.0f,
                             (ofGetHeight() - height) / 2.0f, width, height);

        // Darken the image so the controls remain easy to read.
        ofSetColor(9, 22, 35, 100);
        ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
        ofSetColor(255);
    }

    // MAIN HEADER
    ofSetColor(15, 18, 24);
    ofDrawRectangle(0, 0, ofGetWidth(), 150);

    string headerText = "CURRENCY CONVERTER";
    float textWidth = customFont.stringWidth(headerText);
    float textHeight = customFont.stringHeight(headerText);
    float headerX = (ofGetWidth() - textWidth) / 2.0f;
    float headerY = (150 + textHeight) / 2.0f;

    ofSetColor(0, 0, 0, 200);
    customFont.drawString(headerText, headerX + 2, headerY + 2);

    ofSetColor(255, 215, 0);
    customFont.drawString(headerText, headerX, headerY);

    // CONTROLS STATUS BAR
    ofSetColor(32, 38, 48);
    ofDrawRectangle(0, 150, ofGetWidth(), 50);

    ofSetColor(156, 163, 175);
    string controlsStr =
        "Click Base/Target/Amount | Currency keys: U=USD E=EUR G=GBP R=RON P=PLN J=JPY C=CNY Q=QAR N=NGN S=SEK T=TRY";
    float controlsX = 55;
    ofDrawBitmapString(controlsStr, controlsX, 180);

    float panelWidth = 900;
    float panelHeight = 120;
    float centerX = (ofGetWidth() - panelWidth) / 2.0f;

    // Keep hit boxes synchronized if the window changes size.
    basePanel.set(centerX, 240, panelWidth, panelHeight);
    targetPanel.set(centerX, 390, panelWidth, panelHeight);
    amountBox.set(centerX, 580, panelWidth, panelHeight);

    // BASE CURRENCY PANEL
    if (activeSelectionMode == 1) {
        ofSetColor(14, 116, 144);
    } else {
        ofSetColor(38, 45, 56);
    }
    ofDrawRectangle(basePanel);

    ofSetColor(255);
    string baseText = "BASE CURRENCY:   " + baseCurrency;
    float baseTextHeight = bodyFont.stringHeight(baseText);
    bodyFont.drawString(
        baseText,
        centerX + 40,
        240 + (panelHeight + baseTextHeight) / 2.0f - 4
    );

    // TARGET CURRENCY PANEL
    if (activeSelectionMode == 2) {
        ofSetColor(190, 24, 74);
    } else {
        ofSetColor(38, 45, 56);
    }
    ofDrawRectangle(targetPanel);

    ofSetColor(255);
    string targetText = "TARGET CURRENCY: " + targetCurrency;
    float targetTextHeight = bodyFont.stringHeight(targetText);
    bodyFont.drawString(
        targetText,
        centerX + 40,
        390 + (panelHeight + targetTextHeight) / 2.0f - 4
    );

    // AMOUNT ENTRY
    ofSetColor(156, 163, 175);
    bodyFont.drawString("Amount To Convert:", centerX + 5, 560);

    if (activeSelectionMode == 0) {
        ofSetColor(50, 61, 76);
    } else {
        ofSetColor(15, 18, 24);
    }
    ofDrawRectangle(amountBox);

    // Active amount-field border
    if (activeSelectionMode == 0) {
        ofNoFill();
        ofSetColor(16, 185, 129);
        ofSetLineWidth(3);
        ofDrawRectangle(amountBox);
        ofFill();
    }

    ofSetColor(255);
    string inputText = amountInput;
    if (activeSelectionMode == 0) {
        inputText += " _";
    }

    float inputTextHeight = bodyFont.stringHeight(inputText);
    bodyFont.drawString(
        inputText,
        centerX + 40,
        580 + (panelHeight + inputTextHeight) / 2.0f - 4
    );

    // SEPARATOR
    ofSetColor(55, 65, 81);
    ofSetLineWidth(4);
    ofDrawLine(centerX, 735, centerX + panelWidth, 735);

    // RESULT PANEL
    float resultHeight = 140;

    if (errorState) {
        ofSetColor(185, 28, 28);
        ofDrawRectangle(centerX, 760, panelWidth, resultHeight);

        ofSetColor(255);
        string errText = "ERROR: " + errorMessage;
        float errHeight = bodyFont.stringHeight(errText);
        bodyFont.drawString(
            errText,
            centerX + 40,
            760 + (resultHeight + errHeight) / 2.0f
        );
    }
    else if (apiSuccess && !amountInput.empty()) {
        ofSetColor(16, 185, 129);
        ofDrawRectangle(centerX, 760, panelWidth, resultHeight);

        string finalDisplay =
            amountInput + " " + baseCurrency +
            " = " + ofToString(convertedResult, 2) +
            " " + targetCurrency;

        float finalDisplayHeight = bodyFont.stringHeight(finalDisplay);
        float finalDisplayX = centerX + 40;
        float finalDisplayY =
            760 + (resultHeight + finalDisplayHeight) / 2.0f - 2;

        ofSetColor(15, 18, 24, 50);
        bodyFont.drawString(
            finalDisplay,
            finalDisplayX + 1,
            finalDisplayY + 1
        );

        ofSetColor(15, 18, 24);
        bodyFont.drawString(
            finalDisplay,
            finalDisplayX,
            finalDisplayY
        );
    }
    else {
        ofSetColor(38, 45, 56);
        ofDrawRectangle(centerX, 760, panelWidth, resultHeight);

        ofSetColor(255);
        string syncText =
            apiSuccess
            ? "Enter an amount to convert."
            : "Syncing Live Exchange Data...";

        float syncHeight = bodyFont.stringHeight(syncText);
        bodyFont.drawString(
            syncText,
            centerX + 40,
            760 + (resultHeight + syncHeight) / 2.0f
        );
    }
}

//--------------------------------------------------------------
void ofApp::fetchRates() {
	ofHttpResponse response = ofLoadURL("https://open.er-api.com/v6/latest/GBP");

	if (response.status != 200) {
		apiSuccess = false;
		errorState = true;
		errorMessage = "Could not fetch exchange rates.";
		return;
	}

	try {
		ofJson data = ofJson::parse(response.data.getText());

		if (data.value("result", "") != "success" || !data.contains("rates") || !data["rates"].is_object()) {
			throw std::runtime_error("Invalid rates response");
		}

		exchangeRates = data["rates"];
		apiSuccess = true;
		errorState = false;
		errorMessage = "";
		calculateConversion();
	} catch (const std::exception &) {
		apiSuccess = false;
		errorState = true;
		errorMessage = "Could not read exchange rates.";
	}
}

//--------------------------------------------------------------
void ofApp::calculateConversion() {
    if (!apiSuccess || exchangeRates.empty()) {
        return;
    }

    if (amountInput.empty() || amountInput == ".") {
        convertedResult = 0.0;
        errorState = false;
        return;
    }

    double rawAmount = ofToDouble(amountInput);

    if (exchangeRates.contains(baseCurrency) &&
        exchangeRates.contains(targetCurrency)) {

        double baseRate =
            exchangeRates[baseCurrency].get<double>();

        double targetRate =
            exchangeRates[targetCurrency].get<double>();

        convertedResult =
            rawAmount * (targetRate / baseRate);

        errorState = false;
        errorMessage = "";
    }
    else {
        errorState = true;
        errorMessage =
            "Currency mapping failed for " +
            baseCurrency + " or " + targetCurrency + ".";
    }
}

//--------------------------------------------------------------
void ofApp::setCurrencyFromKey(int key) {
    string selected = "";

    if (key == 'U' || key == 'u') selected = "USD";
    else if (key == 'E' || key == 'e') selected = "EUR";
    else if (key == 'G' || key == 'g') selected = "GBP";
    else if (key == 'R' || key == 'r') selected = "RON";
    else if (key == 'P' || key == 'p') selected = "PLN";
    else if (key == 'J' || key == 'j') selected = "JPY";
    else if (key == 'C' || key == 'c') selected = "CNY";
    else if (key == 'Q' || key == 'q') selected = "QAR";
	else if (key == 'N' || key == 'n') selected = "NGN";
	else if (key == 'S' || key == 's') selected = "SEK";
	else if (key == 'T' || key == 't') selected = "TRY";

    if (selected.empty()) {
        return;
    }

    if (activeSelectionMode == 1) {
        baseCurrency = selected;
    }
    else if (activeSelectionMode == 2) {
        targetCurrency = selected;
    }

    calculateConversion();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    bool modifierHeld =
        ofGetKeyPressed(OF_KEY_CONTROL) ||
        ofGetKeyPressed(OF_KEY_ALT);

    // Existing keyboard shortcuts still work.
    if (modifierHeld && key == '1') {
        activeSelectionMode = 1;
        return;
    }

    if (modifierHeld && key == '2') {
        activeSelectionMode = 2;
        return;
    }

    // Clickable panels also set these modes.
    if (activeSelectionMode == 1 ||
        activeSelectionMode == 2) {

        if ((key >= 'A' && key <= 'Z') ||
            (key >= 'a' && key <= 'z')) {
            setCurrencyFromKey(key);
            return;
        }
    }

    // Numbers, decimal point and backspace edit the amount.
    if ((key >= '0' && key <= '9') ||
        key == '.' ||
        key == OF_KEY_BACKSPACE) {

        activeSelectionMode = 0;
        handleInput(key);
        return;
    }

    if (key == OF_KEY_RETURN) {
        calculateConversion();
    }
}

//--------------------------------------------------------------
void ofApp::handleInput(int key) {
    // First number/decimal typed replaces the initial "100".
    if (clearAmountOnNextInput &&
        ((key >= '0' && key <= '9') || key == '.')) {

        amountInput.clear();
        clearAmountOnNextInput = false;
    }

    if (key == OF_KEY_BACKSPACE) {
        clearAmountOnNextInput = false;

        if (!amountInput.empty()) {
            amountInput.pop_back();
        }
    }
    else if (key == '.') {
        if (amountInput.find('.') == string::npos) {
            if (amountInput.empty()) {
                amountInput = "0";
            }

            amountInput += ".";
        }
    }
    else if (key >= '0' && key <= '9') {
        // Prevent excessively long numeric input.
        if (amountInput.length() < 15) {
            amountInput += static_cast<char>(key);
        }
    }

    calculateConversion();
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
    if (basePanel.inside(x, y)) {
        activeSelectionMode = 1;
        return;
    }

    if (targetPanel.inside(x, y)) {
        activeSelectionMode = 2;
        return;
    }

    if (amountBox.inside(x, y)) {
        activeSelectionMode = 0;
        clearAmountOnNextInput = true;
        return;
    }
}
