#ifndef GUI_H
#define GUI_H

#include "Lato-Regular.h"

// #include "parameter.h"
#include "IParameterView.h"

#include <visage/app.h>
#include <visage/widgets.h>

#include <string>
#include <sstream>

using namespace visage::dimension;

// Some GUI tests
VISAGE_THEME_COLOR(TextColor, 0xffffffff);
VISAGE_THEME_COLOR(ShapeColor, 0xff237757);
VISAGE_THEME_COLOR(LabelColor, 0x44212529);
VISAGE_THEME_COLOR(BackgroundColor, 0xff414549);
VISAGE_THEME_COLOR(DarkBackgroundColor, 0xff212529);
VISAGE_THEME_COLOR(OverlayShadowColor, 0xbb000000);
VISAGE_THEME_COLOR(ShadowColor, 0x88000000);

static constexpr float kHalfPi = 3.14159265358979323846f * 0.5f;

// Currently acts as controller (moving knob) and view (parameter moved through host)
class Knob : public visage::Frame,
             public IParameterView<double> {
public:
  Knob(float w, float xpos, float ypos) : radius{w}, x{xpos}, y{ypos} {}

  void draw(visage::Canvas& canvas) override {
    canvas.setColor(LabelColor);
    canvas.roundedRectangle(0, 0, this->width(), this->height(), 5);
    bool rounded = false;

    canvas.setColor(ShapeColor);
    float thickness = radius/5.0f;
    float center_radians = kHalfPi * 3 + kHalfPi*0.33f + 3.33 * kHalfPi * rotation;
    float radians = 2*kHalfPi-kHalfPi*0.01f;
    canvas.arc(radius*0.1, radius*0.1, radius, thickness, center_radians, radians, rounded);

    canvas.setColor(BackgroundColor);
    float center = kHalfPi * ( 1.0f + 0.33f + 1.66666666);
    float outerradians = 1.666666666 * kHalfPi;
    canvas.arc(0, 0, radius * 1.2f, thickness/10.0f, center, outerradians, rounded);

    visage::Color color = 0xff237757;
    color.setHdr(1.0f + rotation);
    canvas.setColor(color);
    center = kHalfPi * ( 1.0f + 0.33f + 1.66666666 * rotation);
    outerradians = 1.666666666 * kHalfPi * rotation;
    canvas.arc(0, 0, radius * 1.2f, thickness/10.0f, center, outerradians, rounded);

    canvas.setColor(TextColor);
    const visage::Font font(height()*0.07f, Lato_Regular_ttf, Lato_Regular_ttf_len);
    std::ostringstream oss;
    oss << std::setprecision(2) << std::setw(4) << rotation;
    canvas.text(oss.str(), font, visage::Font::kCenter, 0, radius * (1 + 0.2 + 0.1), this->width() * 1.0f, this->height() * 0.1f);
  }

  void mouseDown(const visage::MouseEvent& e) override {
    redraw();

    if (!changed)
      return;

    if (e.repeatClickCount() >= 2){
      setRotation(0.5);
      changed = true;

      return;
    }

    last_drag = e.position.y;
    redraw();
  }

  void mouseUp(const visage::MouseEvent& e) override {
    redraw();
  }

  void mouseDrag(const visage::MouseEvent& e) override {
    redraw();

    float delta = -(e.position.y - last_drag) * 1.0f / height();
    last_drag = e.position.y;
    setRotation(rotation + delta);

    changed = true;
    redraw();
  }

  void mouseEnter(const visage::MouseEvent& e) override {
    redraw();
  }

  void mouseExit(const visage::MouseEvent& e) override {
    redraw();
  }

  bool mouseWheel(const visage::MouseEvent& e) override {
    redraw();

    float delta = e.precise_wheel_delta_y * 1e-1f;
    setRotation(rotation + delta);

    changed = true;
    redraw();

    return true;
  }

  // put isChanged mechanism in controller interface?
  // Otherwise writing through controller always needs a corresponding line
  // to call unsetChanged. Ages badly
  bool isChanged() const { return changed; }
  void unsetChanged() { changed = false; }

  float getRotation() { return rotation; } const
  void setRotation(float newval) {
    // Better get limits from parameter?
    rotation = std::clamp(newval, 0.0f, 1.0f);
  }

  void setView(const double& v) override {
    setRotation(v);
    redraw();
  }

  const double getDisplayValue() const override {
    return rotation;
  }
private:
  bool changed = false;
  int last_drag = 0;
  float rotation = 0.5;
  float radius = 100;
  float x = 0;
  float y = 0;
};

class MyGui : public visage::ApplicationWindow {
public:
  MyGui() {
    this->setWindowDimensions(80_vmin, 60_vmin);

    float circle_radius = this->height() * 0.4f;
    float knob_width = circle_radius * 2.0f * 0.7; // 0.7 because why?!?!
    float knob_height = circle_radius * 1.15f * 2.0f * 0.7;
    knob_ = std::make_unique<Knob>(circle_radius, 0, 0);
    this->addChild(knob_.get());
    knob_->setNativeBounds(10, 10, knob_width, knob_height);

    this->setTitle("Visage Basic Example");
  }
  virtual ~MyGui() {}

  void draw(visage::Canvas& canvas) override {
      canvas.setColor(DarkBackgroundColor);
      canvas.fill(0, 0, this->width(), this->height());
  }

  int runExample() {
    this->show(80_vmin, 60_vmin);
    this->runEventLoop();
    return 0;
  }

  Knob* getKnob() { return knob_.get(); }
private:
  std::unique_ptr<Knob> knob_;
};

#endif //GUI_H
