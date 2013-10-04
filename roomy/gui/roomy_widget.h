// generated by Fast Light User Interface Designer (fluid) version 1.0300

#ifndef roomy_widget_h
#define roomy_widget_h
#include <FL/Fl.H>
#include <FL/Fl_Double_Window.H>
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"
using namespace std;
#include "../dsp/roomy.hxx"
#include "avtk.h"
#include <stdio.h>
#include "header.c"
#include <iostream>

class RoomyUI {
public:
  void update_button(int button);
  RoomyUI();
  Fl_Double_Window *window;
  Avtk::Image *headerImage;
private:
  void cb_headerImage_i(Avtk::Image*, void*);
  static void cb_headerImage(Avtk::Image*, void*);
public:
  Avtk::Reverb *graph;
private:
  void cb_graph_i(Avtk::Reverb*, void*);
  static void cb_graph(Avtk::Reverb*, void*);
public:
  Avtk::Dial *time;
private:
  void cb_time_i(Avtk::Dial*, void*);
  static void cb_time(Avtk::Dial*, void*);
public:
  Avtk::Dial *damping;
private:
  void cb_damping_i(Avtk::Dial*, void*);
  static void cb_damping(Avtk::Dial*, void*);
public:
  Avtk::Dial *dryWet;
private:
  void cb_dryWet_i(Avtk::Dial*, void*);
  static void cb_dryWet(Avtk::Dial*, void*);
public:
  LV2UI_Write_Function write_function; 
  LV2UI_Controller controller; 
  void idle();
  int getWidth();
  int getHeight();
private:
  float gain; 
  float cutoff; 
  float Q; 
public:
  void writePort(int port, float& value);
};
#endif