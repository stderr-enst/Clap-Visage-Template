#ifndef GUI_H
#define GUI_H

#include "parameter.h"

#include <visage/app.h>
#include <visage/widgets.h>

using namespace visage::dimension;

// Some GUI tests
VISAGE_THEME_COLOR(TextColor, 0xffffffff);
VISAGE_THEME_COLOR(ShapeColor, 0xff237757);
VISAGE_THEME_COLOR(LabelColor, 0x44212529);
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
    canvas.setColor(ShapeColor);
    float width = 2.0f * radius;
    float thickness = width*0.1f + 1.0f;
    bool rounded = false;
    float center_radians = kHalfPi * 3 + kHalfPi*0.33f + 3.33 * kHalfPi * rotation;
    float radians = 2*kHalfPi-kHalfPi*0.01f;
    canvas.arc(0, 0, 100, 20, center_radians, radians, rounded);
  }

  void mouseDrag(const visage::MouseEvent& e) override {
    float delta = -e.position.y * 0.1f / height(); // sensitivity
    setRotation(rotation + delta);

    changed = true;

    redraw();
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
  float rotation = 0.5;
  float radius = 100;
  float x = 0;
  float y = 0;
};

class MyGui : public visage::ApplicationWindow {
public:
  MyGui() {
    this->setWindowDimensions(80_vmin, 60_vmin);

    float circle_radius = this->height() * 0.1f;
    float x = this->width() * 0.5f - circle_radius;
    float y = this->height() * 0.5f - circle_radius;
    knob_ = std::make_unique<Knob>(circle_radius, 0, 0);
    knob_->setBounds(70, 70, 300, 300);
    this->addChild(knob_.get());

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
