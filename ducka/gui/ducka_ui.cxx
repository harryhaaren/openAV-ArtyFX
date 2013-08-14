

#include <string>
#include <iostream>

#include <FL/x.H>

// include the URI and global data of this plugin
#include "../dsp/ducka.hxx"

// this is our custom widget include
#include "ducka_widget.h"

// core spec include
#include "lv2/lv2plug.in/ns/lv2core/lv2.h"

// GUI
#include "lv2/lv2plug.in/ns/extensions/ui/ui.h"

#include "../../avtk/avtk.h"

using namespace std;

typedef struct {
    DuckaUI* widget;
    
    float sidechainAmp;
    
    LV2UI_Write_Function write_function;
    LV2UI_Controller controller;
} DuckaGUI;

static LV2UI_Handle instantiate(const struct _LV2UI_Descriptor * descriptor,
                const char * plugin_uri,
                const char * bundle_path,
                LV2UI_Write_Function write_function,
                LV2UI_Controller controller,
                LV2UI_Widget * widget,
                const LV2_Feature * const * features)
{
    if (strcmp(plugin_uri, DUCKA_URI) != 0) {
        fprintf(stderr, "DUCKA_URI error: this GUI does not support plugin with URI %s\n", plugin_uri);
        return NULL;
    }
    
    DuckaGUI* self = (DuckaGUI*)malloc(sizeof(DuckaGUI));
    if (self == NULL) return NULL;
    
    self->controller     = controller;
    self->write_function = write_function;
    
    //cout << "Controller " << controller << "   write_function " << write_function << endl;
    
    void* parentXwindow = 0;
    LV2UI_Resize* resize = NULL;
    
    for (int i = 0; features[i]; ++i) {
      //cout << "feature " << features[i]->URI << endl;
      if (!strcmp(features[i]->URI, LV2_UI__parent)) {
        parentXwindow = features[i]->data;
        //cout << "got parent UI feature: X11 id = " << (Window)parentXwindow << endl;
      } else if (!strcmp(features[i]->URI, LV2_UI__resize)) {
        resize = (LV2UI_Resize*)features[i]->data;
      }
    }
    
    // in case FLTK hasn't opened it yet
    fl_open_display();
    
    //cout << "Creating UI!" << endl;
    self->widget = new DuckaUI();
    
    self->widget->window->border(0);
    
    
    // write functions into the widget
    self->widget->controller = controller;
    self->widget->write_function = write_function;
    
    // set host to change size of the window
    if (resize)
    {
      resize->ui_resize(resize->handle, self->widget->getWidth(), self->widget->getHeight());
    }
    else
    {
      cout << "Ducka: Warning, host doesn't support resize extension.\n\
      Please ask the developers of the host to support this extension. "<< endl;
    }
    
    //cout << "window ID = " << self->widget->window << endl;
    fl_embed( self->widget->window, (Window)parentXwindow );
    
    
    return (LV2UI_Handle)self;
}



static void cleanup(LV2UI_Handle ui) {
    //printf("cleanup()\n");
    DuckaGUI *pluginGui = (DuckaGUI *) ui;
    delete pluginGui->widget;
    free( pluginGui);
}

static void port_event(LV2UI_Handle ui,
               uint32_t port_index,
               uint32_t buffer_size,
               uint32_t format,
               const void * buffer)
{
    DuckaGUI *self = (DuckaGUI *) ui;
    
    /*
    Fl::lock();
    ui->filterLowpass->value( argv[0]->f );
    Fl::unlock();
    Fl::awake();
    */
    
    if ( format == 0 )
    {
      float value =  *(float *)buffer;
      switch ( port_index )
      {
        case DUCKA_THRESHOLD:
            {
              self->widget->graph->threshold( value );
              self->widget->threshold->value( value );
            }
            break;
        case DUCKA_REDUCTION:
            {
              self->widget->graph->reduce( value );
              self->widget->drop->value( value );
            }
            break;
        case DUCKA_RELEASE_TIME:
            {
              self->widget->graph->release( value );
              self->widget->time->value( value );
            }
            break;
        case DUCKA_SIDECHAIN_AMP:
            {
              /// only update when value changes?
              if ( self->sidechainAmp > value + 0.1 ||
                   self->sidechainAmp < value - 0.1)
              {
                self->widget->graph->sidechain( value );
                self->sidechainAmp = value;
              }
            }
            break;
      }
    }
    
    return;
}


static int
idle(LV2UI_Handle handle)
{
	DuckaGUI* self = (DuckaGUI*)handle;
  
  self->widget->idle();
  
	return 0;
}

static const LV2UI_Idle_Interface idle_iface = { idle };

static const void*
extension_data(const char* uri)
{
  //cout << "UI extension data!" << endl;
	if (!strcmp(uri, LV2_UI__idleInterface)) {
    //cout << "giving host idle interface!" << endl;
		return &idle_iface;
	}
	return NULL;
}

static LV2UI_Descriptor descriptors[] = {
    {DUCKA_UI_URI, instantiate, cleanup, port_event, extension_data}
};

const LV2UI_Descriptor * lv2ui_descriptor(uint32_t index) {
    //printf("lv2ui_descriptor(%u) called\n", (unsigned int)index); 
    if (index >= sizeof(descriptors) / sizeof(descriptors[0])) {
        return NULL;
    }
    return descriptors + index;
}
